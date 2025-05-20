#include "pch.h"

#include "DMA.h"

#include "NeverWanted.h"

bool NeverWanted::OnFrame(DMA* dma)
{
	if (!m_NeverWanted)
		return 0;

	if (!GTA5::m_LocalPED_PlayerInfoAddr)
		return 0;

	if (!GTA5::m_LocalPED_PlayerInfo.m_WantedLevel)
		return 0;

	auto vmsh = VMMDLL_Scatter_Initialize(dma->m_vmh, dma->m_PID, VMMDLL_FLAG_NOCACHE);

	int32_t NewWantedLevel = 0x0;

	uintptr_t WantedLevelAddress = GTA5::m_LocalPED_PlayerInfoAddr + Offsets::WantedLevel;

	VMMDLL_Scatter_PrepareWrite(vmsh, WantedLevelAddress, (BYTE*)&NewWantedLevel, sizeof(int32_t));

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);

	std::println("[+] Cleared Wanted Level");

	return 1;
}