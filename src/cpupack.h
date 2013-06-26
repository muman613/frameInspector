#ifndef	__CPUPACK_H__
#define	__CPUPACK_H__

#include <stdint.h>

typedef struct cpuPack {
	uint32_t	        maxCPUID;			// Offset		0
	char				CPUTag[12];			// Offset		4, 8, 12

	uint32_t	        CPUSig_Mask:4;		// Offset		16
	uint32_t	        CPUSig_Model:4;
	uint32_t	        CPUSig_Family:4;
	uint32_t	        CPUSig_Type:4;
	uint32_t	        CPUSig_Unused:16;

	uint32_t	        CPUSig_Brand:8;		// Offset		20
	uint32_t	        CPUSig_CLFLUSH:8;
	uint32_t	        CPUSig_CPUCount:8;
	uint32_t	        CPUSig_APICID:8;

//	uint32_t	CPUSig_ProcFlags;	// Offset		24
	uint32_t	        CPUSig_Flag_FPU:1;
	uint32_t	        CPUSig_Flag_VME:1;
	uint32_t	        CPUSig_Flag_DE:1;
	uint32_t	        CPUSig_Flag_PSE:1;
	uint32_t	        CPUSig_Flag_TSC:1;
	uint32_t	        CPUSig_Flag_MSR:1;
	uint32_t	        CPUSig_Flag_PAE:1;
	uint32_t	        CPUSig_Flag_MCE:1;

	uint32_t	        CPUSig_Flag_CX8:1;
	uint32_t	        CPUSig_Flag_APIC:1;
	uint32_t	        CPUSig_Flag_reserved1:1;
	uint32_t	        CPUSig_Flag_SEP:1;
	uint32_t	        CPUSig_Flag_MTRR:1;
	uint32_t	        CPUSig_Flag_PGE:1;
	uint32_t	        CPUSig_Flag_MCA:1;
	uint32_t	        CPUSig_Flag_CMOV:1;

	uint32_t	        CPUSig_Flag_PAT:1;
	uint32_t	        CPUSig_Flag_PSE36:1;
	uint32_t	        CPUSig_Flag_PSN:1;
	uint32_t	        CPUSig_Flag_CLFL:1;
	uint32_t	        CPUSig_Flag_reserved2:1;
	uint32_t	        CPUSig_Flag_DTES:1;
	uint32_t	        CPUSig_Flag_ACPI:1;
	uint32_t	        CPUSig_Flag_MMX:1;

	uint32_t	        CPUSig_Flag_FXSR:1;
	uint32_t	        CPUSig_Flag_SSE:1;
	uint32_t	        CPUSig_Flag_SSE2:1;
	uint32_t	        CPUSig_Flag_SS:1;
	uint32_t	        CPUSig_Flag_HTT:1;
	uint32_t	        CPUSig_Flag_TM1:1;
	uint32_t	        CPUSig_Flag_IA64:1;
	uint32_t	        CPUSig_Flag_PBE:1;

	uint32_t	        CPUSig_Flag_3DNOW:1;

} CPUPACK;

#endif	// __CPUPACK_H__
