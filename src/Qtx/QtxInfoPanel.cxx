// Copyright (C) 2007-2020  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//

// File   : InfoPanel.cxx
// Author : Viktor UZLOV, Open CASCADE S.A.S. (viktor.uzlov@opencascade.com)
//

#include "QtxInfoPanel.h"

#include <QGroupBox>
#include <QLabel>
#include <QLayoutItem>

QtxInfoPanel::QtxInfoPanel( QWidget* parent )
  : QWidget( parent )
{
  tbar = new QToolBar();
  tbar->setOrientation(Qt::Vertical);
  tbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  QVBoxLayout* layout = new QVBoxLayout(this);
  layout->setMargin( 0 );
  layout->addWidget(tbar);
}

QtxInfoPanel::~QtxInfoPanel()
{
}

int QtxInfoPanel::addLabel( const QString& text, Qt::Alignment alignment, const int groupId )
{
  QLabel *label = new QLabel(text);
  label->setAlignment(alignment);
  label->setWordWrap(true);
  
  int id_new = generateId();
  QToolBar* tb = getToolBar(groupId);
  QAction* q = tb->addWidget(label);
  q->setProperty("id", id_new);
  alignLeft(tb->layout());
  
  return id_new;
}

int QtxInfoPanel::addAction( QAction* action, const int groupId )
{
  int id_new = generateId();
  action->setProperty("id", id_new);
  QToolBar* tb = getToolBar(groupId);
  tb->addAction(action);
  alignLeft(tb->layout());

  return id_new;
}

int QtxInfoPanel::addGroup( const QString& text, const int groupId )
{
  QGroupBox* box = new QGroupBox(text);
  QVBoxLayout *vbox = new QVBoxLayout;
  box->setLayout(vbox);
  QToolBar* toolbar = new QToolBar();
  toolbar->setOrientation(Qt::Vertical);
  toolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  vbox->addWidget(toolbar);
  
  int id_new = generateId();
  QToolBar* tb = getToolBar(groupId);
  QAction* q = tb->addWidget(box);
  box->setProperty("id", id_new);
  alignLeft(tb->layout());

  return id_new;
}

void QtxInfoPanel::remove(const int actionId, const int groupId)
{
  // TODO: Method don't delete QAction
  QToolBar* tb = getToolBar(groupId);
  tb->removeAction(getAction(actionId, groupId));
}

void QtxInfoPanel::clear(const int groupId)
{
  QToolBar* tb = getToolBar(groupId);
  tb->clear();
}

void QtxInfoPanel::alignLeft( QLayout* lay )
{
  for(int i = 0; i < lay->count(); ++i)
    lay->itemAt(i)->setAlignment(Qt::AlignLeft);
}

QToolBar* QtxInfoPanel::getToolBar( const int id )
{
  if (id == -1)
    return this->tbar;
  for(QAction* a : this->tbar->actions())
  {
    QWidget* w= this->tbar->widgetForAction(a);
    if(w != NULL)
    {
        if (w->property("id").value<int>() == id)
        {
          QGroupBox *qgb = dynamic_cast<QGroupBox*>(w);
          if (qgb)
             return dynamic_cast<QToolBar*>(qgb->layout()->itemAt(0)->widget());
        }
    }
  }
  return nullptr;
}

QAction* QtxInfoPanel::getAction( const int actionId, const int groupId )
{
  QToolBar* tb = getToolBar(groupId);
  for(QAction* a : tb->actions())
  {
    if(a->property("id").value<int>() == actionId)
      return a;
  }
  return nullptr;
}

void QtxInfoPanel::setVisible( const int actionId, bool state, const int groupId )
{
  getAction(actionId, groupId)->setVisible(state);
}

void QtxInfoPanel::setEnabled( const int actionId, bool state, const int groupId )
{
  getAction(actionId, groupId)->setEnabled(state);
}

int QtxInfoPanel::generateId() const
{
  static int id = -100;
  return --id;
}
