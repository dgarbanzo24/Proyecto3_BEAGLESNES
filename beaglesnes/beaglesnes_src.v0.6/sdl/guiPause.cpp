/*****************************************************************************
  BeagleSNES - Super Nintendo Entertainment System (TM) emulator for the
  BeagleBoard-xM platform.

  See CREDITS file to find the copyright owners of this file.

  GUI front-end code (c) Copyright 2013 Andrew Henderson (hendersa@icculus.org)

  BeagleSNES homepage: http://www.beaglesnes.org/
  
  BeagleSNES is derived from the Snes9x open source emulator:  
  Snes9x homepage: http://www.snes9x.com/

  Permission to use, copy, modify and/or distribute BeagleSNES in both binary
  and source form, for non-commercial purposes, is hereby granted without
  fee, providing that this license information and copyright notice appear
  with all copies and any derived work.

  This software is provided 'as-is', without any express or implied
  warranty. In no event shall the authors be held liable for any damages
  arising from the use of this software or it's derivatives.

  BeagleSNES is freeware for PERSONAL USE only. Commercial users should
  seek permission of the copyright holders first. Commercial use includes,
  but is not limited to, charging money for BeagleSNES or software derived 
  from BeagleSNES, including BeagleSNES or derivatives in commercial game 
  bundles, and/or using BeagleSNES as a promotion for your commercial 
  product.

  The copyright holders request that bug fixes and improvements to the code
  should be forwarded to them so everyone can benefit from the modifications
  in future versions.

  Super NES and Super Nintendo Entertainment System are trademarks of
  Nintendo Co., Limited and its subsidiary companies.
 *****************************************************************************/

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <fcntl.h>
#include "gui.h"
#include "beagleboard.h"
#include "savepng.h"

/* SNES9X stuff */
#include "snes9x.h"
#include "display.h"
#include "snapshot.h"
#include "gfx.h"

#define PAUSE_GUI_WIDTH 300
#define PAUSE_GUI_HEIGHT 220

enum {
	UL_CORNER = 0,
	UR_CORNER,
	LL_CORNER,
	LR_CORNER,
	TOP_BORDER,
	BOTTOM_BORDER,
	LEFT_BORDER,
	RIGHT_BORDER,
	TOTAL_PIECES
};

static SDL_Surface *pauseGui = NULL;
static SDL_Rect guiPiece[TOTAL_PIECES] = {
	{0,0,8,8}, {24,0,8,8}, {64,0,8,8}, {88,0,8,8},
	{8,0,8,4}, {72,4,8,4}, {32,0,4,8}, {44,0,4,8}
};

static uint32_t bgColor;
static TTF_Font *pauseHeaderFont = NULL;
static TTF_Font *pauseMenuFont = NULL;
static TTF_Font *pauseSmallFont = NULL;
static const char *headerText = "BeagleSNES PAUSED";
static const char *menuText[5] = { "Resume Game", "Load Snapshot", 
	"Save Snapshot", "Return To Game Selection", "LAST SNAPSHOT" }; 
static SDL_Surface *menuImage[7] = { 
	NULL, NULL, NULL, NULL, NULL, NULL, NULL }; 
static SDL_Rect menuTextPos[7] = { {0,0,0,0}, {0,0,0,0}, {0,0,0,0},
	{0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0} };
static SDL_Color textColor={255,255,255};
static SDL_Color inactiveTextColor={96,96,96};
static uint32_t snapshotAvailable = 0;
/* These two are loaded/saved from/to disk */
static SDL_Surface *noSnapshotImage = NULL;
static SDL_Surface *snapshotImage = NULL;
/* This is the tiny 128x96 one */
static SDL_Surface *tinySnapshotImage = NULL;

