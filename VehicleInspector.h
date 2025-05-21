#pragma once

#include "VehicleInfo.h"

class VehicleInspector
{
public:
	static inline VehicleInfo m_VehicleInfo;
	static inline std::mutex m_VehicleInfoMutex;
	static inline bool m_VehicleInspector = false;
public:
	static bool OnFrame();

};