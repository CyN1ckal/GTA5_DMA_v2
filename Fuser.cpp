#include "pch.h"

#include "Fuser.h"

#include "Features.h"

#include <algorithm>

#include "MyImGui.h"

bool Fuser::OnFrame()
{
	if (!m_Fuser)
		return 1;

	static bool FirstRun = true;
	if (FirstRun)
	{
		GetDisplayInfo();
		FirstRun = false;
	}

	ImVec2 WindowPos = ImVec2(rcMonitors[m_MonitorIndex].left, rcMonitors[m_MonitorIndex].top);

	ImGui::SetNextWindowPos(WindowPos, ImGuiCond_Always);

	ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.0f,0.0f,0.0f,1.0f });
	ImGui::PushStyleColor(ImGuiCol_Border, { 0.0f,0.0f,0.0f,0.0f });

	ImGui::PushFont(MyImGui::m_pIBMPlexMono_24);

	ImGuiWindowFlags wnd = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs;

	ImGui::SetNextWindowSize({ static_cast<float>(m_Width), static_cast<float>(m_Height) });

	ImGui::Begin("Fuser", &m_Fuser, wnd);

	std::vector<std::string> InfoStrings;

	if (GodMode::m_GodMode)
		InfoStrings.push_back("Player God Mode");

	if (VehicleGodMode::m_VehicleGodMode)
		InfoStrings.push_back("Vehicle God Mode");

	if (InfiniteAmmo::m_InfiniteAmmo)
		InfoStrings.push_back("Infinite Ammo");

	if (NeverWanted::m_NeverWanted)
		InfoStrings.push_back("Never Wanted");

	std::sort(InfoStrings.begin(), InfoStrings.end(), [](std::string a, std::string b) {return a.size() > b.size(); });

	auto TextHeight = ImGui::CalcTextSize("a").y;

	ImVec2 CursorPos = { 5,5 };
	ImGui::SetCursorPos(CursorPos);

	ImGui::Text("Made with love by CyNickal");
	CursorPos.y += TextHeight;

	ImGui::PushStyleColor(ImGuiCol_Text, { 0.8f,0.05f,0.9f,1.0f });
	for (auto& Str : InfoStrings)
	{
		ImGui::SetCursorPos(CursorPos);
		ImGui::Text(Str.c_str());
		CursorPos.y += TextHeight;
	}
	ImGui::PopStyleColor(1);

	ImGui::End();

	ImGui::PopStyleColor(2);

	ImGui::PopFont();

	return 1;
}