#include "pch.h"

#include "InfiniteAmmo.h"

#include <bitset>


bool InfiniteAmmo::OnFrame(DMA* dma)
{
	if (!GTA5::m_LocalPED_WeaponInventoryAddr)
		return 0;

	std::bitset<32> CurrentAmmoBits(GTA5::m_LocalPED_AmmoModifierBits);

	if (m_InfiniteAmmo && CurrentAmmoBits.test(1))
		return 0;

	if (!m_InfiniteAmmo && !CurrentAmmoBits.test(1))
		return 0;

	auto vmsh = VMMDLL_Scatter_Initialize(dma->m_vmh, dma->m_PID, VMMDLL_FLAG_NOCACHE);

	DWORD BytesRead = 0x0;

	uintptr_t AmmoModifierAddress = GTA5::m_LocalPED_WeaponInventoryAddr + Offsets::AmmoModifier;
	uint32_t AmmoModifierBits = 0x0;
	VMMDLL_Scatter_PrepareEx(vmsh, AmmoModifierAddress, sizeof(uint32_t), (BYTE*)&AmmoModifierBits, &BytesRead);
	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_Clear(vmsh, dma->m_PID, VMMDLL_FLAG_NOCACHE);

	std::bitset<32> bits(AmmoModifierBits);
	if (m_InfiniteAmmo)
	{
		bits.set(1);
	}
	else
	{
		bits.reset(1);
	}

	uint32_t NewBits = bits.to_ulong();

	VMMDLL_Scatter_PrepareWrite(vmsh, AmmoModifierAddress, (BYTE*)&NewBits, sizeof(uint32_t));

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);

	std::println("[+] Infinite Ammo updated {0:d}", m_InfiniteAmmo);

	return 1;
}