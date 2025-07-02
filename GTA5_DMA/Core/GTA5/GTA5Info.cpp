#include "../../pch.h"

#include "GTA5Info.h"

#include "../DMA/DMA.h"

#include "Zydis/Zydis.h"

#include "../../Features/Features.h"

#include "../../Structs/Offsets.h"

#include "../../Structs/VehicleInfo.h"

bool GTA5::m_FindWorldPtr(Mem* dma)
{
	PatternInfo pi;
	pi.ModuleName = GTA5_::Details::m_ProcessName;
	pi.Pattern = Patterns::WorldPattern;
	pi.Mask = Patterns::WorldMask;

	auto SectionOffset = m_Scan.ScanSectionOffset(pi);
	auto RuntimeAddress = m_Scan.Scan(pi);

	if (!SectionOffset || !RuntimeAddress)
		throw std::exception("m_FindWorldPtr Offset");

	ZydisDisassembledInstruction Instruction;

	if (!ZYAN_SUCCESS(ZydisDisassembleIntel(ZYDIS_MACHINE_MODE_LONG_64, RuntimeAddress, m_Scan.GetBuffer() + SectionOffset, 0x15, &Instruction)))
	{
		std::println("m_FindWorldPtr Disassemble");
		return 0;
	}

	m_WorldPtr = Instruction.operands[1].mem.disp.value + RuntimeAddress + Instruction.info.length;

	std::println("[+] GTA5::m_WorldPtr {0:X}", m_WorldPtr);

	return 1;
}

bool GTA5::m_FindBlipPtr(Mem* dma)
{
	PatternInfo pi;
	pi.ModuleName = GTA5_::Details::m_ProcessName;
	pi.Pattern = Patterns::BlipPattern;
	pi.Mask = Patterns::BlipMask;

	auto SectionOffset = m_Scan.ScanSectionOffset(pi);
	auto RuntimeAddress = m_Scan.Scan(pi);

	if (!SectionOffset || !RuntimeAddress)
		throw std::exception("m_FindBlipPtr Offset");

	ZydisDisassembledInstruction Instruction;

	if (!ZYAN_SUCCESS(ZydisDisassembleIntel(ZYDIS_MACHINE_MODE_LONG_64, RuntimeAddress, m_Scan.GetBuffer() + SectionOffset, 0x15, &Instruction)))
	{
		std::println("m_FindBlipPtr Disassemble");
		return 0;
	}

	m_BlipPtr = Instruction.operands[1].mem.disp.value + RuntimeAddress + Instruction.info.length;

	std::println("[+] GTA5::m_BlipPtr {0:X}", m_BlipPtr);

	return 1;
}

