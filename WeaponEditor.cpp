#include "pch.h"

#include "DMA.h"

#include "WeaponEditor.h"

#include "WeaponInspector.h"

bool WeaponEditor::OnFrame()
{
	if (!m_WeaponEditor) return 1;

	ImGuiWindowFlags wnd = ImGuiWindowFlags_AlwaysAutoResize;

	ImGui::Begin("Weapon Editor", &m_WeaponEditor, wnd);

	ImGui::InputInt("Impact Type", &m_DesiredWeaponInfo.m_ImpactType);
	ImGui::InputInt("Impact Explosion", &m_DesiredWeaponInfo.m_ImpactExplosion);
	ImGui::InputFloat("Damage", &m_DesiredWeaponInfo.m_WeaponDamage);
	ImGui::InputFloat("Penetration", &m_DesiredWeaponInfo.m_WeaponPenetration);
	ImGui::InputFloat("Reload Mult", &m_DesiredWeaponInfo.m_WeaponReloadMultiplier);
	ImGui::InputFloat("Fire Rate", &m_DesiredWeaponInfo.m_WeaponFireRate);
	ImGui::InputFloat("Recoil", &m_DesiredWeaponInfo.m_WeaponRecoilAmplitude);
	ImGui::InputFloat("Range", &m_DesiredWeaponInfo.m_WeaponRange);

	if (ImGui::Button("Override"))
	{
		m_RequestedOverride = true;
	}

	ImGui::SameLine();

	if (ImGui::Button("Load"))
	{
		m_DesiredWeaponInfo = WeaponInspector::m_CurrentWeaponInfo;
	}

	ImGui::End();

	return 1;
}

bool WeaponEditor::OnDMAFrame(DMA* dma)
{
	if (!m_RequestedOverride)
		return 1;

	auto vmsh = VMMDLL_Scatter_Initialize(dma->m_vmh, dma->m_PID, VMMDLL_FLAG_NOCACHE);

	uintptr_t ImpactAddr = GTA5::m_LocalPED_WeaponInfoAddr + Offsets::ImpactType;
	uintptr_t DamageAddr = GTA5::m_LocalPED_WeaponInfoAddr + Offsets::WeaponDamage;
	uintptr_t PenetrationAddr = GTA5::m_LocalPED_WeaponInfoAddr + Offsets::WeaponPenetration;
	uintptr_t ReloadMultiplierAddr = GTA5::m_LocalPED_WeaponInfoAddr + Offsets::WeaponReloadMultiplier;
	uintptr_t FireRateAddr = GTA5::m_LocalPED_WeaponInfoAddr + Offsets::WeaponFireRate;
	uintptr_t RecoilAmplitudeAddr = GTA5::m_LocalPED_WeaponInfoAddr + Offsets::WeaponRecoilAmplitude;
	uintptr_t RangeAddr = GTA5::m_LocalPED_WeaponInfoAddr + Offsets::WeaponRange;

	VMMDLL_Scatter_PrepareWrite(vmsh, ImpactAddr, (BYTE*)&m_DesiredWeaponInfo.m_ImpactType, sizeof(m_DesiredWeaponInfo.m_ImpactType) * 2);
	VMMDLL_Scatter_PrepareWrite(vmsh, DamageAddr, (BYTE*)&m_DesiredWeaponInfo.m_WeaponDamage, sizeof(m_DesiredWeaponInfo.m_WeaponDamage));
	VMMDLL_Scatter_PrepareWrite(vmsh, PenetrationAddr, (BYTE*)&m_DesiredWeaponInfo.m_WeaponPenetration, sizeof(m_DesiredWeaponInfo.m_WeaponPenetration));
	VMMDLL_Scatter_PrepareWrite(vmsh, ReloadMultiplierAddr, (BYTE*)&m_DesiredWeaponInfo.m_WeaponReloadMultiplier, sizeof(m_DesiredWeaponInfo.m_WeaponReloadMultiplier));
	VMMDLL_Scatter_PrepareWrite(vmsh, FireRateAddr, (BYTE*)&m_DesiredWeaponInfo.m_WeaponFireRate, sizeof(m_DesiredWeaponInfo.m_WeaponFireRate));
	VMMDLL_Scatter_PrepareWrite(vmsh, RecoilAmplitudeAddr, (BYTE*)&m_DesiredWeaponInfo.m_WeaponRecoilAmplitude, sizeof(m_DesiredWeaponInfo.m_WeaponRecoilAmplitude));
	VMMDLL_Scatter_PrepareWrite(vmsh, RangeAddr, (BYTE*)&m_DesiredWeaponInfo.m_WeaponRange, sizeof(m_DesiredWeaponInfo.m_WeaponRange));

	VMMDLL_Scatter_Execute(vmsh);

	VMMDLL_Scatter_CloseHandle(vmsh);

	m_RequestedOverride = false;

	return 1;
}