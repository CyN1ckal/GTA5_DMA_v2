#pragma once

#include "../../Structs/BlipInfo.h"

class BlipInspector
{
public:
	static inline bool m_BlipInspector = false;

	static inline std::vector<BlipInfo> m_Blips;

	static inline std::mutex m_BlipInspectorMutex;
	static inline Vector3 m_LastPlayerPosition = { 0.0f,0.0f,0.0f };

public:
	static bool OnFrame();
};