bool GTA5::FindOffsets(Mem* dma)
{
	m_Scan.Initialize(dma->m_vmh, dma->m_PID, GTA5_::Details::m_ProcessName);

	PatternInfo pi;
	pi.ModuleName = GTA5_::Details::m_ProcessName;

	pi.Pattern = Patterns::WeaponInventoryPattern;
	pi.Mask = Patterns::WeaponInventoryMask;
	m_FindOffset_Disp1(dma, pi, Offsets::WeaponInventory, "WeaponInventory");

	pi.Pattern = Patterns::PlayerInfoPattern;
	pi.Mask = Patterns::PlayerInfoMask;
	m_FindOffset_Disp0(dma, pi, Offsets::PlayerInfo, "PlayerInfo");

	pi.Pattern = Patterns::WantedLevelPattern;
	pi.Mask = Patterns::WantedLevelMask;
	m_FindOffset_Disp0(dma, pi, Offsets::WantedLevel, "WantedLevel");

	pi.Pattern = Patterns::GodBitsPattern;
	pi.Mask = Patterns::GodBitsMask;
	m_FindOffset_Disp0(dma, pi, Offsets::GodBits, "GodBits");

	pi.Pattern = Patterns::VehicleGodBitsPattern;
	pi.Mask = Patterns::VehicleGodBitsMask;
	m_FindOffset_Disp0(dma, pi, Offsets::VehicleGodBits, "VehicleGodBits");

	pi.Pattern = Patterns::AmmoModifierPattern;
	pi.Mask = Patterns::AmmoModifierMask;
	m_FindOffset_Disp0(dma, pi, Offsets::AmmoModifier, "AmmoModifier");

	pi.Pattern = Patterns::PlayerPositionPattern;
	pi.Mask = Patterns::PlayerPositionMask;
	m_FindOffset_Disp1(dma, pi, Offsets::PlayerPosition, "PlayerPosition");

	pi.Pattern = Patterns::NavigationPattern;
	pi.Mask = Patterns::NavigationMask;
	m_FindOffset_Disp1(dma, pi, Offsets::Navigation, "Navigation");

	pi.Pattern = Patterns::BlipPositionPattern;
	pi.Mask = Patterns::BlipPositionMask;
	m_FindOffset_Disp0(dma, pi, Offsets::BlipPosition, "BlipPosition");

	pi.Pattern = Patterns::BlipIDPattern;
	pi.Mask = Patterns::BlipIDMask;
	m_FindOffset_Disp1(dma, pi, Offsets::BlipID, "BlipID");

	pi.Pattern = Patterns::WeaponManagerPattern;
	pi.Mask = Patterns::WeaponManagerMask;
	m_FindOffset_Disp0(dma, pi, Offsets::WeaponManager, "WeaponManager");

	pi.Pattern = Patterns::WeaponInfoPattern;
	pi.Mask = Patterns::WeaponInfoMask;
	m_FindOffset_Disp1(dma, pi, Offsets::WeaponInfo, "WeaponInfo");

	pi.Pattern = Patterns::WeaponNamePattern;
	pi.Mask = Patterns::WeaponNameMask;
	m_FindOffset_Disp0(dma, pi, Offsets::WeaponName, "WeaponName");

	pi.Pattern = Patterns::WeaponDamagePattern;
	pi.Mask = Patterns::WeaponDamageMask;
	m_FindOffset_Disp1(dma, pi, Offsets::WeaponDamage, "WeaponDamage");

	pi.Pattern = Patterns::WeaponPenetrationPattern;
	pi.Mask = Patterns::WeaponPenetrationMask;
	m_FindOffset_Disp1(dma, pi, Offsets::WeaponPenetration, "WeaponPenetration");

	pi.Pattern = Patterns::WeaponReloadMultiplerPattern;
	pi.Mask = Patterns::WeaponReloadMultiplerMask;
	m_FindOffset_Disp1(dma, pi, Offsets::WeaponReloadMultiplier, "WeaponReloadMultiplier");

	pi.Pattern = Patterns::WeaponFireRatePattern;
	pi.Mask = Patterns::WeaponFireRateMask;
	m_FindOffset_Disp1(dma, pi, Offsets::WeaponFireRate, "WeaponFireRate");

	pi.Pattern = Patterns::VehicleNavigationPattern;
	pi.Mask = Patterns::VehicleNavigationMask;
	m_FindOffset_Disp1(dma, pi, Offsets::VehicleNavigation, "VehicleNavigation");

	pi.Pattern = Patterns::VehiclePositionPattern;
	pi.Mask = Patterns::VehiclePositionMask;
	m_FindOffset_Disp1(dma, pi, Offsets::VehiclePosition, "VehiclePosition");

	pi.Pattern = Patterns::WeaponRangePattern;
	pi.Mask = Patterns::WeaponRangeMask;
	m_FindOffset_Disp1(dma, pi, Offsets::WeaponRange, "WeaponRange");
	
	pi.Pattern = Patterns::VehicleHandlingPattern;
	pi.Mask = Patterns::VehicleHandlingMask;
	m_FindOffset_Disp1(dma, pi, Offsets::VehicleHandling, "VehicleHandling");
	
	pi.Pattern = Patterns::VehicleAccelerationPattern;
	pi.Mask = Patterns::VehicleAccelerationMask;
	m_FindOffset_Disp1(dma, pi, Offsets::VehicleAcceleration, "VehicleAcceleration");

	pi.Pattern = Patterns::VehicleMassPattern;
	pi.Mask = Patterns::VehicleMassMask;
	m_FindOffset_Disp1(dma, pi, Offsets::VehicleMass, "VehicleMass");
	
	pi.Pattern = Patterns::VehicleBrakeForcePattern;
	pi.Mask = Patterns::VehicleBrakeForceMask;
	m_FindOffset_Disp1(dma, pi, Offsets::BrakeForce, "BrakeForce");

	pi.Pattern = Patterns::VehicleDeformPattern;
	pi.Mask = Patterns::VehicleDeformMask;
	m_FindOffset_Disp1(dma, pi, Offsets::VehicleDeformMult, "VehicleDeformMult");

	pi.Pattern = Patterns::VehicleModsPattern;
	pi.Mask = Patterns::VehicleModsMask;
	m_FindOffset_Disp1(dma, pi, Offsets::VehicleMods, "VehicleMods");

	pi.Pattern = Patterns::VehicleModelInfoPattern;
	pi.Mask = Patterns::VehicleModelInfoMask;
	m_FindOffset_Disp1(dma, pi, Offsets::VehicleModelInfo, "VehicleModelInfo");

	m_FindVehicleColorOffsets(dma);

	/* resolves 2 offsets */
	m_FindWeaponImpactOffsets(dma);
	m_FindHealthOffset(dma);
	m_FindVehicleHealthOffsets(dma);

	/* immediate value; not displacement */
	m_FindWeaponRecoilAmplitudeOffset(dma);

	m_FindWorldPtr(dma);
	m_FindBlipPtr(dma);

	m_Scan.Close();

	return 1;
}

bool GTA5::UpdateLocalPlayerAddr(Mem* dma)
{
	ZoneScoped;

	if (!m_WorldAddr)
		return 0;

	auto vmsh = VMMDLL_Scatter_Initialize(dma->m_vmh, dma->m_PID, VMMDLL_FLAG_NOCACHE);

	DWORD BytesRead = 0x0;

	uintptr_t LocalPlayerPtr = m_WorldAddr + 0x8;
	uintptr_t LocalPlayerAddress = 0x0;
	VMMDLL_Scatter_PrepareEx(vmsh, LocalPlayerPtr, sizeof(uintptr_t), (BYTE*)&LocalPlayerAddress, &BytesRead);
	VMMDLL_Scatter_Execute(vmsh);

	if (BytesRead == sizeof(uintptr_t))
		m_LocalPEDAddr = LocalPlayerAddress;

	VMMDLL_Scatter_CloseHandle(vmsh);

	static uintptr_t PreviousLocalPedAddr = 0x0;
	if (PreviousLocalPedAddr != m_LocalPEDAddr)
	{
		std::println("[+] m_LocalPEDAddr {0:X}", m_LocalPEDAddr);
		PreviousLocalPedAddr = m_LocalPEDAddr;
	}

	return 1;
}

bool GTA5::UpdateWorldAddress(Mem* dma)
{
	ZoneScoped;

	if (!m_WorldPtr)
		return 0;

	DWORD BytesRead = 0x0;

	auto vmsh = VMMDLL_Scatter_Initialize(dma->m_vmh, dma->m_PID, VMMDLL_FLAG_NOCACHE);

	uintptr_t WorldAddress = 0x0;
	VMMDLL_Scatter_PrepareEx(vmsh, m_WorldPtr, sizeof(uintptr_t), (BYTE*)&WorldAddress, &BytesRead);
	VMMDLL_Scatter_Execute(vmsh);

	if (BytesRead == sizeof(uintptr_t))
		m_WorldAddr = WorldAddress;

	VMMDLL_Scatter_CloseHandle(vmsh);

	static uintptr_t PreviousWorldAddr = 0x0;
	if (PreviousWorldAddr != m_WorldAddr)
	{
		std::println("[+] m_WorldAddr {0:X}", m_WorldAddr);
		PreviousWorldAddr = m_WorldAddr;
	}

	return 1;
}

