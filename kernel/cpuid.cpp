/* Vendor strings from CPUs. */
#define CPUID_VENDOR_OLDAMD        "AMDisbetter!" // Early engineering samples of AMD K5 processor
#define CPUID_VENDOR_AMD           "AuthenticAMD"
#define CPUID_VENDOR_INTEL         "GenuineIntel"
#define CPUID_VENDOR_VIA           "VIA VIA VIA "
#define CPUID_VENDOR_OLDTRANSMETA  "TransmetaCPU"
#define CPUID_VENDOR_TRANSMETA     "GenuineTMx86"
#define CPUID_VENDOR_CYRIX         "CyrixInstead"
#define CPUID_VENDOR_CENTAUR       "CentaurHauls"
#define CPUID_VENDOR_NEXGEN        "NexGenDriven"
#define CPUID_VENDOR_UMC           "UMC UMC UMC "
#define CPUID_VENDOR_SIS           "SiS SiS SiS "
#define CPUID_VENDOR_NSC           "Geode by NSC"
#define CPUID_VENDOR_RISE          "RiseRiseRise"
#define CPUID_VENDOR_VORTEX        "Vortex86 SoC"
#define CPUID_VENDOR_OLDAO486      "GenuineAO486"
#define CPUID_VENDOR_AO486         "MiSTer AO486"
#define CPUID_VENDOR_ZHAOXIN       "  Shanghai  "
#define CPUID_VENDOR_HYGON         "HygonGenuine"
#define CPUID_VENDOR_ELBRUS        "E2K MACHINE "
 
/* Vendor strings from hypervisors. */
#define CPUID_VENDOR_QEMU          "TCGTCGTCGTCG"
#define CPUID_VENDOR_KVM           " KVMKVMKVM  "
#define CPUID_VENDOR_VMWARE        "VMwareVMware"
#define CPUID_VENDOR_VIRTUALBOX    "VBoxVBoxVBox"
#define CPUID_VENDOR_XEN           "XenVMMXenVMM"
#define CPUID_VENDOR_HYPERV        "Microsoft Hv"
#define CPUID_VENDOR_PARALLELS     " prl hyperv "
#define CPUID_VENDOR_PARALLELS_ALT " lrpepyh vr " // Sometimes Parallels incorrectly encodes "prl hyperv" as "lrpepyh vr" due to an endianness mismatch.
#define CPUID_VENDOR_BHYVE         "bhyve bhyve "
#define CPUID_VENDOR_QNX           " QNXQVMBSQG "

enum {
    CPUID_FEAT_ECX_SSE3         = 1 << 0, 
    CPUID_FEAT_ECX_PCLMUL       = 1 << 1,
    CPUID_FEAT_ECX_DTES64       = 1 << 2,
    CPUID_FEAT_ECX_MONITOR      = 1 << 3,  
    CPUID_FEAT_ECX_DS_CPL       = 1 << 4,  
    CPUID_FEAT_ECX_VMX          = 1 << 5,  
    CPUID_FEAT_ECX_SMX          = 1 << 6,  
    CPUID_FEAT_ECX_EST          = 1 << 7,  
    CPUID_FEAT_ECX_TM2          = 1 << 8,  
    CPUID_FEAT_ECX_SSSE3        = 1 << 9,  
    CPUID_FEAT_ECX_CID          = 1 << 10,
    CPUID_FEAT_ECX_SDBG         = 1 << 11,
    CPUID_FEAT_ECX_FMA          = 1 << 12,
    CPUID_FEAT_ECX_CX16         = 1 << 13, 
    CPUID_FEAT_ECX_XTPR         = 1 << 14, 
    CPUID_FEAT_ECX_PDCM         = 1 << 15, 
    CPUID_FEAT_ECX_PCID         = 1 << 17, 
    CPUID_FEAT_ECX_DCA          = 1 << 18, 
    CPUID_FEAT_ECX_SSE4_1       = 1 << 19, 
    CPUID_FEAT_ECX_SSE4_2       = 1 << 20, 
    CPUID_FEAT_ECX_X2APIC       = 1 << 21, 
    CPUID_FEAT_ECX_MOVBE        = 1 << 22, 
    CPUID_FEAT_ECX_POPCNT       = 1 << 23, 
    CPUID_FEAT_ECX_TSC          = 1 << 24, 
    CPUID_FEAT_ECX_AES          = 1 << 25, 
    CPUID_FEAT_ECX_XSAVE        = 1 << 26, 
    CPUID_FEAT_ECX_OSXSAVE      = 1 << 27, 
    CPUID_FEAT_ECX_AVX          = 1 << 28,
    CPUID_FEAT_ECX_F16C         = 1 << 29,
    CPUID_FEAT_ECX_RDRAND       = 1 << 30,
    CPUID_FEAT_ECX_HYPERVISOR   = 1 << 31,
 
