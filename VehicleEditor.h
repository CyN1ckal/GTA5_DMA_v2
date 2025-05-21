#pragma once
#include "VehicleInfo.h"
#include "DMA.h"

class VehicleEditor
{
public:
	static inline bool m_VehicleEditor = false;
	static inline bool m_RequestedOverride = false;
	static inline VehicleInfo m_VehicleInfo;
public:
	static bool OnFrame();
	static bool OnDMAFrame(DMA* dma);
};