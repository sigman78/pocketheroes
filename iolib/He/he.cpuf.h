/*
 * This file is a part of Pocket Heroes Game project
 * 	http://www.pocketheroes.net
 *	https://code.google.com/p/pocketheroes/
 *
 * Copyright 2004-2010 by Robert Tarasov and Anton Stuk (iO UPG)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */ 

/** @file
 *	@brief Extracting all features the application wants to know about from CPUID information
 */
/** @addtogroup iolib_he
 *  @{
 */
#ifndef _IOLIB_HELIUM_CPU_FEATURES_H_
#define _IOLIB_HELIUM_CPU_FEATURES_H_

/// This class contain all required information about CPU features
class iCPUFeatures
{
public:
	/// Constructor
    iCPUFeatures (){}
    
	/// Resets all CPU fetaures flags
	void Reset() {
        x86_MMX = false;
        x86_CMOV = false;
        x86_3DNow = false;
        x86_3DNowExt = false;
        x86_3DNowPro = false;
        x86_SSE = false;
        x86_SSE2 = false;
    }

    bool x86_MMX;
    bool x86_CMOV;
    bool x86_3DNow;
    bool x86_3DNowExt;
    bool x86_3DNowPro;    // no detection code for this yet! 6/29/2001
    bool x86_SSE;
    bool x86_SSE2;
};

// However this is easier to use (AutoInitialization)
extern iCPUFeatures gCPUFeatures;


/// Initializes CPU feature structure
extern void InitCPUFeatures (void);


//
//
//
//
#define STORE_CLASSICAL_NAME(x)		_stprintf (ChipID.ProcessorName, x)
#define STORE_TLBCACHE_INFO(x,y)	x = (x < y) ? y : x
#define VENDOR_STRING_LENGTH		(12 + 1)
#define CHIPNAME_STRING_LENGTH		(48 + 1)
#define SERIALNUMBER_STRING_LENGTH	(29 + 1)
#define TLBCACHE_INFO_UNITS			(15)
#define CLASSICAL_CPU_FREQ_LOOP		10000000
#define RDTSC_INSTRUCTION			_asm _emit 0x0f _asm _emit 0x31
#define	CPUSPEED_I32TO64(x, y)		(((__int64) x << 32) + y)

#define CPUID_AWARE_COMPILER
#ifdef CPUID_AWARE_COMPILER
	#define CPUID_INSTRUCTION		cpuid
#else
	#define CPUID_INSTRUCTION		_asm _emit 0x0f _asm _emit 0xa2
#endif

#define MMX_FEATURE					0x00000001
#define MMX_PLUS_FEATURE			0x00000002
#define SSE_FEATURE					0x00000004
#define SSE2_FEATURE				0x00000008
#define AMD_3DNOW_FEATURE			0x00000010
#define AMD_3DNOW_PLUS_FEATURE		0x00000020
#define IA64_FEATURE				0x00000040
#define MP_CAPABLE					0x00000080
#define HYPERTHREAD_FEATURE			0x00000100
#define SERIALNUMBER_FEATURE		0x00000200
#define APIC_FEATURE				0x00000400
#define SSE_FP_FEATURE				0x00000800
#define SSE_MMX_FEATURE				0x00001000
#define CMOV_FEATURE				0x00002000
#define MTRR_FEATURE				0x00004000
#define L1CACHE_FEATURE				0x00008000
#define L2CACHE_FEATURE				0x00010000
#define L3CACHE_FEATURE				0x00020000
#define ACPI_FEATURE				0x00040000
#define THERMALMONITOR_FEATURE		0x00080000
#define TEMPSENSEDIODE_FEATURE		0x00100000
#define FREQUENCYID_FEATURE			0x00200000
#define VOLTAGEID_FREQUENCY			0x00400000

typedef	void (*DELAY_FUNC)(unsigned int uiMS);

/// CPU frequency calculation class
class CPUSpeed {
public:
	/// Default constructor
	CPUSpeed ();

	/// Destructor
	~CPUSpeed ();
		
	int CPUSpeedInMHz;

	/// Calculates CPU cycles time difference
	__int64 __cdecl GetCyclesDifference (DELAY_FUNC, unsigned int);
		
private:
	// Functions.
	static void Delay (unsigned int);
	static void DelayOverhead (unsigned int);

protected:
	
};

