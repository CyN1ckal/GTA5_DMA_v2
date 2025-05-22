#pragma once

#include "../../Structs/WeaponInfo.h"

class WeaponEditor
{
public:
	static bool OnFrame();
	static bool OnDMAFrame(DMA* dma);
	
public:
	static inline WeaponInfo m_DesiredWeaponInfo;
	static inline bool m_RequestedOverride = false;
	static inline bool m_WeaponEditor = false;
};