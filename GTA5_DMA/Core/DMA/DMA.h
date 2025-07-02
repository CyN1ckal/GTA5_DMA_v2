#pragma once

class Mem
{
public:
	static constexpr inline bool bDebugMessages = false;

	VMM_HANDLE m_vmh = 0;
	DWORD m_PID = 0;

public:
	template <typename T> T Read(uintptr_t Address)
	{
		T Value;

		DWORD BytesRead = 0x0;

		auto vmsh = VMMDLL_Scatter_Initialize(m_vmh, m_PID, VMMDLL_FLAG_NOCACHE);

		VMMDLL_Scatter_PrepareEx(vmsh, Address, sizeof(T), (BYTE*)&Value, &BytesRead);

		VMMDLL_Scatter_ExecuteRead(vmsh);

		VMMDLL_Scatter_CloseHandle(vmsh);

		if (BytesRead != sizeof(T))
		{
			std::println("[Memory] Incomplete Read @ 0x{0:X}", Address);
			return T();
		}

		return Value;
	};

	template <typename T> T ReadFromPID(uintptr_t Address, DWORD PID)
	{
		T Value;

		DWORD BytesRead = 0x0;

		auto vmsh = VMMDLL_Scatter_Initialize(m_vmh, PID, VMMDLL_FLAG_NOCACHE);

		VMMDLL_Scatter_PrepareEx(vmsh, Address, sizeof(T), (BYTE*)&Value, &BytesRead);

		VMMDLL_Scatter_ExecuteRead(vmsh);

		VMMDLL_Scatter_CloseHandle(vmsh);

		if (BytesRead != sizeof(T))
		{
			std::println("[Memory] Incomplete Read @ 0x{0:X}", Address);
			return 0;
		}

		return Value;
	};

	template <typename T> bool Write(uintptr_t Address, T Value)
	{
		auto vmsh = VMMDLL_Scatter_Initialize(m_vmh, m_PID, VMMDLL_FLAG_NOCACHE);

		VMMDLL_Scatter_PrepareWrite(vmsh, Address, (BYTE*)&Value, sizeof(T));

		VMMDLL_Scatter_Execute(vmsh);

		VMMDLL_Scatter_CloseHandle(vmsh);

		return true;
	};

public:
	const bool IsSet() const
	{
		if (!m_vmh) return false;

		return true;
	}

	const bool LightRefresh() const
	{
		if (!IsSet()) return false;

		if (bDebugMessages)
			std::println("[Memory] Light Refresh");

		VMMDLL_ConfigSet(m_vmh, VMMDLL_OPT_REFRESH_FREQ_TLB, 1);

		return true;
	};

	const bool FullRefresh() const
	{
		if (!IsSet()) return false;

		if (bDebugMessages)
			std::println("[Memory] Full Refresh");

		VMMDLL_ConfigSet(m_vmh, VMMDLL_OPT_REFRESH_ALL, 1);

		return true;
	};

	const bool Initialize()
	{
		if (IsSet()) return true;

		LPCSTR args[] = { "","-device","FPGA","-norefresh" };

		m_vmh = VMMDLL_Initialize(4, args);

		if (!m_vmh)
		{
			std::println("[Memory] Failed to initialize VMM DLL");
			return false;
		}

		std::println("[Memory] VMM DLL initialized successfully");

		return true;
	};

	const bool SetPID(std::string m_ProcessName)
	{
		if (!IsSet()) return false;

		DWORD PID = 0x0;

		if (!VMMDLL_PidGetFromName(m_vmh, m_ProcessName.c_str(), &PID))
		{
			std::println("[Memory] Failed to get PID from process name: {0:s}", m_ProcessName);
			return false;
		}

		m_PID = PID;

		return true;
	};

	const bool Reinitialize()
	{
		if (!IsSet()) return false;

		std::println("[Memory] Reinitializing VMM DLL...");

		Close();

		Initialize();

		return true;
	};

	const bool Close()
	{
		if (!IsSet()) return true;

		VMMDLL_Close(m_vmh);

		m_vmh = 0;

		return true;
	}

	const DWORD GetPID(std::string m_ProcessName) const
	{
		if (!IsSet()) return 0;

		DWORD PID = 0x0;

		VMMDLL_PidGetFromName(m_vmh, m_ProcessName.c_str(), &PID);

		if (!PID)
		{
			std::println("[Memory] Failed to get PID from process name: {0:s}", m_ProcessName);
			return 0;
		}

		return PID;
	}

	const uintptr_t GetModuleBase(std::string ModuleName) const
	{
		uintptr_t Base = VMMDLL_ProcessGetModuleBaseU(m_vmh, m_PID, ModuleName.c_str());

		if (!Base)
		{
			std::println("[Memory] Failed to get base address from module name: {0:s}", ModuleName);
			return 0;
		}

		return Base;
	}

public:
	static bool LightRefreshWrapper(Mem* mem)
	{
		return mem->LightRefresh();
	};
};
