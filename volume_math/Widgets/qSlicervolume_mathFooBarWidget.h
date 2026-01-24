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

#ifndef __qSlicervolume_mathFooBarWidget_h
#define __qSlicervolume_mathFooBarWidget_h

// Qt includes
#include <QWidget>

// FooBar Widgets includes
#include "qSlicervolume_mathModuleWidgetsExport.h"

class qSlicervolume_mathFooBarWidgetPrivate;

class Q_SLICER_MODULE_VOLUME_MATH_WIDGETS_EXPORT qSlicervolume_mathFooBarWidget
  : public QWidget
{
  Q_OBJECT
public:
  typedef QWidget Superclass;
  qSlicervolume_mathFooBarWidget(QWidget *parent=0);
  ~qSlicervolume_mathFooBarWidget() override;

protected slots:

protected:
	void setup() override;

protected:
  QScopedPointer<qSlicervolume_mathFooBarWidgetPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qSlicervolume_mathFooBarWidget);
  Q_DISABLE_COPY(qSlicervolume_mathFooBarWidget);
};

#endif
