/*==============================================================================

  Program: 3D Slicer

  Copyright (c) Kitware Inc.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Jean-Christophe Fillion-Robin, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

// FooBar Widgets includes
#include "qSlicervolume_mathFooBarWidget.h"
#include "ui_qSlicervolume_mathFooBarWidget.h"

//-----------------------------------------------------------------------------
class qSlicervolume_mathFooBarWidgetPrivate
  : public Ui_qSlicervolume_mathFooBarWidget
{
  Q_DECLARE_PUBLIC(qSlicervolume_mathFooBarWidget);
protected:
  qSlicervolume_mathFooBarWidget* const q_ptr;

public:
  qSlicervolume_mathFooBarWidgetPrivate(
    qSlicervolume_mathFooBarWidget& object);
  virtual void setupUi(qSlicervolume_mathFooBarWidget*);
};

// --------------------------------------------------------------------------
qSlicervolume_mathFooBarWidgetPrivate
::qSlicervolume_mathFooBarWidgetPrivate(
  qSlicervolume_mathFooBarWidget& object)
  : q_ptr(&object)
{
}

// --------------------------------------------------------------------------
void qSlicervolume_mathFooBarWidgetPrivate
::setupUi(qSlicervolume_mathFooBarWidget* widget)
{
  this->Ui_qSlicervolume_mathFooBarWidget::setupUi(widget);
}

//-----------------------------------------------------------------------------
// qSlicervolume_mathFooBarWidget methods

//-----------------------------------------------------------------------------
qSlicervolume_mathFooBarWidget
::qSlicervolume_mathFooBarWidget(QWidget* parentWidget)
  : Superclass( parentWidget )
  , d_ptr( new qSlicervolume_mathFooBarWidgetPrivate(*this) )
{
  Q_D(qSlicervolume_mathFooBarWidget);
  d->setupUi(this);
}

//-----------------------------------------------------------------------------
qSlicervolume_mathFooBarWidget
::~qSlicervolume_mathFooBarWidget()
{
}
