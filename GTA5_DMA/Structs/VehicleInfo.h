#pragma once
#include <cstdint>
#include "MyVector.h"

struct color_t
{
public:
	uint8_t b = 0; 
	uint8_t g = 0; 
	uint8_t r = 0; 
	uint8_t a = 0; 
public:
	uint32_t ToU32()
	{
		uint32_t Return = (b << 24) + (g << 16) + (r << 8) + a;
		
		return Return;
	}
	ImVec4 ToImVec4()
	{
		ImVec4 Return = { 0.0f,0.0f,0.0f,0.0f };

		Return.x = static_cast<float>(r) / static_cast<float>(MAXUINT8);
		Return.y = static_cast<float>(g) / static_cast<float>(MAXUINT8);
		Return.z = static_cast<float>(b) / static_cast<float>(MAXUINT8);
		Return.w = static_cast<float>(a) / static_cast<float>(MAXUINT8);

		return Return;
	}
	static color_t FromImVec4(ImVec4 Input)
	{
		color_t Return;
		Return.r = Input.x * MAXUINT8;
		Return.g = Input.y * MAXUINT8;
		Return.b = Input.z * MAXUINT8;
		Return.a = Input.w * MAXUINT8;
		return Return;
	}
};

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
	color_t PrimaryColor;
	color_t SecondaryColor;
	color_t WheelColor;
};