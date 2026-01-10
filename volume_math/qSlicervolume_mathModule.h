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

#ifndef __qSlicervolume_mathModule_h
#define __qSlicervolume_mathModule_h

// Slicer includes
#include "qSlicerLoadableModule.h"

#include "qSlicervolume_mathModuleExport.h"

class qSlicervolume_mathModulePrivate;

class Q_SLICER_QTMODULES_VOLUME_MATH_EXPORT
qSlicervolume_mathModule
  : public qSlicerLoadableModule
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "org.slicer.modules.loadable.qSlicerLoadableModule/1.0");
  Q_INTERFACES(qSlicerLoadableModule);

public:

  typedef qSlicerLoadableModule Superclass;
  explicit qSlicervolume_mathModule(QObject *parent=nullptr);
  ~qSlicervolume_mathModule() override;

  qSlicerGetTitleMacro(tr("volume_math"));

  QString helpText()const override;
  QString acknowledgementText()const override;
  QStringList contributors()const override;

  QIcon icon()const override;

  QStringList categories()const override;
  QStringList dependencies() const override;

protected:

  /// Initialize the module. Register the volumes reader/writer
  void setup() override;

  /// Create and return the widget representation associated to this module
  qSlicerAbstractModuleRepresentation * createWidgetRepresentation() override;

  /// Create and return the logic associated to this module
  vtkMRMLAbstractLogic* createLogic() override;

protected:
  QScopedPointer<qSlicervolume_mathModulePrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qSlicervolume_mathModule);
  Q_DISABLE_COPY(qSlicervolume_mathModule);

};

#endif
