#pragma once

#include "MultiScan.h"
#include "Patterns.h"
#include "MyVector.h"
#include "WeaponInfo.h"
#include "VehicleInfo.h"
#include "PlayerInfo.h"
#include "BlipInfo.h"

class DMA;

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
	static inline uintptr_t m_LocalPED_VehicleNavigationAddr = 0x0;
	static inline uintptr_t m_LocalPED_VehicleHandlingAddr = 0x0;
	static inline uintptr_t m_LocalPED_NavigationAddr = 0x0;
	static inline uintptr_t m_LocalPED_WeaponManagerAddr = 0x0;
	static inline uintptr_t m_LocalPED_WeaponInfoAddr = 0x0;

	static inline PlayerInfo m_LocalPED_PlayerInfo;
	static inline WeaponInfo m_LocalPED_WeaponInfo;
	static inline VehicleInfo m_LocalPED_VehicleInfo;

	static inline std::vector<BlipInfo> m_Blips;

public:
	static bool FindOffsets(DMA* dma);
	static bool UpdateWorldAddress(DMA* dma);
	static bool UpdateLocalPlayerAddr(DMA* dma);
	static bool UpdateLocalPlayerInfo(DMA* dma);
	static bool UpdateWeaponInfo(DMA* dma);
	static bool UpdateVehicleInfo(DMA* dma);
	static bool UpdateBlips(DMA* dma);
	static bool FeatureLoop(DMA* dma);

	static Vector3 GetWaypointLocation();

private:
	static bool m_FindWorldPtr(DMA* dma);
	static bool m_FindBlipPtr(DMA* dma);
	static bool m_FindHealthOffset(DMA* dma);
	static bool m_FindAmmoModifierOffset(DMA* dma);
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
	static bool m_FindVehicleNavigationOffset(DMA* dma);
	static bool m_FindVehiclePositionOffset(DMA* dma);
	static bool m_FindVehicleHealthOffsets(DMA* dma);
	static bool m_FindWeaponRangeOffset(DMA* dma);
	static bool m_FindVehicleHandlingDataOffset(DMA* dma);
	static bool m_FindVehicleAccelerationOffset(DMA* dma);
	static bool m_FindVehicleMassOffset(DMA* dma);
	static bool m_FindVehicleBrakeForceOffset(DMA* dma);
	static bool m_FindVehicleDeformOffset(DMA* dma);

	static bool m_FindOffset_Disp1(DMA* dma, PatternInfo& pi, uint32_t& Offset, std::string Name);
	static bool m_FindOffset_Disp0(DMA* dma, PatternInfo& pi, uint32_t& Offset, std::string Name);
};