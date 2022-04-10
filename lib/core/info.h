#ifndef RELAYX_COLLECTIONS_INFO
#define RELAYX_COLLECTIONS_INFO

#include <stdlib.h>

typedef struct {
  const char*  object_type;
  const char*  variable_name;
  const char*  file;
  const char*  function;
  const size_t line;
} CInfo;

CInfo CInfoCtor(const char* type, 
                const char* name, 
                const char* file,
                const char* function, 
                size_t line
) {
  CInfo info = {type, name, file, function, line};
  return info;
}

#define GET_CINFO(TYPE, COLLECTION) CInfoCtor(#TYPE, #COLLECTION, __FILE__, __PRETTY_FUNCTION__, __LINE__)

#endif // RELAYX_COLLECTIONS_INFO
