#ifndef SVTK_Functor_H
#define SVTK_Functor_H

#include <functional>

#include <string>

#include <VTKViewer_Functor.h>

#include "SALOME_InteractiveObject.hxx"

/*!
  \file SVTK_Functor.h
  This file contains numbers of functors that allows user to perform corresponding operations with existing presentations.
  Combination with algorithms it gives powerful, flexible and simple to extend way to introduce new type of operation.
*/

namespace VTK
{
  //! This functor check, if the actor have pointed entry
  template<class TActor> 
  struct TIsSameEntry
  {
    std::string myEntry;
    //! To construct the functor
    TIsSameEntry(const char* theEntry): 
      myEntry(theEntry) 
    {}
    //! To calculate the functor
    bool operator()(TActor* theActor)
    {
      if ( theActor->hasIO() )
      {
	Handle(SALOME_InteractiveObject) anIO = theActor->getIO();
	if ( anIO->hasEntry() )
	  return myEntry == anIO->getEntry();
      }
      return false;
    }
  };


  //----------------------------------------------------------------
  //! This functor check, if the actor point to the same #SALOME_InteractiveObject
  template<class TActor> 
  struct TIsSameIObject
  {
    Handle(SALOME_InteractiveObject) myIObject;
    //! To construct the functor
    TIsSameIObject(const Handle(SALOME_InteractiveObject)& theIObject):
      myIObject(theIObject)
    {}
    //! To calculate the functor
    bool operator()(TActor* theActor)
    {
      if(theActor->hasIO())
      {
	Handle(SALOME_InteractiveObject) anIO = theActor->getIO();
	return myIObject->isSame(anIO);
      }
      return false;
    }
  };


  //----------------------------------------------------------------
  /*!
    This highlight every input actor
  */
  template<class TActor> 
  struct THighlight
  {
    bool myIsHighlight;
    //! To construct the functor
    THighlight(bool theIsHighlight):
      myIsHighlight( theIsHighlight ) 
    {}
    //! To calculate the functor
    void operator()(TActor* theActor) 
    {
      if(theActor->GetVisibility() && theActor->GetMapper())
	theActor->highlight( myIsHighlight );
    }
  };

}


#endif
