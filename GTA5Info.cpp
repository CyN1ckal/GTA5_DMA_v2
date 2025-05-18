#include "pch.h"

#include "GTA5Info.h"

#include "DMA.h"

#include "Zydis/Zydis.h"

bool GTA5::m_FindWorldPtr(DMA* dma)
{
	PatternInfo pi;
	pi.ModuleName = dma->m_ProcessName;
	pi.Pattern = Patterns::WorldPattern;
	pi.Mask = Patterns::WorldMask;

	auto SectionOffset = m_Scan.ScanSectionOffset(pi);
	auto RuntimeAddress = m_Scan.Scan(pi);

	if (!SectionOffset || !RuntimeAddress) return 0;

	ZydisDisassembledInstruction Instruction;

	if (!ZYAN_SUCCESS(ZydisDisassembleIntel(ZYDIS_MACHINE_MODE_LONG_64, RuntimeAddress, m_Scan.GetBuffer() + SectionOffset, 0x15, &Instruction)))
	{
		std::println("Failed to disassemble instruction!");
		return 0;
	}

	uintptr_t WorldPtr = Instruction.operands[1].mem.disp.value + RuntimeAddress + Instruction.info.length;

	std::println("Displacement: {0:X}", Instruction.operands[1].mem.disp.value);
	std::println("Length: {0:X}", Instruction.info.length);
	std::println("WorldPtr: {0:X}", WorldPtr);

	return 1;
}

bool GTA5::FindPointers(DMA* dma)
{
	m_Scan.Initialize(dma->m_vmh, dma->m_PID, dma->m_ProcessName);

	m_FindWorldPtr(dma);

	m_Scan.Close();

	return 1;
}