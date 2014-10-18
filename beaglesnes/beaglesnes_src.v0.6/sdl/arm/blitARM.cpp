#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <SDL.h>
#include "snes9x.h"
#include "neon_scale2x.h"
#include "gui.h"

void S9xBlitPixARM2x2 (uint8 *srcPtr, int srcRowBytes, uint8 *dstPtr, int dstRowBytes, int width, int height)
{
	fprintf(stderr, "Before neon_scale2x_16_16()\n");//%x, %x, %d, %d, %d, %d)\n", (const uint16_t *)srcPtr, (uint16_t *)dstPtr, width, srcRowBytes, dstRowBytes, height);
SDL_LockSurface(screen);
	neon_scale2x_16_16((const uint16_t *)srcPtr, (uint16_t *)dstPtr, width, width * 2, screen->pitch, height);
SDL_UnlockSurface(screen);
	fprintf(stderr, "After neon_scale2x_16_16()\n");
}

