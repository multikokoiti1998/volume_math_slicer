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
#include <vtkImageMathematics.h>
#include <vtkImageCast.h>
#include <iostream>
#include <vtkPointData.h>

// ITK
#include <itkImage.h>
#include <itkVTKImageToImageFilter.h>
#include <itkIdentityTransform.h>
#include <itkMeanSquaresImageToImageMetricv4.h>
#include <itkCorrelationImageToImageMetricv4.h>
#include <itkLinearInterpolateImageFunction.h>
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


vtkSmartPointer<vtkImageData> vtkSlicervolume_mathLogic::CastToFloat(vtkImageData* input)
{
	if (!input) return nullptr;
//todo
	if (input->GetScalarType() == VTK_FLOAT)
	{
		return input; 
	}

	vtkNew<vtkImageCast> cast;
	cast->SetInputData(input);
	cast->SetOutputScalarTypeToFloat();
	cast->ClampOverflowOn();
	cast->Update();

	vtkSmartPointer<vtkImageData> output =
		vtkSmartPointer<vtkImageData>::New();
	output->DeepCopy(cast->GetOutput());

	return output;
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

	else 
	{
		vtkSmartPointer<vtkImageData> in1 = CastToFloat(imgA);
		vtkSmartPointer<vtkImageData> in2 = imgB ? CastToFloat(imgB) : nullptr;

		vtkDataArray* s = in1->GetPointData()->GetScalars();
		vtkIdType n = s->GetNumberOfTuples() * s->GetNumberOfComponents();
		vtkIdType bad = 0;
		for (vtkIdType i = 0; i < n; i++) {
			double v = s->GetComponent(i, 0);
			if (!std::isfinite(v)) { bad++; if (bad < 10) std::cout << "bad v=" << v << "\n"; }
		}
		std::cout << "bad count=" << bad << "\n";

		if (op == OP_SQRT) {
			vtkNew<vtkImageThreshold> clamp;
			clamp->SetInputData(in1);
			clamp->ThresholdByLower(0.0);
			clamp->SetInValue(0.0);
			clamp->ReplaceOutOff();
			clamp->Update();
			in1 = clamp->GetOutput();
		}
		vtkNew<vtkImageMathematics> math;
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

bool vtkSlicervolume_mathLogic::ComputeMetric(
	vtkMRMLScalarVolumeNode* a,
	vtkMRMLScalarVolumeNode* b,
	VolumeOp metric,
	double& outValue)
{
	outValue = 0.0;
	if (!a || !b) return false;

	vtkImageData* aVtk0 = a->GetImageData();
	vtkImageData* bVtk0 = b->GetImageData();
	if (!aVtk0 || !bVtk0) return false;

	int da[3], db[3];
	aVtk0->GetDimensions(da);
	bVtk0->GetDimensions(db);
	if (da[0] != db[0] || da[1] != db[1] || da[2] != db[2]) return false;

	vtkSmartPointer<vtkImageData> aVtk = CastToFloat(aVtk0);
	vtkSmartPointer<vtkImageData> bVtk = CastToFloat(bVtk0);
	if (!aVtk || !bVtk) return false;

	constexpr unsigned int Dim = 3;
	using PixelType = float;
	using ImageType = itk::Image<PixelType, Dim>;
	using V2I = itk::VTKImageToImageFilter<ImageType>;

	auto convA = V2I::New();
	auto convB = V2I::New();
	convA->SetInput(aVtk);
	convB->SetInput(bVtk);
	convA->Update();
	convB->Update();

	ImageType::Pointer fixed = convA->GetOutput();
	ImageType::Pointer moving = convB->GetOutput();
	if (!fixed || !moving) return false;

	using TransformType = itk::IdentityTransform<double, Dim>;
	auto transform = TransformType::New();
	transform->SetIdentity();

	try {
		if (metric == OP_MSE) {
			using MetricType = itk::MeanSquaresImageToImageMetricv4<ImageType, ImageType>;
			auto m = MetricType::New();
			m->SetFixedImage(fixed);
			m->SetMovingImage(moving);
			m->SetFixedTransform(transform);
			m->SetMovingTransform(transform);
			m->Initialize();
			outValue = static_cast<double>(m->GetValue());
			return true;
		}

		if (metric == OP_NCC) {
			using MetricType = itk::CorrelationImageToImageMetricv4<ImageType, ImageType>;
			auto m = MetricType::New();
			m->SetFixedImage(fixed);
			m->SetMovingImage(moving);
			m->SetFixedTransform(transform);
			m->SetMovingTransform(transform);
			m->Initialize();
			outValue = static_cast<double>(m->GetValue());
			return true;
		}
	}
	catch (const itk::ExceptionObject& e) {
		vtkErrorMacro(<< "ITK metric failed: " << e.what());
		return false;
	}

	return false;
}