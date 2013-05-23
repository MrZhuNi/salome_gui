// Copyright (C) 2005  OPEN CASCADE, CEA/DEN, EDF R&D, PRINCIPIA R&D
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either 
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful 
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
#ifndef SVTK_DoubleSpinBox_H
#define SVTK_DoubleSpinBox_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <QFrame>
#include <SVTK.h>

class SVTK_EXPORT QDblRangeControl
{
public:
  QDblRangeControl();
  QDblRangeControl( double minValue, double maxValue,
                    double lineStep, double pageStep, 
                    double value,
        int    precision = 6, 
        double dblPrecision = 1e-6,
        char   cFlag = 'g');
  virtual       ~QDblRangeControl();
  double        value() const;
  void          setValue( double );
  void          addPage();
  void          subtractPage();
  void          addLine();
  void          subtractLine();
                
  double        minValue()  const;
  double        maxValue()  const;
  void          setRange( double minValue, double maxValue );
  void          setMinValue( double minVal );
  void          setMaxValue( double minVal );
                
  double        lineStep()  const;
  double        pageStep()  const;
  void          setSteps( double line, double page );
                
  int           precision() const;
  void          setPrecision( int precision );
  double        dblPrecision() const;
  void          setDblPrecision( double dblPrecision );
  char          convertFlag() const;
  void          setConvertFlag( char cFlag );

  double        bound( double ) const;
  
protected:
  void          directSetValue( double val );
  double        prevValue()  const;
  
  virtual void  valueChange();
  virtual void  rangeChange();
  virtual void  stepChange();
  bool          equal( double first, double second ) const;
  double        roundPrecision( double value) const;
  
private:
  double        minVal, maxVal;
  double        line,   page;
  double        val,    prevVal;
  int           prec;
  double        dblPrec;
  char          convFlag;
  
private:  // Disabled copy constructor and operator=
#if defined(Q_DISABLE_COPY)
  QDblRangeControl( const QDblRangeControl & );
  QDblRangeControl &operator=( const QDblRangeControl & );
#endif
};

class QPushButton;
class QLineEdit;
class QValidator;

class SVTK_EXPORT SVTK_DoubleSpinBox: public QFrame, public QDblRangeControl
{
  Q_OBJECT

    friend class CATHAREGUI_DataSpinBoxDbl;

public:
  SVTK_DoubleSpinBox( QWidget* parent );
  SVTK_DoubleSpinBox( QWidget*    parent, 
      double      minValue, 
                  double      maxValue, 
                  double      step, 
      int         precision = 6, 
      double      dblPrecision = 1e-6,
      char        cFlag = 'g' );
  ~SVTK_DoubleSpinBox();

  QString           text() const;
  
  virtual QString   prefix() const;
  virtual QString   suffix() const;
  virtual QString   cleanText() const;

  virtual void      setSpecialValueText( const QString &text );
  QString           specialValueText() const;
  
  virtual void      setWrapping( bool on );
  bool              wrapping() const;
  
  enum ButtonSymbols { UpDownArrows, PlusMinus };
  void              setButtonSymbols( ButtonSymbols ); 
  ButtonSymbols     buttonSymbols() const;
  
  virtual void      setValidator( const QValidator* v );
  const QValidator* validator() const;
  
  QSize             sizeHint() const;
  
  double            minValue();
  double            maxValue();
  void              setMinValue( double );
  void              setMaxValue( double );
  double            lineStep();
  void              setLineStep( double );
  double            value();
                    
public slots:
  virtual void      setValue( double );
  virtual void      setPrefix( const QString &text );
  virtual void      setSuffix( const QString &text );
  virtual void      stepUp();
  virtual void      stepDown();
  virtual void      setEnabled( bool );
  virtual void      selectAll();
                    
signals:
  void              valueChanged( double value );
  void              valueChanged( const QString &valueText );

protected:
  virtual QString   mapValueToText( double value );
  virtual double    mapTextToValue( bool* ok );
  QString           currentValueText();
  
  virtual void      updateDisplay();
  virtual void      interpretText();
  
  QPushButton*      upButton() const;
  QPushButton*      downButton() const;
  QLineEdit*        editor() const;
  
  virtual void      valueChange();
  virtual void      rangeChange();
  
  bool              eventFilter( QObject* obj, QEvent* ev );
  void              resizeEvent( QResizeEvent* ev );
  void              wheelEvent( QWheelEvent * );
  void              leaveEvent( QEvent* );
  
  void              styleChange( QStyle& );
  
protected slots:
  void              textChanged();
  
private:
  void              initSpinBox();

  ButtonSymbols     butSymbols;
  QPushButton*      up;
  QPushButton*      down;
  QLineEdit*        vi;
  QValidator*       validate;
  QString           pfix;
  QString           sfix;
  QString           specText;
  bool              wrap;
  bool              edited;
  bool              selreq;
  
  void              arrangeWidgets();
  void              updateButtonSymbols();
  
  private:  // Disabled copy constructor and operator=
#if defined(Q_DISABLE_COPY)
    SVTK_DoubleSpinBox( const SVTK_DoubleSpinBox& );
    SVTK_DoubleSpinBox& operator=( const SVTK_DoubleSpinBox& );
#endif
    
};                  

#endif