bool GTA5::UpdateLocalPlayerInfo(Mem* dma)
{
	ZoneScoped;

	if (!m_LocalPEDAddr)
	{
		UpdateLocalPlayerAddr(dma);
		return 0;
	}

	DWORD HealthBytes = 0x0;
	DWORD GodBytes = 0x0;
	DWORD WeaponInventoryBytes = 0x0;
	DWORD PlayerInfoBytes = 0x0;
	DWORD VehicleBytes = 0x0;
	DWORD NavigationBytes = 0x0;
	DWORD WeaponManagerBytes = 0x0;
	DWORD InVehicleBytes = 0x0;

	auto vmsh = VMMDLL_Scatter_Initialize(dma->m_vmh, dma->m_PID, VMMDLL_FLAG_NOCACHE);

	uintptr_t GodModeBitsAddress = m_LocalPEDAddr + Offsets::GodBits;
	uintptr_t CurrentHealthAddress = m_LocalPEDAddr + Offsets::CurrentHealth;
	uintptr_t WeaponInventoryPtr = m_LocalPEDAddr + Offsets::WeaponInventory;
	uintptr_t PlayerInfoPtr = m_LocalPEDAddr + Offsets::PlayerInfo;
	uintptr_t VehiclePtr = m_LocalPEDAddr + Offsets::VehicleOffset;
	uintptr_t NavigationPtr = m_LocalPEDAddr + Offsets::Navigation;
	uintptr_t WeaponManagerPtr = m_LocalPEDAddr + Offsets::WeaponManager;
	uintptr_t InVehicleBitsAddr = m_LocalPEDAddr + Offsets::InVehicleBits;

	struct Health
	{
		float CurrentHealth;
		float MaxHealth;
	};

	Health health;
	uint32_t GodModeBits = 0x0;
	uintptr_t WeaponInventoryAddr = 0x0;
	uintptr_t PlayerInfoAddr = 0x0;
	uintptr_t VehicleAddr = 0x0;
	uintptr_t NavigationAdd = 0x0;
	uintptr_t WeaponManagerAddr = 0x0;
	uint32_t InVehicleBits = 0x0;

	VMMDLL_Scatter_PrepareEx(vmsh, CurrentHealthAddress, sizeof(float) * 2, (BYTE*)&health, &HealthBytes);
	VMMDLL_Scatter_PrepareEx(vmsh, GodModeBitsAddress, sizeof(uint32_t), (BYTE*)&GodModeBits, &GodBytes);
	VMMDLL_Scatter_PrepareEx(vmsh, WeaponInventoryPtr, sizeof(uintptr_t), (BYTE*)&WeaponInventoryAddr, &WeaponInventoryBytes);
	VMMDLL_Scatter_PrepareEx(vmsh, PlayerInfoPtr, sizeof(uintptr_t), (BYTE*)&PlayerInfoAddr, &PlayerInfoBytes);
	VMMDLL_Scatter_PrepareEx(vmsh, VehiclePtr, sizeof(uintptr_t), (BYTE*)&VehicleAddr, &VehicleBytes);
	VMMDLL_Scatter_PrepareEx(vmsh, NavigationPtr, sizeof(uintptr_t), (BYTE*)&NavigationAdd, &NavigationBytes);
	VMMDLL_Scatter_PrepareEx(vmsh, WeaponManagerPtr, sizeof(uintptr_t), (BYTE*)&WeaponManagerAddr, &WeaponManagerBytes);
	VMMDLL_Scatter_PrepareEx(vmsh, InVehicleBitsAddr, sizeof(uint32_t), (BYTE*)&InVehicleBits, &InVehicleBytes);

	VMMDLL_Scatter_Execute(vmsh);

	if (HealthBytes == sizeof(float) * 2)
	{
		m_LocalPED_PlayerInfo.m_CurrentHealth = health.CurrentHealth;
		m_LocalPED_PlayerInfo.m_MaxHealth = health.MaxHealth;
	}

	if (GodBytes == sizeof(uint32_t))
	{
		m_LocalPED_PlayerInfo.m_GodModeBits = GodModeBits;
	}

	if (WeaponInventoryBytes == sizeof(uintptr_t))
	{
		m_LocalPED_WeaponInventoryAddr = WeaponInventoryAddr;
	}

	if (PlayerInfoBytes == sizeof(uintptr_t))
	{
		m_LocalPED_PlayerInfoAddr = PlayerInfoAddr;
	}

	if (VehicleBytes == sizeof(uintptr_t))
	{
		m_LocalPED_VehicleAddr = VehicleAddr;
	}

	if (NavigationBytes == sizeof(uintptr_t))
	{
		m_LocalPED_NavigationAddr = NavigationAdd;
	}

	if (WeaponManagerBytes == sizeof(uintptr_t))
	{
		m_LocalPED_WeaponManagerAddr = WeaponManagerAddr;
	}

	if (InVehicleBytes == sizeof(uint32_t))
	{
		m_LocalPED_PlayerInfo.m_InVehicleBits = InVehicleBits;
	}

	VMMDLL_Scatter_Clear(vmsh, dma->m_PID, VMMDLL_FLAG_NOCACHE);

	DWORD AmmoModifierBytes = 0x0;
	DWORD WantedLevelBytes = 0x0;
	DWORD PositionBytes = 0x0;
	DWORD WeaponAddrBytes = 0x0;

	uint32_t AmmoModifierBits = 0x0;
	int32_t WantedLevel = 0x0;
	Vector3 PlayerPosition = { 0.0f,0.0f,0.0f };
	uintptr_t WeaponInfoAddr = 0x0;

	uintptr_t AmmoModifierAddress = m_LocalPED_WeaponInventoryAddr + Offsets::AmmoModifier;
	uintptr_t WantedLevelAddress = m_LocalPED_PlayerInfoAddr + Offsets::WantedLevel;
	uintptr_t PlayerPositonAddress = m_LocalPED_NavigationAddr + Offsets::PlayerPosition;
	uintptr_t WeaponInfoAddress = m_LocalPED_WeaponManagerAddr + Offsets::WeaponInfo;

	if (m_LocalPED_WeaponInventoryAddr)
		VMMDLL_Scatter_PrepareEx(vmsh, AmmoModifierAddress, sizeof(uint32_t), (BYTE*)&AmmoModifierBits, &AmmoModifierBytes);

	if (m_LocalPED_PlayerInfoAddr)
		VMMDLL_Scatter_PrepareEx(vmsh, WantedLevelAddress, sizeof(int32_t), (BYTE*)&WantedLevel, &WantedLevelBytes);

	if (m_LocalPED_NavigationAddr)
		VMMDLL_Scatter_PrepareEx(vmsh, PlayerPositonAddress, sizeof(Vector3), (BYTE*)&PlayerPosition, &PositionBytes);

	if (m_LocalPED_WeaponManagerAddr)
		VMMDLL_Scatter_PrepareEx(vmsh, WeaponInfoAddress, sizeof(uintptr_t), (BYTE*)&WeaponInfoAddr, &WeaponAddrBytes);

	if (m_LocalPED_WeaponInventoryAddr || m_LocalPED_PlayerInfoAddr || m_LocalPED_NavigationAddr || m_LocalPED_VehicleAddr || m_LocalPED_WeaponManagerAddr)
		VMMDLL_Scatter_Execute(vmsh);

	if (AmmoModifierBytes == sizeof(uint32_t))
	{
		m_LocalPED_PlayerInfo.m_AmmoModifierBits = AmmoModifierBits;
	}

	if (WantedLevelBytes == sizeof(int32_t))
	{
		m_LocalPED_PlayerInfo.m_WantedLevel = WantedLevel;
	}
	if (PositionBytes == sizeof(Vector3))
	{
		m_LocalPED_PlayerInfo.m_Location.x = PlayerPosition.x;
		m_LocalPED_PlayerInfo.m_Location.y = PlayerPosition.y;
		m_LocalPED_PlayerInfo.m_Location.z = PlayerPosition.z;
	}

	if (WeaponAddrBytes == sizeof(uintptr_t))
	{
		m_LocalPED_WeaponInfoAddr = WeaponInfoAddr;
	}

	VMMDLL_Scatter_CloseHandle(vmsh);

	return 1;
}

