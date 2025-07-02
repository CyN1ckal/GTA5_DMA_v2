#pragma once

#include "../../Core/DMA/DMA.h"

class InfiniteAmmo
{
public:
	static bool OnFrame(Mem* dma);

public:
	static inline bool m_InfiniteAmmo = false;

};