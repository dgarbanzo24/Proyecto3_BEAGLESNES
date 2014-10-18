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
#include <sys/time.h>
#include <SDL.h>
#include <SDL/SDL_image.h>
#include "gui.h"
#include "beagleboard.h"

static SDL_Surface *logoText = NULL;
static SDL_Surface *beagleSprites = NULL;

#if !defined(CAPE_LCD3)
static SDL_Rect beagleRect[3] = {
	{0,0,135,165}, {180,0,135,165}, {360,0,135,165} };
#if defined(BEAGLEBONE_BLACK)
static SDL_Rect teleRect = {102,0,113,280};
static SDL_Rect currentPos = {72, 0, 0, 0};
#else
static SDL_Rect teleRect = {102-40,0,113,280};
static SDL_Rect currentPos = {32, 0, 0, 0};
#endif
#else /* CAPE_LCD3 */
static SDL_Rect beagleRect[3] = {
	{0,0,87,84}, {90,0,85,84}, {179,0,87,84} };
static SDL_Rect teleRect = {28, 0, 57, 140};
static SDL_Rect currentPos = {15,0,0,0};
#endif /* CAPE_LCD3 */

static void doTele(void);

void doSplashScreen(void)
{
	struct timeval startTime, endTime, elapsedTime;
#if !defined(BEAGLEBONE_BLACK)
	SDL_Rect offsetRect = {-40,0,0,0};
#endif

	/* Load the images for the splash screen */
#if defined(CAPE_LCD3)
	logoText = IMG_Load("gfx/splash_320x240_bg.png");
	beagleSprites = IMG_Load("gfx/splash_320x240_boris.png");
#else
	/* Load the images for the splash screen */
	logoText = IMG_Load("gfx/splash_720x480_bg.png");
	beagleSprites = IMG_Load("gfx/splash_720x480_boris.png");
#endif

	/* Load the audio for the splash screen */
	loadSplashAudio();

	gettimeofday(&startTime, NULL);
#if defined(BEAGLEBONE_BLACK)
	SDL_BlitSurface(logoText, NULL, screen, NULL);
#else
	SDL_BlitSurface(logoText, NULL, screen, &offsetRect);
#endif
	SDL_UpdateRect(screen, 0, 0, 0, 0);
	playCoinSnd();
	
	pthread_join(loadingThread, NULL);
	gettimeofday(&endTime, NULL);
	elapsedTime.tv_sec = endTime.tv_sec - startTime.tv_sec;
	elapsedTime.tv_usec = endTime.tv_usec - endTime.tv_usec;	

	if (elapsedTime.tv_sec < 2)
		usleep(2000000 - (elapsedTime.tv_sec * 1000000) - 
			elapsedTime.tv_usec);

	doTele();
	usleep(1500000);
}

static void doTele(void)
{
	int frameCount = 0;
	struct timeval startTime, endTime, elapsedTime;
	int passedTime;
#if defined(CAPE_LCD3)
  	int yBasePos = 58;
  	int yInc = 10;
#else
	int yBasePos = 140;
	int yInc = 20;
#endif /* CAPE_LCD3 */

	playTeleSnd();
	usleep(200000);
	while (frameCount < 17)
	{
		gettimeofday(&startTime, NULL);

		/* Clear teleport area */
		SDL_FillRect(screen, &teleRect, 0x0);

		/* Draw shrinking frame */
		if ((frameCount == 0) || (frameCount == 2))
		{
			currentPos.y = yBasePos;
			SDL_BlitSurface(beagleSprites, &beagleRect[1], screen, &currentPos); 
		}
		/* Draw down frame */
		else if (frameCount == 1)
		{
			currentPos.y = yBasePos;
			SDL_BlitSurface(beagleSprites, &beagleRect[2], screen, &currentPos);
		}
		/* Draw tele up */
		else
		{
			currentPos.y = yBasePos - (frameCount * yInc);
			SDL_BlitSurface(beagleSprites, &beagleRect[0], screen, &currentPos);
		}
		SDL_UpdateRect(screen, teleRect.x, teleRect.y, teleRect.w, teleRect.h);
		gettimeofday(&endTime, NULL);
		frameCount++;
		passedTime = (endTime.tv_sec - startTime.tv_sec) * 1000000;
		passedTime += (endTime.tv_usec - startTime.tv_usec);
		if (passedTime < 14000)
			usleep(14000 - passedTime); 
	}
}
