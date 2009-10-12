
/*
 *  Copyright (C) 1996 Microsoft Corporation. All Rights Reserved.
 *
 *  File: d3dmain.h
 *
 */
#ifndef __D3DMAIN_H__
#define __D3DMAIN_H__

#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <search.h>
#include <d3d9.h>
#include "d3dapp.h"         /* prototypes for D3D helper functions */
#include <d3d9.h>
#include "d3dapp.h"
#include "d3dmacs.h"
#include "new3d.h"
#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct tagd3dmainglobals {
    HWND hWndMain;          /* application window handle */
    HINSTANCE hInstApp;     /* application instance for dialog boxes */
    BOOL bSingleStepMode;        /* render one frame at a time */
    BOOL bDrawAFrame;            /* render on this pass of the main loop */
    BOOL bShowFrameRate;         /* show the frame rate at the top */
    BOOL bShowInfo;              /* show window information at the bottom */

    BOOL bResized; /* the window has resized or some other drastic change, the
                      entire client area should be cleared */
    BOOL bQuit;    /* program is about to terminate */
#if 0 // bjd
    LPDIRECTDRAWSURFACE lpFrameRateBuffer; /* frame rate surface */
    LPDIRECTDRAWSURFACE lpInfoBuffer;      /* window info surface */
#endif
} d3dmainglobals;

extern	d3dmainglobals	myglobs;
extern	VECTOR Camera_Pos;
extern	uint16 NumGroupsVisible;
extern	uint16 GroupImIn;
extern	uint16 NumCollides;
extern	float ticksperframe;
extern	DWORD GroupPolyCol_timeMax;
extern	long debug_long;
extern	RECT cursorclip;
extern	BYTE                    WhoIAm;

extern	int16		NumLevels;
extern	char	ShortLevelNames[MAXLEVELS][32];
extern	int16		LevelNum;
extern	int16		NewLevelNum;
extern	BOOL		PrimaryLightDetail;
extern	BOOL		SecondaryLightDetail;
extern	BOOL		PickupLightDetail;

extern	BYTE		MyGameStatus;

void TestBlt();
void SetCursorClip( BOOL );
void ReallyShowCursor( BOOL );
void EnablePrimaryLights( void );
void DisablePrimaryLights( void );
void EnableSecondaryLights( void );
void DisableSecondaryLights( void );
void EnablePickupLights( void );
void DisablePickupLights( void );
BOOL FlipBuffers();

extern	int		BikeDetail;

void __cdecl Msg( LPSTR fmt, ... );
int __cdecl RetryMsg( LPSTR fmt, ... );

#ifdef __cplusplus
};
#endif

#endif // __D3DMAIN_H__