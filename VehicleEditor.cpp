#include "pch.h"

#include "VehicleEditor.h"

#include "VehicleInspector.h"

bool VehicleEditor::OnFrame()
{
	if (!m_VehicleEditor) return 1;

	ImGui::Begin("Vehicle Editor", &m_VehicleEditor);

	ImGui::InputFloat("Mass", &m_VehicleInfo.m_Mass);

	ImGui::InputFloat("Acceleration", &m_VehicleInfo.m_Acceleration);

	ImGui::InputFloat("Brake Force", &m_VehicleInfo.m_BrakeForce);

	ImGui::InputFloat("Deform Multiplier", &m_VehicleInfo.m_DeformMultiplier);

	if (ImGui::Button("Override"))
		m_RequestedOverride = true;

	ImGui::SameLine();

	if (ImGui::Button("Load"))
		m_VehicleInfo = VehicleInspector::m_VehicleInfo;

	ImGui::End();

	return 1;
}

bool VehicleEditor::OnDMAFrame(DMA* dma)
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

	VMMDLL_Scatter_PrepareWrite(vmsh, MassAddr, (BYTE*)&m_VehicleInfo.m_Mass, sizeof(m_VehicleInfo.m_Mass));
	VMMDLL_Scatter_PrepareWrite(vmsh, AccelerationAddr, (BYTE*)&m_VehicleInfo.m_Acceleration, sizeof(m_VehicleInfo.m_Acceleration));
	VMMDLL_Scatter_PrepareWrite(vmsh, BrakeForceAddr, (BYTE*)&m_VehicleInfo.m_BrakeForce, sizeof(m_VehicleInfo.m_BrakeForce));
	VMMDLL_Scatter_PrepareWrite(vmsh, DeformAddr, (BYTE*)&m_VehicleInfo.m_DeformMultiplier, sizeof(m_VehicleInfo.m_DeformMultiplier));

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);

	m_RequestedOverride = false;

	return 1;
}