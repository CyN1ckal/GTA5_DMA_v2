#pragma once
#include <cstdint>
#include "MyVector.h"
struct PlayerInfo
{
	float m_CurrentHealth = 0.0f;
	float m_MaxHealth = 0.0f;
	uint32_t m_GodModeBits = 0x0;
	uint32_t m_AmmoModifierBits = 0x0;
	int32_t m_WantedLevel = 0x0;
	Vector3 m_Location = { 0.0f, 0.0f, 0.0f };
	uint32_t m_InVehicleBits = false;
};