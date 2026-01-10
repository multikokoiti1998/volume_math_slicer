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

// Qt includes
#include <QDebug>

// Slicer includes
#include "qSlicervolume_mathModuleWidget.h"
#include "ui_qSlicervolume_mathModuleWidget.h"

//-----------------------------------------------------------------------------
class qSlicervolume_mathModuleWidgetPrivate: public Ui_qSlicervolume_mathModuleWidget
{
public:
  qSlicervolume_mathModuleWidgetPrivate();
};

//-----------------------------------------------------------------------------
// qSlicervolume_mathModuleWidgetPrivate methods

//-----------------------------------------------------------------------------
qSlicervolume_mathModuleWidgetPrivate::qSlicervolume_mathModuleWidgetPrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicervolume_mathModuleWidget methods

//-----------------------------------------------------------------------------
qSlicervolume_mathModuleWidget::qSlicervolume_mathModuleWidget(QWidget* _parent)
  : Superclass( _parent )
  , d_ptr( new qSlicervolume_mathModuleWidgetPrivate )
{
}

//-----------------------------------------------------------------------------
qSlicervolume_mathModuleWidget::~qSlicervolume_mathModuleWidget()
{
}

//-----------------------------------------------------------------------------
void qSlicervolume_mathModuleWidget::setup()
{
  Q_D(qSlicervolume_mathModuleWidget);
  d->setupUi(this);
  this->Superclass::setup();
}
