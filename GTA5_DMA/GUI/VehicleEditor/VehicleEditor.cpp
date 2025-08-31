#include "../../pch.h"

#include "VehicleEditor.h"

#include "../../GUI/VehicleInspector/VehicleInspector.h"

#include "Game/GTA5Info.h"

bool VehicleEditor::OnFrame()
{
	if (!m_VehicleEditor) return 1;

	ImGuiWindowFlags wnd = ImGuiWindowFlags_AlwaysAutoResize;

	ImGui::Begin("Vehicle Editor", &m_VehicleEditor, wnd);
	ImGui::PushItemWidth(150);
	ImGui::InputFloat("Mass", &m_VehicleInfo.m_Mass);

	ImGui::InputFloat("Acceleration", &m_VehicleInfo.m_Acceleration);

	ImGui::InputFloat("Brake Force", &m_VehicleInfo.m_BrakeForce);

	ImGui::InputFloat("Deform Multiplier", &m_VehicleInfo.m_DeformMultiplier);

	ImGui::ColorEdit4("Primary Color", &m_DesiredPrimaryColor.x);

	ImGui::ColorEdit4("Secondary Color", &m_DesiredSecondaryColor.x);

	ImGui::ColorEdit4("Wheel Color", &m_DesiredWheelColor.x);
	ImGui::PopItemWidth();

	if (ImGui::Button("Override"))
		m_RequestedOverride = true;

	ImGui::SameLine();

	if (ImGui::Button("Load"))
	{
		m_VehicleInfo = VehicleInspector::m_VehicleInfo;
		m_DesiredPrimaryColor = m_VehicleInfo.PrimaryColor.ToImVec4();
		m_DesiredSecondaryColor = m_VehicleInfo.SecondaryColor.ToImVec4();
		m_DesiredWheelColor = m_VehicleInfo.WheelColor.ToImVec4();
	}

	ImGui::End();

	return 1;
}

bool VehicleEditor::OnDMAFrame(Mem* dma)
{
	if (!m_RequestedOverride)
		return 1;

	if (!GTA5::m_LocalPED_VehicleHandlingAddr)
		return 1;

	auto vmsh = VMMDLL_Scatter_Initialize(dma->m_vmh, dma->m_PID, VMMDLL_FLAG_NOCACHE);

	uintptr_t MassAddr = GTA5::m_LocalPED_VehicleHandlingAddr + Offsets::VehicleMass;
	uintptr_t AccelerationAddr = GTA5::m_LocalPED_VehicleHandlingAddr + Offsets::VehicleAcceleration;
	uintptr_t BrakeForceAddr = GTA5::m_LocalPED_VehicleHandlingAddr + Offsets::BrakeForce;
	uintptr_t DeformAddr = GTA5::m_LocalPED_VehicleHandlingAddr + Offsets::VehicleDeformMult;
	uintptr_t PrimaryColorAddr = GTA5::m_LocalPED_VehicleModelInfoAddr + Offsets::VehiclePrimaryColor;
	uintptr_t SecondaryColorAddr = GTA5::m_LocalPED_VehicleModelInfoAddr + Offsets::VehicleSecondaryColor;
	uintptr_t WheelColorAddr = GTA5::m_LocalPED_VehicleModelInfoAddr + Offsets::VehicleWheelColor;

	color_t PrimaryColor = color_t::FromImVec4(m_DesiredPrimaryColor);
	color_t SecondaryColor = color_t::FromImVec4(m_DesiredSecondaryColor);
	color_t WheelColor = color_t::FromImVec4(m_DesiredWheelColor);

	VMMDLL_Scatter_PrepareWrite(vmsh, MassAddr, (BYTE*)&m_VehicleInfo.m_Mass, sizeof(m_VehicleInfo.m_Mass));
	VMMDLL_Scatter_PrepareWrite(vmsh, AccelerationAddr, (BYTE*)&m_VehicleInfo.m_Acceleration, sizeof(m_VehicleInfo.m_Acceleration));
	VMMDLL_Scatter_PrepareWrite(vmsh, BrakeForceAddr, (BYTE*)&m_VehicleInfo.m_BrakeForce, sizeof(m_VehicleInfo.m_BrakeForce));
	VMMDLL_Scatter_PrepareWrite(vmsh, DeformAddr, (BYTE*)&m_VehicleInfo.m_DeformMultiplier, sizeof(m_VehicleInfo.m_DeformMultiplier));
	VMMDLL_Scatter_PrepareWrite(vmsh, PrimaryColorAddr, (BYTE*)&PrimaryColor, sizeof(PrimaryColor));
	VMMDLL_Scatter_PrepareWrite(vmsh, SecondaryColorAddr, (BYTE*)&SecondaryColor, sizeof(SecondaryColor));
	VMMDLL_Scatter_PrepareWrite(vmsh, WheelColorAddr, (BYTE*)&WheelColor, sizeof(WheelColor));

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);

	m_RequestedOverride = false;

	return 1;
}