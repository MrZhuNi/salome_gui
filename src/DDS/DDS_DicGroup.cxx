// Copyright (C) 2005  CEA/DEN, EDF R&D, OPEN CASCADE, PRINCIPIA R&D
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
#include "DDS_DicGroup.h"

#include "DDS_Dictionary.h"

#include <LDOMString.hxx>
#include <LDOM_Element.hxx>

#include <UnitsAPI.hxx>

#include <Standard_Failure.hxx>
#include <Standard_ErrorHandler.hxx>

#include <stdio.h>

IMPLEMENT_STANDARD_HANDLE(DDS_DicGroup, MMgt_TShared)
IMPLEMENT_STANDARD_RTTIEXT(DDS_DicGroup, MMgt_TShared)

/*!
  \class DDS_DicGroup
  \brief This class provides a set of DDS_DicItem objects from one component.
*/

/*!
  \brief Constructor.

  Create the group with name \a name.

  \param name group name
*/
DDS_DicGroup::DDS_DicGroup( const TCollection_AsciiString& name )
: MMgt_TShared(),
  myName( name ),
  myActiveSystem( UNIT_SYSTEM_SI )
{
}

/*!
  \brief Copy constructor (put in private section to prevent object copying).
*/
DDS_DicGroup::DDS_DicGroup( const DDS_DicGroup& )
{
}

/*!
  \brief Get the name of group (component).
  \return group name
*/
TCollection_AsciiString DDS_DicGroup::GetName() const
{
  return myName;
}

/*!
  \brief Get the names of all defined units systems.
  \param theSystemsSeq returning sequence of names
*/
void DDS_DicGroup::GetUnitSystems( TColStd_SequenceOfAsciiString& theSystemSeq ) const
{
  theSystemSeq.Clear();
  for ( UnitSystemMap::Iterator it( myUnitSystem ); it.More(); it.Next() )
  {
    if ( it.Key() == TCollection_AsciiString( UNIT_SYSTEM_SI ) )
      theSystemSeq.Prepend( it.Key() );
    else
      theSystemSeq.Append( it.Key() );
  }
}

/*!
  \brief Get the label of units system \a name.
  
  If units system is not found, empty string is returned.

  \param make units system name
  \return units system label
*/
TCollection_ExtendedString DDS_DicGroup::GetUnitSystemLabel( const TCollection_AsciiString& name ) const
{
  TCollection_ExtendedString aLabel;
  if ( myUnitSystem.IsBound( name ) )
    aLabel = myUnitSystem.Find( name );
  return aLabel;
}

/*!
  \brief Get the name of active units system.
  \return active units system name
*/
TCollection_AsciiString DDS_DicGroup::GetActiveUnitSystem() const
{
  return myActiveSystem;
}

/*!
  \brief Set the active unit system.
  \param theSystem name of the units system to be made active
*/
void DDS_DicGroup::SetActiveUnitSystem( const TCollection_AsciiString& theSystem )
{
  if ( myUnitSystem.IsBound( theSystem ) )
    myActiveSystem = theSystem;
}

/*!
  \brief Assignment operator (put in private section to prevent object copying).
*/
void DDS_DicGroup::operator=( const DDS_DicGroup& )
{
}

/*!
  \brief Fill the internal data structures from XML parsed structures.
  \param theComponentData component data DOM node
  \param theDocElement document element DOM node
*/
void DDS_DicGroup::FillDataMap( const LDOM_Element& theComponentData, const LDOM_Element& theDocElement )
{
  TCollection_AsciiString aCompName = theComponentData.getAttribute( DDS_Dictionary::KeyWord( "COMPONENT_NAME" ) );

  LDOM_Element systems = theComponentData.GetChildByTagName( DDS_Dictionary::KeyWord( "UNIT_SYSTEMS" ) );
  if ( !systems.isNull() )
  {
    LDOM_NodeList systemList = systems.getElementsByTagName( DDS_Dictionary::KeyWord( "UNIT_SYSTEM" ) );
    for ( Standard_Integer i = 0; i < systemList.getLength(); i++ )
    {
      //const LDOM_Element& aSystem = (const LDOM_Element &)systemList.item( i );
      LDOM_Node aNode = systemList.item( i );
      const LDOM_Element& anElem = (const LDOM_Element&) aNode;
      LDOM_Element aSystem(anElem);
      TCollection_AsciiString aName = aSystem.getAttribute( DDS_Dictionary::KeyWord( "UNIT_SYSTEM_NAME" ) );
      TCollection_ExtendedString aLabel = aSystem.getAttribute( DDS_Dictionary::KeyWord( "UNIT_SYSTEM_LABEL" ) );

      if ( aName.IsEmpty() )
        continue;

      if ( !myUnitSystem.IsBound( aName ) )
        myUnitSystem.Bind( aName, aLabel );


    }
  }

  if ( !myUnitSystem.IsBound( UNIT_SYSTEM_SI ) )
  {
    printf( "Warning: Mandatory unit system SI not defined in component: \"%s\". Added automaticaly", aCompName.ToCString() );
    myUnitSystem.Bind( UNIT_SYSTEM_SI, TCollection_ExtendedString( "System international" ) );
  }

  TColStd_SequenceOfAsciiString unitSystems;
  GetUnitSystems( unitSystems );

  LDOM_NodeList aData = theComponentData.getElementsByTagName( DDS_Dictionary::KeyWord( "DATUM" ) );
  if ( !aData.getLength() )
    return;

  for ( Standard_Integer i = 0; i < aData.getLength(); i++ )
  {
    //LDOM_Element aQuantity = (const LDOM_Element&)aData.item( i );
    LDOM_Node aNode = aData.item( i );
    const LDOM_Element& anElem = (const LDOM_Element&) aNode;
    LDOM_Element aQuantity(anElem);

    // 1. Attributes (id,label,units?,format?,required?)
    TCollection_AsciiString anID = aQuantity.getAttribute( DDS_Dictionary::KeyWord( "DATUM_ID" ) );
    Handle(DDS_DicItem) aDicItem = new DDS_DicItem();

    aDicItem->myComponent = this;
    aDicItem->FillDataMap( anID, aQuantity, theComponentData, theDocElement, unitSystems );
    myDataMap.Add( anID, aDicItem );

    bool exist = false;
    for( int i=1, n=myKeys.Length(); i<=n && !exist; i++ )
      if( myKeys.Value( i )==anID )
      {
#ifdef DEB
        cout << "Doubled key:" << anID << endl;
#endif
        exist = true;
      }
    if( !exist )
      myKeys.Append( anID );
  }
}

/*!
  \brief Get the dictionary item with specified identifier \a theID.

  If dictionary item is not found, null handle is returned.

  \param theID item identifier
  \return dictionary item
*/
Handle_DDS_DicItem DDS_DicGroup::GetDicItem( const TCollection_AsciiString& theID ) const
{
  Handle(DDS_DicItem) aDicItem;
  // get dictionary item by id
  if ( myDataMap.Contains( theID ) )
    aDicItem = myDataMap.FindFromKey( theID );

  return aDicItem;
}

/*!
  \brief Return all keys of the group
  \param seq - string container to be filled with keys
*/
void DDS_DicGroup::GetKeys( TColStd_SequenceOfAsciiString& seq ) const
{
  seq = myKeys;
}
