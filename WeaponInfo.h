#pragma once
#include <cstdint>

struct WeaponInfo
{
	uint32_t m_WeaponName;
	int32_t m_ImpactType;
	int32_t m_ImpactExplosion;
	float m_WeaponDamage;
	float m_WeaponPenetration;
	float m_WeaponReloadMultiplier;
	float m_WeaponFireRate;
	float m_WeaponRecoilAmplitude;
	float m_WeaponRange;
};