#include "pch.h"

#include "Scheduler.h"

#include "DMA.h"

#include "GTA5Info.h"

#include "Features.h"

#include "MyImGui.h"

bool g_Alive = true;

int main() {
	DMA dma;

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

			MyImGui::OnFrame();
		}
	}

	dma.Close();

	MyImGui::Close();

	return 1;
}