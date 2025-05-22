#pragma once

#include "../GTA5/GTA5Info.h"

class DMA
{
public:
	VMM_HANDLE m_vmh = 0;
	DWORD m_PID = 0;
	uintptr_t m_BaseAddress = 0;
	std::string m_ProcessName = "";

private:
	bool m_Initialize();
	bool m_GetPID();
	bool m_GetBaseAddress();

public:
	bool Start(const std::string& ProcessName);
	bool Close();

private:

};
