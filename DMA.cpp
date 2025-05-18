#include "pch.h"

#include "DMA.h"

bool DMA::Close()
{
	if (m_vmh)
	{
		VMMDLL_Close(m_vmh);
		m_vmh = 0;
	}

	m_PID = 0;
	m_BaseAddress = 0;
	m_ProcessName = "";

	return 1;
}

bool DMA::Start(const std::string& ProcessName)
{
	m_ProcessName = ProcessName;

	m_Initialize();
	m_GetPID();
	m_GetBaseAddress();

	std::println("{0:s} with PID {1:d} found @ {2:X}", m_ProcessName.c_str(), m_PID, m_BaseAddress);

	return 1;
}

bool DMA::m_Initialize()
{
	LPCSTR args[] = { "","-device","FPGA","-norefresh" };

	m_vmh = VMMDLL_Initialize(4, args);

	if (!m_vmh)
		throw std::runtime_error("Failed to initialize VMM DLL");

	return 1;
}

bool DMA::m_GetPID()
{
	VMMDLL_PidGetFromName(m_vmh, m_ProcessName.c_str(), &m_PID);

	if (!m_PID)
		throw std::runtime_error("Failed to get PID from process name");

	return 1;
}

bool DMA::m_GetBaseAddress()
{
	m_BaseAddress = VMMDLL_ProcessGetModuleBaseU(m_vmh, m_PID, m_ProcessName.c_str());

	if (!m_BaseAddress)
		throw std::runtime_error("Failed to get base address from process name");

	return 1;
}