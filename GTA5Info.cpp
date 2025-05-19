#include "pch.h"

#include "GTA5Info.h"

#include "DMA.h"

#include "Zydis/Zydis.h"

#include "Features.h"

#include "Offsets.h"

bool GTA5::m_FindWorldPtr(DMA* dma)
{
	PatternInfo pi;
	pi.ModuleName = dma->m_ProcessName;
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

bool GTA5::m_FindBlipPtr(DMA* dma)
{
	PatternInfo pi;
	pi.ModuleName = dma->m_ProcessName;
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

bool GTA5::FindOffsets(DMA* dma)
{
	m_Scan.Initialize(dma->m_vmh, dma->m_PID, dma->m_ProcessName);

	m_FindWeaponInventoryOffset(dma);
	m_FindPlayerInfoOffset(dma);
	m_FindWantedLevelOffset(dma);
	m_FindHealthOffset(dma);
	m_FindGodBitsOffset(dma);
	m_FindVehicleGodBitsOffset(dma);
	m_FindAmmoModifierOffset(dma);
	m_FindPlayerPositionOffset(dma);
	m_FindNavigationOffset(dma);

	m_FindBlipPositionOffset(dma);
	m_FindBlipIDOffset(dma);

	m_FindWorldPtr(dma);
	m_FindBlipPtr(dma);

	m_Scan.Close();

	return 1;
}

bool GTA5::UpdateLocalPlayerAddr(DMA* dma)
{
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

bool GTA5::UpdateWorldAddress(DMA* dma)
{
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

bool GTA5::UpdateLocalPlayerInfo(DMA* dma)
{
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

	auto vmsh = VMMDLL_Scatter_Initialize(dma->m_vmh, dma->m_PID, VMMDLL_FLAG_NOCACHE);

	uintptr_t GodModeBitsAddress = m_LocalPEDAddr + Offsets::GodBits;
	uintptr_t CurrentHealthAddress = m_LocalPEDAddr + Offsets::CurrentHealth;
	uintptr_t WeaponInventoryPtr = m_LocalPEDAddr + Offsets::WeaponInventory;
	uintptr_t PlayerInfoPtr = m_LocalPEDAddr + Offsets::PlayerInfo;
	uintptr_t VehiclePtr = m_LocalPEDAddr + Offsets::VehicleOffset;
	uintptr_t NavigationPtr = m_LocalPEDAddr + Offsets::Navigation;

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

	VMMDLL_Scatter_PrepareEx(vmsh, CurrentHealthAddress, sizeof(float) * 2, (BYTE*)&health, &HealthBytes);
	VMMDLL_Scatter_PrepareEx(vmsh, GodModeBitsAddress, sizeof(uint32_t), (BYTE*)&GodModeBits, &GodBytes);
	VMMDLL_Scatter_PrepareEx(vmsh, WeaponInventoryPtr, sizeof(uintptr_t), (BYTE*)&WeaponInventoryAddr, &WeaponInventoryBytes);
	VMMDLL_Scatter_PrepareEx(vmsh, PlayerInfoPtr, sizeof(uintptr_t), (BYTE*)&PlayerInfoAddr, &PlayerInfoBytes);
	VMMDLL_Scatter_PrepareEx(vmsh, VehiclePtr, sizeof(uintptr_t), (BYTE*)&VehicleAddr, &VehicleBytes);
	VMMDLL_Scatter_PrepareEx(vmsh, NavigationPtr, sizeof(uintptr_t), (BYTE*)&NavigationAdd, &NavigationBytes);

	VMMDLL_Scatter_Execute(vmsh);

	if (HealthBytes == sizeof(float) * 2)
	{
		m_LocalPED_CurrentHealth = health.CurrentHealth;
		m_LocalPED_MaxHealth = health.MaxHealth;
	}

	if (GodBytes == sizeof(uint32_t))
	{
		m_LocalPED_GodModeBits = GodModeBits;
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

	VMMDLL_Scatter_Clear(vmsh, dma->m_PID, VMMDLL_FLAG_NOCACHE);

	DWORD AmmoModifierBytes = 0x0;
	DWORD VehicleGodBytes = 0x0;
	DWORD WantedLevelBytes = 0x0;
	DWORD PositionBytes = 0x0;

	uint32_t AmmoModifierBits = 0x0;
	uint32_t VehicleGodBits = 0x0;
	int32_t WantedLevel = 0x0;
	Vector3 PlayerPosition = { 0.0f,0.0f,0.0f };

	uintptr_t AmmoModifierAddress = m_LocalPED_WeaponInventoryAddr + Offsets::AmmoModifier;
	uintptr_t WantedLevelAddress = m_LocalPED_PlayerInfoAddr + Offsets::WantedLevel;
	uintptr_t VehicleGodBitsAddress = m_LocalPED_VehicleAddr + Offsets::VehicleGodBits;
	uintptr_t PlayerPositonAddress = m_LocalPED_NavigationAddr + Offsets::PlayerPosition;

	if (m_LocalPED_WeaponInventoryAddr)
		VMMDLL_Scatter_PrepareEx(vmsh, AmmoModifierAddress, sizeof(uint32_t), (BYTE*)&AmmoModifierBits, &AmmoModifierBytes);

	if (m_LocalPED_PlayerInfoAddr)
		VMMDLL_Scatter_PrepareEx(vmsh, WantedLevelAddress, sizeof(int32_t), (BYTE*)&WantedLevel, &WantedLevelBytes);

	if (m_LocalPED_VehicleAddr)
		VMMDLL_Scatter_PrepareEx(vmsh, VehicleGodBitsAddress, sizeof(uint32_t), (BYTE*)&VehicleGodBits, &VehicleGodBytes);

	if (m_LocalPED_NavigationAddr)
		VMMDLL_Scatter_PrepareEx(vmsh, PlayerPositonAddress, sizeof(Vector3), (BYTE*)&PlayerPosition, &PositionBytes);

	if (m_LocalPED_WeaponInventoryAddr || m_LocalPED_PlayerInfoAddr || m_LocalPED_NavigationAddr || m_LocalPED_VehicleAddr)
		VMMDLL_Scatter_Execute(vmsh);

	if (AmmoModifierBytes == sizeof(uint32_t))
	{
		m_LocalPED_AmmoModifierBits = AmmoModifierBits;
	}

	if (VehicleGodBytes == sizeof(uint32_t))
	{
		m_LocalPED_VehicleGodModeBits = VehicleGodBits;
	}

	if (WantedLevelBytes == sizeof(int32_t))
	{
		m_LocalPED_WantedLevel = WantedLevel;
	}

	if (PositionBytes == sizeof(Vector3))
	{
		m_LocalPED_Location.x = PlayerPosition.x;
		m_LocalPED_Location.y = PlayerPosition.y;
		m_LocalPED_Location.z = PlayerPosition.z;
	}

	VMMDLL_Scatter_CloseHandle(vmsh);

	return 1;
}

bool GTA5::FeatureLoop(DMA* dma)
{
	GodMode::OnFrame(dma);
	InfiniteAmmo::OnFrame(dma);
	NeverWanted::OnFrame(dma);
	VehicleGodMode::OnFrame(dma);
	Teleport::OnFrame(dma);

	return 1;
}

bool GTA5::m_FindGodBitsOffset(DMA* dma)
{
	PatternInfo pi;
	pi.ModuleName = dma->m_ProcessName;
	pi.Pattern = Patterns::GodBitsPattern;
	pi.Mask = Patterns::GodBitsMask;

	auto SectionOffset = m_Scan.ScanSectionOffset(pi);
	auto RuntimeAddress = m_Scan.Scan(pi);

	if (!SectionOffset || !RuntimeAddress) throw std::exception("m_FindGodBitsOffset Offset");

	ZydisDisassembledInstruction Instruction;

	if (!ZYAN_SUCCESS(ZydisDisassembleIntel(ZYDIS_MACHINE_MODE_LONG_64, RuntimeAddress, m_Scan.GetBuffer() + SectionOffset, 0x15, &Instruction)))
		throw std::exception("m_FindGodBitsOffset Disassemble");

	Offsets::GodBits = Instruction.operands[0].mem.disp.value;

	std::println("[+] Offsets::GodBits {0:X}", Offsets::GodBits);

	return 1;
}

bool GTA5::m_FindHealthOffset(DMA* dma)
{
	PatternInfo pi;
	pi.ModuleName = dma->m_ProcessName;
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

bool GTA5::m_FindWeaponInventoryOffset(DMA* dma)
{
	PatternInfo pi;
	pi.ModuleName = dma->m_ProcessName;
	pi.Pattern = Patterns::WeaponInventoryPattern;
	pi.Mask = Patterns::WeaponInventoryMask;

	auto SectionOffset = m_Scan.ScanSectionOffset(pi);
	auto RuntimeAddress = m_Scan.Scan(pi);

	if (!SectionOffset || !RuntimeAddress) throw std::exception("m_FindWeaponInventoryOffset Offset");

	ZydisDisassembledInstruction Instruction;

	if (!ZYAN_SUCCESS(ZydisDisassembleIntel(ZYDIS_MACHINE_MODE_LONG_64, RuntimeAddress, m_Scan.GetBuffer() + SectionOffset, 0x15, &Instruction)))
		throw std::exception("m_FindHealthOffset Disassemble");

	Offsets::WeaponInventory = Instruction.operands[1].mem.disp.value;

	std::println("[+] Offsets::WeaponInventory {0:X}", Offsets::WeaponInventory);

	return 1;
}

bool GTA5::m_FindAmmoModifierOffset(DMA* dma)
{
	PatternInfo pi;
	pi.ModuleName = dma->m_ProcessName;
	pi.Pattern = Patterns::AmmoModifierPattern;
	pi.Mask = Patterns::AmmoModifierMask;

	auto SectionOffset = m_Scan.ScanSectionOffset(pi);
	auto RuntimeAddress = m_Scan.Scan(pi);

	if (!SectionOffset || !RuntimeAddress) throw std::exception("m_FindAmmoModifierOffset Offset");

	ZydisDisassembledInstruction Instruction;

	if (!ZYAN_SUCCESS(ZydisDisassembleIntel(ZYDIS_MACHINE_MODE_LONG_64, RuntimeAddress, m_Scan.GetBuffer() + SectionOffset, 0x15, &Instruction)))
		throw std::exception("m_FindAmmoModifierOffset Disassemble");

	Offsets::AmmoModifier = Instruction.operands[0].mem.disp.value;

	std::println("[+] Offsets::AmmoModifier {0:X}", Offsets::AmmoModifier);

	return 1;
}

bool GTA5::m_FindPlayerInfoOffset(DMA* dma)
{
	PatternInfo pi;
	pi.ModuleName = dma->m_ProcessName;
	pi.Pattern = Patterns::PlayerInfoPattern;
	pi.Mask = Patterns::PlayerInfoMask;

	auto SectionOffset = m_Scan.ScanSectionOffset(pi);
	auto RuntimeAddress = m_Scan.Scan(pi);

	if (!SectionOffset || !RuntimeAddress) throw std::exception("m_FindAmmoModifierOffset Offset");

	ZydisDisassembledInstruction Instruction;

	if (!ZYAN_SUCCESS(ZydisDisassembleIntel(ZYDIS_MACHINE_MODE_LONG_64, RuntimeAddress, m_Scan.GetBuffer() + SectionOffset, 0x15, &Instruction)))
		throw std::exception("m_FindAmmoModifierOffset Disassemble");

	Offsets::PlayerInfo = Instruction.operands[0].mem.disp.value;

	std::println("[+] Offsets::PlayerInfo {0:X}", Offsets::PlayerInfo);

	return 1;
}

bool GTA5::m_FindWantedLevelOffset(DMA* dma)
{
	PatternInfo pi;
	pi.ModuleName = dma->m_ProcessName;
	pi.Pattern = Patterns::WantedLevelPattern;
	pi.Mask = Patterns::WantedLevelMask;

	auto SectionOffset = m_Scan.ScanSectionOffset(pi);
	auto RuntimeAddress = m_Scan.Scan(pi);

	if (!SectionOffset || !RuntimeAddress) throw std::exception("m_FindWantedLevelOffset Offset");

	ZydisDisassembledInstruction Instruction;

	if (!ZYAN_SUCCESS(ZydisDisassembleIntel(ZYDIS_MACHINE_MODE_LONG_64, RuntimeAddress, m_Scan.GetBuffer() + SectionOffset, 0x15, &Instruction)))
		throw std::exception("m_FindWantedLevelOffset Disassemble");

	Offsets::WantedLevel = Instruction.operands[0].mem.disp.value;

	std::println("[+] Offsets::WantedLevel {0:X}", Offsets::WantedLevel);

	return 1;
}

bool GTA5::m_FindVehicleGodBitsOffset(DMA* dma)
{
	PatternInfo pi;
	pi.ModuleName = dma->m_ProcessName;
	pi.Pattern = Patterns::VehicleGodBitsPattern;
	pi.Mask = Patterns::VehicleGodBitsMask;

	auto SectionOffset = m_Scan.ScanSectionOffset(pi);
	auto RuntimeAddress = m_Scan.Scan(pi);

	if (!SectionOffset || !RuntimeAddress) throw std::exception("m_FindVehicleGodBitsOffset Offset");

	ZydisDisassembledInstruction Instruction;

	if (!ZYAN_SUCCESS(ZydisDisassembleIntel(ZYDIS_MACHINE_MODE_LONG_64, RuntimeAddress, m_Scan.GetBuffer() + SectionOffset, 0x15, &Instruction)))
		throw std::exception("m_FindVehicleGodBitsOffset Disassemble");

	Offsets::VehicleGodBits = Instruction.operands[0].mem.disp.value;

	std::println("[+] Offsets::VehicleGodBits {0:X}", Offsets::VehicleGodBits);

	return 1;
}

bool GTA5::m_FindNavigationOffset(DMA* dma)
{
	PatternInfo pi;
	pi.ModuleName = dma->m_ProcessName;
	pi.Pattern = Patterns::NavigationPattern;
	pi.Mask = Patterns::NavigationMask;

	auto SectionOffset = m_Scan.ScanSectionOffset(pi);
	auto RuntimeAddress = m_Scan.Scan(pi);

	if (!SectionOffset || !RuntimeAddress) throw std::exception("m_FindNavigationOffset Offset");

	ZydisDisassembledInstruction Instruction;

	if (!ZYAN_SUCCESS(ZydisDisassembleIntel(ZYDIS_MACHINE_MODE_LONG_64, RuntimeAddress, m_Scan.GetBuffer() + SectionOffset, 0x15, &Instruction)))
		throw std::exception("m_FindNavigationOffset Disassemble");

	Offsets::Navigation = Instruction.operands[1].mem.disp.value;

	std::println("[+] Offsets::Navigation {0:X}", Offsets::Navigation);

	return 1;
}

bool GTA5::m_FindPlayerPositionOffset(DMA* dma)
{
	PatternInfo pi;
	pi.ModuleName = dma->m_ProcessName;
	pi.Pattern = Patterns::PlayerPositionPattern;
	pi.Mask = Patterns::PlayerPositionMask;

	auto SectionOffset = m_Scan.ScanSectionOffset(pi);
	auto RuntimeAddress = m_Scan.Scan(pi);

	if (!SectionOffset || !RuntimeAddress) throw std::exception("m_FindPlayerPositionOffset Offset");

	ZydisDisassembledInstruction Instruction;

	if (!ZYAN_SUCCESS(ZydisDisassembleIntel(ZYDIS_MACHINE_MODE_LONG_64, RuntimeAddress, m_Scan.GetBuffer() + SectionOffset, 0x15, &Instruction)))
		throw std::exception("m_FindPlayerPositionOffset Disassemble");

	Offsets::PlayerPosition = Instruction.operands[1].mem.disp.value;

	std::println("[+] Offsets::PlayerPosition {0:X}", Offsets::PlayerPosition);

	return 1;
}

bool GTA5::m_FindBlipPositionOffset(DMA* dma)
{
	PatternInfo pi;
	pi.ModuleName = dma->m_ProcessName;
	pi.Pattern = Patterns::BlipPositionPattern;
	pi.Mask = Patterns::BlipPositionMask;

	auto SectionOffset = m_Scan.ScanSectionOffset(pi);
	auto RuntimeAddress = m_Scan.Scan(pi);

	if (!SectionOffset || !RuntimeAddress) throw std::exception("m_FindBlipPositionOffset Offset");

	ZydisDisassembledInstruction Instruction;

	if (!ZYAN_SUCCESS(ZydisDisassembleIntel(ZYDIS_MACHINE_MODE_LONG_64, RuntimeAddress, m_Scan.GetBuffer() + SectionOffset, 0x15, &Instruction)))
		throw std::exception("m_FindBlipPositionOffset Disassemble");

	Offsets::BlipPosition = Instruction.operands[0].mem.disp.value;

	std::println("[+] Offsets::BlipPosition {0:X}", Offsets::BlipPosition);

	return 1;
}

bool GTA5::m_FindBlipIDOffset(DMA* dma)
{
	PatternInfo pi;
	pi.ModuleName = dma->m_ProcessName;
	pi.Pattern = Patterns::BlipIDPattern;
	pi.Mask = Patterns::BlipIDMask;

	auto SectionOffset = m_Scan.ScanSectionOffset(pi);
	auto RuntimeAddress = m_Scan.Scan(pi);

	if (!SectionOffset || !RuntimeAddress) throw std::exception("m_FindBlipIDOffset Offset");

	ZydisDisassembledInstruction Instruction;

	if (!ZYAN_SUCCESS(ZydisDisassembleIntel(ZYDIS_MACHINE_MODE_LONG_64, RuntimeAddress, m_Scan.GetBuffer() + SectionOffset, 0x15, &Instruction)))
		throw std::exception("m_FindBlipIDOffset Disassemble");

	Offsets::BlipID = Instruction.operands[1].mem.disp.value;

	std::println("[+] Offsets::BlipID {0:X}", Offsets::BlipID);

	return 1;
}

static const int MaxBlips = 2000;
std::array<DWORD, MaxBlips> BlipPositionBytes{ 0 };
std::array<DWORD, MaxBlips> BlipIDBytes{ 0 };

bool GTA5::UpdateBlips(DMA* dma)
{
	if (!m_BlipPtr)
		return 0;

	std::vector<uintptr_t>BlipAddresses;
	BlipAddresses.resize(MaxBlips);
	BlipAddresses.reserve(MaxBlips);

	DWORD BytesRead = 0x0;

	auto vmsh = VMMDLL_Scatter_Initialize(dma->m_vmh, dma->m_PID, VMMDLL_FLAG_NOCACHE);

	VMMDLL_Scatter_PrepareEx(vmsh, m_BlipPtr, sizeof(uintptr_t) * MaxBlips, (BYTE*)&BlipAddresses[0], &BytesRead);

	VMMDLL_Scatter_Execute(vmsh);

	if (BytesRead != sizeof(uintptr_t) * MaxBlips)
		throw std::exception(std::format("UpdateBlips BytesRead {0:X}/{1:X}", BytesRead, sizeof(uintptr_t) * MaxBlips).c_str());

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

	m_Blips.assign(Blips.begin(), Blips.end());

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