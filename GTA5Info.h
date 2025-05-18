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

	static inline float m_LocalPED_CurrentHealth = 0.0f;
	static inline float m_LocalPED_MaxHealth = 0.0f;

public:
	static bool FindPointers(DMA* dma);
	static bool UpdateWorldAddress(DMA* dma);
	static bool UpdateLocalPlayerAddr(DMA* dma);
	static bool UpdateLocalPlayerInfo(DMA* dma);
	static bool FeatureLoop(DMA* dma);

private:
	static bool m_FindWorldPtr(DMA* dma);
};