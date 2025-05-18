#pragma once

/* 
  brief: Holds the information necessary for pattern scanning  
*/
struct PatternInfo
{
public:
	std::string Pattern;
	std::string Mask;
	std::string ModuleName;

public:
	bool IsWildCard(int index)
	{
		if (index >= Mask.size())
			return false;

		if (Mask[index] == '?')
			return true;

		return false;
	}

};

/*
  brief: The multi scan class allows for a 1 time copy of the module
		 sections into a buffer. This allows for faster scanning of
		 multiple patterns in the same module.
*/
class MultiScan
{
public:
	bool Initialize(VMM_HANDLE vmh, DWORD PID, std::string ModuleName);
	uintptr_t Scan(PatternInfo pi);
	uintptr_t ScanOffset(PatternInfo pi);
	uintptr_t ScanSectionOffset(PatternInfo pi);
	bool Close();
	BYTE* GetBuffer();

private:
	std::unique_ptr<BYTE[]> m_pBuffer;
	DWORD m_BufferSize;
	uintptr_t m_ModuleBase;
	uintptr_t m_SectionBase;
};