#include "pch.h"

#include "Scheduler.h"

#include "DMA/DMA.h"

#include "Game/GTA5Info.h"

#include "Features/Features.h"

#include "GUI/MyImGui.h"

#include "Config/Config.h"

bool g_Alive = true;

Mem* pDMA = nullptr;

BOOL WINAPI CtrlHandler(DWORD fdwCtrlType)
{
	switch (fdwCtrlType)
	{
		// CTRL-CLOSE: confirm that the user wants to exit.
	case CTRL_CLOSE_EVENT:
		if (pDMA)
			pDMA->Close();

		Config::SaveConfig();
		return FALSE;

	default:
		return FALSE;
	}
}

DWORD WINAPI StartingThread(HMODULE hMod)
{
	std::println("GTA5 DLL Starting Thread!");

	if (!SetConsoleCtrlHandler(CtrlHandler, TRUE))
		return 0;

	Config::LoadConfig();

	auto pMem = GTA5_::Initialize();
	if (!pMem)
	{
		std::println("Failed to initialize GTA5 DMA!");

		FreeLibraryAndExitThread(hMod, 0);

		return true;
	}
	pDMA = pMem;

	{ /* Bosma::Scheduler uses scope deletion */
		unsigned int max_n_threads = 1;
		Bosma::Scheduler s(max_n_threads);

		s.every(std::chrono::milliseconds(7), GTA5::UpdateLocalPlayerInfo, pMem);
		s.every(std::chrono::milliseconds(7), GTA5::FeatureLoop, pMem);
		s.every(std::chrono::milliseconds(500), GTA5::UpdateWeaponInfo, pMem);
		s.every(std::chrono::milliseconds(500), GTA5::UpdateVehicleInfo, pMem);
		s.every(std::chrono::seconds(1), GTA5::UpdateBlips, pMem);
		s.every(std::chrono::seconds(10), GTA5::UpdateLocalPlayerAddr, pMem);
		s.every(std::chrono::seconds(10), GTA5::UpdateWorldAddress, pMem);

		while (g_Alive)
		{
			ZoneScopedN("Render Loop");

			if (GetAsyncKeyState(VK_END) & 1) g_Alive = false;

			if (GetAsyncKeyState(VK_INSERT) & 1) Fuser::m_Fuser = !Fuser::m_Fuser;
		}
	}

	GTA5_::Close();

	Config::SaveConfig();

	FreeLibraryAndExitThread(hMod, 0);

	return true;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		std::println("GTA5_DLL DLL_PROCESS_ATTACH");
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)StartingThread, hModule, 0, 0);
		break;
	case DLL_THREAD_ATTACH:
		std::println("GTA5_DLL DLL_THREAD_ATTACH");
		break;
	case DLL_THREAD_DETACH:
		std::println("GTA5_DLL DLL_THREAD_DETACH");
		break;
	case DLL_PROCESS_DETACH:
		std::println("GTA5_DLL DLL_PROCESS_DETACH");
		break;
	}

	return TRUE;
}

extern "C" __declspec(dllexport) void Render(ImGuiContext* ctx)
{
	ImGui::SetCurrentContext(ctx);

	MyImGui::OnFrame();
}