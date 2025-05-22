#pragma once

#include "../../Core/DMA/DMA.h"

class GodMode
{
public:
	static inline bool m_GodMode = false;
	
public:
	static bool OnFrame(DMA* dma);
};