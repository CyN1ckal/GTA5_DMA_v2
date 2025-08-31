#pragma once

#include "DMA/DMA.h"

class InfiniteAmmo
{
public:
	static bool OnFrame(Mem* dma);

public:
	static inline bool m_InfiniteAmmo = false;

};