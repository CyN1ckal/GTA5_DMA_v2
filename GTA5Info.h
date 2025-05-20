#pragma once

#include "MultiScan.h"
#include "Patterns.h"
#include "MyVector.h"
#include "WeaponInfo.h"

class DMA;

struct BlipInfo
{
	Vector3 WorldPosition;
	int32_t ID;
};

class GTA5
{
public:
	static inline MultiScan m_Scan;
	static inline uintptr_t m_WorldPtr = 0x0;
	static inline uintptr_t m_BlipPtr = 0x0;

	static inline uintptr_t m_WorldAddr = 0x0;
	static inline uintptr_t m_LocalPEDAddr = 0x0;
	static inline uintptr_t m_LocalPED_WeaponInventoryAddr = 0x0;
	static inline uintptr_t m_LocalPED_PlayerInfoAddr = 0x0;
	static inline uintptr_t m_LocalPED_VehicleAddr = 0x0;
	static inline uintptr_t m_LocalPED_NavigationAddr = 0x0;
	static inline uintptr_t m_LocalPED_WeaponManagerAddr = 0x0;
	static inline uintptr_t m_LocalPED_WeaponInfoAddr = 0x0;

	static inline float m_LocalPED_CurrentHealth = 0.0f;
	static inline float m_LocalPED_MaxHealth = 0.0f;
	static inline uint32_t m_LocalPED_GodModeBits = 0x0;
	static inline uint32_t m_LocalPED_VehicleGodModeBits = 0x0;
	static inline uint32_t m_LocalPED_AmmoModifierBits = 0x0;
	static inline int32_t m_LocalPED_WantedLevel = 0x0;
	static inline Vector3 m_LocalPED_Location = { 0.0f, 0.0f, 0.0f };

	static inline WeaponInfo m_LocalPed_WeaponInfo;

	static inline std::vector<BlipInfo> m_Blips;

public:
	static bool FindOffsets(DMA* dma);
	static bool UpdateWorldAddress(DMA* dma);
	static bool UpdateLocalPlayerAddr(DMA* dma);
	static bool UpdateLocalPlayerInfo(DMA* dma);
	static bool UpdateWeaponInfo(DMA* dma);
	static bool UpdateBlips(DMA* dma);
	static bool FeatureLoop(DMA* dma);

	static Vector3 GetWaypointLocation();

private:
	static bool m_FindWorldPtr(DMA* dma);
	static bool m_FindBlipPtr(DMA* dma);
	static bool m_FindGodBitsOffset(DMA* dma);
	static bool m_FindHealthOffset(DMA* dma);
	static bool m_FindWeaponInventoryOffset(DMA* dma);
	static bool m_FindAmmoModifierOffset(DMA* dma);
	static bool m_FindPlayerInfoOffset(DMA* dma);
	static bool m_FindWantedLevelOffset(DMA* dma);
	static bool m_FindVehicleGodBitsOffset(DMA* dma);
	static bool m_FindNavigationOffset(DMA* dma);
	static bool m_FindPlayerPositionOffset(DMA* dma);
	static bool m_FindBlipPositionOffset(DMA* dma);
	static bool m_FindBlipIDOffset(DMA* dma);
	static bool m_FindWeaponManagerOffset(DMA* dma);
	static bool m_FindWeaponInfoOffset(DMA* dma);
	static bool m_FindWeaponNameOffset(DMA* dma);
	static bool m_FindWeaponImpactOffsets(DMA* dma);
	static bool m_FindWeaponDamageOffset(DMA* dma);
	static bool m_FindWeaponPenetrationOffset(DMA* dma);
	static bool m_FindWeaponReloadMultiplierOffset(DMA* dma);
	static bool m_FindWeaponFireRateOffset(DMA* dma);
	static bool m_FindWeaponRecoilAmplitudeOffset(DMA* dma);
};