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

// MRML includes
#include <vtkMRMLScene.h>

// VTK includes
#include <vtkIntArray.h>
#include <vtkNew.h>
#include <vtkObjectFactory.h>
#include <vtkImageThreshold.h>


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

// 演算リスト
bool vtkSlicervolume_mathLogic::AddVolumes(vtkMRMLScalarVolumeNode* a, vtkMRMLScalarVolumeNode* b, vtkMRMLScalarVolumeNode* out)
{
	if (!a || !b || !out)
		return false;

	vtkImageData* imgA = a->GetImageData();
	vtkImageData* imgB = b->GetImageData();
	if (!imgA || !imgB)
		return false;

	int dimsA[3], dimsB[3];
	imgA->GetDimensions(dimsA);
	imgB->GetDimensions(dimsB);

	if (dimsA[0] != dimsB[0] || dimsA[1] != dimsB[1] || dimsA[2] != dimsB[2])
	{
		vtkWarningMacro("Dimension mismatch: resampling is needed before math.");
		return false;
	}

	vtkNew<vtkImageMathematics> math;
	math->SetInput1Data(imgA);
	math->SetInput2Data(imgB);
	math->SetOperationToAdd();
	math->Update();

	vtkImageData* result = math->GetOutput();
	if (!result)
		return false;

	vtkNew<vtkImageData> outImage;
	outImage->DeepCopy(result);

	out->SetAndObserveImageData(outImage);
	out->CopyOrientation(a);
	out->SetSpacing(a->GetSpacing());
	out->SetOrigin(a->GetOrigin());
	out->Modified();

	return true;
}

bool vtkSlicervolume_mathLogic::SubVolumes(vtkMRMLScalarVolumeNode* a, vtkMRMLScalarVolumeNode* b, vtkMRMLScalarVolumeNode* out)
{
	if (!a || !b || !out)
		return false;

	vtkImageData* imgA = a->GetImageData();
	vtkImageData* imgB = b->GetImageData();
	if (!imgA || !imgB)
		return false;

	int dimsA[3], dimsB[3];
	imgA->GetDimensions(dimsA);
	imgB->GetDimensions(dimsB);

	if (dimsA[0] != dimsB[0] || dimsA[1] != dimsB[1] || dimsA[2] != dimsB[2])
	{
		vtkWarningMacro("Dimension mismatch: resampling is needed before math.");
		return false;
	}

	vtkNew<vtkImageMathematics> math;
	math->SetInput1Data(imgA);
	math->SetInput2Data(imgB);
	math->SetOperationToSubtract();
	math->Update();

	vtkImageData* result = math->GetOutput();
	if (!result)
		return false;

	vtkNew<vtkImageData> outImage;
	outImage->DeepCopy(result);

	out->SetAndObserveImageData(outImage);
	out->CopyOrientation(a);
	out->SetSpacing(a->GetSpacing());
	out->SetOrigin(a->GetOrigin());
	out->Modified();

	return true;
}

bool vtkSlicervolume_mathLogic::MulVolumes(vtkMRMLScalarVolumeNode* a, vtkMRMLScalarVolumeNode* b, vtkMRMLScalarVolumeNode* out)
{
	if (!a || !b || !out)
		return false;

	vtkImageData* imgA = a->GetImageData();
	vtkImageData* imgB = b->GetImageData();
	if (!imgA || !imgB)
		return false;

	int dimsA[3], dimsB[3];
	imgA->GetDimensions(dimsA);
	imgB->GetDimensions(dimsB);

	if (dimsA[0] != dimsB[0] || dimsA[1] != dimsB[1] || dimsA[2] != dimsB[2])
	{
		vtkWarningMacro("Dimension mismatch: resampling is needed before math.");
		return false;
	}

	vtkNew<vtkImageMathematics> math;
	math->SetInput1Data(imgA);
	math->SetInput2Data(imgB);
	math->SetOperationToMultiply();
	math->Update();

	vtkImageData* result = math->GetOutput();
	if (!result)
		return false;

	vtkNew<vtkImageData> outImage;
	outImage->DeepCopy(result);

	out->SetAndObserveImageData(outImage);
	out->CopyOrientation(a);
	out->SetSpacing(a->GetSpacing());
	out->SetOrigin(a->GetOrigin());
	out->Modified();

	return true;
}

bool vtkSlicervolume_mathLogic::DivVolumes(vtkMRMLScalarVolumeNode* a, vtkMRMLScalarVolumeNode* b, vtkMRMLScalarVolumeNode* out)
{
	if (!a || !b || !out)
		return false;

	vtkImageData* imgA = a->GetImageData();
	vtkImageData* imgB = b->GetImageData();
	if (!imgA || !imgB)
		return false;

	int dimsA[3], dimsB[3];
	imgA->GetDimensions(dimsA);
	imgB->GetDimensions(dimsB);

	if (dimsA[0] != dimsB[0] || dimsA[1] != dimsB[1] || dimsA[2] != dimsB[2])
	{
		vtkWarningMacro("Dimension mismatch: resampling is needed before math.");
		return false;
	}

	vtkNew<vtkImageMathematics> math;
	math->SetInput1Data(imgA);
	math->SetInput2Data(imgB);
	math->SetOperationToDivide();
	math->Update();

	vtkImageData* result = math->GetOutput();
	if (!result)
		return false;

	vtkNew<vtkImageData> outImage;
	outImage->DeepCopy(result);

	out->SetAndObserveImageData(outImage);
	out->CopyOrientation(a);
	out->SetSpacing(a->GetSpacing());
	out->SetOrigin(a->GetOrigin());
	out->Modified();

	return true;
}

