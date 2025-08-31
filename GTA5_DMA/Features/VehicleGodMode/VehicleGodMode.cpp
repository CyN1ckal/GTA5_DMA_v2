#include "../../pch.h"

#include "DMA/DMA.h"

#include "VehicleGodMode.h"

#include <bitset>

#include "Game/GTA5Info.h"

bool VehicleGodMode::OnFrame(Mem* dma)
{
	if(!GTA5::m_LocalPED_VehicleAddr)
		return 0;

	std::bitset<32> CurrentGodBits(GTA5::m_LocalPED_VehicleInfo.m_GodBits);

	if (m_VehicleGodMode && CurrentGodBits.test(4) && CurrentGodBits.test(8))
		return 0;

	if (!m_VehicleGodMode && !CurrentGodBits.test(4) && !CurrentGodBits.test(8))
		return 0;

	auto vmsh = VMMDLL_Scatter_Initialize(dma->m_vmh, dma->m_PID, VMMDLL_FLAG_NOCACHE);

	DWORD BytesRead = 0x0;

	uintptr_t GodModeBitsAddress = GTA5::m_LocalPED_VehicleAddr + Offsets::VehicleGodBits;
	uint32_t GodModeBits = 0x0;
	VMMDLL_Scatter_PrepareEx(vmsh, GodModeBitsAddress, sizeof(uint32_t), (BYTE*)&GodModeBits, &BytesRead);
	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_Clear(vmsh, dma->m_PID, VMMDLL_FLAG_NOCACHE);

	std::bitset<32> bits(GodModeBits);
	if (m_VehicleGodMode)
	{
		bits.set(4);
		bits.set(8);
	}
	else
	{
		bits.reset(4);
		bits.reset(8);
	}

	uint32_t NewBits = bits.to_ulong();

	VMMDLL_Scatter_PrepareWrite(vmsh, GodModeBitsAddress, (BYTE*)&NewBits, sizeof(uint32_t));

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);

	std::println("[+] Vehicle GodMode updated {0:d}", m_VehicleGodMode);

	GTA5::m_LocalPED_VehicleInfo.m_GodBits = NewBits;

	return 1;
}