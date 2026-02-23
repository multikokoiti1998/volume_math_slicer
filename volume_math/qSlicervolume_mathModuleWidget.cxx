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
#include <QApplication>
#include <QMessageBox>
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

	this->outputVolumeNodeSelector->setNodeTypes(
		QStringList() << "vtkMRMLScalarVolumeNode");

	this->outputVolumeNodeSelector->setNoneEnabled(false);
	this->outputVolumeNodeSelector->setAddEnabled(true);
	this->outputVolumeNodeSelector->setRenameEnabled(true);
	this->outputVolumeNodeSelector->setRemoveEnabled(false);

	// MRML scene を NodeSelector に渡す
	this->inputAVolumeNodeSelector->setMRMLScene(q->mrmlScene());
	this->inputBVolumeNodeSelector->setMRMLScene(q->mrmlScene());
	this->outputVolumeNodeSelector->setMRMLScene(q->mrmlScene());

	this->input1VolumeNodeSelector->setNodeTypes(QStringList() << "vtkMRMLScalarVolumeNode");
	this->input2VolumeNodeSelector->setNodeTypes(QStringList() << "vtkMRMLScalarVolumeNode");
	this->input1VolumeNodeSelector->setMRMLScene(q->mrmlScene());
	this->input2VolumeNodeSelector->setMRMLScene(q->mrmlScene());


	this->ValitationComboBox->setEnabled(true);

	QObject::connect(this->validateButton, SIGNAL(clicked()),
		q, SLOT(onValidate()));

	QObject::connect(this->inputAVolumeNodeSelector, SIGNAL(currentNodeChanged(vtkMRMLNode*)),
		q, SLOT(onInputChanged()));
	QObject::connect(this->inputBVolumeNodeSelector, SIGNAL(currentNodeChanged(vtkMRMLNode*)),
		q, SLOT(onInputChanged()));
	QObject::connect(this->outputVolumeNodeSelector, SIGNAL(currentNodeChanged(vtkMRMLNode*)),
		q, SLOT(onInputChanged()));

	QObject::connect(this->input1VolumeNodeSelector, SIGNAL(currentNodeChanged(vtkMRMLNode*)),
		q, SLOT(onInputChanged()));
	QObject::connect(this->input2VolumeNodeSelector, SIGNAL(currentNodeChanged(vtkMRMLNode*)),
		q, SLOT(onInputChanged()));

	// 演算リストの追加 
	this->operationComboBox->addItem("Add", OP_ADD);
	this->operationComboBox->addItem("Subtract", OP_SUB);
	this->operationComboBox->addItem("Multiply", OP_MUL);
	this->operationComboBox->addItem("Divide", OP_DIV);
	this->operationComboBox->addItem("Min", OP_MIN);
	this->operationComboBox->addItem("Max", OP_MAX);
	this->operationComboBox->addItem("Square", OP_SQR);
	this->operationComboBox->addItem("Square Root", OP_SQRT);
	this->operationComboBox->addItem("Absolute (abs)", OP_ABS);

	// 論理演算
	this->operationComboBox->addItem("Logical AND", OP_AND);
	this->operationComboBox->addItem("Logical OR", OP_OR);
	this->operationComboBox->addItem("Logical NOT", OP_NOT);
	this->operationComboBox->addItem("Logical XOR", OP_XOR);

	//評価リスト
	this->ValitationComboBox->addItem("MSE", OP_MSE);
	this->ValitationComboBox->addItem("NCC", OP_NCC);
	
	QObject::connect(this->applyButton, SIGNAL(clicked()),
		q, SLOT(onApply()));

	QObject::connect(this->validateButton, SIGNAL(clicked()),
		q, SLOT(onValidate()));
}

