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

#ifdef __cplusplus
extern "C" {
#endif
extern so_module so_mod;
#ifdef __cplusplus
};
#endif

#include "utils/logger.h"
#include <stdbool.h>

so_hook achieve_hook;

int setAchieve(void *this, int id, int unlock) {
	printf("setAchieve(%i, %i)\n", id, unlock);
	trophies_unlock(id + 1);
	
	return SO_CONTINUE(int, achieve_hook, this, id, unlock);
}

void so_patch(void) {
    achieve_hook = hook_addr((uintptr_t)so_symbol(&so_mod, "_ZN7Achieve10setAchieveEii"), (uintptr_t)&setAchieve);	
}
