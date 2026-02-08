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

class VTK_SLICER_VOLUME_MATH_MODULE_LOGIC_EXPORT vtkSlicervolume_mathLogic :
	public vtkSlicerModuleLogic
{
public:

	static vtkSlicervolume_mathLogic* New();

	vtkTypeMacro(vtkSlicervolume_mathLogic, vtkSlicerModuleLogic);

	void PrintSelf(ostream& os, vtkIndent indent) override;

	bool AddVolumes(vtkMRMLScalarVolumeNode* a,vtkMRMLScalarVolumeNode* b,vtkMRMLScalarVolumeNode* out);

	bool SubVolumes(vtkMRMLScalarVolumeNode* a, vtkMRMLScalarVolumeNode* b, vtkMRMLScalarVolumeNode* out);

	bool MulVolumes(vtkMRMLScalarVolumeNode* a, vtkMRMLScalarVolumeNode* b, vtkMRMLScalarVolumeNode* out);

	bool DivVolumes(vtkMRMLScalarVolumeNode* a, vtkMRMLScalarVolumeNode* b, vtkMRMLScalarVolumeNode* out);

	bool MinVolumes(vtkMRMLScalarVolumeNode* a, vtkMRMLScalarVolumeNode* b, vtkMRMLScalarVolumeNode* out);

	bool MaxVolumes(vtkMRMLScalarVolumeNode* a, vtkMRMLScalarVolumeNode* b, vtkMRMLScalarVolumeNode* out);

	//2èÊ
	bool SquareVolumes(vtkMRMLScalarVolumeNode* a, vtkMRMLScalarVolumeNode* out);
	//ïΩï˚ç™
	bool SquareRootVolumes(vtkMRMLScalarVolumeNode* a, vtkMRMLScalarVolumeNode* out);


protected:
	vtkSlicervolume_mathLogic();
	~vtkSlicervolume_mathLogic() override;

	void SetMRMLSceneInternal(vtkMRMLScene* newScene) override;
	/// Register MRML Node classes to Scene. Gets called automatically when the MRMLScene is attached to this logic class.
	void RegisterNodes() override;
	void UpdateFromMRMLScene() override;
	void OnMRMLSceneNodeAdded(vtkMRMLNode* node) override;
	void OnMRMLSceneNodeRemoved(vtkMRMLNode* node) override;
private:

	vtkSlicervolume_mathLogic(const vtkSlicervolume_mathLogic&); // Not implemented
	void operator=(const vtkSlicervolume_mathLogic&); // Not implemented
};

#endif
