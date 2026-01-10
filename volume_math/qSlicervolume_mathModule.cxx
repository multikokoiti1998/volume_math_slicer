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
#include <vtkSlicervolume_mathLogic.h>

// volume_math includes
#include "qSlicervolume_mathModule.h"
#include "qSlicervolume_mathModuleWidget.h"

//-----------------------------------------------------------------------------
class qSlicervolume_mathModulePrivate
{
public:
  qSlicervolume_mathModulePrivate();
};

//-----------------------------------------------------------------------------
// qSlicervolume_mathModulePrivate methods

//-----------------------------------------------------------------------------
qSlicervolume_mathModulePrivate::qSlicervolume_mathModulePrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicervolume_mathModule methods

//-----------------------------------------------------------------------------
qSlicervolume_mathModule::qSlicervolume_mathModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qSlicervolume_mathModulePrivate)
{
}

//-----------------------------------------------------------------------------
qSlicervolume_mathModule::~qSlicervolume_mathModule()
{
}

//-----------------------------------------------------------------------------
QString qSlicervolume_mathModule::helpText() const
{
  return "This is a loadable module that can be bundled in an extension";
}

//-----------------------------------------------------------------------------
QString qSlicervolume_mathModule::acknowledgementText() const
{
  return "This work was partially funded by NIH grant NXNNXXNNNNNN-NNXN";
}

//-----------------------------------------------------------------------------
QStringList qSlicervolume_mathModule::contributors() const
{
  QStringList moduleContributors;
  moduleContributors << QString("John Doe (AnyWare Corp.)");
  return moduleContributors;
}

//-----------------------------------------------------------------------------
QIcon qSlicervolume_mathModule::icon() const
{
  return QIcon(":/Icons/volume_math.png");
}

//-----------------------------------------------------------------------------
QStringList qSlicervolume_mathModule::categories() const
{
  return QStringList() << "Examples";
}

//-----------------------------------------------------------------------------
QStringList qSlicervolume_mathModule::dependencies() const
{
  return QStringList();
}

//-----------------------------------------------------------------------------
void qSlicervolume_mathModule::setup()
{
  this->Superclass::setup();
}

//-----------------------------------------------------------------------------
qSlicerAbstractModuleRepresentation* qSlicervolume_mathModule
::createWidgetRepresentation()
{
  return new qSlicervolume_mathModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qSlicervolume_mathModule::createLogic()
{
  return vtkSlicervolume_mathLogic::New();
}
