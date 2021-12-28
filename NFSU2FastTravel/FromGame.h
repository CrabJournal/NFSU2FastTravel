#pragma once

#include "UG1_UG2_common.h"


// size ?, 
struct World {
	void *unk_yet[4];
	int CarsNumber;						// +0x10
	int PlayersCarsNumber;				// +0x14
	int RacingCarsNumber;				// +0x18
	Car* cars[20/* not sure */];		// +0x1C
};

// .data
World** const pCurrentWorld = (World**)0x890080;
float* const GPS_coords_vec2 = (float*)0x8385B0;

// .text
void* const call_World_DoTimeStep = (void*)0x581408;