    CPUID_FEAT_EDX_FPU          = 1 << 0,  
    CPUID_FEAT_EDX_VME          = 1 << 1,  
    CPUID_FEAT_EDX_DE           = 1 << 2,  
    CPUID_FEAT_EDX_PSE          = 1 << 3,  
    CPUID_FEAT_EDX_TSC          = 1 << 4,  
    CPUID_FEAT_EDX_MSR          = 1 << 5,  
    CPUID_FEAT_EDX_PAE          = 1 << 6,  
    CPUID_FEAT_EDX_MCE          = 1 << 7,  
    CPUID_FEAT_EDX_CX8          = 1 << 8,  
    CPUID_FEAT_EDX_APIC         = 1 << 9,  
    CPUID_FEAT_EDX_SEP          = 1 << 11, 
    CPUID_FEAT_EDX_MTRR         = 1 << 12, 
    CPUID_FEAT_EDX_PGE          = 1 << 13, 
    CPUID_FEAT_EDX_MCA          = 1 << 14, 
    CPUID_FEAT_EDX_CMOV         = 1 << 15, 
    CPUID_FEAT_EDX_PAT          = 1 << 16, 
    CPUID_FEAT_EDX_PSE36        = 1 << 17, 
    CPUID_FEAT_EDX_PSN          = 1 << 18, 
    CPUID_FEAT_EDX_CLFLUSH      = 1 << 19, 
    CPUID_FEAT_EDX_DS           = 1 << 21, 
    CPUID_FEAT_EDX_ACPI         = 1 << 22, 
    CPUID_FEAT_EDX_MMX          = 1 << 23, 
    CPUID_FEAT_EDX_FXSR         = 1 << 24, 
    CPUID_FEAT_EDX_SSE          = 1 << 25, 
    CPUID_FEAT_EDX_SSE2         = 1 << 26, 
    CPUID_FEAT_EDX_SS           = 1 << 27, 
    CPUID_FEAT_EDX_HTT          = 1 << 28, 
    CPUID_FEAT_EDX_TM           = 1 << 29, 
    CPUID_FEAT_EDX_IA64         = 1 << 30,
    CPUID_FEAT_EDX_PBE          = 1 << 31
};

#include <cpuid.h>
 
/* Example: Get CPU's model number */
static int get_model(void)
{
    int ebx = 0, unused = 0;
    __cpuid(0, unused, ebx, unused, unused);
    return ebx;
}

static int get_family(void)
{
    int ebx = 0, unused = 0;
    __cpuid(0, unused, ebx, unused, unused);
    return (ebx >> 8) & 0xf;
}

/* Example: Check for builtin local APIC. */
static int check_apic(void)
{
    unsigned int eax = 0, unused = 0, edx = 0;
    __get_cpuid(1, &eax, &unused, &unused, &edx);
    return edx & CPUID_FEAT_EDX_APIC;
}

static int check_pae(void)
{
    unsigned int eax = 0, unused = 0, edx = 0;
    __get_cpuid(1, &eax, &unused, &unused, &edx);
    return edx & CPUID_FEAT_EDX_PAE;
}

static int check_pge(void)
{
	unsigned int eax = 0, unused = 0, edx = 0;
	__get_cpuid(1, &eax, &unused, &unused, &edx);
	return edx & CPUID_FEAT_EDX_PGE;
}

static int check_sse(void)
{
    unsigned int eax = 0, unused = 0, edx = 0;
    __get_cpuid(1, &eax, &unused, &unused, &edx);
    return edx & CPUID_FEAT_EDX_SSE;
}

static int check_sse2(void)
{
    unsigned int eax = 0, unused = 0, edx = 0;
    __get_cpuid(1, &eax, &unused, &unused, &edx);
    return edx & CPUID_FEAT_EDX_SSE2;
}

