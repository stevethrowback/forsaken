
/*
 *  Copyright (C) 1995, 1996 Microsoft Corporation. All Rights Reserved.
 *
 *  File: d3dapp.h
 *
 *  Header to be included in source using D3DApp.  Contains D3DApp function
 *  prototypes and defines.
 *
 *  D3DApp is a collection of helper functions for Direct3D applications.
 *  D3DApp consists of the following files:
 *      d3dapp.h    Main D3DApp header to be included by application
 *      d3dappi.h   Internal header
 *      d3dapp.c    D3DApp functions seen by application.
 *      ddcalls.c   All calls to DirectDraw objects except textures
 *      d3dcalls.c  All calls to Direct3D objects except textures
 *      texture.c   Texture loading and managing texture list
 *      misc.c      Miscellaneous calls
 */

#ifndef __D3DAPP_H__
#define __D3DAPP_H__

//#include <ddraw.h>
#include <d3d9.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef float D3DVALUE, *LPD3DVALUE;

/*
 * DEFINES
 */
#define D3DAPP_WINDOWMINIMUM 50     /* smallest buffer size allowed */
#define D3DAPP_DEFAULTWINDOWDIM 320 /* replaces window size if invalid */
#define D3DAPP_MINBUFFERSIZE 15360  /* minimum "maximum buffer size" for a
                                       D3D driver to be accepted */
#define D3DAPP_MINVERTEXCOUNT 320   /* minimum "maximum vertex count" for a
                                       D3D driver to be accepted */
#define D3DAPP_MAXD3DDRIVERS 5      /* maximum Direct3D drivers ever expected
                                       to find */
#define D3DAPP_MAXTEXTUREFORMATS 10 /* maximum texture formats ever expected
                                       to be reported by a D3D driver */
#define D3DAPP_MAXMODES 32          /* maximum display modes ever expected to
                                       be reported by DirectDraw */
#define D3DAPP_BOGUS -100           /* unused parameters accept this */
#define D3DAPP_YOUDECIDE -25        /* Use this for certain parameters to
                                       have D3DApp decide an appropriate
                                       value for you */
#define D3DAPP_USEWINDOW -24        /* Used in place of fullscreen mode */

/*
 * DATA STRUCTURES
 */

/*
 * D3DAppD3DDriver structure
 * Describes a D3D driver
 */

#if 0 // bjd
typedef struct tagD3DAppD3DDriver {
    char Name[30];      /* short name of the driver */
    char About[50];     /* short string about the driver */
	D3DDEVICEDESC Desc; /* D3DDEVICEDESC for complete information */
    GUID Guid;          /* it's GUID */
    BOOL bIsHardware;   /* does this driver represent a hardware device? */
    BOOL bDoesTextures; /* does this driver do texture mapping? */
    BOOL bDoesZBuffer;  /* can this driver use a z-buffer? */
    BOOL bSquareOnly;  /* can this driver only have Square Texures.. */
    BOOL bCanDoWindow;  /* can it render to Window's display depth? */
	BOOL bTransparency; // Does this Driver do Colour Key Transparency on Textures?
} D3DAppD3DDriver;
#endif

typedef int D3DMATRIXHANDLE;

typedef struct _D3DVIEWPORT { 
    DWORD    dwSize; 
    DWORD    dwX; 
    DWORD    dwY; 
    DWORD    dwWidth; 
    DWORD    dwHeight; 
    D3DVALUE dvScaleX; 
    D3DVALUE dvScaleY; 
    D3DVALUE dvMaxX; 
    D3DVALUE dvMaxY; 
    D3DVALUE dvMinZ; 
    D3DVALUE dvMaxZ; 
} D3DVIEWPORT, *LPD3DVIEWPORT; 

typedef struct {
    DWORD       X;
    DWORD       Y;            /* Viewport Top left */
    DWORD       Width;
    DWORD       Height;       /* Viewport Dimensions */
    float       MinZ;         /* Min/max of clip Volume */
    float       MaxZ;
    D3DVALUE	ScaleX; 
    D3DVALUE	ScaleY; 
} MYD3DVIEWPORT9;

/*
 * D3DAppMode structure
 * Describes a display mode
 */
typedef struct tagD3DAppMode {
    int     w;                /* width */
    int     h;                /* height */
    int     bpp;              /* bits per pixel */
    BOOL    bThisDriverCanDo; /*can current D3D driver render in this mode?*/
} D3DAppMode;

