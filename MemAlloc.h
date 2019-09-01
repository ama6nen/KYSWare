#pragma once
#include "Globals.h"
class IMemAlloc
{
public:
	void* IMemAlloc::Alloc(size_t nSize)
	{
		typedef void*(__thiscall* OriginalFn)(void*, int);
		return CallVFunction<OriginalFn>(this, 1)(this, nSize);
	}
	void* IMemAlloc::Realloc(void *pMem, size_t nSize)
	{
		typedef void*(__thiscall* OriginalFn)(void*, void*, int);
		return CallVFunction<OriginalFn>(this, 3)(this, pMem, nSize);
	}
	void IMemAlloc::Free(void *pMem)
	{
		
		typedef void(__thiscall* OriginalFn)(void*, void*);
		return CallVFunction<OriginalFn>(this, 5)(this, pMem);
	}
};