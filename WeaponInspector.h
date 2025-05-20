#pragma once

#include "WeaponInfo.h"

class WeaponInspector
{
public:
	static bool OnFrame();

public:
	static inline std::mutex m_WeaponInspectorMutex;
	static inline WeaponInfo m_CurrentWeaponInfo;
};