#pragma once

#include "MultiScan.h"
#include "Patterns.h"

class DMA;

class GTA5
{
public:
	static inline MultiScan m_Scan;
	static inline uintptr_t m_WorldPtr = 0x0;

	static inline uintptr_t m_WorldAddr = 0x0;
	static inline uintptr_t m_LocalPEDAddr = 0x0;

public:
	static bool FindPointers(DMA* dma);
	static bool UpdateWorldAddress(DMA* dma);
	static bool UpdateLocalPlayer(DMA* dma);

private:
	static bool m_FindWorldPtr(DMA* dma);

};