static SDL_Surface *menuMarkerImage = NULL;
static SDL_Surface *pauseMenuSurface = NULL;
static uint32_t menuPressDirection = 0;
static uint32_t forceUnpause = 0, done = 0;
static uint32_t frameCounter = 0, currentIndex = 0, nextIndex = 0;
static SDL_Rect markerPos[4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
static SDL_Surface *currentScreen = NULL;

#if defined(BEAGLEBONE_BLACK)
#if defined(CAPE_LCD3)
static const uint32_t xOffset = 10; // (320-300)/2;
static const uint32_t yOffset = 10; // (240-220)/2;
#else
static const uint32_t xOffset = 210; // (720-300)/2;
static const uint32_t yOffset = 130; // (480-220)/2;
#endif /* CAPE_LCD3 */
#else
static const uint32_t xOffset = 170; // (640-300)/2;
static const uint32_t yOffset = 130; // (480-220)/2;
#endif /* BEAGLEBONE_BLACK */

static void renderPauseGui(const char *romname);
static void handleJoystickEvent(SDL_Event *event);
static void shiftSelectedItemUp(void);
static void shiftSelectedItemDown(void);
static void incrementPauseItemFrame(void);
static void loadSnapshot(const char *romname);
static void saveSnapshot(const char *romname);
static void checkForSnapshot(const char *romname);

void loadPauseGui(void) {
	SDL_Surface *tempSurface;
	SDL_PixelFormat *format;

	format = screen->format;

	/* Load the graphics for the pause menu */
	tempSurface = IMG_Load("gfx/pausemenu_320x240.png");
	if (!tempSurface)
		fprintf(stderr, "Unable to load Pause GUI images\n");
	pauseGui = SDL_ConvertSurface(tempSurface, format, 0);
	SDL_FreeSurface(tempSurface);

	bgColor = SDL_MapRGB(screen->format, 0x28, 0x28, 0x80);
	noSnapshotImage = IMG_Load("gfx/blank_snapshot.png");
	if (!noSnapshotImage)
		fprintf(stderr, "Unable to load Pause GUI blank snapshot image\n");
	menuMarkerImage = IMG_Load("gfx/pause_marker.png");
	if (!menuMarkerImage)
		fprintf(stderr, "Unable to load Pause GUI menu marker image\n");

	/* Load menu fonts */
	pauseHeaderFont = TTF_OpenFont("fonts/FreeSansBold.ttf", 26);
	if (!pauseHeaderFont)
		fprintf(stderr, "Unable to load Pause GUI header font\n");

	pauseMenuFont = TTF_OpenFont("fonts/FreeSans.ttf", 20);
	if (!pauseMenuFont)
		fprintf(stderr, "Unable to load Pause GUI menu item font\n");

	pauseSmallFont = TTF_OpenFont("fonts/FreeSansBold.ttf", 12);
	if (!pauseHeaderFont)
		fprintf(stderr, "Unable to load Pause GUI menu small font\n");

	/* Render menu text */
	menuImage[0] = TTF_RenderText_Blended(pauseHeaderFont, headerText,
		textColor);
	menuImage[1] = TTF_RenderText_Blended(pauseMenuFont, menuText[0],
		textColor);
	menuImage[2] = TTF_RenderText_Blended(pauseMenuFont, menuText[1],
		textColor);
	menuImage[3] = TTF_RenderText_Blended(pauseMenuFont, menuText[2],
		textColor);
	menuImage[4] = TTF_RenderText_Blended(pauseMenuFont, menuText[3],
		textColor); 
	menuImage[5] = TTF_RenderText_Blended(pauseMenuFont, menuText[1],
		inactiveTextColor);
	menuImage[6] = TTF_RenderText_Blended(pauseSmallFont, menuText[4],
		textColor);
}

void renderPauseGui(const char *romname) {
	SDL_Rect bgRect = {0,0,(PAUSE_GUI_WIDTH - 8), (PAUSE_GUI_HEIGHT-8)};
	SDL_Rect pos = {0,0,0,0};
	uint32_t i;

	/* Check if there is a snapshot available */
	checkForSnapshot(romname);

	menuTextPos[0].x = xOffset + (PAUSE_GUI_WIDTH / 2) - (menuImage[0]->w / 2); 
	menuTextPos[0].y = yOffset + 8;
	
	for (i=1; i < 5; i++)
	{
		menuTextPos[i].x = xOffset + 19;
		menuTextPos[i].y = yOffset + 32 + (i * 36);
		markerPos[i-1].x = xOffset + 5;
		markerPos[i-1].y = yOffset + 32 + (i * 36);
	}
	menuTextPos[5].x = menuTextPos[2].x;
	menuTextPos[5].y = menuTextPos[2].y;
	menuTextPos[6].x = xOffset + 176;
	menuTextPos[6].y = yOffset + 52; 

	bgRect.x = xOffset + 4; bgRect.y = yOffset + 4;

	/* Corners */
	SDL_SetColorKey(pauseGui, SDL_SRCCOLORKEY, SDL_MapRGB(screen->format, 0xFF, 0x00, 0xFF));
	pos.x = xOffset; pos.y = yOffset;
	SDL_BlitSurface(pauseGui, &(guiPiece[UL_CORNER]), screen, &pos);	
	pos.y = yOffset + PAUSE_GUI_HEIGHT - 8;
	SDL_BlitSurface(pauseGui, &(guiPiece[LL_CORNER]), screen, &pos);
	pos.x = xOffset + PAUSE_GUI_WIDTH - 8; pos.y = yOffset;
	SDL_BlitSurface(pauseGui, &(guiPiece[UR_CORNER]), screen, &pos);
	pos.y = yOffset + PAUSE_GUI_HEIGHT - 8;
	SDL_BlitSurface(pauseGui, &(guiPiece[LR_CORNER]), screen, &pos);
	SDL_SetColorKey(pauseGui, SDL_FALSE, 0);

	/* Top */
	pos.y = yOffset;
	for (i=0; i < 36; i++)
	{
		pos.x = xOffset + 8 + (i*8);
		SDL_BlitSurface(pauseGui, &guiPiece[TOP_BORDER], screen, &pos);
	}

	/* Bottom */
	pos.y = yOffset + PAUSE_GUI_HEIGHT - 4;
	for (i=0; i < 36; i++)
	{     
		pos.x = xOffset + 8 + (i*8);
		SDL_BlitSurface(pauseGui, &guiPiece[BOTTOM_BORDER], screen, &pos);
	}

	/* Left */
	pos.x = xOffset;
	for (i=0; i < 26; i++)
	{     
		pos.y = yOffset + 8 + (i*8);
		SDL_BlitSurface(pauseGui, &guiPiece[LEFT_BORDER], screen, &pos);
	}

	/* Right */
	pos.x = xOffset + PAUSE_GUI_WIDTH - 4;
	for (i=0; i < 26; i++)
	{     
		pos.y = yOffset + 8 + (i*8);
		SDL_BlitSurface(pauseGui, &guiPiece[RIGHT_BORDER], screen, &pos);
	}
	
	/* Background */
	SDL_FillRect(screen, &bgRect, bgColor); 

	/* Menu text */
	SDL_BlitSurface(menuImage[0], NULL, screen, &(menuTextPos[0]));
	SDL_BlitSurface(menuImage[1], NULL, screen, &(menuTextPos[1]));
	for (i=3; i < 5; i++)
		SDL_BlitSurface(menuImage[i], NULL, screen, &(menuTextPos[i]));
	SDL_BlitSurface(menuImage[6], NULL, screen, &(menuTextPos[6]));

	/* Render the marker */
	//SDL_BlitSurface(menuMarkerImage, NULL, screen, &(markerPos[0]));

	/* Determine if the snapshot loading is enabled */
	pos.x = xOffset + 166;
	pos.y = yOffset + 70;

	if (snapshotAvailable)
	{
		SDL_BlitSurface(menuImage[2], NULL, screen, &(menuTextPos[2]));
		SDL_BlitSurface(tinySnapshotImage, NULL, screen, &pos);
	}
	else
	{
		SDL_BlitSurface(menuImage[5], NULL, screen, &(menuTextPos[2]));
		SDL_BlitSurface(noSnapshotImage, NULL, screen, &pos);
	}

	SDL_UpdateRect(screen, 0,0,0,0);

  /* Finally, now that we have rendered everything, cache a copy of it */
	pos.x = xOffset; pos.y = yOffset; 
	pos.w = PAUSE_GUI_WIDTH; pos.h = PAUSE_GUI_HEIGHT;
	pauseMenuSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, PAUSE_GUI_WIDTH,
		PAUSE_GUI_HEIGHT, 16, 0x7C00, 0x03E0, 0x001F, 0);
	SDL_BlitSurface(screen, &pos, pauseMenuSurface, NULL); 
}

