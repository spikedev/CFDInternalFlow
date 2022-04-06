#ifndef __OPENGL_H__
#define __OPENGL_H__

#include <windows.h>
#include <GL/gl.h>
#include "GL/glext.h"
#include "GL/wglext.h"

#if !defined(__GNUC__) && defined(RT_X64) && (defined(WIN32) || defined(_WIN32))
#define gglColor3ub						_gglColor3ub
#define gglColor3fv						_gglColor3fv
#define gglBegin						_gglBegin
#define gglVertex3f						_gglVertex3f
#define gglEnd							_gglEnd
#endif

extern BOOL (WINAPI *gwglCopyContext)(HGLRC, HGLRC, UINT);
extern HGLRC (WINAPI *gwglCreateContext)(HDC);
extern HGLRC (WINAPI *gwglCreateLayerContext)(HDC, int);
extern BOOL (WINAPI *gwglDeleteContext)(HGLRC);
extern HGLRC (WINAPI *gwglGetCurrentContext)(VOID);
extern HDC (WINAPI *gwglGetCurrentDC)(VOID);
extern FARPROC (WINAPI *gwglGetProcAddress)(LPCSTR);
extern BOOL (WINAPI *gwglMakeCurrent)(HDC, HGLRC);
extern BOOL (WINAPI *gwglShareLists)(HGLRC, HGLRC);
extern BOOL (WINAPI *gwglUseFontBitmaps)(HDC, DWORD, DWORD, DWORD);
extern BOOL (WINAPI *gwglUseFontOutlines)(HDC, DWORD, DWORD, DWORD, FLOAT, FLOAT, int, LPGLYPHMETRICSFLOAT);
extern BOOL (WINAPI *gwglDescribeLayerPlane)(HDC, int, int, UINT, LPLAYERPLANEDESCRIPTOR);
extern int (WINAPI *gwglSetLayerPaletteEntries)(HDC, int, int, int, COLORREF *);
extern int (WINAPI *gwglGetLayerPaletteEntries)(HDC, int, int, int, COLORREF *);
extern BOOL (WINAPI *gwglRealizeLayerPalette)(HDC, int, BOOL);
extern BOOL (WINAPI *gwglSwapLayerBuffers)(HDC, UINT);

extern int (WINAPI *gwglChoosePixelFormat)(HDC, CONST PIXELFORMATDESCRIPTOR *);
extern int (WINAPI *gwglDescribePixelFormat) (HDC, int, UINT, LPPIXELFORMATDESCRIPTOR);
extern int (WINAPI *gwglGetPixelFormat)(HDC);
extern BOOL (WINAPI *gwglSetPixelFormat)(HDC, int, CONST PIXELFORMATDESCRIPTOR *);
extern BOOL (WINAPI *gwglSwapBuffers)(HDC);

//=====================================
//
// WGL EXTENSIONS
//
//=====================================

//macros not supported by wglext.h

//WGL_ARB_framebuffer_sRGB
#define WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB					0x20A9

//WGL_ATI_pixel_format_float
#define WGL_RGBA_FLOAT_MODE_ATI								0x8820
#define WGL_COLOR_CLEAR_UNCLAMPED_VALUE_ATI					0x8835
//#define WGL_TYPE_RGBA_FLOAT_ATI							0x21A0

//WGL_ATI_render_texture_rectangle
#define WGL_TEXTURE_RECTANGLE_ATI							0x21A5

//WGL_ARB_buffer_region
extern HANDLE (WINAPI *gwglCreateBufferRegionARB)(HDC hDC, int iLayerPlane, UINT uType);
extern VOID (WINAPI *gwglDeleteBufferRegionARB)(HANDLE hRegion);
extern BOOL (WINAPI *gwglSaveBufferRegionARB)(HANDLE hRegion, int x, int y, int width, int height);
extern BOOL (WINAPI *gwglRestoreBufferRegionARB)(HANDLE hRegion, int x, int y, int width, int height, int xSrc, int ySrc);

//WGL_ARB_extensions_string
extern const char *(WINAPI *gwglGetExtensionsStringARB)(HDC hdc);

//WGL_ARB_pixel_format
extern BOOL (WINAPI *gwglGetPixelFormatAttribivARB)(HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int *piAttributes, int *piValues);
extern BOOL (WINAPI *gwglGetPixelFormatAttribfvARB)(HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int *piAttributes, FLOAT *pfValues);
extern BOOL (WINAPI *gwglChoosePixelFormatARB)(HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);

//WGL_ARB_make_current_read
extern BOOL (WINAPI *gwglMakeContextCurrentARB)(HDC hDrawDC, HDC hReadDC, HGLRC hglrc);
extern HDC (WINAPI *gwglGetCurrentReadDCARB)(void);

//WGL_ARB_pbuffer
extern HPBUFFERARB (WINAPI *gwglCreatePbufferARB)(HDC hDC, int iPixelFormat, int iWidth, int iHeight, const int *piAttribList);
extern HDC (WINAPI *gwglGetPbufferDCARB)(HPBUFFERARB hPbuffer);
extern int (WINAPI *gwglReleasePbufferDCARB)(HPBUFFERARB hPbuffer, HDC hDC);
extern BOOL (WINAPI *gwglDestroyPbufferARB)(HPBUFFERARB hPbuffer);
extern BOOL (WINAPI *gwglQueryPbufferARB)(HPBUFFERARB hPbuffer, int iAttribute, int *piValue);

//WGL_ARB_render_texture
extern BOOL (WINAPI *gwglBindTexImageARB)(HPBUFFERARB hPbuffer, int iBuffer);
extern BOOL (WINAPI *gwglReleaseTexImageARB)(HPBUFFERARB hPbuffer, int iBuffer);
extern BOOL (WINAPI *gwglSetPbufferAttribARB)(HPBUFFERARB hPbuffer, const int *piAttribList);

//WGL_ARB_create_context
extern HGLRC (WINAPI *gwglCreateContextAttribsARB)(HDC hDC, HGLRC hShareContext, const int *attribList);

//WGL_EXT_display_color_table
extern GLboolean (WINAPI *gwglCreateDisplayColorTableEXT)(GLushort id);
extern GLboolean (WINAPI *gwglLoadDisplayColorTableEXT)(const GLushort *table, GLuint length);
extern GLboolean (WINAPI *gwglBindDisplayColorTableEXT)(GLushort id);
extern VOID (WINAPI *gwglDestroyDisplayColorTableEXT)(GLushort id);

//WGL_EXT_extensions_string
extern const char *(WINAPI *gwglGetExtensionsStringEXT)(void);

//WGL_EXT_make_current_read
extern BOOL (WINAPI *gwglMakeContextCurrentEXT)(HDC hDrawDC, HDC hReadDC, HGLRC hglrc);
extern HDC (WINAPI *gwglGetCurrentReadDCEXT)(void);

//WGL_EXT_pixel_format
extern BOOL (WINAPI *gwglGetPixelFormatAttribivEXT)(HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, int *piAttributes, int *piValues);
extern BOOL (WINAPI *gwglGetPixelFormatAttribfvEXT)(HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, int *piAttributes, FLOAT *pfValues);
extern BOOL (WINAPI *gwglChoosePixelFormatEXT)(HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);

//WGL_EXT_pbuffer
extern HPBUFFEREXT(WINAPI *gwglCreatePbufferEXT)(HDC hDC, int iPixelFormat, int iWidth, int iHeight, const int *piAttribList);
extern HDC(WINAPI *gwglGetPbufferDCEXT)(HPBUFFEREXT hPbuffer);
extern int(WINAPI *gwglReleasePbufferDCEXT)(HPBUFFEREXT hPbuffer, HDC hDC);
extern BOOL(WINAPI *gwglDestroyPbufferEXT)(HPBUFFEREXT hPbuffer);
extern BOOL(WINAPI *gwglQueryPbufferEXT)(HPBUFFEREXT hPbuffer, int iAttribute, int *piValue);

//WGL_EXT_swap_control
extern BOOL (WINAPI *gwglSwapIntervalEXT)(int interval);
extern int (WINAPI *gwglGetSwapIntervalEXT)(void);

//WGL_NV_vertex_array_range
extern void *(WINAPI *gwglAllocateMemoryNV)(GLsizei size, GLfloat readfreq, GLfloat writefreq, GLfloat priority);
extern void (WINAPI *gwglFreeMemoryNV)(void *pointer);

//WGL_EXT_multisample
extern void (WINAPI *gwglSampleMaskEXT)(GLclampf value, GLboolean invert);
extern void (WINAPI *gwglSamplePatternEXT)(GLenum pattern);

//WGL_OML_sync_control
extern BOOL (WINAPI *gwglGetSyncValuesOML)(HDC hdc, INT64 *ust, INT64 *msc, INT64 *sbc);
extern BOOL (WINAPI *gwglGetMscRateOML)(HDC hdc, INT32 *numerator, INT32 *denominator);
extern INT64 (WINAPI *gwglSwapBuffersMscOML)(HDC hdc, INT64 target_msc, INT64 divisor, INT64 remainder);
extern INT64 (WINAPI *gwglSwapLayerBuffersMscOML)(HDC hdc, int fuPlanes, INT64 target_msc, INT64 divisor, INT64 remainder);
extern BOOL (WINAPI *gwglWaitForMscOML)(HDC hdc, INT64 target_msc, INT64 divisor, INT64 remainder, INT64 *ust, INT64 *msc, INT64 *sbc);
extern BOOL (WINAPI *gwglWaitForSbcOML)(HDC hdc, INT64 target_sbc, INT64 *ust, INT64 *msc, INT64 *sbc);

//WGL_I3D_digital_video_control
extern BOOL (WINAPI *gwglGetDigitalVideoParametersI3D)(HDC hDC, int iAttribute, int *piValue);
extern BOOL (WINAPI *gwglSetDigitalVideoParametersI3D)(HDC hDC, int iAttribute, const int *piValue);

//WGL_I3D_gamma
extern BOOL (WINAPI *gwglGetGammaTableParametersI3D)(HDC hDC, int iAttribute, int *piValue);
extern BOOL (WINAPI *gwglSetGammaTableParametersI3D)(HDC hDC, int iAttribute, const int *piValue);
extern BOOL (WINAPI *gwglGetGammaTableI3D)(HDC hDC, int iEntries, USHORT *puRed, USHORT *puGreen, USHORT *puBlue);
extern BOOL (WINAPI *gwglSetGammaTableI3D)(HDC hDC, int iEntries, const USHORT *puRed, const USHORT *puGreen, const USHORT *puBlue);

//WGL_I3D_genlock
extern BOOL (WINAPI *gwglEnableGenlockI3D)(HDC hDC);
extern BOOL (WINAPI *gwglDisableGenlockI3D)(HDC hDC);
extern BOOL (WINAPI *gwglIsEnabledGenlockI3D)(HDC hDC, BOOL *pFlag);
extern BOOL (WINAPI *gwglGenlockSourceI3D)(HDC hDC, UINT uSource);
extern BOOL (WINAPI *gwglGetGenlockSourceI3D)(HDC hDC, UINT *uSource);
extern BOOL (WINAPI *gwglGenlockSourceEdgeI3D)(HDC hDC, UINT uEdge);
extern BOOL (WINAPI *gwglGetGenlockSourceEdgeI3D)(HDC hDC, UINT *uEdge);
extern BOOL (WINAPI *gwglGenlockSampleRateI3D)(HDC hDC, UINT uRate);
extern BOOL (WINAPI *gwglGetGenlockSampleRateI3D)(HDC hDC, UINT *uRate);
extern BOOL (WINAPI *gwglGenlockSourceDelayI3D)(HDC hDC, UINT uDelay);
extern BOOL (WINAPI *gwglGetGenlockSourceDelayI3D)(HDC hDC, UINT *uDelay);
extern BOOL (WINAPI *gwglQueryGenlockMaxSourceDelayI3D)(HDC hDC, UINT *uMaxLineDelay, UINT *uMaxPixelDelay);

//WGL_I3D_image_buffer
extern LPVOID (WINAPI *gwglCreateImageBufferI3D)(HDC hDC, DWORD dwSize, UINT uFlags);
extern BOOL (WINAPI *gwglDestroyImageBufferI3D)(HDC hDC, LPVOID pAddress);
extern BOOL (WINAPI *gwglAssociateImageBufferEventsI3D)(HDC hDC, const HANDLE *pEvent, const LPVOID *pAddress, const DWORD *pSize, UINT count);
extern BOOL (WINAPI *gwglReleaseImageBufferEventsI3D)(HDC hDC, const LPVOID *pAddress, UINT count);

//WGL_I3D_swap_frame_lock
extern BOOL (WINAPI *gwglEnableFrameLockI3D)(void);
extern BOOL (WINAPI *gwglDisableFrameLockI3D)(void);
extern BOOL (WINAPI *gwglIsEnabledFrameLockI3D)(BOOL *pFlag);
extern BOOL (WINAPI *gwglQueryFrameLockMasterI3D)(BOOL *pFlag);

//WGL_I3D_swap_frame_usage
extern BOOL (WINAPI *gwglGetFrameUsageI3D)(float *pUsage);
extern BOOL (WINAPI *gwglBeginFrameTrackingI3D)(void);
extern BOOL (WINAPI *gwglEndFrameTrackingI3D)(void);
extern BOOL (WINAPI *gwglQueryFrameTrackingI3D)(DWORD *pFrameCount, DWORD *pMissedFrames, float *pLastMissedUsage);

//WGL_3DL_stereo_control
extern BOOL (WINAPI *gwglSetStereoEmitterState3DL)(HDC hDC, UINT uState);

//WGL_NV_present_video
extern int (WINAPI *gwglEnumerateVideoDevicesNV)(HDC hDC, HVIDEOOUTPUTDEVICENV *phDeviceList);
extern BOOL (WINAPI *gwglBindVideoDeviceNV)(HDC hDC, unsigned int uVideoSlot, HVIDEOOUTPUTDEVICENV hVideoDevice, const int *piAttribList);
extern BOOL (WINAPI *gwglQueryCurrentContextNV)(int iAttribute, int *piValue);

//WGL_NV_video_out
extern BOOL (WINAPI *gwglGetVideoDeviceNV)(HDC hDC, int numDevices, HPVIDEODEV *hVideoDevice);
extern BOOL (WINAPI *gwglReleaseVideoDeviceNV)(HPVIDEODEV hVideoDevice);
extern BOOL (WINAPI *gwglBindVideoImageNV)(HPVIDEODEV hVideoDevice, HPBUFFERARB hPbuffer, int iVideoBuffer);
extern BOOL (WINAPI *gwglReleaseVideoImageNV)(HPBUFFERARB hPbuffer, int iVideoBuffer);
extern BOOL (WINAPI *gwglSendPbufferToVideoNV)(HPBUFFERARB hPbuffer, int iBufferType, unsigned long *pulCounterPbuffer, BOOL bBlock);
extern BOOL (WINAPI *gwglGetVideoInfoNV)(HPVIDEODEV hpVideoDevice, unsigned long *pulCounterOutputPbuffer, unsigned long *pulCounterOutputVideo);

//WGL_NV_swap_group
extern BOOL (WINAPI *gwglJoinSwapGroupNV)(HDC hDC, GLuint group);
extern BOOL (WINAPI *gwglBindSwapBarrierNV)(GLuint group, GLuint barrier);
extern BOOL (WINAPI *gwglQuerySwapGroupNV)(HDC hDC, GLuint *group, GLuint *barrier);
extern BOOL (WINAPI *gwglQueryMaxSwapGroupsNV)(HDC hDC, GLuint *maxGroups, GLuint *maxBarriers);
extern BOOL (WINAPI *gwglQueryFrameCountNV)(HDC hDC, GLuint *count);
extern BOOL (WINAPI *gwglResetFrameCountNV)(HDC hDC);

//WGL_NV_gpu_affinity
extern BOOL (WINAPI *gwglEnumGpusNV)(UINT iGpuIndex, HGPUNV *phGpu);
extern BOOL (WINAPI *gwglEnumGpuDevicesNV)(HGPUNV hGpu, UINT iDeviceIndex, PGPU_DEVICE lpGpuDevice);
extern HDC (WINAPI *gwglCreateAffinityDCNV)(const HGPUNV *phGpuList);
extern BOOL (WINAPI *gwglEnumGpusFromAffinityDCNV)(HDC hAffinityDC, UINT iGpuIndex, HGPUNV *hGpu);
extern BOOL (WINAPI *gwglDeleteDCNV)(HDC hdc);

//=====================================
//
// OPENGL EXTENSIONS
//
//=====================================

//typedefs and macros not supported by glext.h

typedef int GLfixed;
typedef int GLclampx;

//GL_EXT_fragment_lighting
#define GL_FRAGMENT_LIGHTING_EXT							0x8400
#define GL_FRAGMENT_COLOR_MATERIAL_EXT						0x8401
#define GL_FRAGMENT_COLOR_MATERIAL_FACE_EXT					0x8402
#define GL_FRAGMENT_COLOR_MATERIAL_PARAMETER_EXT			0x8403
#define GL_MAX_FRAGMENT_LIGHTS_EXT							0x8404
#define GL_MAX_ACTIVE_LIGHTS_EXT							0x8405
#define GL_CURRENT_RASTER_NORMAL_EXT						0x8406
#define GL_LIGHT_ENV_MODE_EXT								0x8407
#define GL_FRAGMENT_LIGHT_MODEL_LOCAL_VIEWER_EXT			0x8408
#define GL_FRAGMENT_LIGHT_MODEL_TWO_SIDE_EXT				0x8409
#define GL_FRAGMENT_LIGHT_MODEL_AMBIENT_EXT					0x840A
#define GL_FRAGMENT_LIGHT_MODEL_NORMAL_INTERPOLATION_EXT	0x840B
#define GL_FRAGMENT_LIGHT0_EXT								0x840C
#define GL_FRAGMENT_LIGHT1_EXT								0x840D
#define GL_FRAGMENT_LIGHT2_EXT								0x840E
#define GL_FRAGMENT_LIGHT3_EXT								0x840F
#define GL_FRAGMENT_LIGHT4_EXT								0x8410
#define GL_FRAGMENT_LIGHT5_EXT								0x8411
#define GL_FRAGMENT_LIGHT6_EXT								0x8412
#define GL_FRAGMENT_LIGHT7_EXT								0x8413

//GL_EXT_scene_marker
#define GL_SCENE_REQUIRED_EXT								0

//GL_SGIX_pixel_texture_bits
#define GL_COLOR_TO_TEXTURE_COORD_SGIX						0
#define GL_COLOR_BIT_PATTERN_SGIX							0
#define GL_COLOR_VALUE_SGIX									0

//GL_INTEL_texture_scissor
#define GL_TEXTURE_SCISSOR_INTEL							0
#define GL_TEXTURE_SCISSOR_S_INTEL							0
#define GL_TEXTURE_SCISSOR_T_INTEL							0
#define GL_TEXTURE_SCISSOR_R_INTEL							0

//GL_EXT_texture_env
#define GL_TEXTURE_ENV0_EXT									0
#define GL_TEXTURE_ENV1_EXT									0
#define GL_TEXTURE_ENV_MODE_ALPHA_EXT						0
#define GL_ENV_COPY_EXT										0
#define GL_ENV_REPLACE_EXT									0
#define GL_ENV_MODULATE_EXT									0
#define GL_ENV_ADD_EXT										0
#define GL_ENV_SUBTRACT_EXT									0
#define GL_ENV_REVERSE_SUBTRACT_EXT							0
#define GL_ENV_BLEND_EXT									0
#define GL_ENV_REVERSE_BLEND_EXT							0

//GL_SGIX_texture_range
#define GL_RGB_SIGNED_SGIX									0x85E0
#define GL_RGBA_SIGNED_SGIX									0x85E1
#define GL_ALPHA_SIGNED_SGIX								0x85E2
#define GL_LUMINANCE_SIGNED_SGIX							0x85E3
#define GL_INTENSITY_SIGNED_SGIX							0x85E4
#define GL_LUMINANCE_ALPHA_SIGNED_SGIX						0x85E5
#define GL_RGB16_SIGNED_SGIX								0x85E6
#define GL_RGBA16_SIGNED_SGIX								0x85E7
#define GL_ALPHA16_SIGNED_SGIX								0x85E8
#define GL_LUMINANCE16_SIGNED_SGIX							0x85E9
#define GL_INTENSITY16_SIGNED_SGIX							0x85EA
#define GL_LUMINANCE16_ALPHA16_SIGNED_SGIX					0x85EB
#define GL_RGB_EXTENDED_RANGE_SGIX							0x85EC
#define GL_RGBA_EXTENDED_RANGE_SGIX							0x85ED
#define GL_ALPHA_EXTENDED_RANGE_SGIX						0x85EE
#define GL_LUMINANCE_EXTENDED_RANGE_SGIX					0x85EF
#define GL_INTENSITY_EXTENDED_RANGE_SGIX					0x85F0
#define GL_LUMINANCE_ALPHA_EXTENDED_RANGE_SGIX				0x85F1
#define GL_RGB16_EXTENDED_RANGE_SGIX						0x85F2
#define GL_RGBA16_EXTENDED_RANGE_SGIX						0x85F3
#define GL_ALPHA16_EXTENDED_RANGE_SGIX						0x85F4
#define GL_LUMINANCE16_EXTENDED_RANGE_SGIX					0x85F5
#define GL_INTENSITY16_EXTENDED_RANGE_SGIX					0x85F6
#define GL_LUMINANCE16_ALPHA16_EXTENDED_RANGE_SGIX			0x85F7
#define GL_MIN_LUMINANCE_SGIS								0x85F8
#define GL_MAX_LUMINANCE_SGIS								0x85F9
#define GL_MIN_INTENSITY_SGIS								0x85FA
#define GL_MAX_INTENSITY_SGIS								0x85FB

//GL_IBM_static_data
#define GL_ALL_STATIC_DATA_IBM								103060
#define GL_STATIC_VERTEX_ARRAY_IBM							103061

//GL_OES_compressed_paletted_texture
#define GL_PALETTE4_RGB8_OES								0x8B90
#define GL_PALETTE4_RGBA8_OES								0x8B91
#define GL_PALETTE4_R5_G6_B5_OES							0x8B92
#define GL_PALETTE4_RGBA4_OES								0x8B93
#define GL_PALETTE4_RGB5_A1_OES								0x8B94
#define GL_PALETTE8_RGB8_OES								0x8B95
#define GL_PALETTE8_RGBA8_OES								0x8B96
#define GL_PALETTE8_R5_G6_B5_OES							0x8B97
#define GL_PALETTE8_RGBA4_OES								0x8B98
#define GL_PALETTE8_RGB5_A1_OES								0x8B99

//GL_APPLE_vertex_array_range
#define GL_MAX_VERTEX_ARRAY_RANGE_ELEMENT_APPLE				0x8520

//GL_KTX_buffer_region
#define GL_KTX_FRONT_REGION									0x0
#define GL_KTX_BACK_REGION									0x1
#define GL_KTX_Z_REGION										0x2
#define GL_KTX_STENCIL_REGION								0x3

//GL_EXT_Cg_shader
#define GL_CG_VERTEX_SHADER_EXT								0x890E
#define GL_CG_FRAGMENT_SHADER_EXT							0x890F

//GL_ATIX_texture_env_route
#define GL_SECONDARY_COLOR_ATIX								0x8747
#define GL_TEXTURE_OUTPUT_RGB_ATIX							0x8748
#define GL_TEXTURE_OUTPUT_ALPHA_ATIX						0x8749

//GL_ATIX_vertex_shader_output_point_size
#define GL_OUTPUT_POINT_SIZE_ATIX							0x610E

//GL_ATI_texture_compression_3dc
#define GL_COMPRESSED_RGB_3DC_ATI							0x8837

//GL_EXT_texture_rectangle
#define GL_TEXTURE_RECTANGLE_EXT							0x84F5
#define GL_TEXTURE_BINDING_RECTANGLE_EXT					0x84F6
#define GL_PROXY_TEXTURE_RECTANGLE_EXT						0x84F7
#define GL_MAX_RECTANGLE_TEXTURE_SIZE_EXT					0x84F8

//GL_APPLE_float_pixels
#define GL_HALF_APPLE										0x140B
#define GL_RGBA_FLOAT32_APPLE								0x8814
#define GL_RGB_FLOAT32_APPLE								0x8815
#define GL_ALPHA_FLOAT32_APPLE								0x8816
#define GL_INTENSITY_FLOAT32_APPLE							0x8817
#define GL_LUMINANCE_FLOAT32_APPLE							0x8818
#define GL_LUMINANCE_ALPHA_FLOAT32_APPLE					0x8819
#define GL_RGBA_FLOAT16_APPLE								0x881A
#define GL_RGB_FLOAT16_APPLE								0x881B
#define GL_ALPHA_FLOAT16_APPLE								0x881C
#define GL_INTENSITY_FLOAT16_APPLE							0x881D
#define GL_LUMINANCE_FLOAT16_APPLE							0x881E
#define GL_LUMINANCE_ALPHA_FLOAT16_APPLE					0x881F
#define GL_COLOR_FLOAT_APPLE								0x8A0F

//GL_APPLE_pixel_buffer
#define GL_MIN_PBUFFER_VIEWPORT_DIMS_APPLE					0x8A10

//GL_APPLE_texture_range
#define GL_TEXTURE_RANGE_LENGTH_APPLE						0x85B7
#define GL_TEXTURE_RANGE_POINTER_APPLE						0x85B8
#define GL_TEXTURE_STORAGE_HINT_APPLE						0x85BC
#define GL_STORAGE_PRIVATE_APPLE							0x85BD
#define GL_STORAGE_CACHED_APPLE								0x85BE
#define GL_STORAGE_SHARED_APPLE								0x85BF

//GL_OES_fixed_point
#define GL_FIXED_OES										0x140C

//GL_ARB_multitexture
extern void (APIENTRY *gglActiveTextureARB)(GLenum texture);
extern void (APIENTRY *gglClientActiveTextureARB)(GLenum texture);
extern void (APIENTRY *gglMultiTexCoord1dARB)(GLenum target, GLdouble s);
extern void (APIENTRY *gglMultiTexCoord1dvARB)(GLenum target, const GLdouble *v);
extern void (APIENTRY *gglMultiTexCoord1fARB)(GLenum target, GLfloat s);
extern void (APIENTRY *gglMultiTexCoord1fvARB)(GLenum target, const GLfloat *v);
extern void (APIENTRY *gglMultiTexCoord1iARB)(GLenum target, GLint s);
extern void (APIENTRY *gglMultiTexCoord1ivARB)(GLenum target, const GLint *v);
extern void (APIENTRY *gglMultiTexCoord1sARB)(GLenum target, GLshort s);
extern void (APIENTRY *gglMultiTexCoord1svARB)(GLenum target, const GLshort *v);
extern void (APIENTRY *gglMultiTexCoord2dARB)(GLenum target, GLdouble s, GLdouble t);
extern void (APIENTRY *gglMultiTexCoord2dvARB)(GLenum target, const GLdouble *v);
extern void (APIENTRY *gglMultiTexCoord2fARB)(GLenum target, GLfloat s, GLfloat t);
extern void (APIENTRY *gglMultiTexCoord2fvARB)(GLenum target, const GLfloat *v);
extern void (APIENTRY *gglMultiTexCoord2iARB)(GLenum target, GLint s, GLint t);
extern void (APIENTRY *gglMultiTexCoord2ivARB)(GLenum target, const GLint *v);
extern void (APIENTRY *gglMultiTexCoord2sARB)(GLenum target, GLshort s, GLshort t);
extern void (APIENTRY *gglMultiTexCoord2svARB)(GLenum target, const GLshort *v);
extern void (APIENTRY *gglMultiTexCoord3dARB)(GLenum target, GLdouble s, GLdouble t, GLdouble r);
extern void (APIENTRY *gglMultiTexCoord3dvARB)(GLenum target, const GLdouble *v);
extern void (APIENTRY *gglMultiTexCoord3fARB)(GLenum target, GLfloat s, GLfloat t, GLfloat r);
extern void (APIENTRY *gglMultiTexCoord3fvARB)(GLenum target, const GLfloat *v);
extern void (APIENTRY *gglMultiTexCoord3iARB)(GLenum target, GLint s, GLint t, GLint r);
extern void (APIENTRY *gglMultiTexCoord3ivARB)(GLenum target, const GLint *v);
extern void (APIENTRY *gglMultiTexCoord3sARB)(GLenum target, GLshort s, GLshort t, GLshort r);
extern void (APIENTRY *gglMultiTexCoord3svARB)(GLenum target, const GLshort *v);
extern void (APIENTRY *gglMultiTexCoord4dARB)(GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q);
extern void (APIENTRY *gglMultiTexCoord4dvARB)(GLenum target, const GLdouble *v);
extern void (APIENTRY *gglMultiTexCoord4fARB)(GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
extern void (APIENTRY *gglMultiTexCoord4fvARB)(GLenum target, const GLfloat *v);
extern void (APIENTRY *gglMultiTexCoord4iARB)(GLenum target, GLint s, GLint t, GLint r, GLint q);
extern void (APIENTRY *gglMultiTexCoord4ivARB)(GLenum target, const GLint *v);
extern void (APIENTRY *gglMultiTexCoord4sARB)(GLenum target, GLshort s, GLshort t, GLshort r, GLshort q);
extern void (APIENTRY *gglMultiTexCoord4svARB)(GLenum target, const GLshort *v);

//GL_ARB_transpose_matrix
extern void (APIENTRY *gglLoadTransposeMatrixfARB)(const GLfloat *m);
extern void (APIENTRY *gglLoadTransposeMatrixdARB)(const GLdouble *m);
extern void (APIENTRY *gglMultTransposeMatrixfARB)(const GLfloat *m);
extern void (APIENTRY *gglMultTransposeMatrixdARB)(const GLdouble *m);

//GL_ARB_multisample
extern void (APIENTRY *gglSampleCoverageARB)(GLclampf value, GLboolean invert);

//GL_ARB_texture_compression
extern void (APIENTRY *gglCompressedTexImage3DARB)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid *data);
extern void (APIENTRY *gglCompressedTexImage2DARB)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data);
extern void (APIENTRY *gglCompressedTexImage1DARB)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid *data);
extern void (APIENTRY *gglCompressedTexSubImage3DARB)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid *data);
extern void (APIENTRY *gglCompressedTexSubImage2DARB)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data);
extern void (APIENTRY *gglCompressedTexSubImage1DARB)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid *data);
extern void (APIENTRY *gglGetCompressedTexImageARB)(GLenum target, GLint level, void *img);

//GL_ARB_point_parameters
extern void (APIENTRY *gglPointParameterfARB)(GLenum pname, GLfloat param);
extern void (APIENTRY *gglPointParameterfvARB)(GLenum pname, const GLfloat *params);

