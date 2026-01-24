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
	qSlicervolume_mathFooBarWidget* const q_ptr;

public:
	qSlicervolume_mathFooBarWidgetPrivate(
		qSlicervolume_mathFooBarWidget& object);
	virtual void setupUi(qSlicervolume_mathFooBarWidget*);
	void onApplyClicked();
	void onInputAChanged();
	void onInputBChanged();
	void onOutputChanged();
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
	: Superclass(parentWidget)
	, d_ptr(new qSlicervolume_mathFooBarWidgetPrivate(*this))
{
	Q_D(qSlicervolume_mathFooBarWidget);
	d->setupUi(this);
}

//-----------------------------------------------------------------------------
qSlicervolume_mathFooBarWidget
::~qSlicervolume_mathFooBarWidget()
{

}

//void qSlicervolume_mathFooBarWidgetPrivate::onApplyClicked()
//{
//	Q_Q(qSlicervolume_mathFooBarWidget);
//
//	auto* inputA = vtkMRMLScalarVolumeNode::SafeDownCast(
//		this->inputAVolumeNodeSelector->currentNode());
//	auto* inputB = vtkMRMLScalarVolumeNode::SafeDownCast(
//		this->inputBVolumeNodeSelector->currentNode());
//	auto* output = vtkMRMLScalarVolumeNode::SafeDownCast(
//		this->outputVolumeNodeSelector->currentNode());
//
//	if (!inputA || !inputB || !output)
//		return;
//
//	q->logic()->AddVolumes(inputA, inputB, output);
//}

//void qSlicervolume_mathFooBarWidgetPrivate::setupUi(qSlicervolume_mathFooBarWidget* widget)
//{
//	this->Ui_qSlicervolume_mathFooBarWidget::setupUi(widget);
//
//	// Apply
//	QObject::connect(this->applyButton, &QPushButton::clicked,
//		[this]() { this->onApplyClicked(); });
//
//	// 入力変更（qMRMLNodeComboBox想定）
//	QObject::connect(this->inputAVolumeNodeSelector, SIGNAL(currentNodeChanged(vtkMRMLNode*)),
//		widget, SLOT(updateGUIFromMRML())); // 例：Widget側に更新関数があるなら
//	// もしくは lambda で private の関数へ
//	QObject::connect(this->inputAVolumeNodeSelector, SIGNAL(currentNodeChanged(vtkMRMLNode*)),
//		[this](vtkMRMLNode*) { this->onInputAChanged(); });
//
//	QObject::connect(this->inputBVolumeNodeSelector, SIGNAL(currentNodeChanged(vtkMRMLNode*)),
//		[this](vtkMRMLNode*) { this->onInputBChanged(); });
//
//	QObject::connect(this->outputVolumeNodeSelector, SIGNAL(currentNodeChanged(vtkMRMLNode*)),
//		[this](vtkMRMLNode*) { this->onOutputChanged(); });
//}







