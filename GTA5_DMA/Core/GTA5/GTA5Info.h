#pragma once

#include "../../Core/DMA/MultiScan.h"
#include "../../Structs/Patterns.h"
#include "../../Structs/MyVector.h"
#include "../../Structs/WeaponInfo.h"
#include "../../Structs/VehicleInfo.h"
#include "../../Structs/PlayerInfo.h"
#include "../../Structs/BlipInfo.h"

#include "GTA5_DMA/Core/DMA/DMA.h"

namespace GTA5_
{
	Mem* Initialize();
	bool Close();

	inline Mem g_mem;

	namespace Details
	{
		const inline std::string m_ProcessName = "GTA5_Enhanced.exe";
	}
}



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
	static inline uintptr_t m_LocalPED_VehicleModsAddr = 0x0;
	static inline uintptr_t m_LocalPED_VehicleModelInfoAddr = 0x0;
	static inline uintptr_t m_LocalPED_NavigationAddr = 0x0;
	static inline uintptr_t m_LocalPED_WeaponManagerAddr = 0x0;
	static inline uintptr_t m_LocalPED_WeaponInfoAddr = 0x0;

	static inline PlayerInfo m_LocalPED_PlayerInfo;
	static inline WeaponInfo m_LocalPED_WeaponInfo;
	static inline VehicleInfo m_LocalPED_VehicleInfo;

	static inline std::vector<BlipInfo> m_Blips;

public:
	static bool FindOffsets(Mem* dma);
	static bool UpdateWorldAddress(Mem* dma);
	static bool UpdateLocalPlayerAddr(Mem* dma);
	static bool UpdateLocalPlayerInfo(Mem* dma);
	static bool UpdateWeaponInfo(Mem* dma);
	static bool UpdateVehicleInfo(Mem* dma);
	static bool UpdateBlips(Mem* dma);
	static bool FeatureLoop(Mem* dma);

	static Vector3 GetWaypointLocation();

private:
	static bool m_FindWorldPtr(Mem* dma);
	static bool m_FindBlipPtr(Mem* dma);

	static bool m_FindHealthOffset(Mem* dma);
	static bool m_FindWeaponImpactOffsets(Mem* dma);
	static bool m_FindVehicleColorOffsets(Mem* dma);
	static bool m_FindVehicleHealthOffsets(Mem* dma);
	static bool m_FindWeaponRecoilAmplitudeOffset(Mem* dma);

	static bool m_FindOffset_Disp1(Mem* dma, PatternInfo& pi, uint32_t& Offset, std::string Name);
	static bool m_FindOffset_Disp0(Mem* dma, PatternInfo& pi, uint32_t& Offset, std::string Name);
};