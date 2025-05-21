#pragma once
#include <cstdint>
#include "MyVector.h"

struct VehicleInfo
{
	uint32_t m_GodBits;
	float m_Health;
	float m_MaxHealth;
	Vector3 m_Position;
	float m_Mass;
	float m_Acceleration;
	float m_BrakeForce;
	float m_DeformMultiplier;
};