static int check_sse3(void)
{
    unsigned int eax = 0, unused = 0, ecx = 0;
    __get_cpuid(1, &eax, &unused, &ecx, &unused);
    return ecx & CPUID_FEAT_ECX_SSE3;
}

static int check_sse4_1(void)
{
    unsigned int eax = 0, unused = 0, ecx = 0;
    __get_cpuid(1, &eax, &unused, &ecx, &unused);
    return ecx & CPUID_FEAT_ECX_SSE4_1;
}

static int check_sse4_2(void)
{
    unsigned int eax = 0, unused = 0, ecx = 0;
    __get_cpuid(1, &eax, &unused, &ecx, &unused);
    return ecx & CPUID_FEAT_ECX_SSE4_2;
}

static int check_x2apic(void)
{
    unsigned int eax = 0, unused = 0, ecx = 0;
    __get_cpuid(1, &eax, &unused, &ecx, &unused);
    return ecx & CPUID_FEAT_ECX_X2APIC;
}

static int check_movbe(void)
{
    unsigned int eax = 0, unused = 0, ecx = 0;
    __get_cpuid(1, &eax, &unused, &ecx, &unused);
    return ecx & CPUID_FEAT_ECX_MOVBE;
}

static int check_popcnt(void)
{
    unsigned int eax = 0, unused = 0, ecx = 0;
    __get_cpuid(1, &eax, &unused, &ecx, &unused);
    return ecx & CPUID_FEAT_ECX_POPCNT;
}

static int check_tsc(void)
{
    unsigned int eax = 0, unused = 0, ecx = 0;
    __get_cpuid(1, &eax, &unused, &ecx, &unused);
    return ecx & CPUID_FEAT_ECX_TSC;
}

static int check_fpu(void)
{
    unsigned int eax = 0, ecx = 0, edx = 0, ebx = 0;
    __get_cpuid(1, &eax, &ebx, &ecx, &edx);
    return edx & CPUID_FEAT_EDX_FPU;
}

static int check_xsave(void)
{
    unsigned int eax = 0, unused = 0, ecx = 0;
    __get_cpuid(1, &eax, &unused, &ecx, &unused);
    return ecx & CPUID_FEAT_ECX_XSAVE;
}

static char* get_vendor_string(void)
{
    unsigned int unused = 0, ebx = 0, ecx = 0, edx = 0;
    __cpuid(0, unused, ebx, ecx, edx);
    char* vendor_string = (char*)malloc(12);
    vendor_string[0] = (char)(ebx & 0xff);
    vendor_string[1] = (char)((ebx >> 8) & 0xff);
    vendor_string[2] = (char)((ebx >> 16) & 0xff);
    vendor_string[3] = (char)((ebx >> 24) & 0xff);
    vendor_string[4] = (char)(edx & 0xff);
    vendor_string[5] = (char)((edx >> 8) & 0xff);
    vendor_string[6] = (char)((edx >> 16) & 0xff);
    vendor_string[7] = (char)((edx >> 24) & 0xff);
    vendor_string[8] = (char)(ecx & 0xff);
    vendor_string[9] = (char)((ecx >> 8) & 0xff);
    vendor_string[10] = (char)((ecx >> 16) & 0xff);
    vendor_string[11] = (char)((ecx >> 24) & 0xff);
    /*for (int i = 0; i < 4; i++)
        vendor_string[i] = (char)((ebx<<(i*8)));
    for (int i = 0; i < 4; i++)
        vendor_string[i+4] = (char)((ecx<<(i*8)));
    for (int i = 0; i < 4; i++)
        vendor_string[i+8] = (char)((edx<<(i*8)));*/
    return vendor_string;
}

static char* get_virtual_vendor_string(void) {
    unsigned int unused = 0, ebx = 0, ecx = 0, edx = 0;
    __cpuid(0, unused, ebx, ecx, edx);
    char* vendor_string = (char*)malloc(12);
    vendor_string[0] = (char)(ebx & 0xff);
    vendor_string[1] = (char)((ebx >> 8) & 0xff);
    vendor_string[2] = (char)((ebx >> 16) & 0xff);
    vendor_string[3] = (char)((ebx >> 24) & 0xff);
    vendor_string[4] = (char)(edx & 0xff);
    vendor_string[5] = (char)((edx >> 8) & 0xff);
    vendor_string[6] = (char)((edx >> 16) & 0xff);
    vendor_string[7] = (char)((edx >> 24) & 0xff);
    vendor_string[8] = (char)(ecx & 0xff);
    vendor_string[9] = (char)((ecx >> 8) & 0xff);
    vendor_string[10] = (char)((ecx >> 16) & 0xff);
    vendor_string[11] = (char)((ecx >> 24) & 0xff);
    return vendor_string;
}

