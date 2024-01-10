/*
 * patch.c
 *
 * Patching some of the .so internal functions or bridging them to native for
 * better compatibility.
 *
 * Copyright (C) 2023 Volodymyr Atamanenko
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 */

#include "patch.h"

#include <kubridge.h>
#include <so_util/so_util.h>
#include <utils/trophies.h>
#include <stdio.h>
#include <vitasdk.h>

#ifdef __cplusplus
extern "C" {
#endif
extern so_module so_mod;
#ifdef __cplusplus
};
#endif

#include "utils/logger.h"
#include <stdbool.h>

so_hook achieve_hook, stage_hook, takamatsu_hook, takamatsu2_hook;

int setAchieve(void *this, int id, int unlock) {
	printf("setAchieve(%i, %i)\n", id, unlock);
	trophies_unlock(id + 1);
	
	return SO_CONTINUE(int, achieve_hook, this, id, unlock);
}

int ret0() { return 0; }
int ret1() { return 1; }

uint32_t *	I_HeapKaraLoop;

int S_CheckUsefulStage(int stage) {
	// Report as unlocked any not unlockable stage
	switch (stage) {
	case 8:
	case 14: // Chaos
	case 16: // Takematsu Castle (Winter)
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
	case 22:
	case 23:
		return 1;
	default:
		return SO_CONTINUE(int, stage_hook, stage);
	}
}

void exit_process() {
	sceKernelExitProcess(0);
}

int TakamatsuSummer() {
	*I_HeapKaraLoop = 1;
	return SO_CONTINUE(int, takamatsu_hook);
}

int TakamatsuWinter() {
	*I_HeapKaraLoop = 1;
	return SO_CONTINUE(int, takamatsu2_hook);
}

void so_patch(void) {
	// Trophies support
	achieve_hook = hook_addr((uintptr_t)so_symbol(&so_mod, "_ZN7Achieve10setAchieveEii"), (uintptr_t)&setAchieve);
	
	// Disable anything stage related for Takamatsu Castle to not tank framerate
	I_HeapKaraLoop = so_symbol(&so_mod, "I_HeapKaraLoop");
	takamatsu_hook = hook_addr((uintptr_t)so_symbol(&so_mod, "_Z17I_TakamatsuSummerv"), (uintptr_t)&TakamatsuSummer);
	takamatsu2_hook = hook_addr((uintptr_t)so_symbol(&so_mod, "_Z17I_TakamatsuWinterv"), (uintptr_t)&TakamatsuWinter);
	
	// Kill "PertBoss" spawning in Money Pit. No idea what this is but seems to help with framerate tanking
	uint16_t instr16 = 0xd0c9; // beq #0xffffff96
	kuKernelCpuUnrestrictedMemcpy((void *)(so_mod.text_base + 0x10eb7c), &instr16, 2);
	
	// Killing S/N-Fire elements in Money Pit. Seems to help framerate with little changes to the actual stage
	uint32_t instr32 = 0xaf41f43f;
	kuKernelCpuUnrestrictedMemcpy((void *)(so_mod.text_base + 0x10e55e), &instr32, 4);
	instr32 = 0xaf35f43f;
	kuKernelCpuUnrestrictedMemcpy((void *)(so_mod.text_base + 0x10e702), &instr32, 4);
	instr32 = 0xaf3df43f;
	kuKernelCpuUnrestrictedMemcpy((void *)(so_mod.text_base + 0x10e90e), &instr32, 4);
	instr32 = 0xaf3ff43f;
	kuKernelCpuUnrestrictedMemcpy((void *)(so_mod.text_base + 0x10eaf6), &instr32, 4);
	instr32 = 0xaf4ef47f;
	kuKernelCpuUnrestrictedMemcpy((void *)(so_mod.text_base + 0x10ed64), &instr32, 4);
	instr32 = 0xaf54f47f;
	kuKernelCpuUnrestrictedMemcpy((void *)(so_mod.text_base + 0x10ef6c), &instr32, 4);
	
	// Paralyze mice in Money Pit to save on framerate taxing
	hook_addr((uintptr_t)so_symbol(&so_mod, "_Z11I_ObjMouse0v"), (uintptr_t)&ret0);
	
	// Kill ring edge particles spawning. Seems to not affect graphics in any way but helps in Money Pit.
	hook_addr((uintptr_t)so_symbol(&so_mod, "_Z24I_CreateRingEdgeParticleP7FVECTORS0_S0_P7FMATRIX"), (uintptr_t)&ret0);
	
	// Unlock stages that aren't unlockable on Android port
	stage_hook = hook_addr((uintptr_t)so_symbol(&so_mod, "_Z18S_CheckUsefulStagei"), (uintptr_t)&S_CheckUsefulStage);
	
	// Prevent game from crashing when attempting to exit it
	hook_addr((uintptr_t)so_symbol(&so_mod, "_ZN11SoundOpenSL8shutdownEv"), (uintptr_t)&exit_process);
}