uint32_t doPauseGui(const char *romname, int width, int height)
{
	SDL_Rect menuPos = {0,0,0,0};
	struct timeval startTime, endTime;
	uint64_t elapsedTime;
	SDL_Event event;
	uint32_t returnVal = 0;
	uint16_t *srcPixel, *dstPixel;
	uint32_t xPos, yPos;
 
	snapshotAvailable = 0;
	Settings.Paused = 1;
	
	/* Prior to doing any rendering, make a copy of the screen */
	currentScreen = SDL_CreateRGBSurface(SDL_SWSURFACE, screen->w, 
		screen->h, 16, 0x7C00, 0x03E0, 0x001F, 0);
	SDL_BlitSurface(screen, NULL, currentScreen, NULL);

	tinySnapshotImage = SDL_CreateRGBSurface(SDL_SWSURFACE, 128,
		96, 16, 0x7C00, 0x03E0, 0x001F, 0);	
	/* Render the pause menu on the screen and make a copy of it */
	renderPauseGui(romname);

	menuPos.x = xOffset; menuPos.y = yOffset;
	done = 0; forceUnpause = 0;
	nextIndex = currentIndex = frameCounter = 0;

	/* Do drawing and animation loop */
	while (!done) {
		gettimeofday(&startTime, NULL);

		/* Render the pause menu */
		SDL_BlitSurface(pauseMenuSurface, NULL, screen, &menuPos);
		/* Render the marker */
		incrementPauseItemFrame();
		SDL_BlitSurface(menuMarkerImage, NULL, screen, &(markerPos[nextIndex]));
		SDL_UpdateRect(screen,xOffset,yOffset,PAUSE_GUI_WIDTH,PAUSE_GUI_HEIGHT);
		gpioEvents();

		/* Check for events */
		while ( SDL_PollEvent(&event) ) {
			switch (event.type) {
				case SDL_MOUSEBUTTONDOWN:
					break;

				case SDL_JOYBUTTONDOWN:
				case SDL_JOYBUTTONUP:
				case SDL_JOYAXISMOTION:
					handleJoystickEvent(&event);
					break;

				case SDL_KEYDOWN:
					switch (event.key.keysym.sym) {
						case SDLK_UP:
							shiftSelectedItemUp();
							break;
						case SDLK_DOWN:
							shiftSelectedItemDown();
							break;
						case SDLK_r:
							forceUnpause = 1;
						case SDLK_RETURN:
							done = 1;
							break;
						default:
							break;
					} /* Exit keydown switch */
					break;
			
				case SDL_QUIT:
					fprintf(stderr, "SDL_QUIT\n");
					done = 1;
					break;

				default:
					break;
			} /* End eventtype switch */
		} /* End PollEvent loop */

		BESCheckJoysticks();
		gettimeofday(&endTime, NULL);
		elapsedTime = ((endTime.tv_sec - startTime.tv_sec) * 1000000) +
			(endTime.tv_usec - startTime.tv_usec);
		if (elapsedTime < TIME_PER_FRAME)
			usleep(TIME_PER_FRAME - elapsedTime);

		/* Are we really done? */
		if (done)
		{
			if (forceUnpause) /* Pause menu button pressed */
				nextIndex = 0;

			if (nextIndex == 1) { /* Load snapshot */
				loadSnapshot(romname);
				done = 0;
			} else if (nextIndex == 2) { /* Save snapshot */
				Settings.Paused = 0;
				saveSnapshot(romname);
				Settings.Paused = 1;
				//sync();
				
				renderPauseGui(romname);
				done = 0;
			} else if (nextIndex == 3) { /* Quit to menu */
				returnVal = 1;
			} else { /* Exit from pause screen */
				SDL_BlitSurface(currentScreen, NULL, screen, NULL);
				SDL_UpdateRect(screen,0,0,0,0);
				Settings.Paused = 0;
			}	
		}
	} /* End while loop */
	fprintf(stderr, "Pause GUI return val: %d\n", returnVal);
	Settings.Paused = 0;
	return returnVal;
}

