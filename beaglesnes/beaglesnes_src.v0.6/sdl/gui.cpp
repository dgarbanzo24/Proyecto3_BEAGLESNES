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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>

#include <SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>

#include "gui.h"

#if defined(BEAGLEBONE_BLACK)
// USB hotplugging work around hack
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/usbdevice_fs.h>
#endif

pthread_t loadingThread;
static void *loadingThreadFunc(void *);

#define GRADIENT_Y_POS 55

#if 0
#define SELECT_BUTTON 8
#define START_BUTTON 9
#else
int selectButtonNum = 0xFF;
int startButtonNum = 0xFF;
#endif 
/* AWH: DEBUG - Comment in for NTSC overscan markers on screen */
/*#define OVERSCAN_TEST 1*/

static int i, retVal, done;

static SDL_Surface *logo;
static SDL_Surface *gradient;
SDL_Surface *screen;
#if defined(CAPE_LCD3)
static SDL_Rect gradientRect = {0, 24, 0, 0};
static SDL_Rect logoRect = {15, 2, 0, 0};
#else
static SDL_Rect gradientRect = {0, GRADIENT_Y_POS, 0, 0};
static SDL_Rect logoRect = {55, 30, 0, 0};
#endif /* CAPE_LCD3 */

#if defined(BEAGLEBONE_BLACK)
#if defined(CAPE_LCD3)
static SDL_Rect backgroundRect = {0, 55, 320, 240-55};
//audioAvailable = 0;
#else
static SDL_Rect backgroundRect = {0, 80, 720, 480-80};
#endif /* CAPE_LCD3 */
#else
static SDL_Rect backgroundRect = {0, 80, 350, 480-80};
#endif

#if defined(BEAGLEBONE_BLACK)
static const char *joystickPath[NUM_JOYSTICKS*2] = {
  /* USB device paths for gamepads plugged into the USB host port */
  "/dev/input/by-path/platform-musb-hdrc.1.auto-usb-0:1:1.0-joystick",
  "DUMMY",
  /* USB device paths for gamepads plugged into a USB hub */
  "/dev/input/by-path/platform-musb-hdrc.1.auto-usb-0:1.1:1.0-joystick",
  "/dev/input/by-path/platform-musb-hdrc.1.auto-usb-0:1.2:1.0-joystick" };
#elif defined(BEAGLEBOARD_XM)
static const char *joystickPath[NUM_JOYSTICKS] = {
  "/dev/input/by-path/platform-ehci-omap.0-usb-0:2.2:1.0-joystick", 
  "/dev/input/by-path/platform-ehci-omap.0-usb-0:2.4:1.0-joystick" };
#else
static const char *joystickPath[NUM_JOYSTICKS] = {"/dev/input/js0", "/dev/input/js1"};
#endif
static SDL_Joystick *joystick[NUM_JOYSTICKS] = {NULL, NULL};
int BESDeviceMap[NUM_JOYSTICKS] = {-1, -1};
int BESControllerPresent[NUM_JOYSTICKS] = {0, 0};
 

#define JOYSTICK_PLUGGED 1
#define JOYSTICK_UNPLUGGED 0

/* Bitmask of which joysticks are plugged in (1) and which aren't (0) */
static int joystickState = 0;
static int elapsedTime;
static struct timeval startTime, endTime;
static int menuPressDirection = 0;
int volumePressDirection = 0;

//extern gameInfo_t *gameInfo;
int totalGames;
int audioAvailable = 1;
int currentVolume = 32;
int volumeOverlayCount;

void BESResetJoysticks(void) {
  joystickState = 0;
  BESCheckJoysticks();
}