bool GTA5::UpdateWeaponInfo(Mem* dma)
{
	ZoneScoped;

	if (!m_LocalPED_WeaponInfoAddr)
		return 0;

	auto vmsh = VMMDLL_Scatter_Initialize(dma->m_vmh, dma->m_PID, VMMDLL_FLAG_NOCACHE);

	uintptr_t WeaponNameAddr = m_LocalPED_WeaponInfoAddr + Offsets::WeaponName;
	uintptr_t ImpactAddr = m_LocalPED_WeaponInfoAddr + Offsets::ImpactType;
	uintptr_t DamageAddr = m_LocalPED_WeaponInfoAddr + Offsets::WeaponDamage;
	uintptr_t PenetrationAddr = m_LocalPED_WeaponInfoAddr + Offsets::WeaponPenetration;
	uintptr_t ReloadMultiplierAddr = m_LocalPED_WeaponInfoAddr + Offsets::WeaponReloadMultiplier;
	uintptr_t FireRateAddr = m_LocalPED_WeaponInfoAddr + Offsets::WeaponFireRate;
	uintptr_t RecoilAmplitudeAddr = m_LocalPED_WeaponInfoAddr + Offsets::WeaponRecoilAmplitude;
	uintptr_t RangeAddr = m_LocalPED_WeaponInfoAddr + Offsets::WeaponRange;

	struct Impact
	{
		int32_t Type;
		int32_t Explosion;
	};
	Impact impact;

	DWORD BytesRead = 0x0;
	DWORD ImpactBytesRead = 0x0;
	DWORD DamageBytes = 0x0;
	DWORD PenetrationBytes = 0x0;
	DWORD ReloadBytes = 0x0;
	DWORD FireRateBytes = 0x0;
	DWORD RecoilBytes = 0x0;
	DWORD RangeBytes = 0x0;

	uint32_t WeaponName = 0x0;
	float WeaponDamage = 0x0;
	float WeaponPenetration = 0x0;
	float ReloadMultiplier = 0x0;
	float FireRate = 0x0;
	float RecoilAmplitude = 0x0;
	float Range = 0x0;

	VMMDLL_Scatter_PrepareEx(vmsh, WeaponNameAddr, sizeof(uint32_t), (BYTE*)&WeaponName, &BytesRead);
	VMMDLL_Scatter_PrepareEx(vmsh, ImpactAddr, sizeof(Impact), (BYTE*)&impact, &ImpactBytesRead);
	VMMDLL_Scatter_PrepareEx(vmsh, DamageAddr, sizeof(float), (BYTE*)&WeaponDamage, &DamageBytes);
	VMMDLL_Scatter_PrepareEx(vmsh, PenetrationAddr, sizeof(float), (BYTE*)&WeaponPenetration, &PenetrationBytes);
	VMMDLL_Scatter_PrepareEx(vmsh, ReloadMultiplierAddr, sizeof(float), (BYTE*)&ReloadMultiplier, &ReloadBytes);
	VMMDLL_Scatter_PrepareEx(vmsh, FireRateAddr, sizeof(float), (BYTE*)&FireRate, &FireRateBytes);
	VMMDLL_Scatter_PrepareEx(vmsh, RecoilAmplitudeAddr, sizeof(float), (BYTE*)&RecoilAmplitude, &RecoilBytes);
	VMMDLL_Scatter_PrepareEx(vmsh, RangeAddr, sizeof(float), (BYTE*)&Range, &RangeBytes);

	VMMDLL_Scatter_Execute(vmsh);

	if (BytesRead == sizeof(uint32_t))
		m_LocalPED_WeaponInfo.m_WeaponName = WeaponName;

	if (ImpactBytesRead == sizeof(Impact))
	{
		m_LocalPED_WeaponInfo.m_ImpactType = impact.Type;
		m_LocalPED_WeaponInfo.m_ImpactExplosion = impact.Explosion;
	}

	if (DamageBytes == sizeof(float))
		m_LocalPED_WeaponInfo.m_WeaponDamage = WeaponDamage;

	if (PenetrationBytes == sizeof(float))
		m_LocalPED_WeaponInfo.m_WeaponPenetration = WeaponPenetration;

	if (ReloadBytes == sizeof(float))
		m_LocalPED_WeaponInfo.m_WeaponReloadMultiplier = ReloadMultiplier;

	if (FireRateBytes == sizeof(float))
		m_LocalPED_WeaponInfo.m_WeaponFireRate = FireRate;

	if (RecoilBytes == sizeof(float))
		m_LocalPED_WeaponInfo.m_WeaponRecoilAmplitude = RecoilAmplitude;

	if (RangeBytes == sizeof(float))
		m_LocalPED_WeaponInfo.m_WeaponRange = Range;


	{ /* Transfer data to weapon inspector with a mutex */
		std::scoped_lock WeaponInspectorLock(WeaponInspector::m_WeaponInspectorMutex);
		WeaponInspector::m_CurrentWeaponInfo = m_LocalPED_WeaponInfo;
	}

	VMMDLL_Scatter_CloseHandle(vmsh);

	return 1;
}

