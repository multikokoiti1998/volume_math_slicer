/*==============================================================================

  Program: 3D Slicer

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

// volume_math Logic includes
#include "vtkSlicervolume_mathLogic.h"
#include "vtkImageLogic.h"
// MRML includes
#include <vtkMRMLScene.h>

// VTK includes
#include <vtkIntArray.h>
#include <vtkNew.h>
#include <vtkObjectFactory.h>
#include <vtkImageThreshold.h>

// ITK includes
#include <itkImage.h>
#include <itkVTKImageToImageFilter.h>
#include <itkImageRegionConstIterator.h>
#include <cmath>

// STD includes
#include <cassert>

//----------------------------------------------------------------------------
vtkStandardNewMacro(vtkSlicervolume_mathLogic);

//----------------------------------------------------------------------------
vtkSlicervolume_mathLogic::vtkSlicervolume_mathLogic()
{
}

//----------------------------------------------------------------------------
vtkSlicervolume_mathLogic::~vtkSlicervolume_mathLogic()
{
}

//----------------------------------------------------------------------------
void vtkSlicervolume_mathLogic::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os, indent);
}

// ITK•ÏŠ·—p
using ImageType = itk::Image<float, 3>;

static ImageType::Pointer VtkToItkFloat(vtkImageData* vtkImg)
{
	using ConnectorType = itk::VTKImageToImageFilter<ImageType>;
	auto connector = ConnectorType::New();
	connector->SetInput(vtkImg);
	connector->Update();
	return connector->GetOutput();
}

struct SimilarityResult {
	double mse;
	double ncc;   // Pearson correlation
};

static SimilarityResult ComputeMSEandNCC(ImageType* A, ImageType* B)
{
	SimilarityResult r{ 0.0, 0.0 };

	itk::ImageRegionConstIterator<ImageType> itA(A, A->GetLargestPossibleRegion());
	itk::ImageRegionConstIterator<ImageType> itB(B, B->GetLargestPossibleRegion());

	double sumA = 0.0, sumB = 0.0;
	double sumAA = 0.0, sumBB = 0.0, sumAB = 0.0;
	double sumSqDiff = 0.0;
	long long N = 0;

	for (itA.GoToBegin(), itB.GoToBegin(); !itA.IsAtEnd(); ++itA, ++itB) {
		const double a = static_cast<double>(itA.Get());
		const double b = static_cast<double>(itB.Get());
		const double d = a - b;

		sumSqDiff += d * d;

		sumA += a; sumB += b;
		sumAA += a * a;
		sumBB += b * b;
		sumAB += a * b;

		++N;
	}

	if (N == 0) return r;

	r.mse = sumSqDiff / static_cast<double>(N);

	// Pearson correlationiNCCj
	// cov = E[ab] - E[a]E[b]
	const double meanA = sumA / N;
	const double meanB = sumB / N;
	const double cov = (sumAB / N) - meanA * meanB;
	const double varA = (sumAA / N) - meanA * meanA;
	const double varB = (sumBB / N) - meanB * meanB;

	const double denom = std::sqrt(varA * varB);
	r.ncc = (denom > 0.0) ? (cov / denom) : 0.0;

	return r;
}

//---------------------------------------------------------------------------
void vtkSlicervolume_mathLogic::SetMRMLSceneInternal(vtkMRMLScene* newScene)
{
	vtkNew<vtkIntArray> events;
	events->InsertNextValue(vtkMRMLScene::NodeAddedEvent);
	events->InsertNextValue(vtkMRMLScene::NodeRemovedEvent);
	events->InsertNextValue(vtkMRMLScene::EndBatchProcessEvent);
	this->SetAndObserveMRMLSceneEventsInternal(newScene, events.GetPointer());
}

//-----------------------------------------------------------------------------
void vtkSlicervolume_mathLogic::RegisterNodes()
{
	assert(this->GetMRMLScene() != 0);
}

//---------------------------------------------------------------------------
void vtkSlicervolume_mathLogic::UpdateFromMRMLScene()
{
	assert(this->GetMRMLScene() != 0);
}

//---------------------------------------------------------------------------
void vtkSlicervolume_mathLogic
::OnMRMLSceneNodeAdded(vtkMRMLNode* vtkNotUsed(node))
{
}

//---------------------------------------------------------------------------
void vtkSlicervolume_mathLogic
::OnMRMLSceneNodeRemoved(vtkMRMLNode* vtkNotUsed(node))
{
}

bool vtkSlicervolume_mathLogic::ExecuteOperation(
	vtkMRMLScalarVolumeNode* a,
	vtkMRMLScalarVolumeNode* out,
	VolumeOp op,
	vtkMRMLScalarVolumeNode* b)
{
	if (!a || !out) return false;
	vtkImageData* imgA = a->GetImageData();
	if (!imgA) return false;

	vtkImageData* imgB = (b) ? b->GetImageData() : nullptr;
	if (b && !imgB) return false;

	if (imgB) {
		int dimsA[3], dimsB[3];
		imgA->GetDimensions(dimsA);
		imgB->GetDimensions(dimsB);
		if (dimsA[0] != dimsB[0] || dimsA[1] != dimsB[1] || dimsA[2] != dimsB[2]) {
			vtkWarningMacro("Dimension mismatch.");
			return false;
		}
	}

	vtkSmartPointer<vtkImageAlgorithm> filter;

	if (op >= OP_LOGIC_START) {
		vtkNew<vtkImageLogic> logic;
		logic->SetInput1Data(imgA);
		if (imgB) logic->SetInput2Data(imgB);

		switch (op) {
		case OP_AND: logic->SetOperationToAnd(); break;
		case OP_OR:  logic->SetOperationToOr();  break;
		case OP_XOR: logic->SetOperationToXor(); break;
		case OP_NOT: logic->SetOperationToNot(); break;
		default: return false;
		}
		filter = logic;
	}

	else if (op < OP_LOGIC_START)
	{
		vtkNew<vtkImageMathematics> math;

		vtkSmartPointer<vtkImageData> in1;
		vtkSmartPointer<vtkImageData> in2;

		if (op == OP_SQRT) {
			vtkNew<vtkImageThreshold> clamp;
			clamp->SetInputData(imgA);
			clamp->ThresholdByLower(0.0);
			clamp->SetInValue(0.0);
			clamp->ReplaceOutOff();
			clamp->Update();
			in1 = clamp->GetOutput();
		}
		else {
			in1 = imgA;
		}

		in2 = imgB;

		if (op == OP_DIV) {
			vtkNew<vtkImageCast> castA;
			castA->SetInputData(in1);
			castA->SetOutputScalarTypeToFloat();
			castA->ClampOverflowOn();
			castA->Update();
			in1 = castA->GetOutput();

			if (in2) {
				vtkNew<vtkImageCast> castB;
				castB->SetInputData(in2);
				castB->SetOutputScalarTypeToFloat();
				castB->ClampOverflowOn();
				castB->Update();
				in2 = castB->GetOutput();
			}
		}

		math->SetInput1Data(in1);
		if (in2) math->SetInput2Data(in2);

		switch (op) {
		case OP_ADD:  math->SetOperationToAdd(); break;
		case OP_SUB:  math->SetOperationToSubtract(); break;
		case OP_MUL:  math->SetOperationToMultiply(); break;
		case OP_DIV:  math->SetOperationToDivide(); break;
		case OP_MIN:  math->SetOperationToMin(); break;
		case OP_MAX:  math->SetOperationToMax(); break;
		case OP_SQR:  math->SetOperationToSquare(); break;
		case OP_SQRT: math->SetOperationToSquareRoot(); break;
		case OP_ABS:  math->SetOperationToAbsoluteValue(); break;
		default: return false;
		}

		filter = math;
	}
	else
	{
		std::cout << "Unsupported operation." << std::endl;
	}

	if (!filter) return false;
	filter->Update();

	vtkNew<vtkImageData> outImage;
	outImage->DeepCopy(filter->GetOutputDataObject(0));

	std::cout << "scalartype: "
		<< outImage->GetScalarTypeAsString()
		<< std::endl;

	double range[2];
	outImage->GetScalarRange(range);
	std::cout << "range: "
		<< range[0] << " - "
		<< range[1] << std::endl;

	out->SetAndObserveImageData(outImage);
	out->CopyOrientation(a);
	out->SetSpacing(a->GetSpacing());
	out->SetOrigin(a->GetOrigin());
	out->Modified();

	return true;
}