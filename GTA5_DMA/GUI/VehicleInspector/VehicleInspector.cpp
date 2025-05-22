#include "../../pch.h"

#include "VehicleInspector.h"

bool VehicleInspector::OnFrame()
{
	if (!m_VehicleInspector) return 1;

	std::scoped_lock VehicleInfoLock(VehicleInspector::m_VehicleInfoMutex);

	ImGuiWindowFlags wnd = ImGuiWindowFlags_AlwaysAutoResize;

	ImGui::Begin("Vehicle Inspector", &m_VehicleInspector, wnd);

	ImGui::Text(std::format("Health: {0:.2f}/{1:.2f}", m_VehicleInfo.m_Health, m_VehicleInfo.m_MaxHealth).c_str());
	ImGui::Text(std::format("Acceleration: {0:.2f}", m_VehicleInfo.m_Acceleration).c_str());
	ImGui::Text(std::format("Mass: {0:.2f}", m_VehicleInfo.m_Mass).c_str());
	ImGui::Text(std::format("Brake Force: {0:.2f}", m_VehicleInfo.m_BrakeForce).c_str());
	ImGui::Text(std::format("Deform Multiplier: {0:.2f}", m_VehicleInfo.m_DeformMultiplier).c_str());
	ImGui::Text(std::format("God Bits: {0:X}", m_VehicleInfo.m_GodBits).c_str());
	ImGui::Text(std::format("Position: {0:.2f} {1:.2f} {2:.2f}", m_VehicleInfo.m_Position.x, m_VehicleInfo.m_Position.y, m_VehicleInfo.m_Position.z).c_str());

	ImGui::End();

	return 1;
}