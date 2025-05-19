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

	if (!SectionOffset || !RuntimeAddress) return 0;

	ZydisDisassembledInstruction Instruction;

	if (!ZYAN_SUCCESS(ZydisDisassembleIntel(ZYDIS_MACHINE_MODE_LONG_64, RuntimeAddress, m_Scan.GetBuffer() + SectionOffset, 0x15, &Instruction)))
	{
		std::println("Failed to disassemble instruction!");
		return 0;
	}

	m_WorldPtr = Instruction.operands[1].mem.disp.value + RuntimeAddress + Instruction.info.length;

	std::println("[+] m_WorldPtr {0:X}", m_WorldPtr);

	return 1;
}

bool GTA5::FindOffsets(DMA* dma)
{
	m_Scan.Initialize(dma->m_vmh, dma->m_PID, dma->m_ProcessName);

	m_FindWorldPtr(dma);

	m_FindGodBitsOffset(dma);
	m_FindHealthOffset(dma);
	m_FindAmmoModifierOffset(dma);
	m_FindWeaponInventoryOffset(dma);
	m_FindPlayerInfoOffset(dma);

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

	std::println("[+] m_WorldAddr {0:X}", m_WorldAddr);

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

	auto vmsh = VMMDLL_Scatter_Initialize(dma->m_vmh, dma->m_PID, VMMDLL_FLAG_NOCACHE);

	uintptr_t GodModeBitsAddress = m_LocalPEDAddr + Offsets::GodBits;
	uintptr_t CurrentHealthAddress = m_LocalPEDAddr + Offsets::CurrentHealth;
	uintptr_t WeaponInventoryPtr = m_LocalPEDAddr + Offsets::WeaponInventory;

	struct Health
	{
		float CurrentHealth;
		float MaxHealth;
	};

	Health health;
	uint32_t GodModeBits = 0x0;
	uintptr_t WeaponInventoryAddr = 0x0;

	VMMDLL_Scatter_PrepareEx(vmsh, CurrentHealthAddress, sizeof(float) * 2, (BYTE*)&health, &HealthBytes);
	VMMDLL_Scatter_PrepareEx(vmsh, GodModeBitsAddress, sizeof(uint32_t), (BYTE*)&GodModeBits, &GodBytes);
	VMMDLL_Scatter_PrepareEx(vmsh, WeaponInventoryPtr, sizeof(uintptr_t), (BYTE*)&WeaponInventoryAddr, &WeaponInventoryBytes);

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

	VMMDLL_Scatter_Clear(vmsh, dma->m_PID, VMMDLL_FLAG_NOCACHE);

	DWORD AmmoModifierBytes = 0x0;
	uint32_t AmmoModifierBits = 0x0;
	uintptr_t AmmoModifierAddress = m_LocalPED_WeaponInventoryAddr + Offsets::AmmoModifier;
	VMMDLL_Scatter_PrepareEx(vmsh, AmmoModifierAddress, sizeof(uint32_t), (BYTE*)&AmmoModifierBits, &AmmoModifierBytes);

	VMMDLL_Scatter_Execute(vmsh);

	if (AmmoModifierBytes == sizeof(uint32_t))
	{
		m_LocalPED_AmmoModifierBits = AmmoModifierBits;
	}

	VMMDLL_Scatter_CloseHandle(vmsh);

	return 1;
}

bool GTA5::FeatureLoop(DMA* dma)
{
	GodMode::OnFrame(dma);
	InfiniteAmmo::OnFrame(dma);

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

	std::println("[+] Offsets::CurrentHealth {0:X}\n[+] Offsets::MaxHealth {1:X}", Offsets::CurrentHealth, Offsets::MaxHealth);

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