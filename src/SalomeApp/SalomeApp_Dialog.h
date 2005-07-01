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
  typedef QValueList<int>        TypesList;
  typedef QMap<int,QStringList>  SelectedObjects;
  typedef enum
  {
    OneName,
    OneNameOrCount,
    ListOfNames,
    Count
    
  } NameIndication;

public:
  SalomeApp_Dialog( QWidget* = 0, const char* = 0, bool = false,
                    bool = false, const int = Standard, WFlags = 0 );
  virtual ~SalomeApp_Dialog();
  
  virtual void    show();
  
  bool isExclusive() const;
  void setExclusive( const bool );

  void showObject( const int );
  void hideObject( const int );
  void setObjectShown( const int, const bool );
  bool isObjectShown( const int ) const;
          
  void selectObject( const QString&, const int, const QString& );
  void selectObject( const QStringList&, const TypesList&, const QStringList& );
  bool hasSelection( const int ) const;
  void clearSelection( const int = -1 );
  void selectedObject( const int, QStringList& ) const;
  void objectSelection( SelectedObjects& ) const;

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

        QString& typeName( const int );
  const QString& typeName( const int ) const;

  virtual QString selectionDescription( const QStringList&, const TypesList& ) const;
  virtual QString countOfTypes( const TypesList& ) const;

  NameIndication nameIndication() const;
  void           setNameIndication( const NameIndication );
  bool           multipleSelection() const;

private slots:
  void onToggled( bool );

private:
  void    updateButtons( const int = -1 );
  void    updateObject( const int, bool = true );
  void    filterTypes( const int, QStringList&, TypesList&, QStringList& ) const;
  
private:
  typedef struct
  {
    QLineEdit*   myEdit;
    QPushButton* myBtn;
    QLabel*      myLabel;
    QStringList  myNames, myEntries;
    TypesList    myTypes, myPossibleTypes;
    
  } Object;
  
  typedef QMap<int, Object> ObjectMap;

private:
  ObjectMap           myObjects;
  QMap<int,QString>   myTypeNames;
  bool                myIsExclusive;
  QPixmap             myPixmap;
  NameIndication      myNI;
};

#endif