/// CPU Information class
class CPUInfo 
{
public:
	/// Default constructor
	CPUInfo ();

	/// Destructor
	~CPUInfo ();

	/// Returns CPU vendor name
	iCharT* GetVendorString ();

	/// Returns CPU vendor id
	iCharT* GetVendorID ();

	/// Returns CPU type id
	iCharT* GetTypeID ();

	/// Returns CPU family id
	iCharT* GetFamilyID ();

	/// Returns CPU model id
	iCharT* GetModelID ();

	/// Returns CPU stepping code
	iCharT* GetSteppingCode ();

	/// Returns extended CPU name
	iCharT* GetExtendedProcessorName ();

	/// Returns CPU serial number
	iCharT* GetProcessorSerialNumber ();

	/// Returns number of logical processors per physical
	int GetLogicalProcessorsPerPhysical ();

	/// Returns CPU clock frequency
	int GetProcessorClockFrequency ();

	/// Returns CPU APICID
	int GetProcessorAPICID ();

	/// Returns CPU cache size
	int GetProcessorCacheXSize (DWORD);

	/// Returns compatibility with specified CPU feature
	bool DoesCPUSupportFeature (DWORD);

	/// Returns ability to retreive CPU id information
	bool __cdecl DoesCPUSupportCPUID ();

private:
	/// This structure contain common information about CPU
	typedef struct tagID {
		int Type;
		int Family;
		int Model;
		int Revision;
		int ExtendedFamily;
		int ExtendedModel;
		iCharT ProcessorName[CHIPNAME_STRING_LENGTH];
		iCharT Vendor[VENDOR_STRING_LENGTH];
		iCharT SerialNumber[SERIALNUMBER_STRING_LENGTH];
	} ID;

	/// This structure contain power management information about CPU
	typedef struct tagCPUPowerManagement {
		bool HasVoltageID;
		bool HasFrequencyID;
		bool HasTempSenseDiode;
	} CPUPowerManagement;

	/// This structure contain CPU extended features list
	typedef struct tagCPUExtendedFeatures {
		bool Has3DNow;
		bool Has3DNowPlus;
		bool SupportsMP;
		bool HasMMXPlus;
		bool HasSSEMMX;
		bool SupportsHyperthreading;
		int LogicalProcessorsPerPhysical;
		int APIC_ID;
		CPUPowerManagement PowerManagement;
	} CPUExtendedFeatures;	
	
	/// This structure contain CPU features list
	typedef struct CPUtagFeatures {
		bool HasFPU;
		bool HasTSC;
		bool HasMMX;
		bool HasSSE;
		bool HasSSEFP;
		bool HasSSE2;
		bool HasIA64;
		bool HasAPIC;
		bool HasCMOV;
		bool HasMTRR;
		bool HasACPI;
		bool HasSerial;
		bool HasThermal;
		int CPUSpeed;
		int L1CacheSize;
		int L2CacheSize;
		int L3CacheSize;
		CPUExtendedFeatures ExtendedFeatures;
	} CPUFeatures;
    
	/// CPU Manufacturer enumeration
	enum Manufacturer {
		AMD, Intel, NSC, UMC, Cyrix, NexGen, IDT, Rise, Transmeta, UnknownManufacturer
	};

	// Functions.
	bool __cdecl RetrieveCPUFeatures ();
	bool __cdecl RetrieveCPUIdentity ();
	bool __cdecl RetrieveCPUCacheDetails ();
	bool __cdecl RetrieveClassicalCPUCacheDetails ();
	bool __cdecl RetrieveCPUClockSpeed ();
	bool __cdecl RetrieveClassicalCPUClockSpeed ();
	bool __cdecl RetrieveCPUExtendedLevelSupport (int);
	bool __cdecl RetrieveExtendedCPUFeatures ();
	bool __cdecl RetrieveProcessorSerialNumber ();
	bool __cdecl RetrieveCPUPowerManagement ();
	bool __cdecl RetrieveClassicalCPUIdentity ();
	bool __cdecl RetrieveExtendedCPUIdentity ();
	
	// Variables.
	Manufacturer ChipManufacturer;
	CPUFeatures Features;
	CPUSpeed * Speed;
	ID ChipID;
	
protected:

};


#endif //_IOLIB_HELIUM_CPU_FEATURES_H_
/** @} */ // iolib_he