static void handleJoystickEvent(SDL_Event *event)
{
	switch(event->type) {
		case SDL_JOYBUTTONDOWN:
		case SDL_JOYBUTTONUP:
			if ((BESDeviceMap[event->jbutton.which] == 0) && /* Gamepad 0 */
				event->type == SDL_JOYBUTTONDOWN)
			{
				if (acceptButton()) {
					if ((event->jbutton.button ==
						BESButtonMap[0][TAG_SELECT-TAG_FIRST_CONTROL]) ||
						(event->jbutton.button ==
							BESButtonMap[0][TAG_START-TAG_FIRST_CONTROL]) )
					{
						done = 1;
					} 
				} /* acceptButton() check */
			} /* Gamepad check */
			break;

		case SDL_JOYAXISMOTION:
			if ((BESDeviceMap[event->jaxis.which] == 0) && /* Gamepad 0 */
				(event->jaxis.axis)) { /* Axis 1 (up and down) */
				if (event->jaxis.value < 0)
					shiftSelectedItemUp();
					//menuPressDirection = -1;
				else if (event->jaxis.value > 0)
					shiftSelectedItemDown();
					//menuPressDirection = 1;
				//else if (event->jaxis.value == 0)
					//menuPressDirection = 0;
			}
			break;
	}	
}

