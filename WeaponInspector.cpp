#include "pch.h"

#include "WeaponInspector.h"

#include "WeaponNameMap.h"

bool WeaponInspector::OnFrame()
{
	std::scoped_lock WeaponInspectorLock(WeaponInspector::m_WeaponInspectorMutex);

	ImGui::Begin("Weapon Inspector");

		std::string WeaponName;
		if (WeaponNameMap.count(m_CurrentWeaponInfo.m_WeaponName))
			WeaponName = WeaponNameMap.at(m_CurrentWeaponInfo.m_WeaponName);
		else
			WeaponName = std::format("{0:X}", m_CurrentWeaponInfo.m_WeaponName);

	ImGui::Text(std::format("Weapon Name: {0:s}", WeaponName.c_str()).c_str());
	ImGui::Text(std::format("Impact Type: {0:X}",m_CurrentWeaponInfo.m_ImpactType).c_str());
	ImGui::Text(std::format("Impact Explosion: {0:X}",m_CurrentWeaponInfo.m_ImpactExplosion).c_str());
	ImGui::Text(std::format("Damage: {0:.2f}",m_CurrentWeaponInfo.m_WeaponDamage).c_str());
	ImGui::Text(std::format("Penetration: {0:.2f}",m_CurrentWeaponInfo.m_WeaponPenetration).c_str());
	ImGui::Text(std::format("Reload Mult: {0:.2f}",m_CurrentWeaponInfo.m_WeaponReloadMultiplier).c_str());
	ImGui::Text(std::format("Fire Rate: {0:.2f}",m_CurrentWeaponInfo.m_WeaponFireRate).c_str());
	ImGui::Text(std::format("Recoil: {0:.2f}",m_CurrentWeaponInfo.m_WeaponRecoilAmplitude).c_str());
	ImGui::Text(std::format("Range: {0:.2f}",m_CurrentWeaponInfo.m_WeaponRange).c_str());

	ImGui::End();

	return 1;
}