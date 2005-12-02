#include "PythonConsole_PyConsole.h"
#include "PythonConsole_PyEditor.h"
#include "PyInterp_base.h"

#include "SUIT_Desktop.h"

#include <qlayout.h>

using namespace std;

//****************************************************************
PythonConsole::PythonConsole(QWidget* parent, PyInterp_base* interp)
: QWidget(parent), myEditor( 0 )
{
  cerr << "PythonConsole::PythonConsole" << endl;
  // create python interpreter
  myInterp = interp;
  if ( !myInterp )
    myInterp = new PythonConsole_PyInterp();
  
  // initialize Python interpretator
  myInterp->initialize();

  // create editor console
  QVBoxLayout* lay = new QVBoxLayout( this );
  myEditor = new PythonConsole_PyEditor(myInterp, this,"Python Interpreter");
  lay->addWidget( myEditor );
  cerr << "-----------------------------PythonConsole::PythonConsole" << endl;
}

//****************************************************************
PythonConsole::~PythonConsole()
{
}

//****************************************************************
void PythonConsole::exec( const QString& command )
{
  if ( myEditor )
    myEditor->exec( command );
}

//****************************************************************
void PythonConsole::setFont( const QFont& f )
{
  if( myEditor )
    myEditor->setFont( f );
}

//****************************************************************
QFont PythonConsole::font() const
{
  QFont res;
  if( myEditor )
    res = myEditor->font();
  return res;
}
