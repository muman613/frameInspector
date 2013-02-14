#ifndef	__CPUPACK_H__
#define	__CPUPACK_H__

#ifdef __GNUC__

typedef struct cpuPack {
	unsigned	long	maxCPUID;			// Offset		0
	char				CPUTag[12];			// Offset		4, 8, 12

	unsigned	long	CPUSig_Mask:4;		// Offset		16
	unsigned	long	CPUSig_Model:4;
	unsigned	long	CPUSig_Family:4;
	unsigned	long	CPUSig_Type:4;
	unsigned	long	CPUSig_Unused:16;

	unsigned	long	CPUSig_Brand:8;		// Offset		20
	unsigned 	long	CPUSig_CLFLUSH:8;
	unsigned	long	CPUSig_CPUCount:8;
	unsigned	long	CPUSig_APICID:8;

//	unsigned	long	CPUSig_ProcFlags;	// Offset		24
	unsigned	long	CPUSig_Flag_FPU:1;
	unsigned	long	CPUSig_Flag_VME:1;
	unsigned 	long	CPUSig_Flag_DE:1;
	unsigned 	long	CPUSig_Flag_PSE:1;
	unsigned	long	CPUSig_Flag_TSC:1;
	unsigned	long	CPUSig_Flag_MSR:1;
	unsigned	long	CPUSig_Flag_PAE:1;
	unsigned	long	CPUSig_Flag_MCE:1;

	unsigned	long	CPUSig_Flag_CX8:1;
	unsigned	long	CPUSig_Flag_APIC:1;
	unsigned	long	CPUSig_Flag_reserved1:1;
	unsigned	long	CPUSig_Flag_SEP:1;
	unsigned	long	CPUSig_Flag_MTRR:1;
	unsigned	long	CPUSig_Flag_PGE:1;
	unsigned	long	CPUSig_Flag_MCA:1;
	unsigned	long	CPUSig_Flag_CMOV:1;

	unsigned	long	CPUSig_Flag_PAT:1;
	unsigned	long	CPUSig_Flag_PSE36:1;
	unsigned	long	CPUSig_Flag_PSN:1;
	unsigned	long	CPUSig_Flag_CLFL:1;
	unsigned	long	CPUSig_Flag_reserved2:1;
	unsigned	long	CPUSig_Flag_DTES:1;
	unsigned	long	CPUSig_Flag_ACPI:1;
	unsigned	long	CPUSig_Flag_MMX:1;

	unsigned	long	CPUSig_Flag_FXSR:1;
	unsigned	long	CPUSig_Flag_SSE:1;
	unsigned	long	CPUSig_Flag_SSE2:1;
	unsigned	long	CPUSig_Flag_SS:1;
	unsigned	long	CPUSig_Flag_HTT:1;
	unsigned	long	CPUSig_Flag_TM1:1;
	unsigned	long	CPUSig_Flag_IA64:1;
	unsigned	long	CPUSig_Flag_PBE:1;

	unsigned	long	CPUSig_Flag_3DNOW:1;

} CPUPACK;

#else

typedef struct cpuPack {
	unsigned	long	maxCPUID;			// Offset		0
	char				CPUTag[12];			// Offset		4, 8, 12

	unsigned	long	CPUSig_Stepping:4;		// Offset		16
	unsigned	long	CPUSig_Model:4;
	unsigned	long	CPUSig_Family:4;
	unsigned	long	CPUSig_Type:2;
	unsigned	long	CPUSig_Unused1:2;
	unsigned	long	CPUSig_ExtModel:4;		// Offset		20
	unsigned    long    CPUSig_ExtFamily:8;
	unsigned    long    CPUSig_Unused2:4;

    unsigned    long    CPUSig_BrandIndex:8;
	unsigned 	long	CPUSig_CLFLUSH:8;
	unsigned    long    CPUSig_Unused3:8;
	unsigned	long	CPUSig_APICID:8;

    unsigned    long    CPUSig_SSE3:1;
    unsigned    long    CPUSig_Unused4:2;
    unsigned    long    CPUSig_Monitor:1;
    unsigned    long    CPUSig_CPL:1;
    unsigned    long    CPUSig_Unused5:3;
    unsigned    long    CPUSig_ThermalMon:1;
    unsigned    long    CPUSig_Unused6:1;
    unsigned    long    CPUSig_L1Context:1;
    unsigned    long    CPUSig_Unused7:21;

    /* Feature information */
	unsigned	long	CPUSig_Flag_FPU:1;
	unsigned	long	CPUSig_Flag_VME:1;
	unsigned 	long	CPUSig_Flag_DE:1;
	unsigned 	long	CPUSig_Flag_PSE:1;
	unsigned	long	CPUSig_Flag_TSC:1;
	unsigned	long	CPUSig_Flag_MSR:1;
	unsigned	long	CPUSig_Flag_PAE:1;
	unsigned	long	CPUSig_Flag_MCE:1;

	unsigned	long	CPUSig_Flag_CX8:1;
	unsigned	long	CPUSig_Flag_APIC:1;
	unsigned	long	CPUSig_Flag_reserved1:1;
	unsigned	long	CPUSig_Flag_SEP:1;
	unsigned	long	CPUSig_Flag_MTRR:1;
	unsigned	long	CPUSig_Flag_PGE:1;
	unsigned	long	CPUSig_Flag_MCA:1;
	unsigned	long	CPUSig_Flag_CMOV:1;

	unsigned	long	CPUSig_Flag_PAT:1;
	unsigned	long	CPUSig_Flag_PSE36:1;
	unsigned	long	CPUSig_Flag_PSN:1;
	unsigned	long	CPUSig_Flag_CLFSH:1;
	unsigned	long	CPUSig_Flag_reserved2:1;
	unsigned	long	CPUSig_Flag_DTES:1;
	unsigned	long	CPUSig_Flag_ACPI:1;
	unsigned	long	CPUSig_Flag_MMX:1;

	unsigned	long	CPUSig_Flag_FXSR:1;
	unsigned	long	CPUSig_Flag_SSE:1;
	unsigned	long	CPUSig_Flag_SSE2:1;
	unsigned	long	CPUSig_Flag_SS:1;
	unsigned	long	CPUSig_Flag_HTT:1;
	unsigned	long	CPUSig_Flag_TM1:1;
	unsigned	long	CPUSig_Flag_IA64:1;
	unsigned	long	CPUSig_Flag_PBE:1;

	unsigned	long	CPUSig_Flag_3DNOW:1;

} CPUPACK;

#endif
#endif	// __CPUPACK_H__


