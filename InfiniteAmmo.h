#pragma once

#include "DMA.h"

class InfiniteAmmo
{
public:
	static bool OnFrame(DMA* dma);

public:
	static inline bool m_InfiniteAmmo = false;

};