bool GTA5::UpdateVehicleInfo(Mem* dma)
{
	ZoneScoped;

	if (!m_LocalPED_VehicleAddr)
		return 0;

	DWORD BytesRead = 0x0;

	auto vmsh = VMMDLL_Scatter_Initialize(dma->m_vmh, dma->m_PID, VMMDLL_FLAG_NOCACHE);

	DWORD NavigationBytes = 0x0;
	DWORD GodBytes = 0x0;
	DWORD HealthBytes = 0x0;
	DWORD HandlingBytes = 0x0;
	DWORD ModsBytes = 0x0;

	uintptr_t NavigationPtr = m_LocalPED_VehicleAddr + Offsets::VehicleNavigation;
	uintptr_t GodBitsAddr = m_LocalPED_VehicleAddr + Offsets::VehicleGodBits;
	uintptr_t HealthAddr = m_LocalPED_VehicleAddr + Offsets::VehicleHealth;
	uintptr_t HandlingPtr = m_LocalPED_VehicleAddr + Offsets::VehicleHandling;
	uintptr_t ModsPtr = m_LocalPED_VehicleAddr + Offsets::VehicleMods;

	uintptr_t NavigationAddr = 0x0;
	uintptr_t HandlingAddr = 0x0;
	uintptr_t ModsAddr = 0x0;
	uint32_t GodBits = 0x0;
	struct Health
	{
		float Current;
		float Max;
	};
	Health health;

	VMMDLL_Scatter_PrepareEx(vmsh, NavigationPtr, sizeof(uintptr_t), (BYTE*)&NavigationAddr, &NavigationBytes);

	VMMDLL_Scatter_PrepareEx(vmsh, HandlingPtr, sizeof(uintptr_t), (BYTE*)&HandlingAddr, &HandlingBytes);

	VMMDLL_Scatter_PrepareEx(vmsh, ModsPtr, sizeof(uintptr_t), (BYTE*)&ModsAddr, &ModsBytes);

	VMMDLL_Scatter_PrepareEx(vmsh, GodBitsAddr, sizeof(uint32_t), (BYTE*)&GodBits, &GodBytes);

	VMMDLL_Scatter_PrepareEx(vmsh, HealthAddr, sizeof(Health), (BYTE*)&health, &HealthBytes);

	VMMDLL_Scatter_Execute(vmsh);

	if (NavigationBytes == sizeof(uintptr_t))
		m_LocalPED_VehicleNavigationAddr = NavigationAddr;

	if (ModsBytes == sizeof(uintptr_t))
		m_LocalPED_VehicleModsAddr = ModsAddr;

	if (GodBytes == sizeof(uint32_t))
		m_LocalPED_VehicleInfo.m_GodBits = GodBits;

	if (HealthBytes == sizeof(Health))
	{
		m_LocalPED_VehicleInfo.m_Health = health.Current;
		m_LocalPED_VehicleInfo.m_MaxHealth = health.Max;
	}

	if (HandlingBytes == sizeof(uintptr_t))
		m_LocalPED_VehicleHandlingAddr = HandlingAddr;

	VMMDLL_Scatter_Clear(vmsh, dma->m_PID, VMMDLL_FLAG_NOCACHE);

	DWORD PositionBytes = 0x0;
	Vector3 Position = { 0.0f,0.0f,0.0f };
	if (m_LocalPED_VehicleNavigationAddr)
	{
		uintptr_t PositionAddress = m_LocalPED_VehicleNavigationAddr + Offsets::VehiclePosition;
		VMMDLL_Scatter_PrepareEx(vmsh, PositionAddress, sizeof(Vector3), (BYTE*)&Position, &PositionBytes);
	}

	DWORD AccelerationBytes = 0x0;
	float Acceleration = 0.0f;
	DWORD MassBytes = 0x0;
	float Mass = 0.0f;
	DWORD BrakeForceBytes = 0x0;
	float BrakeForce = 0.0f;
	DWORD DeformBytes = 0x0;
	float Deform = 0.0f;
	uintptr_t ModelInfoAddr = 0x0;
	DWORD ModelInfoBytes = 0x0;

	if (m_LocalPED_VehicleHandlingAddr)
	{
		uintptr_t AccelerationAddress = m_LocalPED_VehicleHandlingAddr + Offsets::VehicleAcceleration;
		uintptr_t MassAddr = m_LocalPED_VehicleHandlingAddr + Offsets::VehicleMass;
		uintptr_t BrakeForceAddr = m_LocalPED_VehicleHandlingAddr + Offsets::BrakeForce;
		uintptr_t DeformAddr = m_LocalPED_VehicleHandlingAddr + Offsets::VehicleDeformMult;
		VMMDLL_Scatter_PrepareEx(vmsh, AccelerationAddress, sizeof(float), (BYTE*)&Acceleration, &AccelerationBytes);
		VMMDLL_Scatter_PrepareEx(vmsh, MassAddr, sizeof(float), (BYTE*)&Mass, &MassBytes);
		VMMDLL_Scatter_PrepareEx(vmsh, BrakeForceAddr, sizeof(float), (BYTE*)&BrakeForce, &BrakeForceBytes);
		VMMDLL_Scatter_PrepareEx(vmsh, DeformAddr, sizeof(float), (BYTE*)&Deform, &DeformBytes);
	}

	if (m_LocalPED_VehicleModsAddr)
	{
		uintptr_t ModelInfoPtr = m_LocalPED_VehicleModsAddr + Offsets::VehicleModelInfo;
		VMMDLL_Scatter_PrepareEx(vmsh, ModelInfoPtr, sizeof(uintptr_t), (BYTE*)&ModelInfoAddr, &ModelInfoBytes);
	}

	VMMDLL_Scatter_Execute(vmsh);

	if (ModelInfoBytes == sizeof(uintptr_t))
		m_LocalPED_VehicleModelInfoAddr = ModelInfoAddr;

	if (PositionBytes == sizeof(Vector3))
		m_LocalPED_VehicleInfo.m_Position = Position;

	if (AccelerationBytes == sizeof(float))
		m_LocalPED_VehicleInfo.m_Acceleration = Acceleration;

	if (MassBytes == sizeof(float))
		m_LocalPED_VehicleInfo.m_Mass = Mass;

	if (BrakeForceBytes == sizeof(float))
		m_LocalPED_VehicleInfo.m_BrakeForce = BrakeForce;

	if (DeformBytes == sizeof(float))
		m_LocalPED_VehicleInfo.m_DeformMultiplier = Deform;

	VMMDLL_Scatter_Clear(vmsh, dma->m_PID, VMMDLL_FLAG_NOCACHE);

	DWORD PrimaryColorBytes = 0;
	DWORD WheelColorBytes = 0;
	struct Colors
	{
		color_t PrimaryColor;
		color_t SecondaryColor;
	};
	Colors MainColors;
	color_t WheelColor;

	if (m_LocalPED_VehicleModelInfoAddr)
	{
		uintptr_t PrimaryColorAddr = m_LocalPED_VehicleModelInfoAddr + Offsets::VehiclePrimaryColor;
		uintptr_t WheelColorAddr = m_LocalPED_VehicleModelInfoAddr + Offsets::VehicleWheelColor;

		VMMDLL_Scatter_PrepareEx(vmsh, PrimaryColorAddr, sizeof(Colors), (BYTE*)&MainColors, &PrimaryColorBytes);
		VMMDLL_Scatter_PrepareEx(vmsh, WheelColorAddr, sizeof(color_t), (BYTE*)&WheelColor, &WheelColorBytes);
	}

	VMMDLL_Scatter_Execute(vmsh);

	if (PrimaryColorBytes == sizeof(Colors))
	{
		m_LocalPED_VehicleInfo.PrimaryColor = MainColors.PrimaryColor;
		m_LocalPED_VehicleInfo.SecondaryColor = MainColors.SecondaryColor;
	}

	if(WheelColorBytes == sizeof(color_t))
		m_LocalPED_VehicleInfo.WheelColor = WheelColor;

	VMMDLL_Scatter_CloseHandle(vmsh);

	{
		std::scoped_lock VehicleInfoLock(VehicleInspector::m_VehicleInfoMutex);
		VehicleInspector::m_VehicleInfo = m_LocalPED_VehicleInfo;
	}

	return 1;
}

