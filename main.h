#ifndef __MAIN_H__
#define __MAIN_H__

#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <malloc.h>
#include <ddraw.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#define PROJECT_NAME "CFD Internal Flow"
#define INIFILE_NAME "cfdif.ini"
#define LOGFILE_NAME "log.txt"

#define DIK_BUFFERSIZE 1024					//keyboard buffer
#undef KEYUP
#define KEYUP false
#undef KEYDOWN
#define KEYDOWN true

#if defined(__GNUC__)
	#define _alloca_aligned(x,a)	(void *)((((long)__builtin_alloca((x) + (a - 1))) + (a - 1)) & ~(a - 1))
	#define _align16(x)				x __attribute__ ((aligned(16)))
#elif defined(WIN32) || defined(_WIN32)
	#define _alloca_aligned(x,a)	(void *)((((long)_alloca((x) + (a - 1))) + (a - 1)) & ~(a - 1))
	#define _align16(x)				__declspec(align(16)) x
	#pragma warning(disable : 4996)
#else
	#define _alloca_aligned(x,a)	(void *)((((long)alloca((x) + (a - 1))) + (a - 1)) & ~(a - 1))
	#define _align16(x)				x
#endif

#ifdef _X64
	typedef long long _long;
#else
	typedef long _long;
#endif

#ifndef __cplusplus
	typedef enum {false, true} bool;
#endif

#if !defined(__GNUC__) && defined(_X64) && (defined(WIN32) || defined(_WIN32))
	#define num_threads _num_threads
#endif

#ifndef MAX_PATH
	#define MAX_PATH 260
#endif

#define MAX_EXTENSION_LENGTH 16
#define MAX_TOKEN_LENGTH 64

typedef float vec2[2];
typedef float vec3[3];
typedef _align16(float vec4[4]);

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned int dword;

//----------------------------------------
//Reverse the Items in an Array
//		s = start pointer
//		e = end pointer
//		t = temporary item holder
//NOTE: t is a temporary item (non-pointer) declared prior to calling this
//NOTE: s and e must be *modifiable* pointers
#define REVERSE_ARRAY(s, e, t) {						\
	while(s < e)										\
	{													\
		t = *s;											\
		*s = *e;										\
		*e = t;											\
		s++;											\
		e--;											\
	}													\
}

typedef enum
{
	FM_READ,					//file must exist
	FM_WRITE,					//creates new file or empties existing
	FM_APPEND,					//creates new file or appends existing
	FM_READ_AND_WRITE,			//creates new file or empties existing
	FM_READ_AND_APPEND,			//creates new file or appends existing
	FM_CREATE_WRITE,			//creates new file or errors if already exists
	FM_CREATE_READ_AND_WRITE	//creates new file or errors if already exists
} file_mode_e;

//file handle structure
typedef struct
{
	char path[MAX_PATH];
	char ext[MAX_EXTENSION_LENGTH];
	void *syshandle;
} sysfile_handle_t;

typedef struct _mem_alloc_s
{
	void *orig;
	void *aligned;
	struct _mem_alloc_s *next;
} _mem_alloc_t;

typedef struct filelist_s
{
	char name[MAX_PATH];
	struct filelist_s *subdir;			//list of files in subdirectory if name is a subdir
	struct filelist_s *next;
} filelist_t;

typedef enum
{
	R_DETECT,
	R_GENERIC,
	R_ASM
} rpath_e;

typedef struct
{
	bool keyevents[256];				//true = keydown; false = keyup
	bool glextensions[GGL_MAX_EXTENSIONS];
	bool fullscreen;
	dword width, height, colordepth, zbufferdepth;
	int antialiasing;
	int dumpcpuid;						//0 = none, 1 = standard info
	int num_threads;					//this is the total number of threads (including the process)
										//0 means use the number of cores/hyperthreads

	HANDLE *hThreads;
	bool keyboardacquired;
	rpath_e r_path;
	HWND g_hWnd;
	HINSTANCE g_hInstance;
	HDC hDC;
	HGLRC hRC;
} setup_info_t;

extern setup_info_t si;

