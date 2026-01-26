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
#include <vtkMRMLScalarVolumeNode.h>
#include <vtkMRMLNode.h>

//-----------------------------------------------------------------------------
class qSlicervolume_mathFooBarWidgetPrivate
	: public Ui_qSlicervolume_mathFooBarWidget
{
	Q_DECLARE_PUBLIC(qSlicervolume_mathFooBarWidget);

protected:
	qSlicervolume_mathFooBarWidget *const q_ptr;

public:
	qSlicervolume_mathFooBarWidgetPrivate(
		qSlicervolume_mathFooBarWidget &object);
	virtual void setupUi(qSlicervolume_mathFooBarWidget *);
	void updateApplyState();
	void onApplyClicked();
};

// --------------------------------------------------------------------------
qSlicervolume_mathFooBarWidgetPrivate ::qSlicervolume_mathFooBarWidgetPrivate(
	qSlicervolume_mathFooBarWidget &object)
	: q_ptr(&object)
{
}

// --------------------------------------------------------------------------
void qSlicervolume_mathFooBarWidgetPrivate ::setupUi(qSlicervolume_mathFooBarWidget *widget)
{
	this->Ui_qSlicervolume_mathFooBarWidget::setupUi(widget);
}

//-----------------------------------------------------------------------------
// qSlicervolume_mathFooBarWidget methods

//-----------------------------------------------------------------------------
qSlicervolume_mathFooBarWidget ::qSlicervolume_mathFooBarWidget(QWidget *parentWidget)
	: Superclass(parentWidget), d_ptr(new qSlicervolume_mathFooBarWidgetPrivate(*this))
{
	Q_D(qSlicervolume_mathFooBarWidget);
	d->setupUi(this);
}

//-----------------------------------------------------------------------------
qSlicervolume_mathFooBarWidget ::~qSlicervolume_mathFooBarWidget()
{
}

void qSlicervolume_mathFooBarWidgetPrivate::onApplyClicked()
{
	Q_Q(qSlicervolume_mathFooBarWidget);

	auto *a = vtkMRMLScalarVolumeNode::SafeDownCast(this->inputAVolumeNodeSelector->currentNode());
	auto *b = vtkMRMLScalarVolumeNode::SafeDownCast(this->inputBVolumeNodeSelector->currentNode());
	auto *out = vtkMRMLScalarVolumeNode::SafeDownCast(this->outputVolumeNodeSelector->currentNode());

	if (!a || !b || !out)
		return;
	q->logic()->AddVolumes(a, b, out);
}

void qSlicervolume_mathFooBarWidgetPrivate::setupUi(qSlicervolume_mathFooBarWidget *widget)
{
	this->Ui_qSlicervolume_mathFooBarWidget::setupUi(widget);

	// 入力変更で Apply の有効/無効を更新（qMRMLNodeComboBox想定）
	QObject::connect(this->inputAVolumeNodeSelector, SIGNAL(currentNodeChanged(vtkMRMLNode *)),
					 [this](vtkMRMLNode *)
					 { this->updateApplyState(); });
	QObject::connect(this->inputBVolumeNodeSelector, SIGNAL(currentNodeChanged(vtkMRMLNode *)),
					 [this](vtkMRMLNode *)
					 { this->updateApplyState(); });
	QObject::connect(this->outputVolumeNodeSelector, SIGNAL(currentNodeChanged(vtkMRMLNode *)),
					 [this](vtkMRMLNode *)
					 { this->updateApplyState(); });

	this->updateApplyState();
}

void qSlicervolume_mathFooBarWidget::setMRMLScene(vtkMRMLScene *scene)
{
	this->Superclass::setMRMLScene(scene);

	Q_D(qSlicervolume_mathFooBarWidget);
	if (!scene)
		return;

	d->inputAVolumeNodeSelector->setMRMLScene(scene);
	d->inputBVolumeNodeSelector->setMRMLScene(scene);
	d->outputVolumeNodeSelector->setMRMLScene(scene);
}
