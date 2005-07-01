// File:      SalomeApp_Dialog.h
// Author:    Alexander SOLOVYOV

#ifndef SALOMEAPP_DIALOG_H
#define SALOMEAPP_DIALOG_H

#include <QtxDialog.h>

#include <qvaluelist.h>
#include <qmap.h>
#include <qpixmap.h>


class QLineEdit;
class QPushButton;
class QLabel;
class SUIT_ResourceMgr;


class SalomeApp_Dialog : public QtxDialog
{
  Q_OBJECT
  
public:
  typedef QValueList<int>    TypesList;
  typedef QMap<int,QString>  SelectedObjects;

public:
  SalomeApp_Dialog( QWidget* = 0, const char* = 0, bool = false,
                    bool = false, const int = Standard, WFlags = 0 );
  virtual ~SalomeApp_Dialog();
  
  virtual void    show();
          bool    isExclusive() const;
          void    setExclusive( const bool );

          void    showObject( const int );
          void    hideObject( const int );
          void    setObjectShown( const int, const bool );
          bool    isObjectShown( const int ) const;
          
          void    selectObject( const QString&, const int, const QString& );
          bool    hasSelection( const int ) const;
          void    clearSelection( const int = -1 );
          QString selectedObject( const int ) const;
          void    objectSelection( SelectedObjects& ) const;

signals:
  void selectionChanged ( int );
  void objectActivated  ( int );
  void objectDeactivated( int );
                                                   
protected:
  SUIT_ResourceMgr* resMgr() const;

  int  createObject    ( const QString&, QWidget*, const int = -1 );
  void setObjectPixmap ( const QPixmap& );
  void setObjectPixmap ( const QString&, const QString& );
  void renameObject    ( const int, const QString& );
  void setObjectType   ( const int, const int, ... );
  void setObjectType   ( const int, const TypesList& );
  void addObjectType   ( const int, const int, const int, ... );
  void addObjectType   ( const int, const TypesList& );
  void addObjectType   ( const int, const int );
  void removeObjectType( const int );
  void removeObjectType( const int, const TypesList& );
  void removeObjectType( const int, const int );
  bool hasObjectType   ( const int, const int ) const;
  void objectTypes     ( const int, TypesList& ) const;

private slots:
  void onToggled( bool );

private:
  void updateButtons( const int = -1 );
  void updateObject( const int );
  bool isCorrectType( const int id, const int type ) const;
  
private:
  typedef struct
  {
    QLineEdit*   edit;
    QPushButton* btn;
    QLabel*      label;
    QString      entry;
    int          type;
    TypesList    types;
    
  } Object;
  
  typedef QMap<int, Object> ObjectMap;

private:
  ObjectMap  myObjects;
  bool       myIsExclusive;
  QPixmap    myPixmap;
};

#endif