static const char* get_virtual_vendor(void) {
    if (strcmp(get_virtual_vendor_string(), CPUID_VENDOR_QEMU)) {
        return "QEMU";
    }
    return "Not virtualized or unknown";
}

static const char* get_vendor(void) {
    if (strcmp(get_vendor_string(),"GenuineIntel")) {
        return "Intel";
    } else if (strcmp(get_vendor_string(),CPUID_VENDOR_AMD)) {
        return "AMD";
    } else if (strcmp(get_vendor_string(),CPUID_VENDOR_OLDAMD)) {
        return "AMD (Old)";
    } else if (strcmp(get_vendor_string(),CPUID_VENDOR_VIA)) {
        return "VIA";
    } else if (strcmp(get_vendor_string(),CPUID_VENDOR_AO486)) {
        return "AO486";
    } else if (strcmp(get_vendor_string(),CPUID_VENDOR_OLDAO486)) {
        return "AO486 (Old)";
    } else if (strcmp(get_vendor_string(),CPUID_VENDOR_TRANSMETA)) {
        return "Transmeta";
    } else if (strcmp(get_vendor_string(),CPUID_VENDOR_OLDTRANSMETA)) {
        return "Transmeta (Old)";
    } else if (strcmp(get_vendor_string(),CPUID_VENDOR_NSC)) {
        return "NSC";
    } else if (strcmp(get_vendor_string(),CPUID_VENDOR_RISE)) {
        return "Rise";
    } else if (strcmp(get_vendor_string(),CPUID_VENDOR_SIS)) {
        return "SiS";
    } else if (strcmp(get_vendor_string(),CPUID_VENDOR_UMC)) {
        return "UMC";
    } else if (strcmp(get_vendor_string(),CPUID_VENDOR_NEXGEN)) {
        return "NexGen";
    } else if (strcmp(get_vendor_string(),CPUID_VENDOR_CENTAUR)) {
        return "Centaur";
    } else if (strcmp(get_vendor_string(),CPUID_VENDOR_VORTEX)) {
        return "Vortex";
    } else {
        char* result = (char*)malloc(strlen(get_vendor_string())+strlen("Unknown()"));
        strcpy(result, "Unknown(",9);
        strcat(result, get_vendor_string(),strlen(get_vendor_string()));
        strcat(result, ")",1);
        return result;
    }
}

/* Intel Specific brand list */
const char *Intel[] = {
	"Brand ID Not Supported.", 
	"Intel(R) Celeron(R) processor", 
	"Intel(R) Pentium(R) III processor", 
	"Intel(R) Pentium(R) III Xeon(R) processor", 
	"Intel(R) Pentium(R) III processor", 
	"Reserved", 
	"Mobile Intel(R) Pentium(R) III processor-M", 
	"Mobile Intel(R) Celeron(R) processor", 
	"Intel(R) Pentium(R) 4 processor", 
	"Intel(R) Pentium(R) 4 processor", 
	"Intel(R) Celeron(R) processor", 
	"Intel(R) Xeon(R) Processor", 
	"Intel(R) Xeon(R) processor MP", 
	"Reserved", 
	"Mobile Intel(R) Pentium(R) 4 processor-M", 
	"Mobile Intel(R) Pentium(R) Celeron(R) processor", 
	"Reserved", 
	"Mobile Genuine Intel(R) processor", 
	"Intel(R) Celeron(R) M processor", 
	"Mobile Intel(R) Celeron(R) processor", 
	"Intel(R) Celeron(R) processor", 
	"Mobile Geniune Intel(R) processor", 
	"Intel(R) Pentium(R) M processor", 
	"Mobile Intel(R) Celeron(R) processor"
};

