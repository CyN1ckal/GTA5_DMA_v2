#pragma once
#include "../../Structs/VehicleInfo.h"
#include "../../Core/DMA/DMA.h"

class VehicleEditor
{
public:
	static inline bool m_VehicleEditor = false;
	static inline bool m_RequestedOverride = false;
	static inline VehicleInfo m_VehicleInfo;

	static inline ImVec4 m_DesiredPrimaryColor = { 0.0f,0.0f,0.0f,0.0f };
	static inline ImVec4 m_DesiredSecondaryColor = { 0.0f,0.0f,0.0f,0.0f };
	static inline ImVec4 m_DesiredWheelColor = { 0.0f,0.0f,0.0f,0.0f };
public:
	static bool OnFrame();
	static bool OnDMAFrame(Mem* dma);
};