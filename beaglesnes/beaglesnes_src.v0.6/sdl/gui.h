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

#ifndef __GUI_H__
#define __GUI_H__

#include <pthread.h>
#include <stdint.h>
#include "beagleboard.h"

struct SDL_Surface;
extern SDL_Surface *screen;

#define NUM_JOYSTICKS 2
#define TARGET_FPS 40
#define TIME_PER_FRAME (1000000 / TARGET_FPS)

/* Map of js0, js1, etc. to the proper joystick (-1 if no joystick) */
extern int BESDeviceMap[NUM_JOYSTICKS];
extern void BESResetJoysticks(void);
extern void BESCheckJoysticks(void);
extern int BESControllerPresent[NUM_JOYSTICKS];
extern uint32_t BESPauseCombo;

extern int doGuiSetup(void);
extern int doGui(void);
extern void loadGameInfo(void);
extern void loadGameLists(void);
extern void loadInstruct(void);
extern void loadAudio(void);

/* GPIO */
/* Four axis buttons, eight controller buttons, one pause */
#define GPIO_MAP_SIZE 13
extern int gpioPinSetup(void);
extern void gpioEvents(void);

/* guiParser.c */
extern int loadGameConfig(void);

extern void renderInstruct(SDL_Surface *screen, int gamepadPresent);
extern void renderGameList(SDL_Surface *screen);
extern void renderGameInfo(SDL_Surface *screen, int i);
#if defined (CAPE_LCD3)
extern void renderVolume(SDL_Surface *surface);
#endif /* CAPE_LCD3 */
extern int currentSelectedGameIndex(void);
extern void incrementGameListFrame(void);
extern void shiftSelectedGameUp(void);
extern void shiftSelectedGameDown(void);
extern void enableGuiAudio(void);
extern void disableGuiAudio(void);
extern void initAudio(void);
extern void startAudio(void);
extern void fadeAudio(void);
extern void playSelectSnd(void);
extern void playOverlaySnd(void);
extern void playTeleSnd(void);
extern void playCoinSnd(void);
extern void changeVolume(void);
extern int acceptButton(void);

#define DEFAULT_BOX_IMAGE "box_image.png"
#define DEFAULT_DATE_TEXT "19XX"
#define MAX_GENRE_TYPES 2
#define MAX_TEXT_LINES 5

#define GAME_TITLE_SIZE 64
#define ROM_FILE_SIZE 128
#define IMAGE_FILE_SIZE 128
#define INFO_TEXT_SIZE 64
#define DATE_TEXT_SIZE 5
#define GENRE_TEXT_SIZE 32

/* Snapshot load screen */
enum {
  /* No snapshot for this game */
  SNAPSHOT_LOAD_NONE = 0,
  /* Snapshot available */
  SNAPSHOT_LOAD_SKIP,
  SNAPSHOT_LOAD_RESTORE,
  SNAPSHOT_LOAD_DELETE
};

enum {
  PLATFORM_INVALID = -1,
  PLATFORM_FIRST = 0,
  PLATFORM_SNES = PLATFORM_FIRST,
  PLATFORM_GBA,
  PLATFORM_NES,
  NUM_PLATFORMS
};

enum {
  PLAYER_INVALID = -1,
  PLAYER_ONE = 0,
  PLAYER_TWO,
  NUM_PLAYERS
};

/* Define the XML tags used in the games.xml */
typedef enum {
  /* Start! */
  TAG_FIRST = 0,
  /* Root config tag */
  TAG_CONFIG = TAG_FIRST,
  /* Platform tags */
  TAG_FIRST_PLATFORM,
  TAG_SNES = TAG_FIRST_PLATFORM,
  TAG_GBA,
  TAG_NES,
  TAG_LAST_PLATFORM = TAG_NES,
  /* Game menu tags */
  TAG_GAME_FIRST,
  TAG_GAME = TAG_GAME_FIRST,
  TAG_TITLE,
  TAG_ROM,
  TAG_IMAGE,
  TAG_YEAR,
  /* There can be multiple of these per "game" tag */
  TAG_GENRE,
  TAG_TEXT,
  TAG_GAME_LAST = TAG_TEXT,
  /* Controller config tags */
  TAG_PLAYER1,
  TAG_PLAYER2,
  TAG_FIRST_CONTROL,
  TAG_VAXIS = TAG_FIRST_CONTROL,
  TAG_HAXIS,
  TAG_LEFT,
  TAG_RIGHT,
  TAG_A,
  TAG_B,
  TAG_X,
  TAG_Y,
  TAG_SELECT,
  TAG_START,
  TAG_PAUSE,
  TAG_LAST_CONTROL = TAG_PAUSE,
  /* GPIO control mapping tags */
  TAG_GPIO,
  TAG_FIRST_GPIO_CONTROL,
  TAG_GPIO_GPLEFT = TAG_FIRST_GPIO_CONTROL,
  TAG_GPIO_GPRIGHT,
  TAG_GPIO_GPUP,
  TAG_GPIO_GPDOWN,
  TAG_GPIO_LEFT,
  TAG_GPIO_RIGHT,
  TAG_GPIO_A,
  TAG_GPIO_B,
  TAG_GPIO_X,
  TAG_GPIO_Y,
  TAG_GPIO_SELECT,
  TAG_GPIO_START,
  TAG_GPIO_PAUSE,
  /* Pause key and key combo */
  TAG_PAUSE_COMBO,
  TAG_PAUSE_KEY,
  /* Done! */
  TAG_LAST
} TagUsed_t;

/* Eight buttons, one pause, and two axis */
#define BUTTON_MAP_SIZE 11

extern unsigned char BESButtonMap[NUM_PLAYERS][BUTTON_MAP_SIZE];
extern unsigned char GPIOButtonMap[GPIO_MAP_SIZE];

/* Linked list node for game information */
typedef struct _gameInfo {
  /* The name of the game */
  char gameTitle[GAME_TITLE_SIZE];
  /* Filename of the ROM image */
  char romFile[ROM_FILE_SIZE];
  /* Filename of the image of the game's box */
  char imageFile[IMAGE_FILE_SIZE];
  /* Lines of text that describe the game */
  char infoText[MAX_TEXT_LINES][INFO_TEXT_SIZE];
  /* Four digit year the game was released */
  char dateText[DATE_TEXT_SIZE];
  /* Short descriptive genre text */
  char genreText[MAX_GENRE_TYPES][GENRE_TEXT_SIZE];
  /* Platform this game runs on */
  int platform;
  /* Link to next game in list */
  struct _gameInfo *next;
  /* Link to prev game in list */
  struct _gameInfo *prev;
} gameInfo_t;

extern gameInfo_t *gameInfo;
extern int totalGames;
extern int currentPlatform;

extern int audioAvailable;

extern int currentVolume;
extern int volumePressDirection;
extern int volumeOverlayCount;

extern void shiftSelectedVolumeUp(void);
extern void shiftSelectedVolumeDown(void);

extern int selectButtonNum;
extern int startButtonNum;

extern int emuDone;
/* Emulator launch functions */
extern int gba_main(char *filename);
extern int nes_main(char *filename);

/* Loading background thread */
extern pthread_t loadingThread;
extern void doGuiLoadingThread(void);

/* Splash screen */
extern void loadSplashAudio(void);
extern void doSplashScreen(void);

/* Pause GUI */
extern void loadPauseGui(void);
extern uint32_t doPauseGui(const char *romname, int width, int height);

#endif /* __GUI_H__ */