void shiftSelectedItemDown(void)
{
	/* Already shifting to a new index? */
	if (currentIndex != nextIndex) return;

	/* Are we at the end of the list? */
	if (currentIndex == 3) return;

	/* Start the shifting process */
	nextIndex = currentIndex + 1;

	/* Are we skipping the "load snapshot"? */
	if ((nextIndex == 1) && !snapshotAvailable)
		nextIndex++;
	//playOverlaySnd();
}

void shiftSelectedItemUp(void)
{
	/* Already shifting to a new index? */
	if (currentIndex != nextIndex) return;

	/* Are we at the start of the list? */
	if (currentIndex == 0) return;

	/* Start the shifting process */
	nextIndex = currentIndex - 1;
	if ((nextIndex == 1) && !snapshotAvailable)
		nextIndex--;
	//playOverlaySnd();
}

void incrementPauseItemFrame(void)
{
	if (currentIndex != nextIndex)
		frameCounter++;

	/* After 6 frames (0.15 seconds), you can jump to the
	  next pause menu item */
	if (frameCounter == 6) {
		currentIndex = nextIndex;
		frameCounter = 0;
	}
}

/* controls.cpp */
void loadSnapshot(const char *romname) {
	char temp[1024];
	SDL_Surface *tempSurface;
	SDL_PixelFormat *format;

	format = screen->format;

	/* This duplicates the quickload logic */
	sprintf(temp, "%s/%s.000", S9xGetDirectory(SNAPSHOT_DIR), romname);
	fprintf(stderr, "loadSnapshot(%s), '%s'\n", romname, temp);
	S9xUnfreezeGame(temp);
	Settings.Paused = 1;

	/* Load the screenshot associated with this snapshot */
	if (snapshotImage) SDL_FreeSurface(snapshotImage);
	sprintf(temp, "%s/%s.png", S9xGetDirectory(SNAPSHOT_DIR), romname);
	fprintf(stderr, "loadSnapshot(%s), '%s'\n", romname, temp);
	tempSurface = IMG_Load(temp);
	snapshotImage = SDL_ConvertSurface(tempSurface, format, 0);

	/* Replace the current screen background */
	SDL_BlitSurface(snapshotImage, NULL, screen, NULL);
	/* Backup the snapshot image in case the user does an
	  immediate save again for some reason */
	// AWH SDL_BlitSurface(snapshotImage, NULL, currentScreen, NULL);
	 	
	/* Re-render the pause menu */
	renderPauseGui(romname);
}

