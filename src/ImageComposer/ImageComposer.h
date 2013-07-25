
#ifdef WIN32

  #if defined IMAGE_COMPOSER_EXPORTS || defined ImageComposer_EXPORTS
    #define IMAGE_COMPOSER_API __declspec( dllexport )
  #else
    #define IMAGE_COMPOSER_API __declspec( dllimport )
  #endif

#else
   #define IMAGE_COMPOSER_API 
#endif