bool GTA5::FeatureLoop(Mem* dma)
{
	ZoneScoped;

	GodMode::OnFrame(dma);
	InfiniteAmmo::OnFrame(dma);
	NeverWanted::OnFrame(dma);
	VehicleGodMode::OnFrame(dma);
	Teleport::OnFrame(dma);
	WeaponEditor::OnDMAFrame(dma);
	RefreshHealth::OnFrame(dma);
	VehicleEditor::OnDMAFrame(dma);
	RainbowCar::OnDMAFrame(dma);

	return 1;
}

bool GTA5::m_FindHealthOffset(Mem* dma)
{
	PatternInfo pi;
	pi.ModuleName = GTA5_::Details::m_ProcessName;
	pi.Pattern = Patterns::HealthPattern;
	pi.Mask = Patterns::HealthMask;

	auto SectionOffset = m_Scan.ScanSectionOffset(pi);
	auto RuntimeAddress = m_Scan.Scan(pi);

	if (!SectionOffset || !RuntimeAddress) throw std::exception("m_FindHealthOffset Offset");

	ZydisDisassembledInstruction Instruction;

	if (!ZYAN_SUCCESS(ZydisDisassembleIntel(ZYDIS_MACHINE_MODE_LONG_64, RuntimeAddress, m_Scan.GetBuffer() + SectionOffset, 0x15, &Instruction)))
		throw std::exception("m_FindHealthOffset Disassemble");

	Offsets::CurrentHealth = Instruction.operands[0].mem.disp.value;
	Offsets::MaxHealth = Offsets::CurrentHealth + sizeof(float);

	std::println("[+] Offsets::MaxHealth {0:X}\n[+] Offsets::CurrentHealth {1:X}", Offsets::MaxHealth, Offsets::CurrentHealth);

	return 1;
}

