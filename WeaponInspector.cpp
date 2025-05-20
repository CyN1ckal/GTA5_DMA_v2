#include "pch.h"

#include "WeaponInspector.h"

bool WeaponInspector::OnFrame()
{
	ImGui::Begin("Weapon Inspector");

	ImGui::Text("%s", std::format("Weapon Name: {0:X}",m_CurrentWeaponInfo.m_WeaponName).c_str());
	ImGui::Text("%s", std::format("Impact Type: {0:X}",m_CurrentWeaponInfo.m_ImpactType).c_str());
	ImGui::Text("%s", std::format("Impact Explosion: {0:X}",m_CurrentWeaponInfo.m_ImpactExplosion).c_str());
	ImGui::Text("%s", std::format("Damage: {0:.2f}",m_CurrentWeaponInfo.m_WeaponDamage).c_str());
	ImGui::Text("%s", std::format("Penetration: {0:.2f}",m_CurrentWeaponInfo.m_WeaponPenetration).c_str());
	ImGui::Text("%s", std::format("Reload Mult: {0:.2f}",m_CurrentWeaponInfo.m_WeaponReloadMultiplier).c_str());
	ImGui::Text("%s", std::format("Fire Rate: {0:.2f}",m_CurrentWeaponInfo.m_WeaponFireRate).c_str());
	ImGui::Text("%s", std::format("Recoil: {0:.2f}",m_CurrentWeaponInfo.m_WeaponRecoilAmplitude).c_str());

	ImGui::End();

	return 1;
}