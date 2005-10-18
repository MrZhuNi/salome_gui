#ifndef SVTK_SELECTIONCHANGED_H
#define SVTK_SELECTIONCHANGED_H

#include <qobject.h>

#include "SVTK.h"
#include "SUIT_ViewModel.h"

class SVTK_EXPORT SVTK_ViewModelBase: public SUIT_ViewModel
{
  Q_OBJECT;

public slots:
  void onSelectionChanged()
  {
    emit selectionChanged();
  }

 signals:
  void selectionChanged();
};

#endif