void BESCheckJoysticks(void) {
  bool update = false;
  bool restartJoysticks = false;
  int oldJoystickState = joystickState;
  joystickState = 0;
  char tempBuf[128];
  struct stat joyStat;

  /* Check if the joysticks currently exist */
#if defined(BEAGLEBONE_BLACK)
  for (i=0; i < (NUM_JOYSTICKS*2); i++) {
#else
  for (i=0; i < NUM_JOYSTICKS; i++) {
#endif
    update = false;
#if !defined(PC_PLATFORM)
    retVal = readlink(joystickPath[i], tempBuf, sizeof(tempBuf)-1);
#else
    retVal = stat(joystickPath[i], &joyStat);
    strncpy(tempBuf, joystickPath[i], sizeof(tempBuf)-1);
#endif /* DESKTOP_PC */
    if ((retVal == -1) /* No joystick */ && 
      ((oldJoystickState >> i) & 0x1) /* Joystick was plugged in */ )
    {
      update = true;
      restartJoysticks = true;
      fprintf(stderr, "Player %d Joystick has been unplugged\n", i+1);
    }
    else if ((retVal != -1) /* Joystick exists */ &&
      !((oldJoystickState >> i) & 0x1) /* Joystick was not plugged in */ ) 
    {
      joystickState |= (JOYSTICK_PLUGGED << i); /* Joystick is now plugged in */
      update = true;
      restartJoysticks = true;
      fprintf(stderr, "Player %d Joystick has been plugged in\n", i+1);
    }
    else 
      joystickState |= ((oldJoystickState >> i) & 0x1) << i;

    if (update) {
#if defined(BEAGLEBONE_BLACK)
    if (retVal != -1) {
      /* Are we updating the gamepad in the USB host port? */
      if (i < NUM_JOYSTICKS) {
        BESDeviceMap[tempBuf[strlen(tempBuf) - 1] - '0'] = i;
        BESControllerPresent[i] = 1;
      /* Are we updating gamepads plugged into a USB hub? */
      } else {
        BESDeviceMap[tempBuf[strlen(tempBuf) - 1] - '0'] = (i-NUM_JOYSTICKS);
        BESControllerPresent[(i-NUM_JOYSTICKS)] = 1;
      }     
    }
    else
    {
      /* Are we updating the gamepad in the USB host port? */
      if (i < NUM_JOYSTICKS) {
        BESDeviceMap[i] = -1;
        BESControllerPresent[i] = 0;
      /* Are we updating gamepads plugged into a USB hub? */
      } else {
        BESDeviceMap[(i-NUM_JOYSTICKS)] = -1;
        BESControllerPresent[(i-NUM_JOYSTICKS)] = 0;
      }
    }
#else
    if (retVal != -1) {
      BESDeviceMap[tempBuf[strlen(tempBuf) - 1] - '0'] = i;
      BESControllerPresent[i] = 1;
/* Test */
fprintf(stderr, "Device map [%d], %d\n", (tempBuf[strlen(tempBuf)-1] - '0'), i);
/* Test */
    }
    else {
      BESDeviceMap[i] = -1;
      BESControllerPresent[i] = 0;
/* Test */
fprintf(stderr, "Device map [%d], -1\n", i);
/* Test */

    }
#endif
    } /* End "update" check */
  }

  /* Restart the joystick subsystem */
  if (restartJoysticks) {
    /* Shut down the joystick subsystem */
    SDL_QuitSubSystem(SDL_INIT_JOYSTICK);

    /* Turn the joystick system back on if we have at least one joystick */
    if (joystickState) {
      SDL_InitSubSystem(SDL_INIT_JOYSTICK);
      for (i=0; i < NUM_JOYSTICKS; i++) {
        joystick[i] = SDL_JoystickOpen(i);
      }
      SDL_JoystickEventState(SDL_ENABLE);
    }
  }
}

static void handleJoystickEvent(SDL_Event *event)
{
//fprintf(stderr, "handleJoystickEvent()\n");
  switch(event->type) {
    case SDL_JOYBUTTONDOWN:
    case SDL_JOYBUTTONUP:
      if ((BESDeviceMap[event->jbutton.which] == 0) && // Gamepad 0
        event->type == SDL_JOYBUTTONDOWN)
      {
        if (acceptButton()) {
#if 0 // AWH - New button mapping support
          switch(event->jbutton.button)
          {
            case SELECT_BUTTON:
            case START_BUTTON:
#else
          if ((event->jbutton.button == 
              BESButtonMap[0][TAG_SELECT-TAG_FIRST_CONTROL]) ||
            (event->jbutton.button == 
              BESButtonMap[0][TAG_START-TAG_FIRST_CONTROL]) )
          {
#endif // AWH
              playSelectSnd();
              done = 1;
          }
        }
      }
      break;
    case SDL_JOYAXISMOTION:
      if ((BESDeviceMap[event->jaxis.which] == 0) && // Gamepad 0
        (event->jaxis.axis)) { // Axis 1 (up and down)
        if (event->jaxis.value < 0)
          menuPressDirection = -1;
          //shiftSelectedGameUp();
        else if (event->jaxis.value > 0)
          menuPressDirection = 1;
          //shiftSelectedGameDown();
        else if (event->jaxis.value == 0)
          menuPressDirection = 0;
      }
#if 0 // AWH
      if ((BESDeviceMap[event->jaxis.which] == 0) && // Gamepad 0
        (event->jaxis.axis == 0)) { // Axis 0 (left and right)
        if ((event->jaxis.value < 0) && (currentVolume >= 2)) {
          currentVolume = currentVolume - 2;
          fprintf(stderr, "Decrease volume to %d\n", currentVolume);
        }
        else if ((event->jaxis.value > 0) && (currentVolume < 64)) {
          currentVolume = currentVolume + 2;
          fprintf(stderr, "Increase volume to %d\n", currentVolume);
        }
        changeVolume();
      }
#endif // AWH
      break;
  }
}

static void quit(int rc)
{
  TTF_Quit();
  SDL_Quit();
  exit(rc);
}

int doGuiSetup(void)
{
  /* Initialize SDL */
  if ( SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0 ) {
    fprintf(stderr, "Couldn't initialize SDL: %s\n",SDL_GetError());
    return(1);
  }

  /* Set video mode */
#if defined(BEAGLEBONE_BLACK)
#if defined(CAPE_LCD3)
  if ( (screen=SDL_SetVideoMode(/*320,240,16,0*/0,0,0,0)) == NULL ) {
    fprintf(stderr, "Couldn't set 320x240x16 video mode: %s\n",
#else
  if ( (screen=SDL_SetVideoMode(720,480,16,0)) == NULL ) {
    fprintf(stderr, "Couldn't set 720x480x16 video mode: %s\n",
#endif /* CAPE_LCD3 */
#else
  if ( (screen=SDL_SetVideoMode(640,480,16,0)) == NULL ) {
    fprintf(stderr, "Couldn't set 640x480x16 video mode: %s\n",
#endif
      SDL_GetError());
    quit(2);
  }
  /* Shut off the mouse pointer */
  SDL_ShowCursor(SDL_DISABLE);

  /* Init font engine */
  if(TTF_Init()==-1) {
    fprintf(stderr, "TTF_Init: %s\n", TTF_GetError());
    quit(2);
  }

  return(0);
}

void disableGuiAudio(void)
{
  if (audioAvailable)
  {
    Mix_CloseAudio();
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
  }
}

void enableGuiAudio(void)
{
  if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
    fprintf(stderr, "Couldn't initialize audio: %s\n",SDL_GetError());
    audioAvailable = 0;
  }
  initAudio();
}

void *loadingThreadFunc(void *)
{
  logo = IMG_Load("gfx/logo_trans.png");
  gradient = IMG_Load("gfx/gradient.png");

#if 0 
  /* Get SOMETHING on the screen ASAP */
  done = 0;
  SDL_FillRect(screen, NULL, 0x0);
  SDL_BlitSurface(gradient, NULL, screen, &gradientRect);
  SDL_BlitSurface(logo, NULL, screen, &logoRect);
  SDL_UpdateRect(screen, 0, 0, 0, 0);
#endif
  loadGameConfig();
  loadInstruct();
  loadGameLists();
  loadGameInfo();

#if 0
  SDL_UpdateRect(screen, 0, 0, 0, 0);
#endif
  loadAudio();
  loadPauseGui();
 
  pthread_exit(0);
}

void doGuiLoadingThread(void)
{
  pthread_create(&loadingThread, NULL, (void* (*)(void*)) &loadingThreadFunc, NULL);
}

int doGui(void) {
  int i, k, r, g, b;
  SDL_Event event;
  Uint16 pixel;


  /* Check for joysticks */
  BESResetJoysticks();

  /* Get SOMETHING on the screen ASAP */
  done = 0;
  SDL_FillRect(screen, NULL, 0x0);
  SDL_BlitSurface(gradient, NULL, screen, &gradientRect);
  SDL_BlitSurface(logo, NULL, screen, &logoRect);
  SDL_UpdateRect(screen, 0, 0, 0, 0);
  startAudio();

  while ( !done ) {
    gettimeofday(&startTime, NULL);

    renderGameList(screen);
    renderGameInfo(screen, currentSelectedGameIndex());
    renderInstruct(screen, BESControllerPresent[0]);
#ifdef CAPE_LCD3
    renderVolume(screen);
#endif /* CAPE_LCD3 */
#ifdef OVERSCAN_TEST
    SDL_FillRect(screen, &overscanLeft, 0xF800);
    SDL_FillRect(screen, &overscanRight, 0xF800);
    SDL_FillRect(screen, &overscanTop, 0xF800);
    SDL_FillRect(screen, &overscanBottom, 0xF800);
#endif

    incrementGameListFrame();

    // Update panel regions
    // NTSC SDL_UpdateRect(screen, 0, 80, 350, 482 - 80 - 25);
    // NTSC SDL_UpdateRect(screen, 350, 80, 720-350, 482 - 80); 
#if 0 // AWH
    SDL_UpdateRect(screen, 0, 80, 310, 480 - 80);
    SDL_UpdateRect(screen, 310, 80, 640 - 310, 480 - 80); 
#endif
#if defined(BEAGLEBONE_BLACK)
#if defined(CAPE_LCD3)
    SDL_UpdateRect(screen, 0, 50, 320, 240 - 50);
#else
    SDL_UpdateRect(screen, 0, 80, 720, 480 - 80);
#endif /* CAPE_LCD3 */
#else
    SDL_UpdateRect(screen, 0, 80, 640, 480 - 80);
#endif

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

//#ifdef CAPE_LCD3
        case SDL_KEYUP:
          switch (event.key.keysym.sym) {
            case SDLK_LEFT:
            case SDLK_RIGHT:
              volumePressDirection = 0;
              break;
            default:
              break;
          }
          break;

        case SDL_KEYDOWN:
          switch (event.key.keysym.sym) {
            case SDLK_LEFT:
              shiftSelectedVolumeUp();
              //volumePressDirection = 1;
              //currentVolume = currentVolume - 2;
              //fprintf(stderr, "Decrease volume to %d\n", currentVolume);
              //changeVolume();
              break;
            case SDLK_RIGHT:
              shiftSelectedVolumeDown();
              //volumePressDirection = -1;
              //currentVolume = currentVolume + 2;
              //fprintf(stderr, "Increase volume to %d\n", currentVolume);
              //changeVolume();
              break;
            case SDLK_UP:
              shiftSelectedGameUp();
              break;
            case SDLK_DOWN:
              shiftSelectedGameDown();
              break;
            case SDLK_RETURN:
              playSelectSnd();
              done = 1;
              break;
            default:
              break;
          } // End keydown switch
          break;
//#endif /* CAPE_LCD3 */
        case SDL_QUIT:
fprintf(stderr, "SDL_QUIT\n");
          done = 1;
          break;
        default:
          break;
      } // End eventtype switch
    } // End PollEvent loop
    if (menuPressDirection == -1)
      shiftSelectedGameUp();
    else if (menuPressDirection == 1)
      shiftSelectedGameDown();

    if (volumePressDirection == 1)
      shiftSelectedVolumeUp();
    else if (volumePressDirection == -1)
      shiftSelectedVolumeDown();

    BESCheckJoysticks();
    gettimeofday(&endTime, NULL);
    elapsedTime = ((endTime.tv_sec - startTime.tv_sec) * 1000000) + 
      (endTime.tv_usec - startTime.tv_usec);
    if (elapsedTime < TIME_PER_FRAME)
      usleep(TIME_PER_FRAME - elapsedTime);
  } // End while loop

  fadeAudio();
  // Fade out
  for (i = 0; i < 16; i++) {
    SDL_LockSurface(screen);
    for (k = 0; k < (screen->w * screen->h); k++) {
      pixel = ((Uint16 *)(screen->pixels))[k];
      r = (((pixel & 0xF800) >> 11) - 2) << 11;
      g = (((pixel & 0x07E0) >> 5) - 4) << 5;
      b = (pixel & 0x001F) - 2;
      if (r < 0) r = 0;
      if (g < 0) g = 0;
      if (b < 0) b = 0;
      ((Uint16 *)(screen->pixels))[k] = r | g | b; 
    }
    SDL_UnlockSurface(screen);
#if defined(BEAGLEBONE_BLACK)
#if defined(CAPE_LCD3)
    SDL_UpdateRect(screen, 0, 0, 320, 240);
#else
    SDL_UpdateRect(screen, 0, 0, 720, 480);
#endif
#else
    SDL_UpdateRect(screen, 0, 0, 640, 480);
#endif
  }

  if (audioAvailable) {
    // This is our cleanup
    usleep(1000000); // Give the audio fadeout a little time
  }
  return(currentSelectedGameIndex());
}

void shiftSelectedVolumeUp(void) {
  if (currentVolume < 64) {
    currentVolume = currentVolume + 8;
    //fprintf(stderr, "Increase volume to %d\n", currentVolume);
  }
  changeVolume();
}
void shiftSelectedVolumeDown(void) {
  if (currentVolume >= 8) {
    currentVolume = currentVolume - 8;
    //fprintf(stderr, "Decrease volume to %d\n", currentVolume);
  }
  changeVolume();
}
#ifdef CAPE_LCD3
void renderVolume(SDL_Surface *surface) {
  SDL_Rect bar = {45, 170, 20, 30};
  if (!audioAvailable || volumeOverlayCount <= 0) return;

  for (int i = 0; i < (currentVolume >> 3); i++) {
    SDL_FillRect(surface, &bar, 0x07E0);
    bar.x += 30;
  }
  volumeOverlayCount--;
}
#endif /* CAPE_LCD3 */
