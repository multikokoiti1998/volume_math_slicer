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

void vtkSlicervolume_mathLogic::AddVolumes(
	vtkMRMLScalarVolumeNode* inputA,
	vtkMRMLScalarVolumeNode* inputB,
	vtkMRMLScalarVolumeNode* output)
{
	if (!inputA || !inputB || !output)
		return;

	vtkImageData* imgA = inputA->GetImageData();
	vtkImageData* imgB = inputB->GetImageData();
	if (!imgA || !imgB)
		return;

	// 重要：型が違うと vtkImageMathematics が地雷を踏むので float に統一
	vtkNew<vtkImageCast> castA;
	castA->SetInputData(imgA);
	castA->SetOutputScalarTypeToFloat();

	vtkNew<vtkImageCast> castB;
	castB->SetInputData(imgB);
	castB->SetOutputScalarTypeToFloat();

	vtkNew<vtkImageMathematics> math;
	math->SetOperationToAdd();
	math->SetInputConnection(0, castA->GetOutputPort());
	math->SetInputConnection(1, castB->GetOutputPort());
	math->Update();

	// output に書き込む（DeepCopy推奨：VTKパイプラインの寿命問題を避ける）
	vtkNew<vtkImageData> outImg;
	outImg->DeepCopy(math->GetOutput());

	output->SetAndObserveImageData(outImg);
	output->CopyOrientation(inputA);
	output->SetSpacing(inputA->GetSpacing());
	output->SetOrigin(inputA->GetOrigin());

	output->Modified();
}