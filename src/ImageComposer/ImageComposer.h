
#ifdef WIN32

  #ifdef IMAGE_COMPOSER_EXPORTS
    #define IMAGE_COMPOSER_API __declspec( dllexport )
  #else
    #define IMAGE_COMPOSER_API __declspec( dllimport )
  #endif

#else
   #define IMAGE_COMPOSER_API 
#endif