void *_malloc_aligned(const dword x, const dword a);
void _free_aligned(void *);
LPCWSTR MC2UC(const char *string);
void __cdecl Sys_Error(const char *error, ...);
void __cdecl Sys_Message(const char *msg, ...);
void __cdecl Sys_Log(const char *log, ...);
char *Sys_Timestamp(void);
int Sys_GetTime(void);
void Sys_Shutdown(void);
bool Sys_GetFilePathExtension(const char *path, char *extension, int len);
char *Sys_GetFileExtension(const void *handle);
void *Sys_OpenFile(const char *fname, file_mode_e fm);
void Sys_CloseFile(const void *handle);
long long Sys_GetFileLength(const void *handle);
dword Sys_ReadFile(const void *handle, dword size, byte *data);
filelist_t *Sys_ListFiles(const char *path, const char *ext, int *num_files, const bool subdirs);
void Sys_DeleteFileList(filelist_t *fl);
bool Sys_CreateThreads(const dword num);
void *Sys_CreateSignal(void);
void *Sys_CreateMutex(void);
void Sys_LockMutex(void *mutex);
void Sys_UnlockMutex(void *mutex);
void Sys_SignalThread(void *signal);
void Sys_BlockThread(void *signal, void *mutex);
void Sys_DestroyMutex(void *mutex);
void Sys_DestroySignal(void *signal);
void Sys_ExitThread(const int value);
void Sys_TerminateThreads(const dword num);
bool Sys_ReadToken(char *token, char **line, const dword size);
void Sys_ReadIniFile(void);
void In_Init(void);
void In_ReleaseKeyboard(void);
void In_AcquireKeyboard(void);
int In_TranslateKey(const int key);
void In_UpdateKeyboard(void);
void In_Shutdown(void);
void Sys_SetFullscreenVideo(const dword w, const dword h, const dword colordepth);
void Sys_VideoInit(void);
void Sys_VideoShutdown(void);
void Sys_UpdateScreen(void);
void Sys_InitByteOrder(void);
LRESULT __stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void SetupCores(void);
void Init(void);
void Render(void);
void Shutdown(void);

extern short (*LittleShort)(const short var);
extern int (*LittleInt)(const int var);
extern float (*LittleFloat)(const float var);
extern short (*BigShort)(const short var);
extern int (*BigInt)(const int var);
extern float (*BigFloat)(const float var);

#ifdef INPUT_USE_DIRECTINPUTKEYBOARD
	//key translation level 1 scan codes for DirectInput
	typedef enum
	{
		GKEY_NONE,
		GKEY_ESCAPE,
		GKEY_1,
		GKEY_2,
		GKEY_3,
		GKEY_4,
		GKEY_5,
		GKEY_6,
		GKEY_7,
		GKEY_8,
		GKEY_9,
		GKEY_0,
		GKEY_MINUS,
		GKEY_EQUALS,
		GKEY_BACKSPACE,
		GKEY_TAB,
		GKEY_Q,
		GKEY_W,
		GKEY_E,
		GKEY_R,
		GKEY_T,
		GKEY_Y,
		GKEY_U,
		GKEY_I,
		GKEY_O,
		GKEY_P,
		GKEY_LBRACKET,
		GKEY_RBRACKET,
		GKEY_ENTER,
		GKEY_LCONTROL,
		GKEY_A,
		GKEY_S,
		GKEY_D,
		GKEY_F,
		GKEY_G,
		GKEY_H,
		GKEY_J,
		GKEY_K,
		GKEY_L,
		GKEY_SEMICOLON,
		GKEY_APOSTROPHE,
		GKEY_GRAVE,
		GKEY_LSHIFT,
		GKEY_BACKSLASH,
		GKEY_Z,
		GKEY_X,
		GKEY_C,
		GKEY_V,
		GKEY_B,
		GKEY_N,
		GKEY_M,
		GKEY_COMMA,
		GKEY_PERIOD,
		GKEY_SLASH,
		GKEY_RSHIFT,
		GKEYPAD_MULTIPLY,
		GKEY_LALT,
		GKEY_SPACE,
		GKEY_CAPSLOCK,
		GKEY_F1,
		GKEY_F2,
		GKEY_F3,
		GKEY_F4,
		GKEY_F5,
		GKEY_F6,
		GKEY_F7,
		GKEY_F8,
		GKEY_F9,
		GKEY_F10,
		GKEY_NUMLOCK,
		GKEY_SCROLLLOCK,
		GKEYPAD_7,
		GKEYPAD_8,
		GKEYPAD_9,
		GKEYPAD_SUBTRACT,
		GKEYPAD_4,
		GKEYPAD_5,
		GKEYPAD_6,
		GKEYPAD_ADD,
		GKEYPAD_1,
		GKEYPAD_2,
		GKEYPAD_3,
		GKEYPAD_0,
		GKEYPAD_DECIMAL,
		GKEY_F11,
		GKEY_F12,
		GKEYPAD_ENTER,
		GKEY_RCONTROL,
		GKEY_VOLUMEDOWN,
		GKEY_VOLUMEUP,
		GKEYPAD_DIVIDE,
		GKEY_PRINTSCREEN,
		GKEY_RALT,
		GKEY_PAUSE,
		GKEY_HOME,
		GKEY_UP,
		GKEY_PAGEUP,
		GKEY_LEFT,
		GKEY_RIGHT,
		GKEY_END,
		GKEY_DOWN,
		GKEY_PAGEDOWN,
		GKEY_INSERT,
		GKEY_DELETE,
		MAX_GKEYS
	} keys_e;