/* This table is for those brand strings that have two values depending on the processor signature. It should have the same number of entries as the above table. */
const char *Intel_Other[] = {
	"Reserved", 
	"Reserved", 
	"Reserved", 
	"Intel(R) Celeron(R) processor", 
	"Reserved", 
	"Reserved", 
	"Reserved", 
	"Reserved", 
	"Reserved", 
	"Reserved", 
	"Reserved", 
	"Intel(R) Xeon(R) processor MP", 
	"Reserved", 
	"Reserved", 
	"Intel(R) Xeon(R) processor", 
	"Reserved", 
	"Reserved", 
	"Reserved", 
	"Reserved", 
	"Reserved", 
	"Reserved", 
	"Reserved", 
	"Reserved", 
	"Reserved"
};


/* Print Registers */
void printregs(int eax, int ebx, int ecx, int edx) {
	int j;
	char string[17];
	string[16] = '\0';
	for(j = 0; j < 4; j++) {
		string[j] = eax >> (8 * j);
		string[j + 4] = ebx >> (8 * j);
		string[j + 8] = ecx >> (8 * j);
		string[j + 12] = edx >> (8 * j);
	}
	print_string(string);
}

int do_intel() {
    print_string("Intel Specific Features:\n\r");
	unsigned long eax, ebx, ecx, edx, max_eax, signature, unused;
	int model, family, type, brand, stepping, reserved;
	int extended_family = -1;
	__cpuid(1, eax, ebx, unused, unused);
	model = (eax >> 4) & 0xf;
	family = (eax >> 8) & 0xf;
	type = (eax >> 12) & 0x3;
	brand = ebx & 0xff;
	stepping = eax & 0xf;
	reserved = eax >> 14;
	signature = eax;
	print_string("Type ");
    print_string(HexToString(type));
    print_string(" - ");
	switch(type) {
		case 0:
		print_string("Original OEM");
		break;
		case 1:
		print_string("Overdrive");
		break;
		case 2:
		print_string("Dual-capable");
		break;
		case 3:
		print_string("Reserved");
		break;
	}
	print_string("\n\r");
	print_string("Family ");
    print_string(HexToString(family));
    print_string(" - ");
	switch(family) {
		case 3:
		print_string("i386");
		break;
		case 4:
		print_string("i486");
		break;
		case 5:
		print_string("Pentium");
		break;
		case 6:
		print_string("Pentium Pro");
		break;
		case 15:
		print_string("Pentium 4");
	}
	print_string("\n\r");
	if(family == 15) {
		extended_family = (eax >> 20) & 0xff;
		print_string("Extended family ");print_string(HexToString(extended_family));print_string("\n\r");
	}
	print_string("Model ");print_string(HexToString(model));print_string(" - ");;
	switch(family) {
		case 3:
		break;
		case 4:
		switch(model) {
			case 0:
			case 1:
			print_string("DX");
			break;
			case 2:
			print_string("SX");
			break;
			case 3:
			print_string("487/DX2");
			break;
			case 4:
			print_string("SL");
			break;
			case 5:
			print_string("SX2");
			break;
			case 7:
			print_string("Write-back enhanced DX2");
			break;
			case 8:
			print_string("DX4");
			break;
		}
		break;
		case 5:
		switch(model) {
			case 1:
			print_string("60/66");
			break;
			case 2:
			print_string("75-200");
			break;
			case 3:
			print_string("for 486 system");
			break;
			case 4:
			print_string("MMX");
			break;
		}
		break;
		case 6:
		switch(model) {
			case 1:
			print_string("Pentium Pro");
			break;
			case 3:
			print_string("Pentium II Model 3");
			break;
			case 5:
			print_string("Pentium II Model 5/Xeon/Celeron");
			break;
			case 6:
			print_string("Celeron");
			break;
			case 7:
			print_string("Pentium III/Pentium III Xeon - external L2 cache");
			break;
			case 8:
			print_string("Pentium III/Pentium III Xeon - internal L2 cache");
			break;
		}
		break;
		case 15:
		break;
	}
	print_string("\n\r");
	__cpuid(0x80000000, max_eax, unused, unused, unused);
	/* Quok said: If the max extended eax value is high enough to support the processor brand string
	(values 0x80000002 to 0x80000004), then we'll use that information to return the brand information. 
	Otherwise, we'll refer back to the brand tables above for backwards compatibility with older processors. 
	According to the Sept. 2006 Intel Arch Software Developer's Guide, if extended eax values are supported, 
	then all 3 values for the processor brand string are supported, but we'll test just to make sure and be safe. */
	if(max_eax >= 0x80000004) {
		print_string("Brand: ");
		if(max_eax >= 0x80000002) {
			__cpuid(0x80000002, eax, ebx, ecx, edx);
			printregs(eax, ebx, ecx, edx);
		}
		if(max_eax >= 0x80000003) {
			__cpuid(0x80000003, eax, ebx, ecx, edx);
			printregs(eax, ebx, ecx, edx);
		}
		if(max_eax >= 0x80000004) {
			__cpuid(0x80000004, eax, ebx, ecx, edx);
			printregs(eax, ebx, ecx, edx);
		}
		print_string("\n\r");
	} else if(brand > 0) {
		print_string("Brand ");HexToString(brand);print_string(" - ");
		if(brand < 0x18) {
			if(signature == 0x000006B1 || signature == 0x00000F13) {
				print_string(Intel_Other[brand]);
                print_string("\n\r");
			} else {
				print_string(Intel[brand]);
                print_string("\n\r");
			}
		} else {
			print_string("Reserved\n\r");
		}
	}
	print_string("Stepping: ");
    print_string(HexToString(stepping));
    print_string(" Reserved: ");
    print_string(HexToString(reserved));
    print_string("\n\r");
	return 0;
}

