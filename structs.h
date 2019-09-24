#pragma once

#include <Windows.h>

//macros for getting min and max value from 3 numbers
#define tmin(a, b, c)  (fminf(a, fminf(b, c))) 
#define tmax(a, b, c)  (fmaxf(a, fmaxf(b, c)))

// constants for array sizes
#define NUM_MODEL_COLORS 175
#define NUM_PLAYER_MODELS 2

// rgb color structure
typedef struct _stRGB
{
	int r;
	int g;
	int b;
}stRGB, *pstRGB;

// hsl color structure
typedef struct _stHSL
{
	int h;
	int s;
	int l;
}stHSL, *pstHSL;

// chromebot config structure
typedef struct _config
{
	DWORD key;
	DWORD switchtargetkey;
	DWORD closekey;
	stHSL tolerance;
	int fov;
	int delay;
	int burst;
	int pause;
}config, *pconfig;

// player model structure
typedef struct _player_model
{
	stHSL hslModel[NUM_MODEL_COLORS];
}player_model, *pplayer_model;
