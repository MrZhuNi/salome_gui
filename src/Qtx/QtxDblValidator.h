
#ifndef QTX_DOUBLE_VALIDATOR
#define QTX_DOUBLE_VALIDATOR

#include <qvalidator.h>

class QtxDblValidator : public QDoubleValidator
{
  Q_OBJECT

public:
  QtxDblValidator( const double, const double, const int,
		   QObject*, const char* = 0 );
  ~QtxDblValidator();

  virtual void fixup( QString& ) const;
};

#endif
