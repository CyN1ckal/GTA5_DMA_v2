#include "pch.h"

#include "Scheduler.h"

#include "DMA.h"

#include "GTA5Info.h"

#include "Features.h"

bool g_Alive = true;

int main() {

	DMA dma;

	dma.Start("GTA5_Enhanced.exe");

	GTA5::FindPointers(&dma);

	unsigned int max_n_threads = 1;
	Bosma::Scheduler s(max_n_threads);

	GTA5::UpdateWorldAddress(&dma);
	GTA5::UpdateLocalPlayerAddr(&dma);

	s.every(std::chrono::seconds(10), GTA5::UpdateLocalPlayerAddr, &dma);
	s.every(std::chrono::milliseconds(7), GTA5::UpdateLocalPlayerInfo, &dma);
	s.every(std::chrono::milliseconds(7), GTA5::FeatureLoop, &dma);

	/* pseudo code */
	//s.every(std::chrono::seconds(5), UpdatePedestrianList);
	//s.every(std::chrono::milliseconds(7), UpdateLocalPlayer);
	//s.every(std::chrono::milliseconds(7), UpdatePedistrians);

	while (g_Alive)
	{
		if (GetAsyncKeyState(VK_END)) g_Alive = false;
		if (GetAsyncKeyState(VK_INSERT)) GodMode::m_GodMode = !GodMode::m_GodMode;
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
	}

	dma.Close();

	return 1;
}