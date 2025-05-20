#pragma once

#include "VehicleInfo.h"

class VehicleInspector
{
public:
	static inline VehicleInfo m_VehicleInfo;
	static inline std::mutex m_VehicleInfoMutex;
public:
	static bool OnFrame();

};