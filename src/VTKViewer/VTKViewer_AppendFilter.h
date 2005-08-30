#ifndef VTKVIEWER_APPENDFILTER_H
#define VTKVIEWER_APPENDFILTER_H

#include "VTKViewer.h"

#include <vtkAppendFilter.h>

#include <vector>
//pkv f
#include <map>
using namespace std;
//pkv t

/*! \brief This class used same as vtkAppendFilter. See documentation on VTK for more information.
 */
class VTKVIEWER_EXPORT VTKViewer_AppendFilter : public vtkAppendFilter 
{
public:
  /*! \fn static VTKViewer_AppendFilter *New()
   */
  static VTKViewer_AppendFilter *New();
  
  /*! \fn vtkTypeRevisionMacro(VTKViewer_AppendFilter, vtkAppendFilter)
   *  \brief VTK type revision macros.
   */
  vtkTypeRevisionMacro(VTKViewer_AppendFilter, vtkAppendFilter);

  void SetDoMappingFlag(const bool theFlag);

  bool DoMappingFlag() const;

  int GetElemObjId(int theVtkI, 
		   int& theInputIndex);

  int GetNodeObjId(int theVtkID, 
		   int& theInputIndex);

  //pkv f
  int GetElemVtkID(int theObjID);

  int GetNodeVtkID(int theObjID);
  //pkv t
  //
protected:
  /*! \fn VTKViewer_AppendFilter();
   * \brief Constructor
   */
  VTKViewer_AppendFilter();
  /*! \fn ~VTKViewer_AppendFilter();
   * \brief Destructor.
   */
  ~VTKViewer_AppendFilter();
  /*! \fn void Execute();
   * \brief Filter culculation method.
   */
  virtual void Execute();
  //
  void DoMapping();

  void Reset();
  //
private:
  typedef std::vector<vtkIdType> TVectorId;
  typedef std::vector<int> VectorInt;
  //pkv f
  typedef std::map <int , int, less<int> >             DataMapOfIntegerInteger;
  typedef std::map < int, int, std::less<int> >::iterator   IteratorOfDataMapOfIntegerInteger;
  typedef std::map < int, int, std::less<int> >::value_type PairOfDataMapOfIntegerInteger;
  //pkv t
private:
  bool      myDoMappingFlag;
  TVectorId myNodeIds;
  TVectorId myCellIds;
  VectorInt myNodeRanges;
  VectorInt myCellRanges;
  //pkv f
  DataMapOfIntegerInteger myNodeMapObjIDVtkID;
  DataMapOfIntegerInteger myCellMapObjIDVtkID;
  //pkv t
};

#endif
