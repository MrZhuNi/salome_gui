#ifndef LIGHTAPP_STUDY_H
#define LIGHTAPP_STUDY_H

#include "LightApp.h"

#include "CAM_Study.h"
#include "SUIT_Study.h"

#include "string.h"
#include "vector.h"

#ifdef WIN32
#pragma warning( disable:4251 )
#endif

class SUIT_Application;
class CAM_DataModel;

class LIGHTAPP_EXPORT LightApp_Study : public CAM_Study
{
  Q_OBJECT

public:
  LightApp_Study( SUIT_Application* );
  virtual ~LightApp_Study();

  virtual void        createDocument();
  virtual bool        openDocument( const QString& );

  virtual void        saveDocument();
  virtual bool        saveDocumentAs( const QString& );
  virtual bool        loadDocument( const QString& ); 

  virtual void        closeDocument(bool permanently = true);

  virtual bool        isSaved()  const;
  virtual bool        isModified() const;

  /** @name methods to be used by  modules*/
  //@{
  virtual std::vector<std::string> GetListOfFiles () const;
  virtual void                     SetListOfFiles (const std::vector<std::string> theListOfFiles);

  virtual std::string GetTmpDir (const char* theURL,
                                  const bool  isMultiFile);

  virtual void        RemoveTemporaryFiles (const bool isMultiFile) const;
  //@}
  // END: methods to be used by  modules

signals:
  void                saved( SUIT_Study* );
  void                opened( SUIT_Study* );
  void                closed( SUIT_Study* );
  void                created( SUIT_Study* );
};

#endif 
