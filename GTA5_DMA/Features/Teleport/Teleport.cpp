#include "../../pch.h"

#include <thread>

#include "../../Structs/MyVector.h"

#include "../../Core/DMA/DMA.h"

#include "Teleport.h"

bool Teleport::OnFrame(DMA* dma)
{
	if (m_RequestedTeleport)
	{
		std::println("[+] Teleport Requested!");

		GTA5::UpdateBlips(dma);

		Vector3 Position = GTA5::GetWaypointLocation();

		if (Position.x == 0.0f)
		{
			std::println("  [-] No Waypoint Found");
			m_RequestedTeleport = false;
			return 0;
		}

		if (Position.z == 20.0f)
			Position.z = -255.0f;
		else
			Position.z += 2.0f;

		SetPlayerLocation(dma, Position);

		m_RequestedTeleport = false;
	}

	return 1;
}

bool Teleport::SetPlayerLocation(DMA* dma, Vector3& Location)
{
	m_StartingLocation = GTA5::m_LocalPED_PlayerInfo.m_Location;

	auto vmsh = VMMDLL_Scatter_Initialize(dma->m_vmh, dma->m_PID, VMMDLL_FLAG_NOCACHE);

	uintptr_t PlayerPositionAddress = GTA5::m_LocalPED_NavigationAddr + Offsets::PlayerPosition;
	uintptr_t VehiclePositionAddress = GTA5::m_LocalPED_VehicleNavigationAddr + Offsets::VehiclePosition;

	VMMDLL_Scatter_PrepareWrite(vmsh, PlayerPositionAddress, (BYTE*)&Location, sizeof(Vector3));

	if (GTA5::m_LocalPED_PlayerInfo.m_InVehicleBits & 0x1)
		VMMDLL_Scatter_PrepareWrite(vmsh, VehiclePositionAddress, (BYTE*)&Location, sizeof(Vector3));

	for (int i = 0; i < 250; i++)
	{
		VMMDLL_Scatter_Execute(vmsh);

		std::this_thread::sleep_for(std::chrono::milliseconds(7));

		GTA5::UpdateLocalPlayerInfo(dma);

		float DistanceFromStart = GTA5::m_LocalPED_PlayerInfo.m_Location.Distance(m_StartingLocation);
		float DistanceFromTarget = GTA5::m_LocalPED_PlayerInfo.m_Location.Distance(Location);

		if (DistanceFromStart > 25.0f && (DistanceFromTarget < 25.0f && DistanceFromTarget > 0.01f))
		{
			std::println("  [-] Teleport Successful");
			break;
		}
		if (i == 249)
			std::println("  [-] Teleport Failed");
	}

	VMMDLL_Scatter_CloseHandle(vmsh);

	return 1;
}