void saveSnapshot(const char *romname) {
	/* This duplicates the quicksave logic */
	char temp[1024];
	int fdfile, fddir;
	SDL_PixelFormat format;

	fddir = open(S9xGetDirectory(SNAPSHOT_DIR), 0, O_RDWR);
	sprintf(temp, "%s/%s.000", S9xGetDirectory(SNAPSHOT_DIR), romname);
	fprintf(stderr, "saveSnapshot(%s), '%s'\n", romname, temp);
	S9xFreezeGame(temp);
	fdfile = open(temp, 0, O_RDWR);
	if (fdfile > 0)
	{
		fsync(fddir);
		fsync(fdfile);
		close(fdfile);
	}

	/* Write out the screenshot */
	sprintf(temp, "%s/%s.png", S9xGetDirectory(SNAPSHOT_DIR), romname);
	fprintf(stderr, "saveSnapshot(%s), '%s'\n", romname, temp);
	if (snapshotImage) SDL_FreeSurface(snapshotImage);
	snapshotImage = SDL_CreateRGBSurface(0,screen->w, screen->h, 32,
		0xFF0000, 0xFF00, 0xFF, 0xFF000000); 
	SDL_BlitSurface(currentScreen, NULL, snapshotImage, NULL);
	SDL_SavePNG(snapshotImage, temp);
	fdfile = open(temp, 0, O_RDWR);
	if (fdfile > 0)
	{
		fsync(fddir);
		fsync(fdfile);
		close(fdfile);
	}
	close(fddir);
	checkForSnapshot(romname);
}

void checkForSnapshot(const char *romname)
{
	struct stat fileinfo;
	char temp[1024];
	uint32_t dstRow, dstCol;
	uint16_t *srcPixel, *dstPixel;
	float xStep, yStep;
	SDL_Surface *tempSurface;

	snapshotAvailable = 1;
	/* Check for a ROM snapshot */
	sprintf(temp, "%s/%s.000", S9xGetDirectory(SNAPSHOT_DIR), romname);
	if (stat(temp, &fileinfo))
	{
		fprintf(stderr, "Error stat-ing snapshot (ROM)\n");
		snapshotAvailable = 0;
		return;			
	}

	/* Check for a screen snapshot */
	sprintf(temp, "%s/%s.png", S9xGetDirectory(SNAPSHOT_DIR), romname);
	if (stat(temp, &fileinfo))
	{
		fprintf(stderr, "Error stat-ing snapshot (image)\n");
		snapshotAvailable = 0;
		return;
	}

	/* Try to load the screen snapshot */
	if (snapshotImage) SDL_FreeSurface(snapshotImage);
	tempSurface = IMG_Load(temp);
	if (!tempSurface)
	{
		fprintf(stderr, "Error loading snapshot (image)\n");
		snapshotAvailable = 0;
		return;
	} 
	snapshotImage = SDL_ConvertSurface(tempSurface, screen->format, 0);
	SDL_FreeSurface(tempSurface);

	SDL_LockSurface(snapshotImage);
	SDL_LockSurface(tinySnapshotImage);
	/* Shrink the screen snapshot from disk to the 128x96 size */
#if 1
	if (snapshotImage->w == 720)
		xStep = (snapshotImage->w - 40) / 128.0;
	else if (snapshotImage->w == 320)
		xStep = (snapshotImage->w - 32) / 128.0;
	else
		xStep = snapshotImage->w / 128.0;
	yStep = snapshotImage->h / 96.0;
	for (dstRow = 0; dstRow < 96; dstRow++)
	{
		srcPixel = (uint16_t *)snapshotImage->pixels;
		srcPixel += (dstRow * (snapshotImage->pitch / 2) * 
			((int)(yStep)) ); 
		if (snapshotImage->w == 720)
			srcPixel += 40;
		else if (snapshotImage->w == 320)
			srcPixel += 32;
		dstPixel = (uint16_t *)tinySnapshotImage->pixels;
		dstPixel += (dstRow * tinySnapshotImage->pitch / 2);
		           
		for (dstCol = 0; dstCol < 128; dstCol++)
		{                 
			*dstPixel++ = *srcPixel;
			srcPixel += (int)xStep;
		}                 
	}
#endif
	SDL_UnlockSurface(tinySnapshotImage);
	SDL_UnlockSurface(snapshotImage);
}

