#ifndef RELAYX_COLLECTIONS_PROTECTION
#define RELAYX_COLLECTIONS_PROTECTION

#include <stdint.h>

///////////////////////////////////////////////////////////////////////////////////
// --------------------------->>> Canary Protection <<<----------------------------
#ifdef COLLECTIONS_CANARY_PROTECT

typedef uint64_t CCanary;
const CCanary CANARY_VALUE = 0xFEEDC0FFEE2FACE;

CCanary* GetStartCanary(const char* data) {
  return (CCanary*)(data - sizeof(CCanary));
}

CCanary* GetEndCanary(const char* data, size_t dataSize) {
  return (CCanary*)(data + dataSize);
}

#endif // COLLECTIONS_CANARY_PROTECT ----------------------------------------------
///////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////
// --------------------------->>> HashSum Protection <<<---------------------------
#ifdef COLLECTIONS_HASH_PROTECT

typedef uint64_t Chash;

Chash HashSum(const char* data, size_t dataSize) {
  Chash hash = 5381;

  for (size_t i = 0; i < dataSize; ++i) {
    hash += (hash << 5) + data[i];
  }

  return hash;
}

#endif // COLLECTIONS_HASH_PROTECT ------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////

#endif // RELAYX_COLLECTIONS_PROTECTION
