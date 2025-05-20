#include "pch.h"

#include "VehicleInspector.h"

bool VehicleInspector::OnFrame()
{
	std::scoped_lock VehicleInfoLock(VehicleInspector::m_VehicleInfoMutex);

	ImGui::Begin("Vehicle Inspector");

	ImGui::Text(std::format("Health: {0:.2f}/{1:.2f}", m_VehicleInfo.m_Health, m_VehicleInfo.m_MaxHealth).c_str());
	ImGui::Text(std::format("God Bits: {0:X}", m_VehicleInfo.m_GodBits).c_str());
	ImGui::Text(std::format("Position: {0:.2f} {1:.2f} {2:.2f}", m_VehicleInfo.m_Position.x, m_VehicleInfo.m_Position.y, m_VehicleInfo.m_Position.z).c_str());

	ImGui::End();

	return 1;
}