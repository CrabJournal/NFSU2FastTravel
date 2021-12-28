#pragma once

#include "R_math.h"

void WriteProtectedMem(void* dst, const void *src, size_t size) {
	DWORD oldp;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldp);
	R_memcpy(dst, src, size);
	VirtualProtect(dst, size, oldp, &oldp); // return old protection to reduce segment fragmentation
}
void WriteProtectedMem(void* dst, const void *src, size_t size, void* backup) {
	R_memcpy(backup, dst, size);
	WriteProtectedMem(dst, src, size);
}
inline int GetRelativeAddress(int* addr1, void* addr2) {
	return (char*)addr2 - (char*)(addr1 + 1);
}
int WriteProtectedMemEIPRelativeAdderess(int* place, void* new_addr) {
	int old = *place;
	int new_addr_rel = GetRelativeAddress(place, new_addr);
	WriteProtectedMem(place, &new_addr_rel, 4);
	return old;
}
inline void* WriteProtectedMemEIPRelativeAdderess_ReAddr(int* place, void* new_addr) {
	return (void*)(WriteProtectedMemEIPRelativeAdderess(place, new_addr) + (int)place + 4);
}