bool vtkSlicervolume_mathLogic::MinVolumes(vtkMRMLScalarVolumeNode* a, vtkMRMLScalarVolumeNode* b, vtkMRMLScalarVolumeNode* out)
{
	if (!a || !b || !out)
		return false;

	vtkImageData* imgA = a->GetImageData();
	vtkImageData* imgB = b->GetImageData();
	if (!imgA || !imgB)
		return false;

	int dimsA[3], dimsB[3];
	imgA->GetDimensions(dimsA);
	imgB->GetDimensions(dimsB);

	if (dimsA[0] != dimsB[0] || dimsA[1] != dimsB[1] || dimsA[2] != dimsB[2])
	{
		vtkWarningMacro("Dimension mismatch: resampling is needed before math.");
		return false;
	}

	vtkNew<vtkImageMathematics> math;
	math->SetInput1Data(imgA);
	math->SetInput2Data(imgB);
	math->SetOperationToMin();
	math->Update();

	vtkImageData* result = math->GetOutput();
	if (!result)
		return false;

	vtkNew<vtkImageData> outImage;
	outImage->DeepCopy(result);

	out->SetAndObserveImageData(outImage);
	out->CopyOrientation(a);
	out->SetSpacing(a->GetSpacing());
	out->SetOrigin(a->GetOrigin());
	out->Modified();

	return true;
}

bool vtkSlicervolume_mathLogic::MaxVolumes(vtkMRMLScalarVolumeNode* a, vtkMRMLScalarVolumeNode* b, vtkMRMLScalarVolumeNode* out)
{
	if (!a || !b || !out)
		return false;

	vtkImageData* imgA = a->GetImageData();
	vtkImageData* imgB = b->GetImageData();
	if (!imgA || !imgB)
		return false;

	int dimsA[3], dimsB[3];
	imgA->GetDimensions(dimsA);
	imgB->GetDimensions(dimsB);

	if (dimsA[0] != dimsB[0] || dimsA[1] != dimsB[1] || dimsA[2] != dimsB[2])
	{
		vtkWarningMacro("Dimension mismatch: resampling is needed before math.");
		return false;
	}

	vtkNew<vtkImageMathematics> math;
	math->SetInput1Data(imgA);
	math->SetInput2Data(imgB);
	math->SetOperationToMax();
	math->Update();

	vtkImageData* result = math->GetOutput();
	if (!result)
		return false;

	vtkNew<vtkImageData> outImage;
	outImage->DeepCopy(result);

	out->SetAndObserveImageData(outImage);
	out->CopyOrientation(a);
	out->SetSpacing(a->GetSpacing());
	out->SetOrigin(a->GetOrigin());
	out->Modified();

	return true;
}

bool vtkSlicervolume_mathLogic::SquareVolumes(vtkMRMLScalarVolumeNode* a, vtkMRMLScalarVolumeNode* out)
{
	if (!a || !out)
		return false;

	vtkImageData* imgA = a->GetImageData();

	if (!imgA)
		return false;

	int dimsA[3], dimsB[3];
	imgA->GetDimensions(dimsA);

	vtkNew<vtkImageMathematics> math;
	math->SetInput1Data(imgA);
	math->SetOperationToSquare();
	math->Update();

	vtkImageData* result = math->GetOutput();
	if (!result)
		return false;

	vtkNew<vtkImageData> outImage;
	outImage->DeepCopy(result);

	out->SetAndObserveImageData(outImage);
	out->CopyOrientation(a);
	out->SetSpacing(a->GetSpacing());
	out->SetOrigin(a->GetOrigin());
	out->Modified();

	return true;
}

bool vtkSlicervolume_mathLogic::SquareRootVolumes(vtkMRMLScalarVolumeNode* a, vtkMRMLScalarVolumeNode* out)
{
	if (!a || !out)
		return false;

	vtkImageData* imgA = a->GetImageData();

	if (!imgA)
		return false;

	int dimsA[3];
	imgA->GetDimensions(dimsA);

	//負の画素値を0にクランプ
	vtkNew<vtkImageThreshold> clamp;
	clamp->SetInputData(imgA);
	clamp->ThresholdByLower(0.0);
	clamp->SetInValue(0.0);
	clamp->ReplaceOutOff();
	clamp->Update();

	vtkNew<vtkImageMathematics> math;
	math->SetInput1Data(imgA);
	math->SetOperationToSquareRoot();
	math->Update();

	vtkImageData* result = math->GetOutput();
	if (!result)
		return false;

	vtkNew<vtkImageData> outImage;
	outImage->DeepCopy(result);

	out->SetAndObserveImageData(outImage);
	out->CopyOrientation(a);
	out->SetSpacing(a->GetSpacing());
	out->SetOrigin(a->GetOrigin());
	out->Modified();

	return true;
}