/* AMD-specific information */
int do_amd(void) {
	print_string("AMD Specific Features:\n\r");
	unsigned long extended, eax, ebx, ecx, edx, unused;
	int family, model, stepping, reserved;
	__cpuid(1, eax, unused, unused, unused);
	model = (eax >> 4) & 0xf;
	family = (eax >> 8) & 0xf;
	stepping = eax & 0xf;
	reserved = eax >> 12;
	print_string("Family: ");
    print_string(HexToString(family));
    print_string(" Model: ");
    print_string(HexToString(model));
    print_string(" [");
	switch(family) {
		case 4:
		print_string("486 Model ");
        print_string(HexToString(model));
		break;
		case 5:
		switch(model) {
			case 0:
			case 1:
			case 2:
			case 3:
			case 6:
			case 7:
			print_string("K6 Model ");
            print_string(HexToString(model));
			break;
			case 8:
			print_string("K6-2 Model 8");
			break;
			case 9:
			print_string("K6-III Model 9");
			break;
			default:
			print_string("K5/K6 Model ");
            print_string(HexToString(model));
			break;
		}
		break;
		case 6:
		switch(model) {
			case 1:
			case 2:
			case 4:
			print_string("Athlon Model ");
            print_string(HexToString(model));
			break;
			case 3:
			print_string("Duron Model 3");
			break;
			case 6:
			print_string("Athlon MP/Mobile Athlon Model 6");
			break;
			case 7:
			print_string("Mobile Duron Model 7");
			break;
			default:
			print_string("Duron/Athlon Model ");
            print_string(HexToString(model));
			break;
		}
		break;
	}
	print_string("]\n\r");
	__cpuid(0x80000000, extended, unused, unused, unused);
	if(extended == 0) {
		return 0;
	}
	if(extended >= 0x80000002) {
		unsigned int j;
		print_string("Detected Processor Name: ");
		for(j = 0x80000002; j <= 0x80000004; j++) {
			__cpuid(j, eax, ebx, ecx, edx);
			printregs(eax, ebx, ecx, edx);
		}
		print_string("\n\r");
	}
	if(extended >= 0x80000007) {
		__cpuid(0x80000007, unused, unused, unused, edx);
		if(edx & 1) {
			print_string("Temperature Sensing Diode Detected!\n\r");
		}
	}
	print_string("Stepping: ");
    print_string(HexToString(stepping));
    print_string(" Reserved: ");
    print_string(HexToString(reserved));
    print_string("\n\r");
	return 0;
}

static void cpu_detect(void)
{
    unsigned int unused = 0, ebx = 0, ecx = 0, edx = 0;
    __cpuid(0, unused, ebx, ecx, edx);
    switch (ebx) {
        case 0x756e6547:
            do_intel();
            break;
        case 0x68747541:
            do_amd();
            break;
        default:
            print_string("Unknown x86 vendor\n\r");
            break;
    }
}