//GL_ARB_vertex_blend
extern void (APIENTRY *gglWeightbvARB)(GLint size, const GLbyte *weights);
extern void (APIENTRY *gglWeightsvARB)(GLint size, const GLshort *weights);
extern void (APIENTRY *gglWeightivARB)(GLint size, const GLint *weights);
extern void (APIENTRY *gglWeightfvARB)(GLint size, const GLfloat *weights);
extern void (APIENTRY *gglWeightdvARB)(GLint size, const GLdouble *weights);
extern void (APIENTRY *gglWeightubvARB)(GLint size, const GLubyte *weights);
extern void (APIENTRY *gglWeightusvARB)(GLint size, const GLushort *weights);
extern void (APIENTRY *gglWeightuivARB)(GLint size, const GLuint *weights);
extern void (APIENTRY *gglWeightPointerARB)(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
extern void (APIENTRY *gglVertexBlendARB)(GLint count);

//GL_ARB_matrix_palette
extern void (APIENTRY *gglCurrentPaletteMatrixARB)(GLint index);
extern void (APIENTRY *gglMatrixIndexubvARB)(GLint size, const GLubyte *indices);
extern void (APIENTRY *gglMatrixIndexusvARB)(GLint size, const GLushort *indices);
extern void (APIENTRY *gglMatrixIndexuivARB)(GLint size, const GLuint *indices);
extern void (APIENTRY *gglMatrixIndexPointerARB)(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);

//GL_ARB_window_pos
extern void (APIENTRY *gglWindowPos2dARB)(GLdouble x, GLdouble y);
extern void (APIENTRY *gglWindowPos2dvARB)(const GLdouble *v);
extern void (APIENTRY *gglWindowPos2fARB)(GLfloat x, GLfloat y);
extern void (APIENTRY *gglWindowPos2fvARB)(const GLfloat *v);
extern void (APIENTRY *gglWindowPos2iARB)(GLint x, GLint y);
extern void (APIENTRY *gglWindowPos2ivARB)(const GLint *v);
extern void (APIENTRY *gglWindowPos2sARB)(GLshort x, GLshort y);
extern void (APIENTRY *gglWindowPos2svARB)(const GLshort *v);
extern void (APIENTRY *gglWindowPos3dARB)(GLdouble x, GLdouble y, GLdouble z);
extern void (APIENTRY *gglWindowPos3dvARB)(const GLdouble *v);
extern void (APIENTRY *gglWindowPos3fARB)(GLfloat x, GLfloat y, GLfloat z);
extern void (APIENTRY *gglWindowPos3fvARB)(const GLfloat *v);
extern void (APIENTRY *gglWindowPos3iARB)(GLint x, GLint y, GLint z);
extern void (APIENTRY *gglWindowPos3ivARB)(const GLint *v);
extern void (APIENTRY *gglWindowPos3sARB)(GLshort x, GLshort y, GLshort z);
extern void (APIENTRY *gglWindowPos3svARB)(const GLshort *v);

//GL_ARB_vertex_program
extern void (APIENTRY *gglVertexAttrib1sARB)(GLuint index, GLshort x);
extern void (APIENTRY *gglVertexAttrib1fARB)(GLuint index, GLfloat x);
extern void (APIENTRY *gglVertexAttrib1dARB)(GLuint index, GLdouble x);
extern void (APIENTRY *gglVertexAttrib2sARB)(GLuint index, GLshort x, GLshort y);
extern void (APIENTRY *gglVertexAttrib2fARB)(GLuint index, GLfloat x, GLfloat y);
extern void (APIENTRY *gglVertexAttrib2dARB)(GLuint index, GLdouble x, GLdouble y);
extern void (APIENTRY *gglVertexAttrib3sARB)(GLuint index, GLshort x, GLshort y, GLshort z);
extern void (APIENTRY *gglVertexAttrib3fARB)(GLuint index, GLfloat x, GLfloat y, GLfloat z);
extern void (APIENTRY *gglVertexAttrib3dARB)(GLuint index, GLdouble x, GLdouble y, GLdouble z);
extern void (APIENTRY *gglVertexAttrib4sARB)(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
extern void (APIENTRY *gglVertexAttrib4fARB)(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern void (APIENTRY *gglVertexAttrib4dARB)(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern void (APIENTRY *gglVertexAttrib4NubARB)(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
extern void (APIENTRY *gglVertexAttrib1svARB)(GLuint index, const GLshort *v);
extern void (APIENTRY *gglVertexAttrib1fvARB)(GLuint index, const GLfloat *v);
extern void (APIENTRY *gglVertexAttrib1dvARB)(GLuint index, const GLdouble *v);
extern void (APIENTRY *gglVertexAttrib2svARB)(GLuint index, const GLshort *v);
extern void (APIENTRY *gglVertexAttrib2fvARB)(GLuint index, const GLfloat *v);
extern void (APIENTRY *gglVertexAttrib2dvARB)(GLuint index, const GLdouble *v);
extern void (APIENTRY *gglVertexAttrib3svARB)(GLuint index, const GLshort *v);
extern void (APIENTRY *gglVertexAttrib3fvARB)(GLuint index, const GLfloat *v);
extern void (APIENTRY *gglVertexAttrib3dvARB)(GLuint index, const GLdouble *v);
extern void (APIENTRY *gglVertexAttrib4bvARB)(GLuint index, const GLbyte *v);
extern void (APIENTRY *gglVertexAttrib4svARB)(GLuint index, const GLshort *v);
extern void (APIENTRY *gglVertexAttrib4ivARB)(GLuint index, const GLint *v);
extern void (APIENTRY *gglVertexAttrib4ubvARB)(GLuint index, const GLubyte *v);
extern void (APIENTRY *gglVertexAttrib4usvARB)(GLuint index, const GLushort *v);
extern void (APIENTRY *gglVertexAttrib4uivARB)(GLuint index, const GLuint *v);
extern void (APIENTRY *gglVertexAttrib4fvARB)(GLuint index, const GLfloat *v);
extern void (APIENTRY *gglVertexAttrib4dvARB)(GLuint index, const GLdouble *v);
extern void (APIENTRY *gglVertexAttrib4NbvARB)(GLuint index, const GLbyte *v);
extern void (APIENTRY *gglVertexAttrib4NsvARB)(GLuint index, const GLshort *v);
extern void (APIENTRY *gglVertexAttrib4NivARB)(GLuint index, const GLint *v);
extern void (APIENTRY *gglVertexAttrib4NubvARB)(GLuint index, const GLubyte *v);
extern void (APIENTRY *gglVertexAttrib4NusvARB)(GLuint index, const GLushort *v);
extern void (APIENTRY *gglVertexAttrib4NuivARB)(GLuint index, const GLuint *v);
extern void (APIENTRY *gglVertexAttribPointerARB)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
extern void (APIENTRY *gglEnableVertexAttribArrayARB)(GLuint index);
extern void (APIENTRY *gglDisableVertexAttribArrayARB)(GLuint index);
extern void (APIENTRY *gglProgramStringARB)(GLenum target, GLenum format, GLsizei len, const GLvoid *string);
extern void (APIENTRY *gglBindProgramARB)(GLenum target, GLuint program);
extern void (APIENTRY *gglDeleteProgramsARB)(GLsizei n, const GLuint *programs);
extern void (APIENTRY *gglGenProgramsARB)(GLsizei n, GLuint *programs);
extern void (APIENTRY *gglProgramEnvParameter4dARB)(GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern void (APIENTRY *gglProgramEnvParameter4dvARB)(GLenum target, GLuint index, const GLdouble *params);
extern void (APIENTRY *gglProgramEnvParameter4fARB)(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern void (APIENTRY *gglProgramEnvParameter4fvARB)(GLenum target, GLuint index, const GLfloat *params);
extern void (APIENTRY *gglProgramLocalParameter4dARB)(GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern void (APIENTRY *gglProgramLocalParameter4dvARB)(GLenum target, GLuint index, const GLdouble *params);
extern void (APIENTRY *gglProgramLocalParameter4fARB)(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern void (APIENTRY *gglProgramLocalParameter4fvARB)(GLenum target, GLuint index, const GLfloat *params);
extern void (APIENTRY *gglGetProgramEnvParameterdvARB)(GLenum target, GLuint index, GLdouble *params);
extern void (APIENTRY *gglGetProgramEnvParameterfvARB)(GLenum target, GLuint index, GLfloat *params);
extern void (APIENTRY *gglGetProgramLocalParameterdvARB)(GLenum target, GLuint index, GLdouble *params);
extern void (APIENTRY *gglGetProgramLocalParameterfvARB)(GLenum target, GLuint index, GLfloat *params);
extern void (APIENTRY *gglGetProgramivARB)(GLenum target, GLenum pname, GLint *params);
extern void (APIENTRY *gglGetProgramStringARB)(GLenum target, GLenum pname, GLvoid *string);
extern void (APIENTRY *gglGetVertexAttribdvARB)(GLuint index, GLenum pname, GLdouble *params);
extern void (APIENTRY *gglGetVertexAttribfvARB)(GLuint index, GLenum pname, GLfloat *params);
extern void (APIENTRY *gglGetVertexAttribivARB)(GLuint index, GLenum pname, GLint *params);
extern void (APIENTRY *gglGetVertexAttribPointervARB)(GLuint index, GLenum pname, GLvoid **pointer);
extern GLboolean (APIENTRY *gglIsProgramARB)(GLuint program);

//GL_ARB_fragment_program
extern void (APIENTRY *gglProgramStringARB)(GLenum target, GLenum format, GLsizei len, const GLvoid *string);
extern void (APIENTRY *gglBindProgramARB)(GLenum target, GLuint program);
extern void (APIENTRY *gglDeleteProgramsARB)(GLsizei n, const GLuint *programs);
extern void (APIENTRY *gglGenProgramsARB)(GLsizei n, GLuint *programs);
extern void (APIENTRY *gglProgramEnvParameter4dARB)(GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern void (APIENTRY *gglProgramEnvParameter4dvARB)(GLenum target, GLuint index, const GLdouble *params);
extern void (APIENTRY *gglProgramEnvParameter4fARB)(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern void (APIENTRY *gglProgramEnvParameter4fvARB)(GLenum target, GLuint index, const GLfloat *params);
extern void (APIENTRY *gglProgramLocalParameter4dARB)(GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern void (APIENTRY *gglProgramLocalParameter4dvARB)(GLenum target, GLuint index, const GLdouble *params);
extern void (APIENTRY *gglProgramLocalParameter4fARB)(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern void (APIENTRY *gglProgramLocalParameter4fvARB)(GLenum target, GLuint index, const GLfloat *params);
extern void (APIENTRY *gglGetProgramEnvParameterdvARB)(GLenum target, GLuint index, GLdouble *params);
extern void (APIENTRY *gglGetProgramEnvParameterfvARB)(GLenum target, GLuint index, GLfloat *params);
extern void (APIENTRY *gglGetProgramLocalParameterdvARB)(GLenum target, GLuint index, GLdouble *params);
extern void (APIENTRY *gglGetProgramLocalParameterfvARB)(GLenum target, GLuint index, GLfloat *params);
extern void (APIENTRY *gglGetProgramivARB)(GLenum target, GLenum pname, GLint *params);
extern void (APIENTRY *gglGetProgramStringARB)(GLenum target, GLenum pname, GLvoid *string);
extern GLboolean (APIENTRY *gglIsProgramARB)(GLuint program);

//GL_ARB_vertex_buffer_object
extern void (APIENTRY *gglBindBufferARB)(GLenum target, GLuint buffer);
extern void (APIENTRY *gglDeleteBuffersARB)(GLsizei n, const GLuint *buffers);
extern void (APIENTRY *gglGenBuffersARB)(GLsizei n, GLuint *buffers);
extern GLboolean (APIENTRY *gglIsBufferARB)(GLuint buffer);
extern void (APIENTRY *gglBufferDataARB)(GLenum target, GLsizeiptrARB size, const GLvoid *data, GLenum usage);
extern void (APIENTRY *gglBufferSubDataARB)(GLenum target, GLintptrARB offset, GLsizeiptrARB size, const GLvoid *data);
extern void (APIENTRY *gglGetBufferSubDataARB)(GLenum target, GLintptrARB offset, GLsizeiptrARB size, GLvoid *data);
extern void *(APIENTRY *gglMapBufferARB)(GLenum target, GLenum access);
extern GLboolean (APIENTRY *gglUnmapBufferARB)(GLenum target);
extern void (APIENTRY *gglGetBufferParameterivARB)(GLenum target, GLenum pname, GLint *params);
extern void (APIENTRY *gglGetBufferPointervARB)(GLenum target, GLenum pname, GLvoid **params);

//GL_ARB_occlusion_query
extern void (APIENTRY *gglGenQueriesARB)(GLsizei n, GLuint *ids);
extern void (APIENTRY *gglDeleteQueriesARB)(GLsizei n, const GLuint *ids);
extern GLboolean (APIENTRY *gglIsQueryARB)(GLuint id);
extern void (APIENTRY *gglBeginQueryARB)(GLenum target, GLuint id);
extern void (APIENTRY *gglEndQueryARB)(GLenum target);
extern void (APIENTRY *gglGetQueryivARB)(GLenum target, GLenum pname, GLint *params);
extern void (APIENTRY *gglGetQueryObjectivARB)(GLuint id, GLenum pname, GLint *params);
extern void (APIENTRY *gglGetQueryObjectuivARB)(GLuint id, GLenum pname, GLuint *params);

//GL_ARB_shader_objects
extern void (APIENTRY *gglDeleteObjectARB)(GLhandleARB obj);
extern GLhandleARB (APIENTRY *gglGetHandleARB)(GLenum pname);
extern void (APIENTRY *gglDetachObjectARB)(GLhandleARB containerObj, GLhandleARB attachedObj);
extern GLhandleARB (APIENTRY *gglCreateShaderObjectARB)(GLenum shaderType);
extern void (APIENTRY *gglShaderSourceARB)(GLhandleARB shaderObj, GLsizei count, const GLcharARB* *string, const GLint *length);
extern void (APIENTRY *gglCompileShaderARB)(GLhandleARB shaderObj);
extern GLhandleARB (APIENTRY *gglCreateProgramObjectARB)(void);
extern void (APIENTRY *gglAttachObjectARB)(GLhandleARB containerObj, GLhandleARB obj);
extern void (APIENTRY *gglLinkProgramARB)(GLhandleARB programObj);
extern void (APIENTRY *gglUseProgramObjectARB)(GLhandleARB programObj);
extern void (APIENTRY *gglValidateProgramARB)(GLhandleARB programObj);
extern void (APIENTRY *gglUniform1fARB)(GLint location, GLfloat v0);
extern void (APIENTRY *gglUniform2fARB)(GLint location, GLfloat v0, GLfloat v1);
extern void (APIENTRY *gglUniform3fARB)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
extern void (APIENTRY *gglUniform4fARB)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
extern void (APIENTRY *gglUniform1iARB)(GLint location, GLint v0);
extern void (APIENTRY *gglUniform2iARB)(GLint location, GLint v0, GLint v1);
extern void (APIENTRY *gglUniform3iARB)(GLint location, GLint v0, GLint v1, GLint v2);
extern void (APIENTRY *gglUniform4iARB)(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
extern void (APIENTRY *gglUniform1fvARB)(GLint location, GLsizei count, const GLfloat *value);
extern void (APIENTRY *gglUniform2fvARB)(GLint location, GLsizei count, const GLfloat *value);
extern void (APIENTRY *gglUniform3fvARB)(GLint location, GLsizei count, const GLfloat *value);
extern void (APIENTRY *gglUniform4fvARB)(GLint location, GLsizei count, const GLfloat *value);
extern void (APIENTRY *gglUniform1ivARB)(GLint location, GLsizei count, const GLint *value);
extern void (APIENTRY *gglUniform2ivARB)(GLint location, GLsizei count, const GLint *value);
extern void (APIENTRY *gglUniform3ivARB)(GLint location, GLsizei count, const GLint *value);
extern void (APIENTRY *gglUniform4ivARB)(GLint location, GLsizei count, const GLint *value);
extern void (APIENTRY *gglUniformMatrix2fvARB)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void (APIENTRY *gglUniformMatrix3fvARB)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void (APIENTRY *gglUniformMatrix4fvARB)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void (APIENTRY *gglGetObjectParameterfvARB)(GLhandleARB obj, GLenum pname, GLfloat *params);
extern void (APIENTRY *gglGetObjectParameterivARB)(GLhandleARB obj, GLenum pname, GLint *params);
extern void (APIENTRY *gglGetInfoLogARB)(GLhandleARB obj, GLsizei maxLength, GLsizei *length, GLcharARB *infoLog);
extern void (APIENTRY *gglGetAttachedObjectsARB)(GLhandleARB containerObj, GLsizei maxCount, GLsizei *count, GLhandleARB *obj);
extern GLint (APIENTRY *gglGetUniformLocationARB)(GLhandleARB programObj, const GLcharARB *name);
extern void (APIENTRY *gglGetActiveUniformARB)(GLhandleARB programObj, GLuint index, GLsizei maxLength, GLsizei *length, GLint *size, GLenum *type, GLcharARB *name);
extern void (APIENTRY *gglGetUniformfvARB)(GLhandleARB programObj, GLint location, GLfloat *params);
extern void (APIENTRY *gglGetUniformivARB)(GLhandleARB programObj, GLint location, GLint *params);
extern void (APIENTRY *gglGetShaderSourceARB)(GLhandleARB obj, GLsizei maxLength, GLsizei *length, GLcharARB *source);

//GL_ARB_vertex_shader
extern void (APIENTRY *gglBindAttribLocationARB)(GLhandleARB programObj, GLuint index, const GLcharARB *name);
extern void (APIENTRY *gglGetActiveAttribARB)(GLhandleARB programObj, GLuint index, GLsizei maxLength, GLsizei *length, GLint *size, GLenum *type, GLcharARB *name);
extern GLint (APIENTRY *gglGetAttribLocationARB)(GLhandleARB programObj, const GLcharARB *name);

//GL_ARB_draw_buffers
extern void (APIENTRY *gglDrawBuffersARB)(GLsizei n, const GLenum *bufs);

//GL_ARB_color_buffer_float
extern void (APIENTRY *gglClampColorARB)(GLenum target, GLenum clamp);

//GL_ARB_draw_instanced
extern void (APIENTRY *gglDrawArraysInstancedARB)(GLenum mode, GLint first, GLsizei count, GLsizei primcount);
extern void (APIENTRY *gglDrawElementsInstancedARB)(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei primcount);

//GL_ARB_framebuffer_object
//part of OpenGL 3.0, defined as a separate extension for older hardware that can't support 3.0
extern GLboolean (APIENTRY *gglIsRenderbuffer)(GLuint renderbuffer);
extern void (APIENTRY *gglBindRenderbuffer)(GLenum target, GLuint renderbuffer);
extern void (APIENTRY *gglDeleteRenderbuffers)(GLsizei n, const GLuint *renderbuffers);
extern void (APIENTRY *gglGenRenderbuffers)(GLsizei n, GLuint *renderbuffers);
extern void (APIENTRY *gglRenderbufferStorage)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
extern void (APIENTRY *gglGetRenderbufferParameteriv)(GLenum target, GLenum pname, GLint *params);
extern GLboolean (APIENTRY *gglIsFramebuffer)(GLuint framebuffer);
extern void (APIENTRY *gglBindFramebuffer)(GLenum target, GLuint framebuffer);
extern void (APIENTRY *gglDeleteFramebuffers)(GLsizei n, const GLuint *framebuffers);
extern void (APIENTRY *gglGenFramebuffers)(GLsizei n, GLuint *framebuffers);
extern GLenum (APIENTRY *gglCheckFramebufferStatus)(GLenum target);
extern void (APIENTRY *gglFramebufferTexture1D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
extern void (APIENTRY *gglFramebufferTexture2D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
extern void (APIENTRY *gglFramebufferTexture3D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
extern void (APIENTRY *gglFramebufferRenderbuffer)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
extern void (APIENTRY *gglGetFramebufferAttachmentParameteriv)(GLenum target, GLenum attachment, GLenum pname, GLint *params);
extern void (APIENTRY *gglGenerateMipmap)(GLenum target);
extern void (APIENTRY *gglBlitFramebuffer)(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
extern void (APIENTRY *gglRenderbufferStorageMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
extern void (APIENTRY *gglFramebufferTextureLayer)(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);

//GL_ARB_geometry_shader4
extern void (APIENTRY *gglProgramParameteriARB)(GLuint program, GLenum pname, GLint value);
extern void (APIENTRY *gglFramebufferTextureARB)(GLenum target, GLenum attachment, GLuint texture, GLint level);
extern void (APIENTRY *gglFramebufferTextureLayerARB)(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
extern void (APIENTRY *gglFramebufferTextureFaceARB)(GLenum target, GLenum attachment, GLuint texture, GLint level, GLenum face);

//GL_ARB_instanced_arrays
//part of OpenGL 3.3, defined as a separate extension for older hardware that can't support 3.3
extern void (APIENTRYP *gglVertexAttribDivisor)(GLuint index, GLuint divisor);

//GL_ARB_map_buffer_range
//part of OpenGL 3.0, defined as a separate extension for older hardware that can't support 3.0
extern void (APIENTRY *gglMapBufferRange)(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
extern void (APIENTRY *gglFlushMappedBufferRange)(GLenum target, GLintptr offset, GLsizeiptr length);

//GL_ARB_texture_buffer_object
extern void (APIENTRY *gglTexBufferARB)(GLenum target, GLenum internalformat, GLuint buffer);

//GL_ARB_vertex_array_object
//part of OpenGL 3.0, defined as a separate extension for older hardware that can't support 3.0
extern void (APIENTRY *gglBindVertexArray)(GLuint array);
extern void (APIENTRY *gglDeleteVertexArrays)(GLsizei n, const GLuint *arrays);
extern void (APIENTRY *gglGenVertexArrays)(GLsizei n, GLuint *arrays);
extern GLboolean (APIENTRY *gglIsVertexArray)(GLuint array);

//GL_EXT_blend_color
extern void (APIENTRY *gglBlendColorEXT)(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);

//GL_EXT_polygon_offset
extern void (APIENTRY *gglPolygonOffsetEXT)(GLfloat factor, GLfloat bias);

//GL_EXT_texture3D
extern void (APIENTRY *gglTexImage3DEXT)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
extern void (APIENTRY *gglTexSubImage3DEXT)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels);

//GL_SGIS_texture_filter4
extern void (APIENTRY *gglGetTexFilterFuncSGIS)(GLenum target, GLenum filter, GLfloat *weights);
extern void (APIENTRY *gglTexFilterFuncSGIS)(GLenum target, GLenum filter, GLsizei n, const GLfloat *weights);

//GL_EXT_subtexture
extern void (APIENTRY *gglTexSubImage1DEXT)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels);
extern void (APIENTRY *gglTexSubImage2DEXT)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);

//GL_EXT_copy_texture
extern void (APIENTRY *gglCopyTexImage1DEXT)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
extern void (APIENTRY *gglCopyTexImage2DEXT)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
extern void (APIENTRY *gglCopyTexSubImage1DEXT)(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
extern void (APIENTRY *gglCopyTexSubImage2DEXT)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
extern void (APIENTRY *gglCopyTexSubImage3DEXT)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);

//GL_EXT_histogram
extern void (APIENTRY *gglGetHistogramEXT)(GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid *values);
extern void (APIENTRY *gglGetHistogramParameterfvEXT)(GLenum target, GLenum pname, GLfloat *params);
extern void (APIENTRY *gglGetHistogramParameterivEXT)(GLenum target, GLenum pname, GLint *params);
extern void (APIENTRY *gglGetMinmaxEXT)(GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid *values);
extern void (APIENTRY *gglGetMinmaxParameterfvEXT)(GLenum target, GLenum pname, GLfloat *params);
extern void (APIENTRY *gglGetMinmaxParameterivEXT)(GLenum target, GLenum pname, GLint *params);
extern void (APIENTRY *gglHistogramEXT)(GLenum target, GLsizei width, GLenum internalformat, GLboolean sink);
extern void (APIENTRY *gglMinmaxEXT)(GLenum target, GLenum internalformat, GLboolean sink);
extern void (APIENTRY *gglResetHistogramEXT)(GLenum target);
extern void (APIENTRY *gglResetMinmaxEXT)(GLenum target);

//GL_EXT_convolution
extern void (APIENTRY *gglConvolutionFilter1DEXT)(GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *image);
extern void (APIENTRY *gglConvolutionFilter2DEXT)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *image);
extern void (APIENTRY *gglConvolutionParameterfEXT)(GLenum target, GLenum pname, GLfloat params);
extern void (APIENTRY *gglConvolutionParameterfvEXT)(GLenum target, GLenum pname, const GLfloat *params);
extern void (APIENTRY *gglConvolutionParameteriEXT)(GLenum target, GLenum pname, GLint params);
extern void (APIENTRY *gglConvolutionParameterivEXT)(GLenum target, GLenum pname, const GLint *params);
extern void (APIENTRY *gglCopyConvolutionFilter1DEXT)(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
extern void (APIENTRY *gglCopyConvolutionFilter2DEXT)(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height);
extern void (APIENTRY *gglGetConvolutionFilterEXT)(GLenum target, GLenum format, GLenum type, GLvoid *image);
extern void (APIENTRY *gglGetConvolutionParameterfvEXT)(GLenum target, GLenum pname, GLfloat *params);
extern void (APIENTRY *gglGetConvolutionParameterivEXT)(GLenum target, GLenum pname, GLint *params);
extern void (APIENTRY *gglGetSeparableFilterEXT)(GLenum target, GLenum format, GLenum type, GLvoid *row, GLvoid *column, GLvoid *span);
extern void (APIENTRY *gglSeparableFilter2DEXT)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *row, const GLvoid *column);

//GL_SGI_color_table
extern void (APIENTRY *gglColorTableSGI)(GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *table);
extern void (APIENTRY *gglColorTableParameterfvSGI)(GLenum target, GLenum pname, const GLfloat *params);
extern void (APIENTRY *gglColorTableParameterivSGI)(GLenum target, GLenum pname, const GLint *params);
extern void (APIENTRY *gglCopyColorTableSGI)(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
extern void (APIENTRY *gglGetColorTableSGI)(GLenum target, GLenum format, GLenum type, GLvoid *table);
extern void (APIENTRY *gglGetColorTableParameterfvSGI)(GLenum target, GLenum pname, GLfloat *params);
extern void (APIENTRY *gglGetColorTableParameterivSGI)(GLenum target, GLenum pname, GLint *params);

//GL_SGIX_pixel_texture
extern void (APIENTRY *gglPixelTexGenSGIX)(GLenum mode);

//GL_SGIS_pixel_texture
extern void (APIENTRY *gglPixelTexGenParameteriSGIS)(GLenum pname, GLint param);
extern void (APIENTRY *gglPixelTexGenParameterivSGIS)(GLenum pname, const GLint *params);
extern void (APIENTRY *gglPixelTexGenParameterfSGIS)(GLenum pname, GLfloat param);
extern void (APIENTRY *gglPixelTexGenParameterfvSGIS)(GLenum pname, const GLfloat *params);
extern void (APIENTRY *gglGetPixelTexGenParameterivSGIS)(GLenum pname, GLint *params);
extern void (APIENTRY *gglGetPixelTexGenParameterfvSGIS)(GLenum pname, GLfloat *params);

//GL_SGIS_texture4D
extern void (APIENTRY *gglTexImage4DSGIS)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLsizei size4d, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
extern void (APIENTRY *gglTexSubImage4DSGIS)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint woffset, GLsizei width, GLsizei height, GLsizei depth, GLsizei size4d, GLenum format, GLenum type, const GLvoid *pixels);

//GL_EXT_texture_object
extern GLboolean (APIENTRY *gglAreTexturesResidentEXT)(GLsizei n, const GLuint *textures, GLboolean *residences);
extern void (APIENTRY *gglBindTextureEXT)(GLenum target, GLuint texture);
extern void (APIENTRY *gglDeleteTexturesEXT)(GLsizei n, const GLuint *textures);
extern void (APIENTRY *gglGenTexturesEXT)(GLsizei n, GLuint *textures);
extern GLboolean (APIENTRY *gglIsTextureEXT)(GLuint texture);
extern void (APIENTRY *gglPrioritizeTexturesEXT)(GLsizei n, const GLuint *textures, const GLclampf *priorities);

//GL_SGIS_detail_texture
extern void (APIENTRY *gglDetailTexFuncSGIS)(GLenum target, GLsizei n, const GLfloat *points);
extern void (APIENTRY *gglGetDetailTexFuncSGIS)(GLenum target, GLfloat *points);

//GL_SGIS_sharpen_texture
extern void (APIENTRY *gglSharpenTexFuncSGIS)(GLenum target, GLsizei n, const GLfloat *points);
extern void (APIENTRY *gglGetSharpenTexFuncSGIS)(GLenum target, GLfloat *points);

//GL_SGIS_multisample
extern void (APIENTRY *gglSampleMaskSGIS)(GLclampf value, GLboolean invert);
extern void (APIENTRY *gglSamplePatternSGIS)(GLenum pattern);

//GL_EXT_vertex_array
extern void (APIENTRY *gglArrayElementEXT)(GLint i);
extern void (APIENTRY *gglColorPointerEXT)(GLint size, GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer);
extern void (APIENTRY *gglDrawArraysEXT)(GLenum mode, GLint first, GLsizei count);
extern void (APIENTRY *gglEdgeFlagPointerEXT)(GLsizei stride, GLsizei count, const GLboolean *pointer);
extern void (APIENTRY *gglGetPointervEXT)(GLenum pname, GLvoid* *params);
extern void (APIENTRY *gglIndexPointerEXT)(GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer);
extern void (APIENTRY *gglNormalPointerEXT)(GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer);
extern void (APIENTRY *gglTexCoordPointerEXT)(GLint size, GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer);
extern void (APIENTRY *gglVertexPointerEXT)(GLint size, GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer);

//GL_EXT_blend_minmax
extern void (APIENTRY *gglBlendEquationEXT)(GLenum mode);

//GL_SGIX_sprite
extern void (APIENTRY *gglSpriteParameterfSGIX)(GLenum pname, GLfloat param);
extern void (APIENTRY *gglSpriteParameterfvSGIX)(GLenum pname, const GLfloat *params);
extern void (APIENTRY *gglSpriteParameteriSGIX)(GLenum pname, GLint param);
extern void (APIENTRY *gglSpriteParameterivSGIX)(GLenum pname, const GLint *params);

//GL_EXT_point_parameters
extern void (APIENTRY *gglPointParameterfEXT)(GLenum pname, GLfloat param);
extern void (APIENTRY *gglPointParameterfvEXT)(GLenum pname, const GLfloat *params);

//GL_SGIS_point_parameters
extern void (APIENTRY *gglPointParameterfSGIS)(GLenum pname, GLfloat param);
extern void (APIENTRY *gglPointParameterfvSGIS)(GLenum pname, const GLfloat *params);

//GL_SGIX_instruments
extern GLint (APIENTRY *gglGetInstrumentsSGIX)(void);
extern void (APIENTRY *gglInstrumentsBufferSGIX)(GLsizei size, GLint *buffer);
extern GLint (APIENTRY *gglPollInstrumentsSGIX)(GLint *marker_p);
extern void (APIENTRY *gglReadInstrumentsSGIX)(GLint marker);
extern void (APIENTRY *gglStartInstrumentsSGIX)(void);
extern void (APIENTRY *gglStopInstrumentsSGIX)(GLint marker);

//GL_SGIX_framezoom
extern void (APIENTRY *gglFrameZoomSGIX)(GLint factor);

//GL_SGIX_tag_sample_buffer
extern void (APIENTRY *gglTagSampleBufferSGIX)(void);

//GL_SGIX_polynomial_ffd
extern void (APIENTRY *gglDeformationMap3dSGIX)(GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, GLdouble w1, GLdouble w2, GLint wstride, GLint worder, const GLdouble *points);
extern void (APIENTRY *gglDeformationMap3fSGIX)(GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, GLfloat w1, GLfloat w2, GLint wstride, GLint worder, const GLfloat *points);
extern void (APIENTRY *gglDeformSGIX)(GLbitfield mask);
extern void (APIENTRY *gglLoadIdentityDeformationMapSGIX)(GLbitfield mask);

//GL_SGIX_reference_plane
extern void (APIENTRY *gglReferencePlaneSGIX)(const GLdouble *equation);

//GL_SGIX_flush_raster
extern void (APIENTRY *gglFlushRasterSGIX)(void);

//GL_SGIS_fog_function
extern void (APIENTRY *gglFogFuncSGIS)(GLsizei n, const GLfloat *points);
extern void (APIENTRY *gglGetFogFuncSGIS)(GLfloat *points);

//GL_HP_image_transform
extern void (APIENTRY *gglImageTransformParameteriHP)(GLenum target, GLenum pname, GLint param);
extern void (APIENTRY *gglImageTransformParameterfHP)(GLenum target, GLenum pname, GLfloat param);
extern void (APIENTRY *gglImageTransformParameterivHP)(GLenum target, GLenum pname, const GLint *params);
extern void (APIENTRY *gglImageTransformParameterfvHP)(GLenum target, GLenum pname, const GLfloat *params);
extern void (APIENTRY *gglGetImageTransformParameterivHP)(GLenum target, GLenum pname, GLint *params);
extern void (APIENTRY *gglGetImageTransformParameterfvHP)(GLenum target, GLenum pname, GLfloat *params);

//GL_EXT_color_subtable
extern void (APIENTRY *gglColorSubTableEXT)(GLenum target, GLsizei start, GLsizei count, GLenum format, GLenum type, const GLvoid *data);
extern void (APIENTRY *gglCopyColorSubTableEXT)(GLenum target, GLsizei start, GLint x, GLint y, GLsizei width);

//GL_PGI_misc_hints
extern void (APIENTRY *gglHintPGI)(GLenum target, GLint mode);

//GL_EXT_paletted_texture
extern void (APIENTRY *gglColorTableEXT)(GLenum target, GLenum internalFormat, GLsizei width, GLenum format, GLenum type, const GLvoid *table);
extern void (APIENTRY *gglGetColorTableEXT)(GLenum target, GLenum format, GLenum type, GLvoid *data);
extern void (APIENTRY *gglGetColorTableParameterivEXT)(GLenum target, GLenum pname, GLint *params);
extern void (APIENTRY *gglGetColorTableParameterfvEXT)(GLenum target, GLenum pname, GLfloat *params);

//GL_SGIX_list_priority
extern void (APIENTRY *gglGetListParameterfvSGIX)(GLuint list, GLenum pname, GLfloat *params);
extern void (APIENTRY *gglGetListParameterivSGIX)(GLuint list, GLenum pname, GLint *params);
extern void (APIENTRY *gglListParameterfSGIX)(GLuint list, GLenum pname, GLfloat param);
extern void (APIENTRY *gglListParameterfvSGIX)(GLuint list, GLenum pname, const GLfloat *params);
extern void (APIENTRY *gglListParameteriSGIX)(GLuint list, GLenum pname, GLint param);
extern void (APIENTRY *gglListParameterivSGIX)(GLuint list, GLenum pname, const GLint *params);

//GL_EXT_index_material
extern void (APIENTRY *gglIndexMaterialEXT)(GLenum face, GLenum mode);

//GL_EXT_index_func
extern void (APIENTRY *gglIndexFuncEXT)(GLenum func, GLclampf ref);

//GL_EXT_compiled_vertex_array
extern void (APIENTRY *gglLockArraysEXT)(GLint first, GLsizei count);
extern void (APIENTRY *gglUnlockArraysEXT)(void);

//GL_EXT_cull_vertex
extern void (APIENTRY *gglCullParameterdvEXT)(GLenum pname, GLdouble *params);
extern void (APIENTRY *gglCullParameterfvEXT)(GLenum pname, GLfloat *params);

//GL_SGIX_fragment_lighting
extern void (APIENTRY *gglFragmentColorMaterialSGIX)(GLenum face, GLenum mode);
extern void (APIENTRY *gglFragmentLightfSGIX)(GLenum light, GLenum pname, GLfloat param);
extern void (APIENTRY *gglFragmentLightfvSGIX)(GLenum light, GLenum pname, const GLfloat *params);
extern void (APIENTRY *gglFragmentLightiSGIX)(GLenum light, GLenum pname, GLint param);
extern void (APIENTRY *gglFragmentLightivSGIX)(GLenum light, GLenum pname, const GLint *params);
extern void (APIENTRY *gglFragmentLightModelfSGIX)(GLenum pname, GLfloat param);
extern void (APIENTRY *gglFragmentLightModelfvSGIX)(GLenum pname, const GLfloat *params);
extern void (APIENTRY *gglFragmentLightModeliSGIX)(GLenum pname, GLint param);
extern void (APIENTRY *gglFragmentLightModelivSGIX)(GLenum pname, const GLint *params);
extern void (APIENTRY *gglFragmentMaterialfSGIX)(GLenum face, GLenum pname, GLfloat param);
extern void (APIENTRY *gglFragmentMaterialfvSGIX)(GLenum face, GLenum pname, const GLfloat *params);
extern void (APIENTRY *gglFragmentMaterialiSGIX)(GLenum face, GLenum pname, GLint param);
extern void (APIENTRY *gglFragmentMaterialivSGIX)(GLenum face, GLenum pname, const GLint *params);
extern void (APIENTRY *gglGetFragmentLightfvSGIX)(GLenum light, GLenum pname, GLfloat *params);
extern void (APIENTRY *gglGetFragmentLightivSGIX)(GLenum light, GLenum pname, GLint *params);
extern void (APIENTRY *gglGetFragmentMaterialfvSGIX)(GLenum face, GLenum pname, GLfloat *params);
extern void (APIENTRY *gglGetFragmentMaterialivSGIX)(GLenum face, GLenum pname, GLint *params);
extern void (APIENTRY *gglLightEnviSGIX)(GLenum pname, GLint param);

//GL_EXT_fragment_lighting
extern void (APIENTRY *gglFragmentColorMaterialEXT)(GLenum face, GLenum mode);
extern void (APIENTRY *gglFragmentLightfEXT)(GLenum light, GLenum pname, GLfloat param);
extern void (APIENTRY *gglFragmentLightfvEXT)(GLenum light, GLenum pname, const GLfloat *params);
extern void (APIENTRY *gglFragmentLightiEXT)(GLenum light, GLenum pname, GLint param);
extern void (APIENTRY *gglFragmentLightivEXT)(GLenum light, GLenum pname, const GLint *params);
extern void (APIENTRY *gglFragmentLightModelfEXT)(GLenum pname, GLfloat param);
extern void (APIENTRY *gglFragmentLightModelfvEXT)(GLenum pname, const GLfloat *params);
extern void (APIENTRY *gglFragmentLightModeliEXT)(GLenum pname, GLint param);
extern void (APIENTRY *gglFragmentLightModelivEXT)(GLenum pname, const GLint *params);
extern void (APIENTRY *gglFragmentMaterialfEXT)(GLenum face, GLenum pname, GLfloat param);
extern void (APIENTRY *gglFragmentMaterialfvEXT)(GLenum face, GLenum pname, const GLfloat *params);
extern void (APIENTRY *gglFragmentMaterialiEXT)(GLenum face, GLenum pname, GLint param);
extern void (APIENTRY *gglFragmentMaterialivEXT)(GLenum face, GLenum pname, const GLint *params);
extern void (APIENTRY *gglGetFragmentLightfvEXT)(GLenum light, GLenum pname, GLfloat *params);
extern void (APIENTRY *gglGetFragmentLightivEXT)(GLenum light, GLenum pname, GLint *params);
extern void (APIENTRY *gglGetFragmentMaterialfvEXT)(GLenum face, GLenum pname, GLfloat *params);
extern void (APIENTRY *gglGetFragmentMaterialivEXT)(GLenum face, GLenum pname, GLint *params);
extern void (APIENTRY *gglLightEnviEXT)(GLenum pname, GLint param);

//GL_EXT_draw_range_elements
extern void (APIENTRY *gglDrawRangeElementsEXT)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices);

//GL_EXT_light_texture
extern void (APIENTRY *gglApplyTextureEXT)(GLenum mode);
extern void (APIENTRY *gglTextureLightEXT)(GLenum pname);
extern void (APIENTRY *gglTextureMaterialEXT)(GLenum face, GLenum mode);

//GL_EXT_scene_marker
extern void (APIENTRY *gglBeginSceneEXT)(GLvoid);
extern void (APIENTRY *gglEndSceneEXT)(GLvoid);

//GL_SGIX_async
extern void (APIENTRY *gglAsyncMarkerSGIX)(GLuint marker);
extern GLint (APIENTRY *gglFinishAsyncSGIX)(GLuint *markerp);
extern GLint (APIENTRY *gglPollAsyncSGIX)(GLuint *markerp);
extern GLuint (APIENTRY *gglGenAsyncMarkersSGIX)(GLsizei range);
extern void (APIENTRY *gglDeleteAsyncMarkersSGIX)(GLuint marker, GLsizei range);
extern GLboolean (APIENTRY *gglIsAsyncMarkerSGIX)(GLuint marker);

//GL_INTEL_texture_scissor
extern void (APIENTRY *gglTexScissorINTEL)(GLenum target, GLclampf tlow, GLclampf thigh);
extern void (APIENTRY *gglTexScissorFuncINTEL)(GLenum target, GLenum lfunc, GLenum hfunc);

//GL_INTEL_parallel_arrays
extern void (APIENTRY *gglVertexPointervINTEL)(GLint size, GLenum type, const GLvoid* *pointer);
extern void (APIENTRY *gglNormalPointervINTEL)(GLenum type, const GLvoid* *pointer);
extern void (APIENTRY *gglColorPointervINTEL)(GLint size, GLenum type, const GLvoid* *pointer);
extern void (APIENTRY *gglTexCoordPointervINTEL)(GLint size, GLenum type, const GLvoid* *pointer);

//GL_EXT_pixel_transform
extern void (APIENTRY *gglPixelTransformParameteriEXT)(GLenum target, GLenum pname, GLint param);
extern void (APIENTRY *gglPixelTransformParameterfEXT)(GLenum target, GLenum pname, GLfloat param);
extern void (APIENTRY *gglPixelTransformParameterivEXT)(GLenum target, GLenum pname, const GLint *params);
extern void (APIENTRY *gglPixelTransformParameterfvEXT)(GLenum target, GLenum pname, const GLfloat *params);

//GL_EXT_secondary_color
extern void (APIENTRY *gglSecondaryColor3bEXT)(GLbyte red, GLbyte green, GLbyte blue);
extern void (APIENTRY *gglSecondaryColor3bvEXT)(const GLbyte *v);
extern void (APIENTRY *gglSecondaryColor3dEXT)(GLdouble red, GLdouble green, GLdouble blue);
extern void (APIENTRY *gglSecondaryColor3dvEXT)(const GLdouble *v);
extern void (APIENTRY *gglSecondaryColor3fEXT)(GLfloat red, GLfloat green, GLfloat blue);
extern void (APIENTRY *gglSecondaryColor3fvEXT)(const GLfloat *v);
extern void (APIENTRY *gglSecondaryColor3iEXT)(GLint red, GLint green, GLint blue);
extern void (APIENTRY *gglSecondaryColor3ivEXT)(const GLint *v);
extern void (APIENTRY *gglSecondaryColor3sEXT)(GLshort red, GLshort green, GLshort blue);
extern void (APIENTRY *gglSecondaryColor3svEXT)(const GLshort *v);
extern void (APIENTRY *gglSecondaryColor3ubEXT)(GLubyte red, GLubyte green, GLubyte blue);
extern void (APIENTRY *gglSecondaryColor3ubvEXT)(const GLubyte *v);
extern void (APIENTRY *gglSecondaryColor3uiEXT)(GLuint red, GLuint green, GLuint blue);
extern void (APIENTRY *gglSecondaryColor3uivEXT)(const GLuint *v);
extern void (APIENTRY *gglSecondaryColor3usEXT)(GLushort red, GLushort green, GLushort blue);
extern void (APIENTRY *gglSecondaryColor3usvEXT)(const GLushort *v);
extern void (APIENTRY *gglSecondaryColorPointerEXT)(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);

//GL_EXT_texture_perturb_normal
extern void (APIENTRY *gglTextureNormalEXT)(GLenum mode);

//GL_EXT_multi_draw_arrays
extern void (APIENTRY *gglMultiDrawArraysEXT)(GLenum mode, GLint *first, GLsizei *count, GLsizei primcount);
extern void (APIENTRY *gglMultiDrawElementsEXT)(GLenum mode, const GLsizei *count, GLenum type, const GLvoid* *indices, GLsizei primcount);

//GL_EXT_fog_coord
extern void (APIENTRY *gglFogCoordfEXT)(GLfloat coord);
extern void (APIENTRY *gglFogCoordfvEXT)(const GLfloat *coord);
extern void (APIENTRY *gglFogCoorddEXT)(GLdouble coord);
extern void (APIENTRY *gglFogCoorddvEXT)(const GLdouble *coord);
extern void (APIENTRY *gglFogCoordPointerEXT)(GLenum type, GLsizei stride, const GLvoid *pointer);

//GL_EXT_coordinate_frame
extern void (APIENTRY *gglTangent3bEXT)(GLbyte tx, GLbyte ty, GLbyte tz);
extern void (APIENTRY *gglTangent3bvEXT)(const GLbyte *v);
extern void (APIENTRY *gglTangent3dEXT)(GLdouble tx, GLdouble ty, GLdouble tz);
extern void (APIENTRY *gglTangent3dvEXT)(const GLdouble *v);
extern void (APIENTRY *gglTangent3fEXT)(GLfloat tx, GLfloat ty, GLfloat tz);
extern void (APIENTRY *gglTangent3fvEXT)(const GLfloat *v);
extern void (APIENTRY *gglTangent3iEXT)(GLint tx, GLint ty, GLint tz);
extern void (APIENTRY *gglTangent3ivEXT)(const GLint *v);
extern void (APIENTRY *gglTangent3sEXT)(GLshort tx, GLshort ty, GLshort tz);
extern void (APIENTRY *gglTangent3svEXT)(const GLshort *v);
extern void (APIENTRY *gglBinormal3bEXT)(GLbyte bx, GLbyte by, GLbyte bz);
extern void (APIENTRY *gglBinormal3bvEXT)(const GLbyte *v);
extern void (APIENTRY *gglBinormal3dEXT)(GLdouble bx, GLdouble by, GLdouble bz);
extern void (APIENTRY *gglBinormal3dvEXT)(const GLdouble *v);
extern void (APIENTRY *gglBinormal3fEXT)(GLfloat bx, GLfloat by, GLfloat bz);
extern void (APIENTRY *gglBinormal3fvEXT)(const GLfloat *v);
extern void (APIENTRY *gglBinormal3iEXT)(GLint bx, GLint by, GLint bz);
extern void (APIENTRY *gglBinormal3ivEXT)(const GLint *v);
extern void (APIENTRY *gglBinormal3sEXT)(GLshort bx, GLshort by, GLshort bz);
extern void (APIENTRY *gglBinormal3svEXT)(const GLshort *v);
extern void (APIENTRY *gglTangentPointerEXT)(GLenum type, GLsizei stride, const GLvoid *pointer);
extern void (APIENTRY *gglBinormalPointerEXT)(GLenum type, GLsizei stride, const GLvoid *pointer);

//GL_SUNX_constant_data
extern void (APIENTRY *gglFinishTextureSUNX)(void);

//GL_SUN_global_alpha
extern void (APIENTRY *gglGlobalAlphaFactorbSUN)(GLbyte factor);
extern void (APIENTRY *gglGlobalAlphaFactorsSUN)(GLshort factor);
extern void (APIENTRY *gglGlobalAlphaFactoriSUN)(GLint factor);
extern void (APIENTRY *gglGlobalAlphaFactorfSUN)(GLfloat factor);
extern void (APIENTRY *gglGlobalAlphaFactordSUN)(GLdouble factor);
extern void (APIENTRY *gglGlobalAlphaFactorubSUN)(GLubyte factor);
extern void (APIENTRY *gglGlobalAlphaFactorusSUN)(GLushort factor);
extern void (APIENTRY *gglGlobalAlphaFactoruiSUN)(GLuint factor);

//GL_SUN_triangle_list
extern void (APIENTRY *gglReplacementCodeuiSUN)(GLuint code);
extern void (APIENTRY *gglReplacementCodeusSUN)(GLushort code);
extern void (APIENTRY *gglReplacementCodeubSUN)(GLubyte code);
extern void (APIENTRY *gglReplacementCodeuivSUN)(const GLuint *code);
extern void (APIENTRY *gglReplacementCodeusvSUN)(const GLushort *code);
extern void (APIENTRY *gglReplacementCodeubvSUN)(const GLubyte *code);
extern void (APIENTRY *gglReplacementCodePointerSUN)(GLenum type, GLsizei stride, const GLvoid* *pointer);

//GL_SUN_vertex
extern void (APIENTRY *gglColor4ubVertex2fSUN)(GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y);
extern void (APIENTRY *gglColor4ubVertex2fvSUN)(const GLubyte *c, const GLfloat *v);
extern void (APIENTRY *gglColor4ubVertex3fSUN)(GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y, GLfloat z);
extern void (APIENTRY *gglColor4ubVertex3fvSUN)(const GLubyte *c, const GLfloat *v);
extern void (APIENTRY *gglColor3fVertex3fSUN)(GLfloat r, GLfloat g, GLfloat b, GLfloat x, GLfloat y, GLfloat z);
extern void (APIENTRY *gglColor3fVertex3fvSUN)(const GLfloat *c, const GLfloat *v);
extern void (APIENTRY *gglNormal3fVertex3fSUN)(GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
extern void (APIENTRY *gglNormal3fVertex3fvSUN)(const GLfloat *n, const GLfloat *v);
extern void (APIENTRY *gglColor4fNormal3fVertex3fSUN)(GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
extern void (APIENTRY *gglColor4fNormal3fVertex3fvSUN)(const GLfloat *c, const GLfloat *n, const GLfloat *v);
extern void (APIENTRY *gglTexCoord2fVertex3fSUN)(GLfloat s, GLfloat t, GLfloat x, GLfloat y, GLfloat z);
extern void (APIENTRY *gglTexCoord2fVertex3fvSUN)(const GLfloat *tc, const GLfloat *v);
extern void (APIENTRY *gglTexCoord4fVertex4fSUN)(GLfloat s, GLfloat t, GLfloat p, GLfloat q, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern void (APIENTRY *gglTexCoord4fVertex4fvSUN)(const GLfloat *tc, const GLfloat *v);
extern void (APIENTRY *gglTexCoord2fColor4ubVertex3fSUN)(GLfloat s, GLfloat t, GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y, GLfloat z);
extern void (APIENTRY *gglTexCoord2fColor4ubVertex3fvSUN)(const GLfloat *tc, const GLubyte *c, const GLfloat *v);
extern void (APIENTRY *gglTexCoord2fColor3fVertex3fSUN)(GLfloat s, GLfloat t, GLfloat r, GLfloat g, GLfloat b, GLfloat x, GLfloat y, GLfloat z);
extern void (APIENTRY *gglTexCoord2fColor3fVertex3fvSUN)(const GLfloat *tc, const GLfloat *c, const GLfloat *v);
extern void (APIENTRY *gglTexCoord2fNormal3fVertex3fSUN)(GLfloat s, GLfloat t, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
extern void (APIENTRY *gglTexCoord2fNormal3fVertex3fvSUN)(const GLfloat *tc, const GLfloat *n, const GLfloat *v);
extern void (APIENTRY *gglTexCoord2fColor4fNormal3fVertex3fSUN)(GLfloat s, GLfloat t, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
extern void (APIENTRY *gglTexCoord2fColor4fNormal3fVertex3fvSUN)(const GLfloat *tc, const GLfloat *c, const GLfloat *n, const GLfloat *v);
extern void (APIENTRY *gglTexCoord4fColor4fNormal3fVertex4fSUN)(GLfloat s, GLfloat t, GLfloat p, GLfloat q, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern void (APIENTRY *gglTexCoord4fColor4fNormal3fVertex4fvSUN)(const GLfloat *tc, const GLfloat *c, const GLfloat *n, const GLfloat *v);
extern void (APIENTRY *gglReplacementCodeuiVertex3fSUN)(GLuint rc, GLfloat x, GLfloat y, GLfloat z);
extern void (APIENTRY *gglReplacementCodeuiVertex3fvSUN)(const GLuint *rc, const GLfloat *v);
extern void (APIENTRY *gglReplacementCodeuiColor4ubVertex3fSUN)(GLuint rc, GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y, GLfloat z);
extern void (APIENTRY *gglReplacementCodeuiColor4ubVertex3fvSUN)(const GLuint *rc, const GLubyte *c, const GLfloat *v);
extern void (APIENTRY *gglReplacementCodeuiColor3fVertex3fSUN)(GLuint rc, GLfloat r, GLfloat g, GLfloat b, GLfloat x, GLfloat y, GLfloat z);
extern void (APIENTRY *gglReplacementCodeuiColor3fVertex3fvSUN)(const GLuint *rc, const GLfloat *c, const GLfloat *v);
extern void (APIENTRY *gglReplacementCodeuiNormal3fVertex3fSUN)(GLuint rc, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
extern void (APIENTRY *gglReplacementCodeuiNormal3fVertex3fvSUN)(const GLuint *rc, const GLfloat *n, const GLfloat *v);
extern void (APIENTRY *gglReplacementCodeuiColor4fNormal3fVertex3fSUN)(GLuint rc, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
extern void (APIENTRY *gglReplacementCodeuiColor4fNormal3fVertex3fvSUN)(const GLuint *rc, const GLfloat *c, const GLfloat *n, const GLfloat *v);
extern void (APIENTRY *gglReplacementCodeuiTexCoord2fVertex3fSUN)(GLuint rc, GLfloat s, GLfloat t, GLfloat x, GLfloat y, GLfloat z);
extern void (APIENTRY *gglReplacementCodeuiTexCoord2fVertex3fvSUN)(const GLuint *rc, const GLfloat *tc, const GLfloat *v);
extern void (APIENTRY *gglReplacementCodeuiTexCoord2fNormal3fVertex3fSUN)(GLuint rc, GLfloat s, GLfloat t, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
extern void (APIENTRY *gglReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN)(const GLuint *rc, const GLfloat *tc, const GLfloat *n, const GLfloat *v);
extern void (APIENTRY *gglReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN)(GLuint rc, GLfloat s, GLfloat t, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
extern void (APIENTRY *gglReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN)(const GLuint *rc, const GLfloat *tc, const GLfloat *c, const GLfloat *n, const GLfloat *v);

//GL_EXT_blend_func_separate
extern void (APIENTRY *gglBlendFuncSeparateEXT)(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);

//GL_EXT_vertex_weighting
extern void (APIENTRY *gglVertexWeightfEXT)(GLfloat weight);
extern void (APIENTRY *gglVertexWeightfvEXT)(const GLfloat *weight);
extern void (APIENTRY *gglVertexWeightPointerEXT)(GLsizei size, GLenum type, GLsizei stride, const GLvoid *pointer);

//GL_NV_vertex_array_range
extern void (APIENTRY *gglFlushVertexArrayRangeNV)(void);
extern void (APIENTRY *gglVertexArrayRangeNV)(GLsizei length, const GLvoid *pointer);

//GL_NV_register_combiners
extern void (APIENTRY *gglCombinerParameterfvNV)(GLenum pname, const GLfloat *params);
extern void (APIENTRY *gglCombinerParameterfNV)(GLenum pname, GLfloat param);
extern void (APIENTRY *gglCombinerParameterivNV)(GLenum pname, const GLint *params);
extern void (APIENTRY *gglCombinerParameteriNV)(GLenum pname, GLint param);
extern void (APIENTRY *gglCombinerInputNV)(GLenum stage, GLenum portion, GLenum variable, GLenum input, GLenum mapping, GLenum componentUsage);
extern void (APIENTRY *gglCombinerOutputNV)(GLenum stage, GLenum portion, GLenum abOutput, GLenum cdOutput, GLenum sumOutput, GLenum scale, GLenum bias, GLboolean abDotProduct, GLboolean cdDotProduct, GLboolean muxSum);
extern void (APIENTRY *gglFinalCombinerInputNV)(GLenum variable, GLenum input, GLenum mapping, GLenum componentUsage);
extern void (APIENTRY *gglGetCombinerInputParameterfvNV)(GLenum stage, GLenum portion, GLenum variable, GLenum pname, GLfloat *params);
extern void (APIENTRY *gglGetCombinerInputParameterivNV)(GLenum stage, GLenum portion, GLenum variable, GLenum pname, GLint *params);
extern void (APIENTRY *gglGetCombinerOutputParameterfvNV)(GLenum stage, GLenum portion, GLenum pname, GLfloat *params);
extern void (APIENTRY *gglGetCombinerOutputParameterivNV)(GLenum stage, GLenum portion, GLenum pname, GLint *params);
extern void (APIENTRY *gglGetFinalCombinerInputParameterfvNV)(GLenum variable, GLenum pname, GLfloat *params);
extern void (APIENTRY *gglGetFinalCombinerInputParameterivNV)(GLenum variable, GLenum pname, GLint *params);

//GL_MESA_resize_buffers
extern void (APIENTRY *gglResizeBuffersMESA)(void);

//GL_MESA_window_pos
extern void (APIENTRY *gglWindowPos2dMESA)(GLdouble x, GLdouble y);
extern void (APIENTRY *gglWindowPos2dvMESA)(const GLdouble *v);
extern void (APIENTRY *gglWindowPos2fMESA)(GLfloat x, GLfloat y);
extern void (APIENTRY *gglWindowPos2fvMESA)(const GLfloat *v);
extern void (APIENTRY *gglWindowPos2iMESA)(GLint x, GLint y);
extern void (APIENTRY *gglWindowPos2ivMESA)(const GLint *v);
extern void (APIENTRY *gglWindowPos2sMESA)(GLshort x, GLshort y);
extern void (APIENTRY *gglWindowPos2svMESA)(const GLshort *v);
extern void (APIENTRY *gglWindowPos3dMESA)(GLdouble x, GLdouble y, GLdouble z);
extern void (APIENTRY *gglWindowPos3dvMESA)(const GLdouble *v);
extern void (APIENTRY *gglWindowPos3fMESA)(GLfloat x, GLfloat y, GLfloat z);
extern void (APIENTRY *gglWindowPos3fvMESA)(const GLfloat *v);
extern void (APIENTRY *gglWindowPos3iMESA)(GLint x, GLint y, GLint z);
extern void (APIENTRY *gglWindowPos3ivMESA)(const GLint *v);
extern void (APIENTRY *gglWindowPos3sMESA)(GLshort x, GLshort y, GLshort z);
extern void (APIENTRY *gglWindowPos3svMESA)(const GLshort *v);
extern void (APIENTRY *gglWindowPos4dMESA)(GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern void (APIENTRY *gglWindowPos4dvMESA)(const GLdouble *v);
extern void (APIENTRY *gglWindowPos4fMESA)(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern void (APIENTRY *gglWindowPos4fvMESA)(const GLfloat *v);
extern void (APIENTRY *gglWindowPos4iMESA)(GLint x, GLint y, GLint z, GLint w);
extern void (APIENTRY *gglWindowPos4ivMESA)(const GLint *v);
extern void (APIENTRY *gglWindowPos4sMESA)(GLshort x, GLshort y, GLshort z, GLshort w);
extern void (APIENTRY *gglWindowPos4svMESA)(const GLshort *v);

//GL_IBM_multimode_draw_arrays
extern void (APIENTRY *gglMultiModeDrawArraysIBM)(GLenum mode, const GLint *first, const GLsizei *count, GLsizei primcount, GLint modestride);
extern void (APIENTRY *gglMultiModeDrawElementsIBM)(const GLenum *mode, const GLsizei *count, GLenum type, const GLvoid* *indices, GLsizei primcount, GLint modestride);

//GL_IBM_vertex_array_lists
extern void (APIENTRY *gglColorPointerListIBM)(GLint size, GLenum type, GLint stride, const GLvoid* *pointer, GLint ptrstride);
extern void (APIENTRY *gglSecondaryColorPointerListIBM)(GLint size, GLenum type, GLint stride, const GLvoid* *pointer, GLint ptrstride);
extern void (APIENTRY *gglEdgeFlagPointerListIBM)(GLint stride, const GLboolean* *pointer, GLint ptrstride);
extern void (APIENTRY *gglFogCoordPointerListIBM)(GLenum type, GLint stride, const GLvoid* *pointer, GLint ptrstride);
extern void (APIENTRY *gglIndexPointerListIBM)(GLenum type, GLint stride, const GLvoid* *pointer, GLint ptrstride);
extern void (APIENTRY *gglNormalPointerListIBM)(GLenum type, GLint stride, const GLvoid* *pointer, GLint ptrstride);
extern void (APIENTRY *gglTexCoordPointerListIBM)(GLint size, GLenum type, GLint stride, const GLvoid* *pointer, GLint ptrstride);
extern void (APIENTRY *gglVertexPointerListIBM)(GLint size, GLenum type, GLint stride, const GLvoid* *pointer, GLint ptrstride);

//GL_3DFX_tbuffer
extern void (APIENTRY *gglTbufferMask3DFX)(GLuint mask);

//GL_EXT_multisample
extern void (APIENTRY *gglSampleMaskEXT)(GLclampf value, GLboolean invert);
extern void (APIENTRY *gglSamplePatternEXT)(GLenum pattern);

//GL_SGIS_texture_color_mask
extern void (APIENTRY *gglTextureColorMaskSGIS)(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);

//GL_SGIX_igloo_interface
extern void (APIENTRY *gglIglooInterfaceSGIX)(GLenum pname, const GLvoid *params);

//GL_NV_fence
extern void (APIENTRY *gglDeleteFencesNV)(GLsizei n, const GLuint *fences);
extern void (APIENTRY *gglGenFencesNV)(GLsizei n, GLuint *fences);
extern GLboolean (APIENTRY *gglIsFenceNV)(GLuint fence);
extern GLboolean (APIENTRY *gglTestFenceNV)(GLuint fence);
extern void (APIENTRY *gglGetFenceivNV)(GLuint fence, GLenum pname, GLint *params);
extern void (APIENTRY *gglFinishFenceNV)(GLuint fence);
extern void (APIENTRY *gglSetFenceNV)(GLuint fence, GLenum condition);

//GL_IBM_static_data
void (APIENTRY *gglFlushStaticDataIBM)(GLenum target);

//GL_NV_evaluators
extern void (APIENTRY *gglMapControlPointsNV)(GLenum target, GLuint index, GLenum type, GLsizei ustride, GLsizei vstride, GLint uorder, GLint vorder, GLboolean packed, const GLvoid *points);
extern void (APIENTRY *gglMapParameterivNV)(GLenum target, GLenum pname, const GLint *params);
extern void (APIENTRY *gglMapParameterfvNV)(GLenum target, GLenum pname, const GLfloat *params);
extern void (APIENTRY *gglGetMapControlPointsNV)(GLenum target, GLuint index, GLenum type, GLsizei ustride, GLsizei vstride, GLboolean packed, GLvoid *points);
extern void (APIENTRY *gglGetMapParameterivNV)(GLenum target, GLenum pname, GLint *params);
extern void (APIENTRY *gglGetMapParameterfvNV)(GLenum target, GLenum pname, GLfloat *params);
extern void (APIENTRY *gglGetMapAttribParameterivNV)(GLenum target, GLuint index, GLenum pname, GLint *params);
extern void (APIENTRY *gglGetMapAttribParameterfvNV)(GLenum target, GLuint index, GLenum pname, GLfloat *params);
extern void (APIENTRY *gglEvalMapsNV)(GLenum target, GLenum mode);

//GL_NV_register_combiners2
extern void (APIENTRY *gglCombinerStageParameterfvNV)(GLenum stage, GLenum pname, const GLfloat *params);
extern void (APIENTRY *gglGetCombinerStageParameterfvNV)(GLenum stage, GLenum pname, GLfloat *params);

//GL_NV_vertex_program
extern GLboolean (APIENTRY *gglAreProgramsResidentNV)(GLsizei n, const GLuint *programs, GLboolean *residences);
extern void (APIENTRY *gglBindProgramNV)(GLenum target, GLuint id);
extern void (APIENTRY *gglDeleteProgramsNV)(GLsizei n, const GLuint *programs);
extern void (APIENTRY *gglExecuteProgramNV)(GLenum target, GLuint id, const GLfloat *params);
extern void (APIENTRY *gglGenProgramsNV)(GLsizei n, GLuint *programs);
extern void (APIENTRY *gglGetProgramParameterdvNV)(GLenum target, GLuint index, GLenum pname, GLdouble *params);
extern void (APIENTRY *gglGetProgramParameterfvNV)(GLenum target, GLuint index, GLenum pname, GLfloat *params);
extern void (APIENTRY *gglGetProgramivNV)(GLuint id, GLenum pname, GLint *params);
extern void (APIENTRY *gglGetProgramStringNV)(GLuint id, GLenum pname, GLubyte *program);
extern void (APIENTRY *gglGetTrackMatrixivNV)(GLenum target, GLuint address, GLenum pname, GLint *params);
extern void (APIENTRY *gglGetVertexAttribdvNV)(GLuint index, GLenum pname, GLdouble *params);
extern void (APIENTRY *gglGetVertexAttribfvNV)(GLuint index, GLenum pname, GLfloat *params);
extern void (APIENTRY *gglGetVertexAttribivNV)(GLuint index, GLenum pname, GLint *params);
extern void (APIENTRY *gglGetVertexAttribPointervNV)(GLuint index, GLenum pname, GLvoid* *pointer);
extern GLboolean (APIENTRY *gglIsProgramNV)(GLuint id);
extern void (APIENTRY *gglLoadProgramNV)(GLenum target, GLuint id, GLsizei len, const GLubyte *program);
extern void (APIENTRY *gglProgramParameter4dNV)(GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern void (APIENTRY *gglProgramParameter4dvNV)(GLenum target, GLuint index, const GLdouble *v);
extern void (APIENTRY *gglProgramParameter4fNV)(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern void (APIENTRY *gglProgramParameter4fvNV)(GLenum target, GLuint index, const GLfloat *v);
extern void (APIENTRY *gglProgramParameters4dvNV)(GLenum target, GLuint index, GLuint count, const GLdouble *v);
extern void (APIENTRY *gglProgramParameters4fvNV)(GLenum target, GLuint index, GLuint count, const GLfloat *v);
extern void (APIENTRY *gglRequestResidentProgramsNV)(GLsizei n, const GLuint *programs);
extern void (APIENTRY *gglTrackMatrixNV)(GLenum target, GLuint address, GLenum matrix, GLenum transform);
extern void (APIENTRY *gglVertexAttribPointerNV)(GLuint index, GLint fsize, GLenum type, GLsizei stride, const GLvoid *pointer);
extern void (APIENTRY *gglVertexAttrib1dNV)(GLuint index, GLdouble x);
extern void (APIENTRY *gglVertexAttrib1dvNV)(GLuint index, const GLdouble *v);
extern void (APIENTRY *gglVertexAttrib1fNV)(GLuint index, GLfloat x);
extern void (APIENTRY *gglVertexAttrib1fvNV)(GLuint index, const GLfloat *v);
extern void (APIENTRY *gglVertexAttrib1sNV)(GLuint index, GLshort x);
extern void (APIENTRY *gglVertexAttrib1svNV)(GLuint index, const GLshort *v);
extern void (APIENTRY *gglVertexAttrib2dNV)(GLuint index, GLdouble x, GLdouble y);
extern void (APIENTRY *gglVertexAttrib2dvNV)(GLuint index, const GLdouble *v);
extern void (APIENTRY *gglVertexAttrib2fNV)(GLuint index, GLfloat x, GLfloat y);
extern void (APIENTRY *gglVertexAttrib2fvNV)(GLuint index, const GLfloat *v);
extern void (APIENTRY *gglVertexAttrib2sNV)(GLuint index, GLshort x, GLshort y);
extern void (APIENTRY *gglVertexAttrib2svNV)(GLuint index, const GLshort *v);
extern void (APIENTRY *gglVertexAttrib3dNV)(GLuint index, GLdouble x, GLdouble y, GLdouble z);
extern void (APIENTRY *gglVertexAttrib3dvNV)(GLuint index, const GLdouble *v);
extern void (APIENTRY *gglVertexAttrib3fNV)(GLuint index, GLfloat x, GLfloat y, GLfloat z);
extern void (APIENTRY *gglVertexAttrib3fvNV)(GLuint index, const GLfloat *v);
extern void (APIENTRY *gglVertexAttrib3sNV)(GLuint index, GLshort x, GLshort y, GLshort z);
extern void (APIENTRY *gglVertexAttrib3svNV)(GLuint index, const GLshort *v);
extern void (APIENTRY *gglVertexAttrib4dNV)(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern void (APIENTRY *gglVertexAttrib4dvNV)(GLuint index, const GLdouble *v);
extern void (APIENTRY *gglVertexAttrib4fNV)(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern void (APIENTRY *gglVertexAttrib4fvNV)(GLuint index, const GLfloat *v);
extern void (APIENTRY *gglVertexAttrib4sNV)(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
extern void (APIENTRY *gglVertexAttrib4svNV)(GLuint index, const GLshort *v);
extern void (APIENTRY *gglVertexAttrib4ubNV)(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
extern void (APIENTRY *gglVertexAttrib4ubvNV)(GLuint index, const GLubyte *v);
extern void (APIENTRY *gglVertexAttribs1dvNV)(GLuint index, GLsizei count, const GLdouble *v);
extern void (APIENTRY *gglVertexAttribs1fvNV)(GLuint index, GLsizei count, const GLfloat *v);
extern void (APIENTRY *gglVertexAttribs1svNV)(GLuint index, GLsizei count, const GLshort *v);
extern void (APIENTRY *gglVertexAttribs2dvNV)(GLuint index, GLsizei count, const GLdouble *v);
extern void (APIENTRY *gglVertexAttribs2fvNV)(GLuint index, GLsizei count, const GLfloat *v);
extern void (APIENTRY *gglVertexAttribs2svNV)(GLuint index, GLsizei count, const GLshort *v);
extern void (APIENTRY *gglVertexAttribs3dvNV)(GLuint index, GLsizei count, const GLdouble *v);
extern void (APIENTRY *gglVertexAttribs3fvNV)(GLuint index, GLsizei count, const GLfloat *v);
extern void (APIENTRY *gglVertexAttribs3svNV)(GLuint index, GLsizei count, const GLshort *v);
extern void (APIENTRY *gglVertexAttribs4dvNV)(GLuint index, GLsizei count, const GLdouble *v);
extern void (APIENTRY *gglVertexAttribs4fvNV)(GLuint index, GLsizei count, const GLfloat *v);
extern void (APIENTRY *gglVertexAttribs4svNV)(GLuint index, GLsizei count, const GLshort *v);
extern void (APIENTRY *gglVertexAttribs4ubvNV)(GLuint index, GLsizei count, const GLubyte *v);

//GL_ATI_envmap_bumpmap
extern void (APIENTRY *glTexBumpParameterivATI)(GLenum pname, const GLint *param);
extern void (APIENTRY *glTexBumpParameterfvATI)(GLenum pname, const GLfloat *param);
extern void (APIENTRY *glGetTexBumpParameterivATI)(GLenum pname, GLint *param);
extern void (APIENTRY *glGetTexBumpParameterfvATI)(GLenum pname, GLfloat *param);

//GL_ATI_fragment_shader
extern GLuint (APIENTRY *gglGenFragmentShadersATI)(GLuint range);
extern void (APIENTRY *gglBindFragmentShaderATI)(GLuint id);
extern void (APIENTRY *gglDeleteFragmentShaderATI)(GLuint id);
extern void (APIENTRY *gglBeginFragmentShaderATI)(void);
extern void (APIENTRY *gglEndFragmentShaderATI)(void);
extern void (APIENTRY *gglPassTexCoordATI)(GLuint dst, GLuint coord, GLenum swizzle);
extern void (APIENTRY *gglSampleMapATI)(GLuint dst, GLuint interp, GLenum swizzle);
extern void (APIENTRY *gglColorFragmentOp1ATI)(GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod);
extern void (APIENTRY *gglColorFragmentOp2ATI)(GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod);
extern void (APIENTRY *gglColorFragmentOp3ATI)(GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod, GLuint arg3, GLuint arg3Rep, GLuint arg3Mod);
extern void (APIENTRY *gglAlphaFragmentOp1ATI)(GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod);
extern void (APIENTRY *gglAlphaFragmentOp2ATI)(GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod);
extern void (APIENTRY *gglAlphaFragmentOp3ATI)(GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod, GLuint arg3, GLuint arg3Rep, GLuint arg3Mod);
extern void (APIENTRY *gglSetFragmentShaderConstantATI)(GLuint dst, const GLfloat *value);

//GL_ATI_pn_triangles
extern void (APIENTRY *gglPNTrianglesiATI)(GLenum pname, GLint param);
extern void (APIENTRY *gglPNTrianglesfATI)(GLenum pname, GLfloat param);

//GL_ATI_vertex_array_object
extern GLuint (APIENTRY *gglNewObjectBufferATI)(GLsizei size, const GLvoid *pointer, GLenum usage);
extern GLboolean (APIENTRY *gglIsObjectBufferATI)(GLuint buffer);
extern void (APIENTRY *gglUpdateObjectBufferATI)(GLuint buffer, GLuint offset, GLsizei size, const GLvoid *pointer, GLenum preserve);
extern void (APIENTRY *gglGetObjectBufferfvATI)(GLuint buffer, GLenum pname, GLfloat *params);
extern void (APIENTRY *gglGetObjectBufferivATI)(GLuint buffer, GLenum pname, GLint *params);
extern void (APIENTRY *gglDeleteObjectBufferATI)(GLuint buffer);
extern void (APIENTRY *gglArrayObjectATI)(GLenum array, GLint size, GLenum type, GLsizei stride, GLuint buffer, GLuint offset);
extern void (APIENTRY *gglGetArrayObjectfvATI)(GLenum array, GLenum pname, GLfloat *params);
extern void (APIENTRY *gglGetArrayObjectivATI)(GLenum array, GLenum pname, GLint *params);
extern void (APIENTRY *gglVariantArrayObjectATI)(GLuint id, GLenum type, GLsizei stride, GLuint buffer, GLuint offset);
extern void (APIENTRY *gglGetVariantArrayObjectfvATI)(GLuint id, GLenum pname, GLfloat *params);
extern void (APIENTRY *gglGetVariantArrayObjectivATI)(GLuint id, GLenum pname, GLint *params);

//GL_EXT_vertex_shader
extern void (APIENTRY *gglBeginVertexShaderEXT)(void);
extern void (APIENTRY *gglEndVertexShaderEXT)(void);
extern void (APIENTRY *gglBindVertexShaderEXT)(GLuint id);
extern GLuint (APIENTRY *gglGenVertexShadersEXT)(GLuint range);
extern void (APIENTRY *gglDeleteVertexShaderEXT)(GLuint id);
extern void (APIENTRY *gglShaderOp1EXT)(GLenum op, GLuint res, GLuint arg1);
extern void (APIENTRY *gglShaderOp2EXT)(GLenum op, GLuint res, GLuint arg1, GLuint arg2);
extern void (APIENTRY *gglShaderOp3EXT)(GLenum op, GLuint res, GLuint arg1, GLuint arg2, GLuint arg3);
extern void (APIENTRY *gglSwizzleEXT)(GLuint res, GLuint in, GLenum outX, GLenum outY, GLenum outZ, GLenum outW);
extern void (APIENTRY *gglWriteMaskEXT)(GLuint res, GLuint in, GLenum outX, GLenum outY, GLenum outZ, GLenum outW);
extern void (APIENTRY *gglInsertComponentEXT)(GLuint res, GLuint src, GLuint num);
extern void (APIENTRY *gglExtractComponentEXT)(GLuint res, GLuint src, GLuint num);
extern GLuint (APIENTRY *gglGenSymbolsEXT)(GLenum datatype, GLenum storagetype, GLenum range, GLuint components);
extern void (APIENTRY *gglSetInvariantEXT)(GLuint id, GLenum type, const void *addr);
extern void (APIENTRY *gglSetLocalConstantEXT)(GLuint id, GLenum type, const void *addr);
extern void (APIENTRY *gglVariantbvEXT)(GLuint id, const GLbyte *addr);
extern void (APIENTRY *gglVariantsvEXT)(GLuint id, const GLshort *addr);
extern void (APIENTRY *gglVariantivEXT)(GLuint id, const GLint *addr);
extern void (APIENTRY *gglVariantfvEXT)(GLuint id, const GLfloat *addr);
extern void (APIENTRY *gglVariantdvEXT)(GLuint id, const GLdouble *addr);
extern void (APIENTRY *gglVariantubvEXT)(GLuint id, const GLubyte *addr);
extern void (APIENTRY *gglVariantusvEXT)(GLuint id, const GLushort *addr);
extern void (APIENTRY *gglVariantuivEXT)(GLuint id, const GLuint *addr);
extern void (APIENTRY *gglVariantPointerEXT)(GLuint id, GLenum type, GLuint stride, const void *addr);
extern void (APIENTRY *gglEnableVariantClientStateEXT)(GLuint id);
extern void (APIENTRY *gglDisableVariantClientStateEXT)(GLuint id);
extern GLuint (APIENTRY *gglBindLightParameterEXT)(GLenum light, GLenum value);
extern GLuint (APIENTRY *gglBindMaterialParameterEXT)(GLenum face, GLenum value);
extern GLuint (APIENTRY *gglBindTexGenParameterEXT)(GLenum unit, GLenum coord, GLenum value);
extern GLuint (APIENTRY *gglBindTextureUnitParameterEXT)(GLenum unit, GLenum value);
extern GLuint (APIENTRY *gglBindParameterEXT)(GLenum value);
extern GLboolean (APIENTRY *gglIsVariantEnabledEXT)(GLuint id, GLenum cap);
extern void (APIENTRY *gglGetVariantBooleanvEXT)(GLuint id, GLenum value, GLboolean *data);
extern void (APIENTRY *gglGetVariantIntegervEXT)(GLuint id, GLenum value, GLint *data);
extern void (APIENTRY *gglGetVariantFloatvEXT)(GLuint id, GLenum value, GLfloat *data);
extern void (APIENTRY *gglGetVariantPointervEXT)(GLuint id, GLenum value, GLvoid* *data);
extern void (APIENTRY *gglGetInvariantBooleanvEXT)(GLuint id, GLenum value, GLboolean *data);
extern void (APIENTRY *gglGetInvariantIntegervEXT)(GLuint id, GLenum value, GLint *data);
extern void (APIENTRY *gglGetInvariantFloatvEXT)(GLuint id, GLenum value, GLfloat *data);
extern void (APIENTRY *gglGetLocalConstantBooleanvEXT)(GLuint id, GLenum value, GLboolean *data);
extern void (APIENTRY *gglGetLocalConstantIntegervEXT)(GLuint id, GLenum value, GLint *data);
extern void (APIENTRY *gglGetLocalConstantFloatvEXT)(GLuint id, GLenum value, GLfloat *data);

//GL_ATI_vertex_streams
extern void (APIENTRY *gglVertexStream1sATI)(GLenum stream, GLshort x);
extern void (APIENTRY *gglVertexStream1svATI)(GLenum stream, const GLshort *coords);
extern void (APIENTRY *gglVertexStream1iATI)(GLenum stream, GLint x);
extern void (APIENTRY *gglVertexStream1ivATI)(GLenum stream, const GLint *coords);
extern void (APIENTRY *gglVertexStream1fATI)(GLenum stream, GLfloat x);
extern void (APIENTRY *gglVertexStream1fvATI)(GLenum stream, const GLfloat *coords);
extern void (APIENTRY *gglVertexStream1dATI)(GLenum stream, GLdouble x);
extern void (APIENTRY *gglVertexStream1dvATI)(GLenum stream, const GLdouble *coords);
extern void (APIENTRY *gglVertexStream2sATI)(GLenum stream, GLshort x, GLshort y);
extern void (APIENTRY *gglVertexStream2svATI)(GLenum stream, const GLshort *coords);
extern void (APIENTRY *gglVertexStream2iATI)(GLenum stream, GLint x, GLint y);
extern void (APIENTRY *gglVertexStream2ivATI)(GLenum stream, const GLint *coords);
extern void (APIENTRY *gglVertexStream2fATI)(GLenum stream, GLfloat x, GLfloat y);
extern void (APIENTRY *gglVertexStream2fvATI)(GLenum stream, const GLfloat *coords);
extern void (APIENTRY *gglVertexStream2dATI)(GLenum stream, GLdouble x, GLdouble y);
extern void (APIENTRY *gglVertexStream2dvATI)(GLenum stream, const GLdouble *coords);
extern void (APIENTRY *gglVertexStream3sATI)(GLenum stream, GLshort x, GLshort y, GLshort z);
extern void (APIENTRY *gglVertexStream3svATI)(GLenum stream, const GLshort *coords);
extern void (APIENTRY *gglVertexStream3iATI)(GLenum stream, GLint x, GLint y, GLint z);
extern void (APIENTRY *gglVertexStream3ivATI)(GLenum stream, const GLint *coords);
extern void (APIENTRY *gglVertexStream3fATI)(GLenum stream, GLfloat x, GLfloat y, GLfloat z);
extern void (APIENTRY *gglVertexStream3fvATI)(GLenum stream, const GLfloat *coords);
extern void (APIENTRY *gglVertexStream3dATI)(GLenum stream, GLdouble x, GLdouble y, GLdouble z);
extern void (APIENTRY *gglVertexStream3dvATI)(GLenum stream, const GLdouble *coords);
extern void (APIENTRY *gglVertexStream4sATI)(GLenum stream, GLshort x, GLshort y, GLshort z, GLshort w);
extern void (APIENTRY *gglVertexStream4svATI)(GLenum stream, const GLshort *coords);
extern void (APIENTRY *gglVertexStream4iATI)(GLenum stream, GLint x, GLint y, GLint z, GLint w);
extern void (APIENTRY *gglVertexStream4ivATI)(GLenum stream, const GLint *coords);
extern void (APIENTRY *gglVertexStream4fATI)(GLenum stream, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern void (APIENTRY *gglVertexStream4fvATI)(GLenum stream, const GLfloat *coords);
extern void (APIENTRY *gglVertexStream4dATI)(GLenum stream, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern void (APIENTRY *gglVertexStream4dvATI)(GLenum stream, const GLdouble *coords);
extern void (APIENTRY *gglNormalStream3bATI)(GLenum stream, GLbyte nx, GLbyte ny, GLbyte nz);
extern void (APIENTRY *gglNormalStream3bvATI)(GLenum stream, const GLbyte *coords);
extern void (APIENTRY *gglNormalStream3sATI)(GLenum stream, GLshort nx, GLshort ny, GLshort nz);
extern void (APIENTRY *gglNormalStream3svATI)(GLenum stream, const GLshort *coords);
extern void (APIENTRY *gglNormalStream3iATI)(GLenum stream, GLint nx, GLint ny, GLint nz);
extern void (APIENTRY *gglNormalStream3ivATI)(GLenum stream, const GLint *coords);
extern void (APIENTRY *gglNormalStream3fATI)(GLenum stream, GLfloat nx, GLfloat ny, GLfloat nz);
extern void (APIENTRY *gglNormalStream3fvATI)(GLenum stream, const GLfloat *coords);
extern void (APIENTRY *gglNormalStream3dATI)(GLenum stream, GLdouble nx, GLdouble ny, GLdouble nz);
extern void (APIENTRY *gglNormalStream3dvATI)(GLenum stream, const GLdouble *coords);
extern void (APIENTRY *gglClientActiveVertexStreamATI)(GLenum stream);
extern void (APIENTRY *gglVertexBlendEnviATI)(GLenum pname, GLint param);
extern void (APIENTRY *gglVertexBlendEnvfATI)(GLenum pname, GLfloat param);

//GL_ATI_element_array
extern void (APIENTRY *gglElementPointerATI)(GLenum type, const GLvoid *pointer);
extern void (APIENTRY *gglDrawElementArrayATI)(GLenum mode, GLsizei count);
extern void (APIENTRY *gglDrawRangeElementArrayATI)(GLenum mode, GLuint start, GLuint end, GLsizei count);

//GL_SUN_mesh_array
extern void (APIENTRY *gglDrawMeshArraysSUN)(GLenum mode, GLint first, GLsizei count, GLsizei width);

//GL_NV_occlusion_query
extern void (APIENTRY *gglGenOcclusionQueriesNV)(GLsizei n, GLuint *ids);
extern void (APIENTRY *gglDeleteOcclusionQueriesNV)(GLsizei n, const GLuint *ids);
extern GLboolean (APIENTRY *gglIsOcclusionQueryNV)(GLuint id);
extern void (APIENTRY *gglBeginOcclusionQueryNV)(GLuint id);
extern void (APIENTRY *gglEndOcclusionQueryNV)(void);
extern void (APIENTRY *gglGetOcclusionQueryivNV)(GLuint id, GLenum pname, GLint *params);
extern void (APIENTRY *gglGetOcclusionQueryuivNV)(GLuint id, GLenum pname, GLuint *params);

//GL_NV_point_sprite
extern void (APIENTRY *gglPointParameteriNV)(GLenum pname, GLint param);
extern void (APIENTRY *gglPointParameterivNV)(GLenum pname, const GLint *params);

//GL_EXT_stencil_two_side
extern void (APIENTRY *gglActiveStencilFaceEXT)(GLenum face);

//GL_APPLE_element_array
extern void (APIENTRY *gglElementPointerAPPLE)(GLenum type, const GLvoid *pointer);
extern void (APIENTRY *gglDrawElementArrayAPPLE)(GLenum mode, GLint first, GLsizei count);
extern void (APIENTRY *gglDrawRangeElementArrayAPPLE)(GLenum mode, GLuint start, GLuint end, GLint first, GLsizei count);
extern void (APIENTRY *gglMultiDrawElementArrayAPPLE)(GLenum mode, const GLint *first, const GLsizei *count, GLsizei primcount);
extern void (APIENTRY *gglMultiDrawRangeElementArrayAPPLE)(GLenum mode, GLuint start, GLuint end, const GLint *first, const GLsizei *count, GLsizei primcount);

//GL_APPLE_fence
extern void (APIENTRY *gglGenFencesAPPLE)(GLsizei n, GLuint *fences);
extern void (APIENTRY *gglDeleteFencesAPPLE)(GLsizei n, const GLuint *fences);
extern void (APIENTRY *gglSetFenceAPPLE)(GLuint fence);
extern GLboolean (APIENTRY *gglIsFenceAPPLE)(GLuint fence);
extern GLboolean (APIENTRY *gglTestFenceAPPLE)(GLuint fence);
extern void (APIENTRY *gglFinishFenceAPPLE)(GLuint fence);
extern GLboolean (APIENTRY *gglTestObjectAPPLE)(GLenum object, GLuint name);
extern void (APIENTRY *gglFinishObjectAPPLE)(GLenum object, GLint name);

//GL_APPLE_vertex_array_object
extern void (APIENTRY *gglBindVertexArrayAPPLE)(GLuint array);
extern void (APIENTRY *gglDeleteVertexArraysAPPLE)(GLsizei n, const GLuint *arrays);
extern void (APIENTRY *gglGenVertexArraysAPPLE)(GLsizei n, const GLuint *arrays);
extern GLboolean (APIENTRY *gglIsVertexArrayAPPLE)(GLuint array);

//GL_APPLE_vertex_array_range
extern void (APIENTRY *gglVertexArrayRangeAPPLE)(GLsizei length, GLvoid *pointer);
extern void (APIENTRY *gglFlushVertexArrayRangeAPPLE)(GLsizei length, GLvoid *pointer);
extern void (APIENTRY *gglVertexArrayParameteriAPPLE)(GLenum pname, GLint param);

//GL_ATI_draw_buffers
extern void (APIENTRY *gglDrawBuffersATI)(GLsizei n, const GLenum *bufs);

//GL_NV_fragment_program
extern void (APIENTRY *gglProgramNamedParameter4fNV)(GLuint id, GLsizei len, const GLubyte *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern void (APIENTRY *gglProgramNamedParameter4dNV)(GLuint id, GLsizei len, const GLubyte *name, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern void (APIENTRY *gglProgramNamedParameter4fvNV)(GLuint id, GLsizei len, const GLubyte *name, const GLfloat *v);
extern void (APIENTRY *gglProgramNamedParameter4dvNV)(GLuint id, GLsizei len, const GLubyte *name, const GLdouble *v);
extern void (APIENTRY *gglGetProgramNamedParameterfvNV)(GLuint id, GLsizei len, const GLubyte *name, GLfloat *params);
extern void (APIENTRY *gglGetProgramNamedParameterdvNV)(GLuint id, GLsizei len, const GLubyte *name, GLdouble *params);

//GL_NV_half_float
extern void (APIENTRY *gglVertex2hNV)(GLhalfNV x, GLhalfNV y);
extern void (APIENTRY *gglVertex2hvNV)(const GLhalfNV *v);
extern void (APIENTRY *gglVertex3hNV)(GLhalfNV x, GLhalfNV y, GLhalfNV z);
extern void (APIENTRY *gglVertex3hvNV)(const GLhalfNV *v);
extern void (APIENTRY *gglVertex4hNV)(GLhalfNV x, GLhalfNV y, GLhalfNV z, GLhalfNV w);
extern void (APIENTRY *gglVertex4hvNV)(const GLhalfNV *v);
extern void (APIENTRY *gglNormal3hNV)(GLhalfNV nx, GLhalfNV ny, GLhalfNV nz);
extern void (APIENTRY *gglNormal3hvNV)(const GLhalfNV *v);
extern void (APIENTRY *gglColor3hNV)(GLhalfNV red, GLhalfNV green, GLhalfNV blue);
extern void (APIENTRY *gglColor3hvNV)(const GLhalfNV *v);
extern void (APIENTRY *gglColor4hNV)(GLhalfNV red, GLhalfNV green, GLhalfNV blue, GLhalfNV alpha);
extern void (APIENTRY *gglColor4hvNV)(const GLhalfNV *v);
extern void (APIENTRY *gglTexCoord1hNV)(GLhalfNV s);
extern void (APIENTRY *gglTexCoord1hvNV)(const GLhalfNV *v);
extern void (APIENTRY *gglTexCoord2hNV)(GLhalfNV s, GLhalfNV t);
extern void (APIENTRY *gglTexCoord2hvNV)(const GLhalfNV *v);
extern void (APIENTRY *gglTexCoord3hNV)(GLhalfNV s, GLhalfNV t, GLhalfNV r);
extern void (APIENTRY *gglTexCoord3hvNV)(const GLhalfNV *v);
extern void (APIENTRY *gglTexCoord4hNV)(GLhalfNV s, GLhalfNV t, GLhalfNV r, GLhalfNV q);
extern void (APIENTRY *gglTexCoord4hvNV)(const GLhalfNV *v);
extern void (APIENTRY *gglMultiTexCoord1hNV)(GLenum target, GLhalfNV s);
extern void (APIENTRY *gglMultiTexCoord1hvNV)(GLenum target, const GLhalfNV *v);
extern void (APIENTRY *gglMultiTexCoord2hNV)(GLenum target, GLhalfNV s, GLhalfNV t);
extern void (APIENTRY *gglMultiTexCoord2hvNV)(GLenum target, const GLhalfNV *v);
extern void (APIENTRY *gglMultiTexCoord3hNV)(GLenum target, GLhalfNV s, GLhalfNV t, GLhalfNV r);
extern void (APIENTRY *gglMultiTexCoord3hvNV)(GLenum target, const GLhalfNV *v);
extern void (APIENTRY *gglMultiTexCoord4hNV)(GLenum target, GLhalfNV s, GLhalfNV t, GLhalfNV r, GLhalfNV q);
extern void (APIENTRY *gglMultiTexCoord4hvNV)(GLenum target, const GLhalfNV *v);
extern void (APIENTRY *gglFogCoordhNV)(GLhalfNV fog);
extern void (APIENTRY *gglFogCoordhvNV)(const GLhalfNV *fog);
extern void (APIENTRY *gglSecondaryColor3hNV)(GLhalfNV red, GLhalfNV green, GLhalfNV blue);
extern void (APIENTRY *gglSecondaryColor3hvNV)(const GLhalfNV *v);
extern void (APIENTRY *gglVertexWeighthNV)(GLhalfNV weight);
extern void (APIENTRY *gglVertexWeighthvNV)(const GLhalfNV *weight);
extern void (APIENTRY *gglVertexAttrib1hNV)(GLuint index, GLhalfNV x);
extern void (APIENTRY *gglVertexAttrib1hvNV)(GLuint index, const GLhalfNV *v);
extern void (APIENTRY *gglVertexAttrib2hNV)(GLuint index, GLhalfNV x, GLhalfNV y);
extern void (APIENTRY *gglVertexAttrib2hvNV)(GLuint index, const GLhalfNV *v);
extern void (APIENTRY *gglVertexAttrib3hNV)(GLuint index, GLhalfNV x, GLhalfNV y, GLhalfNV z);
extern void (APIENTRY *gglVertexAttrib3hvNV)(GLuint index, const GLhalfNV *v);
extern void (APIENTRY *gglVertexAttrib4hNV)(GLuint index, GLhalfNV x, GLhalfNV y, GLhalfNV z, GLhalfNV w);
extern void (APIENTRY *gglVertexAttrib4hvNV)(GLuint index, const GLhalfNV *v);
extern void (APIENTRY *gglVertexAttribs1hvNV)(GLuint index, GLsizei n, const GLhalfNV *v);
extern void (APIENTRY *gglVertexAttribs2hvNV)(GLuint index, GLsizei n, const GLhalfNV *v);
extern void (APIENTRY *gglVertexAttribs3hvNV)(GLuint index, GLsizei n, const GLhalfNV *v);
extern void (APIENTRY *gglVertexAttribs4hvNV)(GLuint index, GLsizei n, const GLhalfNV *v);

//GL_NV_pixel_data_range
extern void (APIENTRY *gglPixelDataRangeNV)(GLenum target, GLsizei length, GLvoid *pointer);
extern void (APIENTRY *gglFlushPixelDataRangeNV)(GLenum target);

//GL_NV_primitive_restart
extern void (APIENTRY *gglPrimitiveRestartNV)(void);
extern void (APIENTRY *gglPrimitiveRestartIndexNV)(GLuint index);

//GL_ATI_map_object_buffer
extern GLvoid* (APIENTRY *gglMapObjectBufferATI)(GLuint buffer);
extern void (APIENTRY *gglUnmapObjectBufferATI)(GLuint buffer);

//GL_ATI_separate_stencil
extern void (APIENTRY *gglStencilOpSeparateATI)(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
extern void (APIENTRY *gglStencilFuncSeparateATI)(GLenum frontfunc, GLenum backfunc, GLint ref, GLuint mask);

//GL_ATI_vertex_attrib_array_object
extern void (APIENTRY *gglVertexAttribArrayObjectATI)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLuint buffer, GLuint offset);
extern void (APIENTRY *gglGetVertexAttribArrayObjectfvATI)(GLuint index, GLenum pname, GLfloat *params);
extern void (APIENTRY *gglGetVertexAttribArrayObjectivATI)(GLuint index, GLenum pname, GLint *params);

//GL_OES_fixed_point
extern void (APIENTRY *gglAlphaFuncxOES)(GLenum func, GLclampx ref);
extern void (APIENTRY *gglClearColorxOES)(GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha);
extern void (APIENTRY *gglClearDepthxOES)(GLclampx depth);
extern void (APIENTRY *gglClipPlanexOES)(GLenum plane, const GLfixed *equation);
extern void (APIENTRY *gglColor4xOES)(GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha);
extern void (APIENTRY *gglDepthRangexOES)(GLclampx zNear, GLclampx zFar);
extern void (APIENTRY *gglFogxOES)(GLenum pname, GLfixed param);
extern void (APIENTRY *gglFogxvOES)(GLenum pname, const GLfixed *params);
extern void (APIENTRY *gglFrustumxOES)(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar);
extern void (APIENTRY *gglGetClipPlanexOES)(GLenum pname, GLfixed eqn[4]);
extern void (APIENTRY *gglGetFixedvOES)(GLenum pname, GLfixed *params);
extern void (APIENTRY *gglGetLightxvOES)(GLenum light, GLenum pname, GLfixed *params);
extern void (APIENTRY *gglGetMaterialxvOES)(GLenum face, GLenum pname, GLfixed *params);
extern void (APIENTRY *gglGetTexEnvxvOES)(GLenum env, GLenum pname, GLfixed *params);
extern void (APIENTRY *gglGetTexParameterxvOES)(GLenum target, GLenum pname, GLfixed *params);
extern void (APIENTRY *gglLightModelxOES)(GLenum pname, GLfixed param);
extern void (APIENTRY *gglLightModelxvOES)(GLenum pname, const GLfixed *params);
extern void (APIENTRY *gglLightxOES)(GLenum light, GLenum pname, GLfixed param);
extern void (APIENTRY *gglLightxvOES)(GLenum light, GLenum pname, const GLfixed *params);
extern void (APIENTRY *gglLineWidthxOES)(GLfixed width);
extern void (APIENTRY *gglLoadMatrixxOES)(const GLfixed *m);
extern void (APIENTRY *gglMaterialxOES)(GLenum face, GLenum pname, GLfixed param);
extern void (APIENTRY *gglMaterialxvOES)(GLenum face, GLenum pname, const GLfixed *params);
extern void (APIENTRY *gglMultMatrixxOES)(const GLfixed *m);
extern void (APIENTRY *gglMultiTexCoord4xOES)(GLenum target, GLfixed s, GLfixed t, GLfixed r, GLfixed q);
extern void (APIENTRY *gglNormal3xOES)(GLfixed nx, GLfixed ny, GLfixed nz);
extern void (APIENTRY *gglOrthoxOES)(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar);
extern void (APIENTRY *gglPointParameterxOES)(GLenum pname, GLfixed param);
extern void (APIENTRY *gglPointParameterxvOES)(GLenum pname, const GLfixed *params);
extern void (APIENTRY *gglPointSizexOES)(GLfixed size);
extern void (APIENTRY *gglPolygonOffsetxOES)(GLfixed factor, GLfixed units);
extern void (APIENTRY *gglRotatexOES)(GLfixed angle, GLfixed x, GLfixed y, GLfixed z);
extern void (APIENTRY *gglSampleCoveragexOES)(GLclampx value, GLboolean invert);
extern void (APIENTRY *gglScalexOES)(GLfixed x, GLfixed y, GLfixed z);
extern void (APIENTRY *gglTexEnvxOES)(GLenum target, GLenum pname, GLfixed param);
extern void (APIENTRY *gglTexEnvxvOES)(GLenum target, GLenum pname, const GLfixed *params);
extern void (APIENTRY *gglTexParameterxOES)(GLenum target, GLenum pname, GLfixed param);
extern void (APIENTRY *gglTexParameterxvOES)(GLenum target, GLenum pname, const GLfixed *params);
extern void (APIENTRY *gglTranslatexOES)(GLfixed x, GLfixed y, GLfixed z);

//GL_OES_single_precision
extern void (APIENTRY *gglDepthRangefOES)(GLclampf zNear, GLclampf zFar);
extern void (APIENTRY *gglFrustumfOES)(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);
extern void (APIENTRY *gglOrthofOES)(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);
extern void (APIENTRY *gglClipPlanefOES)(GLenum plane, const GLfloat *equation);
extern void (APIENTRY *gglGetClipPlanefOES)(GLenum pname, GLfloat eqn[4]);
extern void (APIENTRY *gglClearDepthfOES)(GLclampf depth);

//GL_OES_query_matrix
extern GLbitfield (APIENTRY *gglQueryMatrixxOES)(GLfixed mantissa[16], GLint exponent[16]);

//GL_EXT_depth_bounds_test
extern void (APIENTRY *gglDepthBoundsEXT)(GLclampd zmin, GLclampd zmax);

//GL_EXT_blend_equation_separate
extern void (APIENTRY *gglBlendEquationSeparateEXT)(GLenum modeRGB, GLenum modeAlpha);

//GL_EXT_framebuffer_object
extern GLboolean (APIENTRY *gglIsRenderbufferEXT)(GLuint renderbuffer);
extern void (APIENTRY *gglBindRenderbufferEXT)(GLenum target, GLuint renderbuffer);
extern void (APIENTRY *gglDeleteRenderbuffersEXT)(GLsizei n, const GLuint *renderbuffers);
extern void (APIENTRY *gglGenRenderbuffersEXT)(GLsizei n, GLuint *renderbuffers);
extern void (APIENTRY *gglRenderbufferStorageEXT)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
extern void (APIENTRY *gglGetRenderbufferParameterivEXT)(GLenum target, GLenum pname, GLint *params);
extern GLboolean (APIENTRY *gglIsFramebufferEXT)(GLuint framebuffer);
extern void (APIENTRY *gglBindFramebufferEXT)(GLenum target, GLuint framebuffer);
extern void (APIENTRY *gglDeleteFramebuffersEXT)(GLsizei n, const GLuint *framebuffers);
extern void (APIENTRY *gglGenFramebuffersEXT)(GLsizei n, GLuint *framebuffers);
extern GLenum (APIENTRY *gglCheckFramebufferStatusEXT)(GLenum target);
extern void (APIENTRY *gglFramebufferTexture1DEXT)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
extern void (APIENTRY *gglFramebufferTexture2DEXT)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
extern void (APIENTRY *gglFramebufferTexture3DEXT)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
extern void (APIENTRY *gglFramebufferRenderbufferEXT)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
extern void (APIENTRY *gglGetFramebufferAttachmentParameterivEXT)(GLenum target, GLenum attachment, GLenum pname, GLint *params);
extern void (APIENTRY *gglGenerateMipmapEXT)(GLenum target);

//GL_GREMEDY_string_marker
extern void (APIENTRY *gglStringMarkerGREMEDY)(GLsizei len, const GLvoid *string);

//GL_EXT_stencil_clear_tag
extern void (APIENTRY *gglStencilClearTagEXT)(GLsizei stencilTagBits, GLuint stencilClearTag);

//GL_EXT_framebuffer_blit
extern void (APIENTRY *gglBlitFramebufferEXT)(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);

//GL_EXT_framebuffer_multisample
extern void (APIENTRY *gglRenderbufferStorageMultisampleEXT)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);

//GL_KTX_buffer_region
extern GLuint (APIENTRY *gglBufferRegionEnabledEXT)(void);
extern void (APIENTRY *gglDeleteBufferRegionEXT)(GLenum region);
extern void (APIENTRY *gglDrawBufferRegionEXT)(GLuint region, GLint x, GLint y, GLsizei width, GLsizei height, GLint xDest, GLint yDest);
extern GLuint (APIENTRY *gglNewBufferRegionEXT)(GLenum region);
extern void (APIENTRY *gglReadBufferRegionEXT)(GLuint region, GLint x, GLint y, GLsizei width, GLsizei height);

//GL_SUN_read_video_pixels
extern void (APIENTRY *gglReadVideoPixelsSUN)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid* pixels);

//GL_WIN_swap_hint
extern void (APIENTRY *gglAddSwapHintRectWIN)(GLint x, GLint y, GLsizei width, GLsizei height);

//=====================================
//
// OPENGL FUNCTIONS
//
//=====================================

//GL_VERSION_1_1
extern GLboolean (APIENTRY *gglAreTexturesResident)(GLsizei n, const GLuint *textures, GLboolean *residences);
extern void (APIENTRY *gglArrayElement)(GLint i);
extern void (APIENTRY *gglBindTexture)(GLenum target, GLuint texture);
extern void (APIENTRY *gglColorPointer)(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
extern void (APIENTRY *gglCopyTexImage1D)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
extern void (APIENTRY *gglCopyTexImage2D)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
extern void (APIENTRY *gglCopyTexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
extern void (APIENTRY *gglCopyTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
extern void (APIENTRY *gglDeleteTextures)(GLsizei n, const GLuint *textures);
extern void (APIENTRY *gglDisableClientState)(GLenum array);
extern void (APIENTRY *gglDrawArrays)(GLenum mode, GLint first, GLsizei count);
extern void (APIENTRY *gglDrawElements)(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
extern void (APIENTRY *gglEdgeFlagPointer)(GLsizei stride, const GLboolean *pointer);
extern void (APIENTRY *gglEnableClientState)(GLenum array);
extern void (APIENTRY *gglGenTextures)(GLsizei n, GLuint *textures);
extern void (APIENTRY *gglGetPointerv)(GLenum pname, GLvoid* *params);
extern void (APIENTRY *gglIndexub)(GLubyte c);
extern void (APIENTRY *gglIndexubv)(const GLubyte *c);
extern void (APIENTRY *gglIndexPointer)(GLenum type, GLsizei stride, const GLvoid *pointer);
extern void (APIENTRY *gglInterleavedArrays)(GLenum format, GLsizei stride, const GLvoid *pointer);
extern GLboolean (APIENTRY *gglIsTexture)(GLuint texture);
extern void (APIENTRY *gglNormalPointer)(GLenum type, GLsizei stride, const GLvoid *pointer);
extern void (APIENTRY *gglPolygonOffset)(GLfloat factor, GLfloat units);
extern void (APIENTRY *gglPopClientAttrib)(void);
extern void (APIENTRY *gglPrioritizeTextures)(GLsizei n, const GLuint *textures, const GLclampf *priorities);
extern void (APIENTRY *gglPushClientAttrib)(GLbitfield mask);
extern void (APIENTRY *gglTexCoordPointer)(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
extern void (APIENTRY *gglTexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels);
extern void (APIENTRY *gglTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
extern void (APIENTRY *gglVertexPointer)(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);

//GL_VERSION_1_2
extern void (APIENTRY *gglDrawRangeElements)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices);
extern void (APIENTRY *gglTexImage3D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
extern void (APIENTRY *gglTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels);
extern void (APIENTRY *gglCopyTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);

//GL_ARB_imaging
extern void (APIENTRY *gglBlendColor)(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
extern void (APIENTRY *gglBlendEquation)(GLenum mode);
extern void (APIENTRY *gglColorTable)(GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *table);
extern void (APIENTRY *gglColorTableParameterfv)(GLenum target, GLenum pname, const GLfloat *params);
extern void (APIENTRY *gglColorTableParameteriv)(GLenum target, GLenum pname, const GLint *params);
extern void (APIENTRY *gglCopyColorTable)(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
extern void (APIENTRY *gglGetColorTable)(GLenum target, GLenum format, GLenum type, GLvoid *table);
extern void (APIENTRY *gglGetColorTableParameterfv)(GLenum target, GLenum pname, GLfloat *params);
extern void (APIENTRY *gglGetColorTableParameteriv)(GLenum target, GLenum pname, GLint *params);
extern void (APIENTRY *gglColorSubTable)(GLenum target, GLsizei start, GLsizei count, GLenum format, GLenum type, const GLvoid *data);
extern void (APIENTRY *gglCopyColorSubTable)(GLenum target, GLsizei start, GLint x, GLint y, GLsizei width);
extern void (APIENTRY *gglConvolutionFilter1D)(GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *image);
extern void (APIENTRY *gglConvolutionFilter2D)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *image);
extern void (APIENTRY *gglConvolutionParameterf)(GLenum target, GLenum pname, GLfloat params);
extern void (APIENTRY *gglConvolutionParameterfv)(GLenum target, GLenum pname, const GLfloat *params);
extern void (APIENTRY *gglConvolutionParameteri)(GLenum target, GLenum pname, GLint params);
extern void (APIENTRY *gglConvolutionParameteriv)(GLenum target, GLenum pname, const GLint *params);
extern void (APIENTRY *gglCopyConvolutionFilter1D)(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
extern void (APIENTRY *gglCopyConvolutionFilter2D)(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height);
extern void (APIENTRY *gglGetConvolutionFilter)(GLenum target, GLenum format, GLenum type, GLvoid *image);
extern void (APIENTRY *gglGetConvolutionParameterfv)(GLenum target, GLenum pname, GLfloat *params);
extern void (APIENTRY *gglGetConvolutionParameteriv)(GLenum target, GLenum pname, GLint *params);
extern void (APIENTRY *gglGetSeparableFilter)(GLenum target, GLenum format, GLenum type, GLvoid *row, GLvoid *column, GLvoid *span);
extern void (APIENTRY *gglSeparableFilter2D)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *row, const GLvoid *column);
extern void (APIENTRY *gglGetHistogram)(GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid *values);
extern void (APIENTRY *gglGetHistogramParameterfv)(GLenum target, GLenum pname, GLfloat *params);
extern void (APIENTRY *gglGetHistogramParameteriv)(GLenum target, GLenum pname, GLint *params);
extern void (APIENTRY *gglGetMinmax)(GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid *values);
extern void (APIENTRY *gglGetMinmaxParameterfv)(GLenum target, GLenum pname, GLfloat *params);
extern void (APIENTRY *gglGetMinmaxParameteriv)(GLenum target, GLenum pname, GLint *params);
extern void (APIENTRY *gglHistogram)(GLenum target, GLsizei width, GLenum internalformat, GLboolean sink);
extern void (APIENTRY *gglMinmax)(GLenum target, GLenum internalformat, GLboolean sink);
extern void (APIENTRY *gglResetHistogram)(GLenum target);
extern void (APIENTRY *gglResetMinmax)(GLenum target);

//GL_VERSION_1_3
extern void (APIENTRY *gglActiveTexture)(GLenum texture);
extern void (APIENTRY *gglClientActiveTexture)(GLenum texture);
extern void (APIENTRY *gglMultiTexCoord1d)(GLenum target, GLdouble s);
extern void (APIENTRY *gglMultiTexCoord1dv)(GLenum target, const GLdouble *v);
extern void (APIENTRY *gglMultiTexCoord1f)(GLenum target, GLfloat s);
extern void (APIENTRY *gglMultiTexCoord1fv)(GLenum target, const GLfloat *v);
extern void (APIENTRY *gglMultiTexCoord1i)(GLenum target, GLint s);
extern void (APIENTRY *gglMultiTexCoord1iv)(GLenum target, const GLint *v);
extern void (APIENTRY *gglMultiTexCoord1s)(GLenum target, GLshort s);
extern void (APIENTRY *gglMultiTexCoord1sv)(GLenum target, const GLshort *v);
extern void (APIENTRY *gglMultiTexCoord2d)(GLenum target, GLdouble s, GLdouble t);
extern void (APIENTRY *gglMultiTexCoord2dv)(GLenum target, const GLdouble *v);
extern void (APIENTRY *gglMultiTexCoord2f)(GLenum target, GLfloat s, GLfloat t);
extern void (APIENTRY *gglMultiTexCoord2fv)(GLenum target, const GLfloat *v);
extern void (APIENTRY *gglMultiTexCoord2i)(GLenum target, GLint s, GLint t);
extern void (APIENTRY *gglMultiTexCoord2iv)(GLenum target, const GLint *v);
extern void (APIENTRY *gglMultiTexCoord2s)(GLenum target, GLshort s, GLshort t);
extern void (APIENTRY *gglMultiTexCoord2sv)(GLenum target, const GLshort *v);
extern void (APIENTRY *gglMultiTexCoord3d)(GLenum target, GLdouble s, GLdouble t, GLdouble r);
extern void (APIENTRY *gglMultiTexCoord3dv)(GLenum target, const GLdouble *v);
extern void (APIENTRY *gglMultiTexCoord3f)(GLenum target, GLfloat s, GLfloat t, GLfloat r);
extern void (APIENTRY *gglMultiTexCoord3fv)(GLenum target, const GLfloat *v);
extern void (APIENTRY *gglMultiTexCoord3i)(GLenum target, GLint s, GLint t, GLint r);
extern void (APIENTRY *gglMultiTexCoord3iv)(GLenum target, const GLint *v);
extern void (APIENTRY *gglMultiTexCoord3s)(GLenum target, GLshort s, GLshort t, GLshort r);
extern void (APIENTRY *gglMultiTexCoord3sv)(GLenum target, const GLshort *v);
extern void (APIENTRY *gglMultiTexCoord4d)(GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q);
extern void (APIENTRY *gglMultiTexCoord4dv)(GLenum target, const GLdouble *v);
extern void (APIENTRY *gglMultiTexCoord4f)(GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
extern void (APIENTRY *gglMultiTexCoord4fv)(GLenum target, const GLfloat *v);
extern void (APIENTRY *gglMultiTexCoord4i)(GLenum target, GLint s, GLint t, GLint r, GLint q);
extern void (APIENTRY *gglMultiTexCoord4iv)(GLenum target, const GLint *v);
extern void (APIENTRY *gglMultiTexCoord4s)(GLenum target, GLshort s, GLshort t, GLshort r, GLshort q);
extern void (APIENTRY *gglMultiTexCoord4sv)(GLenum target, const GLshort *v);
extern void (APIENTRY *gglLoadTransposeMatrixf)(const GLfloat *m);
extern void (APIENTRY *gglLoadTransposeMatrixd)(const GLdouble *m);
extern void (APIENTRY *gglMultTransposeMatrixf)(const GLfloat *m);
extern void (APIENTRY *gglMultTransposeMatrixd)(const GLdouble *m);
extern void (APIENTRY *gglSampleCoverage)(GLclampf value, GLboolean invert);
extern void (APIENTRY *gglCompressedTexImage3D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid *data);
extern void (APIENTRY *gglCompressedTexImage2D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data);
extern void (APIENTRY *gglCompressedTexImage1D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid *data);
extern void (APIENTRY *gglCompressedTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid *data);
extern void (APIENTRY *gglCompressedTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data);
extern void (APIENTRY *gglCompressedTexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid *data);
extern void (APIENTRY *gglGetCompressedTexImage)(GLenum target, GLint level, void *img);

//GL_VERSION_1_4
extern void (APIENTRY *gglBlendFuncSeparate)(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
extern void (APIENTRY *gglFogCoordf)(GLfloat coord);
extern void (APIENTRY *gglFogCoordfv)(const GLfloat *coord);
extern void (APIENTRY *gglFogCoordd)(GLdouble coord);
extern void (APIENTRY *gglFogCoorddv)(const GLdouble *coord);
extern void (APIENTRY *gglFogCoordPointer)(GLenum type, GLsizei stride, const GLvoid *pointer);
extern void (APIENTRY *gglMultiDrawArrays)(GLenum mode, GLint *first, GLsizei *count, GLsizei primcount);
extern void (APIENTRY *gglMultiDrawElements)(GLenum mode, const GLsizei *count, GLenum type, const GLvoid **indices, GLsizei primcount);
extern void (APIENTRY *gglPointParameterf)(GLenum pname, GLfloat param);
extern void (APIENTRY *gglPointParameterfv)(GLenum pname, const GLfloat *params);
extern void (APIENTRY *gglPointParameteri)(GLenum pname, GLint param);
extern void (APIENTRY *gglPointParameteriv)(GLenum pname, const GLint *params);
extern void (APIENTRY *gglSecondaryColor3b)(GLbyte red, GLbyte green, GLbyte blue);
extern void (APIENTRY *gglSecondaryColor3bv)(const GLbyte *v);
extern void (APIENTRY *gglSecondaryColor3d)(GLdouble red, GLdouble green, GLdouble blue);
extern void (APIENTRY *gglSecondaryColor3dv)(const GLdouble *v);
extern void (APIENTRY *gglSecondaryColor3f)(GLfloat red, GLfloat green, GLfloat blue);
extern void (APIENTRY *gglSecondaryColor3fv)(const GLfloat *v);
extern void (APIENTRY *gglSecondaryColor3i)(GLint red, GLint green, GLint blue);
extern void (APIENTRY *gglSecondaryColor3iv)(const GLint *v);
extern void (APIENTRY *gglSecondaryColor3s)(GLshort red, GLshort green, GLshort blue);
extern void (APIENTRY *gglSecondaryColor3sv)(const GLshort *v);
extern void (APIENTRY *gglSecondaryColor3ub)(GLubyte red, GLubyte green, GLubyte blue);
extern void (APIENTRY *gglSecondaryColor3ubv)(const GLubyte *v);
extern void (APIENTRY *gglSecondaryColor3ui)(GLuint red, GLuint green, GLuint blue);
extern void (APIENTRY *gglSecondaryColor3uiv)(const GLuint *v);
extern void (APIENTRY *gglSecondaryColor3us)(GLushort red, GLushort green, GLushort blue);
extern void (APIENTRY *gglSecondaryColor3usv)(const GLushort *v);
extern void (APIENTRY *gglSecondaryColorPointer)(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
extern void (APIENTRY *gglWindowPos2d)(GLdouble x, GLdouble y);
extern void (APIENTRY *gglWindowPos2dv)(const GLdouble *p);
extern void (APIENTRY *gglWindowPos2f)(GLfloat x, GLfloat y);
extern void (APIENTRY *gglWindowPos2fv)(const GLfloat *p);
extern void (APIENTRY *gglWindowPos2i)(GLint x, GLint y);
extern void (APIENTRY *gglWindowPos2iv)(const GLint *p);
extern void (APIENTRY *gglWindowPos2s)(GLshort x, GLshort y);
extern void (APIENTRY *gglWindowPos2sv)(const GLshort *p);
extern void (APIENTRY *gglWindowPos3d)(GLdouble x, GLdouble y, GLdouble z);
extern void (APIENTRY *gglWindowPos3dv)(const GLdouble *p);
extern void (APIENTRY *gglWindowPos3f)(GLfloat x, GLfloat y, GLfloat z);
extern void (APIENTRY *gglWindowPos3fv)(const GLfloat *p);
extern void (APIENTRY *gglWindowPos3i)(GLint x, GLint y, GLint z);
extern void (APIENTRY *gglWindowPos3iv)(const GLint *p);
extern void (APIENTRY *gglWindowPos3s)(GLshort x, GLshort y, GLshort z);
extern void (APIENTRY *gglWindowPos3sv)(const GLshort *p);

//GL_VERSION_1_5
extern void (APIENTRY *gglGenQueries)(GLsizei n, GLuint *ids);
extern void (APIENTRY *gglDeleteQueries)(GLsizei n, const GLuint *ids);
extern GLboolean (APIENTRY *gglIsQuery)(GLuint id);
extern void (APIENTRY *gglBeginQuery)(GLenum target, GLuint id);
extern void (APIENTRY *gglEndQuery)(GLenum target);
extern void (APIENTRY *gglGetQueryiv)(GLenum target, GLenum pname, GLint *params);
extern void (APIENTRY *gglGetQueryObjectiv)(GLuint id, GLenum pname, GLint *params);
extern void (APIENTRY *gglGetQueryObjectuiv)(GLuint id, GLenum pname, GLuint *params);
extern void (APIENTRY *gglBindBuffer)(GLenum target, GLuint buffer);
extern void (APIENTRY *gglDeleteBuffers)(GLsizei n, const GLuint *buffers);
extern void (APIENTRY *gglGenBuffers)(GLsizei n, GLuint *buffers);
extern GLboolean (APIENTRY *gglIsBuffer)(GLuint buffer);
extern void (APIENTRY *gglBufferData)(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
extern void (APIENTRY *gglBufferSubData)(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data);
extern void (APIENTRY *gglGetBufferSubData)(GLenum target, GLintptr offset, GLsizeiptr size, GLvoid *data);
extern GLvoid* (APIENTRY *gglMapBuffer)(GLenum target, GLenum access);
extern GLboolean (APIENTRY *gglUnmapBuffer)(GLenum target);
extern void (APIENTRY *gglGetBufferParameteriv)(GLenum target, GLenum pname, GLint *params);
extern void (APIENTRY *gglGetBufferPointerv)(GLenum target, GLenum pname, GLvoid* *params);

//GL_VERSION_2_0
extern void (APIENTRY *gglBlendEquationSeparate)(GLenum modeRGB, GLenum modeAlpha);
extern void (APIENTRY *gglDrawBuffers)(GLsizei n, const GLenum *bufs);
extern void (APIENTRY *gglStencilOpSeparate)(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
extern void (APIENTRY *gglStencilFuncSeparate)(GLenum frontfunc, GLenum backfunc, GLint ref, GLuint mask);
extern void (APIENTRY *gglStencilMaskSeparate)(GLenum face, GLuint mask);
extern void (APIENTRY *gglAttachShader)(GLuint program, GLuint shader);
extern void (APIENTRY *gglBindAttribLocation)(GLuint program, GLuint index, const GLchar *name);
extern void (APIENTRY *gglCompileShader)(GLuint shader);
extern GLuint (APIENTRY *gglCreateProgram)(void);
extern GLuint (APIENTRY *gglCreateShader)(GLenum type);
extern void (APIENTRY *gglDeleteProgram)(GLuint program);
extern void (APIENTRY *gglDeleteShader)(GLuint shader);
extern void (APIENTRY *gglDetachShader)(GLuint program, GLuint shader);
extern void (APIENTRY *gglDisableVertexAttribArray)(GLuint index);
extern void (APIENTRY *gglEnableVertexAttribArray)(GLuint index);
extern void (APIENTRY *gglGetActiveAttrib)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
extern void (APIENTRY *gglGetActiveUniform)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
extern void (APIENTRY *gglGetAttachedShaders)(GLuint program, GLsizei maxCount, GLsizei *count, GLuint *obj);
extern GLint (APIENTRY *gglGetAttribLocation)(GLuint program, const GLchar *name);
extern void (APIENTRY *gglGetProgramiv)(GLuint program, GLenum pname, GLint *params);
extern void (APIENTRY *gglGetProgramInfoLog)(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
extern void (APIENTRY *gglGetShaderiv)(GLuint shader, GLenum pname, GLint *params);
extern void (APIENTRY *gglGetShaderInfoLog)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
extern void (APIENTRY *gglGetShaderSource)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source);
extern GLint (APIENTRY *gglGetUniformLocation)(GLuint program, const GLchar *name);
extern void (APIENTRY *gglGetUniformfv)(GLuint program, GLint location, GLfloat *params);
extern void (APIENTRY *gglGetUniformiv)(GLuint program, GLint location, GLint *params);
extern void (APIENTRY *gglGetVertexAttribdv)(GLuint index, GLenum pname, GLdouble *params);
extern void (APIENTRY *gglGetVertexAttribfv)(GLuint index, GLenum pname, GLfloat *params);
extern void (APIENTRY *gglGetVertexAttribiv)(GLuint index, GLenum pname, GLint *params);
extern void (APIENTRY *gglGetVertexAttribPointerv)(GLuint index, GLenum pname, GLvoid* *pointer);
extern GLboolean (APIENTRY *gglIsProgram)(GLuint program);
extern GLboolean (APIENTRY *gglIsShader)(GLuint shader);
extern void (APIENTRY *gglLinkProgram)(GLuint program);
extern void (APIENTRY *gglShaderSource)(GLuint shader, GLsizei count, const GLchar* *string, const GLint *length);
extern void (APIENTRY *gglUseProgram)(GLuint program);
extern void (APIENTRY *gglUniform1f)(GLint location, GLfloat v0);
extern void (APIENTRY *gglUniform2f)(GLint location, GLfloat v0, GLfloat v1);
extern void (APIENTRY *gglUniform3f)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
extern void (APIENTRY *gglUniform4f)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
extern void (APIENTRY *gglUniform1i)(GLint location, GLint v0);
extern void (APIENTRY *gglUniform2i)(GLint location, GLint v0, GLint v1);
extern void (APIENTRY *gglUniform3i)(GLint location, GLint v0, GLint v1, GLint v2);
extern void (APIENTRY *gglUniform4i)(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
extern void (APIENTRY *gglUniform1fv)(GLint location, GLsizei count, const GLfloat *value);
extern void (APIENTRY *gglUniform2fv)(GLint location, GLsizei count, const GLfloat *value);
extern void (APIENTRY *gglUniform3fv)(GLint location, GLsizei count, const GLfloat *value);
extern void (APIENTRY *gglUniform4fv)(GLint location, GLsizei count, const GLfloat *value);
extern void (APIENTRY *gglUniform1iv)(GLint location, GLsizei count, const GLint *value);
extern void (APIENTRY *gglUniform2iv)(GLint location, GLsizei count, const GLint *value);
extern void (APIENTRY *gglUniform3iv)(GLint location, GLsizei count, const GLint *value);
extern void (APIENTRY *gglUniform4iv)(GLint location, GLsizei count, const GLint *value);
extern void (APIENTRY *gglUniformMatrix2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void (APIENTRY *gglUniformMatrix3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void (APIENTRY *gglUniformMatrix4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void (APIENTRY *gglValidateProgram)(GLuint program);
extern void (APIENTRY *gglVertexAttrib1d)(GLuint index, GLdouble x);
extern void (APIENTRY *gglVertexAttrib1dv)(GLuint index, const GLdouble *v);
extern void (APIENTRY *gglVertexAttrib1f)(GLuint index, GLfloat x);
extern void (APIENTRY *gglVertexAttrib1fv)(GLuint index, const GLfloat *v);
extern void (APIENTRY *gglVertexAttrib1s)(GLuint index, GLshort x);
extern void (APIENTRY *gglVertexAttrib1sv)(GLuint index, const GLshort *v);
extern void (APIENTRY *gglVertexAttrib2d)(GLuint index, GLdouble x, GLdouble y);
extern void (APIENTRY *gglVertexAttrib2dv)(GLuint index, const GLdouble *v);
extern void (APIENTRY *gglVertexAttrib2f)(GLuint index, GLfloat x, GLfloat y);
extern void (APIENTRY *gglVertexAttrib2fv)(GLuint index, const GLfloat *v);
extern void (APIENTRY *gglVertexAttrib2s)(GLuint index, GLshort x, GLshort y);
extern void (APIENTRY *gglVertexAttrib2sv)(GLuint index, const GLshort *v);
extern void (APIENTRY *gglVertexAttrib3d)(GLuint index, GLdouble x, GLdouble y, GLdouble z);
extern void (APIENTRY *gglVertexAttrib3dv)(GLuint index, const GLdouble *v);
extern void (APIENTRY *gglVertexAttrib3f)(GLuint index, GLfloat x, GLfloat y, GLfloat z);
extern void (APIENTRY *gglVertexAttrib3fv)(GLuint index, const GLfloat *v);
extern void (APIENTRY *gglVertexAttrib3s)(GLuint index, GLshort x, GLshort y, GLshort z);
extern void (APIENTRY *gglVertexAttrib3sv)(GLuint index, const GLshort *v);
extern void (APIENTRY *gglVertexAttrib4Nbv)(GLuint index, const GLbyte *v);
extern void (APIENTRY *gglVertexAttrib4Niv)(GLuint index, const GLint *v);
extern void (APIENTRY *gglVertexAttrib4Nsv)(GLuint index, const GLshort *v);
extern void (APIENTRY *gglVertexAttrib4Nub)(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
extern void (APIENTRY *gglVertexAttrib4Nubv)(GLuint index, const GLubyte *v);
extern void (APIENTRY *gglVertexAttrib4Nuiv)(GLuint index, const GLuint *v);
extern void (APIENTRY *gglVertexAttrib4Nusv)(GLuint index, const GLushort *v);
extern void (APIENTRY *gglVertexAttrib4bv)(GLuint index, const GLbyte *v);
extern void (APIENTRY *gglVertexAttrib4d)(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern void (APIENTRY *gglVertexAttrib4dv)(GLuint index, const GLdouble *v);
extern void (APIENTRY *gglVertexAttrib4f)(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern void (APIENTRY *gglVertexAttrib4fv)(GLuint index, const GLfloat *v);
extern void (APIENTRY *gglVertexAttrib4iv)(GLuint index, const GLint *v);
extern void (APIENTRY *gglVertexAttrib4s)(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
extern void (APIENTRY *gglVertexAttrib4sv)(GLuint index, const GLshort *v);
extern void (APIENTRY *gglVertexAttrib4ubv)(GLuint index, const GLubyte *v);
extern void (APIENTRY *gglVertexAttrib4uiv)(GLuint index, const GLuint *v);
extern void (APIENTRY *gglVertexAttrib4usv)(GLuint index, const GLushort *v);
extern void (APIENTRY *gglVertexAttribPointer)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);

//GL_VERSION_2_1
extern void (APIENTRY *gglUniformMatrix2x3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void (APIENTRY *gglUniformMatrix3x2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void (APIENTRY *gglUniformMatrix2x4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void (APIENTRY *gglUniformMatrix4x2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void (APIENTRY *gglUniformMatrix3x4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void (APIENTRY *gglUniformMatrix4x3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

//GL_VERSION_3_0
//includes GL_ARB_map_buffer_range
//includes GL_ARB_framebuffer_object
//includes GL_ARB_vertex_array_object
extern void (APIENTRY *gglColorMaski)(GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a);
extern void (APIENTRY *gglGetBooleani_v)(GLenum target, GLuint index, GLboolean *data);
extern void (APIENTRY *gglGetIntegeri_v)(GLenum target, GLuint index, GLint *data);
extern void (APIENTRY *gglEnablei)(GLenum target, GLuint index);
extern void (APIENTRY *gglDisablei)(GLenum target, GLuint index);
extern GLboolean (APIENTRY *gglIsEnabledi)(GLenum target, GLuint index);
extern void (APIENTRY *gglBeginTransformFeedback)(GLenum primitiveMode);
extern void (APIENTRY *gglEndTransformFeedback)(void);
extern void (APIENTRY *gglBindBufferRange)(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
extern void (APIENTRY *gglBindBufferBase)(GLenum target, GLuint index, GLuint buffer);
extern void (APIENTRY *gglTransformFeedbackVaryings)(GLuint program, GLsizei count, const GLint *locations, GLenum bufferMode);
extern void (APIENTRY *gglGetTransformFeedbackVarying)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name);
extern void (APIENTRY *gglClampColor)(GLenum target, GLenum clamp);
extern void (APIENTRY *gglBeginConditionalRender)(GLuint id, GLenum mode);
extern void (APIENTRY *gglEndConditionalRender)(void);
extern void (APIENTRY *gglVertexAttribIPointer)(GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer);
extern void (APIENTRY *gglGetVertexAttribIiv)(GLuint index, GLenum pname, GLint *params);
extern void (APIENTRY *gglGetVertexAttribIuiv)(GLuint index, GLenum pname, GLuint *params);
extern void (APIENTRY *gglVertexAttribI1i)(GLuint index, GLint x);
extern void (APIENTRY *gglVertexAttribI2i)(GLuint index, GLint x, GLint y);
extern void (APIENTRY *gglVertexAttribI3i)(GLuint index, GLint x, GLint y, GLint z);
extern void (APIENTRY *gglVertexAttribI4i)(GLuint index, GLint x, GLint y, GLint z, GLint w);
extern void (APIENTRY *gglVertexAttribI1ui)(GLuint index, GLuint x);
extern void (APIENTRY *gglVertexAttribI2ui)(GLuint index, GLuint x, GLuint y);
extern void (APIENTRY *gglVertexAttribI3ui)(GLuint index, GLuint x, GLuint y, GLuint z);
extern void (APIENTRY *gglVertexAttribI4ui)(GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
extern void (APIENTRY *gglVertexAttribI1iv)(GLuint index, const GLint *v);
extern void (APIENTRY *gglVertexAttribI2iv)(GLuint index, const GLint *v);
extern void (APIENTRY *gglVertexAttribI3iv)(GLuint index, const GLint *v);
extern void (APIENTRY *gglVertexAttribI4iv)(GLuint index, const GLint *v);
extern void (APIENTRY *gglVertexAttribI1uiv)(GLuint index, const GLuint *v);
extern void (APIENTRY *gglVertexAttribI2uiv)(GLuint index, const GLuint *v);
extern void (APIENTRY *gglVertexAttribI3uiv)(GLuint index, const GLuint *v);
extern void (APIENTRY *gglVertexAttribI4uiv)(GLuint index, const GLuint *v);
extern void (APIENTRY *gglVertexAttribI4bv)(GLuint index, const GLbyte *v);
extern void (APIENTRY *gglVertexAttribI4sv)(GLuint index, const GLshort *v);
extern void (APIENTRY *gglVertexAttribI4ubv)(GLuint index, const GLubyte *v);
extern void (APIENTRY *gglVertexAttribI4usv)(GLuint index, const GLushort *v);
extern void (APIENTRY *gglGetUniformuiv)(GLuint program, GLint location, GLuint *params);
extern void (APIENTRY *gglBindFragDataLocation)(GLuint program, GLuint color, const GLchar *name);
extern GLint (APIENTRY *gglGetFragDataLocation)(GLuint program, const GLchar *name);
extern void (APIENTRY *gglUniform1ui)(GLint location, GLuint v0);
extern void (APIENTRY *gglUniform2ui)(GLint location, GLuint v0, GLuint v1);
extern void (APIENTRY *gglUniform3ui)(GLint location, GLuint v0, GLuint v1, GLuint v2);
extern void (APIENTRY *gglUniform4ui)(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
extern void (APIENTRY *gglUniform1uiv)(GLint location, GLsizei count, const GLuint *value);
extern void (APIENTRY *gglUniform2uiv)(GLint location, GLsizei count, const GLuint *value);
extern void (APIENTRY *gglUniform3uiv)(GLint location, GLsizei count, const GLuint *value);
extern void (APIENTRY *gglUniform4uiv)(GLint location, GLsizei count, const GLuint *value);
extern void (APIENTRY *gglTexParameterIiv)(GLenum target, GLenum pname, const GLint *params);
extern void (APIENTRY *gglTexParameterIuiv)(GLenum target, GLenum pname, const GLuint *params);
extern void (APIENTRY *gglGetTexParameterIiv)(GLenum target, GLenum pname, GLint *params);
extern void (APIENTRY *gglGetTexParameterIuiv)(GLenum target, GLenum pname, GLuint *params);
extern void (APIENTRY *gglClearBufferiv)(GLenum buffer, GLint drawbuffer, const GLint *value);
extern void (APIENTRY *gglClearBufferuiv)(GLenum buffer, GLint drawbuffer, const GLuint *value);
extern void (APIENTRY *gglClearBufferfv)(GLenum buffer, GLint drawbuffer, const GLfloat *value);
extern void (APIENTRY *gglClearBufferfi)(GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil);
extern const GLubyte *(APIENTRY *gglGetStringi)(GLenum name, GLuint index);
//extern GLboolean (APIENTRY *gglIsRenderbuffer)(GLuint renderbuffer);
//extern void (APIENTRY *gglBindRenderbuffer)(GLenum target, GLuint renderbuffer);
//extern void (APIENTRY *gglDeleteRenderbuffers)(GLsizei n, const GLuint *renderbuffers);
//extern void (APIENTRY *gglGenRenderbuffers)(GLsizei n, GLuint *renderbuffers);
//extern void (APIENTRY *gglRenderbufferStorage)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
//extern void (APIENTRY *gglGetRenderbufferParameteriv)(GLenum target, GLenum pname, GLint *params);
//extern GLboolean (APIENTRY *gglIsFramebuffer)(GLuint framebuffer);
//extern void (APIENTRY *gglBindFramebuffer)(GLenum target, GLuint framebuffer);
//extern void (APIENTRY *gglDeleteFramebuffers)(GLsizei n, const GLuint *framebuffers);
//extern void (APIENTRY *gglGenFramebuffers)(GLsizei n, GLuint *framebuffers);
//extern GLenum (APIENTRY *gglCheckFramebufferStatus)(GLenum target);
//extern void (APIENTRY *gglFramebufferTexture1D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
//extern void (APIENTRY *gglFramebufferTexture2D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
//extern void (APIENTRY *gglFramebufferTexture3D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
//extern void (APIENTRY *gglFramebufferRenderbuffer)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
//extern void (APIENTRY *gglGetFramebufferAttachmentParameteriv)(GLenum target, GLenum attachment, GLenum pname, GLint *params);
//extern void (APIENTRY *gglGenerateMipmap)(GLenum target);
//extern void (APIENTRY *gglBlitFramebuffer)(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
//extern void (APIENTRY *gglRenderbufferStorageMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
//extern void (APIENTRY *gglFramebufferTextureLayer)(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
//extern void *(APIENTRY *gglMapBufferRange)(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
//extern void (APIENTRY *gglFlushMappedBufferRange)(GLenum target, GLintptr offset, GLsizeiptr length);
//extern void (APIENTRY *gglBindVertexArray)(GLuint array);
//extern void (APIENTRY *gglDeleteVertexArrays)(GLsizei n, const GLuint *arrays);
//extern void (APIENTRY *gglGenVertexArrays)(GLsizei n, GLuint *arrays);
//extern GLboolean (APIENTRY *gglIsVertexArray)(GLuint array);

//GL_VERSION_3_1
extern void (APIENTRY *gglDrawArraysInstanced)(GLenum mode, GLint first, GLsizei count, GLsizei instancecount);
extern void (APIENTRY *gglDrawElementsInstanced)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount);
extern void (APIENTRY *gglTexBuffer)(GLenum target, GLenum internalformat, GLuint buffer);
extern void (APIENTRY *gglPrimitiveRestartIndex)(GLuint index);
extern void (APIENTRY *gglCopyBufferSubData)(GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
extern void (APIENTRY *gglGetUniformIndices)(GLuint program, GLsizei uniformCount, const GLchar *const*uniformNames, GLuint *uniformIndices);
extern void (APIENTRY *gglGetActiveUniformsiv)(GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params);
extern void (APIENTRY *gglGetActiveUniformName)(GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformName);
extern GLuint (APIENTRY *gglGetUniformBlockIndex)(GLuint program, const GLchar *uniformBlockName);
extern void (APIENTRY *gglGetActiveUniformBlockiv)(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params);
extern void (APIENTRY *gglGetActiveUniformBlockName)(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName);
extern void (APIENTRY *gglUniformBlockBinding)(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);

//GL_VERSION_3_2
extern void (APIENTRY *gglDrawElementsBaseVertex)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLint basevertex);
extern void (APIENTRY *gglDrawRangeElementsBaseVertex)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices, GLint basevertex);
extern void (APIENTRY *gglDrawElementsInstancedBaseVertex)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLint basevertex);
extern void (APIENTRY *gglMultiDrawElementsBaseVertex)(GLenum mode, const GLsizei *count, GLenum type, const void *const*indices, GLsizei drawcount, const GLint *basevertex);
extern void (APIENTRY *gglProvokingVertex)(GLenum mode);
extern GLsync (APIENTRY *gglFenceSync)(GLenum condition, GLbitfield flags);
extern GLboolean (APIENTRY *gglIsSync)(GLsync sync);
extern void (APIENTRY *gglDeleteSync)(GLsync sync);
extern GLenum (APIENTRY *gglClientWaitSync)(GLsync sync, GLbitfield flags, GLuint64 timeout);
extern void (APIENTRY *gglWaitSync)(GLsync sync, GLbitfield flags, GLuint64 timeout);
extern void (APIENTRY *gglGetInteger64v)(GLenum pname, GLint64 *data);
extern void (APIENTRY *gglGetSynciv)(GLsync sync, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values);
extern void (APIENTRY *gglGetInteger64i_v)(GLenum target, GLuint index, GLint64 *data);
extern void (APIENTRY *gglGetBufferParameteri64v)(GLenum target, GLenum pname, GLint64 *params);
extern void (APIENTRY *gglFramebufferTexture)(GLenum target, GLenum attachment, GLuint texture, GLint level);
extern void (APIENTRY *gglTexImage2DMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
extern void (APIENTRY *gglTexImage3DMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
extern void (APIENTRY *gglGetMultisamplefv)(GLenum pname, GLuint index, GLfloat *val);
extern void (APIENTRY *gglSampleMaski)(GLuint maskNumber, GLbitfield mask);

//GL_VERSION_3_3
//includes GL_ARB_instanced_arrays
extern void (APIENTRY *gglBindFragDataLocationIndexed)(GLuint program, GLuint colorNumber, GLuint index, const GLchar *name);
extern GLint (APIENTRY *gglGetFragDataIndex)(GLuint program, const GLchar *name);
extern void (APIENTRY *gglGenSamplers)(GLsizei count, GLuint *samplers);
extern void (APIENTRY *gglDeleteSamplers)(GLsizei count, const GLuint *samplers);
extern GLboolean (APIENTRY *gglIsSampler)(GLuint sampler);
extern void (APIENTRY *gglBindSampler)(GLuint unit, GLuint sampler);
extern void (APIENTRY *gglSamplerParameteri)(GLuint sampler, GLenum pname, GLint param);
extern void (APIENTRY *gglSamplerParameteriv)(GLuint sampler, GLenum pname, const GLint *param);
extern void (APIENTRY *gglSamplerParameterf)(GLuint sampler, GLenum pname, GLfloat param);
extern void (APIENTRY *gglSamplerParameterfv)(GLuint sampler, GLenum pname, const GLfloat *param);
extern void (APIENTRY *gglSamplerParameterIiv)(GLuint sampler, GLenum pname, const GLint *param);
extern void (APIENTRY *gglSamplerParameterIuiv)(GLuint sampler, GLenum pname, const GLuint *param);
extern void (APIENTRY *gglGetSamplerParameteriv)(GLuint sampler, GLenum pname, GLint *params);
extern void (APIENTRY *gglGetSamplerParameterIiv)(GLuint sampler, GLenum pname, GLint *params);
extern void (APIENTRY *gglGetSamplerParameterfv)(GLuint sampler, GLenum pname, GLfloat *params);
extern void (APIENTRY *gglGetSamplerParameterIuiv)(GLuint sampler, GLenum pname, GLuint *params);
extern void (APIENTRY *gglQueryCounter)(GLuint id, GLenum target);
extern void (APIENTRY *gglGetQueryObjecti64v)(GLuint id, GLenum pname, GLint64 *params);
extern void (APIENTRY *gglGetQueryObjectui64v)(GLuint id, GLenum pname, GLuint64 *params);
//extern void (APIENTRY *gglVertexAttribDivisor)(GLuint index, GLuint divisor);
extern void (APIENTRY *gglVertexAttribP1ui)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
extern void (APIENTRY *gglVertexAttribP1uiv)(GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
extern void (APIENTRY *gglVertexAttribP2ui)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
extern void (APIENTRY *gglVertexAttribP2uiv)(GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
extern void (APIENTRY *gglVertexAttribP3ui)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
extern void (APIENTRY *gglVertexAttribP3uiv)(GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
extern void (APIENTRY *gglVertexAttribP4ui)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
extern void (APIENTRY *gglVertexAttribP4uiv)(GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
extern void (APIENTRY *gglVertexP2ui)(GLenum type, GLuint value);
extern void (APIENTRY *gglVertexP2uiv)(GLenum type, const GLuint *value);
extern void (APIENTRY *gglVertexP3ui)(GLenum type, GLuint value);
extern void (APIENTRY *gglVertexP3uiv)(GLenum type, const GLuint *value);
extern void (APIENTRY *gglVertexP4ui)(GLenum type, GLuint value);
extern void (APIENTRY *gglVertexP4uiv)(GLenum type, const GLuint *value);
extern void (APIENTRY *gglTexCoordP1ui)(GLenum type, GLuint coords);
extern void (APIENTRY *gglTexCoordP1uiv)(GLenum type, const GLuint *coords);
extern void (APIENTRY *gglTexCoordP2ui)(GLenum type, GLuint coords);
extern void (APIENTRY *gglTexCoordP2uiv)(GLenum type, const GLuint *coords);
extern void (APIENTRY *gglTexCoordP3ui)(GLenum type, GLuint coords);
extern void (APIENTRY *gglTexCoordP3uiv)(GLenum type, const GLuint *coords);
extern void (APIENTRY *gglTexCoordP4ui)(GLenum type, GLuint coords);
extern void (APIENTRY *gglTexCoordP4uiv)(GLenum type, const GLuint *coords);
extern void (APIENTRY *gglMultiTexCoordP1ui)(GLenum texture, GLenum type, GLuint coords);
extern void (APIENTRY *gglMultiTexCoordP1uiv)(GLenum texture, GLenum type, const GLuint *coords);
extern void (APIENTRY *gglMultiTexCoordP2ui)(GLenum texture, GLenum type, GLuint coords);
extern void (APIENTRY *gglMultiTexCoordP2uiv)(GLenum texture, GLenum type, const GLuint *coords);
extern void (APIENTRY *gglMultiTexCoordP3ui)(GLenum texture, GLenum type, GLuint coords);
extern void (APIENTRY *gglMultiTexCoordP3uiv)(GLenum texture, GLenum type, const GLuint *coords);
extern void (APIENTRY *gglMultiTexCoordP4ui)(GLenum texture, GLenum type, GLuint coords);
extern void (APIENTRY *gglMultiTexCoordP4uiv)(GLenum texture, GLenum type, const GLuint *coords);
extern void (APIENTRY *gglNormalP3ui)(GLenum type, GLuint coords);
extern void (APIENTRY *gglNormalP3uiv)(GLenum type, const GLuint *coords);
extern void (APIENTRY *gglColorP3ui)(GLenum type, GLuint color);
extern void (APIENTRY *gglColorP3uiv)(GLenum type, const GLuint *color);
extern void (APIENTRY *gglColorP4ui)(GLenum type, GLuint color);
extern void (APIENTRY *gglColorP4uiv)(GLenum type, const GLuint *color);
extern void (APIENTRY *gglSecondaryColorP3ui)(GLenum type, GLuint color);
extern void (APIENTRY *gglSecondaryColorP3uiv)(GLenum type, const GLuint *color);

//GL_VERSION_4_0
extern void (APIENTRY *gglMinSampleShading)(GLfloat value);
extern void (APIENTRY *gglBlendEquationi)(GLuint buf, GLenum mode);
extern void (APIENTRY *gglBlendEquationSeparatei)(GLuint buf, GLenum modeRGB, GLenum modeAlpha);
extern void (APIENTRY *gglBlendFunci)(GLuint buf, GLenum src, GLenum dst);
extern void (APIENTRY *gglBlendFuncSeparatei)(GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
extern void (APIENTRY *gglDrawArraysIndirect)(GLenum mode, const void *indirect);
extern void (APIENTRY *gglDrawElementsIndirect)(GLenum mode, GLenum type, const void *indirect);
extern void (APIENTRY *gglUniform1d)(GLint location, GLdouble x);
extern void (APIENTRY *gglUniform2d)(GLint location, GLdouble x, GLdouble y);
extern void (APIENTRY *gglUniform3d)(GLint location, GLdouble x, GLdouble y, GLdouble z);
extern void (APIENTRY *gglUniform4d)(GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern void (APIENTRY *gglUniform1dv)(GLint location, GLsizei count, const GLdouble *value);
extern void (APIENTRY *gglUniform2dv)(GLint location, GLsizei count, const GLdouble *value);
extern void (APIENTRY *gglUniform3dv)(GLint location, GLsizei count, const GLdouble *value);
extern void (APIENTRY *gglUniform4dv)(GLint location, GLsizei count, const GLdouble *value);
extern void (APIENTRY *gglUniformMatrix2dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
extern void (APIENTRY *gglUniformMatrix3dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
extern void (APIENTRY *gglUniformMatrix4dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
extern void (APIENTRY *gglUniformMatrix2x3dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
extern void (APIENTRY *gglUniformMatrix2x4dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
extern void (APIENTRY *gglUniformMatrix3x2dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
extern void (APIENTRY *gglUniformMatrix3x4dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
extern void (APIENTRY *gglUniformMatrix4x2dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
extern void (APIENTRY *gglUniformMatrix4x3dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
extern void (APIENTRY *gglGetUniformdv)(GLuint program, GLint location, GLdouble *params);
extern GLint (APIENTRY *gglGetSubroutineUniformLocation)(GLuint program, GLenum shadertype, const GLchar *name);
extern GLuint (APIENTRY *gglGetSubroutineIndex)(GLuint program, GLenum shadertype, const GLchar *name);
extern void (APIENTRY *gglGetActiveSubroutineUniformiv)(GLuint program, GLenum shadertype, GLuint index, GLenum pname, GLint *values);
extern void (APIENTRY *gglGetActiveSubroutineUniformName)(GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei *length, GLchar *name);
extern void (APIENTRY *gglGetActiveSubroutineName)(GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei *length, GLchar *name);
extern void (APIENTRY *gglUniformSubroutinesuiv)(GLenum shadertype, GLsizei count, const GLuint *indices);
extern void (APIENTRY *gglGetUniformSubroutineuiv)(GLenum shadertype, GLint location, GLuint *params);
extern void (APIENTRY *gglGetProgramStageiv)(GLuint program, GLenum shadertype, GLenum pname, GLint *values);
extern void (APIENTRY *gglPatchParameteri)(GLenum pname, GLint value);
extern void (APIENTRY *gglPatchParameterfv)(GLenum pname, const GLfloat *values);
extern void (APIENTRY *gglBindTransformFeedback)(GLenum target, GLuint id);
extern void (APIENTRY *gglDeleteTransformFeedbacks)(GLsizei n, const GLuint *ids);
extern void (APIENTRY *gglGenTransformFeedbacks)(GLsizei n, GLuint *ids);
extern GLboolean (APIENTRY *gglIsTransformFeedback)(GLuint id);
extern void (APIENTRY *gglPauseTransformFeedback)(void);
extern void (APIENTRY *gglResumeTransformFeedback)(void);
extern void (APIENTRY *gglDrawTransformFeedback)(GLenum mode, GLuint id);
extern void (APIENTRY *gglDrawTransformFeedbackStream)(GLenum mode, GLuint id, GLuint stream);
extern void (APIENTRY *gglBeginQueryIndexed)(GLenum target, GLuint index, GLuint id);
extern void (APIENTRY *gglEndQueryIndexed)(GLenum target, GLuint index);
extern void (APIENTRY *gglGetQueryIndexediv)(GLenum target, GLuint index, GLenum pname, GLint *params);

//GL_VERSION_4_1
extern void (APIENTRY *gglReleaseShaderCompiler)(void);
extern void (APIENTRY *gglShaderBinary)(GLsizei count, const GLuint *shaders, GLenum binaryformat, const void *binary, GLsizei length);
extern void (APIENTRY *gglGetShaderPrecisionFormat)(GLenum shadertype, GLenum precisiontype, GLint *range, GLint *precision);
extern void (APIENTRY *gglDepthRangef)(GLfloat n, GLfloat f);
extern void (APIENTRY *gglClearDepthf)(GLfloat d);
extern void (APIENTRY *gglGetProgramBinary)(GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, void *binary);
extern void (APIENTRY *gglProgramBinary)(GLuint program, GLenum binaryFormat, const void *binary, GLsizei length);
extern void (APIENTRY *gglProgramParameteri)(GLuint program, GLenum pname, GLint value);
extern void (APIENTRY *gglUseProgramStages)(GLuint pipeline, GLbitfield stages, GLuint program);
extern void (APIENTRY *gglActiveShaderProgram)(GLuint pipeline, GLuint program);
extern GLuint (APIENTRY *gglCreateShaderProgramv)(GLenum type, GLsizei count, const GLchar *const*strings);
extern void (APIENTRY *gglBindProgramPipeline)(GLuint pipeline);
extern void (APIENTRY *gglDeleteProgramPipelines)(GLsizei n, const GLuint *pipelines);
extern void (APIENTRY *gglGenProgramPipelines)(GLsizei n, GLuint *pipelines);
extern GLboolean (APIENTRY *gglIsProgramPipeline)(GLuint pipeline);
extern void (APIENTRY *gglGetProgramPipelineiv)(GLuint pipeline, GLenum pname, GLint *params);
extern void (APIENTRY *gglProgramUniform1i)(GLuint program, GLint location, GLint v0);
extern void (APIENTRY *gglProgramUniform1iv)(GLuint program, GLint location, GLsizei count, const GLint *value);
extern void (APIENTRY *gglProgramUniform1f)(GLuint program, GLint location, GLfloat v0);
extern void (APIENTRY *gglProgramUniform1fv)(GLuint program, GLint location, GLsizei count, const GLfloat *value);
extern void (APIENTRY *gglProgramUniform1d)(GLuint program, GLint location, GLdouble v0);
extern void (APIENTRY *gglProgramUniform1dv)(GLuint program, GLint location, GLsizei count, const GLdouble *value);
extern void (APIENTRY *gglProgramUniform1ui)(GLuint program, GLint location, GLuint v0);
extern void (APIENTRY *gglProgramUniform1uiv)(GLuint program, GLint location, GLsizei count, const GLuint *value);
extern void (APIENTRY *gglProgramUniform2i)(GLuint program, GLint location, GLint v0, GLint v1);
extern void (APIENTRY *gglProgramUniform2iv)(GLuint program, GLint location, GLsizei count, const GLint *value);
extern void (APIENTRY *gglProgramUniform2f)(GLuint program, GLint location, GLfloat v0, GLfloat v1);
extern void (APIENTRY *gglProgramUniform2fv)(GLuint program, GLint location, GLsizei count, const GLfloat *value);
extern void (APIENTRY *gglProgramUniform2d)(GLuint program, GLint location, GLdouble v0, GLdouble v1);
extern void (APIENTRY *gglProgramUniform2dv)(GLuint program, GLint location, GLsizei count, const GLdouble *value);
extern void (APIENTRY *gglProgramUniform2ui)(GLuint program, GLint location, GLuint v0, GLuint v1);
extern void (APIENTRY *gglProgramUniform2uiv)(GLuint program, GLint location, GLsizei count, const GLuint *value);
extern void (APIENTRY *gglProgramUniform3i)(GLuint program, GLint location, GLint v0, GLint v1, GLint v2);
extern void (APIENTRY *gglProgramUniform3iv)(GLuint program, GLint location, GLsizei count, const GLint *value);
extern void (APIENTRY *gglProgramUniform3f)(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
extern void (APIENTRY *gglProgramUniform3fv)(GLuint program, GLint location, GLsizei count, const GLfloat *value);
extern void (APIENTRY *gglProgramUniform3d)(GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2);
extern void (APIENTRY *gglProgramUniform3dv)(GLuint program, GLint location, GLsizei count, const GLdouble *value);
extern void (APIENTRY *gglProgramUniform3ui)(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2);
extern void (APIENTRY *gglProgramUniform3uiv)(GLuint program, GLint location, GLsizei count, const GLuint *value);
extern void (APIENTRY *gglProgramUniform4i)(GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
extern void (APIENTRY *gglProgramUniform4iv)(GLuint program, GLint location, GLsizei count, const GLint *value);
extern void (APIENTRY *gglProgramUniform4f)(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
extern void (APIENTRY *gglProgramUniform4fv)(GLuint program, GLint location, GLsizei count, const GLfloat *value);
extern void (APIENTRY *gglProgramUniform4d)(GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3);
extern void (APIENTRY *gglProgramUniform4dv)(GLuint program, GLint location, GLsizei count, const GLdouble *value);
extern void (APIENTRY *gglProgramUniform4ui)(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
extern void (APIENTRY *gglProgramUniform4uiv)(GLuint program, GLint location, GLsizei count, const GLuint *value);
extern void (APIENTRY *gglProgramUniformMatrix2fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void (APIENTRY *gglProgramUniformMatrix3fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void (APIENTRY *gglProgramUniformMatrix4fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void (APIENTRY *gglProgramUniformMatrix2dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
extern void (APIENTRY *gglProgramUniformMatrix3dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
extern void (APIENTRY *gglProgramUniformMatrix4dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
extern void (APIENTRY *gglProgramUniformMatrix2x3fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void (APIENTRY *gglProgramUniformMatrix3x2fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void (APIENTRY *gglProgramUniformMatrix2x4fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void (APIENTRY *gglProgramUniformMatrix4x2fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void (APIENTRY *gglProgramUniformMatrix3x4fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void (APIENTRY *gglProgramUniformMatrix4x3fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void (APIENTRY *gglProgramUniformMatrix2x3dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
extern void (APIENTRY *gglProgramUniformMatrix3x2dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
extern void (APIENTRY *gglProgramUniformMatrix2x4dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
extern void (APIENTRY *gglProgramUniformMatrix4x2dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
extern void (APIENTRY *gglProgramUniformMatrix3x4dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
extern void (APIENTRY *gglProgramUniformMatrix4x3dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
extern void (APIENTRY *gglValidateProgramPipeline)(GLuint pipeline);
extern void (APIENTRY *gglGetProgramPipelineInfoLog)(GLuint pipeline, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
extern void (APIENTRY *gglVertexAttribL1d)(GLuint index, GLdouble x);
extern void (APIENTRY *gglVertexAttribL2d)(GLuint index, GLdouble x, GLdouble y);
extern void (APIENTRY *gglVertexAttribL3d)(GLuint index, GLdouble x, GLdouble y, GLdouble z);
extern void (APIENTRY *gglVertexAttribL4d)(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern void (APIENTRY *gglVertexAttribL1dv)(GLuint index, const GLdouble *v);
extern void (APIENTRY *gglVertexAttribL2dv)(GLuint index, const GLdouble *v);
extern void (APIENTRY *gglVertexAttribL3dv)(GLuint index, const GLdouble *v);
extern void (APIENTRY *gglVertexAttribL4dv)(GLuint index, const GLdouble *v);
extern void (APIENTRY *gglVertexAttribLPointer)(GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer);
extern void (APIENTRY *gglGetVertexAttribLdv)(GLuint index, GLenum pname, GLdouble *params);
extern void (APIENTRY *gglViewportArrayv)(GLuint first, GLsizei count, const GLfloat *v);
extern void (APIENTRY *gglViewportIndexedf)(GLuint index, GLfloat x, GLfloat y, GLfloat w, GLfloat h);
extern void (APIENTRY *gglViewportIndexedfv)(GLuint index, const GLfloat *v);
extern void (APIENTRY *gglScissorArrayv)(GLuint first, GLsizei count, const GLint *v);
extern void (APIENTRY *gglScissorIndexed)(GLuint index, GLint left, GLint bottom, GLsizei width, GLsizei height);
extern void (APIENTRY *gglScissorIndexedv)(GLuint index, const GLint *v);
extern void (APIENTRY *gglDepthRangeArrayv)(GLuint first, GLsizei count, const GLdouble *v);
extern void (APIENTRY *gglDepthRangeIndexed)(GLuint index, GLdouble n, GLdouble f);
extern void (APIENTRY *gglGetFloati_v)(GLenum target, GLuint index, GLfloat *data);
extern void (APIENTRY *gglGetDoublei_v)(GLenum target, GLuint index, GLdouble *data);

//GL_VERSION_4_2
extern void (APIENTRY *gglDrawArraysInstancedBaseInstance)(GLenum mode, GLint first, GLsizei count, GLsizei instancecount, GLuint baseinstance);
extern void (APIENTRY *gglDrawElementsInstancedBaseInstance)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLuint baseinstance);
extern void (APIENTRY *gglDrawElementsInstancedBaseVertexBaseInstance)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLint basevertex, GLuint baseinstance);
extern void (APIENTRY *gglGetInternalformativ)(GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint *params);
extern void (APIENTRY *gglGetActiveAtomicCounterBufferiv)(GLuint program, GLuint bufferIndex, GLenum pname, GLint *params);
extern void (APIENTRY *gglBindImageTexture)(GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format);
extern void (APIENTRY *gglMemoryBarrier)(GLbitfield barriers);
extern void (APIENTRY *gglTexStorage1D)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width);
extern void (APIENTRY *gglTexStorage2D)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
extern void (APIENTRY *gglTexStorage3D)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
extern void (APIENTRY *gglDrawTransformFeedbackInstanced)(GLenum mode, GLuint id, GLsizei instancecount);
extern void (APIENTRY *gglDrawTransformFeedbackStreamInstanced)(GLenum mode, GLuint id, GLuint stream, GLsizei instancecount);

//GL_VERSION_4_3
extern void (APIENTRY *gglClearBufferData)(GLenum target, GLenum internalformat, GLenum format, GLenum type, const void *data);
extern void (APIENTRY *gglClearBufferSubData)(GLenum target, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void *data);
extern void (APIENTRY *gglDispatchCompute)(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z);
extern void (APIENTRY *gglDispatchComputeIndirect)(GLintptr indirect);
extern void (APIENTRY *gglCopyImageSubData)(GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth);
extern void (APIENTRY *gglFramebufferParameteri)(GLenum target, GLenum pname, GLint param);
extern void (APIENTRY *gglGetFramebufferParameteriv)(GLenum target, GLenum pname, GLint *params);
extern void (APIENTRY *gglGetInternalformati64v)(GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint64 *params);
extern void (APIENTRY *gglInvalidateTexSubImage)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth);
extern void (APIENTRY *gglInvalidateTexImage)(GLuint texture, GLint level);
extern void (APIENTRY *gglInvalidateBufferSubData)(GLuint buffer, GLintptr offset, GLsizeiptr length);
extern void (APIENTRY *gglInvalidateBufferData)(GLuint buffer);
extern void (APIENTRY *gglInvalidateFramebuffer)(GLenum target, GLsizei numAttachments, const GLenum *attachments);
extern void (APIENTRY *gglInvalidateSubFramebuffer)(GLenum target, GLsizei numAttachments, const GLenum *attachments, GLint x, GLint y, GLsizei width, GLsizei height);
extern void (APIENTRY *gglMultiDrawArraysIndirect)(GLenum mode, const void *indirect, GLsizei drawcount, GLsizei stride);
extern void (APIENTRY *gglMultiDrawElementsIndirect)(GLenum mode, GLenum type, const void *indirect, GLsizei drawcount, GLsizei stride);
extern void (APIENTRY *gglGetProgramInterfaceiv)(GLuint program, GLenum programInterface, GLenum pname, GLint *params);
extern GLuint (APIENTRY *gglGetProgramResourceIndex)(GLuint program, GLenum programInterface, const GLchar *name);
extern void (APIENTRY *gglGetProgramResourceName)(GLuint program, GLenum programInterface, GLuint index, GLsizei bufSize, GLsizei *length, GLchar *name);
extern void (APIENTRY *gglGetProgramResourceiv)(GLuint program, GLenum programInterface, GLuint index, GLsizei propCount, const GLenum *props, GLsizei bufSize, GLsizei *length, GLint *params);
extern GLint (APIENTRY *gglGetProgramResourceLocation)(GLuint program, GLenum programInterface, const GLchar *name);
extern GLint (APIENTRY *gglGetProgramResourceLocationIndex)(GLuint program, GLenum programInterface, const GLchar *name);
extern void (APIENTRY *gglShaderStorageBlockBinding)(GLuint program, GLuint storageBlockIndex, GLuint storageBlockBinding);
extern void (APIENTRY *gglTexBufferRange)(GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);
extern void (APIENTRY *gglTexStorage2DMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
extern void (APIENTRY *gglTexStorage3DMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
extern void (APIENTRY *gglTextureView)(GLuint texture, GLenum target, GLuint origtexture, GLenum internalformat, GLuint minlevel, GLuint numlevels, GLuint minlayer, GLuint numlayers);
extern void (APIENTRY *gglBindVertexBuffer)(GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
extern void (APIENTRY *gglVertexAttribFormat)(GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
extern void (APIENTRY *gglVertexAttribIFormat)(GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
extern void (APIENTRY *gglVertexAttribLFormat)(GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
extern void (APIENTRY *gglVertexAttribBinding)(GLuint attribindex, GLuint bindingindex);
extern void (APIENTRY *gglVertexBindingDivisor)(GLuint bindingindex, GLuint divisor);
extern void (APIENTRY *gglDebugMessageControl)(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled);
extern void (APIENTRY *gglDebugMessageInsert)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *buf);
extern void (APIENTRY *gglDebugMessageCallback)(GLDEBUGPROC callback, const void *userParam);
extern GLuint (APIENTRY *gglGetDebugMessageLog)(GLuint count, GLsizei bufSize, GLenum *sources, GLenum *types, GLuint *ids, GLenum *severities, GLsizei *lengths, GLchar *messageLog);
extern void (APIENTRY *gglPushDebugGroup)(GLenum source, GLuint id, GLsizei length, const GLchar *message);
extern void (APIENTRY *gglPopDebugGroup)(void);
extern void (APIENTRY *gglObjectLabel)(GLenum identifier, GLuint name, GLsizei length, const GLchar *label);
extern void (APIENTRY *gglGetObjectLabel)(GLenum identifier, GLuint name, GLsizei bufSize, GLsizei *length, GLchar *label);
extern void (APIENTRY *gglObjectPtrLabel)(const void *ptr, GLsizei length, const GLchar *label);
extern void (APIENTRY *gglGetObjectPtrLabel)(const void *ptr, GLsizei bufSize, GLsizei *length, GLchar *label);

//GL_VERSION_4_4
extern void (APIENTRY *gglBufferStorage)(GLenum target, GLsizeiptr size, const void *data, GLbitfield flags);
extern void (APIENTRY *gglClearTexImage)(GLuint texture, GLint level, GLenum format, GLenum type, const void *data);
extern void (APIENTRY *gglClearTexSubImage)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *data);
extern void (APIENTRY *gglBindBuffersBase)(GLenum target, GLuint first, GLsizei count, const GLuint *buffers);
extern void (APIENTRY *gglBindBuffersRange)(GLenum target, GLuint first, GLsizei count, const GLuint *buffers, const GLintptr *offsets, const GLsizeiptr *sizes);
extern void (APIENTRY *gglBindTextures)(GLuint first, GLsizei count, const GLuint *textures);
extern void (APIENTRY *gglBindSamplers)(GLuint first, GLsizei count, const GLuint *samplers);
extern void (APIENTRY *gglBindImageTextures)(GLuint first, GLsizei count, const GLuint *textures);
extern void (APIENTRY *gglBindVertexBuffers)(GLuint first, GLsizei count, const GLuint *buffers, const GLintptr *offsets, const GLsizei *strides);

//GL_VERSION_4_5
extern void (APIENTRY *gglClipControl)(GLenum origin, GLenum depth);
extern void (APIENTRY *gglCreateTransformFeedbacks)(GLsizei n, GLuint *ids);
extern void (APIENTRY *gglTransformFeedbackBufferBase)(GLuint xfb, GLuint index, GLuint buffer);
extern void (APIENTRY *gglTransformFeedbackBufferRange)(GLuint xfb, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
extern void (APIENTRY *gglGetTransformFeedbackiv)(GLuint xfb, GLenum pname, GLint *param);
extern void (APIENTRY *gglGetTransformFeedbacki_v)(GLuint xfb, GLenum pname, GLuint index, GLint *param);
extern void (APIENTRY *gglGetTransformFeedbacki64_v)(GLuint xfb, GLenum pname, GLuint index, GLint64 *param);
extern void (APIENTRY *gglCreateBuffers)(GLsizei n, GLuint *buffers);
extern void (APIENTRY *gglNamedBufferStorage)(GLuint buffer, GLsizeiptr size, const void *data, GLbitfield flags);
extern void (APIENTRY *gglNamedBufferData)(GLuint buffer, GLsizeiptr size, const void *data, GLenum usage);
extern void (APIENTRY *gglNamedBufferSubData)(GLuint buffer, GLintptr offset, GLsizeiptr size, const void *data);
extern void (APIENTRY *gglCopyNamedBufferSubData)(GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
extern void (APIENTRY *gglClearNamedBufferData)(GLuint buffer, GLenum internalformat, GLenum format, GLenum type, const void *data);
extern void (APIENTRY *gglClearNamedBufferSubData)(GLuint buffer, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void *data);
extern void *(APIENTRY *gglMapNamedBuffer)(GLuint buffer, GLenum access);
extern void *(APIENTRY *gglMapNamedBufferRange)(GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access);
extern GLboolean (APIENTRY *gglUnmapNamedBuffer)(GLuint buffer);
extern void (APIENTRY *gglFlushMappedNamedBufferRange)(GLuint buffer, GLintptr offset, GLsizeiptr length);
extern void (APIENTRY *gglGetNamedBufferParameteriv)(GLuint buffer, GLenum pname, GLint *params);
extern void (APIENTRY *gglGetNamedBufferParameteri64v)(GLuint buffer, GLenum pname, GLint64 *params);
extern void (APIENTRY *gglGetNamedBufferPointerv)(GLuint buffer, GLenum pname, void **params);
extern void (APIENTRY *gglGetNamedBufferSubData)(GLuint buffer, GLintptr offset, GLsizeiptr size, void *data);
extern void (APIENTRY *gglCreateFramebuffers)(GLsizei n, GLuint *framebuffers);
extern void (APIENTRY *gglNamedFramebufferRenderbuffer)(GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
extern void (APIENTRY *gglNamedFramebufferParameteri)(GLuint framebuffer, GLenum pname, GLint param);
extern void (APIENTRY *gglNamedFramebufferTexture)(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level);
extern void (APIENTRY *gglNamedFramebufferTextureLayer)(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint layer);
extern void (APIENTRY *gglNamedFramebufferDrawBuffer)(GLuint framebuffer, GLenum buf);
extern void (APIENTRY *gglNamedFramebufferDrawBuffers)(GLuint framebuffer, GLsizei n, const GLenum *bufs);
extern void (APIENTRY *gglNamedFramebufferReadBuffer)(GLuint framebuffer, GLenum src);
extern void (APIENTRY *gglInvalidateNamedFramebufferData)(GLuint framebuffer, GLsizei numAttachments, const GLenum *attachments);
extern void (APIENTRY *gglInvalidateNamedFramebufferSubData)(GLuint framebuffer, GLsizei numAttachments, const GLenum *attachments, GLint x, GLint y, GLsizei width, GLsizei height);
extern void (APIENTRY *gglClearNamedFramebufferiv)(GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLint *value);
extern void (APIENTRY *gglClearNamedFramebufferuiv)(GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLuint *value);
extern void (APIENTRY *gglClearNamedFramebufferfv)(GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLfloat *value);
extern void (APIENTRY *gglClearNamedFramebufferfi)(GLuint framebuffer, GLenum buffer, const GLfloat depth, GLint stencil);
extern void (APIENTRY *gglBlitNamedFramebuffer)(GLuint readFramebuffer, GLuint drawFramebuffer, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
extern GLenum (APIENTRY *gglCheckNamedFramebufferStatus)(GLuint framebuffer, GLenum target);
extern void (APIENTRY *gglGetNamedFramebufferParameteriv)(GLuint framebuffer, GLenum pname, GLint *param);
extern void (APIENTRY *gglGetNamedFramebufferAttachmentParameteriv)(GLuint framebuffer, GLenum attachment, GLenum pname, GLint *params);
extern void (APIENTRY *gglCreateRenderbuffers)(GLsizei n, GLuint *renderbuffers);
extern void (APIENTRY *gglNamedRenderbufferStorage)(GLuint renderbuffer, GLenum internalformat, GLsizei width, GLsizei height);
extern void (APIENTRY *gglNamedRenderbufferStorageMultisample)(GLuint renderbuffer, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
extern void (APIENTRY *gglGetNamedRenderbufferParameteriv)(GLuint renderbuffer, GLenum pname, GLint *params);
extern void (APIENTRY *gglCreateTextures)(GLenum target, GLsizei n, GLuint *textures);
extern void (APIENTRY *gglTextureBuffer)(GLuint texture, GLenum internalformat, GLuint buffer);
extern void (APIENTRY *gglTextureBufferRange)(GLuint texture, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);
extern void (APIENTRY *gglTextureStorage1D)(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width);
extern void (APIENTRY *gglTextureStorage2D)(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
extern void (APIENTRY *gglTextureStorage3D)(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
extern void (APIENTRY *gglTextureStorage2DMultisample)(GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
extern void (APIENTRY *gglTextureStorage3DMultisample)(GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
extern void (APIENTRY *gglTextureSubImage1D)(GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels);
extern void (APIENTRY *gglTextureSubImage2D)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
extern void (APIENTRY *gglTextureSubImage3D)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels);
extern void (APIENTRY *gglCompressedTextureSubImage1D)(GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void *data);
extern void (APIENTRY *gglCompressedTextureSubImage2D)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data);
extern void (APIENTRY *gglCompressedTextureSubImage3D)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data);
extern void (APIENTRY *gglCopyTextureSubImage1D)(GLuint texture, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
extern void (APIENTRY *gglCopyTextureSubImage2D)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
extern void (APIENTRY *gglCopyTextureSubImage3D)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
extern void (APIENTRY *gglTextureParameterf)(GLuint texture, GLenum pname, GLfloat param);
extern void (APIENTRY *gglTextureParameterfv)(GLuint texture, GLenum pname, const GLfloat *param);
extern void (APIENTRY *gglTextureParameteri)(GLuint texture, GLenum pname, GLint param);
extern void (APIENTRY *gglTextureParameterIiv)(GLuint texture, GLenum pname, const GLint *params);
extern void (APIENTRY *gglTextureParameterIuiv)(GLuint texture, GLenum pname, const GLuint *params);
extern void (APIENTRY *gglTextureParameteriv)(GLuint texture, GLenum pname, const GLint *param);
extern void (APIENTRY *gglGenerateTextureMipmap)(GLuint texture);
extern void (APIENTRY *gglBindTextureUnit)(GLuint unit, GLuint texture);
extern void (APIENTRY *gglGetTextureImage)(GLuint texture, GLint level, GLenum format, GLenum type, GLsizei bufSize, void *pixels);
extern void (APIENTRY *gglGetCompressedTextureImage)(GLuint texture, GLint level, GLsizei bufSize, void *pixels);
extern void (APIENTRY *gglGetTextureLevelParameterfv)(GLuint texture, GLint level, GLenum pname, GLfloat *params);
extern void (APIENTRY *gglGetTextureLevelParameteriv)(GLuint texture, GLint level, GLenum pname, GLint *params);
extern void (APIENTRY *gglGetTextureParameterfv)(GLuint texture, GLenum pname, GLfloat *params);
extern void (APIENTRY *gglGetTextureParameterIiv)(GLuint texture, GLenum pname, GLint *params);
extern void (APIENTRY *gglGetTextureParameterIuiv)(GLuint texture, GLenum pname, GLuint *params);
extern void (APIENTRY *gglGetTextureParameteriv)(GLuint texture, GLenum pname, GLint *params);
extern void (APIENTRY *gglCreateVertexArrays)(GLsizei n, GLuint *arrays);
extern void (APIENTRY *gglDisableVertexArrayAttrib)(GLuint vaobj, GLuint index);
extern void (APIENTRY *gglEnableVertexArrayAttrib)(GLuint vaobj, GLuint index);
extern void (APIENTRY *gglVertexArrayElementBuffer)(GLuint vaobj, GLuint buffer);
extern void (APIENTRY *gglVertexArrayVertexBuffer)(GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
extern void (APIENTRY *gglVertexArrayVertexBuffers)(GLuint vaobj, GLuint first, GLsizei count, const GLuint *buffers, const GLintptr *offsets, const GLsizei *strides);
extern void (APIENTRY *gglVertexArrayAttribBinding)(GLuint vaobj, GLuint attribindex, GLuint bindingindex);
extern void (APIENTRY *gglVertexArrayAttribFormat)(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
extern void (APIENTRY *gglVertexArrayAttribIFormat)(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
extern void (APIENTRY *gglVertexArrayAttribLFormat)(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
extern void (APIENTRY *gglVertexArrayBindingDivisor)(GLuint vaobj, GLuint bindingindex, GLuint divisor);
extern void (APIENTRY *gglGetVertexArrayiv)(GLuint vaobj, GLenum pname, GLint *param);
extern void (APIENTRY *gglGetVertexArrayIndexediv)(GLuint vaobj, GLuint index, GLenum pname, GLint *param);
extern void (APIENTRY *gglGetVertexArrayIndexed64iv)(GLuint vaobj, GLuint index, GLenum pname, GLint64 *param);
extern void (APIENTRY *gglCreateSamplers)(GLsizei n, GLuint *samplers);
extern void (APIENTRY *gglCreateProgramPipelines)(GLsizei n, GLuint *pipelines);
extern void (APIENTRY *gglCreateQueries)(GLenum target, GLsizei n, GLuint *ids);
extern void (APIENTRY *gglGetQueryBufferObjecti64v)(GLuint id, GLuint buffer, GLenum pname, GLintptr offset);
extern void (APIENTRY *gglGetQueryBufferObjectiv)(GLuint id, GLuint buffer, GLenum pname, GLintptr offset);
extern void (APIENTRY *gglGetQueryBufferObjectui64v)(GLuint id, GLuint buffer, GLenum pname, GLintptr offset);
extern void (APIENTRY *gglGetQueryBufferObjectuiv)(GLuint id, GLuint buffer, GLenum pname, GLintptr offset);
extern void (APIENTRY *gglMemoryBarrierByRegion)(GLbitfield barriers);
extern void (APIENTRY *gglGetTextureSubImage)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, GLsizei bufSize, void *pixels);
extern void (APIENTRY *gglGetCompressedTextureSubImage)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLsizei bufSize, void *pixels);
extern GLenum (APIENTRY *gglGetGraphicsResetStatus)(void);
extern void (APIENTRY *gglGetnCompressedTexImage)(GLenum target, GLint lod, GLsizei bufSize, void *pixels);
extern void (APIENTRY *gglGetnTexImage)(GLenum target, GLint level, GLenum format, GLenum type, GLsizei bufSize, void *pixels);
extern void (APIENTRY *gglGetnUniformdv)(GLuint program, GLint location, GLsizei bufSize, GLdouble *params);
extern void (APIENTRY *gglGetnUniformfv)(GLuint program, GLint location, GLsizei bufSize, GLfloat *params);
extern void (APIENTRY *gglGetnUniformiv)(GLuint program, GLint location, GLsizei bufSize, GLint *params);
extern void (APIENTRY *gglGetnUniformuiv)(GLuint program, GLint location, GLsizei bufSize, GLuint *params);
extern void (APIENTRY *gglReadnPixels)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void *data);
extern void (APIENTRY *gglGetnMapdv)(GLenum target, GLenum query, GLsizei bufSize, GLdouble *v);
extern void (APIENTRY *gglGetnMapfv)(GLenum target, GLenum query, GLsizei bufSize, GLfloat *v);
extern void (APIENTRY *gglGetnMapiv)(GLenum target, GLenum query, GLsizei bufSize, GLint *v);
extern void (APIENTRY *gglGetnPixelMapfv)(GLenum map, GLsizei bufSize, GLfloat *values);
extern void (APIENTRY *gglGetnPixelMapuiv)(GLenum map, GLsizei bufSize, GLuint *values);
extern void (APIENTRY *gglGetnPixelMapusv)(GLenum map, GLsizei bufSize, GLushort *values);
extern void (APIENTRY *gglGetnPolygonStipple)(GLsizei bufSize, GLubyte *pattern);
extern void (APIENTRY *gglGetnColorTable)(GLenum target, GLenum format, GLenum type, GLsizei bufSize, void *table);
extern void (APIENTRY *gglGetnConvolutionFilter)(GLenum target, GLenum format, GLenum type, GLsizei bufSize, void *image);
extern void (APIENTRY *gglGetnSeparableFilter)(GLenum target, GLenum format, GLenum type, GLsizei rowBufSize, void *row, GLsizei columnBufSize, void *column, void *span);
extern void (APIENTRY *gglGetnHistogram)(GLenum target, GLboolean reset, GLenum format, GLenum type, GLsizei bufSize, void *values);
extern void (APIENTRY *gglGetnMinmax)(GLenum target, GLboolean reset, GLenum format, GLenum type, GLsizei bufSize, void *values);
extern void (APIENTRY *gglTextureBarrier)(void);

//GL_STANDARD
extern void (APIENTRY *gglAccum)(GLenum op, GLfloat value);
extern void (APIENTRY *gglAlphaFunc)(GLenum func, GLclampf ref);
extern void (APIENTRY *gglBegin)(GLenum mode);
extern void (APIENTRY *gglBitmap)(GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte *bitmap);
extern void (APIENTRY *gglBlendFunc)(GLenum sfactor, GLenum dfactor);
extern void (APIENTRY *gglCallList)(GLuint list);
extern void (APIENTRY *gglCallLists)(GLsizei n, GLenum type, const GLvoid *lists);
extern void (APIENTRY *gglClear)(GLbitfield mask);
extern void (APIENTRY *gglClearAccum)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
extern void (APIENTRY *gglClearColor)(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
extern void (APIENTRY *gglClearDepth)(GLclampd depth);
extern void (APIENTRY *gglClearIndex)(GLfloat c);
extern void (APIENTRY *gglClearStencil)(GLint s);
extern void (APIENTRY *gglClipPlane)(GLenum plane, const GLdouble *equation);
extern void (APIENTRY *gglColor3b)(GLbyte red, GLbyte green, GLbyte blue);
extern void (APIENTRY *gglColor3bv)(const GLbyte *v);
extern void (APIENTRY *gglColor3d)(GLdouble red, GLdouble green, GLdouble blue);
extern void (APIENTRY *gglColor3dv)(const GLdouble *v);
extern void (APIENTRY *gglColor3f)(GLfloat red, GLfloat green, GLfloat blue);
extern void (APIENTRY *gglColor3fv)(const GLfloat *v);
extern void (APIENTRY *gglColor3i)(GLint red, GLint green, GLint blue);
extern void (APIENTRY *gglColor3iv)(const GLint *v);
extern void (APIENTRY *gglColor3s)(GLshort red, GLshort green, GLshort blue);
extern void (APIENTRY *gglColor3sv)(const GLshort *v);
extern void (APIENTRY *gglColor3ub)(GLubyte red, GLubyte green, GLubyte blue);
extern void (APIENTRY *gglColor3ubv)(const GLubyte *v);
extern void (APIENTRY *gglColor3ui)(GLuint red, GLuint green, GLuint blue);
extern void (APIENTRY *gglColor3uiv)(const GLuint *v);
extern void (APIENTRY *gglColor3us)(GLushort red, GLushort green, GLushort blue);
extern void (APIENTRY *gglColor3usv)(const GLushort *v);
extern void (APIENTRY *gglColor4b)(GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha);
extern void (APIENTRY *gglColor4bv)(const GLbyte *v);
extern void (APIENTRY *gglColor4d)(GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha);
extern void (APIENTRY *gglColor4dv)(const GLdouble *v);
extern void (APIENTRY *gglColor4f)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
extern void (APIENTRY *gglColor4fv)(const GLfloat *v);
extern void (APIENTRY *gglColor4i)(GLint red, GLint green, GLint blue, GLint alpha);
extern void (APIENTRY *gglColor4iv)(const GLint *v);
extern void (APIENTRY *gglColor4s)(GLshort red, GLshort green, GLshort blue, GLshort alpha);
extern void (APIENTRY *gglColor4sv)(const GLshort *v);
extern void (APIENTRY *gglColor4ub)(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
extern void (APIENTRY *gglColor4ubv)(const GLubyte *v);
extern void (APIENTRY *gglColor4ui)(GLuint red, GLuint green, GLuint blue, GLuint alpha);
extern void (APIENTRY *gglColor4uiv)(const GLuint *v);
extern void (APIENTRY *gglColor4us)(GLushort red, GLushort green, GLushort blue, GLushort alpha);
extern void (APIENTRY *gglColor4usv)(const GLushort *v);
extern void (APIENTRY *gglColorMask)(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
extern void (APIENTRY *gglColorMaterial)(GLenum face, GLenum mode);
extern void (APIENTRY *gglCopyPixels)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum type);
extern void (APIENTRY *gglCullFace)(GLenum mode);
extern void (APIENTRY *gglDeleteLists)(GLuint list, GLsizei range);
extern void (APIENTRY *gglDepthFunc)(GLenum func);
extern void (APIENTRY *gglDepthMask)(GLboolean flag);
extern void (APIENTRY *gglDepthRange)(GLclampd zNear, GLclampd zFar);
extern void (APIENTRY *gglDisable)(GLenum cap);
extern void (APIENTRY *gglDrawBuffer)(GLenum mode);
extern void (APIENTRY *gglDrawPixels)(GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
extern void (APIENTRY *gglEdgeFlag)(GLboolean flag);
extern void (APIENTRY *gglEdgeFlagv)(const GLboolean *flag);
extern void (APIENTRY *gglEnable)(GLenum cap);
extern void (APIENTRY *gglEnd)(void);
extern void (APIENTRY *gglEndList)(void);
extern void (APIENTRY *gglEvalCoord1d)(GLdouble u);
extern void (APIENTRY *gglEvalCoord1dv)(const GLdouble *u);
extern void (APIENTRY *gglEvalCoord1f)(GLfloat u);
extern void (APIENTRY *gglEvalCoord1fv)(const GLfloat *u);
extern void (APIENTRY *gglEvalCoord2d)(GLdouble u, GLdouble v);
extern void (APIENTRY *gglEvalCoord2dv)(const GLdouble *u);
extern void (APIENTRY *gglEvalCoord2f)(GLfloat u, GLfloat v);
extern void (APIENTRY *gglEvalCoord2fv)(const GLfloat *u);
extern void (APIENTRY *gglEvalMesh1)(GLenum mode, GLint i1, GLint i2);
extern void (APIENTRY *gglEvalMesh2)(GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2);
extern void (APIENTRY *gglEvalPoint1)(GLint i);
extern void (APIENTRY *gglEvalPoint2)(GLint i, GLint j);
extern void (APIENTRY *gglFeedbackBuffer)(GLsizei size, GLenum type, GLfloat *buffer);
extern void (APIENTRY *gglFinish)(void);
extern void (APIENTRY *gglFlush)(void);
extern void (APIENTRY *gglFogf)(GLenum pname, GLfloat param);
extern void (APIENTRY *gglFogfv)(GLenum pname, const GLfloat *params);
extern void (APIENTRY *gglFogi)(GLenum pname, GLint param);
extern void (APIENTRY *gglFogiv)(GLenum pname, const GLint *params);
extern void (APIENTRY *gglFrontFace)(GLenum mode);
extern void (APIENTRY *gglFrustum)(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
extern GLuint (APIENTRY *gglGenLists)(GLsizei range);
extern void (APIENTRY *gglGetBooleanv)(GLenum pname, GLboolean *params);
extern void (APIENTRY *gglGetClipPlane)(GLenum plane, GLdouble *equation);
extern void (APIENTRY *gglGetDoublev)(GLenum pname, GLdouble *params);
extern GLenum (APIENTRY *gglGetError)(void);
extern void (APIENTRY *gglGetFloatv)(GLenum pname, GLfloat *params);
extern void (APIENTRY *gglGetIntegerv)(GLenum pname, GLint *params);
extern void (APIENTRY *gglGetLightfv)(GLenum light, GLenum pname, GLfloat *params);
extern void (APIENTRY *gglGetLightiv)(GLenum light, GLenum pname, GLint *params);
extern void (APIENTRY *gglGetMapdv)(GLenum target, GLenum query, GLdouble *v);
extern void (APIENTRY *gglGetMapfv)(GLenum target, GLenum query, GLfloat *v);
extern void (APIENTRY *gglGetMapiv)(GLenum target, GLenum query, GLint *v);
extern void (APIENTRY *gglGetMaterialfv)(GLenum face, GLenum pname, GLfloat *params);
extern void (APIENTRY *gglGetMaterialiv)(GLenum face, GLenum pname, GLint *params);
extern void (APIENTRY *gglGetPixelMapfv)(GLenum map, GLfloat *values);
extern void (APIENTRY *gglGetPixelMapuiv)(GLenum map, GLuint *values);
extern void (APIENTRY *gglGetPixelMapusv)(GLenum map, GLushort *values);
extern void (APIENTRY *gglGetPolygonStipple)(GLubyte *mask);
extern const GLubyte *(APIENTRY *gglGetString)(GLenum name);
extern void (APIENTRY *gglGetTexEnvfv)(GLenum target, GLenum pname, GLfloat *params);
extern void (APIENTRY *gglGetTexEnviv)(GLenum target, GLenum pname, GLint *params);
extern void (APIENTRY *gglGetTexGendv)(GLenum coord, GLenum pname, GLdouble *params);
extern void (APIENTRY *gglGetTexGenfv)(GLenum coord, GLenum pname, GLfloat *params);
extern void (APIENTRY *gglGetTexGeniv)(GLenum coord, GLenum pname, GLint *params);
extern void (APIENTRY *gglGetTexImage)(GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels);
extern void (APIENTRY *gglGetTexLevelParameterfv)(GLenum target, GLint level, GLenum pname, GLfloat *params);
extern void (APIENTRY *gglGetTexLevelParameteriv)(GLenum target, GLint level, GLenum pname, GLint *params);
extern void (APIENTRY *gglGetTexParameterfv)(GLenum target, GLenum pname, GLfloat *params);
extern void (APIENTRY *gglGetTexParameteriv)(GLenum target, GLenum pname, GLint *params);
extern void (APIENTRY *gglHint)(GLenum target, GLenum mode);
extern void (APIENTRY *gglIndexMask)(GLuint mask);
extern void (APIENTRY *gglIndexd)(GLdouble c);
extern void (APIENTRY *gglIndexdv)(const GLdouble *c);
extern void (APIENTRY *gglIndexf)(GLfloat c);
extern void (APIENTRY *gglIndexfv)(const GLfloat *c);
extern void (APIENTRY *gglIndexi)(GLint c);
extern void (APIENTRY *gglIndexiv)(const GLint *c);
extern void (APIENTRY *gglIndexs)(GLshort c);
extern void (APIENTRY *gglIndexsv)(const GLshort *c);
extern void (APIENTRY *gglInitNames)(void);
extern GLboolean (APIENTRY *gglIsEnabled)(GLenum cap);
extern GLboolean (APIENTRY *gglIsList)(GLuint list);
extern void (APIENTRY *gglLightModelf)(GLenum pname, GLfloat param);
extern void (APIENTRY *gglLightModelfv)(GLenum pname, const GLfloat *params);
extern void (APIENTRY *gglLightModeli)(GLenum pname, GLint param);
extern void (APIENTRY *gglLightModeliv)(GLenum pname, const GLint *params);
extern void (APIENTRY *gglLightf)(GLenum light, GLenum pname, GLfloat param);
extern void (APIENTRY *gglLightfv)(GLenum light, GLenum pname, const GLfloat *params);
extern void (APIENTRY *gglLighti)(GLenum light, GLenum pname, GLint param);
extern void (APIENTRY *gglLightiv)(GLenum light, GLenum pname, const GLint *params);
extern void (APIENTRY *gglLineStipple)(GLint factor, GLushort pattern);
extern void (APIENTRY *gglLineWidth)(GLfloat width);
extern void (APIENTRY *gglListBase)(GLuint base);
extern void (APIENTRY *gglLoadIdentity)(void);
extern void (APIENTRY *gglLoadMatrixd)(const GLdouble *m);
extern void (APIENTRY *gglLoadMatrixf)(const GLfloat *m);
extern void (APIENTRY *gglLoadName)(GLuint name);
extern void (APIENTRY *gglLogicOp)(GLenum opcode);
extern void (APIENTRY *gglMap1d)(GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble *points);
extern void (APIENTRY *gglMap1f)(GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points);
extern void (APIENTRY *gglMap2d)(GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble *points);
extern void (APIENTRY *gglMap2f)(GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat *points);
extern void (APIENTRY *gglMapGrid1d)(GLint un, GLdouble u1, GLdouble u2);
extern void (APIENTRY *gglMapGrid1f)(GLint un, GLfloat u1, GLfloat u2);
extern void (APIENTRY *gglMapGrid2d)(GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2);
extern void (APIENTRY *gglMapGrid2f)(GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2);
extern void (APIENTRY *gglMaterialf)(GLenum face, GLenum pname, GLfloat param);
extern void (APIENTRY *gglMaterialfv)(GLenum face, GLenum pname, const GLfloat *params);
extern void (APIENTRY *gglMateriali)(GLenum face, GLenum pname, GLint param);
extern void (APIENTRY *gglMaterialiv)(GLenum face, GLenum pname, const GLint *params);
extern void (APIENTRY *gglMatrixMode)(GLenum mode);
extern void (APIENTRY *gglMultMatrixd)(const GLdouble *m);
extern void (APIENTRY *gglMultMatrixf)(const GLfloat *m);
extern void (APIENTRY *gglNewList)(GLuint list, GLenum mode);
extern void (APIENTRY *gglNormal3b)(GLbyte nx, GLbyte ny, GLbyte nz);
extern void (APIENTRY *gglNormal3bv)(const GLbyte *v);
extern void (APIENTRY *gglNormal3d)(GLdouble nx, GLdouble ny, GLdouble nz);
extern void (APIENTRY *gglNormal3dv)(const GLdouble *v);
extern void (APIENTRY *gglNormal3f)(GLfloat nx, GLfloat ny, GLfloat nz);
extern void (APIENTRY *gglNormal3fv)(const GLfloat *v);
extern void (APIENTRY *gglNormal3i)(GLint nx, GLint ny, GLint nz);
extern void (APIENTRY *gglNormal3iv)(const GLint *v);
extern void (APIENTRY *gglNormal3s)(GLshort nx, GLshort ny, GLshort nz);
extern void (APIENTRY *gglNormal3sv)(const GLshort *v);
extern void (APIENTRY *gglOrtho)(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
extern void (APIENTRY *gglPassThrough)(GLfloat token);
extern void (APIENTRY *gglPixelMapfv)(GLenum map, GLint mapsize, const GLfloat *values);
extern void (APIENTRY *gglPixelMapuiv)(GLenum map, GLint mapsize, const GLuint *values);
extern void (APIENTRY *gglPixelMapusv)(GLenum map, GLint mapsize, const GLushort *values);
extern void (APIENTRY *gglPixelStoref)(GLenum pname, GLfloat param);
extern void (APIENTRY *gglPixelStorei)(GLenum pname, GLint param);
extern void (APIENTRY *gglPixelTransferf)(GLenum pname, GLfloat param);
extern void (APIENTRY *gglPixelTransferi)(GLenum pname, GLint param);
extern void (APIENTRY *gglPixelZoom)(GLfloat xfactor, GLfloat yfactor);
extern void (APIENTRY *gglPointSize)(GLfloat size);
extern void (APIENTRY *gglPolygonMode)(GLenum face, GLenum mode);
extern void (APIENTRY *gglPolygonStipple)(const GLubyte *mask);
extern void (APIENTRY *gglPopAttrib)(void);
extern void (APIENTRY *gglPopMatrix)(void);
extern void (APIENTRY *gglPopName)(void);
extern void (APIENTRY *gglPushAttrib)(GLbitfield mask);
extern void (APIENTRY *gglPushMatrix)(void);
extern void (APIENTRY *gglPushName)(GLuint name);
extern void (APIENTRY *gglRasterPos2d)(GLdouble x, GLdouble y);
extern void (APIENTRY *gglRasterPos2dv)(const GLdouble *v);
extern void (APIENTRY *gglRasterPos2f)(GLfloat x, GLfloat y);
extern void (APIENTRY *gglRasterPos2fv)(const GLfloat *v);
extern void (APIENTRY *gglRasterPos2i)(GLint x, GLint y);
extern void (APIENTRY *gglRasterPos2iv)(const GLint *v);
extern void (APIENTRY *gglRasterPos2s)(GLshort x, GLshort y);
extern void (APIENTRY *gglRasterPos2sv)(const GLshort *v);
extern void (APIENTRY *gglRasterPos3d)(GLdouble x, GLdouble y, GLdouble z);
extern void (APIENTRY *gglRasterPos3dv)(const GLdouble *v);
extern void (APIENTRY *gglRasterPos3f)(GLfloat x, GLfloat y, GLfloat z);
extern void (APIENTRY *gglRasterPos3fv)(const GLfloat *v);
extern void (APIENTRY *gglRasterPos3i)(GLint x, GLint y, GLint z);
extern void (APIENTRY *gglRasterPos3iv)(const GLint *v);
extern void (APIENTRY *gglRasterPos3s)(GLshort x, GLshort y, GLshort z);
extern void (APIENTRY *gglRasterPos3sv)(const GLshort *v);
extern void (APIENTRY *gglRasterPos4d)(GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern void (APIENTRY *gglRasterPos4dv)(const GLdouble *v);
extern void (APIENTRY *gglRasterPos4f)(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern void (APIENTRY *gglRasterPos4fv)(const GLfloat *v);
extern void (APIENTRY *gglRasterPos4i)(GLint x, GLint y, GLint z, GLint w);
extern void (APIENTRY *gglRasterPos4iv)(const GLint *v);
extern void (APIENTRY *gglRasterPos4s)(GLshort x, GLshort y, GLshort z, GLshort w);
extern void (APIENTRY *gglRasterPos4sv)(const GLshort *v);
extern void (APIENTRY *gglReadBuffer)(GLenum mode);
extern void (APIENTRY *gglReadPixels)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
extern void (APIENTRY *gglRectd)(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2);
extern void (APIENTRY *gglRectdv)(const GLdouble *v1, const GLdouble *v2);
extern void (APIENTRY *gglRectf)(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
extern void (APIENTRY *gglRectfv)(const GLfloat *v1, const GLfloat *v2);
extern void (APIENTRY *gglRecti)(GLint x1, GLint y1, GLint x2, GLint y2);
extern void (APIENTRY *gglRectiv)(const GLint *v1, const GLint *v2);
extern void (APIENTRY *gglRects)(GLshort x1, GLshort y1, GLshort x2, GLshort y2);
extern void (APIENTRY *gglRectsv)(const GLshort *v1, const GLshort *v2);
extern GLint (APIENTRY *gglRenderMode)(GLenum mode);
extern void (APIENTRY *gglRotated)(GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
extern void (APIENTRY *gglRotatef)(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
extern void (APIENTRY *gglScaled)(GLdouble x, GLdouble y, GLdouble z);
extern void (APIENTRY *gglScalef)(GLfloat x, GLfloat y, GLfloat z);
extern void (APIENTRY *gglScissor)(GLint x, GLint y, GLsizei width, GLsizei height);
extern void (APIENTRY *gglSelectBuffer)(GLsizei size, GLuint *buffer);
extern void (APIENTRY *gglShadeModel)(GLenum mode);
extern void (APIENTRY *gglStencilFunc)(GLenum func, GLint ref, GLuint mask);
extern void (APIENTRY *gglStencilMask)(GLuint mask);
extern void (APIENTRY *gglStencilOp)(GLenum fail, GLenum zfail, GLenum zpass);
extern void (APIENTRY *gglTexCoord1d)(GLdouble s);
extern void (APIENTRY *gglTexCoord1dv)(const GLdouble *v);
extern void (APIENTRY *gglTexCoord1f)(GLfloat s);
extern void (APIENTRY *gglTexCoord1fv)(const GLfloat *v);
extern void (APIENTRY *gglTexCoord1i)(GLint s);
extern void (APIENTRY *gglTexCoord1iv)(const GLint *v);
extern void (APIENTRY *gglTexCoord1s)(GLshort s);
extern void (APIENTRY *gglTexCoord1sv)(const GLshort *v);
extern void (APIENTRY *gglTexCoord2d)(GLdouble s, GLdouble t);
extern void (APIENTRY *gglTexCoord2dv)(const GLdouble *v);
extern void (APIENTRY *gglTexCoord2f)(GLfloat s, GLfloat t);
extern void (APIENTRY *gglTexCoord2fv)(const GLfloat *v);
extern void (APIENTRY *gglTexCoord2i)(GLint s, GLint t);
extern void (APIENTRY *gglTexCoord2iv)(const GLint *v);
extern void (APIENTRY *gglTexCoord2s)(GLshort s, GLshort t);
extern void (APIENTRY *gglTexCoord2sv)(const GLshort *v);
extern void (APIENTRY *gglTexCoord3d)(GLdouble s, GLdouble t, GLdouble r);
extern void (APIENTRY *gglTexCoord3dv)(const GLdouble *v);
extern void (APIENTRY *gglTexCoord3f)(GLfloat s, GLfloat t, GLfloat r);
extern void (APIENTRY *gglTexCoord3fv)(const GLfloat *v);
extern void (APIENTRY *gglTexCoord3i)(GLint s, GLint t, GLint r);
extern void (APIENTRY *gglTexCoord3iv)(const GLint *v);
extern void (APIENTRY *gglTexCoord3s)(GLshort s, GLshort t, GLshort r);
extern void (APIENTRY *gglTexCoord3sv)(const GLshort *v);
extern void (APIENTRY *gglTexCoord4d)(GLdouble s, GLdouble t, GLdouble r, GLdouble q);
extern void (APIENTRY *gglTexCoord4dv)(const GLdouble *v);
extern void (APIENTRY *gglTexCoord4f)(GLfloat s, GLfloat t, GLfloat r, GLfloat q);
extern void (APIENTRY *gglTexCoord4fv)(const GLfloat *v);
extern void (APIENTRY *gglTexCoord4i)(GLint s, GLint t, GLint r, GLint q);
extern void (APIENTRY *gglTexCoord4iv)(const GLint *v);
extern void (APIENTRY *gglTexCoord4s)(GLshort s, GLshort t, GLshort r, GLshort q);
extern void (APIENTRY *gglTexCoord4sv)(const GLshort *v);
extern void (APIENTRY *gglTexEnvf)(GLenum target, GLenum pname, GLfloat param);
extern void (APIENTRY *gglTexEnvfv)(GLenum target, GLenum pname, const GLfloat *params);
extern void (APIENTRY *gglTexEnvi)(GLenum target, GLenum pname, GLint param);
extern void (APIENTRY *gglTexEnviv)(GLenum target, GLenum pname, const GLint *params);
extern void (APIENTRY *gglTexGend)(GLenum coord, GLenum pname, GLdouble param);
extern void (APIENTRY *gglTexGendv)(GLenum coord, GLenum pname, const GLdouble *params);
extern void (APIENTRY *gglTexGenf)(GLenum coord, GLenum pname, GLfloat param);
extern void (APIENTRY *gglTexGenfv)(GLenum coord, GLenum pname, const GLfloat *params);
extern void (APIENTRY *gglTexGeni)(GLenum coord, GLenum pname, GLint param);
extern void (APIENTRY *gglTexGeniv)(GLenum coord, GLenum pname, const GLint *params);
extern void (APIENTRY *gglTexImage1D)(GLenum target, GLint level, GLint components, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
extern void (APIENTRY *gglTexImage2D)(GLenum target, GLint level, GLint components, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
extern void (APIENTRY *gglTexParameterf)(GLenum target, GLenum pname, GLfloat param);
extern void (APIENTRY *gglTexParameterfv)(GLenum target, GLenum pname, const GLfloat *params);
extern void (APIENTRY *gglTexParameteri)(GLenum target, GLenum pname, GLint param);
extern void (APIENTRY *gglTexParameteriv)(GLenum target, GLenum pname, const GLint *params);
extern void (APIENTRY *gglTranslated)(GLdouble x, GLdouble y, GLdouble z);
extern void (APIENTRY *gglTranslatef)(GLfloat x, GLfloat y, GLfloat z);
extern void (APIENTRY *gglVertex2d)(GLdouble x, GLdouble y);
extern void (APIENTRY *gglVertex2dv)(const GLdouble *v);
extern void (APIENTRY *gglVertex2f)(GLfloat x, GLfloat y);
extern void (APIENTRY *gglVertex2fv)(const GLfloat *v);
extern void (APIENTRY *gglVertex2i)(GLint x, GLint y);
extern void (APIENTRY *gglVertex2iv)(const GLint *v);
extern void (APIENTRY *gglVertex2s)(GLshort x, GLshort y);
extern void (APIENTRY *gglVertex2sv)(const GLshort *v);
extern void (APIENTRY *gglVertex3d)(GLdouble x, GLdouble y, GLdouble z);
extern void (APIENTRY *gglVertex3dv)(const GLdouble *v);
extern void (APIENTRY *gglVertex3f)(GLfloat x, GLfloat y, GLfloat z);
extern void (APIENTRY *gglVertex3fv)(const GLfloat *v);
extern void (APIENTRY *gglVertex3i)(GLint x, GLint y, GLint z);
extern void (APIENTRY *gglVertex3iv)(const GLint *v);
extern void (APIENTRY *gglVertex3s)(GLshort x, GLshort y, GLshort z);
extern void (APIENTRY *gglVertex3sv)(const GLshort *v);
extern void (APIENTRY *gglVertex4d)(GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern void (APIENTRY *gglVertex4dv)(const GLdouble *v);
extern void (APIENTRY *gglVertex4f)(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern void (APIENTRY *gglVertex4fv)(const GLfloat *v);
extern void (APIENTRY *gglVertex4i)(GLint x, GLint y, GLint z, GLint w);
extern void (APIENTRY *gglVertex4iv)(const GLint *v);
extern void (APIENTRY *gglVertex4s)(GLshort x, GLshort y, GLshort z, GLshort w);
extern void (APIENTRY *gglVertex4sv)(const GLshort *v);
extern void (APIENTRY *gglViewport)(GLint x, GLint y, GLsizei width, GLsizei height);

typedef enum
{
	//wgl extensions
	GWGL_ARB_buffer_region,							//4
	GWGL_ARB_multisample,							//5
	GWGL_ARB_extensions_string,						//8
	GWGL_ARB_pixel_format,							//9
	GWGL_ARB_make_current_read,						//10
	GWGL_ARB_pbuffer,								//11
	GWGL_ARB_render_texture,						//20
	GWGL_ARB_pixel_format_float,					//39
	GWGL_ARB_framebuffer_sRGB,						//46
	GWGL_ARB_create_context,						//55
	GWGL_ARB_create_context_profile,				//74	//TODO:
	GWGL_ARB_create_context_robustness,				//102	//TODO:
	GWGL_ARB_robustness_application_isolation,		//143	//TODO:
	GWGL_ARB_robustness_share_group_isolation,		//143	//TODO:
	GWGL_ARB_context_flush_control,					//168	//TODO:

	GWGL_EXT_display_color_table,					//167
	GWGL_EXT_extensions_string,						//168
	GWGL_EXT_make_current_read,						//169
	GWGL_EXT_pixel_format,							//170
	GWGL_EXT_pbuffer,								//171
	GWGL_EXT_swap_control,							//172
	GWGL_EXT_depth_float,							//177
	GWGL_NV_vertex_array_range,						//190
	GWGL_3DFX_multisample,							//207
	GWGL_EXT_multisample,							//209
	GWGL_OML_sync_control,							//242
	GWGL_I3D_digital_video_control,					//250
	GWGL_I3D_gamma,									//251
	GWGL_I3D_genlock,								//252
	GWGL_I3D_image_buffer,							//253
	GWGL_I3D_swap_frame_lock,						//254
	GWGL_I3D_swap_frame_usage,						//255
	GWGL_NV_render_depth_texture,					//263
	GWGL_NV_render_texture_rectangle,				//264
	GWGL_ATI_pixel_format_float,					//278
	GWGL_NV_float_buffer,							//281
	GWGL_ATI_render_texture_rectangle,				//?
	GWGL_3DL_stereo_control,						//313
	GWGL_EXT_pixel_format_packed_float,				//328
	GWGL_EXT_framebuffer_sRGB,						//337
	GWGL_NV_present_video,							//347
	GWGL_NV_video_out,								//349
	GWGL_NV_swap_group,								//351
	GWGL_NV_gpu_affinity,							//355
	GWGL_AMD_gpu_association,						//361	//TODO:
	GWGL_NV_video_capture,							//374	//TODO:
	GWGL_NV_copy_image,								//376	//TODO:
	GWGL_EXT_create_context_es2_profile,			//400	//TODO:
	GWGL_EXT_create_context_es_profile,				//400	//TODO:
	GWGL_NV_DX_interop,								//407	//TODO:
	GWGL_NV_DX_interop2,							//412	//TODO:
	GWGL_EXT_swap_control_tear,						//415	//TODO:
	GWGL_NV_delay_before_swap,						//436	//TODO:
	GWGL_3DFX_gamma_control,						//TODO: ???
	GWGL_ATI_pbuffer_memory_hint,					//TODO: ???
	GWGL_EXT_buffer_region,							//TODO: ???
	GWGL_EXT_gamma_control,							//TODO: ???
	GWGL_EXT_render_texture,						//TODO: ???
	GWGL_EXT_swap_interval,							//TODO: ???
	GWGL_MTX_video_preview,							//TODO: ???
	GWGL_NV_multisample_coverage,					//TODO: ???
	GWGL_NV_video_output,							//TODO: ??? WGL_NV_video_out???

	//gl extensions
	GGL_ARB_imaging,
	GGL_ARB_multitexture,							//1
	GGL_ARB_transpose_matrix,						//3
	GGL_ARB_multisample,							//5
	GGL_ARB_texture_env_add,						//6
	GGL_ARB_texture_cube_map,						//7
	GGL_ARB_texture_compression,					//12
	GGL_ARB_texture_border_clamp,					//13
	GGL_ARB_point_parameters,						//14
	GGL_ARB_vertex_blend,							//15
	GGL_ARB_matrix_palette,							//16
	GGL_ARB_texture_env_combine,					//17
	GGL_ARB_texture_env_crossbar,					//18
	GGL_ARB_texture_env_dot3,						//19
	GGL_ARB_texture_mirrored_repeat,				//21
	GGL_ARB_depth_texture,							//22
	GGL_ARB_shadow,									//23
	GGL_ARB_shadow_ambient,							//24
	GGL_ARB_window_pos,								//25
	GGL_ARB_vertex_program,							//26
	GGL_ARB_fragment_program,						//27
	GGL_ARB_vertex_buffer_object,					//28
	GGL_ARB_occlusion_query,						//29
	GGL_ARB_shader_objects,							//30
	GGL_ARB_vertex_shader,							//31
	GGL_ARB_fragment_shader,						//32
	GGL_ARB_shading_language_100,					//33
	GGL_ARB_texture_non_power_of_two,				//34
	GGL_ARB_point_sprite,							//35
	GGL_ARB_fragment_program_shadow,				//36
	GGL_ARB_draw_buffers,							//37
	GGL_ARB_texture_rectangle,						//38
	GGL_ARB_color_buffer_float,						//39
	GGL_ARB_half_float_pixel,						//40
	GGL_ARB_texture_float,							//41
	GGL_ARB_pixel_buffer_object,					//42
	GGL_ARB_depth_buffer_float,						//43
	GGL_ARB_draw_instanced,							//44
	GGL_ARB_framebuffer_object,						//45
	GGL_ARB_framebuffer_sRGB,						//46
	GGL_ARB_geometry_shader4,						//47
	GGL_ARB_half_float_vertex,						//48
	GGL_ARB_instanced_arrays,						//49
	GGL_ARB_map_buffer_range,						//50
	GGL_ARB_texture_buffer_object,					//51
	GGL_ARB_texture_compression_rgtc,				//52
	GGL_ARB_texture_rg,								//53
	GGL_ARB_vertex_array_object,					//54
	GGL_ARB_uniform_buffer_object,					//57	//TODO:
	GGL_ARB_compatibility,							//58	//TODO:
	GGL_ARB_copy_buffer,							//59	//TODO:
	GGL_ARB_shader_texture_lod,						//60	//TODO:
	GGL_ARB_depth_clamp,							//61	//TODO:
	GGL_ARB_draw_elements_base_vertex,				//62	//TODO:
	GGL_ARB_fragment_coord_conventions,				//63	//TODO:
	GGL_ARB_provoking_vertex,						//64	//TODO:
	GGL_ARB_seamless_cube_map,						//65	//TODO:
	GGL_ARB_sync,									//66	//TODO:
	GGL_ARB_texture_multisample,					//67	//TODO:
	GGL_ARB_vertex_array_bgra,						//68	//TODO:
	GGL_ARB_draw_buffers_blend,						//69	//TODO:
	GGL_ARB_sample_shading,							//70	//TODO:
	GGL_ARB_texture_cube_map_array,					//71	//TODO:
	GGL_ARB_texture_gather,							//72	//TODO:
	GGL_ARB_texture_query_lod,						//73	//TODO:
	GGL_ARB_shading_language_include,				//76	//TODO:
	GGL_ARB_texture_compression_bptc,				//77	//TODO:
	GGL_ARB_blend_func_extended,					//78	//TODO:
	GGL_ARB_explicit_attrib_location,				//79	//TODO:
	GGL_ARB_occlusion_query2,						//80	//TODO:
	GGL_ARB_sampler_objects,						//81	//TODO:
	GGL_ARB_shader_bit_encoding,					//82	//TODO:
	GGL_ARB_texture_rgb10_a2ui,						//83	//TODO:
	GGL_ARB_texture_swizzle,						//84	//TODO:
	GGL_ARB_timer_query,							//85	//TODO:
	GGL_ARB_vertex_type_2_10_10_10_rev,				//86	//TODO:
	GGL_ARB_draw_indirect,							//87	//TODO:
	GGL_ARB_gpu_shader5,							//88	//TODO:
	GGL_ARB_gpu_shader_fp64,						//89	//TODO:
	GGL_ARB_shader_subroutine,						//90	//TODO:
	GGL_ARB_tesselation_shader,						//91	//TODO:
	GGL_ARB_texture_buffer_object_rgb32,			//92	//TODO:
	GGL_ARB_transform_feedback2,					//93	//TODO:
	GGL_ARB_transform_feedback3,					//94	//TODO:
	GGL_ARB_ES2_compatibility,						//95	//TODO:
	GGL_ARB_get_program_binary,						//96	//TODO:
	GGL_ARB_separate_shader_objects,				//97	//TODO:
	GGL_ARB_shader_precision,						//98	//TODO:
	GGL_ARB_vertex_attrib_64bit,					//99	//TODO:
	GGL_ARB_viewport_array,							//100	//TODO:
	GGL_ARB_cl_event,								//103	//TODO:
	GGL_ARB_debug_output,							//104	//TODO:
	GGL_ARB_robustness,								//105	//TODO:
	GGL_ARB_shader_stencil_export,					//106	//TODO:
	GGL_ARB_base_instance,							//107	//TODO:
	GGL_ARB_shading_language_420pack,				//108	//TODO:
	GGL_ARB_transform_feedback_instanced,			//109	//TODO:
	GGL_ARB_compressed_texture_pixel_storage,		//110	//TODO:
	GGL_ARB_conservative_depth,						//111	//TODO:
	GGL_ARB_internalformat_query,					//112	//TODO:
	GGL_ARB_map_buffer_alignment,					//113	//TODO:
	GGL_ARB_shader_atomic_counters,					//114	//TODO:
	GGL_ARB_shader_image_load_store,				//115	//TODO:
	GGL_ARB_shading_language_packing,				//116	//TODO:
	GGL_ARB_texture_storage,						//117	//TODO:
	GGL_ARB_arrays_of_arrays,						//120	//TODO:
	GGL_ARB_clear_buffer_object,					//121	//TODO:
	GGL_ARB_compute_shader,							//122	//TODO:
	GGL_ARB_copy_image,								//123	//TODO:
	GGL_ARB_texture_view,							//124	//TODO:
	GGL_ARB_vertex_attrib_binding,					//125	//TODO:
	GGL_ARB_robustness_isolation,					//126	//TODO:
	GGL_ARB_ES3_compatibility,						//127	//TODO:
	GGL_ARB_explicit_uniform_location,				//128	//TODO:
	GGL_ARB_fragment_layer_viewport,				//129	//TODO:
	GGL_ARB_frambuffer_no_attachments,				//130	//TODO:
	GGL_ARB_internalformat_query2,					//131	//TODO:
	GGL_ARB_invalidate_subdata,						//132	//TODO:
	GGL_ARB_multi_draw_indirect,					//133	//TODO:
	GGL_ARB_program_interface_query,				//134	//TODO:
	GGL_ARB_robust_buffer_access_behavior,			//135	//TODO:
	GGL_ARB_shader_image_size,						//136	//TODO:
	GGL_ARB_shader_storage_buffer_object,			//137	//TODO:
	GGL_ARB_stencil_texturing,						//138	//TODO:
	GGL_ARB_texture_buffer_range,					//139	//TODO:
	GGL_ARB_texture_query_levels,					//140	//TODO:
	GGL_ARB_texture_storage_multisample,			//141	//TODO:
	GGL_ARB_buffer_storage,							//144	//TODO:
	GGL_ARB_clear_texture,							//145	//TODO:
	GGL_ARB_enhanced_layouts,						//146	//TODO:
	GGL_ARB_multi_bind,								//147	//TODO:
	GGL_ARB_query_buffer_object,					//148	//TODO:
	GGL_ARB_texture_mirror_clamp_to_edge,			//149	//TODO:
	GGL_ARB_texture_stencil8,						//150	//TODO:
	GGL_ARB_vertex_type_10f_11f_11f_rev,			//151	//TODO:
	GGL_ARB_bindless_texture,						//152	//TODO:
	GGL_ARB_compute_variable_group_size,			//153	//TODO:
	GGL_ARB_indirect_parameters,					//154	//TODO:
	GGL_ARB_seamless_cubemap_per_texture,			//155	//TODO:
	GGL_ARB_shader_draw_parameters,					//156	//TODO:
	GGL_ARB_shader_group_vote,						//157	//TODO:
	GGL_ARB_sparse_texture,							//158	//TODO:
	GGL_ARB_ES3_1_compatibility,					//159	//TODO:
	GGL_ARB_clip_control,							//160	//TODO:
	GGL_ARB_conditional_render_inverted,			//161	//TODO:
	GGL_ARB_cull_distance,							//162	//TODO:
	GGL_ARB_derivative_control,						//163	//TODO:
	GGL_ARB_direct_state_access,					//164	//TODO:
	GGL_ARB_get_texture_sub_image,					//165	//TODO:
	GGL_ARB_shader_texture_image_samples,			//166	//TODO:
	GGL_ARB_texture_barrier,						//167	//TODO:
	GGL_ARB_pipeline_statistics_query,				//171	//TODO:
	GGL_ARB_sparse_buffer,							//172	//TODO:
	GGL_ARB_transform_feedback_overflow_query,		//173	//TODO:
	GGL_ARB_shading_language_120,					//TODO: ???
	GGL_ARB_gpu_shader4,							//TODO: ???
	GGL_ARB_uber_buffers,							//TODO: ???
	GGL_ARB_uber_mem_image,							//TODO: ???
	GGL_ARB_uber_vertex_array,						//TODO: ???
	GGL_ARB_swap_buffers,							//TODO: ???
	GGL_ARB_make_current_read,						//TODO: ??? this is a WGL extension?

	GGL_KHR_texture_compression_astc_hdr,			//118	//TODO:
	GGL_KHR_texture_compression_astc_ldr,			//118	//TODO:
	GGL_KHR_debug,									//119	//TODO:
	GGL_KHR_context_flush_control,					//168	//TODO:
	GGL_KHR_robust_buffer_access_behavior,			//169	//TODO:
	GGL_KHR_robustness,								//170	//TODO:

	GGL_EXT_abgr,									//1
	GGL_EXT_blend_color,							//2
	GGL_EXT_polygon_offset,							//3
	GGL_EXT_texture,								//4
	GGL_EXT_texture3D,								//6
	GGL_SGIS_texture_filter4,						//7
	GGL_EXT_subtexture,								//9
	GGL_EXT_copy_texture,							//10
	GGL_EXT_histogram,								//11
	GGL_EXT_convolution,							//12
	GGL_SGI_color_matrix,							//13
	GGL_SGI_color_table,							//14
	GGL_SGIX_pixel_texture,							//15a
	GGL_SGIS_pixel_texture,							//15
	GGL_SGIS_texture4D,								//16
	GGL_SGI_texture_color_table,					//17
	GGL_EXT_cmyka,									//18
	GGL_EXT_texture_object,							//20
	GGL_SGIS_detail_texture,						//21
	GGL_SGIS_sharpen_texture,						//22
	GGL_EXT_packed_pixels,							//23
	GGL_SGIS_texture_lod,							//24
	GGL_SGIS_multisample,							//25
	GGL_EXT_rescale_normal,							//27
	GGL_EXT_vertex_array,							//30
	GGL_EXT_misc_attribute,							//31
	GGL_SGIS_generate_mipmap,						//32
	GGL_SGIX_clipmap,								//33
	GGL_SGIX_shadow,								//34
	GGL_SGIS_texture_edge_clamp,					//35
	GGL_EXT_texture_edge_clamp,						//??
	GGL_SGIS_texture_border_clamp,					//36
	GGL_EXT_blend_minmax,							//37
	GGL_EXT_blend_subtract,							//38
	GGL_EXT_blend_logic_op,							//39
	GGL_SGIX_interlace,								//45
	GGL_SGIX_pixel_tiles,
	GGL_SGIX_texture_select,						//51
	GGL_SGIX_sprite,								//52
	GGL_SGIX_texture_multi_buffer,					//53
	GGL_EXT_point_parameters,						//54
	GGL_SGIS_point_parameters,
	GGL_SGIX_instruments,							//55
	GGL_SGIX_texture_scale_bias,					//56
	GGL_SGIX_framezoom,								//57
	GGL_SGIX_tag_sample_buffer,						//58
	GGL_SGIX_polynomial_ffd,						//59?
	GGL_SGIX_reference_plane,						//60
	GGL_SGIX_flush_raster,							//61
	GGL_SGIX_depth_texture,							//63
	GGL_SGIS_fog_function,							//64
	GGL_SGIX_fog_offset,							//65
	GGL_HP_image_transform,							//66
	GGL_HP_convolution_border_modes,				//67
	GGL_INGR_palette_buffer,
	GGL_SGIX_texture_add_env,						//69
	GGL_EXT_color_subtable,							//74
	GGL_PGI_vertex_hints,							//76
	GGL_PGI_misc_hints,								//77
	GGL_EXT_paletted_texture,						//78
	GGL_EXT_clip_volume_hint,						//79
	GGL_SGIX_list_priority,							//80
	GGL_SGIX_ir_instrument1,						//81
	GGL_SGIX_calligraphic_fragment,					//82?
	GGL_SGIX_texture_lod_bias,						//84
	GGL_SGIX_shadow_ambient,						//90
	GGL_EXT_index_texture,							//93
	GGL_EXT_index_material,							//94
	GGL_EXT_index_func,								//95
	GGL_EXT_index_array_formats,					//96
	GGL_EXT_compiled_vertex_array,					//97
	GGL_EXT_cull_vertex,							//98
	GGL_SGIX_ycrcb,									//101
	GGL_SGIX_fragment_lighting,
	GGL_EXT_fragment_lighting,						//102
	GGL_IBM_rasterpos_clip,							//110
	GGL_HP_texture_lighting,						//111
	GGL_EXT_draw_range_elements,					//112
	GGL_WIN_phong_shading,							//113
	GGL_WIN_specular_fog,							//114
	GGL_SGIS_color_range,							//115 //Linux only
	GGL_EXT_light_texture,							//117
	GGL_SGIX_blend_alpha_minmax,					//119
	GGL_EXT_scene_marker,							//120
	GGL_SGIX_pixel_texture_bits,					//127
	GGL_SGIX_impact_pixel_texture,
	GGL_EXT_bgra,									//129
	GGL_SGIX_async,									//132
	GGL_SGIX_async_pixel,							//133
	GGL_SGIX_async_histogram,						//134
	GGL_INTEL_texture_scissor,						//135
	GGL_INTEL_parallel_arrays,						//136
	GGL_HP_occlusion_test,							//137
	GGL_EXT_pixel_transform,						//138
	GGL_EXT_pixel_transform_color_table,			//139
	GGL_EXT_shared_texture_palette,					//141
	GGL_EXT_separate_specular_color,				//144
	GGL_EXT_secondary_color,						//145
	GGL_EXT_texture_env,							//146
	GGL_EXT_texture_perturb_normal,					//147
	GGL_EXT_multi_draw_arrays,						//148
	GGL_EXT_fog_coord,								//149
	GGL_REND_screen_coordinates,					//155
	GGL_EXT_coordinate_frame,						//156
	GGL_EXT_texture_env_combine,					//158
	GGL_APPLE_specular_vector,						//159
	GGL_APPLE_transform_hint,						//160
	GGL_SGIX_fog_scale,
	GGL_SUNX_constant_data,							//163
	GGL_SUN_global_alpha,							//164
	GGL_SUN_triangle_list,							//165
	GGL_SUN_vertex,									//166
	GGL_EXT_blend_func_separate,					//173
	GGL_INGR_color_clamp,							//174
	GGL_INGR_interlace_read,						//175
	GGL_EXT_stencil_wrap,							//176
	GGL_EXT_422_pixels,								//178
	GGL_NV_texgen_reflection,						//179
	GGL_EXT_texture_cube_map,						//180?
	GGL_SGIX_texture_range,							//181
	GGL_SUN_convolution_border_modes,				//182
	GGL_EXT_texture_env_add,						//185
	GGL_EXT_texture_lod_bias,						//186
	GGL_EXT_texture_filter_anisotropic,				//187
	GGL_EXT_vertex_weighting,						//188
	GGL_NV_light_max_exponent,						//189
	GGL_NV_vertex_array_range,						//190
	GGL_NV_register_combiners,						//191
	GGL_NV_fog_distance,							//192
	GGL_NV_texgen_emboss,							//193
	GGL_NV_blend_square,							//194
	GGL_NV_texture_env_combine4,					//195
	GGL_MESA_resize_buffers,						//196
	GGL_MESA_window_pos,							//197
	GGL_EXT_texture_compression_s3tc,				//198
	GGL_IBM_cull_vertex,							//199
	GGL_IBM_multimode_draw_arrays,					//200
	GGL_IBM_vertex_array_lists,						//201
	GGL_SGIX_subsample,
	GGL_SGIX_ycrcba,
	GGL_SGIX_ycrcb_subsample,
	GGL_SGIX_depth_pass_instrument,
	GGL_3DFX_texture_compression_FXT1,				//206
	GGL_3DFX_multisample,							//207
	GGL_3DFX_tbuffer,								//208
	GGL_EXT_multisample,							//209
	GGL_SGIX_vertex_preclip,						//210
	GGL_SGIX_convolution_accuracy,					//211?
	GGL_SGIX_resample,								//212
	GGL_SGIS_point_line_texgen,						//213?
	GGL_SGIS_texture_color_mask,					//214
	GGL_SGIX_igloo_interface,						//219?
	GGL_EXT_texture_env_dot3,						//220
	GGL_ATI_texture_mirror_once,					//221
	GGL_NV_fence,									//222
	GGL_IBM_static_data,							//223
	GGL_IBM_texture_mirrored_repeat,				//224
	GGL_NV_evaluators,								//225
	GGL_NV_packed_depth_stencil,					//226
	GGL_NV_register_combiners2,						//227
	GGL_NV_texture_compression_vtc,					//228
	GGL_NV_texture_rectangle,						//229
	GGL_NV_texture_shader,							//230
	GGL_NV_texture_shader2,							//231
	GGL_NV_vertex_array_range2,						//232
	GGL_NV_vertex_program,							//233
	GGL_SGIX_texture_coordinate_clamp,				//235
	GGL_SGIX_scalebias_hint,						//236?
	GGL_OML_interlace,								//239
	GGL_OML_subsample,								//240
	GGL_OML_resample,								//241
	GGL_NV_copy_depth_to_color,						//243
	GGL_ATI_envmap_bumpmap,							//244
	GGL_ATI_fragment_shader,						//245
	GGL_ATI_pn_triangles,							//246
	GGL_ATI_vertex_array_object,					//247
	GGL_EXT_vertex_shader,							//248
	GGL_ATI_vertex_streams,							//249
	GGL_ATI_element_array,							//256
	GGL_SUN_mesh_array,								//257
	GGL_SUN_slice_accum,							//258
	GGL_NV_multisample_filter_hint,					//259
	GGL_NV_depth_clamp,								//260
	GGL_NV_occlusion_query,							//261
	GGL_NV_point_sprite,							//262
	GGL_NV_texture_shader3,							//265
	GGL_NV_vertex_program1_1,						//266
	GGL_EXT_shadow_funcs,							//267
	GGL_EXT_stencil_two_side,						//268
	GGL_ATI_text_fragment_shader,					//269
	GGL_APPLE_client_storage,						//270
	GGL_APPLE_element_array,						//271
	GGL_APPLE_fence,								//272
	GGL_APPLE_vertex_array_object,					//273
	GGL_APPLE_vertex_array_range,					//274
	GGL_APPLE_ycbcr_422,							//275
	GGL_S3_s3tc,									//276
	GGL_ATI_draw_buffers,							//277
	GGL_ATI_texture_env_combine3,					//279
	GGL_ATI_texture_float,							//280
	GGL_NV_float_buffer,							//281
	GGL_NV_fragment_program,						//282
	GGL_NV_half_float,								//283
	GGL_NV_pixel_data_range,						//284
	GGL_NV_primitive_restart,						//285
	GGL_NV_texture_expand_normal,					//286
	GGL_NV_vertex_program2,							//287
	GGL_ATI_map_object_buffer,						//288
	GGL_ATI_separate_stencil,						//289
	GGL_ATI_vertex_attrib_array_object,				//290
	GGL_OES_byte_coordinates,						//291
	GGL_OES_fixed_point,							//292
	GGL_OES_single_precision,						//293
	GGL_OES_compressed_paletted_texture,			//294
	GGL_OES_read_format,							//295
	GGL_OES_query_matrix,							//296
	GGL_EXT_depth_bounds_test,						//297
	GGL_EXT_texture_mirror_clamp,					//298
	GGL_EXT_blend_equation_separate,				//299
	GGL_MESA_pack_invert,							//300
	GGL_MESA_ycbcr_texture,							//301
	GGL_EXT_pixel_buffer_object,					//302
	GGL_NV_fragment_program_option,					//303
	GGL_NV_fragment_program2,						//304
	GGL_NV_vertex_program2_option,					//305
	GGL_NV_vertex_program3,							//306
	GGL_EXT_texture_compression_dxt1,				//309
	GGL_EXT_framebuffer_object,						//310
	GGL_GREMEDY_string_marker,						//311
	GGL_EXT_packed_depth_stencil,					//312
	GGL_EXT_stencil_clear_tag,						//314
	GGL_EXT_texture_sRGB,							//315
	GGL_EXT_framebuffer_blit,						//316
	GGL_EXT_framebuffer_multisample,				//317
	GGL_MESAX_texture_stack,						//318
	GGL_EXT_timer_query,							//319 TODO:
	GGL_EXT_gpu_program_parameters,					//320 TODO:
	GGL_APPLE_flush_buffer_range,					//321 TODO:
	GGL_NV_gpu_program4,							//322 TODO:
	GGL_NV_geometry_program4,						//323 TODO:
	GGL_EXT_geometry_shader4,						//324 TODO:
	GGL_NV_vertex_program4,							//325 TODO:
	GGL_EXT_gpu_shader4,							//326 TODO:
	GGL_EXT_draw_instanced,							//327 TODO:
	GGL_EXT_packed_float,							//328 TODO:
	GGL_EXT_texture_array,							//329 TODO:
	GGL_EXT_texture_buffer_object,					//330 TODO:
	GGL_EXT_texture_compression_latc,				//331 TODO:
	GGL_EXT_texture_compression_rgtc,				//332 TODO:
	GGL_EXT_texture_shared_exponent,				//333 TODO:
	GGL_NV_depth_buffer_float,						//334 TODO:
	GGL_NV_fragment_program4,						//335 TODO:
	GGL_NV_framebuffer_multisample_coverage,		//336 TODO:
	GGL_EXT_framebuffer_sRGB,						//337 TODO:
	GGL_NV_geometry_shader4,						//338 TODO:
	GGL_NV_parameter_buffer_object,					//339 TODO:
	GGL_EXT_draw_buffers2,							//340 TODO:
	GGL_NV_transform_feedback,						//341 TODO:
	GGL_EXT_bindable_uniform,						//342 TODO:
	GGL_EXT_texture_integer,						//343 TODO:
	GGL_GREMEDY_frame_terminator,					//345 TODO:
	GGL_NV_conditional_render,						//346 TODO:
	GGL_NV_present_video,							//347 TODO:
	GGL_EXT_transform_feedback,						//352 TODO:
	GGL_EXT_direct_state_access,					//353 TODO:
	GGL_EXT_vertex_array_bgra,						//354 TODO:
	GGL_EXT_texture_swizzle,						//356 TODO:
	GGL_NV_explicit_multisample,					//357 TODO:
	GGL_NV_transform_feedback2,						//358 TODO:
	GGL_ATI_meminfo,								//359	//TODO:
	GGL_AMD_performance_monitor,					//360	//TODO:
	GGL_AMD_texture_texture4,						//362	//TODO:
	GGL_AMD_vertex_shader_tesselator,				//363	//TODO:
	GGL_EXT_provoking_vertex,						//364	//TODO:
	GGL_EXT_texture_snorm,							//365	//TODO:
	GGL_AMD_draw_buffers_blend,						//366	//TODO:
	GGL_APPLE_texture_range,						//367	//TODO:
	GGL_APPLE_float_pixels,							//368	//TODO:
	GGL_APPLE_vertex_program_evaluators,			//369	//TODO:
	GGL_APPLE_aux_depth_stencil,					//370	//TODO:
	GGL_APPLE_object_purgeable,						//371	//TODO:
	GGL_APPLE_row_bytes,							//372	//TODO:
	GGL_APPLE_rgb_422,								//373	//TODO:
	GGL_NV_video_capture,							//374	//TODO:
	GGL_EXT_swap_control,							//375	//TODO:
	GGL_NV_copy_image,								//376	//TODO:
	GGL_EXT_separate_shader_objects,				//377	//TODO:
	GGL_NV_parameter_buffer_object2,				//378	//TODO:
	GGL_NV_shader_buffer_load,						//379	//TODO:
	GGL_NV_vertex_buffer_unified_memory,			//380	//TODO:
	GGL_NV_texture_barrier,							//381	//TODO:
	GGL_AMD_shader_stencil_export,					//382	//TODO:
	GGL_AMD_seamless_cubemap_per_texture,			//383	//TODO:
	GGL_INTEL_swap_event,							//384	//TODO:
	GGL_AMD_conservative_depth,						//385	//TODO:
	GGL_EXT_shader_image_load_store,				//386	//TODO:
	GGL_EXT_vertex_attrib_64bit,					//387	//TODO:
	GGL_NV_gpu_program5,							//388	//TODO:
	GGL_NV_gpu_shader5,								//389	//TODO:
	GGL_NV_shader_buffer_store,						//390	//TODO:
	GGL_NV_tesselation_program5,					//391	//TODO:
	GGL_NV_vertex_attrib_integer_64bit,				//392	//TODO:
	GGL_NV_multisample_coverage,					//393	//TODO:
	GGL_AMD_name_gen_delete,						//394	//TODO:
	GGL_AMD_debug_output,							//395	//TODO:
	GGL_NV_vdpau_interop,							//396	//TODO:
	GGL_AMD_transform_feedback3_lines_triangles,	//397	//TODO:
	GGL_AMD_depth_clamp_separate,					//401	//TODO:
	GGL_EXT_texture_sRGB_decode,					//402	//TODO:
	GGL_NV_texture_multisample,						//403	//TODO:
	GGL_AMD_blend_minmax_factor,					//404	//TODO:
	GGL_AMD_sample_positions,						//405	//TODO:
	GGL_EXT_x11_sync_object,						//406	//TODO:
	GGL_AMD_multi_draw_indirect,					//408	//TODO:
	GGL_EXT_framebuffer_multisample_bit_scaled,		//409	//TODO:
	GGL_NV_path_rendering,							//410	//TODO:
	GGL_AMD_pinned_memory,							//411	//TODO:
	GGL_AMD_stencil_operation_extended,				//413	//TODO:
	GGL_AMD_vertex_shader_viewport_index,			//416	//TODO:
	GGL_AMD_vertex_shader_layer,					//417	//TODO:
	GGL_NV_bindless_texture,						//418	//TODO:
	GGL_NV_shader_atomic_float,						//419	//TODO:
	GGL_AMD_query_buffer_object,					//420	//TODO:
	GGL_NV_compute_program5,						//421	//TODO:
	GGL_NV_shader_storage_buffer_object,			//422	//TODO:
	GGL_NV_shader_atomic_counters,					//423	//TODO:
	GGL_NV_deep_texture3D,							//424	//TODO:
	GGL_NVX_conditional_render,						//425	//TODO:
	GGL_AMD_sparse_texture,							//426	//TODO:
	GGL_AMD_shader_trinary_minmax,					//428	//TODO:
	GGL_INTEL_map_texture,							//429	//TODO:
	GGL_NV_draw_texture,							//430	//TODO:
	GGL_AMD_interleaved_elements,					//431	//TODO:
	GGL_NV_bindless_multi_draw_indirect,			//432	//TODO:
	GGL_NV_blender_equation_advanced,				//433	//TODO:
	GGL_NV_blender_equation_advanced_coherent,		//433	//TODO:
	GGL_NV_gpu_program5_mem_extended,				//434	//TODO:
	GGL_AMD_shader_atomic_counter_ops,				//435	//TODO:
	GGL_EXT_shader_integer_mix,						//437	//TODO:
	GGL_NVX_gpu_memory_info,						//438	//TODO:
	GGL_EXT_debug_label,							//439	//TODO:
	GGL_EXT_debug_marker,							//440	//TODO:
	GGL_INTEL_fragment_shader_ordering,				//441	//TODO:
	GGL_AMD_occlusion_query_event,					//442	//TODO:
	GGL_INTEL_performance_query,					//443	//TODO:
	GGL_AMD_shader_stencil_value_export,			//444	//TODO:
	GGL_NV_shader_thread_group,						//447	//TODO:
	GGL_NV_shader_thread_shuffle,					//448	//TODO:
	GGL_EXT_shader_image_load_formatted,			//449	//TODO:
	GGL_AMD_transform_feedback4,					//450	//TODO:
	GGL_AMD_gpu_shader_int64,						//451	//TODO:
	GGL_EXT_glx_stereo_tree,						//452	//TODO:
	GGL_AMD_gcn_shader,								//453	//TODO:
	GGL_NV_shader_atomic_int64,						//455	//TODO:
	GGL_NV_bindless_multi_draw_indirect_count,		//456	//TODO:
	GGL_KHR_blend_equation_advanced,				//458	//TODO:
	GGL_KHR_blend_equation_advanced_coherent,		//458	//TODO:
	GGL_NV_uniform_buffer_unified_memory,			//459	//TODO:
	GGL_EXT_polygon_offset_clamp,					//460	//TODO:
	GGL_EXT_post_depth_coverage,					//461	//TODO:
	GGL_EXT_raster_multisample,						//462	//TODO:
	GGL_EXT_sparse_texture2,						//463	//TODO:
	GGL_EXT_texture_filter_minmax,					//464	//TODO:
	GGL_NV_conservative_raster,						//465	//TODO:
	GGL_NV_fill_rectangle,							//466	//TODO:
	GGL_NV_fragment_coverage_to_color,				//467	//TODO:
	GGL_NV_fragment_shader_interlock,				//468	//TODO:
	GGL_NV_framebuffer_mixed_samples,				//469	//TODO:
	GGL_NV_geometry_shader_passthrough,				//470	//TODO:
	GGL_NV_path_rendering_shadred_edge,				//471	//TODO:
	GGL_NV_sample_locations,						//472	//TODO:
	GGL_NV_sample_mask_override_coverage,			//473	//TODO:
	GGL_NV_shader_atomic_fp16_vector,				//474	//TODO:
	GGL_NV_internalformat_sample_query,				//475	//TODO:
	GGL_NV_viewport_array2,							//476	//TODO:
	GGL_KTX_buffer_region,
	GGL_SUN_read_video_pixels,
	GGL_EXT_Cg_shader,
	GGL_ATIX_point_sprites,
	GGL_ATIX_texture_env_route,
	GGL_ATIX_vertex_shader_output_point_size,
	GGL_ATI_texture_compression_3dc,
	GGL_EXT_texture_rectangle,
	GGL_APPLE_pixel_buffer,
	GGL_WIN_swap_hint,
	GGL_OES_blend_equation_separate,				//TODO: XXX http://cgit.freedesktop.org/mesa/mesa/tree/include/GLES/glext.h?h=gallium-0.1
	GGL_OES_blend_func_separate,					//TODO: XXX
	GGL_OES_blend_subtract,							//TODO: XXX
	GGL_OES_compressed_ETC1_RGB8_texture,			//TODO: XXX
	GGL_OES_draw_texture,							//TODO: XXX
	GGL_OES_framebuffer_object,						//TODO: XXX
	GGL_OES_matrix_get,								//TODO: XXX
	GGL_OES_matrix_palette,							//TODO: XXX
	GGL_OES_extended_matrix_palette,				//TODO: XXX
	GGL_OES_stencil_wrap,							//TODO: XXX
	GGL_OES_texture_mirrored_repeat,				//TODO: XXX
	GGL_OES_texture_cube_map,						//TODO: XXX
	GGL_OES_texture_env_crossbar,					//TODO: XXX
	GGL_OES_EGL_image,								//TODO: XXX
	GGL_OES_depth24,								//TODO: XXX
	GGL_OES_depth32,								//TODO: XXX
	GGL_OES_mapbuffer,								//TODO: XXX
	GGL_OES_rgb8_rgba8,								//TODO: XXX
	GGL_OES_stencil1,								//TODO: XXX
	GGL_OES_stencil4,								//TODO: XXX
	GGL_OES_stencil8,								//TODO: XXX
	GGL_AMD_compressed_3DC_texture,					//TODO: XXX
	GGL_AMD_compressed_ATC_texture,					//TODO: XXX
	GGL_OES_element_index_uint,						//TODO: XXX
	GGL_OES_fbo_render_mipmap,						//TODO: XXX
	GGL_3DL_direct_texture_access2,					//TODO: ???
	GGL_3Dlabs_multisample_transparency_id,			//TODO: ???
	GGL_3Dlabs_multisample_transparency_range,		//TODO: ???
	GGL_AMDX_vertex_shader_tessellator,				//TODO: ???
	GGL_APPLE_packed_pixel,							//TODO: ??? GL_APPLE_packed_pixels???
//3DFX_set_global_palette ///XXX: ?????????
	GGL_ATI_lock_texture,							//TODO: ???
	GGL_ATI_pixel_format_float,						//TODO: ??? WGL_ATI_pixel_format_float???
	GGL_ATI_shader_texture_lod,						//TODO: ???
	GGL_ATI_vertex_blend,							//TODO: ???
	GGL_ATI_vertex_shader,							//TODO: ???
	GGL_ATIX_pn_triangles,							//TODO: ???
	GGL_ATIX_texture_env_combine3,					//TODO: ???
	GGL_Autodesk_facet_normal,						//TODO: ???
	GGL_Autodesk_valid_back_buffer_hint,			//TODO: ???
	GGL_DIMD_YUV,									//TODO: ???
	GGL_EXT_color_matrix,							//TODO: ???
	GGL_EXT_color_table,							//TODO: ???
	GGL_EXT_convolution_border_modes,				//TODO: ???
	GGL_EXT_depth_buffer_float,						//TODO: ???
	GGL_EXT_fog_function,							//TODO: ???
	GGL_EXT_fog_offset,								//TODO: ???
	GGL_EXT_generate_mipmap,						//TODO: ???
	GGL_EXT_interlace,								//TODO: ???
	GGL_EXT_packed_pixels_12,						//TODO: ???
	GGL_EXT_pixel_format,							//TODO: ???
	GGL_EXT_pixel_texture,							//TODO: ???
	GGL_EXT_texgen_reflection,						//TODO: ???
	GGL_EXT_texture_border_clamp,					//TODO: ???
	GGL_EXT_texture_color_table,					//TODO: ???
	GGL_EXT_texture_lod,							//TODO: ???
	GGL_EXT_texture4D,								//TODO: ???
	GGL_EXTX_framebuffer_mixed_formats,				//TODO: ???
	GGL_EXTX_packed_depth_stencil,					//TODO: ???
	GGL_FGL_lock_texture,							//TODO: ???
	GGL_GL2_geometry_shader,						//TODO: ???
	GGL_I3D_argb,									//TODO: ???
	GGL_I3D_color_clamp,							//TODO: ???
	GGL_I3D_interlace_read,							//TODO: ???
	GGL_IBM_clip_check,								//TODO: ???
	GGL_IBM_load_named_matrix,						//TODO: ???
	GGL_IBM_multi_draw_arrays,						//TODO: ??? GL_IBM_multimode_draw_arrays???
	GGL_IBM_occlusion_cull,							//TODO: ???
	GGL_IBM_pixel_filter_hint,						//TODO: ???
	GGL_IBM_rescale_normal,							//TODO: ???
	GGL_IBM_texture_clamp_nodraw,					//TODO: ???
	GGL_IBM_YCbCr,									//TODO: ???
	GGL_IMG_read_format,							//TODO: ???
	GGL_IMG_texture_compression_pvrtc,				//TODO: ???
	GGL_IMG_texture_env_enhanced_fixed_function,	//TODO: ???
	GGL_IMG_texture_format_BGRA8888,				//TODO: ???
	GGL_IMG_user_clip_planes,						//TODO: ???
	GGL_IMG_vertex_program, 						//TODO: ???
	GGL_INGR_blend_func_separate,					//TODO: ???
	GGL_INGR_multiple_palette,						//TODO: ???
	GGL_MESA_program_debug,							//TODO: ???
	GGL_MTX_fragment_shader,						//TODO: ???
	GGL_MTX_precision_dpi,							//TODO: ???
	GGL_NV_centroid_sample,							//TODO: ???
	GGL_NV_framebuffer_multisample_ex,				//TODO: ???
	GGL_NV_pixel_buffer_object,						//TODO: ???
	GGL_NV_texture_compression_latc,				//TODO: ???
	GGL_NV_timer_query,								//TODO: ???
	GGL_NVX_flush_hold,								//TODO: ???
	GGL_NVX_instanced_arrays,						//TODO: ???
	GGL_NVX_ycrcb,									//TODO: ???
	GGL_OES_conditional_query,						//TODO: ???
	GGL_OES_point_size_array,						//TODO: ???
	GGL_SGI_compiled_vertex_array,					//TODO: ???
	GGL_SGI_cull_vertex,							//TODO: ???
	GGL_SGI_index_array_formats,					//TODO: ???
	GGL_SGI_index_func, 							//TODO: ???
	GGL_SGI_index_material,							//TODO: ???
	GGL_SGI_index_texture,							//TODO: ???
	GGL_SGI_make_current_read,						//TODO: ???
	GGL_SGI_texture_add_env,						//TODO: ???
	GGL_SGI_texture_edge_clamp,						//TODO: ???
	GGL_SGI_texture_lod,							//TODO: ???
	GGL_SGIS_multitexture,							//TODO: ???
	GGL_EXT_static_vertex_array,					//TODO: ???
	GGL_EXT_vertex_array_set,						//TODO:	???
	GGL_EXT_vertex_array_setXXX,					//TODO: ???
	GGL_SGIX_fog_texture,							//TODO: ???
	GGL_SGIX_fragment_specular_lighting,			//TODO: ???
	GGL_SGIX_pbuffer,								//TODO: ???
	GGL_SUN_multidraw_arrays,						//TODO: ???
	GGL_WGL_ARB_extensions_string,					//TODO: ??? WGL_ARB_extensions_string???
	GGL_WGL_EXT_extensions_string,					//TODO: ??? WGL_EXT_extensions_string???
	GGL_WGL_EXT_swap_control,						//TODO: ??? WGL_EXT_swap_control???

	GGL_APPLE_packed_pixels,						//TODO: ???
	GGL_APPLE_flush_render,							//TODO: ???
	GGL_ATI_point_cull_mode,						//TODO: ???
	GGL_ATI_blend_equation_separate,				//TODO: ???
	GGL_ATI_blend_weighted_minmax,					//TODO: ???
	GGL_ATI_array_rev_comps_in_4_bytes,				//TODO: ???
	GGL_SUN_blend_src_mult_dst_alpha,				//TODO: ???
	GGL_SUN_read_write_samples,						//TODO: ???
	GGL_SUN_targeted_texture,						//TODO: ???
	GGL_MAX_EXTENSIONS
} gl_extensions_t;

#define GLENUMERATION_FALSE		0
#define GLENUMERATION_TRUE		1

#define GLDRIVER_NONE			0
#define GLDRIVER_ANY			1
#define GLDRIVER_DEFAULT		2
#define GLDRIVER_3DFX			3

//driver information
extern char *vendor, *renderer, *version, *extensions;
extern int glversion[2];	//[0] = minor version; [1] = major version

//loads the proper OpenGL driver and dynloads it
const int OpenGL_Init(const int gldriver, void (*errHandler)(const char *error, ...));
void OpenGL_Shutdown(void);

//extensions that aren't supported are set to NULL
void OpenGL_EnableExtensions(HDC hDC, int (*OpenGL_ExtensionsCallback)(const char *name, gl_extensions_t extension, void *reserved), void *element);
void OpenGL_DisableExtensions(void);

#endif
