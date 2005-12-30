
#include "QtxDblValidator.h"

QtxDblValidator::QtxDblValidator( const double bot, const double top, const int dec,
				  QObject* o, const char* name )
: QDoubleValidator( bot, top, dec, o, name )
{
}

QtxDblValidator::~QtxDblValidator()
{
}

void QtxDblValidator::fixup( QString& str ) const
{
  bool ok = false;
  double d = str.toDouble( &ok );
  if( ok )
  {
    if( d<bottom() )
      str = QString::number( bottom() );
    else if( d>top() )
      str = QString::number( top() );
  }
  else
    str = "0";
}
