#include "stdafx.h"

#include "FromGame.h"
#include "MemWriter.h"

#define _thisfastcall(CLASS) CLASS* _this, int

typedef void(__thiscall *T_World_DoTimeStep)(float timeElapsed);
T_World_DoTimeStep World_DoTimeStep_backup;

void __fastcall World_DoTimeStep_Hook(_thisfastcall(World), float timeElapsed) {
	World_DoTimeStep_backup(timeElapsed);
	if (GetAsyncKeyState('1') & 1) {
		R_memcpy(&_this->cars[0]->mover->body->position, GPS_coords_vec2, 8);
		R_memset(&_this->cars[0]->mover->body->velocity, 0, 12);
	}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
	if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
		World_DoTimeStep_backup = 
			(T_World_DoTimeStep)WriteProtectedMemEIPRelativeAdderess_ReAddr((int*)call_World_DoTimeStep, World_DoTimeStep_Hook);
	}
    return TRUE;
}