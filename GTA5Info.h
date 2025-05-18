#pragma once

#include "MultiScan.h"
#include "Patterns.h"

class DMA;

class GTA5
{
public:
	static inline uintptr_t m_WorldPtr = 0x0;
	static inline MultiScan m_Scan;

public:
	static bool FindPointers(DMA* dma);

private:
	static bool m_FindWorldPtr(DMA* dma);

};