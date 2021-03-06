#include "stdafx.h"

#include "FromGame.h"
#include "MemWriter.h"

#define _thisfastcall(CLASS) CLASS* _this, int
#define GetOffset(PTR, OFFSET) ( (char*)(PTR) + OFFSET )

typedef void(__thiscall *T_World_DoTimeStep)(World* _this, float timeElapsed);
T_World_DoTimeStep World_DoTimeStep_backup;

const SHORT ButtonPressedNow = 0x8000;
const DWORD RedefNo = 0x1;

typedef void(__thiscall *T_NotificationMessage)(MenuScreen* _this, DWORD hash, void* FEObj, DWORD p3, DWORD p4);
T_NotificationMessage GameWorldMapScreen_NotificationMessage_Backup;

void __fastcall GameWorldMapScreen_NotificationMessage_Hook(_thisfastcall(GameWorldMapScreen), 
	DWORD hash, void* FEObj, DWORD p3, DWORD p4) {
	// 0x911AB364, 0xD9CA113D - esc
	// 0xB4623F67 - No
	UnlockedTriggerZone* zone = &_this->mapElement->ptr0x14->zone;
	if (hash == RedefNo) {
		RigidBody* plBody = (*pCurrentWorld)->cars[0]->mover->body;
		R_memcpy(&(*pCurrentWorld)->cars[0]->mover->body->position, GetOffset(_this->mapElement->ptr0x14->zone.obj_, 4), 8);
		if (zone->ShowcaseData != nullptr) {
			// photoshoot
			plBody->position.x += 8;
			plBody->position.y += 8;
		}
		R_memset(&plBody->velocity, 0, 12);
		hash = 0x911AB364;
	}
	GameWorldMapScreen_NotificationMessage_Backup(_this, hash, FEObj, p3, p4);
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
	if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
		// from ExOpts
		const uintptr_t base = (uintptr_t)0x400000; // there's no ASLR
		IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)(base);
		IMAGE_NT_HEADERS* nt = (IMAGE_NT_HEADERS*)(base + dos->e_lfanew);
		if ((base + nt->OptionalHeader.AddressOfEntryPoint) == 0x75BCC7) { // Check if .exe file is compatible - Thanks to thelink2012 and MWisBest
			void *tmp = GameWorldMapScreen_NotificationMessage_Hook;
			WriteProtectedMem((void*)0x797F28, &tmp, 4, &GameWorldMapScreen_NotificationMessage_Backup);
			WriteProtectedMem((void*)0x4B113E, &RedefNo, 4);
			
		}
		else {
			MessageBoxA(NULL, "Unsupported .exe", "NFSU2 Fast Travel", MB_ICONERROR);
			return FALSE;
		}
	}
    return TRUE;
}