#else
	//follows VK_* virtual key codes
	typedef enum
	{
		GKEY_NONE,
		GKEY_BACKSPACE = 0x08,
		GKEY_TAB,
		GKEY_ENTER = 0x0D,
		GKEY_PAUSE = 0x13,
		GKEY_CAPSLOCK,
		GKEY_ESCAPE = 0x1B,
		GKEY_SPACE = 0x20,
		GKEY_PAGEUP,
		GKEY_PAGEDOWN,
		GKEY_END,
		GKEY_HOME,
		GKEY_LEFT,
		GKEY_UP,
		GKEY_RIGHT,
		GKEY_DOWN,
		GKEY_PRINTSCREEN = 0x2C,
		GKEY_INSERT,
		GKEY_DELETE,
		GKEY_0 = 0x30,
		GKEY_1,
		GKEY_2,
		GKEY_3,
		GKEY_4,
		GKEY_5,
		GKEY_6,
		GKEY_7,
		GKEY_8,
		GKEY_9,
		GKEY_A = 0x41,
		GKEY_B,
		GKEY_C,
		GKEY_D,
		GKEY_E,
		GKEY_F,
		GKEY_G,
		GKEY_H,
		GKEY_I,
		GKEY_J,
		GKEY_K,
		GKEY_L,
		GKEY_M,
		GKEY_N,
		GKEY_O,
		GKEY_P,
		GKEY_Q,
		GKEY_R,
		GKEY_S,
		GKEY_T,
		GKEY_U,
		GKEY_V,
		GKEY_W,
		GKEY_X,
		GKEY_Y,
		GKEY_Z,
		GKEYPAD_0 = 0x60,
		GKEYPAD_1,
		GKEYPAD_2,
		GKEYPAD_3,
		GKEYPAD_4,
		GKEYPAD_5,
		GKEYPAD_6,
		GKEYPAD_7,
		GKEYPAD_8,
		GKEYPAD_9,
		GKEYPAD_MULTIPLY,
		GKEYPAD_ADD,
		GKEYPAD_ENTER,
		GKEYPAD_SUBTRACT,
		GKEYPAD_DECIMAL,
		GKEYPAD_DIVIDE,
		GKEY_F1,
		GKEY_F2,
		GKEY_F3,
		GKEY_F4,
		GKEY_F5,
		GKEY_F6,
		GKEY_F7,
		GKEY_F8,
		GKEY_F9,
		GKEY_F10,
		GKEY_F11,
		GKEY_F12,
		GKEY_NUMLOCK = 0x90,
		GKEY_SCROLLLOCK,
		GKEY_LSHIFT = 0xA0,
		GKEY_RSHIFT,
		GKEY_LCONTROL,
		GKEY_RCONTROL,
		GKEY_LALT,
		GKEY_RALT,
		GKEY_VOLUMEDOWN = 0xAE,
		GKEY_VOLUMEUP,
		GKEY_SEMICOLON = 0xBA,
		GKEY_EQUALS,
		GKEY_COMMA,
		GKEY_MINUS,
		GKEY_PERIOD,
		GKEY_SLASH,
		GKEY_GRAVE,
		GKEY_LBRACKET = 0xDB,
		GKEY_BACKSLASH,
		GKEY_RBRACKET,
		GKEY_APOSTROPHE,
		MAX_GKEYS
	} keys_e;
#endif

#endif		//__MAIN_H__
