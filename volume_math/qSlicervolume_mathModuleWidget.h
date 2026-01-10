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

#ifndef __qSlicervolume_mathModuleWidget_h
#define __qSlicervolume_mathModuleWidget_h

// Slicer includes
#include "qSlicerAbstractModuleWidget.h"

#include "qSlicervolume_mathModuleExport.h"

class qSlicervolume_mathModuleWidgetPrivate;
class vtkMRMLNode;

class Q_SLICER_QTMODULES_VOLUME_MATH_EXPORT qSlicervolume_mathModuleWidget :
  public qSlicerAbstractModuleWidget
{
  Q_OBJECT

public:

  typedef qSlicerAbstractModuleWidget Superclass;
  qSlicervolume_mathModuleWidget(QWidget *parent=0);
  virtual ~qSlicervolume_mathModuleWidget();

public slots:


protected:
  QScopedPointer<qSlicervolume_mathModuleWidgetPrivate> d_ptr;

  void setup() override;

private:
  Q_DECLARE_PRIVATE(qSlicervolume_mathModuleWidget);
  Q_DISABLE_COPY(qSlicervolume_mathModuleWidget);
};

#endif
