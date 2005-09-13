//  File   : LightApp_NameDlg.h
//  Author : Vadim SANDLER
//  $Header$

#ifndef LIGHTAPP_NAMEDLG_H
#define LIGHTAPP_NAMEDLG_H

#include <qdialog.h>

class QLineEdit;
class QPushButton;

//=================================================================================
// class    : LightApp_NameDlg
/*! purpose  : Common <Rename> dialog box class*/
//=================================================================================
class LightApp_NameDlg : public QDialog
{ 
  Q_OBJECT

public:
  LightApp_NameDlg( QWidget* parent = 0 );
  ~LightApp_NameDlg();
    
  void            setName( const QString& name );
  QString         name();
    
  static QString  getName( QWidget* parent = 0, const QString& oldName = QString::null );
    
protected slots:
  void accept();
  
private:
  QPushButton*    myButtonOk;
  QPushButton*    myButtonCancel;
  QLineEdit*      myLineEdit;
};

#endif // LightApp_NAMEDLG_H