/*
 * D3DAppInfo structure
 * Contains all the information D3DApp makes available to the application. A
 * pointer to the internal, read only copy is returned by the initializing
 * function.
 */
typedef struct tagD3DAppInfo {
    HWND                    hwnd;          /*handle of window being managed*/

	/*
	* Direct3D objects and information
	*/
    LPDIRECT3D9              lpD3D;         /* D3D object */
    LPDIRECT3DDEVICE9        lpD3DDevice;   /* D3D device */
	MYD3DVIEWPORT9			 D3DViewport; /* D3D viewport, created by application */
    int                     NumDrivers;    /* number of D3D drivers avail. */
//    int                     CurrDriver;    /* number of curr. D3D driver */
//    D3DAppD3DDriver         Driver[D3DAPP_MAXD3DDRIVERS]; /* avail. drivers*/
//    D3DAppD3DDriver         ThisDriver;    /* description of this driver, identical to Driver[CurrDriver] */
    /*
     * DirectDraw objects and information
     */
//    LPDIRECTDRAW            lpDD;          /* DirectDraw object */
    BOOL                    bIsPrimary;    /* Is this the primary DD device?
               If FALSE, we're using a hardware DD device that cannot
               display a window and so only fullscreen modes are available */

    int                     NumModes; /* number of available display modes */
    int                     CurrMode; /* number of current display mode (only
                                         when fullscreen) */
    D3DAppMode              Mode[D3DAPP_MAXMODES]; /* desc avail modes */
    D3DAppMode              ThisMode; /* description of this mode, identical
                                         to Mode[CurrMode] */
    BOOL                    bFullscreen; /* in fullscreen exclusive mode? */
    D3DAppMode              WindowsDisplay; /* current Windows disply mode */

    SIZE                    szClient;         /* dimensions of client win */
    POINT                   pClientOnPrimary; /* position of client area */
    POINT                   pWindow;		  /* position of win */

    BOOL                    bPaused;           /* the app is paused */
    BOOL                    bAppActive;        /* the app is active */
    BOOL                    bMinimized;        /* app window is minimized */
    BOOL                    bRenderingIsOK;    /* All objects etc. necessary
                                                  for rendering are in ok */
} D3DAppInfo;

/*
 * FUNCTION PROTOTYPES
 */

/*
 * D3DAppWindowProc
 * To be truly effective, D3DApp should be allowed to trap incoming window
 * messages such as WM_SIZE.  Call D3DAppWindowProc at the begining of the
 * application's main window WindowProc with the message information.  If
 * bStopProcessing is set to TRUE, stop processing the message and return
 * lresult.
 */
BOOL D3DAppWindowProc(BOOL* bStopProcessing, LRESULT* lresult, HWND hwnd,
                      UINT message, WPARAM wParam, LPARAM lParam);

/*
 * D3DAppFullscreen
 * Places the app in a fullscreen mode using the current driver.
 */
BOOL D3DAppFullscreen(int mode);

/*
 * D3DAppWindow
 * Places the application in windowed mode at the given client size.  If w
 * and h are D3DAPP_YOUDECIDE, D3DApp will decide on a suitable client size.
 * If called while in fullscreen, restores the display mode and returns the
 * hooked window to the size it was before a call to D3DAppFullscreen or to
 * the size specified.
 */
BOOL D3DAppWindowMode( int mode );
BOOL D3DAppWindow(int w, int h, int bpp);

/*
 * D3DAppChangeDriver 
 * Changes the driver.  If the current display mode is incompatible with the
 * driver, a new one will be selected and employed.  A new texture format is
 * selected and textures are reloaded, hence their handles may change.  By
 * default, paletted formats are prefered.
 */
//BOOL D3DAppChangeDriver(int driver, DWORD flags);

/*
 * D3DAppPause
 * Use D3DAppPause(TRUE) to pause the app and D3DAppPause(FALSE) to unpause.
 * When fullscreen, the menu bar is redrawn.  bPaused is updated to reflect
 * the current status.
 */
BOOL D3DAppPause(BOOL flag);

/*
 * D3DAppErrorToString
 * Converts a DirectDraw, Direct3D or Direct3D RM error code to a string.
 */
char* D3DAppErrorToString(HRESULT error);

/*
 * D3DAppDestroy
 * Destroys all objects including Direct Draw.  Call before program
 * termination.
 */
BOOL D3DAppDestroy(void);

#ifdef __cplusplus
};
#endif

#endif // __D3DAPP_H__