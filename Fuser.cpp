#include "pch.h"

#include "Fuser.h"

#include "Features.h"

#include <algorithm>

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

	ImGuiWindowFlags wnd = ImGuiWindowFlags_NoDecoration;

	ImGui::SetNextWindowSize({ static_cast<float>(m_Width), static_cast<float>(m_Height) });

	ImGui::Begin("Fuser", &m_Fuser, wnd);

	std::vector<std::string> InfoStrings;

	InfoStrings.push_back("Made with love by CyNickal");

	if (GodMode::m_GodMode)
		InfoStrings.push_back("God Mode");

	if (InfiniteAmmo::m_InfiniteAmmo)
		InfoStrings.push_back("Infinite Ammo");

	std::sort(InfoStrings.begin(), InfoStrings.end(), [](std::string a, std::string b) {return a.size() > b.size(); });

	auto TextHeight = ImGui::CalcTextSize("a").y;

	ImVec2 CursorPos = { 0,0 };
	ImGui::SetCursorPos(CursorPos);

	for (auto& Str : InfoStrings)
	{
		ImGui::SetCursorPos(CursorPos);
		ImGui::Text(Str.c_str());
		CursorPos.y += TextHeight;
	}

	ImGui::End();

	ImGui::PopStyleColor(2);

	return 1;
}