#include "../pch.h"

#include "MyImGui.h"

#pragma comment(lib, "d3d11.lib")

#include "../Features/Features.h"

#include "Fonts.h"

extern bool g_Alive;

bool MyImGui::OnFrame()
{
	if (Fonts::IBMPlexMono == nullptr)
		Details::LoadFonts();

	if (Fonts::IBMPlexMono == nullptr)
	{
		std::println("Failed to load fonts!");

		return false;
	}

	ImGui::PushFont(Fonts::IBMPlexMono, 24.0f);

	Fuser::OnFrame();

	WeaponInspector::OnFrame();
	WeaponEditor::OnFrame();

	VehicleInspector::OnFrame();
	VehicleEditor::OnFrame();

	BlipInspector::OnFrame();

	MainMenu::Render();

	ImGui::PopFont();

	return 1;
}

std::string MenuTitle = "CyNickal GTA";
bool MainMenu::Render()
{
	ImGui::PushFont(Fonts::IBMPlexMono, 32.0f);

	auto TextSize = ImGui::CalcTextSize(MenuTitle.data(), MenuTitle.data() + MenuTitle.size());
	const auto& Style = ImGui::GetStyle();

	ImGui::SetNextWindowSizeConstraints({ TextSize.x + ((Style.FramePadding.x + Style.WindowPadding.x) * 2.0f), 200.0f }, { 1000.0f,1000.0f });

	ImGuiWindowFlags wnd = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize;

	ImGui::Begin("Main Menu", &m_MainMenu, wnd);

	auto WindowWidth = ImGui::GetContentRegionAvail().x;

	unsigned int Offset = (WindowWidth - TextSize.x) / 2;
	Offset -= Style.FramePadding.x;

	auto CursorPos = ImGui::GetCursorPos();
	CursorPos.x += Offset;

	ImGui::SetCursorPos(CursorPos);

	ImGui::Text("%s", MenuTitle.c_str());

	ImGui::PopFont();

	if (ImGui::CollapsingHeader("Self"))
	{
		ImGui::Indent();

		ImGui::Checkbox("God Mode", &GodMode::m_GodMode);

		ImGui::Checkbox("Never Wanted", &NeverWanted::m_NeverWanted);

		ImGui::Checkbox("Refresh Health", &RefreshHealth::m_RefreshHealth);
		if (RefreshHealth::m_RefreshHealth)
		{
			ImGui::SameLine();
			ImGui::SetNextItemWidth(-FLT_MIN);
			ImGui::SliderFloat("##Heal Threshold", &RefreshHealth::m_HealthThreshold, 0.1f, 0.99f);
		}

		if (ImGui::Button("Teleport To Waypoint"))
		{
			Teleport::m_RequestedTeleport = true;
		}

		ImGui::Unindent();
	}

	if (ImGui::CollapsingHeader("Vehicle"))
	{
		ImGui::Indent();

		ImGui::Checkbox("Vehicle God Mode", &VehicleGodMode::m_VehicleGodMode);

		ImGui::Checkbox("Vehicle Inspector", &VehicleInspector::m_VehicleInspector);

		ImGui::Checkbox("Vehicle Editor", &VehicleEditor::m_VehicleEditor);

		ImGui::Checkbox("Rainbow Car", &RainbowCar::m_RainbowCar);

		ImGui::Unindent();
	}

	if (ImGui::CollapsingHeader("Weapon"))
	{
		ImGui::Indent();

		ImGui::Checkbox("Weapon Inspector", &WeaponInspector::m_WeaponInspector);

		ImGui::Checkbox("Weapon Editor", &WeaponEditor::m_WeaponEditor);

		ImGui::Checkbox("Infinite Ammo", &InfiniteAmmo::m_InfiniteAmmo);

		ImGui::Unindent();
	}

	if (ImGui::CollapsingHeader("Fuser"))
	{
		ImGui::Indent();

		ImGui::Checkbox("Toggle", &Fuser::m_Fuser);

		ImGui::SetNextItemWidth(120.0f);
		ImGui::InputInt("Monitor", &Fuser::m_MonitorIndex);

		ImGui::SetNextItemWidth(120.0f);
		ImGui::InputInt("Width", &Fuser::m_Width);

		ImGui::SetNextItemWidth(120.0f);
		ImGui::InputInt("Height	", &Fuser::m_Height);

		ImGui::Unindent();
	}

	if (ImGui::CollapsingHeader("Unsorted"))
	{
		ImGui::Indent();

		ImGui::Checkbox("Blip Inspector", &BlipInspector::m_BlipInspector);

		ImGui::Unindent();
	}

	ImGui::End();

	return 1;
}

bool MyImGui::Details::LoadFonts()
{
	Fonts::IBMPlexMono = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(FontData::IBMPlexBold, sizeof(FontData::IBMPlexBold));

	return true;
}