bool GTA5::m_FindWeaponImpactOffsets(Mem* dma)
{
	PatternInfo pi;
	pi.ModuleName = GTA5_::Details::m_ProcessName;
	pi.Pattern = Patterns::WeaponImpactTypePattern;
	pi.Mask = Patterns::WeaponImpactTypeMask;

	auto SectionOffset = m_Scan.ScanSectionOffset(pi);
	auto RuntimeAddress = m_Scan.Scan(pi);

	if (!SectionOffset || !RuntimeAddress) throw std::exception("m_FindWeaponImpactOffsets Offset");

	ZydisDisassembledInstruction Instruction;

	if (!ZYAN_SUCCESS(ZydisDisassembleIntel(ZYDIS_MACHINE_MODE_LONG_64, RuntimeAddress, m_Scan.GetBuffer() + SectionOffset, 0x15, &Instruction)))
		throw std::exception("m_FindWeaponImpactOffsets Disassemble");

	Offsets::ImpactType = Instruction.operands[0].mem.disp.value;
	Offsets::ImpactExplosion = Offsets::ImpactType + 0x4;

	std::println("[+] Offsets::ImpactType {0:X}", Offsets::ImpactType);
	std::println("[+] Offsets::ImpactExplosion {0:X}", Offsets::ImpactExplosion);

	return 1;
}

bool GTA5::m_FindVehicleColorOffsets(Mem* dma)
{
	PatternInfo pi;
	pi.ModuleName = GTA5_::Details::m_ProcessName;
	pi.Pattern = Patterns::VehicleColorPattern;
	pi.Mask = Patterns::VehicleColorMask;

	auto SectionOffset = m_Scan.ScanSectionOffset(pi);
	auto RuntimeAddress = m_Scan.Scan(pi);

	if (!SectionOffset || !RuntimeAddress) throw std::exception("m_FindVehicleColorOffsets Offset");

	ZydisDisassembledInstruction Instruction;

	if (!ZYAN_SUCCESS(ZydisDisassembleIntel(ZYDIS_MACHINE_MODE_LONG_64, RuntimeAddress, m_Scan.GetBuffer() + SectionOffset, 0x15, &Instruction)))
		throw std::exception("m_FindVehicleColorOffsets Disassemble");

	Offsets::VehiclePrimaryColor = Instruction.operands[0].mem.disp.value;
	Offsets::VehicleSecondaryColor = Offsets::VehiclePrimaryColor + sizeof(color_t);
	Offsets::VehicleWheelColor = Offsets::VehicleSecondaryColor + (sizeof(color_t) * 3);

	std::println("[+] Offsets::VehiclePrimaryColor {0:X}", Offsets::VehiclePrimaryColor);
	std::println("[+] Offsets::VehicleSecondaryColor {0:X}", Offsets::VehicleSecondaryColor);
	std::println("[+] Offsets::VehicleWheelColor {0:X}", Offsets::VehicleWheelColor);

	return 1;
}

bool GTA5::m_FindWeaponRecoilAmplitudeOffset(Mem* dma)
{
	PatternInfo pi;
	pi.ModuleName = GTA5_::Details::m_ProcessName;
	pi.Pattern = Patterns::WeaponRecoilAmplitudePattern;
	pi.Mask = Patterns::WeaponRecoilAmplitudeMask;

	auto SectionOffset = m_Scan.ScanSectionOffset(pi);
	auto RuntimeAddress = m_Scan.Scan(pi);

	if (!SectionOffset || !RuntimeAddress) throw std::exception("m_FindWeaponRecoilAmplitudeOffset Offset");

	ZydisDisassembledInstruction Instruction;

	if (!ZYAN_SUCCESS(ZydisDisassembleIntel(ZYDIS_MACHINE_MODE_LONG_64, RuntimeAddress, m_Scan.GetBuffer() + SectionOffset, 0x15, &Instruction)))
		throw std::exception("m_FindWeaponRecoilAmplitudeOffset Disassemble");

	Offsets::WeaponRecoilAmplitude = Instruction.operands[1].imm.value.u;

	std::println("[+] Offsets::WeaponRecoilAmplitude {0:X}", Offsets::WeaponRecoilAmplitude);

	return 1;
}

bool GTA5::m_FindVehicleHealthOffsets(Mem* dma)
{
	PatternInfo pi;
	pi.ModuleName = GTA5_::Details::m_ProcessName;
	pi.Pattern = Patterns::VehicleHealthPattern;
	pi.Mask = Patterns::VehicleHealthMask;

	auto SectionOffset = m_Scan.ScanSectionOffset(pi);
	auto RuntimeAddress = m_Scan.Scan(pi);

	if (!SectionOffset || !RuntimeAddress) throw std::exception("m_FindVehicleHealthOffset Offset");

	ZydisDisassembledInstruction Instruction;

	ZyanUSize Offset = 0;

	if (!ZYAN_SUCCESS(ZydisDisassembleIntel(ZYDIS_MACHINE_MODE_LONG_64, RuntimeAddress, m_Scan.GetBuffer() + SectionOffset, 0x15, &Instruction)))
		throw std::exception("m_FindVehicleHealthOffset Disassemble");

	Offsets::VehicleHealth = Instruction.operands[1].mem.disp.value;

	Offset += Instruction.info.length;

	if (!ZYAN_SUCCESS(ZydisDisassembleIntel(ZYDIS_MACHINE_MODE_LONG_64, RuntimeAddress + Offset, m_Scan.GetBuffer() + SectionOffset + Offset, 0x15, &Instruction)))
		throw std::exception("m_FindVehicleHealthOffset Disassemble");

	Offsets::VehicleMaxHealth = Instruction.operands[1].mem.disp.value;

	std::println("[+] Offsets::VehicleHealth {0:X}", Offsets::VehicleHealth);
	std::println("[+] Offsets::VehicleMaxHealth {0:X}", Offsets::VehicleMaxHealth);

	return 1;
}

