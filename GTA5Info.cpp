#include "pch.h"

#include "GTA5Info.h"

#include "DMA.h"

#include "Zydis/Zydis.h"

#include "Features.h"

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

bool GTA5::FindPointers(DMA* dma)
{
	m_Scan.Initialize(dma->m_vmh, dma->m_PID, dma->m_ProcessName);

	m_FindWorldPtr(dma);

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

	std::println("[+] m_LocalPEDAddr {0:X}", m_LocalPEDAddr);

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

	DWORD BytesRead = 0x0;

	auto vmsh = VMMDLL_Scatter_Initialize(dma->m_vmh, dma->m_PID, VMMDLL_FLAG_NOCACHE);

	uintptr_t CurrentHealthAddress = m_LocalPEDAddr + 0x280;
	uintptr_t MaxHealthAddress = m_LocalPEDAddr + 0x284;

	struct Health
	{
		float CurrentHealth;
		float MaxHealth;
	};

	Health health;

	VMMDLL_Scatter_PrepareEx(vmsh, CurrentHealthAddress, sizeof(float) * 2, (BYTE*)&health, &BytesRead);
	VMMDLL_Scatter_Execute(vmsh);

	if (BytesRead == sizeof(float) * 2)
	{
		m_LocalPED_CurrentHealth = health.CurrentHealth;
		m_LocalPED_MaxHealth = health.MaxHealth;
	}

	VMMDLL_Scatter_CloseHandle(vmsh);

	//std::println("[+] Local Health {0:.2f} / {1:.2f}", m_LocalPED_CurrentHealth, m_LocalPED_MaxHealth);

	return 1;
}



bool GTA5::FeatureLoop(DMA* dma)
{
	GodMode::OnFrame(dma);

	return 1;
}