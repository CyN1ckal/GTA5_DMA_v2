#include "../../pch.h"

#include "VehicleInspector.h"

bool VehicleInspector::OnFrame()
{
	if (!m_VehicleInspector) return 1;

	std::scoped_lock VehicleInfoLock(VehicleInspector::m_VehicleInfoMutex);

	ImGuiWindowFlags wnd = ImGuiWindowFlags_AlwaysAutoResize;

	auto& Info = m_VehicleInfo;

	ImGui::Begin("Vehicle Inspector", &m_VehicleInspector, wnd);

	ImGui::Text(std::format("Health: {0:.2f}/{1:.2f}", Info.m_Health, Info.m_MaxHealth).c_str());
	ImGui::Text(std::format("Acceleration: {0:.2f}", Info.m_Acceleration).c_str());
	ImGui::Text(std::format("Mass: {0:.2f}", Info.m_Mass).c_str());
	ImGui::Text(std::format("Brake Force: {0:.2f}", Info.m_BrakeForce).c_str());
	ImGui::Text(std::format("Deform Multiplier: {0:.2f}", Info.m_DeformMultiplier).c_str());
	ImGui::Text(std::format("God Bits: {0:X}", Info.m_GodBits).c_str());
	ImGui::Text(std::format("Position: {0:.2f} {1:.2f} {2:.2f}", Info.m_Position.x, Info.m_Position.y, Info.m_Position.z).c_str());

	ImVec4 PrimaryColor = Info.PrimaryColor.ToImVec4();
	ImGui::ColorEdit4("Primary Color", &PrimaryColor.x);

	ImVec4 SecondaryColor = Info.SecondaryColor.ToImVec4();
	ImGui::ColorEdit4("Secondary Color", &SecondaryColor.x);

	ImVec4 WheelColor = Info.WheelColor.ToImVec4();
	ImGui::ColorEdit4("Wheel Color", &WheelColor.x);

	ImGui::End();

	return 1;
}