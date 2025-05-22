#pragma once

#include "../../Structs/WeaponInfo.h"

class WeaponInspector
{
public:
	static bool OnFrame();

public:
	static inline std::mutex m_WeaponInspectorMutex;
	static inline WeaponInfo m_CurrentWeaponInfo;
	static inline bool m_WeaponInspector = false;
};