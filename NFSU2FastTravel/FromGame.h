#pragma once

#include "UG1_UG2_common.h"

struct UnlockedTriggerZone;

struct FEngine;

// size 0x18 (was 0x10 in UG1)
struct cFEng {
	void* Vtable;				// 
	BYTE unk0x4;				// +0x4
	BYTE unk0x5;				// +0x5
	BYTE unk0x6;				// +0x6
	BYTE unk0x7;				// +0x7
	FEngine* feng;				// +0x8
	void* unk_obj0xC;			// +0xC / size 0x3420 (same as UG1)

};

// size 0x5254
struct FEngine {
	// +0x10C
};
// 0C407210 - Accept code

// size unk
struct MenuScreen {
	virtual ~MenuScreen();
	virtual void MaybeOverrideActiveButtons();
	virtual void NotificationMessage(DWORD hash, void* FEobj, DWORD p3, DWORD p4);

	const char* fng;				// +0x4
	DWORD unk0x8[3];				// +0x8
	const char* fng2;				// +0x14
};

// size mb 0x18, as part of 0x24 from  0x4E056C
struct UnlockedTriggerZone {
	void* obj_;				// +0x0 obj allocated in block size 0x20C30
	/*
	{
		// +0x4 vector3 mb pos
		// +0x34 - vector3int mb pos
	}
	*/
};


struct unkObj0 {
	DWORD unk0x0[3];
	UnlockedTriggerZone zone;
};

// size 0x2C
struct WorldMapElement {
	BYTE unk0x0[0x14];
	unkObj0* ptr0x14;
};

struct GameWorldMapScreen : MenuScreen  {
	BYTE unk0x10[0x34];
	WorldMapElement* mapElement;	// +0x4C
	DWORD unk0x50[3];	
	void* mbSideBarCategory;		// +0x5C
};

// 
struct GPSManager {
	DWORD unk0x0;
	DWORD unk0x4[3];
	vector3a destination_coords;			// +0x10
	float vec2_dst_coords[2];				// +0x20
	BYTE unk0x28;
	bool isGPS_activated;					// +0x29
};

// size 0x1600, from 0x60A71E
struct World {
	void *unk_yet[4];
	int CarsNumber;						// +0x10
	int PlayersCarsNumber;				// +0x14
	int RacingCarsNumber;				// +0x18
	Car* cars[20/* not sure */];		// +0x1C, size 0x9E0
};

// .data
World** const pCurrentWorld = (World**)0x890080;
GPSManager* const TheGPSManager = (GPSManager*)0x838590;
void* const TheGameFlowManager = (void*)0x8654A4;
bool* const _IsLostFocus = (bool*)0x8709E0;

// .text
void* const call_World_DoTimeStep = (void*)0x581408;