static const int MaxBlips = 1700;
std::array<DWORD, MaxBlips> BlipPositionBytes{ 0 };
std::array<DWORD, MaxBlips> BlipIDBytes{ 0 };

bool GTA5::UpdateBlips(Mem* dma)
{
	ZoneScoped;

	if (!m_BlipPtr)
	{
		std::println("m_BlipPtr is null!");
		return false;
	}

	std::vector<uintptr_t>BlipAddresses;
	BlipAddresses.resize(MaxBlips);
	BlipAddresses.reserve(MaxBlips);

	DWORD BytesRead = 0x0;

	auto vmsh = VMMDLL_Scatter_Initialize(dma->m_vmh, dma->m_PID, VMMDLL_FLAG_NOCACHE);

	VMMDLL_Scatter_PrepareEx(vmsh, m_BlipPtr, sizeof(uintptr_t) * MaxBlips, (BYTE*)&BlipAddresses[0], &BytesRead);

	VMMDLL_Scatter_Execute(vmsh);

	if (BytesRead != sizeof(uintptr_t) * MaxBlips)
	{
		std::println("[-] UpdateBlips BytesRead {0:X}/{1:X}", BytesRead, sizeof(uintptr_t) * MaxBlips);
		return false;
	}

	BlipAddresses.erase(std::remove(BlipAddresses.begin(), BlipAddresses.end(), 0x0), BlipAddresses.end());

	VMMDLL_Scatter_Clear(vmsh, dma->m_PID, VMMDLL_FLAG_NOCACHE);

	std::vector<BlipInfo> Blips;
	Blips.resize(BlipAddresses.size());
	Blips.reserve(BlipAddresses.size());

	for (int i = 0; i < BlipAddresses.size(); i++)
	{
		uintptr_t BlipAddress = BlipAddresses[i];

		uintptr_t BlipPositonAddress = BlipAddress + Offsets::BlipPosition;
		uintptr_t BlipIDAddress = BlipAddress + Offsets::BlipID;

		VMMDLL_Scatter_PrepareEx(vmsh, BlipPositonAddress, sizeof(Vector3), (BYTE*)&Blips[i].WorldPosition, &BlipPositionBytes[i]);
		VMMDLL_Scatter_PrepareEx(vmsh, BlipIDAddress, sizeof(int32_t), (BYTE*)&Blips[i].ID, &BlipIDBytes[i]);
	}

	VMMDLL_Scatter_ExecuteRead(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);

	std::vector<BlipInfo> CompleteBlips;
	for (int i = 0; i < BlipAddresses.size(); i++)
	{
		if (BlipIDBytes[i] == sizeof(int32_t) && BlipPositionBytes[i] == sizeof(Vector3))
			CompleteBlips.push_back(Blips[i]);
		else
			std::println("  [-] Incomplete Blip");
	}

	m_Blips.assign(CompleteBlips.begin(), CompleteBlips.end());

	{
		std::scoped_lock BlipInspectorLock(BlipInspector::m_BlipInspectorMutex);
		BlipInspector::m_Blips.assign(CompleteBlips.begin(), CompleteBlips.end());
		BlipInspector::m_LastPlayerPosition = m_LocalPED_PlayerInfo.m_Location;
	}

	return 1;
}

Vector3 GTA5::GetWaypointLocation()
{
	for (auto Blip : m_Blips)
	{
		if (Blip.ID == 8)
			return Blip.WorldPosition;
	}

	Vector3 Return = { 0.0f,0.0f,0.0f };
	return Return;
}

bool GTA5::m_FindOffset_Disp1(Mem* dma, PatternInfo& pi, uint32_t& Offset, std::string Name)
{
	auto SectionOffset = m_Scan.ScanSectionOffset(pi);
	auto RuntimeAddress = m_Scan.Scan(pi);

	if (!SectionOffset || !RuntimeAddress) throw std::exception(std::format("{0:s} Offset", Name.c_str()).c_str());

	ZydisDisassembledInstruction Instruction;

	if (!ZYAN_SUCCESS(ZydisDisassembleIntel(ZYDIS_MACHINE_MODE_LONG_64, RuntimeAddress, m_Scan.GetBuffer() + SectionOffset, 0x15, &Instruction)))
		throw std::exception(std::format("{0:s} Disassemble", Name.c_str()).c_str());

	Offset = Instruction.operands[1].mem.disp.value;

	std::println("[+] Offsets::{0:s} {1:X}", Name.c_str(), Offset);

	return 1;
}

bool GTA5::m_FindOffset_Disp0(Mem* dma, PatternInfo& pi, uint32_t& Offset, std::string Name)
{
	auto SectionOffset = m_Scan.ScanSectionOffset(pi);
	auto RuntimeAddress = m_Scan.Scan(pi);

	if (!SectionOffset || !RuntimeAddress) throw std::exception(std::format("{0:s} Offset", Name.c_str()).c_str());

	ZydisDisassembledInstruction Instruction;

	if (!ZYAN_SUCCESS(ZydisDisassembleIntel(ZYDIS_MACHINE_MODE_LONG_64, RuntimeAddress, m_Scan.GetBuffer() + SectionOffset, 0x15, &Instruction)))
		throw std::exception(std::format("{0:s} Disassemble", Name.c_str()).c_str());

	Offset = Instruction.operands[0].mem.disp.value;

	std::println("[+] Offsets::{0:s} {1:X}", Name.c_str(), Offset);

	return 1;
}

Mem* GTA5_::Initialize()
{
	if (!g_mem.Initialize()) return nullptr;

	std::println("[+] GTA5_::Initialize()");

	g_mem.SetPID(Details::m_ProcessName);

	GTA5::FindOffsets(&g_mem);
	GTA5::UpdateWorldAddress(&g_mem);
	GTA5::UpdateLocalPlayerAddr(&g_mem);

	return &GTA5_::g_mem;
}

bool GTA5_::Close()
{
	g_mem.Close();

	return true;
}