void qSlicervolume_mathModuleWidgetPrivate::updateApplyState()
{
	Q_Q(qSlicervolume_mathModuleWidget);

	int opIndex = this->operationComboBox->currentIndex();
	VolumeOp op = static_cast<VolumeOp>(this->operationComboBox->itemData(opIndex).toInt());

	bool hasA = this->inputAVolumeNodeSelector->currentNode() != nullptr;
	bool hasB = this->inputBVolumeNodeSelector->currentNode() != nullptr;
	bool hasOut = this->outputVolumeNodeSelector->currentNode() != nullptr;

	bool isUnaryOperation = (op == OP_SQR || op == OP_SQRT || op == OP_NOT || op == OP_ABS);

	bool canApply = hasA && hasOut && (isUnaryOperation || hasB);
	this->applyButton->setEnabled(canApply);

	this->inputBVolumeNodeSelector->setEnabled(!isUnaryOperation);
}

void qSlicervolume_mathModuleWidget::setMRMLScene(vtkMRMLScene* scene)
{
	this->Superclass::setMRMLScene(scene);

	Q_D(qSlicervolume_mathModuleWidget);
	if (!d->inputAVolumeNodeSelector)
		return;
	//volume
	d->inputAVolumeNodeSelector->setNodeTypes(
		QStringList() << "vtkMRMLScalarVolumeNode");
	d->inputBVolumeNodeSelector->setNodeTypes(
		QStringList() << "vtkMRMLScalarVolumeNode");
	d->outputVolumeNodeSelector->setNodeTypes(
		QStringList() << "vtkMRMLScalarVolumeNode");
	d->inputAVolumeNodeSelector->setMRMLScene(scene);
	d->inputBVolumeNodeSelector->setMRMLScene(scene);
	d->outputVolumeNodeSelector->setMRMLScene(scene);
	//metrics
	d->input1VolumeNodeSelector->setNodeTypes(
		QStringList() << "vtkMRMLScalarVolumeNode");
	d->input2VolumeNodeSelector->setNodeTypes(
		QStringList() << "vtkMRMLScalarVolumeNode");
	d->MRMLTreeView->setNodeTypes(
		QStringList() << "vtkMRMLScalarVolumeNode");
	d->input1VolumeNodeSelector->setMRMLScene(scene);
	d->input2VolumeNodeSelector->setMRMLScene(scene);
	d->MRMLTreeView->model()->setHeaderData(0, Qt::Horizontal, "Current_Load_Volume");
	d->MRMLTreeView->setMRMLScene(scene);

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

	if (!a || !out) {
		QMessageBox::warning(this, "Missing input",
			"Please select Input A and Output volume.");
		return;
	}

	int opIndex = d->operationComboBox->currentIndex();
	VolumeOp op = static_cast<VolumeOp>(d->operationComboBox->itemData(opIndex).toInt());

	auto* logic = vtkSlicervolume_mathLogic::SafeDownCast(this->logic());
	if (logic)
	{

		QApplication::setOverrideCursor(Qt::WaitCursor);

		bool success = logic->ExecuteOperation(a, out, op, b);

		QApplication::restoreOverrideCursor();

		if (!success) {
			QMessageBox::critical(this, "Error", "Calculation failed. Please check the image dimensions");
		}
	}

}

void qSlicervolume_mathModuleWidget::onValidate()
{
	Q_D(qSlicervolume_mathModuleWidget);

	auto* a = vtkMRMLScalarVolumeNode::SafeDownCast(d->input1VolumeNodeSelector->currentNode());
	auto* b = vtkMRMLScalarVolumeNode::SafeDownCast(d->input2VolumeNodeSelector->currentNode());

	if (!a || !b) {
		QMessageBox::warning(this, "Missing input", "Please select Input1 and Input2 volumes.");
		return;
	}

	int idx = d->ValitationComboBox->currentIndex();
	VolumeOp metric = static_cast<VolumeOp>(d->ValitationComboBox->itemData(idx).toInt());

	auto* logic = vtkSlicervolume_mathLogic::SafeDownCast(this->logic());
	if (!logic) return;

	QApplication::setOverrideCursor(Qt::WaitCursor);

	double value = 0.0;
	bool ok = logic->ComputeMetric(a, b, metric, value);  

	QApplication::restoreOverrideCursor();

	if (!ok) {
		QMessageBox::critical(this, "Error", "Metric computation failed. Please check dimensions/types.");
		return;
	}

	// 結果表示
	d->validationResultLabel->setText(QString::number(value, 'g', 12));
}