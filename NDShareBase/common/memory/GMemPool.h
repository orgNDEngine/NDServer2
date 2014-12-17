#ifndef _GMEM_POOL_H_
#define _GMEM_POOL_H_

extern "C"  _declspec(dllimport) typedef void GMemPool;

extern "C"  _declspec(dllimport) void GInitMemoryDLL();

extern "C"  _declspec(dllimport) void GCloseMemoryDLL();

extern "C"  _declspec(dllimport) GMemPool *GNewMemory();

extern "C"  _declspec(dllimport) void GDestroyMemory(GMemPool *pPool);

extern "C"  _declspec(dllimport) void* GMalloc(GMemPool *pPool, int size);

extern "C"  _declspec(dllimport) void GClearPool(GMemPool *pPool);
#endif //_GMEM_POOL_H_
