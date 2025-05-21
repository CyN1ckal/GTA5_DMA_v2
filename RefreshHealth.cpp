#include "pch.h"

#include "DMA.h"

#include "RefreshHealth.h"

bool RefreshHealth::OnFrame(DMA* dma)
{
	if (!m_RefreshHealth) return 1;

	auto& PlayerInfo = GTA5::m_LocalPED_PlayerInfo;

	float HealthPercentage = PlayerInfo.m_CurrentHealth / PlayerInfo.m_MaxHealth;

	if (HealthPercentage > m_HealthThreshold) return 1;

	auto vmsh = VMMDLL_Scatter_Initialize(dma->m_vmh, dma->m_PID, VMMDLL_FLAG_NOCACHE);

	uintptr_t HealthAddr = GTA5::m_LocalPEDAddr + Offsets::CurrentHealth;

	VMMDLL_Scatter_PrepareWrite(vmsh, HealthAddr, (BYTE*)&PlayerInfo.m_MaxHealth,sizeof(float));

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);

	std::println("[+] Healed Player");

	return 1;
}