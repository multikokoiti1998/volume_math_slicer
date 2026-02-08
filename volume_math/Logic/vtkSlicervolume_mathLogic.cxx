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

bool vtkSlicervolume_mathLogic::ExecuteOperation(
	vtkMRMLScalarVolumeNode* a,
	vtkMRMLScalarVolumeNode* out,
	VolumeOp op,
	vtkMRMLScalarVolumeNode* b)
{
	// 1. 基本的なバリデーション
	if (!a || !out) return false;
	vtkImageData* imgA = a->GetImageData();
	if (!imgA) return false;

	// 2. 二項演算の場合の共通チェック
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

	// 3. フィルタの選択と設定
	vtkSmartPointer<vtkImageAlgorithm> filter;

	if (op >= OP_LOGIC_START) {
		// --- vtkImageLogic を使用するケース ---
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
	else {
		// --- vtkImageMathematics を使用するケース ---
		vtkNew<vtkImageMathematics> math;

		// 特殊処理：SquareRoot のためのクランプ
		if (op == OP_SQRT) {
			vtkNew<vtkImageThreshold> clamp;
			clamp->SetInputData(imgA);
			clamp->ThresholdByLower(0.0);
			clamp->SetInValue(0.0);
			clamp->ReplaceOutOff();
			clamp->Update();
			math->SetInput1Data(clamp->GetOutput());
		}
		else {
			math->SetInput1Data(imgA);
		}

		if (imgB) math->SetInput2Data(imgB);

		switch (op) {
		case OP_ADD:      math->SetOperationToAdd(); break;
		case OP_SUB:      math->SetOperationToSubtract(); break;
		case OP_MUL:      math->SetOperationToMultiply(); break;
		case OP_DIV:      math->SetOperationToDivide(); break;
		case OP_MIN:      math->SetOperationToMin(); break;
		case OP_MAX:      math->SetOperationToMax(); break;
		case OP_SQR:      math->SetOperationToSquare(); break;
		case OP_SQRT:     math->SetOperationToSquareRoot(); break;
		case OP_ABS:      math->SetOperationToAbsoluteValue(); break;
		default: return false;
		}
		filter = math;
	}

	// 4. 実行と結果の反映（共通処理）
	if (!filter) return false;
	filter->Update();

	// 出力データの作成とメタデータのコピー
	vtkNew<vtkImageData> outImage;
	outImage->DeepCopy(filter->GetOutputDataObject(0));

	out->SetAndObserveImageData(outImage);
	out->CopyOrientation(a);
	out->SetSpacing(a->GetSpacing());
	out->SetOrigin(a->GetOrigin());
	out->Modified();

	return true;
}