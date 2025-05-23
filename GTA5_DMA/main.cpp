#include "pch.h"

#include "Scheduler.h"

#include "Core/DMA/DMA.h"

#include "Core/GTA5/GTA5Info.h"

#include "Features/Features.h"

#include "GUI/MyImGui.h"

#include "Core/Config/Config.h"

bool g_Alive = true;

DMA* pDMA = nullptr;

BOOL WINAPI CtrlHandler(DWORD fdwCtrlType)
{
	switch (fdwCtrlType)
	{
	// CTRL-CLOSE: confirm that the user wants to exit.
	case CTRL_CLOSE_EVENT:
		pDMA->Close();
		Config::SaveConfig();
		return FALSE;

	default:
		return FALSE;
	}
}

int main() {
	if (!SetConsoleCtrlHandler(CtrlHandler, TRUE))
		return 0;

	Config::LoadConfig();

	DMA dma;
	pDMA = &dma;
	try
	{
		dma.Start("GTA5_Enhanced.exe");
		GTA5::FindOffsets(&dma);
		GTA5::UpdateWorldAddress(&dma);
		GTA5::UpdateLocalPlayerAddr(&dma);
		MyImGui::Initialize();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;

		if (dma.m_vmh)
			dma.Close();

		system("pause");

		return 0;
	}

	{ /* Bosma::Scheduler uses scope deletion */
		unsigned int max_n_threads = 1;
		Bosma::Scheduler s(max_n_threads);

		s.every(std::chrono::milliseconds(7), GTA5::UpdateLocalPlayerInfo, &dma);
		s.every(std::chrono::milliseconds(7), GTA5::FeatureLoop, &dma);
		s.every(std::chrono::milliseconds(500), GTA5::UpdateWeaponInfo, &dma);
		s.every(std::chrono::milliseconds(500), GTA5::UpdateVehicleInfo, &dma);
		s.every(std::chrono::seconds(1), GTA5::UpdateBlips, &dma);
		s.every(std::chrono::seconds(10), GTA5::UpdateLocalPlayerAddr, &dma);
		s.every(std::chrono::seconds(10), GTA5::UpdateWorldAddress, &dma);

		while (g_Alive)
		{
			ZoneScopedN("Render Loop");

			if (GetAsyncKeyState(VK_END) & 1) g_Alive = false;

			if (GetAsyncKeyState(VK_INSERT) & 1) Fuser::m_Fuser = !Fuser::m_Fuser;

			MyImGui::OnFrame();
		}
	}

	dma.Close();

	MyImGui::Close();

	Config::SaveConfig();

	return 1;
}