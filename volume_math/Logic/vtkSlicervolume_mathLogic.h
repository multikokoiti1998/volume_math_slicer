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

// .NAME vtkSlicervolume_mathLogic - slicer logic class for volumes manipulation
// .SECTION Description
// This class manages the logic associated with reading, saving,
// and changing propertied of the volumes


#ifndef __vtkSlicervolume_mathLogic_h
#define __vtkSlicervolume_mathLogic_h

// Slicer includes
#include "vtkSlicerModuleLogic.h"

// MRML includes

// STD includes
#include <cstdlib>

#include "vtkSlicervolume_mathModuleLogicExport.h"
#include <vtkMRMLScalarVolumeNode.h>
#include <vtkImageData.h>
#include <vtkImageCast.h>
#include <vtkImageMathematics.h>
#include <vtkNew.h>
#include <vtkSmartPointer.h>


enum VolumeOp {
	OP_MATH_START = 100,
	OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_MIN, OP_MAX, OP_ABS, OP_SQR, OP_SQRT,
	OP_LOGIC_START = 200,
	OP_AND, OP_OR, OP_XOR, OP_NOT
};

enum ValidationOp {
	OP_MSE = 1,
	OP_NCC = 2
};

class VTK_SLICER_VOLUME_MATH_MODULE_LOGIC_EXPORT vtkSlicervolume_mathLogic :
	public vtkSlicerModuleLogic
{
public:

	static vtkSlicervolume_mathLogic* New();



	vtkTypeMacro(vtkSlicervolume_mathLogic, vtkSlicerModuleLogic);

	void PrintSelf(ostream& os, vtkIndent indent) override;

	// ã§í èàóùä÷êî
	bool ExecuteOperation(
		vtkMRMLScalarVolumeNode* a,
		vtkMRMLScalarVolumeNode* out,
		VolumeOp op,
		vtkMRMLScalarVolumeNode* b = nullptr);

	bool ComputeMetric(
		vtkMRMLScalarVolumeNode* a,
		vtkMRMLScalarVolumeNode* b,
		VolumeOp metric,
		double& outValue);

	vtkSmartPointer<vtkImageData> CastToFloat(vtkImageData* input);

protected:
	vtkSlicervolume_mathLogic();
	~vtkSlicervolume_mathLogic() override;

	void SetMRMLSceneInternal(vtkMRMLScene* newScene) override;
	void RegisterNodes() override;
	void UpdateFromMRMLScene() override;
	void OnMRMLSceneNodeAdded(vtkMRMLNode* node) override;
	void OnMRMLSceneNodeRemoved(vtkMRMLNode* node) override;
private:

	vtkSlicervolume_mathLogic(const vtkSlicervolume_mathLogic&); 
	void operator=(const vtkSlicervolume_mathLogic&); 
};

#endif
