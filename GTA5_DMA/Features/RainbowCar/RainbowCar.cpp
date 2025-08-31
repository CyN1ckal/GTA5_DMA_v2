#include "../../pch.h"

#include "DMA/DMA.h"

#include "RainbowCar.h"

#include "Game/GTA5Info.h"

bool RainbowCar::OnDMAFrame(Mem* dma)
{
	if (!m_RainbowCar) return 0;

	if (!GTA5::m_LocalPED_VehicleModelInfoAddr) return 0;

	uintptr_t PrimaryColorAddr = GTA5::m_LocalPED_VehicleModelInfoAddr + Offsets::VehiclePrimaryColor;
	uintptr_t SecondaryColorAddr = GTA5::m_LocalPED_VehicleModelInfoAddr + Offsets::VehicleSecondaryColor;

	auto vmsh = VMMDLL_Scatter_Initialize(dma->m_vmh, dma->m_PID, VMMDLL_FLAG_NOCACHE);

	static ImVec4 LastColor = { 0.0f,0.0f,0.0f,1.0f };

	color_t NewColor = color_t::FromImVec4(LastColor);

	VMMDLL_Scatter_PrepareWrite(vmsh, PrimaryColorAddr, (BYTE*)&NewColor, sizeof(color_t));
	VMMDLL_Scatter_PrepareWrite(vmsh, SecondaryColorAddr, (BYTE*)&NewColor, sizeof(color_t));
	
	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);

	static bool bRedDecreasing = true;
	if (bRedDecreasing)
	{
		LastColor.x -= 0.01f;

		if (LastColor.x < 0.02f)
			bRedDecreasing = false;
	}
	else
	{
		LastColor.x += 0.01f;

		if (LastColor.x > 0.98f)
			bRedDecreasing = true;

	}

	static bool bGreenDecreasing = true;
	if (bGreenDecreasing)
	{
		LastColor.y -= 0.015f;

		if (LastColor.y < 0.02f)
			bGreenDecreasing = false;
	}
	else
	{
		LastColor.y += 0.015f;

		if (LastColor.y > 0.98f)
			bGreenDecreasing = true;

	}

	static bool bBlueDecreasing = true;
	if (bBlueDecreasing)
	{
		LastColor.z -= 0.019f;

		if (LastColor.z < 0.02f)
			bBlueDecreasing = false;
	}
	else
	{
		LastColor.z += 0.019f;

		if (LastColor.z > 0.98f)
			bBlueDecreasing = true;

	}

	return 1;
}