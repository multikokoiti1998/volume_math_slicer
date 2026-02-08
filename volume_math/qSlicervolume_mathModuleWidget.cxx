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

// MRML includes
#include <vtkMRMLScene.h>
#include <vtkMRMLScalarVolumeNode.h>

// Slicer includes
#include "qSlicervolume_mathModuleWidget.h"
#include "ui_qSlicervolume_mathModuleWidget.h"

#include "vtkSlicervolume_mathLogic.h"


//-----------------------------------------------------------------------------
class qSlicervolume_mathModuleWidgetPrivate : public Ui_qSlicervolume_mathModuleWidget
{
	Q_DECLARE_PUBLIC(qSlicervolume_mathModuleWidget);

public:
	qSlicervolume_mathModuleWidgetPrivate(qSlicervolume_mathModuleWidget& object);

	void init();
	void updateApplyState();

private:
	qSlicervolume_mathModuleWidget* const q_ptr;
};

//-----------------------------------------------------------------------------
// qSlicervolume_mathModuleWidgetPrivate methods

//-----------------------------------------------------------------------------
qSlicervolume_mathModuleWidgetPrivate::qSlicervolume_mathModuleWidgetPrivate(qSlicervolume_mathModuleWidget& object)
	: q_ptr(&object)
{
}

//-----------------------------------------------------------------------------
// qSlicervolume_mathModuleWidget methods

//-----------------------------------------------------------------------------
qSlicervolume_mathModuleWidget::qSlicervolume_mathModuleWidget(QWidget* _parent)
	: Superclass(_parent)
	, d_ptr(new qSlicervolume_mathModuleWidgetPrivate(*this))
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

	d->init();
}

void qSlicervolume_mathModuleWidgetPrivate::init()
{
	Q_Q(qSlicervolume_mathModuleWidget);

	// MRML scene を NodeSelector に渡す（重要）
	// ※ objectName はあなたのuiに合わせる
	this->inputAVolumeNodeSelector->setMRMLScene(q->mrmlScene());
	this->inputBVolumeNodeSelector->setMRMLScene(q->mrmlScene());
	this->outputVolumeNodeSelector->setMRMLScene(q->mrmlScene());

	// まずはApply無効
	this->applyButton->setEnabled(false);

	// 入力が変わったら apply の有効/無効を更新
	QObject::connect(this->inputAVolumeNodeSelector, SIGNAL(currentNodeChanged(vtkMRMLNode*)),
		q, SLOT(onInputChanged()));
	QObject::connect(this->inputBVolumeNodeSelector, SIGNAL(currentNodeChanged(vtkMRMLNode*)),
		q, SLOT(onInputChanged()));
	QObject::connect(this->outputVolumeNodeSelector, SIGNAL(currentNodeChanged(vtkMRMLNode*)),
		q, SLOT(onInputChanged()));

	// Apply
	QObject::connect(this->applyButton, SIGNAL(clicked()),
		q, SLOT(onApply()));
}

void qSlicervolume_mathModuleWidgetPrivate::updateApplyState()
{
	bool ok =
		this->inputAVolumeNodeSelector->currentNode() &&
		this->inputBVolumeNodeSelector->currentNode() &&
		this->outputVolumeNodeSelector->currentNode();

	this->applyButton->setEnabled(ok);
}

void qSlicervolume_mathModuleWidget::setMRMLScene(vtkMRMLScene* scene)
{
	this->Superclass::setMRMLScene(scene);

	Q_D(qSlicervolume_mathModuleWidget);
	if (!d->inputAVolumeNodeSelector)
		return;

	d->inputAVolumeNodeSelector->setMRMLScene(scene);
	d->inputBVolumeNodeSelector->setMRMLScene(scene);
	d->outputVolumeNodeSelector->setMRMLScene(scene);

	d->updateApplyState();
}

void qSlicervolume_mathModuleWidget::onInputChanged()
{
	Q_D(qSlicervolume_mathModuleWidget);
	d->updateApplyState();
}

void qSlicervolume_mathModuleWidget::onApply()
{
	Q_D(qSlicervolume_mathModuleWidget);

	auto* a = vtkMRMLScalarVolumeNode::SafeDownCast(d->inputAVolumeNodeSelector->currentNode());
	auto* b = vtkMRMLScalarVolumeNode::SafeDownCast(d->inputBVolumeNodeSelector->currentNode());
	auto* out = vtkMRMLScalarVolumeNode::SafeDownCast(d->outputVolumeNodeSelector->currentNode());

	if (!a || !b || !out)
	{
		qWarning() << "Invalid volume node(s)";
		return;
	}

	auto* logic = vtkSlicervolume_mathLogic::SafeDownCast(this->logic());
	if (!logic)
	{
		qWarning() << "Logic not available";
		return;
	}

}
