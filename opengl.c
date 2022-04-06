#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "opengl.h"

//TODO: GL_EXT_timer_query  #define GL_TIME_ELAPSED_EXT 0x88BF
//TODO: go through all the extensions and make sure they are not incomplete
//TODO: runs these by gl extension viewer
//this module is entirely independent of any other module in the engine

static void (*OpenGL_Error)(const char *error, ...);
char *StringToken(const char *haystack, const char *needle);
void OpenGL_GetVersion(void);

BOOL (WINAPI *gwglCopyContext)(HGLRC, HGLRC, UINT);
HGLRC (WINAPI *gwglCreateContext)(HDC);
HGLRC (WINAPI *gwglCreateLayerContext)(HDC, int);
BOOL (WINAPI *gwglDeleteContext)(HGLRC);
HGLRC (WINAPI *gwglGetCurrentContext)(VOID);
HDC (WINAPI *gwglGetCurrentDC)(VOID);
FARPROC (WINAPI *gwglGetProcAddress)(LPCSTR);
BOOL (WINAPI *gwglMakeCurrent)(HDC, HGLRC);
BOOL (WINAPI *gwglShareLists)(HGLRC, HGLRC);
BOOL (WINAPI *gwglUseFontBitmaps)(HDC, DWORD, DWORD, DWORD);
BOOL (WINAPI *gwglUseFontOutlines)(HDC, DWORD, DWORD, DWORD, FLOAT, FLOAT, int, LPGLYPHMETRICSFLOAT);
BOOL (WINAPI *gwglDescribeLayerPlane)(HDC, int, int, UINT, LPLAYERPLANEDESCRIPTOR);
int (WINAPI *gwglSetLayerPaletteEntries)(HDC, int, int, int, COLORREF *);
int (WINAPI *gwglGetLayerPaletteEntries)(HDC, int, int, int, COLORREF *);
BOOL (WINAPI *gwglRealizeLayerPalette)(HDC, int, BOOL);
BOOL (WINAPI *gwglSwapLayerBuffers)(HDC, UINT);

//function pointers used for MCDs
int (WINAPI *gwglChoosePixelFormat)(HDC, CONST PIXELFORMATDESCRIPTOR *);
int (WINAPI *gwglDescribePixelFormat) (HDC, int, UINT, LPPIXELFORMATDESCRIPTOR);
int (WINAPI *gwglGetPixelFormat)(HDC);
BOOL (WINAPI *gwglSetPixelFormat)(HDC, int, CONST PIXELFORMATDESCRIPTOR *);
BOOL (WINAPI *gwglSwapBuffers)(HDC);

//=====================================
//
// WGL EXTENSIONS
//
//=====================================

//WGL_ARB_buffer_region
HANDLE (WINAPI *gwglCreateBufferRegionARB)(HDC hDC, int iLayerPlane, UINT uType);
VOID (WINAPI *gwglDeleteBufferRegionARB)(HANDLE hRegion);
BOOL (WINAPI *gwglSaveBufferRegionARB)(HANDLE hRegion, int x, int y, int width, int height);
BOOL (WINAPI *gwglRestoreBufferRegionARB)(HANDLE hRegion, int x, int y, int width, int height, int xSrc, int ySrc);

//WGL_ARB_extensions_string
const char *(WINAPI *gwglGetExtensionsStringARB)(HDC hdc);

//WGL_ARB_pixel_format
BOOL (WINAPI *gwglGetPixelFormatAttribivARB)(HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int *piAttributes, int *piValues);
BOOL (WINAPI *gwglGetPixelFormatAttribfvARB)(HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int *piAttributes, FLOAT *pfValues);
BOOL (WINAPI *gwglChoosePixelFormatARB)(HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);

//WGL_ARB_make_current_read
BOOL (WINAPI *gwglMakeContextCurrentARB)(HDC hDrawDC, HDC hReadDC, HGLRC hglrc);
HDC (WINAPI *gwglGetCurrentReadDCARB)(void);

//WGL_ARB_pbuffer
HPBUFFERARB (WINAPI *gwglCreatePbufferARB)(HDC hDC, int iPixelFormat, int iWidth, int iHeight, const int *piAttribList);
HDC (WINAPI *gwglGetPbufferDCARB)(HPBUFFERARB hPbuffer);
int (WINAPI *gwglReleasePbufferDCARB)(HPBUFFERARB hPbuffer, HDC hDC);
BOOL (WINAPI *gwglDestroyPbufferARB)(HPBUFFERARB hPbuffer);
BOOL (WINAPI *gwglQueryPbufferARB)(HPBUFFERARB hPbuffer, int iAttribute, int *piValue);

//WGL_ARB_render_texture
BOOL (WINAPI *gwglBindTexImageARB)(HPBUFFERARB hPbuffer, int iBuffer);
BOOL (WINAPI *gwglReleaseTexImageARB)(HPBUFFERARB hPbuffer, int iBuffer);
BOOL (WINAPI *gwglSetPbufferAttribARB)(HPBUFFERARB hPbuffer, const int *piAttribList);

//WGL_ARB_create_context
HGLRC (WINAPI *gwglCreateContextAttribsARB)(HDC hDC, HGLRC hShareContext, const int *attribList);

//WGL_EXT_display_color_table
GLboolean (WINAPI *gwglCreateDisplayColorTableEXT)(GLushort id);
GLboolean (WINAPI *gwglLoadDisplayColorTableEXT)(const GLushort *table, GLuint length);
GLboolean (WINAPI *gwglBindDisplayColorTableEXT)(GLushort id);
VOID (WINAPI *gwglDestroyDisplayColorTableEXT)(GLushort id);

//WGL_EXT_extensions_string
const char *(WINAPI *gwglGetExtensionsStringEXT)(void);

//WGL_EXT_make_current_read
BOOL (WINAPI *gwglMakeContextCurrentEXT)(HDC hDrawDC, HDC hReadDC, HGLRC hglrc);
HDC (WINAPI *gwglGetCurrentReadDCEXT)(void);

//WGL_EXT_pixel_format
BOOL (WINAPI *gwglGetPixelFormatAttribivEXT)(HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, int *piAttributes, int *piValues);
BOOL (WINAPI *gwglGetPixelFormatAttribfvEXT)(HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, int *piAttributes, FLOAT *pfValues);
BOOL (WINAPI *gwglChoosePixelFormatEXT)(HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);

//WGL_EXT_pbuffer
HPBUFFEREXT(WINAPI *gwglCreatePbufferEXT)(HDC hDC, int iPixelFormat, int iWidth, int iHeight, const int *piAttribList);
HDC(WINAPI *gwglGetPbufferDCEXT)(HPBUFFEREXT hPbuffer);
int(WINAPI *gwglReleasePbufferDCEXT)(HPBUFFEREXT hPbuffer, HDC hDC);
BOOL(WINAPI *gwglDestroyPbufferEXT)(HPBUFFEREXT hPbuffer);
BOOL(WINAPI *gwglQueryPbufferEXT)(HPBUFFEREXT hPbuffer, int iAttribute, int *piValue);

//WGL_EXT_swap_control
BOOL (WINAPI *gwglSwapIntervalEXT)(int interval);
int (WINAPI *gwglGetSwapIntervalEXT)(void);

//WGL_NV_vertex_array_range
void *(WINAPI *gwglAllocateMemoryNV)(GLsizei size, GLfloat readfreq, GLfloat writefreq, GLfloat priority);
void (WINAPI *gwglFreeMemoryNV)(void *pointer);

//WGL_EXT_multisample
void (WINAPI *gwglSampleMaskEXT)(GLclampf value, GLboolean invert);
void (WINAPI *gwglSamplePatternEXT)(GLenum pattern);

//WGL_OML_sync_control
BOOL (WINAPI *gwglGetSyncValuesOML)(HDC hdc, INT64 *ust, INT64 *msc, INT64 *sbc);
BOOL (WINAPI *gwglGetMscRateOML)(HDC hdc, INT32 *numerator, INT32 *denominator);
INT64 (WINAPI *gwglSwapBuffersMscOML)(HDC hdc, INT64 target_msc, INT64 divisor, INT64 remainder);
INT64 (WINAPI *gwglSwapLayerBuffersMscOML)(HDC hdc, int fuPlanes, INT64 target_msc, INT64 divisor, INT64 remainder);
BOOL (WINAPI *gwglWaitForMscOML)(HDC hdc, INT64 target_msc, INT64 divisor, INT64 remainder, INT64 *ust, INT64 *msc, INT64 *sbc);
BOOL (WINAPI *gwglWaitForSbcOML)(HDC hdc, INT64 target_sbc, INT64 *ust, INT64 *msc, INT64 *sbc);

//WGL_I3D_digital_video_control
BOOL (WINAPI *gwglGetDigitalVideoParametersI3D)(HDC hDC, int iAttribute, int *piValue);
BOOL (WINAPI *gwglSetDigitalVideoParametersI3D)(HDC hDC, int iAttribute, const int *piValue);

//WGL_I3D_gamma
BOOL (WINAPI *gwglGetGammaTableParametersI3D)(HDC hDC, int iAttribute, int *piValue);
BOOL (WINAPI *gwglSetGammaTableParametersI3D)(HDC hDC, int iAttribute, const int *piValue);
BOOL (WINAPI *gwglGetGammaTableI3D)(HDC hDC, int iEntries, USHORT *puRed, USHORT *puGreen, USHORT *puBlue);
BOOL (WINAPI *gwglSetGammaTableI3D)(HDC hDC, int iEntries, const USHORT *puRed, const USHORT *puGreen, const USHORT *puBlue);

//WGL_I3D_genlock
BOOL (WINAPI *gwglEnableGenlockI3D)(HDC hDC);
BOOL (WINAPI *gwglDisableGenlockI3D)(HDC hDC);
BOOL (WINAPI *gwglIsEnabledGenlockI3D)(HDC hDC, BOOL *pFlag);
BOOL (WINAPI *gwglGenlockSourceI3D)(HDC hDC, UINT uSource);
BOOL (WINAPI *gwglGetGenlockSourceI3D)(HDC hDC, UINT *uSource);
BOOL (WINAPI *gwglGenlockSourceEdgeI3D)(HDC hDC, UINT uEdge);
BOOL (WINAPI *gwglGetGenlockSourceEdgeI3D)(HDC hDC, UINT *uEdge);
BOOL (WINAPI *gwglGenlockSampleRateI3D)(HDC hDC, UINT uRate);
BOOL (WINAPI *gwglGetGenlockSampleRateI3D)(HDC hDC, UINT *uRate);
BOOL (WINAPI *gwglGenlockSourceDelayI3D)(HDC hDC, UINT uDelay);
BOOL (WINAPI *gwglGetGenlockSourceDelayI3D)(HDC hDC, UINT *uDelay);
BOOL (WINAPI *gwglQueryGenlockMaxSourceDelayI3D)(HDC hDC, UINT *uMaxLineDelay, UINT *uMaxPixelDelay);

//WGL_I3D_image_buffer
LPVOID (WINAPI *gwglCreateImageBufferI3D)(HDC hDC, DWORD dwSize, UINT uFlags);
BOOL (WINAPI *gwglDestroyImageBufferI3D)(HDC hDC, LPVOID pAddress);
BOOL (WINAPI *gwglAssociateImageBufferEventsI3D)(HDC hDC, const HANDLE *pEvent, const LPVOID *pAddress, const DWORD *pSize, UINT count);
BOOL (WINAPI *gwglReleaseImageBufferEventsI3D)(HDC hDC, const LPVOID *pAddress, UINT count);

//WGL_I3D_swap_frame_lock
BOOL (WINAPI *gwglEnableFrameLockI3D)(void);
BOOL (WINAPI *gwglDisableFrameLockI3D)(void);
BOOL (WINAPI *gwglIsEnabledFrameLockI3D)(BOOL *pFlag);
BOOL (WINAPI *gwglQueryFrameLockMasterI3D)(BOOL *pFlag);

//WGL_I3D_swap_frame_usage
BOOL (WINAPI *gwglGetFrameUsageI3D)(float *pUsage);
BOOL (WINAPI *gwglBeginFrameTrackingI3D)(void);
BOOL (WINAPI *gwglEndFrameTrackingI3D)(void);
BOOL (WINAPI *gwglQueryFrameTrackingI3D)(DWORD *pFrameCount, DWORD *pMissedFrames, float *pLastMissedUsage);

//WGL_3DL_stereo_control
BOOL (WINAPI *gwglSetStereoEmitterState3DL)(HDC hDC, UINT uState);

//WGL_NV_present_video
int (WINAPI *gwglEnumerateVideoDevicesNV)(HDC hDC, HVIDEOOUTPUTDEVICENV *phDeviceList);
BOOL (WINAPI *gwglBindVideoDeviceNV)(HDC hDC, unsigned int uVideoSlot, HVIDEOOUTPUTDEVICENV hVideoDevice, const int *piAttribList);
BOOL (WINAPI *gwglQueryCurrentContextNV)(int iAttribute, int *piValue);

//WGL_NV_video_out
BOOL (WINAPI *gwglGetVideoDeviceNV)(HDC hDC, int numDevices, HPVIDEODEV *hVideoDevice);
BOOL (WINAPI *gwglReleaseVideoDeviceNV)(HPVIDEODEV hVideoDevice);
BOOL (WINAPI *gwglBindVideoImageNV)(HPVIDEODEV hVideoDevice, HPBUFFERARB hPbuffer, int iVideoBuffer);
BOOL (WINAPI *gwglReleaseVideoImageNV)(HPBUFFERARB hPbuffer, int iVideoBuffer);
BOOL (WINAPI *gwglSendPbufferToVideoNV)(HPBUFFERARB hPbuffer, int iBufferType, unsigned long *pulCounterPbuffer, BOOL bBlock);
BOOL (WINAPI *gwglGetVideoInfoNV)(HPVIDEODEV hpVideoDevice, unsigned long *pulCounterOutputPbuffer, unsigned long *pulCounterOutputVideo);

//WGL_NV_swap_group
BOOL (WINAPI *gwglJoinSwapGroupNV)(HDC hDC, GLuint group);
BOOL (WINAPI *gwglBindSwapBarrierNV)(GLuint group, GLuint barrier);
BOOL (WINAPI *gwglQuerySwapGroupNV)(HDC hDC, GLuint *group, GLuint *barrier);
BOOL (WINAPI *gwglQueryMaxSwapGroupsNV)(HDC hDC, GLuint *maxGroups, GLuint *maxBarriers);
BOOL (WINAPI *gwglQueryFrameCountNV)(HDC hDC, GLuint *count);
BOOL (WINAPI *gwglResetFrameCountNV)(HDC hDC);

//WGL_NV_gpu_affinity
BOOL (WINAPI *gwglEnumGpusNV)(UINT iGpuIndex, HGPUNV *phGpu);
BOOL (WINAPI *gwglEnumGpuDevicesNV)(HGPUNV hGpu, UINT iDeviceIndex, PGPU_DEVICE lpGpuDevice);
HDC (WINAPI *gwglCreateAffinityDCNV)(const HGPUNV *phGpuList);
BOOL (WINAPI *gwglEnumGpusFromAffinityDCNV)(HDC hAffinityDC, UINT iGpuIndex, HGPUNV *hGpu);
BOOL (WINAPI *gwglDeleteDCNV)(HDC hdc);

//=====================================
//
// OPENGL EXTENSIONS
//
//=====================================

//GL_ARB_multitexture
void (APIENTRY *gglActiveTextureARB)(GLenum texture);
void (APIENTRY *gglClientActiveTextureARB)(GLenum texture);
void (APIENTRY *gglMultiTexCoord1dARB)(GLenum target, GLdouble s);
void (APIENTRY *gglMultiTexCoord1dvARB)(GLenum target, const GLdouble *v);
void (APIENTRY *gglMultiTexCoord1fARB)(GLenum target, GLfloat s);
void (APIENTRY *gglMultiTexCoord1fvARB)(GLenum target, const GLfloat *v);
void (APIENTRY *gglMultiTexCoord1iARB)(GLenum target, GLint s);
void (APIENTRY *gglMultiTexCoord1ivARB)(GLenum target, const GLint *v);
void (APIENTRY *gglMultiTexCoord1sARB)(GLenum target, GLshort s);
void (APIENTRY *gglMultiTexCoord1svARB)(GLenum target, const GLshort *v);
void (APIENTRY *gglMultiTexCoord2dARB)(GLenum target, GLdouble s, GLdouble t);
void (APIENTRY *gglMultiTexCoord2dvARB)(GLenum target, const GLdouble *v);
void (APIENTRY *gglMultiTexCoord2fARB)(GLenum target, GLfloat s, GLfloat t);
void (APIENTRY *gglMultiTexCoord2fvARB)(GLenum target, const GLfloat *v);
void (APIENTRY *gglMultiTexCoord2iARB)(GLenum target, GLint s, GLint t);
void (APIENTRY *gglMultiTexCoord2ivARB)(GLenum target, const GLint *v);
void (APIENTRY *gglMultiTexCoord2sARB)(GLenum target, GLshort s, GLshort t);
void (APIENTRY *gglMultiTexCoord2svARB)(GLenum target, const GLshort *v);
void (APIENTRY *gglMultiTexCoord3dARB)(GLenum target, GLdouble s, GLdouble t, GLdouble r);
void (APIENTRY *gglMultiTexCoord3dvARB)(GLenum target, const GLdouble *v);
void (APIENTRY *gglMultiTexCoord3fARB)(GLenum target, GLfloat s, GLfloat t, GLfloat r);
void (APIENTRY *gglMultiTexCoord3fvARB)(GLenum target, const GLfloat *v);
void (APIENTRY *gglMultiTexCoord3iARB)(GLenum target, GLint s, GLint t, GLint r);
void (APIENTRY *gglMultiTexCoord3ivARB)(GLenum target, const GLint *v);
void (APIENTRY *gglMultiTexCoord3sARB)(GLenum target, GLshort s, GLshort t, GLshort r);
void (APIENTRY *gglMultiTexCoord3svARB)(GLenum target, const GLshort *v);
void (APIENTRY *gglMultiTexCoord4dARB)(GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q);
void (APIENTRY *gglMultiTexCoord4dvARB)(GLenum target, const GLdouble *v);
void (APIENTRY *gglMultiTexCoord4fARB)(GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
void (APIENTRY *gglMultiTexCoord4fvARB)(GLenum target, const GLfloat *v);
void (APIENTRY *gglMultiTexCoord4iARB)(GLenum target, GLint s, GLint t, GLint r, GLint q);
void (APIENTRY *gglMultiTexCoord4ivARB)(GLenum target, const GLint *v);
void (APIENTRY *gglMultiTexCoord4sARB)(GLenum target, GLshort s, GLshort t, GLshort r, GLshort q);
void (APIENTRY *gglMultiTexCoord4svARB)(GLenum target, const GLshort *v);

//GL_ARB_transpose_matrix
void (APIENTRY *gglLoadTransposeMatrixfARB)(const GLfloat *m);
void (APIENTRY *gglLoadTransposeMatrixdARB)(const GLdouble *m);
void (APIENTRY *gglMultTransposeMatrixfARB)(const GLfloat *m);
void (APIENTRY *gglMultTransposeMatrixdARB)(const GLdouble *m);

//GL_ARB_multisample
void (APIENTRY *gglSampleCoverageARB)(GLclampf value, GLboolean invert);

//GL_ARB_texture_compression
void (APIENTRY *gglCompressedTexImage3DARB)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid *data);
void (APIENTRY *gglCompressedTexImage2DARB)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data);
void (APIENTRY *gglCompressedTexImage1DARB)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid *data);
void (APIENTRY *gglCompressedTexSubImage3DARB)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid *data);
void (APIENTRY *gglCompressedTexSubImage2DARB)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data);
void (APIENTRY *gglCompressedTexSubImage1DARB)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid *data);
void (APIENTRY *gglGetCompressedTexImageARB)(GLenum target, GLint level, void *img);

//GL_ARB_point_parameters
void (APIENTRY *gglPointParameterfARB)(GLenum pname, GLfloat param);
void (APIENTRY *gglPointParameterfvARB)(GLenum pname, const GLfloat *params);

//GL_ARB_vertex_blend
void (APIENTRY *gglWeightbvARB)(GLint size, const GLbyte *weights);
void (APIENTRY *gglWeightsvARB)(GLint size, const GLshort *weights);
void (APIENTRY *gglWeightivARB)(GLint size, const GLint *weights);
void (APIENTRY *gglWeightfvARB)(GLint size, const GLfloat *weights);
void (APIENTRY *gglWeightdvARB)(GLint size, const GLdouble *weights);
void (APIENTRY *gglWeightubvARB)(GLint size, const GLubyte *weights);
void (APIENTRY *gglWeightusvARB)(GLint size, const GLushort *weights);
void (APIENTRY *gglWeightuivARB)(GLint size, const GLuint *weights);
void (APIENTRY *gglWeightPointerARB)(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
void (APIENTRY *gglVertexBlendARB)(GLint count);

//GL_ARB_matrix_palette
void (APIENTRY *gglCurrentPaletteMatrixARB)(GLint index);
void (APIENTRY *gglMatrixIndexubvARB)(GLint size, const GLubyte *indices);
void (APIENTRY *gglMatrixIndexusvARB)(GLint size, const GLushort *indices);
void (APIENTRY *gglMatrixIndexuivARB)(GLint size, const GLuint *indices);
void (APIENTRY *gglMatrixIndexPointerARB)(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);

//GL_ARB_window_pos
void (APIENTRY *gglWindowPos2dARB)(GLdouble x, GLdouble y);
void (APIENTRY *gglWindowPos2dvARB)(const GLdouble *v);
void (APIENTRY *gglWindowPos2fARB)(GLfloat x, GLfloat y);
void (APIENTRY *gglWindowPos2fvARB)(const GLfloat *v);
void (APIENTRY *gglWindowPos2iARB)(GLint x, GLint y);
void (APIENTRY *gglWindowPos2ivARB)(const GLint *v);
void (APIENTRY *gglWindowPos2sARB)(GLshort x, GLshort y);
void (APIENTRY *gglWindowPos2svARB)(const GLshort *v);
void (APIENTRY *gglWindowPos3dARB)(GLdouble x, GLdouble y, GLdouble z);
void (APIENTRY *gglWindowPos3dvARB)(const GLdouble *v);
void (APIENTRY *gglWindowPos3fARB)(GLfloat x, GLfloat y, GLfloat z);
void (APIENTRY *gglWindowPos3fvARB)(const GLfloat *v);
void (APIENTRY *gglWindowPos3iARB)(GLint x, GLint y, GLint z);
void (APIENTRY *gglWindowPos3ivARB)(const GLint *v);
void (APIENTRY *gglWindowPos3sARB)(GLshort x, GLshort y, GLshort z);
void (APIENTRY *gglWindowPos3svARB)(const GLshort *v);

//GL_ARB_vertex_program
void (APIENTRY *gglVertexAttrib1sARB)(GLuint index, GLshort x);
void (APIENTRY *gglVertexAttrib1fARB)(GLuint index, GLfloat x);
void (APIENTRY *gglVertexAttrib1dARB)(GLuint index, GLdouble x);
void (APIENTRY *gglVertexAttrib2sARB)(GLuint index, GLshort x, GLshort y);
void (APIENTRY *gglVertexAttrib2fARB)(GLuint index, GLfloat x, GLfloat y);
void (APIENTRY *gglVertexAttrib2dARB)(GLuint index, GLdouble x, GLdouble y);
void (APIENTRY *gglVertexAttrib3sARB)(GLuint index, GLshort x, GLshort y, GLshort z);
void (APIENTRY *gglVertexAttrib3fARB)(GLuint index, GLfloat x, GLfloat y, GLfloat z);
void (APIENTRY *gglVertexAttrib3dARB)(GLuint index, GLdouble x, GLdouble y, GLdouble z);
void (APIENTRY *gglVertexAttrib4sARB)(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
void (APIENTRY *gglVertexAttrib4fARB)(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void (APIENTRY *gglVertexAttrib4dARB)(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void (APIENTRY *gglVertexAttrib4NubARB)(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
void (APIENTRY *gglVertexAttrib1svARB)(GLuint index, const GLshort *v);
void (APIENTRY *gglVertexAttrib1fvARB)(GLuint index, const GLfloat *v);
void (APIENTRY *gglVertexAttrib1dvARB)(GLuint index, const GLdouble *v);
void (APIENTRY *gglVertexAttrib2svARB)(GLuint index, const GLshort *v);
void (APIENTRY *gglVertexAttrib2fvARB)(GLuint index, const GLfloat *v);
void (APIENTRY *gglVertexAttrib2dvARB)(GLuint index, const GLdouble *v);
void (APIENTRY *gglVertexAttrib3svARB)(GLuint index, const GLshort *v);
void (APIENTRY *gglVertexAttrib3fvARB)(GLuint index, const GLfloat *v);
void (APIENTRY *gglVertexAttrib3dvARB)(GLuint index, const GLdouble *v);
void (APIENTRY *gglVertexAttrib4bvARB)(GLuint index, const GLbyte *v);
void (APIENTRY *gglVertexAttrib4svARB)(GLuint index, const GLshort *v);
void (APIENTRY *gglVertexAttrib4ivARB)(GLuint index, const GLint *v);
void (APIENTRY *gglVertexAttrib4ubvARB)(GLuint index, const GLubyte *v);
void (APIENTRY *gglVertexAttrib4usvARB)(GLuint index, const GLushort *v);
void (APIENTRY *gglVertexAttrib4uivARB)(GLuint index, const GLuint *v);
void (APIENTRY *gglVertexAttrib4fvARB)(GLuint index, const GLfloat *v);
void (APIENTRY *gglVertexAttrib4dvARB)(GLuint index, const GLdouble *v);
void (APIENTRY *gglVertexAttrib4NbvARB)(GLuint index, const GLbyte *v);
void (APIENTRY *gglVertexAttrib4NsvARB)(GLuint index, const GLshort *v);
void (APIENTRY *gglVertexAttrib4NivARB)(GLuint index, const GLint *v);
void (APIENTRY *gglVertexAttrib4NubvARB)(GLuint index, const GLubyte *v);
void (APIENTRY *gglVertexAttrib4NusvARB)(GLuint index, const GLushort *v);
void (APIENTRY *gglVertexAttrib4NuivARB)(GLuint index, const GLuint *v);
void (APIENTRY *gglVertexAttribPointerARB)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
void (APIENTRY *gglEnableVertexAttribArrayARB)(GLuint index);
void (APIENTRY *gglDisableVertexAttribArrayARB)(GLuint index);
void (APIENTRY *gglProgramStringARB)(GLenum target, GLenum format, GLsizei len, const GLvoid *string);
void (APIENTRY *gglBindProgramARB)(GLenum target, GLuint program);
void (APIENTRY *gglDeleteProgramsARB)(GLsizei n, const GLuint *programs);
void (APIENTRY *gglGenProgramsARB)(GLsizei n, GLuint *programs);
void (APIENTRY *gglProgramEnvParameter4dARB)(GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void (APIENTRY *gglProgramEnvParameter4dvARB)(GLenum target, GLuint index, const GLdouble *params);
void (APIENTRY *gglProgramEnvParameter4fARB)(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void (APIENTRY *gglProgramEnvParameter4fvARB)(GLenum target, GLuint index, const GLfloat *params);
void (APIENTRY *gglProgramLocalParameter4dARB)(GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void (APIENTRY *gglProgramLocalParameter4dvARB)(GLenum target, GLuint index, const GLdouble *params);
void (APIENTRY *gglProgramLocalParameter4fARB)(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void (APIENTRY *gglProgramLocalParameter4fvARB)(GLenum target, GLuint index, const GLfloat *params);
void (APIENTRY *gglGetProgramEnvParameterdvARB)(GLenum target, GLuint index, GLdouble *params);
void (APIENTRY *gglGetProgramEnvParameterfvARB)(GLenum target, GLuint index, GLfloat *params);
void (APIENTRY *gglGetProgramLocalParameterdvARB)(GLenum target, GLuint index, GLdouble *params);
void (APIENTRY *gglGetProgramLocalParameterfvARB)(GLenum target, GLuint index, GLfloat *params);
void (APIENTRY *gglGetProgramivARB)(GLenum target, GLenum pname, GLint *params);
void (APIENTRY *gglGetProgramStringARB)(GLenum target, GLenum pname, GLvoid *string);
void (APIENTRY *gglGetVertexAttribdvARB)(GLuint index, GLenum pname, GLdouble *params);
void (APIENTRY *gglGetVertexAttribfvARB)(GLuint index, GLenum pname, GLfloat *params);
void (APIENTRY *gglGetVertexAttribivARB)(GLuint index, GLenum pname, GLint *params);
void (APIENTRY *gglGetVertexAttribPointervARB)(GLuint index, GLenum pname, GLvoid **pointer);
GLboolean (APIENTRY *gglIsProgramARB)(GLuint program);

//GL_ARB_fragment_program
void (APIENTRY *gglProgramStringARB)(GLenum target, GLenum format, GLsizei len, const GLvoid *string);
void (APIENTRY *gglBindProgramARB)(GLenum target, GLuint program);
void (APIENTRY *gglDeleteProgramsARB)(GLsizei n, const GLuint *programs);
void (APIENTRY *gglGenProgramsARB)(GLsizei n, GLuint *programs);
void (APIENTRY *gglProgramEnvParameter4dARB)(GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void (APIENTRY *gglProgramEnvParameter4dvARB)(GLenum target, GLuint index, const GLdouble *params);
void (APIENTRY *gglProgramEnvParameter4fARB)(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void (APIENTRY *gglProgramEnvParameter4fvARB)(GLenum target, GLuint index, const GLfloat *params);
void (APIENTRY *gglProgramLocalParameter4dARB)(GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void (APIENTRY *gglProgramLocalParameter4dvARB)(GLenum target, GLuint index, const GLdouble *params);
void (APIENTRY *gglProgramLocalParameter4fARB)(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void (APIENTRY *gglProgramLocalParameter4fvARB)(GLenum target, GLuint index, const GLfloat *params);
void (APIENTRY *gglGetProgramEnvParameterdvARB)(GLenum target, GLuint index, GLdouble *params);
void (APIENTRY *gglGetProgramEnvParameterfvARB)(GLenum target, GLuint index, GLfloat *params);
void (APIENTRY *gglGetProgramLocalParameterdvARB)(GLenum target, GLuint index, GLdouble *params);
void (APIENTRY *gglGetProgramLocalParameterfvARB)(GLenum target, GLuint index, GLfloat *params);
void (APIENTRY *gglGetProgramivARB)(GLenum target, GLenum pname, GLint *params);
void (APIENTRY *gglGetProgramStringARB)(GLenum target, GLenum pname, GLvoid *string);
GLboolean (APIENTRY *gglIsProgramARB)(GLuint program);

//GL_ARB_vertex_buffer_object
void (APIENTRY *gglBindBufferARB)(GLenum target, GLuint buffer);
void (APIENTRY *gglDeleteBuffersARB)(GLsizei n, const GLuint *buffers);
void (APIENTRY *gglGenBuffersARB)(GLsizei n, GLuint *buffers);
GLboolean (APIENTRY *gglIsBufferARB)(GLuint buffer);
void (APIENTRY *gglBufferDataARB)(GLenum target, GLsizeiptrARB size, const GLvoid *data, GLenum usage);
void (APIENTRY *gglBufferSubDataARB)(GLenum target, GLintptrARB offset, GLsizeiptrARB size, const GLvoid *data);
void (APIENTRY *gglGetBufferSubDataARB)(GLenum target, GLintptrARB offset, GLsizeiptrARB size, GLvoid *data);
void *(APIENTRY *gglMapBufferARB)(GLenum target, GLenum access);
GLboolean (APIENTRY *gglUnmapBufferARB)(GLenum target);
void (APIENTRY *gglGetBufferParameterivARB)(GLenum target, GLenum pname, GLint *params);
void (APIENTRY *gglGetBufferPointervARB)(GLenum target, GLenum pname, GLvoid **params);

//GL_ARB_occlusion_query
void (APIENTRY *gglGenQueriesARB)(GLsizei n, GLuint *ids);
void (APIENTRY *gglDeleteQueriesARB)(GLsizei n, const GLuint *ids);
GLboolean (APIENTRY *gglIsQueryARB)(GLuint id);
void (APIENTRY *gglBeginQueryARB)(GLenum target, GLuint id);
void (APIENTRY *gglEndQueryARB)(GLenum target);
void (APIENTRY *gglGetQueryivARB)(GLenum target, GLenum pname, GLint *params);
void (APIENTRY *gglGetQueryObjectivARB)(GLuint id, GLenum pname, GLint *params);
void (APIENTRY *gglGetQueryObjectuivARB)(GLuint id, GLenum pname, GLuint *params);

//GL_ARB_shader_objects
void (APIENTRY *gglDeleteObjectARB)(GLhandleARB obj);
GLhandleARB (APIENTRY *gglGetHandleARB)(GLenum pname);
void (APIENTRY *gglDetachObjectARB)(GLhandleARB containerObj, GLhandleARB attachedObj);
GLhandleARB (APIENTRY *gglCreateShaderObjectARB)(GLenum shaderType);
void (APIENTRY *gglShaderSourceARB)(GLhandleARB shaderObj, GLsizei count, const GLcharARB* *string, const GLint *length);
void (APIENTRY *gglCompileShaderARB)(GLhandleARB shaderObj);
GLhandleARB (APIENTRY *gglCreateProgramObjectARB)(void);
void (APIENTRY *gglAttachObjectARB)(GLhandleARB containerObj, GLhandleARB obj);
void (APIENTRY *gglLinkProgramARB)(GLhandleARB programObj);
void (APIENTRY *gglUseProgramObjectARB)(GLhandleARB programObj);
void (APIENTRY *gglValidateProgramARB)(GLhandleARB programObj);
void (APIENTRY *gglUniform1fARB)(GLint location, GLfloat v0);
void (APIENTRY *gglUniform2fARB)(GLint location, GLfloat v0, GLfloat v1);
void (APIENTRY *gglUniform3fARB)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
void (APIENTRY *gglUniform4fARB)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
void (APIENTRY *gglUniform1iARB)(GLint location, GLint v0);
void (APIENTRY *gglUniform2iARB)(GLint location, GLint v0, GLint v1);
void (APIENTRY *gglUniform3iARB)(GLint location, GLint v0, GLint v1, GLint v2);
void (APIENTRY *gglUniform4iARB)(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
void (APIENTRY *gglUniform1fvARB)(GLint location, GLsizei count, const GLfloat *value);
void (APIENTRY *gglUniform2fvARB)(GLint location, GLsizei count, const GLfloat *value);
void (APIENTRY *gglUniform3fvARB)(GLint location, GLsizei count, const GLfloat *value);
void (APIENTRY *gglUniform4fvARB)(GLint location, GLsizei count, const GLfloat *value);
void (APIENTRY *gglUniform1ivARB)(GLint location, GLsizei count, const GLint *value);
void (APIENTRY *gglUniform2ivARB)(GLint location, GLsizei count, const GLint *value);
void (APIENTRY *gglUniform3ivARB)(GLint location, GLsizei count, const GLint *value);
void (APIENTRY *gglUniform4ivARB)(GLint location, GLsizei count, const GLint *value);
void (APIENTRY *gglUniformMatrix2fvARB)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void (APIENTRY *gglUniformMatrix3fvARB)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void (APIENTRY *gglUniformMatrix4fvARB)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void (APIENTRY *gglGetObjectParameterfvARB)(GLhandleARB obj, GLenum pname, GLfloat *params);
void (APIENTRY *gglGetObjectParameterivARB)(GLhandleARB obj, GLenum pname, GLint *params);
void (APIENTRY *gglGetInfoLogARB)(GLhandleARB obj, GLsizei maxLength, GLsizei *length, GLcharARB *infoLog);
void (APIENTRY *gglGetAttachedObjectsARB)(GLhandleARB containerObj, GLsizei maxCount, GLsizei *count, GLhandleARB *obj);
GLint (APIENTRY *gglGetUniformLocationARB)(GLhandleARB programObj, const GLcharARB *name);
void (APIENTRY *gglGetActiveUniformARB)(GLhandleARB programObj, GLuint index, GLsizei maxLength, GLsizei *length, GLint *size, GLenum *type, GLcharARB *name);
void (APIENTRY *gglGetUniformfvARB)(GLhandleARB programObj, GLint location, GLfloat *params);
void (APIENTRY *gglGetUniformivARB)(GLhandleARB programObj, GLint location, GLint *params);
void (APIENTRY *gglGetShaderSourceARB)(GLhandleARB obj, GLsizei maxLength, GLsizei *length, GLcharARB *source);

//GL_ARB_vertex_shader
void (APIENTRY *gglBindAttribLocationARB)(GLhandleARB programObj, GLuint index, const GLcharARB *name);
void (APIENTRY *gglGetActiveAttribARB)(GLhandleARB programObj, GLuint index, GLsizei maxLength, GLsizei *length, GLint *size, GLenum *type, GLcharARB *name);
GLint (APIENTRY *gglGetAttribLocationARB)(GLhandleARB programObj, const GLcharARB *name);

//GL_ARB_draw_buffers
void (APIENTRY *gglDrawBuffersARB)(GLsizei n, const GLenum *bufs);

//GL_ARB_color_buffer_float
void (APIENTRY *gglClampColorARB)(GLenum target, GLenum clamp);

//GL_ARB_draw_instanced
void (APIENTRY *gglDrawArraysInstancedARB)(GLenum mode, GLint first, GLsizei count, GLsizei primcount);
void (APIENTRY *gglDrawElementsInstancedARB)(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei primcount);

//GL_ARB_framebuffer_object
//part of OpenGL 3.0, defined as a separate extension for older hardware that can't support 3.0
GLboolean (APIENTRY *gglIsRenderbuffer)(GLuint renderbuffer);
void (APIENTRY *gglBindRenderbuffer)(GLenum target, GLuint renderbuffer);
void (APIENTRY *gglDeleteRenderbuffers)(GLsizei n, const GLuint *renderbuffers);
void (APIENTRY *gglGenRenderbuffers)(GLsizei n, GLuint *renderbuffers);
void (APIENTRY *gglRenderbufferStorage)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
void (APIENTRY *gglGetRenderbufferParameteriv)(GLenum target, GLenum pname, GLint *params);
GLboolean (APIENTRY *gglIsFramebuffer)(GLuint framebuffer);
void (APIENTRY *gglBindFramebuffer)(GLenum target, GLuint framebuffer);
void (APIENTRY *gglDeleteFramebuffers)(GLsizei n, const GLuint *framebuffers);
void (APIENTRY *gglGenFramebuffers)(GLsizei n, GLuint *framebuffers);
GLenum (APIENTRY *gglCheckFramebufferStatus)(GLenum target);
void (APIENTRY *gglFramebufferTexture1D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
void (APIENTRY *gglFramebufferTexture2D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
void (APIENTRY *gglFramebufferTexture3D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
void (APIENTRY *gglFramebufferRenderbuffer)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
void (APIENTRY *gglGetFramebufferAttachmentParameteriv)(GLenum target, GLenum attachment, GLenum pname, GLint *params);
void (APIENTRY *gglGenerateMipmap)(GLenum target);
void (APIENTRY *gglBlitFramebuffer)(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
void (APIENTRY *gglRenderbufferStorageMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
void (APIENTRY *gglFramebufferTextureLayer)(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);

//GL_ARB_geometry_shader4
void (APIENTRY *gglProgramParameteriARB)(GLuint program, GLenum pname, GLint value);
void (APIENTRY *gglFramebufferTextureARB)(GLenum target, GLenum attachment, GLuint texture, GLint level);
void (APIENTRY *gglFramebufferTextureLayerARB)(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
void (APIENTRY *gglFramebufferTextureFaceARB)(GLenum target, GLenum attachment, GLuint texture, GLint level, GLenum face);

//GL_ARB_instanced_arrays
//part of OpenGL 3.3, defined as a separate extension for older hardware that can't support 3.3
void (APIENTRYP *gglVertexAttribDivisor)(GLuint index, GLuint divisor);

//GL_ARB_map_buffer_range
//part of OpenGL 3.0, defined as a separate extension for older hardware that can't support 3.0
void (APIENTRY *gglMapBufferRange)(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
void (APIENTRY *gglFlushMappedBufferRange)(GLenum target, GLintptr offset, GLsizeiptr length);

//GL_ARB_texture_buffer_object
void (APIENTRY *gglTexBufferARB)(GLenum target, GLenum internalformat, GLuint buffer);

//GL_ARB_vertex_array_object
//part of OpenGL 3.0, defined as a separate extension for older hardware that can't support 3.0
void (APIENTRY *gglBindVertexArray)(GLuint array);
void (APIENTRY *gglDeleteVertexArrays)(GLsizei n, const GLuint *arrays);
void (APIENTRY *gglGenVertexArrays)(GLsizei n, GLuint *arrays);
GLboolean (APIENTRY *gglIsVertexArray)(GLuint array);

//GL_EXT_blend_color
void (APIENTRY *gglBlendColorEXT)(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);

//GL_EXT_polygon_offset
void (APIENTRY *gglPolygonOffsetEXT)(GLfloat factor, GLfloat bias);

//GL_EXT_texture3D
void (APIENTRY *gglTexImage3DEXT)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
void (APIENTRY *gglTexSubImage3DEXT)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels);

//GL_SGIS_texture_filter4
void (APIENTRY *gglGetTexFilterFuncSGIS)(GLenum target, GLenum filter, GLfloat *weights);
void (APIENTRY *gglTexFilterFuncSGIS)(GLenum target, GLenum filter, GLsizei n, const GLfloat *weights);

//GL_EXT_subtexture
void (APIENTRY *gglTexSubImage1DEXT)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels);
void (APIENTRY *gglTexSubImage2DEXT)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);

//GL_EXT_copy_texture
void (APIENTRY *gglCopyTexImage1DEXT)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
void (APIENTRY *gglCopyTexImage2DEXT)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
void (APIENTRY *gglCopyTexSubImage1DEXT)(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
void (APIENTRY *gglCopyTexSubImage2DEXT)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
void (APIENTRY *gglCopyTexSubImage3DEXT)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);

//GL_EXT_histogram
void (APIENTRY *gglGetHistogramEXT)(GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid *values);
void (APIENTRY *gglGetHistogramParameterfvEXT)(GLenum target, GLenum pname, GLfloat *params);
void (APIENTRY *gglGetHistogramParameterivEXT)(GLenum target, GLenum pname, GLint *params);
void (APIENTRY *gglGetMinmaxEXT)(GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid *values);
void (APIENTRY *gglGetMinmaxParameterfvEXT)(GLenum target, GLenum pname, GLfloat *params);
void (APIENTRY *gglGetMinmaxParameterivEXT)(GLenum target, GLenum pname, GLint *params);
void (APIENTRY *gglHistogramEXT)(GLenum target, GLsizei width, GLenum internalformat, GLboolean sink);
void (APIENTRY *gglMinmaxEXT)(GLenum target, GLenum internalformat, GLboolean sink);
void (APIENTRY *gglResetHistogramEXT)(GLenum target);
void (APIENTRY *gglResetMinmaxEXT)(GLenum target);

//GL_EXT_convolution
void (APIENTRY *gglConvolutionFilter1DEXT)(GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *image);
void (APIENTRY *gglConvolutionFilter2DEXT)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *image);
void (APIENTRY *gglConvolutionParameterfEXT)(GLenum target, GLenum pname, GLfloat params);
void (APIENTRY *gglConvolutionParameterfvEXT)(GLenum target, GLenum pname, const GLfloat *params);
void (APIENTRY *gglConvolutionParameteriEXT)(GLenum target, GLenum pname, GLint params);
void (APIENTRY *gglConvolutionParameterivEXT)(GLenum target, GLenum pname, const GLint *params);
void (APIENTRY *gglCopyConvolutionFilter1DEXT)(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
void (APIENTRY *gglCopyConvolutionFilter2DEXT)(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height);
void (APIENTRY *gglGetConvolutionFilterEXT)(GLenum target, GLenum format, GLenum type, GLvoid *image);
void (APIENTRY *gglGetConvolutionParameterfvEXT)(GLenum target, GLenum pname, GLfloat *params);
void (APIENTRY *gglGetConvolutionParameterivEXT)(GLenum target, GLenum pname, GLint *params);
void (APIENTRY *gglGetSeparableFilterEXT)(GLenum target, GLenum format, GLenum type, GLvoid *row, GLvoid *column, GLvoid *span);
void (APIENTRY *gglSeparableFilter2DEXT)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *row, const GLvoid *column);

//GL_SGI_color_table
void (APIENTRY *gglColorTableSGI)(GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *table);
void (APIENTRY *gglColorTableParameterfvSGI)(GLenum target, GLenum pname, const GLfloat *params);
void (APIENTRY *gglColorTableParameterivSGI)(GLenum target, GLenum pname, const GLint *params);
void (APIENTRY *gglCopyColorTableSGI)(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
void (APIENTRY *gglGetColorTableSGI)(GLenum target, GLenum format, GLenum type, GLvoid *table);
void (APIENTRY *gglGetColorTableParameterfvSGI)(GLenum target, GLenum pname, GLfloat *params);
void (APIENTRY *gglGetColorTableParameterivSGI)(GLenum target, GLenum pname, GLint *params);

//GL_SGIX_pixel_texture
void (APIENTRY *gglPixelTexGenSGIX)(GLenum mode);

//GL_SGIS_pixel_texture
void (APIENTRY *gglPixelTexGenParameteriSGIS)(GLenum pname, GLint param);
void (APIENTRY *gglPixelTexGenParameterivSGIS)(GLenum pname, const GLint *params);
void (APIENTRY *gglPixelTexGenParameterfSGIS)(GLenum pname, GLfloat param);
void (APIENTRY *gglPixelTexGenParameterfvSGIS)(GLenum pname, const GLfloat *params);
void (APIENTRY *gglGetPixelTexGenParameterivSGIS)(GLenum pname, GLint *params);
void (APIENTRY *gglGetPixelTexGenParameterfvSGIS)(GLenum pname, GLfloat *params);

//GL_SGIS_texture4D
void (APIENTRY *gglTexImage4DSGIS)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLsizei size4d, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
void (APIENTRY *gglTexSubImage4DSGIS)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint woffset, GLsizei width, GLsizei height, GLsizei depth, GLsizei size4d, GLenum format, GLenum type, const GLvoid *pixels);

//GL_EXT_texture_object
GLboolean (APIENTRY *gglAreTexturesResidentEXT)(GLsizei n, const GLuint *textures, GLboolean *residences);
void (APIENTRY *gglBindTextureEXT)(GLenum target, GLuint texture);
void (APIENTRY *gglDeleteTexturesEXT)(GLsizei n, const GLuint *textures);
void (APIENTRY *gglGenTexturesEXT)(GLsizei n, GLuint *textures);
GLboolean (APIENTRY *gglIsTextureEXT)(GLuint texture);
void (APIENTRY *gglPrioritizeTexturesEXT)(GLsizei n, const GLuint *textures, const GLclampf *priorities);

//GL_SGIS_detail_texture
void (APIENTRY *gglDetailTexFuncSGIS)(GLenum target, GLsizei n, const GLfloat *points);
void (APIENTRY *gglGetDetailTexFuncSGIS)(GLenum target, GLfloat *points);

//GL_SGIS_sharpen_texture
void (APIENTRY *gglSharpenTexFuncSGIS)(GLenum target, GLsizei n, const GLfloat *points);
void (APIENTRY *gglGetSharpenTexFuncSGIS)(GLenum target, GLfloat *points);

//GL_SGIS_multisample
void (APIENTRY *gglSampleMaskSGIS)(GLclampf value, GLboolean invert);
void (APIENTRY *gglSamplePatternSGIS)(GLenum pattern);

//GL_EXT_vertex_array
void (APIENTRY *gglArrayElementEXT)(GLint i);
void (APIENTRY *gglColorPointerEXT)(GLint size, GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer);
void (APIENTRY *gglDrawArraysEXT)(GLenum mode, GLint first, GLsizei count);
void (APIENTRY *gglEdgeFlagPointerEXT)(GLsizei stride, GLsizei count, const GLboolean *pointer);
void (APIENTRY *gglGetPointervEXT)(GLenum pname, GLvoid* *params);
void (APIENTRY *gglIndexPointerEXT)(GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer);
void (APIENTRY *gglNormalPointerEXT)(GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer);
void (APIENTRY *gglTexCoordPointerEXT)(GLint size, GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer);
void (APIENTRY *gglVertexPointerEXT)(GLint size, GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer);

//GL_EXT_blend_minmax
void (APIENTRY *gglBlendEquationEXT)(GLenum mode);

//GL_SGIX_sprite
void (APIENTRY *gglSpriteParameterfSGIX)(GLenum pname, GLfloat param);
void (APIENTRY *gglSpriteParameterfvSGIX)(GLenum pname, const GLfloat *params);
void (APIENTRY *gglSpriteParameteriSGIX)(GLenum pname, GLint param);
void (APIENTRY *gglSpriteParameterivSGIX)(GLenum pname, const GLint *params);

//GL_EXT_point_parameters
void (APIENTRY *gglPointParameterfEXT)(GLenum pname, GLfloat param);
void (APIENTRY *gglPointParameterfvEXT)(GLenum pname, const GLfloat *params);

//GL_SGIS_point_parameters
void (APIENTRY *gglPointParameterfSGIS)(GLenum pname, GLfloat param);
void (APIENTRY *gglPointParameterfvSGIS)(GLenum pname, const GLfloat *params);

//GL_SGIX_instruments
GLint (APIENTRY *gglGetInstrumentsSGIX)(void);
void (APIENTRY *gglInstrumentsBufferSGIX)(GLsizei size, GLint *buffer);
GLint (APIENTRY *gglPollInstrumentsSGIX)(GLint *marker_p);
void (APIENTRY *gglReadInstrumentsSGIX)(GLint marker);
void (APIENTRY *gglStartInstrumentsSGIX)(void);
void (APIENTRY *gglStopInstrumentsSGIX)(GLint marker);

//GL_SGIX_framezoom
void (APIENTRY *gglFrameZoomSGIX)(GLint factor);

//GL_SGIX_tag_sample_buffer
void (APIENTRY *gglTagSampleBufferSGIX)(void);

//GL_SGIX_polynomial_ffd
void (APIENTRY *gglDeformationMap3dSGIX)(GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, GLdouble w1, GLdouble w2, GLint wstride, GLint worder, const GLdouble *points);
void (APIENTRY *gglDeformationMap3fSGIX)(GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, GLfloat w1, GLfloat w2, GLint wstride, GLint worder, const GLfloat *points);
void (APIENTRY *gglDeformSGIX)(GLbitfield mask);
void (APIENTRY *gglLoadIdentityDeformationMapSGIX)(GLbitfield mask);

//GL_SGIX_reference_plane
void (APIENTRY *gglReferencePlaneSGIX)(const GLdouble *equation);

//GL_SGIX_flush_raster
void (APIENTRY *gglFlushRasterSGIX)(void);

//GL_SGIS_fog_function
void (APIENTRY *gglFogFuncSGIS)(GLsizei n, const GLfloat *points);
void (APIENTRY *gglGetFogFuncSGIS)(GLfloat *points);

//GL_HP_image_transform
void (APIENTRY *gglImageTransformParameteriHP)(GLenum target, GLenum pname, GLint param);
void (APIENTRY *gglImageTransformParameterfHP)(GLenum target, GLenum pname, GLfloat param);
void (APIENTRY *gglImageTransformParameterivHP)(GLenum target, GLenum pname, const GLint *params);
void (APIENTRY *gglImageTransformParameterfvHP)(GLenum target, GLenum pname, const GLfloat *params);
void (APIENTRY *gglGetImageTransformParameterivHP)(GLenum target, GLenum pname, GLint *params);
void (APIENTRY *gglGetImageTransformParameterfvHP)(GLenum target, GLenum pname, GLfloat *params);

//GL_EXT_color_subtable
void (APIENTRY *gglColorSubTableEXT)(GLenum target, GLsizei start, GLsizei count, GLenum format, GLenum type, const GLvoid *data);
void (APIENTRY *gglCopyColorSubTableEXT)(GLenum target, GLsizei start, GLint x, GLint y, GLsizei width);

//GL_PGI_misc_hints
void (APIENTRY *gglHintPGI)(GLenum target, GLint mode);

//GL_EXT_paletted_texture
void (APIENTRY *gglColorTableEXT)(GLenum target, GLenum internalFormat, GLsizei width, GLenum format, GLenum type, const GLvoid *table);
void (APIENTRY *gglGetColorTableEXT)(GLenum target, GLenum format, GLenum type, GLvoid *data);
void (APIENTRY *gglGetColorTableParameterivEXT)(GLenum target, GLenum pname, GLint *params);
void (APIENTRY *gglGetColorTableParameterfvEXT)(GLenum target, GLenum pname, GLfloat *params);

//GL_SGIX_list_priority
void (APIENTRY *gglGetListParameterfvSGIX)(GLuint list, GLenum pname, GLfloat *params);
void (APIENTRY *gglGetListParameterivSGIX)(GLuint list, GLenum pname, GLint *params);
void (APIENTRY *gglListParameterfSGIX)(GLuint list, GLenum pname, GLfloat param);
void (APIENTRY *gglListParameterfvSGIX)(GLuint list, GLenum pname, const GLfloat *params);
void (APIENTRY *gglListParameteriSGIX)(GLuint list, GLenum pname, GLint param);
void (APIENTRY *gglListParameterivSGIX)(GLuint list, GLenum pname, const GLint *params);

//GL_EXT_index_material
void (APIENTRY *gglIndexMaterialEXT)(GLenum face, GLenum mode);

//GL_EXT_index_func
void (APIENTRY *gglIndexFuncEXT)(GLenum func, GLclampf ref);

//GL_EXT_compiled_vertex_array
void (APIENTRY *gglLockArraysEXT)(GLint first, GLsizei count);
void (APIENTRY *gglUnlockArraysEXT)(void);

//GL_EXT_cull_vertex
void (APIENTRY *gglCullParameterdvEXT)(GLenum pname, GLdouble *params);
void (APIENTRY *gglCullParameterfvEXT)(GLenum pname, GLfloat *params);

//GL_SGIX_fragment_lighting
void (APIENTRY *gglFragmentColorMaterialSGIX)(GLenum face, GLenum mode);
void (APIENTRY *gglFragmentLightfSGIX)(GLenum light, GLenum pname, GLfloat param);
void (APIENTRY *gglFragmentLightfvSGIX)(GLenum light, GLenum pname, const GLfloat *params);
void (APIENTRY *gglFragmentLightiSGIX)(GLenum light, GLenum pname, GLint param);
void (APIENTRY *gglFragmentLightivSGIX)(GLenum light, GLenum pname, const GLint *params);
void (APIENTRY *gglFragmentLightModelfSGIX)(GLenum pname, GLfloat param);
void (APIENTRY *gglFragmentLightModelfvSGIX)(GLenum pname, const GLfloat *params);
void (APIENTRY *gglFragmentLightModeliSGIX)(GLenum pname, GLint param);
void (APIENTRY *gglFragmentLightModelivSGIX)(GLenum pname, const GLint *params);
void (APIENTRY *gglFragmentMaterialfSGIX)(GLenum face, GLenum pname, GLfloat param);
void (APIENTRY *gglFragmentMaterialfvSGIX)(GLenum face, GLenum pname, const GLfloat *params);
void (APIENTRY *gglFragmentMaterialiSGIX)(GLenum face, GLenum pname, GLint param);
void (APIENTRY *gglFragmentMaterialivSGIX)(GLenum face, GLenum pname, const GLint *params);
void (APIENTRY *gglGetFragmentLightfvSGIX)(GLenum light, GLenum pname, GLfloat *params);
void (APIENTRY *gglGetFragmentLightivSGIX)(GLenum light, GLenum pname, GLint *params);
void (APIENTRY *gglGetFragmentMaterialfvSGIX)(GLenum face, GLenum pname, GLfloat *params);
void (APIENTRY *gglGetFragmentMaterialivSGIX)(GLenum face, GLenum pname, GLint *params);
void (APIENTRY *gglLightEnviSGIX)(GLenum pname, GLint param);

//GL_EXT_fragment_lighting
void (APIENTRY *gglFragmentColorMaterialEXT)(GLenum face, GLenum mode);
void (APIENTRY *gglFragmentLightfEXT)(GLenum light, GLenum pname, GLfloat param);
void (APIENTRY *gglFragmentLightfvEXT)(GLenum light, GLenum pname, const GLfloat *params);
void (APIENTRY *gglFragmentLightiEXT)(GLenum light, GLenum pname, GLint param);
void (APIENTRY *gglFragmentLightivEXT)(GLenum light, GLenum pname, const GLint *params);
void (APIENTRY *gglFragmentLightModelfEXT)(GLenum pname, GLfloat param);
void (APIENTRY *gglFragmentLightModelfvEXT)(GLenum pname, const GLfloat *params);
void (APIENTRY *gglFragmentLightModeliEXT)(GLenum pname, GLint param);
void (APIENTRY *gglFragmentLightModelivEXT)(GLenum pname, const GLint *params);
void (APIENTRY *gglFragmentMaterialfEXT)(GLenum face, GLenum pname, GLfloat param);
void (APIENTRY *gglFragmentMaterialfvEXT)(GLenum face, GLenum pname, const GLfloat *params);
void (APIENTRY *gglFragmentMaterialiEXT)(GLenum face, GLenum pname, GLint param);
void (APIENTRY *gglFragmentMaterialivEXT)(GLenum face, GLenum pname, const GLint *params);
void (APIENTRY *gglGetFragmentLightfvEXT)(GLenum light, GLenum pname, GLfloat *params);
void (APIENTRY *gglGetFragmentLightivEXT)(GLenum light, GLenum pname, GLint *params);
void (APIENTRY *gglGetFragmentMaterialfvEXT)(GLenum face, GLenum pname, GLfloat *params);
void (APIENTRY *gglGetFragmentMaterialivEXT)(GLenum face, GLenum pname, GLint *params);
void (APIENTRY *gglLightEnviEXT)(GLenum pname, GLint param);

//GL_EXT_draw_range_elements
void (APIENTRY *gglDrawRangeElementsEXT)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices);

//GL_EXT_light_texture
void (APIENTRY *gglApplyTextureEXT)(GLenum mode);
void (APIENTRY *gglTextureLightEXT)(GLenum pname);
void (APIENTRY *gglTextureMaterialEXT)(GLenum face, GLenum mode);

//GL_EXT_scene_marker
void (APIENTRY *gglBeginSceneEXT)(GLvoid);
void (APIENTRY *gglEndSceneEXT)(GLvoid);

//GL_SGIX_async
void (APIENTRY *gglAsyncMarkerSGIX)(GLuint marker);
GLint (APIENTRY *gglFinishAsyncSGIX)(GLuint *markerp);
GLint (APIENTRY *gglPollAsyncSGIX)(GLuint *markerp);
GLuint (APIENTRY *gglGenAsyncMarkersSGIX)(GLsizei range);
void (APIENTRY *gglDeleteAsyncMarkersSGIX)(GLuint marker, GLsizei range);
GLboolean (APIENTRY *gglIsAsyncMarkerSGIX)(GLuint marker);

//GL_INTEL_texture_scissor
void (APIENTRY *gglTexScissorINTEL)(GLenum target, GLclampf tlow, GLclampf thigh);
void (APIENTRY *gglTexScissorFuncINTEL)(GLenum target, GLenum lfunc, GLenum hfunc);

//GL_INTEL_parallel_arrays
void (APIENTRY *gglVertexPointervINTEL)(GLint size, GLenum type, const GLvoid* *pointer);
void (APIENTRY *gglNormalPointervINTEL)(GLenum type, const GLvoid* *pointer);
void (APIENTRY *gglColorPointervINTEL)(GLint size, GLenum type, const GLvoid* *pointer);
void (APIENTRY *gglTexCoordPointervINTEL)(GLint size, GLenum type, const GLvoid* *pointer);

//GL_EXT_pixel_transform
void (APIENTRY *gglPixelTransformParameteriEXT)(GLenum target, GLenum pname, GLint param);
void (APIENTRY *gglPixelTransformParameterfEXT)(GLenum target, GLenum pname, GLfloat param);
void (APIENTRY *gglPixelTransformParameterivEXT)(GLenum target, GLenum pname, const GLint *params);
void (APIENTRY *gglPixelTransformParameterfvEXT)(GLenum target, GLenum pname, const GLfloat *params);

//GL_EXT_secondary_color
void (APIENTRY *gglSecondaryColor3bEXT)(GLbyte red, GLbyte green, GLbyte blue);
void (APIENTRY *gglSecondaryColor3bvEXT)(const GLbyte *v);
void (APIENTRY *gglSecondaryColor3dEXT)(GLdouble red, GLdouble green, GLdouble blue);
void (APIENTRY *gglSecondaryColor3dvEXT)(const GLdouble *v);
void (APIENTRY *gglSecondaryColor3fEXT)(GLfloat red, GLfloat green, GLfloat blue);
void (APIENTRY *gglSecondaryColor3fvEXT)(const GLfloat *v);
void (APIENTRY *gglSecondaryColor3iEXT)(GLint red, GLint green, GLint blue);
void (APIENTRY *gglSecondaryColor3ivEXT)(const GLint *v);
void (APIENTRY *gglSecondaryColor3sEXT)(GLshort red, GLshort green, GLshort blue);
void (APIENTRY *gglSecondaryColor3svEXT)(const GLshort *v);
void (APIENTRY *gglSecondaryColor3ubEXT)(GLubyte red, GLubyte green, GLubyte blue);
void (APIENTRY *gglSecondaryColor3ubvEXT)(const GLubyte *v);
void (APIENTRY *gglSecondaryColor3uiEXT)(GLuint red, GLuint green, GLuint blue);
void (APIENTRY *gglSecondaryColor3uivEXT)(const GLuint *v);
void (APIENTRY *gglSecondaryColor3usEXT)(GLushort red, GLushort green, GLushort blue);
void (APIENTRY *gglSecondaryColor3usvEXT)(const GLushort *v);
void (APIENTRY *gglSecondaryColorPointerEXT)(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);

//GL_EXT_texture_perturb_normal
void (APIENTRY *gglTextureNormalEXT)(GLenum mode);

//GL_EXT_multi_draw_arrays
void (APIENTRY *gglMultiDrawArraysEXT)(GLenum mode, GLint *first, GLsizei *count, GLsizei primcount);
void (APIENTRY *gglMultiDrawElementsEXT)(GLenum mode, const GLsizei *count, GLenum type, const GLvoid* *indices, GLsizei primcount);

//GL_EXT_fog_coord
void (APIENTRY *gglFogCoordfEXT)(GLfloat coord);
void (APIENTRY *gglFogCoordfvEXT)(const GLfloat *coord);
void (APIENTRY *gglFogCoorddEXT)(GLdouble coord);
void (APIENTRY *gglFogCoorddvEXT)(const GLdouble *coord);
void (APIENTRY *gglFogCoordPointerEXT)(GLenum type, GLsizei stride, const GLvoid *pointer);

//GL_EXT_coordinate_frame
void (APIENTRY *gglTangent3bEXT)(GLbyte tx, GLbyte ty, GLbyte tz);
void (APIENTRY *gglTangent3bvEXT)(const GLbyte *v);
void (APIENTRY *gglTangent3dEXT)(GLdouble tx, GLdouble ty, GLdouble tz);
void (APIENTRY *gglTangent3dvEXT)(const GLdouble *v);
void (APIENTRY *gglTangent3fEXT)(GLfloat tx, GLfloat ty, GLfloat tz);
void (APIENTRY *gglTangent3fvEXT)(const GLfloat *v);
void (APIENTRY *gglTangent3iEXT)(GLint tx, GLint ty, GLint tz);
void (APIENTRY *gglTangent3ivEXT)(const GLint *v);
void (APIENTRY *gglTangent3sEXT)(GLshort tx, GLshort ty, GLshort tz);
void (APIENTRY *gglTangent3svEXT)(const GLshort *v);
void (APIENTRY *gglBinormal3bEXT)(GLbyte bx, GLbyte by, GLbyte bz);
void (APIENTRY *gglBinormal3bvEXT)(const GLbyte *v);
void (APIENTRY *gglBinormal3dEXT)(GLdouble bx, GLdouble by, GLdouble bz);
void (APIENTRY *gglBinormal3dvEXT)(const GLdouble *v);
void (APIENTRY *gglBinormal3fEXT)(GLfloat bx, GLfloat by, GLfloat bz);
void (APIENTRY *gglBinormal3fvEXT)(const GLfloat *v);
void (APIENTRY *gglBinormal3iEXT)(GLint bx, GLint by, GLint bz);
void (APIENTRY *gglBinormal3ivEXT)(const GLint *v);
void (APIENTRY *gglBinormal3sEXT)(GLshort bx, GLshort by, GLshort bz);
void (APIENTRY *gglBinormal3svEXT)(const GLshort *v);
void (APIENTRY *gglTangentPointerEXT)(GLenum type, GLsizei stride, const GLvoid *pointer);
void (APIENTRY *gglBinormalPointerEXT)(GLenum type, GLsizei stride, const GLvoid *pointer);

//GL_SUNX_constant_data
void (APIENTRY *gglFinishTextureSUNX)(void);

//GL_SUN_global_alpha
void (APIENTRY *gglGlobalAlphaFactorbSUN)(GLbyte factor);
void (APIENTRY *gglGlobalAlphaFactorsSUN)(GLshort factor);
void (APIENTRY *gglGlobalAlphaFactoriSUN)(GLint factor);
void (APIENTRY *gglGlobalAlphaFactorfSUN)(GLfloat factor);
void (APIENTRY *gglGlobalAlphaFactordSUN)(GLdouble factor);
void (APIENTRY *gglGlobalAlphaFactorubSUN)(GLubyte factor);
void (APIENTRY *gglGlobalAlphaFactorusSUN)(GLushort factor);
void (APIENTRY *gglGlobalAlphaFactoruiSUN)(GLuint factor);

//GL_SUN_triangle_list
void (APIENTRY *gglReplacementCodeuiSUN)(GLuint code);
void (APIENTRY *gglReplacementCodeusSUN)(GLushort code);
void (APIENTRY *gglReplacementCodeubSUN)(GLubyte code);
void (APIENTRY *gglReplacementCodeuivSUN)(const GLuint *code);
void (APIENTRY *gglReplacementCodeusvSUN)(const GLushort *code);
void (APIENTRY *gglReplacementCodeubvSUN)(const GLubyte *code);
void (APIENTRY *gglReplacementCodePointerSUN)(GLenum type, GLsizei stride, const GLvoid* *pointer);

//GL_SUN_vertex
void (APIENTRY *gglColor4ubVertex2fSUN)(GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y);
void (APIENTRY *gglColor4ubVertex2fvSUN)(const GLubyte *c, const GLfloat *v);
void (APIENTRY *gglColor4ubVertex3fSUN)(GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y, GLfloat z);
void (APIENTRY *gglColor4ubVertex3fvSUN)(const GLubyte *c, const GLfloat *v);
void (APIENTRY *gglColor3fVertex3fSUN)(GLfloat r, GLfloat g, GLfloat b, GLfloat x, GLfloat y, GLfloat z);
void (APIENTRY *gglColor3fVertex3fvSUN)(const GLfloat *c, const GLfloat *v);
void (APIENTRY *gglNormal3fVertex3fSUN)(GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
void (APIENTRY *gglNormal3fVertex3fvSUN)(const GLfloat *n, const GLfloat *v);
void (APIENTRY *gglColor4fNormal3fVertex3fSUN)(GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
void (APIENTRY *gglColor4fNormal3fVertex3fvSUN)(const GLfloat *c, const GLfloat *n, const GLfloat *v);
void (APIENTRY *gglTexCoord2fVertex3fSUN)(GLfloat s, GLfloat t, GLfloat x, GLfloat y, GLfloat z);
void (APIENTRY *gglTexCoord2fVertex3fvSUN)(const GLfloat *tc, const GLfloat *v);
void (APIENTRY *gglTexCoord4fVertex4fSUN)(GLfloat s, GLfloat t, GLfloat p, GLfloat q, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void (APIENTRY *gglTexCoord4fVertex4fvSUN)(const GLfloat *tc, const GLfloat *v);
void (APIENTRY *gglTexCoord2fColor4ubVertex3fSUN)(GLfloat s, GLfloat t, GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y, GLfloat z);
void (APIENTRY *gglTexCoord2fColor4ubVertex3fvSUN)(const GLfloat *tc, const GLubyte *c, const GLfloat *v);
void (APIENTRY *gglTexCoord2fColor3fVertex3fSUN)(GLfloat s, GLfloat t, GLfloat r, GLfloat g, GLfloat b, GLfloat x, GLfloat y, GLfloat z);
void (APIENTRY *gglTexCoord2fColor3fVertex3fvSUN)(const GLfloat *tc, const GLfloat *c, const GLfloat *v);
void (APIENTRY *gglTexCoord2fNormal3fVertex3fSUN)(GLfloat s, GLfloat t, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
void (APIENTRY *gglTexCoord2fNormal3fVertex3fvSUN)(const GLfloat *tc, const GLfloat *n, const GLfloat *v);
void (APIENTRY *gglTexCoord2fColor4fNormal3fVertex3fSUN)(GLfloat s, GLfloat t, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
void (APIENTRY *gglTexCoord2fColor4fNormal3fVertex3fvSUN)(const GLfloat *tc, const GLfloat *c, const GLfloat *n, const GLfloat *v);
void (APIENTRY *gglTexCoord4fColor4fNormal3fVertex4fSUN)(GLfloat s, GLfloat t, GLfloat p, GLfloat q, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void (APIENTRY *gglTexCoord4fColor4fNormal3fVertex4fvSUN)(const GLfloat *tc, const GLfloat *c, const GLfloat *n, const GLfloat *v);
void (APIENTRY *gglReplacementCodeuiVertex3fSUN)(GLuint rc, GLfloat x, GLfloat y, GLfloat z);
void (APIENTRY *gglReplacementCodeuiVertex3fvSUN)(const GLuint *rc, const GLfloat *v);
void (APIENTRY *gglReplacementCodeuiColor4ubVertex3fSUN)(GLuint rc, GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y, GLfloat z);
void (APIENTRY *gglReplacementCodeuiColor4ubVertex3fvSUN)(const GLuint *rc, const GLubyte *c, const GLfloat *v);
void (APIENTRY *gglReplacementCodeuiColor3fVertex3fSUN)(GLuint rc, GLfloat r, GLfloat g, GLfloat b, GLfloat x, GLfloat y, GLfloat z);
void (APIENTRY *gglReplacementCodeuiColor3fVertex3fvSUN)(const GLuint *rc, const GLfloat *c, const GLfloat *v);
void (APIENTRY *gglReplacementCodeuiNormal3fVertex3fSUN)(GLuint rc, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
void (APIENTRY *gglReplacementCodeuiNormal3fVertex3fvSUN)(const GLuint *rc, const GLfloat *n, const GLfloat *v);
void (APIENTRY *gglReplacementCodeuiColor4fNormal3fVertex3fSUN)(GLuint rc, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
void (APIENTRY *gglReplacementCodeuiColor4fNormal3fVertex3fvSUN)(const GLuint *rc, const GLfloat *c, const GLfloat *n, const GLfloat *v);
void (APIENTRY *gglReplacementCodeuiTexCoord2fVertex3fSUN)(GLuint rc, GLfloat s, GLfloat t, GLfloat x, GLfloat y, GLfloat z);
void (APIENTRY *gglReplacementCodeuiTexCoord2fVertex3fvSUN)(const GLuint *rc, const GLfloat *tc, const GLfloat *v);
void (APIENTRY *gglReplacementCodeuiTexCoord2fNormal3fVertex3fSUN)(GLuint rc, GLfloat s, GLfloat t, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
void (APIENTRY *gglReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN)(const GLuint *rc, const GLfloat *tc, const GLfloat *n, const GLfloat *v);
void (APIENTRY *gglReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN)(GLuint rc, GLfloat s, GLfloat t, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
void (APIENTRY *gglReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN)(const GLuint *rc, const GLfloat *tc, const GLfloat *c, const GLfloat *n, const GLfloat *v);

//GL_EXT_blend_func_separate
void (APIENTRY *gglBlendFuncSeparateEXT)(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);

//GL_EXT_vertex_weighting
void (APIENTRY *gglVertexWeightfEXT)(GLfloat weight);
void (APIENTRY *gglVertexWeightfvEXT)(const GLfloat *weight);
void (APIENTRY *gglVertexWeightPointerEXT)(GLsizei size, GLenum type, GLsizei stride, const GLvoid *pointer);

//GL_NV_vertex_array_range
void (APIENTRY *gglFlushVertexArrayRangeNV)(void);
void (APIENTRY *gglVertexArrayRangeNV)(GLsizei length, const GLvoid *pointer);

//GL_NV_register_combiners
void (APIENTRY *gglCombinerParameterfvNV)(GLenum pname, const GLfloat *params);
void (APIENTRY *gglCombinerParameterfNV)(GLenum pname, GLfloat param);
void (APIENTRY *gglCombinerParameterivNV)(GLenum pname, const GLint *params);
void (APIENTRY *gglCombinerParameteriNV)(GLenum pname, GLint param);
void (APIENTRY *gglCombinerInputNV)(GLenum stage, GLenum portion, GLenum variable, GLenum input, GLenum mapping, GLenum componentUsage);
void (APIENTRY *gglCombinerOutputNV)(GLenum stage, GLenum portion, GLenum abOutput, GLenum cdOutput, GLenum sumOutput, GLenum scale, GLenum bias, GLboolean abDotProduct, GLboolean cdDotProduct, GLboolean muxSum);
void (APIENTRY *gglFinalCombinerInputNV)(GLenum variable, GLenum input, GLenum mapping, GLenum componentUsage);
void (APIENTRY *gglGetCombinerInputParameterfvNV)(GLenum stage, GLenum portion, GLenum variable, GLenum pname, GLfloat *params);
void (APIENTRY *gglGetCombinerInputParameterivNV)(GLenum stage, GLenum portion, GLenum variable, GLenum pname, GLint *params);
void (APIENTRY *gglGetCombinerOutputParameterfvNV)(GLenum stage, GLenum portion, GLenum pname, GLfloat *params);
void (APIENTRY *gglGetCombinerOutputParameterivNV)(GLenum stage, GLenum portion, GLenum pname, GLint *params);
void (APIENTRY *gglGetFinalCombinerInputParameterfvNV)(GLenum variable, GLenum pname, GLfloat *params);
void (APIENTRY *gglGetFinalCombinerInputParameterivNV)(GLenum variable, GLenum pname, GLint *params);

//GL_MESA_resize_buffers
void (APIENTRY *gglResizeBuffersMESA)(void);

//GL_MESA_window_pos
void (APIENTRY *gglWindowPos2dMESA)(GLdouble x, GLdouble y);
void (APIENTRY *gglWindowPos2dvMESA)(const GLdouble *v);
void (APIENTRY *gglWindowPos2fMESA)(GLfloat x, GLfloat y);
void (APIENTRY *gglWindowPos2fvMESA)(const GLfloat *v);
void (APIENTRY *gglWindowPos2iMESA)(GLint x, GLint y);
void (APIENTRY *gglWindowPos2ivMESA)(const GLint *v);
void (APIENTRY *gglWindowPos2sMESA)(GLshort x, GLshort y);
void (APIENTRY *gglWindowPos2svMESA)(const GLshort *v);
void (APIENTRY *gglWindowPos3dMESA)(GLdouble x, GLdouble y, GLdouble z);
void (APIENTRY *gglWindowPos3dvMESA)(const GLdouble *v);
void (APIENTRY *gglWindowPos3fMESA)(GLfloat x, GLfloat y, GLfloat z);
void (APIENTRY *gglWindowPos3fvMESA)(const GLfloat *v);
void (APIENTRY *gglWindowPos3iMESA)(GLint x, GLint y, GLint z);
void (APIENTRY *gglWindowPos3ivMESA)(const GLint *v);
void (APIENTRY *gglWindowPos3sMESA)(GLshort x, GLshort y, GLshort z);
void (APIENTRY *gglWindowPos3svMESA)(const GLshort *v);
void (APIENTRY *gglWindowPos4dMESA)(GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void (APIENTRY *gglWindowPos4dvMESA)(const GLdouble *v);
void (APIENTRY *gglWindowPos4fMESA)(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void (APIENTRY *gglWindowPos4fvMESA)(const GLfloat *v);
void (APIENTRY *gglWindowPos4iMESA)(GLint x, GLint y, GLint z, GLint w);
void (APIENTRY *gglWindowPos4ivMESA)(const GLint *v);
void (APIENTRY *gglWindowPos4sMESA)(GLshort x, GLshort y, GLshort z, GLshort w);
void (APIENTRY *gglWindowPos4svMESA)(const GLshort *v);

//GL_IBM_multimode_draw_arrays
void (APIENTRY *gglMultiModeDrawArraysIBM)(GLenum mode, const GLint *first, const GLsizei *count, GLsizei primcount, GLint modestride);
void (APIENTRY *gglMultiModeDrawElementsIBM)(const GLenum *mode, const GLsizei *count, GLenum type, const GLvoid* *indices, GLsizei primcount, GLint modestride);

//GL_IBM_vertex_array_lists
void (APIENTRY *gglColorPointerListIBM)(GLint size, GLenum type, GLint stride, const GLvoid* *pointer, GLint ptrstride);
void (APIENTRY *gglSecondaryColorPointerListIBM)(GLint size, GLenum type, GLint stride, const GLvoid* *pointer, GLint ptrstride);
void (APIENTRY *gglEdgeFlagPointerListIBM)(GLint stride, const GLboolean* *pointer, GLint ptrstride);
void (APIENTRY *gglFogCoordPointerListIBM)(GLenum type, GLint stride, const GLvoid* *pointer, GLint ptrstride);
void (APIENTRY *gglIndexPointerListIBM)(GLenum type, GLint stride, const GLvoid* *pointer, GLint ptrstride);
void (APIENTRY *gglNormalPointerListIBM)(GLenum type, GLint stride, const GLvoid* *pointer, GLint ptrstride);
void (APIENTRY *gglTexCoordPointerListIBM)(GLint size, GLenum type, GLint stride, const GLvoid* *pointer, GLint ptrstride);
void (APIENTRY *gglVertexPointerListIBM)(GLint size, GLenum type, GLint stride, const GLvoid* *pointer, GLint ptrstride);

//GL_3DFX_tbuffer
void (APIENTRY *gglTbufferMask3DFX)(GLuint mask);

//GL_EXT_multisample
void (APIENTRY *gglSampleMaskEXT)(GLclampf value, GLboolean invert);
void (APIENTRY *gglSamplePatternEXT)(GLenum pattern);

//GL_SGIS_texture_color_mask
void (APIENTRY *gglTextureColorMaskSGIS)(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);

//GL_SGIX_igloo_interface
void (APIENTRY *gglIglooInterfaceSGIX)(GLenum pname, const GLvoid *params);

//GL_NV_fence
void (APIENTRY *gglDeleteFencesNV)(GLsizei n, const GLuint *fences);
void (APIENTRY *gglGenFencesNV)(GLsizei n, GLuint *fences);
GLboolean (APIENTRY *gglIsFenceNV)(GLuint fence);
GLboolean (APIENTRY *gglTestFenceNV)(GLuint fence);
void (APIENTRY *gglGetFenceivNV)(GLuint fence, GLenum pname, GLint *params);
void (APIENTRY *gglFinishFenceNV)(GLuint fence);
void (APIENTRY *gglSetFenceNV)(GLuint fence, GLenum condition);

//GL_IBM_static_data
void (APIENTRY *gglFlushStaticDataIBM)(GLenum target);

//GL_NV_evaluators
void (APIENTRY *gglMapControlPointsNV)(GLenum target, GLuint index, GLenum type, GLsizei ustride, GLsizei vstride, GLint uorder, GLint vorder, GLboolean packed, const GLvoid *points);
void (APIENTRY *gglMapParameterivNV)(GLenum target, GLenum pname, const GLint *params);
void (APIENTRY *gglMapParameterfvNV)(GLenum target, GLenum pname, const GLfloat *params);
void (APIENTRY *gglGetMapControlPointsNV)(GLenum target, GLuint index, GLenum type, GLsizei ustride, GLsizei vstride, GLboolean packed, GLvoid *points);
void (APIENTRY *gglGetMapParameterivNV)(GLenum target, GLenum pname, GLint *params);
void (APIENTRY *gglGetMapParameterfvNV)(GLenum target, GLenum pname, GLfloat *params);
void (APIENTRY *gglGetMapAttribParameterivNV)(GLenum target, GLuint index, GLenum pname, GLint *params);
void (APIENTRY *gglGetMapAttribParameterfvNV)(GLenum target, GLuint index, GLenum pname, GLfloat *params);
void (APIENTRY *gglEvalMapsNV)(GLenum target, GLenum mode);

//GL_NV_register_combiners2
void (APIENTRY *gglCombinerStageParameterfvNV)(GLenum stage, GLenum pname, const GLfloat *params);
void (APIENTRY *gglGetCombinerStageParameterfvNV)(GLenum stage, GLenum pname, GLfloat *params);

//GL_NV_vertex_program
GLboolean (APIENTRY *gglAreProgramsResidentNV)(GLsizei n, const GLuint *programs, GLboolean *residences);
void (APIENTRY *gglBindProgramNV)(GLenum target, GLuint id);
void (APIENTRY *gglDeleteProgramsNV)(GLsizei n, const GLuint *programs);
void (APIENTRY *gglExecuteProgramNV)(GLenum target, GLuint id, const GLfloat *params);
void (APIENTRY *gglGenProgramsNV)(GLsizei n, GLuint *programs);
void (APIENTRY *gglGetProgramParameterdvNV)(GLenum target, GLuint index, GLenum pname, GLdouble *params);
void (APIENTRY *gglGetProgramParameterfvNV)(GLenum target, GLuint index, GLenum pname, GLfloat *params);
void (APIENTRY *gglGetProgramivNV)(GLuint id, GLenum pname, GLint *params);
void (APIENTRY *gglGetProgramStringNV)(GLuint id, GLenum pname, GLubyte *program);
void (APIENTRY *gglGetTrackMatrixivNV)(GLenum target, GLuint address, GLenum pname, GLint *params);
void (APIENTRY *gglGetVertexAttribdvNV)(GLuint index, GLenum pname, GLdouble *params);
void (APIENTRY *gglGetVertexAttribfvNV)(GLuint index, GLenum pname, GLfloat *params);
void (APIENTRY *gglGetVertexAttribivNV)(GLuint index, GLenum pname, GLint *params);
void (APIENTRY *gglGetVertexAttribPointervNV)(GLuint index, GLenum pname, GLvoid* *pointer);
GLboolean (APIENTRY *gglIsProgramNV)(GLuint id);
void (APIENTRY *gglLoadProgramNV)(GLenum target, GLuint id, GLsizei len, const GLubyte *program);
void (APIENTRY *gglProgramParameter4dNV)(GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void (APIENTRY *gglProgramParameter4dvNV)(GLenum target, GLuint index, const GLdouble *v);
void (APIENTRY *gglProgramParameter4fNV)(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void (APIENTRY *gglProgramParameter4fvNV)(GLenum target, GLuint index, const GLfloat *v);
void (APIENTRY *gglProgramParameters4dvNV)(GLenum target, GLuint index, GLuint count, const GLdouble *v);
void (APIENTRY *gglProgramParameters4fvNV)(GLenum target, GLuint index, GLuint count, const GLfloat *v);
void (APIENTRY *gglRequestResidentProgramsNV)(GLsizei n, const GLuint *programs);
void (APIENTRY *gglTrackMatrixNV)(GLenum target, GLuint address, GLenum matrix, GLenum transform);
void (APIENTRY *gglVertexAttribPointerNV)(GLuint index, GLint fsize, GLenum type, GLsizei stride, const GLvoid *pointer);
void (APIENTRY *gglVertexAttrib1dNV)(GLuint index, GLdouble x);
void (APIENTRY *gglVertexAttrib1dvNV)(GLuint index, const GLdouble *v);
void (APIENTRY *gglVertexAttrib1fNV)(GLuint index, GLfloat x);
void (APIENTRY *gglVertexAttrib1fvNV)(GLuint index, const GLfloat *v);
void (APIENTRY *gglVertexAttrib1sNV)(GLuint index, GLshort x);
void (APIENTRY *gglVertexAttrib1svNV)(GLuint index, const GLshort *v);
void (APIENTRY *gglVertexAttrib2dNV)(GLuint index, GLdouble x, GLdouble y);
void (APIENTRY *gglVertexAttrib2dvNV)(GLuint index, const GLdouble *v);
void (APIENTRY *gglVertexAttrib2fNV)(GLuint index, GLfloat x, GLfloat y);
void (APIENTRY *gglVertexAttrib2fvNV)(GLuint index, const GLfloat *v);
void (APIENTRY *gglVertexAttrib2sNV)(GLuint index, GLshort x, GLshort y);
void (APIENTRY *gglVertexAttrib2svNV)(GLuint index, const GLshort *v);
void (APIENTRY *gglVertexAttrib3dNV)(GLuint index, GLdouble x, GLdouble y, GLdouble z);
void (APIENTRY *gglVertexAttrib3dvNV)(GLuint index, const GLdouble *v);
void (APIENTRY *gglVertexAttrib3fNV)(GLuint index, GLfloat x, GLfloat y, GLfloat z);
void (APIENTRY *gglVertexAttrib3fvNV)(GLuint index, const GLfloat *v);
void (APIENTRY *gglVertexAttrib3sNV)(GLuint index, GLshort x, GLshort y, GLshort z);
void (APIENTRY *gglVertexAttrib3svNV)(GLuint index, const GLshort *v);
void (APIENTRY *gglVertexAttrib4dNV)(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void (APIENTRY *gglVertexAttrib4dvNV)(GLuint index, const GLdouble *v);
void (APIENTRY *gglVertexAttrib4fNV)(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void (APIENTRY *gglVertexAttrib4fvNV)(GLuint index, const GLfloat *v);
void (APIENTRY *gglVertexAttrib4sNV)(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
void (APIENTRY *gglVertexAttrib4svNV)(GLuint index, const GLshort *v);
void (APIENTRY *gglVertexAttrib4ubNV)(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
void (APIENTRY *gglVertexAttrib4ubvNV)(GLuint index, const GLubyte *v);
void (APIENTRY *gglVertexAttribs1dvNV)(GLuint index, GLsizei count, const GLdouble *v);
void (APIENTRY *gglVertexAttribs1fvNV)(GLuint index, GLsizei count, const GLfloat *v);
void (APIENTRY *gglVertexAttribs1svNV)(GLuint index, GLsizei count, const GLshort *v);
void (APIENTRY *gglVertexAttribs2dvNV)(GLuint index, GLsizei count, const GLdouble *v);
void (APIENTRY *gglVertexAttribs2fvNV)(GLuint index, GLsizei count, const GLfloat *v);
void (APIENTRY *gglVertexAttribs2svNV)(GLuint index, GLsizei count, const GLshort *v);
void (APIENTRY *gglVertexAttribs3dvNV)(GLuint index, GLsizei count, const GLdouble *v);
void (APIENTRY *gglVertexAttribs3fvNV)(GLuint index, GLsizei count, const GLfloat *v);
void (APIENTRY *gglVertexAttribs3svNV)(GLuint index, GLsizei count, const GLshort *v);
void (APIENTRY *gglVertexAttribs4dvNV)(GLuint index, GLsizei count, const GLdouble *v);
void (APIENTRY *gglVertexAttribs4fvNV)(GLuint index, GLsizei count, const GLfloat *v);
void (APIENTRY *gglVertexAttribs4svNV)(GLuint index, GLsizei count, const GLshort *v);
void (APIENTRY *gglVertexAttribs4ubvNV)(GLuint index, GLsizei count, const GLubyte *v);

//GL_ATI_envmap_bumpmap
void (APIENTRY *gglTexBumpParameterivATI)(GLenum pname, const GLint *param);
void (APIENTRY *gglTexBumpParameterfvATI)(GLenum pname, const GLfloat *param);
void (APIENTRY *gglGetTexBumpParameterivATI)(GLenum pname, GLint *param);
void (APIENTRY *gglGetTexBumpParameterfvATI)(GLenum pname, GLfloat *param);

//GL_ATI_fragment_shader
GLuint (APIENTRY *gglGenFragmentShadersATI)(GLuint range);
void (APIENTRY *gglBindFragmentShaderATI)(GLuint id);
void (APIENTRY *gglDeleteFragmentShaderATI)(GLuint id);
void (APIENTRY *gglBeginFragmentShaderATI)(void);
void (APIENTRY *gglEndFragmentShaderATI)(void);
void (APIENTRY *gglPassTexCoordATI)(GLuint dst, GLuint coord, GLenum swizzle);
void (APIENTRY *gglSampleMapATI)(GLuint dst, GLuint interp, GLenum swizzle);
void (APIENTRY *gglColorFragmentOp1ATI)(GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod);
void (APIENTRY *gglColorFragmentOp2ATI)(GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod);
void (APIENTRY *gglColorFragmentOp3ATI)(GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod, GLuint arg3, GLuint arg3Rep, GLuint arg3Mod);
void (APIENTRY *gglAlphaFragmentOp1ATI)(GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod);
void (APIENTRY *gglAlphaFragmentOp2ATI)(GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod);
void (APIENTRY *gglAlphaFragmentOp3ATI)(GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod, GLuint arg3, GLuint arg3Rep, GLuint arg3Mod);
void (APIENTRY *gglSetFragmentShaderConstantATI)(GLuint dst, const GLfloat *value);

//GL_ATI_pn_triangles
void (APIENTRY *gglPNTrianglesiATI)(GLenum pname, GLint param);
void (APIENTRY *gglPNTrianglesfATI)(GLenum pname, GLfloat param);

//GL_ATI_vertex_array_object
GLuint (APIENTRY *gglNewObjectBufferATI)(GLsizei size, const GLvoid *pointer, GLenum usage);
GLboolean (APIENTRY *gglIsObjectBufferATI)(GLuint buffer);
void (APIENTRY *gglUpdateObjectBufferATI)(GLuint buffer, GLuint offset, GLsizei size, const GLvoid *pointer, GLenum preserve);
void (APIENTRY *gglGetObjectBufferfvATI)(GLuint buffer, GLenum pname, GLfloat *params);
void (APIENTRY *gglGetObjectBufferivATI)(GLuint buffer, GLenum pname, GLint *params);
void (APIENTRY *gglDeleteObjectBufferATI)(GLuint buffer);
void (APIENTRY *gglArrayObjectATI)(GLenum array, GLint size, GLenum type, GLsizei stride, GLuint buffer, GLuint offset);
void (APIENTRY *gglGetArrayObjectfvATI)(GLenum array, GLenum pname, GLfloat *params);
void (APIENTRY *gglGetArrayObjectivATI)(GLenum array, GLenum pname, GLint *params);
void (APIENTRY *gglVariantArrayObjectATI)(GLuint id, GLenum type, GLsizei stride, GLuint buffer, GLuint offset);
void (APIENTRY *gglGetVariantArrayObjectfvATI)(GLuint id, GLenum pname, GLfloat *params);
void (APIENTRY *gglGetVariantArrayObjectivATI)(GLuint id, GLenum pname, GLint *params);

//GL_EXT_vertex_shader
void (APIENTRY *gglBeginVertexShaderEXT)(void);
void (APIENTRY *gglEndVertexShaderEXT)(void);
void (APIENTRY *gglBindVertexShaderEXT)(GLuint id);
GLuint (APIENTRY *gglGenVertexShadersEXT)(GLuint range);
void (APIENTRY *gglDeleteVertexShaderEXT)(GLuint id);
void (APIENTRY *gglShaderOp1EXT)(GLenum op, GLuint res, GLuint arg1);
void (APIENTRY *gglShaderOp2EXT)(GLenum op, GLuint res, GLuint arg1, GLuint arg2);
void (APIENTRY *gglShaderOp3EXT)(GLenum op, GLuint res, GLuint arg1, GLuint arg2, GLuint arg3);
void (APIENTRY *gglSwizzleEXT)(GLuint res, GLuint in, GLenum outX, GLenum outY, GLenum outZ, GLenum outW);
void (APIENTRY *gglWriteMaskEXT)(GLuint res, GLuint in, GLenum outX, GLenum outY, GLenum outZ, GLenum outW);
void (APIENTRY *gglInsertComponentEXT)(GLuint res, GLuint src, GLuint num);
void (APIENTRY *gglExtractComponentEXT)(GLuint res, GLuint src, GLuint num);
GLuint (APIENTRY *gglGenSymbolsEXT)(GLenum datatype, GLenum storagetype, GLenum range, GLuint components);
void (APIENTRY *gglSetInvariantEXT)(GLuint id, GLenum type, const void *addr);
void (APIENTRY *gglSetLocalConstantEXT)(GLuint id, GLenum type, const void *addr);
void (APIENTRY *gglVariantbvEXT)(GLuint id, const GLbyte *addr);
void (APIENTRY *gglVariantsvEXT)(GLuint id, const GLshort *addr);
void (APIENTRY *gglVariantivEXT)(GLuint id, const GLint *addr);
void (APIENTRY *gglVariantfvEXT)(GLuint id, const GLfloat *addr);
void (APIENTRY *gglVariantdvEXT)(GLuint id, const GLdouble *addr);
void (APIENTRY *gglVariantubvEXT)(GLuint id, const GLubyte *addr);
void (APIENTRY *gglVariantusvEXT)(GLuint id, const GLushort *addr);
void (APIENTRY *gglVariantuivEXT)(GLuint id, const GLuint *addr);
void (APIENTRY *gglVariantPointerEXT)(GLuint id, GLenum type, GLuint stride, const void *addr);
void (APIENTRY *gglEnableVariantClientStateEXT)(GLuint id);
void (APIENTRY *gglDisableVariantClientStateEXT)(GLuint id);
GLuint (APIENTRY *gglBindLightParameterEXT)(GLenum light, GLenum value);
GLuint (APIENTRY *gglBindMaterialParameterEXT)(GLenum face, GLenum value);
GLuint (APIENTRY *gglBindTexGenParameterEXT)(GLenum unit, GLenum coord, GLenum value);
GLuint (APIENTRY *gglBindTextureUnitParameterEXT)(GLenum unit, GLenum value);
GLuint (APIENTRY *gglBindParameterEXT)(GLenum value);
GLboolean (APIENTRY *gglIsVariantEnabledEXT)(GLuint id, GLenum cap);
void (APIENTRY *gglGetVariantBooleanvEXT)(GLuint id, GLenum value, GLboolean *data);
void (APIENTRY *gglGetVariantIntegervEXT)(GLuint id, GLenum value, GLint *data);
void (APIENTRY *gglGetVariantFloatvEXT)(GLuint id, GLenum value, GLfloat *data);
void (APIENTRY *gglGetVariantPointervEXT)(GLuint id, GLenum value, GLvoid* *data);
void (APIENTRY *gglGetInvariantBooleanvEXT)(GLuint id, GLenum value, GLboolean *data);
void (APIENTRY *gglGetInvariantIntegervEXT)(GLuint id, GLenum value, GLint *data);
void (APIENTRY *gglGetInvariantFloatvEXT)(GLuint id, GLenum value, GLfloat *data);
void (APIENTRY *gglGetLocalConstantBooleanvEXT)(GLuint id, GLenum value, GLboolean *data);
void (APIENTRY *gglGetLocalConstantIntegervEXT)(GLuint id, GLenum value, GLint *data);
void (APIENTRY *gglGetLocalConstantFloatvEXT)(GLuint id, GLenum value, GLfloat *data);

//GL_ATI_vertex_streams
void (APIENTRY *gglVertexStream1sATI)(GLenum stream, GLshort x);
void (APIENTRY *gglVertexStream1svATI)(GLenum stream, const GLshort *coords);
void (APIENTRY *gglVertexStream1iATI)(GLenum stream, GLint x);
void (APIENTRY *gglVertexStream1ivATI)(GLenum stream, const GLint *coords);
void (APIENTRY *gglVertexStream1fATI)(GLenum stream, GLfloat x);
void (APIENTRY *gglVertexStream1fvATI)(GLenum stream, const GLfloat *coords);
void (APIENTRY *gglVertexStream1dATI)(GLenum stream, GLdouble x);
void (APIENTRY *gglVertexStream1dvATI)(GLenum stream, const GLdouble *coords);
void (APIENTRY *gglVertexStream2sATI)(GLenum stream, GLshort x, GLshort y);
void (APIENTRY *gglVertexStream2svATI)(GLenum stream, const GLshort *coords);
void (APIENTRY *gglVertexStream2iATI)(GLenum stream, GLint x, GLint y);
void (APIENTRY *gglVertexStream2ivATI)(GLenum stream, const GLint *coords);
void (APIENTRY *gglVertexStream2fATI)(GLenum stream, GLfloat x, GLfloat y);
void (APIENTRY *gglVertexStream2fvATI)(GLenum stream, const GLfloat *coords);
void (APIENTRY *gglVertexStream2dATI)(GLenum stream, GLdouble x, GLdouble y);
void (APIENTRY *gglVertexStream2dvATI)(GLenum stream, const GLdouble *coords);
void (APIENTRY *gglVertexStream3sATI)(GLenum stream, GLshort x, GLshort y, GLshort z);
void (APIENTRY *gglVertexStream3svATI)(GLenum stream, const GLshort *coords);
void (APIENTRY *gglVertexStream3iATI)(GLenum stream, GLint x, GLint y, GLint z);
void (APIENTRY *gglVertexStream3ivATI)(GLenum stream, const GLint *coords);
void (APIENTRY *gglVertexStream3fATI)(GLenum stream, GLfloat x, GLfloat y, GLfloat z);
void (APIENTRY *gglVertexStream3fvATI)(GLenum stream, const GLfloat *coords);
void (APIENTRY *gglVertexStream3dATI)(GLenum stream, GLdouble x, GLdouble y, GLdouble z);
void (APIENTRY *gglVertexStream3dvATI)(GLenum stream, const GLdouble *coords);
void (APIENTRY *gglVertexStream4sATI)(GLenum stream, GLshort x, GLshort y, GLshort z, GLshort w);
void (APIENTRY *gglVertexStream4svATI)(GLenum stream, const GLshort *coords);
void (APIENTRY *gglVertexStream4iATI)(GLenum stream, GLint x, GLint y, GLint z, GLint w);
void (APIENTRY *gglVertexStream4ivATI)(GLenum stream, const GLint *coords);
void (APIENTRY *gglVertexStream4fATI)(GLenum stream, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void (APIENTRY *gglVertexStream4fvATI)(GLenum stream, const GLfloat *coords);
void (APIENTRY *gglVertexStream4dATI)(GLenum stream, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void (APIENTRY *gglVertexStream4dvATI)(GLenum stream, const GLdouble *coords);
void (APIENTRY *gglNormalStream3bATI)(GLenum stream, GLbyte nx, GLbyte ny, GLbyte nz);
void (APIENTRY *gglNormalStream3bvATI)(GLenum stream, const GLbyte *coords);
void (APIENTRY *gglNormalStream3sATI)(GLenum stream, GLshort nx, GLshort ny, GLshort nz);
void (APIENTRY *gglNormalStream3svATI)(GLenum stream, const GLshort *coords);
void (APIENTRY *gglNormalStream3iATI)(GLenum stream, GLint nx, GLint ny, GLint nz);
void (APIENTRY *gglNormalStream3ivATI)(GLenum stream, const GLint *coords);
void (APIENTRY *gglNormalStream3fATI)(GLenum stream, GLfloat nx, GLfloat ny, GLfloat nz);
void (APIENTRY *gglNormalStream3fvATI)(GLenum stream, const GLfloat *coords);
void (APIENTRY *gglNormalStream3dATI)(GLenum stream, GLdouble nx, GLdouble ny, GLdouble nz);
void (APIENTRY *gglNormalStream3dvATI)(GLenum stream, const GLdouble *coords);
void (APIENTRY *gglClientActiveVertexStreamATI)(GLenum stream);
void (APIENTRY *gglVertexBlendEnviATI)(GLenum pname, GLint param);
void (APIENTRY *gglVertexBlendEnvfATI)(GLenum pname, GLfloat param);

//GL_ATI_element_array
void (APIENTRY *gglElementPointerATI)(GLenum type, const GLvoid *pointer);
void (APIENTRY *gglDrawElementArrayATI)(GLenum mode, GLsizei count);
void (APIENTRY *gglDrawRangeElementArrayATI)(GLenum mode, GLuint start, GLuint end, GLsizei count);

//GL_SUN_mesh_array
void (APIENTRY *gglDrawMeshArraysSUN)(GLenum mode, GLint first, GLsizei count, GLsizei width);

//GL_NV_occlusion_query
void (APIENTRY *gglGenOcclusionQueriesNV)(GLsizei n, GLuint *ids);
void (APIENTRY *gglDeleteOcclusionQueriesNV)(GLsizei n, const GLuint *ids);
GLboolean (APIENTRY *gglIsOcclusionQueryNV)(GLuint id);
void (APIENTRY *gglBeginOcclusionQueryNV)(GLuint id);
void (APIENTRY *gglEndOcclusionQueryNV)(void);
void (APIENTRY *gglGetOcclusionQueryivNV)(GLuint id, GLenum pname, GLint *params);
void (APIENTRY *gglGetOcclusionQueryuivNV)(GLuint id, GLenum pname, GLuint *params);

//GL_NV_point_sprite
void (APIENTRY *gglPointParameteriNV)(GLenum pname, GLint param);
void (APIENTRY *gglPointParameterivNV)(GLenum pname, const GLint *params);

//GL_EXT_stencil_two_side
void (APIENTRY *gglActiveStencilFaceEXT)(GLenum face);

//GL_APPLE_element_array
void (APIENTRY *gglElementPointerAPPLE)(GLenum type, const GLvoid *pointer);
void (APIENTRY *gglDrawElementArrayAPPLE)(GLenum mode, GLint first, GLsizei count);
void (APIENTRY *gglDrawRangeElementArrayAPPLE)(GLenum mode, GLuint start, GLuint end, GLint first, GLsizei count);
void (APIENTRY *gglMultiDrawElementArrayAPPLE)(GLenum mode, const GLint *first, const GLsizei *count, GLsizei primcount);
void (APIENTRY *gglMultiDrawRangeElementArrayAPPLE)(GLenum mode, GLuint start, GLuint end, const GLint *first, const GLsizei *count, GLsizei primcount);

//GL_APPLE_fence
void (APIENTRY *gglGenFencesAPPLE)(GLsizei n, GLuint *fences);
void (APIENTRY *gglDeleteFencesAPPLE)(GLsizei n, const GLuint *fences);
void (APIENTRY *gglSetFenceAPPLE)(GLuint fence);
GLboolean (APIENTRY *gglIsFenceAPPLE)(GLuint fence);
GLboolean (APIENTRY *gglTestFenceAPPLE)(GLuint fence);
void (APIENTRY *gglFinishFenceAPPLE)(GLuint fence);
GLboolean (APIENTRY *gglTestObjectAPPLE)(GLenum object, GLuint name);
void (APIENTRY *gglFinishObjectAPPLE)(GLenum object, GLint name);

//GL_APPLE_vertex_array_object
void (APIENTRY *gglBindVertexArrayAPPLE)(GLuint array);
void (APIENTRY *gglDeleteVertexArraysAPPLE)(GLsizei n, const GLuint *arrays);
void (APIENTRY *gglGenVertexArraysAPPLE)(GLsizei n, const GLuint *arrays);
GLboolean (APIENTRY *gglIsVertexArrayAPPLE)(GLuint array);

//GL_APPLE_vertex_array_range
void (APIENTRY *gglVertexArrayRangeAPPLE)(GLsizei length, GLvoid *pointer);
void (APIENTRY *gglFlushVertexArrayRangeAPPLE)(GLsizei length, GLvoid *pointer);
void (APIENTRY *gglVertexArrayParameteriAPPLE)(GLenum pname, GLint param);

//GL_ATI_draw_buffers
void (APIENTRY *gglDrawBuffersATI)(GLsizei n, const GLenum *bufs);

//GL_NV_fragment_program
void (APIENTRY *gglProgramNamedParameter4fNV)(GLuint id, GLsizei len, const GLubyte *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void (APIENTRY *gglProgramNamedParameter4dNV)(GLuint id, GLsizei len, const GLubyte *name, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void (APIENTRY *gglProgramNamedParameter4fvNV)(GLuint id, GLsizei len, const GLubyte *name, const GLfloat *v);
void (APIENTRY *gglProgramNamedParameter4dvNV)(GLuint id, GLsizei len, const GLubyte *name, const GLdouble *v);
void (APIENTRY *gglGetProgramNamedParameterfvNV)(GLuint id, GLsizei len, const GLubyte *name, GLfloat *params);
void (APIENTRY *gglGetProgramNamedParameterdvNV)(GLuint id, GLsizei len, const GLubyte *name, GLdouble *params);

//GL_NV_half_float
void (APIENTRY *gglVertex2hNV)(GLhalfNV x, GLhalfNV y);
void (APIENTRY *gglVertex2hvNV)(const GLhalfNV *v);
void (APIENTRY *gglVertex3hNV)(GLhalfNV x, GLhalfNV y, GLhalfNV z);
void (APIENTRY *gglVertex3hvNV)(const GLhalfNV *v);
void (APIENTRY *gglVertex4hNV)(GLhalfNV x, GLhalfNV y, GLhalfNV z, GLhalfNV w);
void (APIENTRY *gglVertex4hvNV)(const GLhalfNV *v);
void (APIENTRY *gglNormal3hNV)(GLhalfNV nx, GLhalfNV ny, GLhalfNV nz);
void (APIENTRY *gglNormal3hvNV)(const GLhalfNV *v);
void (APIENTRY *gglColor3hNV)(GLhalfNV red, GLhalfNV green, GLhalfNV blue);
void (APIENTRY *gglColor3hvNV)(const GLhalfNV *v);
void (APIENTRY *gglColor4hNV)(GLhalfNV red, GLhalfNV green, GLhalfNV blue, GLhalfNV alpha);
void (APIENTRY *gglColor4hvNV)(const GLhalfNV *v);
void (APIENTRY *gglTexCoord1hNV)(GLhalfNV s);
void (APIENTRY *gglTexCoord1hvNV)(const GLhalfNV *v);
void (APIENTRY *gglTexCoord2hNV)(GLhalfNV s, GLhalfNV t);
void (APIENTRY *gglTexCoord2hvNV)(const GLhalfNV *v);
void (APIENTRY *gglTexCoord3hNV)(GLhalfNV s, GLhalfNV t, GLhalfNV r);
void (APIENTRY *gglTexCoord3hvNV)(const GLhalfNV *v);
void (APIENTRY *gglTexCoord4hNV)(GLhalfNV s, GLhalfNV t, GLhalfNV r, GLhalfNV q);
void (APIENTRY *gglTexCoord4hvNV)(const GLhalfNV *v);
void (APIENTRY *gglMultiTexCoord1hNV)(GLenum target, GLhalfNV s);
void (APIENTRY *gglMultiTexCoord1hvNV)(GLenum target, const GLhalfNV *v);
void (APIENTRY *gglMultiTexCoord2hNV)(GLenum target, GLhalfNV s, GLhalfNV t);
void (APIENTRY *gglMultiTexCoord2hvNV)(GLenum target, const GLhalfNV *v);
void (APIENTRY *gglMultiTexCoord3hNV)(GLenum target, GLhalfNV s, GLhalfNV t, GLhalfNV r);
void (APIENTRY *gglMultiTexCoord3hvNV)(GLenum target, const GLhalfNV *v);
void (APIENTRY *gglMultiTexCoord4hNV)(GLenum target, GLhalfNV s, GLhalfNV t, GLhalfNV r, GLhalfNV q);
void (APIENTRY *gglMultiTexCoord4hvNV)(GLenum target, const GLhalfNV *v);
void (APIENTRY *gglFogCoordhNV)(GLhalfNV fog);
void (APIENTRY *gglFogCoordhvNV)(const GLhalfNV *fog);
void (APIENTRY *gglSecondaryColor3hNV)(GLhalfNV red, GLhalfNV green, GLhalfNV blue);
void (APIENTRY *gglSecondaryColor3hvNV)(const GLhalfNV *v);
void (APIENTRY *gglVertexWeighthNV)(GLhalfNV weight);
void (APIENTRY *gglVertexWeighthvNV)(const GLhalfNV *weight);
void (APIENTRY *gglVertexAttrib1hNV)(GLuint index, GLhalfNV x);
void (APIENTRY *gglVertexAttrib1hvNV)(GLuint index, const GLhalfNV *v);
void (APIENTRY *gglVertexAttrib2hNV)(GLuint index, GLhalfNV x, GLhalfNV y);
void (APIENTRY *gglVertexAttrib2hvNV)(GLuint index, const GLhalfNV *v);
void (APIENTRY *gglVertexAttrib3hNV)(GLuint index, GLhalfNV x, GLhalfNV y, GLhalfNV z);
void (APIENTRY *gglVertexAttrib3hvNV)(GLuint index, const GLhalfNV *v);
void (APIENTRY *gglVertexAttrib4hNV)(GLuint index, GLhalfNV x, GLhalfNV y, GLhalfNV z, GLhalfNV w);
void (APIENTRY *gglVertexAttrib4hvNV)(GLuint index, const GLhalfNV *v);
void (APIENTRY *gglVertexAttribs1hvNV)(GLuint index, GLsizei n, const GLhalfNV *v);
void (APIENTRY *gglVertexAttribs2hvNV)(GLuint index, GLsizei n, const GLhalfNV *v);
void (APIENTRY *gglVertexAttribs3hvNV)(GLuint index, GLsizei n, const GLhalfNV *v);
void (APIENTRY *gglVertexAttribs4hvNV)(GLuint index, GLsizei n, const GLhalfNV *v);

//GL_NV_pixel_data_range
void (APIENTRY *gglPixelDataRangeNV)(GLenum target, GLsizei length, GLvoid *pointer);
void (APIENTRY *gglFlushPixelDataRangeNV)(GLenum target);

//GL_NV_primitive_restart
void (APIENTRY *gglPrimitiveRestartNV)(void);
void (APIENTRY *gglPrimitiveRestartIndexNV)(GLuint index);

//GL_ATI_map_object_buffer
GLvoid* (APIENTRY *gglMapObjectBufferATI)(GLuint buffer);
void (APIENTRY *gglUnmapObjectBufferATI)(GLuint buffer);

//GL_ATI_separate_stencil
void (APIENTRY *gglStencilOpSeparateATI)(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
void (APIENTRY *gglStencilFuncSeparateATI)(GLenum frontfunc, GLenum backfunc, GLint ref, GLuint mask);

//GL_ATI_vertex_attrib_array_object
void (APIENTRY *gglVertexAttribArrayObjectATI)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLuint buffer, GLuint offset);
void (APIENTRY *gglGetVertexAttribArrayObjectfvATI)(GLuint index, GLenum pname, GLfloat *params);
void (APIENTRY *gglGetVertexAttribArrayObjectivATI)(GLuint index, GLenum pname, GLint *params);

//GL_OES_fixed_point
void (APIENTRY *gglAlphaFuncxOES)(GLenum func, GLclampx ref);
void (APIENTRY *gglClearColorxOES)(GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha);
void (APIENTRY *gglClearDepthxOES)(GLclampx depth);
void (APIENTRY *gglClipPlanexOES)(GLenum plane, const GLfixed *equation);
void (APIENTRY *gglColor4xOES)(GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha);
void (APIENTRY *gglDepthRangexOES)(GLclampx zNear, GLclampx zFar);
void (APIENTRY *gglFogxOES)(GLenum pname, GLfixed param);
void (APIENTRY *gglFogxvOES)(GLenum pname, const GLfixed *params);
void (APIENTRY *gglFrustumxOES)(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar);
void (APIENTRY *gglGetClipPlanexOES)(GLenum pname, GLfixed eqn[4]);
void (APIENTRY *gglGetFixedvOES)(GLenum pname, GLfixed *params);
void (APIENTRY *gglGetLightxvOES)(GLenum light, GLenum pname, GLfixed *params);
void (APIENTRY *gglGetMaterialxvOES)(GLenum face, GLenum pname, GLfixed *params);
void (APIENTRY *gglGetTexEnvxvOES)(GLenum env, GLenum pname, GLfixed *params);
void (APIENTRY *gglGetTexParameterxvOES)(GLenum target, GLenum pname, GLfixed *params);
void (APIENTRY *gglLightModelxOES)(GLenum pname, GLfixed param);
void (APIENTRY *gglLightModelxvOES)(GLenum pname, const GLfixed *params);
void (APIENTRY *gglLightxOES)(GLenum light, GLenum pname, GLfixed param);
void (APIENTRY *gglLightxvOES)(GLenum light, GLenum pname, const GLfixed *params);
void (APIENTRY *gglLineWidthxOES)(GLfixed width);
void (APIENTRY *gglLoadMatrixxOES)(const GLfixed *m);
void (APIENTRY *gglMaterialxOES)(GLenum face, GLenum pname, GLfixed param);
void (APIENTRY *gglMaterialxvOES)(GLenum face, GLenum pname, const GLfixed *params);
void (APIENTRY *gglMultMatrixxOES)(const GLfixed *m);
void (APIENTRY *gglMultiTexCoord4xOES)(GLenum target, GLfixed s, GLfixed t, GLfixed r, GLfixed q);
void (APIENTRY *gglNormal3xOES)(GLfixed nx, GLfixed ny, GLfixed nz);
void (APIENTRY *gglOrthoxOES)(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar);
void (APIENTRY *gglPointParameterxOES)(GLenum pname, GLfixed param);
void (APIENTRY *gglPointParameterxvOES)(GLenum pname, const GLfixed *params);
void (APIENTRY *gglPointSizexOES)(GLfixed size);
void (APIENTRY *gglPolygonOffsetxOES)(GLfixed factor, GLfixed units);
void (APIENTRY *gglRotatexOES)(GLfixed angle, GLfixed x, GLfixed y, GLfixed z);
void (APIENTRY *gglSampleCoveragexOES)(GLclampx value, GLboolean invert);
void (APIENTRY *gglScalexOES)(GLfixed x, GLfixed y, GLfixed z);
void (APIENTRY *gglTexEnvxOES)(GLenum target, GLenum pname, GLfixed param);
void (APIENTRY *gglTexEnvxvOES)(GLenum target, GLenum pname, const GLfixed *params);
void (APIENTRY *gglTexParameterxOES)(GLenum target, GLenum pname, GLfixed param);
void (APIENTRY *gglTexParameterxvOES)(GLenum target, GLenum pname, const GLfixed *params);
void (APIENTRY *gglTranslatexOES)(GLfixed x, GLfixed y, GLfixed z);

//GL_OES_single_precision
void (APIENTRY *gglDepthRangefOES)(GLclampf zNear, GLclampf zFar);
void (APIENTRY *gglFrustumfOES)(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);
void (APIENTRY *gglOrthofOES)(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);
void (APIENTRY *gglClipPlanefOES)(GLenum plane, const GLfloat *equation);
void (APIENTRY *gglGetClipPlanefOES)(GLenum pname, GLfloat eqn[4]);
void (APIENTRY *gglClearDepthfOES)(GLclampf depth);

//GL_OES_query_matrix
GLbitfield (APIENTRY *gglQueryMatrixxOES)(GLfixed mantissa[16], GLint exponent[16]);

//GL_EXT_depth_bounds_test
void (APIENTRY *gglDepthBoundsEXT)(GLclampd zmin, GLclampd zmax);

//GL_EXT_blend_equation_separate
void (APIENTRY *gglBlendEquationSeparateEXT)(GLenum modeRGB, GLenum modeAlpha);

//GL_EXT_framebuffer_object
GLboolean (APIENTRY *gglIsRenderbufferEXT)(GLuint renderbuffer);
void (APIENTRY *gglBindRenderbufferEXT)(GLenum target, GLuint renderbuffer);
void (APIENTRY *gglDeleteRenderbuffersEXT)(GLsizei n, const GLuint *renderbuffers);
void (APIENTRY *gglGenRenderbuffersEXT)(GLsizei n, GLuint *renderbuffers);
void (APIENTRY *gglRenderbufferStorageEXT)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
void (APIENTRY *gglGetRenderbufferParameterivEXT)(GLenum target, GLenum pname, GLint *params);
GLboolean (APIENTRY *gglIsFramebufferEXT)(GLuint framebuffer);
void (APIENTRY *gglBindFramebufferEXT)(GLenum target, GLuint framebuffer);
void (APIENTRY *gglDeleteFramebuffersEXT)(GLsizei n, const GLuint *framebuffers);
void (APIENTRY *gglGenFramebuffersEXT)(GLsizei n, GLuint *framebuffers);
GLenum (APIENTRY *gglCheckFramebufferStatusEXT)(GLenum target);
void (APIENTRY *gglFramebufferTexture1DEXT)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
void (APIENTRY *gglFramebufferTexture2DEXT)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
void (APIENTRY *gglFramebufferTexture3DEXT)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
void (APIENTRY *gglFramebufferRenderbufferEXT)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
void (APIENTRY *gglGetFramebufferAttachmentParameterivEXT)(GLenum target, GLenum attachment, GLenum pname, GLint *params);
void (APIENTRY *gglGenerateMipmapEXT)(GLenum target);

//GL_GREMEDY_string_marker
void (APIENTRY *gglStringMarkerGREMEDY)(GLsizei len, const GLvoid *string);

//GL_EXT_stencil_clear_tag
void (APIENTRY *gglStencilClearTagEXT)(GLsizei stencilTagBits, GLuint stencilClearTag);

//GL_EXT_framebuffer_blit
void (APIENTRY *gglBlitFramebufferEXT)(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);

//GL_EXT_framebuffer_multisample
void (APIENTRY *gglRenderbufferStorageMultisampleEXT)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);

//GL_KTX_buffer_region
GLuint (APIENTRY *gglBufferRegionEnabledEXT)(void);
void (APIENTRY *gglDeleteBufferRegionEXT)(GLenum region);
void (APIENTRY *gglDrawBufferRegionEXT)(GLuint region, GLint x, GLint y, GLsizei width, GLsizei height, GLint xDest, GLint yDest);
GLuint (APIENTRY *gglNewBufferRegionEXT)(GLenum region);
void (APIENTRY *gglReadBufferRegionEXT)(GLuint region, GLint x, GLint y, GLsizei width, GLsizei height);

//GL_SUN_read_video_pixels
void (APIENTRY *gglReadVideoPixelsSUN)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid* pixels);

//GL_WIN_swap_hint
void (APIENTRY *gglAddSwapHintRectWIN)(GLint x, GLint y, GLsizei width, GLsizei height);

//=====================================
//
// OPENGL FUNCTIONS
//
//=====================================

//GL_VERSION_1_1
GLboolean (APIENTRY *gglAreTexturesResident)(GLsizei n, const GLuint *textures, GLboolean *residences);
void (APIENTRY *gglArrayElement)(GLint i);
void (APIENTRY *gglBindTexture)(GLenum target, GLuint texture);
void (APIENTRY *gglColorPointer)(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
void (APIENTRY *gglCopyTexImage1D)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
void (APIENTRY *gglCopyTexImage2D)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
void (APIENTRY *gglCopyTexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
void (APIENTRY *gglCopyTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
void (APIENTRY *gglDeleteTextures)(GLsizei n, const GLuint *textures);
void (APIENTRY *gglDisableClientState)(GLenum array);
void (APIENTRY *gglDrawArrays)(GLenum mode, GLint first, GLsizei count);
void (APIENTRY *gglDrawElements)(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
void (APIENTRY *gglEdgeFlagPointer)(GLsizei stride, const GLboolean *pointer);
void (APIENTRY *gglEnableClientState)(GLenum array);
void (APIENTRY *gglGenTextures)(GLsizei n, GLuint *textures);
void (APIENTRY *gglGetPointerv)(GLenum pname, GLvoid* *params);
void (APIENTRY *gglIndexub)(GLubyte c);
void (APIENTRY *gglIndexubv)(const GLubyte *c);
void (APIENTRY *gglIndexPointer)(GLenum type, GLsizei stride, const GLvoid *pointer);
void (APIENTRY *gglInterleavedArrays)(GLenum format, GLsizei stride, const GLvoid *pointer);
GLboolean (APIENTRY *gglIsTexture)(GLuint texture);
void (APIENTRY *gglNormalPointer)(GLenum type, GLsizei stride, const GLvoid *pointer);
void (APIENTRY *gglPolygonOffset)(GLfloat factor, GLfloat units);
void (APIENTRY *gglPopClientAttrib)(void);
void (APIENTRY *gglPrioritizeTextures)(GLsizei n, const GLuint *textures, const GLclampf *priorities);
void (APIENTRY *gglPushClientAttrib)(GLbitfield mask);
void (APIENTRY *gglTexCoordPointer)(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
void (APIENTRY *gglTexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels);
void (APIENTRY *gglTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
void (APIENTRY *gglVertexPointer)(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);

//GL_VERSION_1_2
void (APIENTRY *gglDrawRangeElements)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices);
void (APIENTRY *gglTexImage3D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
void (APIENTRY *gglTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels);
void (APIENTRY *gglCopyTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);

//GL_ARB_imaging
void (APIENTRY *gglBlendColor)(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
void (APIENTRY *gglBlendEquation)(GLenum mode);
void (APIENTRY *gglColorTable)(GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *table);
void (APIENTRY *gglColorTableParameterfv)(GLenum target, GLenum pname, const GLfloat *params);
void (APIENTRY *gglColorTableParameteriv)(GLenum target, GLenum pname, const GLint *params);
void (APIENTRY *gglCopyColorTable)(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
void (APIENTRY *gglGetColorTable)(GLenum target, GLenum format, GLenum type, GLvoid *table);
void (APIENTRY *gglGetColorTableParameterfv)(GLenum target, GLenum pname, GLfloat *params);
void (APIENTRY *gglGetColorTableParameteriv)(GLenum target, GLenum pname, GLint *params);
void (APIENTRY *gglColorSubTable)(GLenum target, GLsizei start, GLsizei count, GLenum format, GLenum type, const GLvoid *data);
void (APIENTRY *gglCopyColorSubTable)(GLenum target, GLsizei start, GLint x, GLint y, GLsizei width);
void (APIENTRY *gglConvolutionFilter1D)(GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *image);
void (APIENTRY *gglConvolutionFilter2D)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *image);
void (APIENTRY *gglConvolutionParameterf)(GLenum target, GLenum pname, GLfloat params);
void (APIENTRY *gglConvolutionParameterfv)(GLenum target, GLenum pname, const GLfloat *params);
void (APIENTRY *gglConvolutionParameteri)(GLenum target, GLenum pname, GLint params);
void (APIENTRY *gglConvolutionParameteriv)(GLenum target, GLenum pname, const GLint *params);
void (APIENTRY *gglCopyConvolutionFilter1D)(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
void (APIENTRY *gglCopyConvolutionFilter2D)(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height);
void (APIENTRY *gglGetConvolutionFilter)(GLenum target, GLenum format, GLenum type, GLvoid *image);
void (APIENTRY *gglGetConvolutionParameterfv)(GLenum target, GLenum pname, GLfloat *params);
void (APIENTRY *gglGetConvolutionParameteriv)(GLenum target, GLenum pname, GLint *params);
void (APIENTRY *gglGetSeparableFilter)(GLenum target, GLenum format, GLenum type, GLvoid *row, GLvoid *column, GLvoid *span);
void (APIENTRY *gglSeparableFilter2D)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *row, const GLvoid *column);
void (APIENTRY *gglGetHistogram)(GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid *values);
void (APIENTRY *gglGetHistogramParameterfv)(GLenum target, GLenum pname, GLfloat *params);
void (APIENTRY *gglGetHistogramParameteriv)(GLenum target, GLenum pname, GLint *params);
void (APIENTRY *gglGetMinmax)(GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid *values);
void (APIENTRY *gglGetMinmaxParameterfv)(GLenum target, GLenum pname, GLfloat *params);
void (APIENTRY *gglGetMinmaxParameteriv)(GLenum target, GLenum pname, GLint *params);
void (APIENTRY *gglHistogram)(GLenum target, GLsizei width, GLenum internalformat, GLboolean sink);
void (APIENTRY *gglMinmax)(GLenum target, GLenum internalformat, GLboolean sink);
void (APIENTRY *gglResetHistogram)(GLenum target);
void (APIENTRY *gglResetMinmax)(GLenum target);

//GL_VERSION_1_3
void (APIENTRY *gglActiveTexture)(GLenum texture);
void (APIENTRY *gglClientActiveTexture)(GLenum texture);
void (APIENTRY *gglMultiTexCoord1d)(GLenum target, GLdouble s);
void (APIENTRY *gglMultiTexCoord1dv)(GLenum target, const GLdouble *v);
void (APIENTRY *gglMultiTexCoord1f)(GLenum target, GLfloat s);
void (APIENTRY *gglMultiTexCoord1fv)(GLenum target, const GLfloat *v);
void (APIENTRY *gglMultiTexCoord1i)(GLenum target, GLint s);
void (APIENTRY *gglMultiTexCoord1iv)(GLenum target, const GLint *v);
void (APIENTRY *gglMultiTexCoord1s)(GLenum target, GLshort s);
void (APIENTRY *gglMultiTexCoord1sv)(GLenum target, const GLshort *v);
void (APIENTRY *gglMultiTexCoord2d)(GLenum target, GLdouble s, GLdouble t);
void (APIENTRY *gglMultiTexCoord2dv)(GLenum target, const GLdouble *v);
void (APIENTRY *gglMultiTexCoord2f)(GLenum target, GLfloat s, GLfloat t);
void (APIENTRY *gglMultiTexCoord2fv)(GLenum target, const GLfloat *v);
void (APIENTRY *gglMultiTexCoord2i)(GLenum target, GLint s, GLint t);
void (APIENTRY *gglMultiTexCoord2iv)(GLenum target, const GLint *v);
void (APIENTRY *gglMultiTexCoord2s)(GLenum target, GLshort s, GLshort t);
void (APIENTRY *gglMultiTexCoord2sv)(GLenum target, const GLshort *v);
void (APIENTRY *gglMultiTexCoord3d)(GLenum target, GLdouble s, GLdouble t, GLdouble r);
void (APIENTRY *gglMultiTexCoord3dv)(GLenum target, const GLdouble *v);
void (APIENTRY *gglMultiTexCoord3f)(GLenum target, GLfloat s, GLfloat t, GLfloat r);
void (APIENTRY *gglMultiTexCoord3fv)(GLenum target, const GLfloat *v);
void (APIENTRY *gglMultiTexCoord3i)(GLenum target, GLint s, GLint t, GLint r);
void (APIENTRY *gglMultiTexCoord3iv)(GLenum target, const GLint *v);
void (APIENTRY *gglMultiTexCoord3s)(GLenum target, GLshort s, GLshort t, GLshort r);
void (APIENTRY *gglMultiTexCoord3sv)(GLenum target, const GLshort *v);
void (APIENTRY *gglMultiTexCoord4d)(GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q);
void (APIENTRY *gglMultiTexCoord4dv)(GLenum target, const GLdouble *v);
void (APIENTRY *gglMultiTexCoord4f)(GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
void (APIENTRY *gglMultiTexCoord4fv)(GLenum target, const GLfloat *v);
void (APIENTRY *gglMultiTexCoord4i)(GLenum target, GLint s, GLint t, GLint r, GLint q);
void (APIENTRY *gglMultiTexCoord4iv)(GLenum target, const GLint *v);
void (APIENTRY *gglMultiTexCoord4s)(GLenum target, GLshort s, GLshort t, GLshort r, GLshort q);
void (APIENTRY *gglMultiTexCoord4sv)(GLenum target, const GLshort *v);
void (APIENTRY *gglLoadTransposeMatrixf)(const GLfloat *m);
void (APIENTRY *gglLoadTransposeMatrixd)(const GLdouble *m);
void (APIENTRY *gglMultTransposeMatrixf)(const GLfloat *m);
void (APIENTRY *gglMultTransposeMatrixd)(const GLdouble *m);
void (APIENTRY *gglSampleCoverage)(GLclampf value, GLboolean invert);
void (APIENTRY *gglCompressedTexImage3D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid *data);
void (APIENTRY *gglCompressedTexImage2D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data);
void (APIENTRY *gglCompressedTexImage1D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid *data);
void (APIENTRY *gglCompressedTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid *data);
void (APIENTRY *gglCompressedTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data);
void (APIENTRY *gglCompressedTexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid *data);
void (APIENTRY *gglGetCompressedTexImage)(GLenum target, GLint level, void *img);

//GL_VERSION_1_4
void (APIENTRY *gglBlendFuncSeparate)(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
void (APIENTRY *gglFogCoordf)(GLfloat coord);
void (APIENTRY *gglFogCoordfv)(const GLfloat *coord);
void (APIENTRY *gglFogCoordd)(GLdouble coord);
void (APIENTRY *gglFogCoorddv)(const GLdouble *coord);
void (APIENTRY *gglFogCoordPointer)(GLenum type, GLsizei stride, const GLvoid *pointer);
void (APIENTRY *gglMultiDrawArrays)(GLenum mode, GLint *first, GLsizei *count, GLsizei primcount);
void (APIENTRY *gglMultiDrawElements)(GLenum mode, const GLsizei *count, GLenum type, const GLvoid **indices, GLsizei primcount);
void (APIENTRY *gglPointParameterf)(GLenum pname, GLfloat param);
void (APIENTRY *gglPointParameterfv)(GLenum pname, const GLfloat *params);
void (APIENTRY *gglPointParameteri)(GLenum pname, GLint param);
void (APIENTRY *gglPointParameteriv)(GLenum pname, const GLint *params);
void (APIENTRY *gglSecondaryColor3b)(GLbyte red, GLbyte green, GLbyte blue);
void (APIENTRY *gglSecondaryColor3bv)(const GLbyte *v);
void (APIENTRY *gglSecondaryColor3d)(GLdouble red, GLdouble green, GLdouble blue);
void (APIENTRY *gglSecondaryColor3dv)(const GLdouble *v);
void (APIENTRY *gglSecondaryColor3f)(GLfloat red, GLfloat green, GLfloat blue);
void (APIENTRY *gglSecondaryColor3fv)(const GLfloat *v);
void (APIENTRY *gglSecondaryColor3i)(GLint red, GLint green, GLint blue);
void (APIENTRY *gglSecondaryColor3iv)(const GLint *v);
void (APIENTRY *gglSecondaryColor3s)(GLshort red, GLshort green, GLshort blue);
void (APIENTRY *gglSecondaryColor3sv)(const GLshort *v);
void (APIENTRY *gglSecondaryColor3ub)(GLubyte red, GLubyte green, GLubyte blue);
void (APIENTRY *gglSecondaryColor3ubv)(const GLubyte *v);
void (APIENTRY *gglSecondaryColor3ui)(GLuint red, GLuint green, GLuint blue);
void (APIENTRY *gglSecondaryColor3uiv)(const GLuint *v);
void (APIENTRY *gglSecondaryColor3us)(GLushort red, GLushort green, GLushort blue);
void (APIENTRY *gglSecondaryColor3usv)(const GLushort *v);
void (APIENTRY *gglSecondaryColorPointer)(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
void (APIENTRY *gglWindowPos2d)(GLdouble x, GLdouble y);
void (APIENTRY *gglWindowPos2dv)(const GLdouble *p);
void (APIENTRY *gglWindowPos2f)(GLfloat x, GLfloat y);
void (APIENTRY *gglWindowPos2fv)(const GLfloat *p);
void (APIENTRY *gglWindowPos2i)(GLint x, GLint y);
void (APIENTRY *gglWindowPos2iv)(const GLint *p);
void (APIENTRY *gglWindowPos2s)(GLshort x, GLshort y);
void (APIENTRY *gglWindowPos2sv)(const GLshort *p);
void (APIENTRY *gglWindowPos3d)(GLdouble x, GLdouble y, GLdouble z);
void (APIENTRY *gglWindowPos3dv)(const GLdouble *p);
void (APIENTRY *gglWindowPos3f)(GLfloat x, GLfloat y, GLfloat z);
void (APIENTRY *gglWindowPos3fv)(const GLfloat *p);
void (APIENTRY *gglWindowPos3i)(GLint x, GLint y, GLint z);
void (APIENTRY *gglWindowPos3iv)(const GLint *p);
void (APIENTRY *gglWindowPos3s)(GLshort x, GLshort y, GLshort z);
void (APIENTRY *gglWindowPos3sv)(const GLshort *p);

//GL_VERSION_1_5
void (APIENTRY *gglGenQueries)(GLsizei n, GLuint *ids);
void (APIENTRY *gglDeleteQueries)(GLsizei n, const GLuint *ids);
GLboolean (APIENTRY *gglIsQuery)(GLuint id);
void (APIENTRY *gglBeginQuery)(GLenum target, GLuint id);
void (APIENTRY *gglEndQuery)(GLenum target);
void (APIENTRY *gglGetQueryiv)(GLenum target, GLenum pname, GLint *params);
void (APIENTRY *gglGetQueryObjectiv)(GLuint id, GLenum pname, GLint *params);
void (APIENTRY *gglGetQueryObjectuiv)(GLuint id, GLenum pname, GLuint *params);
void (APIENTRY *gglBindBuffer)(GLenum target, GLuint buffer);
void (APIENTRY *gglDeleteBuffers)(GLsizei n, const GLuint *buffers);
void (APIENTRY *gglGenBuffers)(GLsizei n, GLuint *buffers);
GLboolean (APIENTRY *gglIsBuffer)(GLuint buffer);
void (APIENTRY *gglBufferData)(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
void (APIENTRY *gglBufferSubData)(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data);
void (APIENTRY *gglGetBufferSubData)(GLenum target, GLintptr offset, GLsizeiptr size, GLvoid *data);
GLvoid* (APIENTRY *gglMapBuffer)(GLenum target, GLenum access);
GLboolean (APIENTRY *gglUnmapBuffer)(GLenum target);
void (APIENTRY *gglGetBufferParameteriv)(GLenum target, GLenum pname, GLint *params);
void (APIENTRY *gglGetBufferPointerv)(GLenum target, GLenum pname, GLvoid* *params);

//GL_VERSION_2_0
void (APIENTRY *gglBlendEquationSeparate)(GLenum modeRGB, GLenum modeAlpha);
void (APIENTRY *gglDrawBuffers)(GLsizei n, const GLenum *bufs);
void (APIENTRY *gglStencilOpSeparate)(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
void (APIENTRY *gglStencilFuncSeparate)(GLenum frontfunc, GLenum backfunc, GLint ref, GLuint mask);
void (APIENTRY *gglStencilMaskSeparate)(GLenum face, GLuint mask);
void (APIENTRY *gglAttachShader)(GLuint program, GLuint shader);
void (APIENTRY *gglBindAttribLocation)(GLuint program, GLuint index, const GLchar *name);
void (APIENTRY *gglCompileShader)(GLuint shader);
GLuint (APIENTRY *gglCreateProgram)(void);
GLuint (APIENTRY *gglCreateShader)(GLenum type);
void (APIENTRY *gglDeleteProgram)(GLuint program);
void (APIENTRY *gglDeleteShader)(GLuint shader);
void (APIENTRY *gglDetachShader)(GLuint program, GLuint shader);
void (APIENTRY *gglDisableVertexAttribArray)(GLuint index);
void (APIENTRY *gglEnableVertexAttribArray)(GLuint index);
void (APIENTRY *gglGetActiveAttrib)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
void (APIENTRY *gglGetActiveUniform)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
void (APIENTRY *gglGetAttachedShaders)(GLuint program, GLsizei maxCount, GLsizei *count, GLuint *obj);
GLint (APIENTRY *gglGetAttribLocation)(GLuint program, const GLchar *name);
void (APIENTRY *gglGetProgramiv)(GLuint program, GLenum pname, GLint *params);
void (APIENTRY *gglGetProgramInfoLog)(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
void (APIENTRY *gglGetShaderiv)(GLuint shader, GLenum pname, GLint *params);
void (APIENTRY *gglGetShaderInfoLog)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
void (APIENTRY *gglGetShaderSource)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source);
GLint (APIENTRY *gglGetUniformLocation)(GLuint program, const GLchar *name);
void (APIENTRY *gglGetUniformfv)(GLuint program, GLint location, GLfloat *params);
void (APIENTRY *gglGetUniformiv)(GLuint program, GLint location, GLint *params);
void (APIENTRY *gglGetVertexAttribdv)(GLuint index, GLenum pname, GLdouble *params);
void (APIENTRY *gglGetVertexAttribfv)(GLuint index, GLenum pname, GLfloat *params);
void (APIENTRY *gglGetVertexAttribiv)(GLuint index, GLenum pname, GLint *params);
void (APIENTRY *gglGetVertexAttribPointerv)(GLuint index, GLenum pname, GLvoid* *pointer);
GLboolean (APIENTRY *gglIsProgram)(GLuint program);
GLboolean (APIENTRY *gglIsShader)(GLuint shader);
void (APIENTRY *gglLinkProgram)(GLuint program);
void (APIENTRY *gglShaderSource)(GLuint shader, GLsizei count, const GLchar* *string, const GLint *length);
void (APIENTRY *gglUseProgram)(GLuint program);
void (APIENTRY *gglUniform1f)(GLint location, GLfloat v0);
void (APIENTRY *gglUniform2f)(GLint location, GLfloat v0, GLfloat v1);
void (APIENTRY *gglUniform3f)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
void (APIENTRY *gglUniform4f)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
void (APIENTRY *gglUniform1i)(GLint location, GLint v0);
void (APIENTRY *gglUniform2i)(GLint location, GLint v0, GLint v1);
void (APIENTRY *gglUniform3i)(GLint location, GLint v0, GLint v1, GLint v2);
void (APIENTRY *gglUniform4i)(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
void (APIENTRY *gglUniform1fv)(GLint location, GLsizei count, const GLfloat *value);
void (APIENTRY *gglUniform2fv)(GLint location, GLsizei count, const GLfloat *value);
void (APIENTRY *gglUniform3fv)(GLint location, GLsizei count, const GLfloat *value);
void (APIENTRY *gglUniform4fv)(GLint location, GLsizei count, const GLfloat *value);
void (APIENTRY *gglUniform1iv)(GLint location, GLsizei count, const GLint *value);
void (APIENTRY *gglUniform2iv)(GLint location, GLsizei count, const GLint *value);
void (APIENTRY *gglUniform3iv)(GLint location, GLsizei count, const GLint *value);
void (APIENTRY *gglUniform4iv)(GLint location, GLsizei count, const GLint *value);
void (APIENTRY *gglUniformMatrix2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void (APIENTRY *gglUniformMatrix3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void (APIENTRY *gglUniformMatrix4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void (APIENTRY *gglValidateProgram)(GLuint program);
void (APIENTRY *gglVertexAttrib1d)(GLuint index, GLdouble x);
void (APIENTRY *gglVertexAttrib1dv)(GLuint index, const GLdouble *v);
void (APIENTRY *gglVertexAttrib1f)(GLuint index, GLfloat x);
void (APIENTRY *gglVertexAttrib1fv)(GLuint index, const GLfloat *v);
void (APIENTRY *gglVertexAttrib1s)(GLuint index, GLshort x);
void (APIENTRY *gglVertexAttrib1sv)(GLuint index, const GLshort *v);
void (APIENTRY *gglVertexAttrib2d)(GLuint index, GLdouble x, GLdouble y);
void (APIENTRY *gglVertexAttrib2dv)(GLuint index, const GLdouble *v);
void (APIENTRY *gglVertexAttrib2f)(GLuint index, GLfloat x, GLfloat y);
void (APIENTRY *gglVertexAttrib2fv)(GLuint index, const GLfloat *v);
void (APIENTRY *gglVertexAttrib2s)(GLuint index, GLshort x, GLshort y);
void (APIENTRY *gglVertexAttrib2sv)(GLuint index, const GLshort *v);
void (APIENTRY *gglVertexAttrib3d)(GLuint index, GLdouble x, GLdouble y, GLdouble z);
void (APIENTRY *gglVertexAttrib3dv)(GLuint index, const GLdouble *v);
void (APIENTRY *gglVertexAttrib3f)(GLuint index, GLfloat x, GLfloat y, GLfloat z);
void (APIENTRY *gglVertexAttrib3fv)(GLuint index, const GLfloat *v);
void (APIENTRY *gglVertexAttrib3s)(GLuint index, GLshort x, GLshort y, GLshort z);
void (APIENTRY *gglVertexAttrib3sv)(GLuint index, const GLshort *v);
void (APIENTRY *gglVertexAttrib4Nbv)(GLuint index, const GLbyte *v);
void (APIENTRY *gglVertexAttrib4Niv)(GLuint index, const GLint *v);
void (APIENTRY *gglVertexAttrib4Nsv)(GLuint index, const GLshort *v);
void (APIENTRY *gglVertexAttrib4Nub)(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
void (APIENTRY *gglVertexAttrib4Nubv)(GLuint index, const GLubyte *v);
void (APIENTRY *gglVertexAttrib4Nuiv)(GLuint index, const GLuint *v);
void (APIENTRY *gglVertexAttrib4Nusv)(GLuint index, const GLushort *v);
void (APIENTRY *gglVertexAttrib4bv)(GLuint index, const GLbyte *v);
void (APIENTRY *gglVertexAttrib4d)(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void (APIENTRY *gglVertexAttrib4dv)(GLuint index, const GLdouble *v);
void (APIENTRY *gglVertexAttrib4f)(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void (APIENTRY *gglVertexAttrib4fv)(GLuint index, const GLfloat *v);
void (APIENTRY *gglVertexAttrib4iv)(GLuint index, const GLint *v);
void (APIENTRY *gglVertexAttrib4s)(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
void (APIENTRY *gglVertexAttrib4sv)(GLuint index, const GLshort *v);
void (APIENTRY *gglVertexAttrib4ubv)(GLuint index, const GLubyte *v);
void (APIENTRY *gglVertexAttrib4uiv)(GLuint index, const GLuint *v);
void (APIENTRY *gglVertexAttrib4usv)(GLuint index, const GLushort *v);
void (APIENTRY *gglVertexAttribPointer)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);

//GL_VERSION_2_1
void (APIENTRY *gglUniformMatrix2x3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void (APIENTRY *gglUniformMatrix3x2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void (APIENTRY *gglUniformMatrix2x4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void (APIENTRY *gglUniformMatrix4x2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void (APIENTRY *gglUniformMatrix3x4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void (APIENTRY *gglUniformMatrix4x3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

//GL_VERSION_3_0
//includes GL_ARB_map_buffer_range
//includes GL_ARB_framebuffer_object
//includes GL_ARB_vertex_array_object
void (APIENTRY *gglColorMaski)(GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a);
void (APIENTRY *gglGetBooleani_v)(GLenum target, GLuint index, GLboolean *data);
void (APIENTRY *gglGetIntegeri_v)(GLenum target, GLuint index, GLint *data);
void (APIENTRY *gglEnablei)(GLenum target, GLuint index);
void (APIENTRY *gglDisablei)(GLenum target, GLuint index);
GLboolean (APIENTRY *gglIsEnabledi)(GLenum target, GLuint index);
void (APIENTRY *gglBeginTransformFeedback)(GLenum primitiveMode);
void (APIENTRY *gglEndTransformFeedback)(void);
void (APIENTRY *gglBindBufferRange)(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
void (APIENTRY *gglBindBufferBase)(GLenum target, GLuint index, GLuint buffer);
void (APIENTRY *gglTransformFeedbackVaryings)(GLuint program, GLsizei count, const GLint *locations, GLenum bufferMode);
void (APIENTRY *gglGetTransformFeedbackVarying)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name);
void (APIENTRY *gglClampColor)(GLenum target, GLenum clamp);
void (APIENTRY *gglBeginConditionalRender)(GLuint id, GLenum mode);
void (APIENTRY *gglEndConditionalRender)(void);
void (APIENTRY *gglVertexAttribIPointer)(GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer);
void (APIENTRY *gglGetVertexAttribIiv)(GLuint index, GLenum pname, GLint *params);
void (APIENTRY *gglGetVertexAttribIuiv)(GLuint index, GLenum pname, GLuint *params);
void (APIENTRY *gglVertexAttribI1i)(GLuint index, GLint x);
void (APIENTRY *gglVertexAttribI2i)(GLuint index, GLint x, GLint y);
void (APIENTRY *gglVertexAttribI3i)(GLuint index, GLint x, GLint y, GLint z);
void (APIENTRY *gglVertexAttribI4i)(GLuint index, GLint x, GLint y, GLint z, GLint w);
void (APIENTRY *gglVertexAttribI1ui)(GLuint index, GLuint x);
void (APIENTRY *gglVertexAttribI2ui)(GLuint index, GLuint x, GLuint y);
void (APIENTRY *gglVertexAttribI3ui)(GLuint index, GLuint x, GLuint y, GLuint z);
void (APIENTRY *gglVertexAttribI4ui)(GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
void (APIENTRY *gglVertexAttribI1iv)(GLuint index, const GLint *v);
void (APIENTRY *gglVertexAttribI2iv)(GLuint index, const GLint *v);
void (APIENTRY *gglVertexAttribI3iv)(GLuint index, const GLint *v);
void (APIENTRY *gglVertexAttribI4iv)(GLuint index, const GLint *v);
void (APIENTRY *gglVertexAttribI1uiv)(GLuint index, const GLuint *v);
void (APIENTRY *gglVertexAttribI2uiv)(GLuint index, const GLuint *v);
void (APIENTRY *gglVertexAttribI3uiv)(GLuint index, const GLuint *v);
void (APIENTRY *gglVertexAttribI4uiv)(GLuint index, const GLuint *v);
void (APIENTRY *gglVertexAttribI4bv)(GLuint index, const GLbyte *v);
void (APIENTRY *gglVertexAttribI4sv)(GLuint index, const GLshort *v);
void (APIENTRY *gglVertexAttribI4ubv)(GLuint index, const GLubyte *v);
void (APIENTRY *gglVertexAttribI4usv)(GLuint index, const GLushort *v);
void (APIENTRY *gglGetUniformuiv)(GLuint program, GLint location, GLuint *params);
void (APIENTRY *gglBindFragDataLocation)(GLuint program, GLuint color, const GLchar *name);
GLint (APIENTRY *gglGetFragDataLocation)(GLuint program, const GLchar *name);
void (APIENTRY *gglUniform1ui)(GLint location, GLuint v0);
void (APIENTRY *gglUniform2ui)(GLint location, GLuint v0, GLuint v1);
void (APIENTRY *gglUniform3ui)(GLint location, GLuint v0, GLuint v1, GLuint v2);
void (APIENTRY *gglUniform4ui)(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
void (APIENTRY *gglUniform1uiv)(GLint location, GLsizei count, const GLuint *value);
void (APIENTRY *gglUniform2uiv)(GLint location, GLsizei count, const GLuint *value);
void (APIENTRY *gglUniform3uiv)(GLint location, GLsizei count, const GLuint *value);
void (APIENTRY *gglUniform4uiv)(GLint location, GLsizei count, const GLuint *value);
void (APIENTRY *gglTexParameterIiv)(GLenum target, GLenum pname, const GLint *params);
void (APIENTRY *gglTexParameterIuiv)(GLenum target, GLenum pname, const GLuint *params);
void (APIENTRY *gglGetTexParameterIiv)(GLenum target, GLenum pname, GLint *params);
void (APIENTRY *gglGetTexParameterIuiv)(GLenum target, GLenum pname, GLuint *params);
void (APIENTRY *gglClearBufferiv)(GLenum buffer, GLint drawbuffer, const GLint *value);
void (APIENTRY *gglClearBufferuiv)(GLenum buffer, GLint drawbuffer, const GLuint *value);
void (APIENTRY *gglClearBufferfv)(GLenum buffer, GLint drawbuffer, const GLfloat *value);
void (APIENTRY *gglClearBufferfi)(GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil);
const GLubyte *(APIENTRY *gglGetStringi)(GLenum name, GLuint index);
//GLboolean (APIENTRY *gglIsRenderbuffer)(GLuint renderbuffer);
//void (APIENTRY *gglBindRenderbuffer)(GLenum target, GLuint renderbuffer);
//void (APIENTRY *gglDeleteRenderbuffers)(GLsizei n, const GLuint *renderbuffers);
//void (APIENTRY *gglGenRenderbuffers)(GLsizei n, GLuint *renderbuffers);
//void (APIENTRY *gglRenderbufferStorage)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
//void (APIENTRY *gglGetRenderbufferParameteriv)(GLenum target, GLenum pname, GLint *params);
//GLboolean (APIENTRY *gglIsFramebuffer)(GLuint framebuffer);
//void (APIENTRY *gglBindFramebuffer)(GLenum target, GLuint framebuffer);
//void (APIENTRY *gglDeleteFramebuffers)(GLsizei n, const GLuint *framebuffers);
//void (APIENTRY *gglGenFramebuffers)(GLsizei n, GLuint *framebuffers);
//GLenum (APIENTRY *gglCheckFramebufferStatus)(GLenum target);
//void (APIENTRY *gglFramebufferTexture1D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
//void (APIENTRY *gglFramebufferTexture2D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
//void (APIENTRY *gglFramebufferTexture3D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
//void (APIENTRY *gglFramebufferRenderbuffer)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
//void (APIENTRY *gglGetFramebufferAttachmentParameteriv)(GLenum target, GLenum attachment, GLenum pname, GLint *params);
//void (APIENTRY *gglGenerateMipmap)(GLenum target);
//void (APIENTRY *gglBlitFramebuffer)(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
//void (APIENTRY *gglRenderbufferStorageMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
//void (APIENTRY *gglFramebufferTextureLayer)(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
//void *(APIENTRY *gglMapBufferRange)(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
//void (APIENTRY *gglFlushMappedBufferRange)(GLenum target, GLintptr offset, GLsizeiptr length);
//void (APIENTRY *gglBindVertexArray)(GLuint array);
//void (APIENTRY *gglDeleteVertexArrays)(GLsizei n, const GLuint *arrays);
//void (APIENTRY *gglGenVertexArrays)(GLsizei n, GLuint *arrays);
//GLboolean (APIENTRY *gglIsVertexArray)(GLuint array);

//GL_VERSION_3_1
void (APIENTRY *gglDrawArraysInstanced)(GLenum mode, GLint first, GLsizei count, GLsizei instancecount);
void (APIENTRY *gglDrawElementsInstanced)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount);
void (APIENTRY *gglTexBuffer)(GLenum target, GLenum internalformat, GLuint buffer);
void (APIENTRY *gglPrimitiveRestartIndex)(GLuint index);
void (APIENTRY *gglCopyBufferSubData)(GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
void (APIENTRY *gglGetUniformIndices)(GLuint program, GLsizei uniformCount, const GLchar *const*uniformNames, GLuint *uniformIndices);
void (APIENTRY *gglGetActiveUniformsiv)(GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params);
void (APIENTRY *gglGetActiveUniformName)(GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformName);
GLuint (APIENTRY *gglGetUniformBlockIndex)(GLuint program, const GLchar *uniformBlockName);
void (APIENTRY *gglGetActiveUniformBlockiv)(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params);
void (APIENTRY *gglGetActiveUniformBlockName)(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName);
void (APIENTRY *gglUniformBlockBinding)(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);

//GL_VERSION_3_2
void (APIENTRY *gglDrawElementsBaseVertex)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLint basevertex);
void (APIENTRY *gglDrawRangeElementsBaseVertex)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices, GLint basevertex);
void (APIENTRY *gglDrawElementsInstancedBaseVertex)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLint basevertex);
void (APIENTRY *gglMultiDrawElementsBaseVertex)(GLenum mode, const GLsizei *count, GLenum type, const void *const*indices, GLsizei drawcount, const GLint *basevertex);
void (APIENTRY *gglProvokingVertex)(GLenum mode);
GLsync (APIENTRY *gglFenceSync)(GLenum condition, GLbitfield flags);
GLboolean (APIENTRY *gglIsSync)(GLsync sync);
void (APIENTRY *gglDeleteSync)(GLsync sync);
GLenum (APIENTRY *gglClientWaitSync)(GLsync sync, GLbitfield flags, GLuint64 timeout);
void (APIENTRY *gglWaitSync)(GLsync sync, GLbitfield flags, GLuint64 timeout);
void (APIENTRY *gglGetInteger64v)(GLenum pname, GLint64 *data);
void (APIENTRY *gglGetSynciv)(GLsync sync, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values);
void (APIENTRY *gglGetInteger64i_v)(GLenum target, GLuint index, GLint64 *data);
void (APIENTRY *gglGetBufferParameteri64v)(GLenum target, GLenum pname, GLint64 *params);
void (APIENTRY *gglFramebufferTexture)(GLenum target, GLenum attachment, GLuint texture, GLint level);
void (APIENTRY *gglTexImage2DMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
void (APIENTRY *gglTexImage3DMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
void (APIENTRY *gglGetMultisamplefv)(GLenum pname, GLuint index, GLfloat *val);
void (APIENTRY *gglSampleMaski)(GLuint maskNumber, GLbitfield mask);

//GL_VERSION_3_3
//includes GL_ARB_instanced_arrays
void (APIENTRY *gglBindFragDataLocationIndexed)(GLuint program, GLuint colorNumber, GLuint index, const GLchar *name);
GLint (APIENTRY *gglGetFragDataIndex)(GLuint program, const GLchar *name);
void (APIENTRY *gglGenSamplers)(GLsizei count, GLuint *samplers);
void (APIENTRY *gglDeleteSamplers)(GLsizei count, const GLuint *samplers);
GLboolean (APIENTRY *gglIsSampler)(GLuint sampler);
void (APIENTRY *gglBindSampler)(GLuint unit, GLuint sampler);
void (APIENTRY *gglSamplerParameteri)(GLuint sampler, GLenum pname, GLint param);
void (APIENTRY *gglSamplerParameteriv)(GLuint sampler, GLenum pname, const GLint *param);
void (APIENTRY *gglSamplerParameterf)(GLuint sampler, GLenum pname, GLfloat param);
void (APIENTRY *gglSamplerParameterfv)(GLuint sampler, GLenum pname, const GLfloat *param);
void (APIENTRY *gglSamplerParameterIiv)(GLuint sampler, GLenum pname, const GLint *param);
void (APIENTRY *gglSamplerParameterIuiv)(GLuint sampler, GLenum pname, const GLuint *param);
void (APIENTRY *gglGetSamplerParameteriv)(GLuint sampler, GLenum pname, GLint *params);
void (APIENTRY *gglGetSamplerParameterIiv)(GLuint sampler, GLenum pname, GLint *params);
void (APIENTRY *gglGetSamplerParameterfv)(GLuint sampler, GLenum pname, GLfloat *params);
void (APIENTRY *gglGetSamplerParameterIuiv)(GLuint sampler, GLenum pname, GLuint *params);
void (APIENTRY *gglQueryCounter)(GLuint id, GLenum target);
void (APIENTRY *gglGetQueryObjecti64v)(GLuint id, GLenum pname, GLint64 *params);
void (APIENTRY *gglGetQueryObjectui64v)(GLuint id, GLenum pname, GLuint64 *params);
//void (APIENTRY *gglVertexAttribDivisor)(GLuint index, GLuint divisor);
void (APIENTRY *gglVertexAttribP1ui)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
void (APIENTRY *gglVertexAttribP1uiv)(GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
void (APIENTRY *gglVertexAttribP2ui)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
void (APIENTRY *gglVertexAttribP2uiv)(GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
void (APIENTRY *gglVertexAttribP3ui)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
void (APIENTRY *gglVertexAttribP3uiv)(GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
void (APIENTRY *gglVertexAttribP4ui)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
void (APIENTRY *gglVertexAttribP4uiv)(GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
void (APIENTRY *gglVertexP2ui)(GLenum type, GLuint value);
void (APIENTRY *gglVertexP2uiv)(GLenum type, const GLuint *value);
void (APIENTRY *gglVertexP3ui)(GLenum type, GLuint value);
void (APIENTRY *gglVertexP3uiv)(GLenum type, const GLuint *value);
void (APIENTRY *gglVertexP4ui)(GLenum type, GLuint value);
void (APIENTRY *gglVertexP4uiv)(GLenum type, const GLuint *value);
void (APIENTRY *gglTexCoordP1ui)(GLenum type, GLuint coords);
void (APIENTRY *gglTexCoordP1uiv)(GLenum type, const GLuint *coords);
void (APIENTRY *gglTexCoordP2ui)(GLenum type, GLuint coords);
void (APIENTRY *gglTexCoordP2uiv)(GLenum type, const GLuint *coords);
void (APIENTRY *gglTexCoordP3ui)(GLenum type, GLuint coords);
void (APIENTRY *gglTexCoordP3uiv)(GLenum type, const GLuint *coords);
void (APIENTRY *gglTexCoordP4ui)(GLenum type, GLuint coords);
void (APIENTRY *gglTexCoordP4uiv)(GLenum type, const GLuint *coords);
void (APIENTRY *gglMultiTexCoordP1ui)(GLenum texture, GLenum type, GLuint coords);
void (APIENTRY *gglMultiTexCoordP1uiv)(GLenum texture, GLenum type, const GLuint *coords);
void (APIENTRY *gglMultiTexCoordP2ui)(GLenum texture, GLenum type, GLuint coords);
void (APIENTRY *gglMultiTexCoordP2uiv)(GLenum texture, GLenum type, const GLuint *coords);
void (APIENTRY *gglMultiTexCoordP3ui)(GLenum texture, GLenum type, GLuint coords);
void (APIENTRY *gglMultiTexCoordP3uiv)(GLenum texture, GLenum type, const GLuint *coords);
void (APIENTRY *gglMultiTexCoordP4ui)(GLenum texture, GLenum type, GLuint coords);
void (APIENTRY *gglMultiTexCoordP4uiv)(GLenum texture, GLenum type, const GLuint *coords);
void (APIENTRY *gglNormalP3ui)(GLenum type, GLuint coords);
void (APIENTRY *gglNormalP3uiv)(GLenum type, const GLuint *coords);
void (APIENTRY *gglColorP3ui)(GLenum type, GLuint color);
void (APIENTRY *gglColorP3uiv)(GLenum type, const GLuint *color);
void (APIENTRY *gglColorP4ui)(GLenum type, GLuint color);
void (APIENTRY *gglColorP4uiv)(GLenum type, const GLuint *color);
void (APIENTRY *gglSecondaryColorP3ui)(GLenum type, GLuint color);
void (APIENTRY *gglSecondaryColorP3uiv)(GLenum type, const GLuint *color);

//GL_VERSION_4_0
void (APIENTRY *gglMinSampleShading)(GLfloat value);
void (APIENTRY *gglBlendEquationi)(GLuint buf, GLenum mode);
void (APIENTRY *gglBlendEquationSeparatei)(GLuint buf, GLenum modeRGB, GLenum modeAlpha);
void (APIENTRY *gglBlendFunci)(GLuint buf, GLenum src, GLenum dst);
void (APIENTRY *gglBlendFuncSeparatei)(GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
void (APIENTRY *gglDrawArraysIndirect)(GLenum mode, const void *indirect);
void (APIENTRY *gglDrawElementsIndirect)(GLenum mode, GLenum type, const void *indirect);
void (APIENTRY *gglUniform1d)(GLint location, GLdouble x);
void (APIENTRY *gglUniform2d)(GLint location, GLdouble x, GLdouble y);
void (APIENTRY *gglUniform3d)(GLint location, GLdouble x, GLdouble y, GLdouble z);
void (APIENTRY *gglUniform4d)(GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void (APIENTRY *gglUniform1dv)(GLint location, GLsizei count, const GLdouble *value);
void (APIENTRY *gglUniform2dv)(GLint location, GLsizei count, const GLdouble *value);
void (APIENTRY *gglUniform3dv)(GLint location, GLsizei count, const GLdouble *value);
void (APIENTRY *gglUniform4dv)(GLint location, GLsizei count, const GLdouble *value);
void (APIENTRY *gglUniformMatrix2dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
void (APIENTRY *gglUniformMatrix3dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
void (APIENTRY *gglUniformMatrix4dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
void (APIENTRY *gglUniformMatrix2x3dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
void (APIENTRY *gglUniformMatrix2x4dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
void (APIENTRY *gglUniformMatrix3x2dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
void (APIENTRY *gglUniformMatrix3x4dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
void (APIENTRY *gglUniformMatrix4x2dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
void (APIENTRY *gglUniformMatrix4x3dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
void (APIENTRY *gglGetUniformdv)(GLuint program, GLint location, GLdouble *params);
GLint (APIENTRY *gglGetSubroutineUniformLocation)(GLuint program, GLenum shadertype, const GLchar *name);
GLuint (APIENTRY *gglGetSubroutineIndex)(GLuint program, GLenum shadertype, const GLchar *name);
void (APIENTRY *gglGetActiveSubroutineUniformiv)(GLuint program, GLenum shadertype, GLuint index, GLenum pname, GLint *values);
void (APIENTRY *gglGetActiveSubroutineUniformName)(GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei *length, GLchar *name);
void (APIENTRY *gglGetActiveSubroutineName)(GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei *length, GLchar *name);
void (APIENTRY *gglUniformSubroutinesuiv)(GLenum shadertype, GLsizei count, const GLuint *indices);
void (APIENTRY *gglGetUniformSubroutineuiv)(GLenum shadertype, GLint location, GLuint *params);
void (APIENTRY *gglGetProgramStageiv)(GLuint program, GLenum shadertype, GLenum pname, GLint *values);
void (APIENTRY *gglPatchParameteri)(GLenum pname, GLint value);
void (APIENTRY *gglPatchParameterfv)(GLenum pname, const GLfloat *values);
void (APIENTRY *gglBindTransformFeedback)(GLenum target, GLuint id);
void (APIENTRY *gglDeleteTransformFeedbacks)(GLsizei n, const GLuint *ids);
void (APIENTRY *gglGenTransformFeedbacks)(GLsizei n, GLuint *ids);
GLboolean (APIENTRY *gglIsTransformFeedback)(GLuint id);
void (APIENTRY *gglPauseTransformFeedback)(void);
void (APIENTRY *gglResumeTransformFeedback)(void);
void (APIENTRY *gglDrawTransformFeedback)(GLenum mode, GLuint id);
void (APIENTRY *gglDrawTransformFeedbackStream)(GLenum mode, GLuint id, GLuint stream);
void (APIENTRY *gglBeginQueryIndexed)(GLenum target, GLuint index, GLuint id);
void (APIENTRY *gglEndQueryIndexed)(GLenum target, GLuint index);
void (APIENTRY *gglGetQueryIndexediv)(GLenum target, GLuint index, GLenum pname, GLint *params);

//GL_VERSION_4_1
void (APIENTRY *gglReleaseShaderCompiler)(void);
void (APIENTRY *gglShaderBinary)(GLsizei count, const GLuint *shaders, GLenum binaryformat, const void *binary, GLsizei length);
void (APIENTRY *gglGetShaderPrecisionFormat)(GLenum shadertype, GLenum precisiontype, GLint *range, GLint *precision);
void (APIENTRY *gglDepthRangef)(GLfloat n, GLfloat f);
void (APIENTRY *gglClearDepthf)(GLfloat d);
void (APIENTRY *gglGetProgramBinary)(GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, void *binary);
void (APIENTRY *gglProgramBinary)(GLuint program, GLenum binaryFormat, const void *binary, GLsizei length);
void (APIENTRY *gglProgramParameteri)(GLuint program, GLenum pname, GLint value);
void (APIENTRY *gglUseProgramStages)(GLuint pipeline, GLbitfield stages, GLuint program);
void (APIENTRY *gglActiveShaderProgram)(GLuint pipeline, GLuint program);
GLuint (APIENTRY *gglCreateShaderProgramv)(GLenum type, GLsizei count, const GLchar *const*strings);
void (APIENTRY *gglBindProgramPipeline)(GLuint pipeline);
void (APIENTRY *gglDeleteProgramPipelines)(GLsizei n, const GLuint *pipelines);
void (APIENTRY *gglGenProgramPipelines)(GLsizei n, GLuint *pipelines);
GLboolean (APIENTRY *gglIsProgramPipeline)(GLuint pipeline);
void (APIENTRY *gglGetProgramPipelineiv)(GLuint pipeline, GLenum pname, GLint *params);
void (APIENTRY *gglProgramUniform1i)(GLuint program, GLint location, GLint v0);
void (APIENTRY *gglProgramUniform1iv)(GLuint program, GLint location, GLsizei count, const GLint *value);
void (APIENTRY *gglProgramUniform1f)(GLuint program, GLint location, GLfloat v0);
void (APIENTRY *gglProgramUniform1fv)(GLuint program, GLint location, GLsizei count, const GLfloat *value);
void (APIENTRY *gglProgramUniform1d)(GLuint program, GLint location, GLdouble v0);
void (APIENTRY *gglProgramUniform1dv)(GLuint program, GLint location, GLsizei count, const GLdouble *value);
void (APIENTRY *gglProgramUniform1ui)(GLuint program, GLint location, GLuint v0);
void (APIENTRY *gglProgramUniform1uiv)(GLuint program, GLint location, GLsizei count, const GLuint *value);
void (APIENTRY *gglProgramUniform2i)(GLuint program, GLint location, GLint v0, GLint v1);
void (APIENTRY *gglProgramUniform2iv)(GLuint program, GLint location, GLsizei count, const GLint *value);
void (APIENTRY *gglProgramUniform2f)(GLuint program, GLint location, GLfloat v0, GLfloat v1);
void (APIENTRY *gglProgramUniform2fv)(GLuint program, GLint location, GLsizei count, const GLfloat *value);
void (APIENTRY *gglProgramUniform2d)(GLuint program, GLint location, GLdouble v0, GLdouble v1);
void (APIENTRY *gglProgramUniform2dv)(GLuint program, GLint location, GLsizei count, const GLdouble *value);
void (APIENTRY *gglProgramUniform2ui)(GLuint program, GLint location, GLuint v0, GLuint v1);
void (APIENTRY *gglProgramUniform2uiv)(GLuint program, GLint location, GLsizei count, const GLuint *value);
void (APIENTRY *gglProgramUniform3i)(GLuint program, GLint location, GLint v0, GLint v1, GLint v2);
void (APIENTRY *gglProgramUniform3iv)(GLuint program, GLint location, GLsizei count, const GLint *value);
void (APIENTRY *gglProgramUniform3f)(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
void (APIENTRY *gglProgramUniform3fv)(GLuint program, GLint location, GLsizei count, const GLfloat *value);
void (APIENTRY *gglProgramUniform3d)(GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2);
void (APIENTRY *gglProgramUniform3dv)(GLuint program, GLint location, GLsizei count, const GLdouble *value);
void (APIENTRY *gglProgramUniform3ui)(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2);
void (APIENTRY *gglProgramUniform3uiv)(GLuint program, GLint location, GLsizei count, const GLuint *value);
void (APIENTRY *gglProgramUniform4i)(GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
void (APIENTRY *gglProgramUniform4iv)(GLuint program, GLint location, GLsizei count, const GLint *value);
void (APIENTRY *gglProgramUniform4f)(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
void (APIENTRY *gglProgramUniform4fv)(GLuint program, GLint location, GLsizei count, const GLfloat *value);
void (APIENTRY *gglProgramUniform4d)(GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3);
void (APIENTRY *gglProgramUniform4dv)(GLuint program, GLint location, GLsizei count, const GLdouble *value);
void (APIENTRY *gglProgramUniform4ui)(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
void (APIENTRY *gglProgramUniform4uiv)(GLuint program, GLint location, GLsizei count, const GLuint *value);
void (APIENTRY *gglProgramUniformMatrix2fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void (APIENTRY *gglProgramUniformMatrix3fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void (APIENTRY *gglProgramUniformMatrix4fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void (APIENTRY *gglProgramUniformMatrix2dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
void (APIENTRY *gglProgramUniformMatrix3dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
void (APIENTRY *gglProgramUniformMatrix4dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
void (APIENTRY *gglProgramUniformMatrix2x3fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void (APIENTRY *gglProgramUniformMatrix3x2fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void (APIENTRY *gglProgramUniformMatrix2x4fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void (APIENTRY *gglProgramUniformMatrix4x2fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void (APIENTRY *gglProgramUniformMatrix3x4fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void (APIENTRY *gglProgramUniformMatrix4x3fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void (APIENTRY *gglProgramUniformMatrix2x3dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
void (APIENTRY *gglProgramUniformMatrix3x2dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
void (APIENTRY *gglProgramUniformMatrix2x4dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
void (APIENTRY *gglProgramUniformMatrix4x2dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
void (APIENTRY *gglProgramUniformMatrix3x4dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
void (APIENTRY *gglProgramUniformMatrix4x3dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
void (APIENTRY *gglValidateProgramPipeline)(GLuint pipeline);
void (APIENTRY *gglGetProgramPipelineInfoLog)(GLuint pipeline, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
void (APIENTRY *gglVertexAttribL1d)(GLuint index, GLdouble x);
void (APIENTRY *gglVertexAttribL2d)(GLuint index, GLdouble x, GLdouble y);
void (APIENTRY *gglVertexAttribL3d)(GLuint index, GLdouble x, GLdouble y, GLdouble z);
void (APIENTRY *gglVertexAttribL4d)(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void (APIENTRY *gglVertexAttribL1dv)(GLuint index, const GLdouble *v);
void (APIENTRY *gglVertexAttribL2dv)(GLuint index, const GLdouble *v);
void (APIENTRY *gglVertexAttribL3dv)(GLuint index, const GLdouble *v);
void (APIENTRY *gglVertexAttribL4dv)(GLuint index, const GLdouble *v);
void (APIENTRY *gglVertexAttribLPointer)(GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer);
void (APIENTRY *gglGetVertexAttribLdv)(GLuint index, GLenum pname, GLdouble *params);
void (APIENTRY *gglViewportArrayv)(GLuint first, GLsizei count, const GLfloat *v);
void (APIENTRY *gglViewportIndexedf)(GLuint index, GLfloat x, GLfloat y, GLfloat w, GLfloat h);
void (APIENTRY *gglViewportIndexedfv)(GLuint index, const GLfloat *v);
void (APIENTRY *gglScissorArrayv)(GLuint first, GLsizei count, const GLint *v);
void (APIENTRY *gglScissorIndexed)(GLuint index, GLint left, GLint bottom, GLsizei width, GLsizei height);
void (APIENTRY *gglScissorIndexedv)(GLuint index, const GLint *v);
void (APIENTRY *gglDepthRangeArrayv)(GLuint first, GLsizei count, const GLdouble *v);
void (APIENTRY *gglDepthRangeIndexed)(GLuint index, GLdouble n, GLdouble f);
void (APIENTRY *gglGetFloati_v)(GLenum target, GLuint index, GLfloat *data);
void (APIENTRY *gglGetDoublei_v)(GLenum target, GLuint index, GLdouble *data);

//GL_VERSION_4_2
void (APIENTRY *gglDrawArraysInstancedBaseInstance)(GLenum mode, GLint first, GLsizei count, GLsizei instancecount, GLuint baseinstance);
void (APIENTRY *gglDrawElementsInstancedBaseInstance)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLuint baseinstance);
void (APIENTRY *gglDrawElementsInstancedBaseVertexBaseInstance)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLint basevertex, GLuint baseinstance);
void (APIENTRY *gglGetInternalformativ)(GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint *params);
void (APIENTRY *gglGetActiveAtomicCounterBufferiv)(GLuint program, GLuint bufferIndex, GLenum pname, GLint *params);
void (APIENTRY *gglBindImageTexture)(GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format);
void (APIENTRY *gglMemoryBarrier)(GLbitfield barriers);
void (APIENTRY *gglTexStorage1D)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width);
void (APIENTRY *gglTexStorage2D)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
void (APIENTRY *gglTexStorage3D)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
void (APIENTRY *gglDrawTransformFeedbackInstanced)(GLenum mode, GLuint id, GLsizei instancecount);
void (APIENTRY *gglDrawTransformFeedbackStreamInstanced)(GLenum mode, GLuint id, GLuint stream, GLsizei instancecount);

//GL_VERSION_4_3
void (APIENTRY *gglClearBufferData)(GLenum target, GLenum internalformat, GLenum format, GLenum type, const void *data);
void (APIENTRY *gglClearBufferSubData)(GLenum target, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void *data);
void (APIENTRY *gglDispatchCompute)(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z);
void (APIENTRY *gglDispatchComputeIndirect)(GLintptr indirect);
void (APIENTRY *gglCopyImageSubData)(GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth);
void (APIENTRY *gglFramebufferParameteri)(GLenum target, GLenum pname, GLint param);
void (APIENTRY *gglGetFramebufferParameteriv)(GLenum target, GLenum pname, GLint *params);
void (APIENTRY *gglGetInternalformati64v)(GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint64 *params);
void (APIENTRY *gglInvalidateTexSubImage)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth);
void (APIENTRY *gglInvalidateTexImage)(GLuint texture, GLint level);
void (APIENTRY *gglInvalidateBufferSubData)(GLuint buffer, GLintptr offset, GLsizeiptr length);
void (APIENTRY *gglInvalidateBufferData)(GLuint buffer);
void (APIENTRY *gglInvalidateFramebuffer)(GLenum target, GLsizei numAttachments, const GLenum *attachments);
void (APIENTRY *gglInvalidateSubFramebuffer)(GLenum target, GLsizei numAttachments, const GLenum *attachments, GLint x, GLint y, GLsizei width, GLsizei height);
void (APIENTRY *gglMultiDrawArraysIndirect)(GLenum mode, const void *indirect, GLsizei drawcount, GLsizei stride);
void (APIENTRY *gglMultiDrawElementsIndirect)(GLenum mode, GLenum type, const void *indirect, GLsizei drawcount, GLsizei stride);
void (APIENTRY *gglGetProgramInterfaceiv)(GLuint program, GLenum programInterface, GLenum pname, GLint *params);
GLuint (APIENTRY *gglGetProgramResourceIndex)(GLuint program, GLenum programInterface, const GLchar *name);
void (APIENTRY *gglGetProgramResourceName)(GLuint program, GLenum programInterface, GLuint index, GLsizei bufSize, GLsizei *length, GLchar *name);
void (APIENTRY *gglGetProgramResourceiv)(GLuint program, GLenum programInterface, GLuint index, GLsizei propCount, const GLenum *props, GLsizei bufSize, GLsizei *length, GLint *params);
GLint (APIENTRY *gglGetProgramResourceLocation)(GLuint program, GLenum programInterface, const GLchar *name);
GLint (APIENTRY *gglGetProgramResourceLocationIndex)(GLuint program, GLenum programInterface, const GLchar *name);
void (APIENTRY *gglShaderStorageBlockBinding)(GLuint program, GLuint storageBlockIndex, GLuint storageBlockBinding);
void (APIENTRY *gglTexBufferRange)(GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);
void (APIENTRY *gglTexStorage2DMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
void (APIENTRY *gglTexStorage3DMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
void (APIENTRY *gglTextureView)(GLuint texture, GLenum target, GLuint origtexture, GLenum internalformat, GLuint minlevel, GLuint numlevels, GLuint minlayer, GLuint numlayers);
void (APIENTRY *gglBindVertexBuffer)(GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
void (APIENTRY *gglVertexAttribFormat)(GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
void (APIENTRY *gglVertexAttribIFormat)(GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
void (APIENTRY *gglVertexAttribLFormat)(GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
void (APIENTRY *gglVertexAttribBinding)(GLuint attribindex, GLuint bindingindex);
void (APIENTRY *gglVertexBindingDivisor)(GLuint bindingindex, GLuint divisor);
void (APIENTRY *gglDebugMessageControl)(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled);
void (APIENTRY *gglDebugMessageInsert)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *buf);
void (APIENTRY *gglDebugMessageCallback)(GLDEBUGPROC callback, const void *userParam);
GLuint (APIENTRY *gglGetDebugMessageLog)(GLuint count, GLsizei bufSize, GLenum *sources, GLenum *types, GLuint *ids, GLenum *severities, GLsizei *lengths, GLchar *messageLog);
void (APIENTRY *gglPushDebugGroup)(GLenum source, GLuint id, GLsizei length, const GLchar *message);
void (APIENTRY *gglPopDebugGroup)(void);
void (APIENTRY *gglObjectLabel)(GLenum identifier, GLuint name, GLsizei length, const GLchar *label);
void (APIENTRY *gglGetObjectLabel)(GLenum identifier, GLuint name, GLsizei bufSize, GLsizei *length, GLchar *label);
void (APIENTRY *gglObjectPtrLabel)(const void *ptr, GLsizei length, const GLchar *label);
void (APIENTRY *gglGetObjectPtrLabel)(const void *ptr, GLsizei bufSize, GLsizei *length, GLchar *label);

//GL_VERSION_4_4
void (APIENTRY *gglBufferStorage)(GLenum target, GLsizeiptr size, const void *data, GLbitfield flags);
void (APIENTRY *gglClearTexImage)(GLuint texture, GLint level, GLenum format, GLenum type, const void *data);
void (APIENTRY *gglClearTexSubImage)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *data);
void (APIENTRY *gglBindBuffersBase)(GLenum target, GLuint first, GLsizei count, const GLuint *buffers);
void (APIENTRY *gglBindBuffersRange)(GLenum target, GLuint first, GLsizei count, const GLuint *buffers, const GLintptr *offsets, const GLsizeiptr *sizes);
void (APIENTRY *gglBindTextures)(GLuint first, GLsizei count, const GLuint *textures);
void (APIENTRY *gglBindSamplers)(GLuint first, GLsizei count, const GLuint *samplers);
void (APIENTRY *gglBindImageTextures)(GLuint first, GLsizei count, const GLuint *textures);
void (APIENTRY *gglBindVertexBuffers)(GLuint first, GLsizei count, const GLuint *buffers, const GLintptr *offsets, const GLsizei *strides);

//GL_VERSION_4_5
void (APIENTRY *gglClipControl)(GLenum origin, GLenum depth);
void (APIENTRY *gglCreateTransformFeedbacks)(GLsizei n, GLuint *ids);
void (APIENTRY *gglTransformFeedbackBufferBase)(GLuint xfb, GLuint index, GLuint buffer);
void (APIENTRY *gglTransformFeedbackBufferRange)(GLuint xfb, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
void (APIENTRY *gglGetTransformFeedbackiv)(GLuint xfb, GLenum pname, GLint *param);
void (APIENTRY *gglGetTransformFeedbacki_v)(GLuint xfb, GLenum pname, GLuint index, GLint *param);
void (APIENTRY *gglGetTransformFeedbacki64_v)(GLuint xfb, GLenum pname, GLuint index, GLint64 *param);
void (APIENTRY *gglCreateBuffers)(GLsizei n, GLuint *buffers);
void (APIENTRY *gglNamedBufferStorage)(GLuint buffer, GLsizeiptr size, const void *data, GLbitfield flags);
void (APIENTRY *gglNamedBufferData)(GLuint buffer, GLsizeiptr size, const void *data, GLenum usage);
void (APIENTRY *gglNamedBufferSubData)(GLuint buffer, GLintptr offset, GLsizeiptr size, const void *data);
void (APIENTRY *gglCopyNamedBufferSubData)(GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
void (APIENTRY *gglClearNamedBufferData)(GLuint buffer, GLenum internalformat, GLenum format, GLenum type, const void *data);
void (APIENTRY *gglClearNamedBufferSubData)(GLuint buffer, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void *data);
void *(APIENTRY *gglMapNamedBuffer)(GLuint buffer, GLenum access);
void *(APIENTRY *gglMapNamedBufferRange)(GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access);
GLboolean (APIENTRY *gglUnmapNamedBuffer)(GLuint buffer);
void (APIENTRY *gglFlushMappedNamedBufferRange)(GLuint buffer, GLintptr offset, GLsizeiptr length);
void (APIENTRY *gglGetNamedBufferParameteriv)(GLuint buffer, GLenum pname, GLint *params);
void (APIENTRY *gglGetNamedBufferParameteri64v)(GLuint buffer, GLenum pname, GLint64 *params);
void (APIENTRY *gglGetNamedBufferPointerv)(GLuint buffer, GLenum pname, void **params);
void (APIENTRY *gglGetNamedBufferSubData)(GLuint buffer, GLintptr offset, GLsizeiptr size, void *data);
void (APIENTRY *gglCreateFramebuffers)(GLsizei n, GLuint *framebuffers);
void (APIENTRY *gglNamedFramebufferRenderbuffer)(GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
void (APIENTRY *gglNamedFramebufferParameteri)(GLuint framebuffer, GLenum pname, GLint param);
void (APIENTRY *gglNamedFramebufferTexture)(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level);
void (APIENTRY *gglNamedFramebufferTextureLayer)(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint layer);
void (APIENTRY *gglNamedFramebufferDrawBuffer)(GLuint framebuffer, GLenum buf);
void (APIENTRY *gglNamedFramebufferDrawBuffers)(GLuint framebuffer, GLsizei n, const GLenum *bufs);
void (APIENTRY *gglNamedFramebufferReadBuffer)(GLuint framebuffer, GLenum src);
void (APIENTRY *gglInvalidateNamedFramebufferData)(GLuint framebuffer, GLsizei numAttachments, const GLenum *attachments);
void (APIENTRY *gglInvalidateNamedFramebufferSubData)(GLuint framebuffer, GLsizei numAttachments, const GLenum *attachments, GLint x, GLint y, GLsizei width, GLsizei height);
void (APIENTRY *gglClearNamedFramebufferiv)(GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLint *value);
void (APIENTRY *gglClearNamedFramebufferuiv)(GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLuint *value);
void (APIENTRY *gglClearNamedFramebufferfv)(GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLfloat *value);
void (APIENTRY *gglClearNamedFramebufferfi)(GLuint framebuffer, GLenum buffer, const GLfloat depth, GLint stencil);
void (APIENTRY *gglBlitNamedFramebuffer)(GLuint readFramebuffer, GLuint drawFramebuffer, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
GLenum (APIENTRY *gglCheckNamedFramebufferStatus)(GLuint framebuffer, GLenum target);
void (APIENTRY *gglGetNamedFramebufferParameteriv)(GLuint framebuffer, GLenum pname, GLint *param);
void (APIENTRY *gglGetNamedFramebufferAttachmentParameteriv)(GLuint framebuffer, GLenum attachment, GLenum pname, GLint *params);
void (APIENTRY *gglCreateRenderbuffers)(GLsizei n, GLuint *renderbuffers);
void (APIENTRY *gglNamedRenderbufferStorage)(GLuint renderbuffer, GLenum internalformat, GLsizei width, GLsizei height);
void (APIENTRY *gglNamedRenderbufferStorageMultisample)(GLuint renderbuffer, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
void (APIENTRY *gglGetNamedRenderbufferParameteriv)(GLuint renderbuffer, GLenum pname, GLint *params);
void (APIENTRY *gglCreateTextures)(GLenum target, GLsizei n, GLuint *textures);
void (APIENTRY *gglTextureBuffer)(GLuint texture, GLenum internalformat, GLuint buffer);
void (APIENTRY *gglTextureBufferRange)(GLuint texture, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);
void (APIENTRY *gglTextureStorage1D)(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width);
void (APIENTRY *gglTextureStorage2D)(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
void (APIENTRY *gglTextureStorage3D)(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
void (APIENTRY *gglTextureStorage2DMultisample)(GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
void (APIENTRY *gglTextureStorage3DMultisample)(GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
void (APIENTRY *gglTextureSubImage1D)(GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels);
void (APIENTRY *gglTextureSubImage2D)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
void (APIENTRY *gglTextureSubImage3D)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels);
void (APIENTRY *gglCompressedTextureSubImage1D)(GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void *data);
void (APIENTRY *gglCompressedTextureSubImage2D)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data);
void (APIENTRY *gglCompressedTextureSubImage3D)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data);
void (APIENTRY *gglCopyTextureSubImage1D)(GLuint texture, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
void (APIENTRY *gglCopyTextureSubImage2D)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
void (APIENTRY *gglCopyTextureSubImage3D)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
void (APIENTRY *gglTextureParameterf)(GLuint texture, GLenum pname, GLfloat param);
void (APIENTRY *gglTextureParameterfv)(GLuint texture, GLenum pname, const GLfloat *param);
void (APIENTRY *gglTextureParameteri)(GLuint texture, GLenum pname, GLint param);
void (APIENTRY *gglTextureParameterIiv)(GLuint texture, GLenum pname, const GLint *params);
void (APIENTRY *gglTextureParameterIuiv)(GLuint texture, GLenum pname, const GLuint *params);
void (APIENTRY *gglTextureParameteriv)(GLuint texture, GLenum pname, const GLint *param);
void (APIENTRY *gglGenerateTextureMipmap)(GLuint texture);
void (APIENTRY *gglBindTextureUnit)(GLuint unit, GLuint texture);
void (APIENTRY *gglGetTextureImage)(GLuint texture, GLint level, GLenum format, GLenum type, GLsizei bufSize, void *pixels);
void (APIENTRY *gglGetCompressedTextureImage)(GLuint texture, GLint level, GLsizei bufSize, void *pixels);
void (APIENTRY *gglGetTextureLevelParameterfv)(GLuint texture, GLint level, GLenum pname, GLfloat *params);
void (APIENTRY *gglGetTextureLevelParameteriv)(GLuint texture, GLint level, GLenum pname, GLint *params);
void (APIENTRY *gglGetTextureParameterfv)(GLuint texture, GLenum pname, GLfloat *params);
void (APIENTRY *gglGetTextureParameterIiv)(GLuint texture, GLenum pname, GLint *params);
void (APIENTRY *gglGetTextureParameterIuiv)(GLuint texture, GLenum pname, GLuint *params);
void (APIENTRY *gglGetTextureParameteriv)(GLuint texture, GLenum pname, GLint *params);
void (APIENTRY *gglCreateVertexArrays)(GLsizei n, GLuint *arrays);
void (APIENTRY *gglDisableVertexArrayAttrib)(GLuint vaobj, GLuint index);
void (APIENTRY *gglEnableVertexArrayAttrib)(GLuint vaobj, GLuint index);
void (APIENTRY *gglVertexArrayElementBuffer)(GLuint vaobj, GLuint buffer);
void (APIENTRY *gglVertexArrayVertexBuffer)(GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
void (APIENTRY *gglVertexArrayVertexBuffers)(GLuint vaobj, GLuint first, GLsizei count, const GLuint *buffers, const GLintptr *offsets, const GLsizei *strides);
void (APIENTRY *gglVertexArrayAttribBinding)(GLuint vaobj, GLuint attribindex, GLuint bindingindex);
void (APIENTRY *gglVertexArrayAttribFormat)(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
void (APIENTRY *gglVertexArrayAttribIFormat)(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
void (APIENTRY *gglVertexArrayAttribLFormat)(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
void (APIENTRY *gglVertexArrayBindingDivisor)(GLuint vaobj, GLuint bindingindex, GLuint divisor);
void (APIENTRY *gglGetVertexArrayiv)(GLuint vaobj, GLenum pname, GLint *param);
void (APIENTRY *gglGetVertexArrayIndexediv)(GLuint vaobj, GLuint index, GLenum pname, GLint *param);
void (APIENTRY *gglGetVertexArrayIndexed64iv)(GLuint vaobj, GLuint index, GLenum pname, GLint64 *param);
void (APIENTRY *gglCreateSamplers)(GLsizei n, GLuint *samplers);
void (APIENTRY *gglCreateProgramPipelines)(GLsizei n, GLuint *pipelines);
void (APIENTRY *gglCreateQueries)(GLenum target, GLsizei n, GLuint *ids);
void (APIENTRY *gglGetQueryBufferObjecti64v)(GLuint id, GLuint buffer, GLenum pname, GLintptr offset);
void (APIENTRY *gglGetQueryBufferObjectiv)(GLuint id, GLuint buffer, GLenum pname, GLintptr offset);
void (APIENTRY *gglGetQueryBufferObjectui64v)(GLuint id, GLuint buffer, GLenum pname, GLintptr offset);
void (APIENTRY *gglGetQueryBufferObjectuiv)(GLuint id, GLuint buffer, GLenum pname, GLintptr offset);
void (APIENTRY *gglMemoryBarrierByRegion)(GLbitfield barriers);
void (APIENTRY *gglGetTextureSubImage)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, GLsizei bufSize, void *pixels);
void (APIENTRY *gglGetCompressedTextureSubImage)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLsizei bufSize, void *pixels);
GLenum (APIENTRY *gglGetGraphicsResetStatus)(void);
void (APIENTRY *gglGetnCompressedTexImage)(GLenum target, GLint lod, GLsizei bufSize, void *pixels);
void (APIENTRY *gglGetnTexImage)(GLenum target, GLint level, GLenum format, GLenum type, GLsizei bufSize, void *pixels);
void (APIENTRY *gglGetnUniformdv)(GLuint program, GLint location, GLsizei bufSize, GLdouble *params);
void (APIENTRY *gglGetnUniformfv)(GLuint program, GLint location, GLsizei bufSize, GLfloat *params);
void (APIENTRY *gglGetnUniformiv)(GLuint program, GLint location, GLsizei bufSize, GLint *params);
void (APIENTRY *gglGetnUniformuiv)(GLuint program, GLint location, GLsizei bufSize, GLuint *params);
void (APIENTRY *gglReadnPixels)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void *data);
void (APIENTRY *gglGetnMapdv)(GLenum target, GLenum query, GLsizei bufSize, GLdouble *v);
void (APIENTRY *gglGetnMapfv)(GLenum target, GLenum query, GLsizei bufSize, GLfloat *v);
void (APIENTRY *gglGetnMapiv)(GLenum target, GLenum query, GLsizei bufSize, GLint *v);
void (APIENTRY *gglGetnPixelMapfv)(GLenum map, GLsizei bufSize, GLfloat *values);
void (APIENTRY *gglGetnPixelMapuiv)(GLenum map, GLsizei bufSize, GLuint *values);
void (APIENTRY *gglGetnPixelMapusv)(GLenum map, GLsizei bufSize, GLushort *values);
void (APIENTRY *gglGetnPolygonStipple)(GLsizei bufSize, GLubyte *pattern);
void (APIENTRY *gglGetnColorTable)(GLenum target, GLenum format, GLenum type, GLsizei bufSize, void *table);
void (APIENTRY *gglGetnConvolutionFilter)(GLenum target, GLenum format, GLenum type, GLsizei bufSize, void *image);
void (APIENTRY *gglGetnSeparableFilter)(GLenum target, GLenum format, GLenum type, GLsizei rowBufSize, void *row, GLsizei columnBufSize, void *column, void *span);
void (APIENTRY *gglGetnHistogram)(GLenum target, GLboolean reset, GLenum format, GLenum type, GLsizei bufSize, void *values);
void (APIENTRY *gglGetnMinmax)(GLenum target, GLboolean reset, GLenum format, GLenum type, GLsizei bufSize, void *values);
void (APIENTRY *gglTextureBarrier)(void);

//GL_STANDARD
void (APIENTRY *gglAccum)(GLenum op, GLfloat value);
void (APIENTRY *gglAlphaFunc)(GLenum func, GLclampf ref);
void (APIENTRY *gglBegin)(GLenum mode);
void (APIENTRY *gglBitmap)(GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte *bitmap);
void (APIENTRY *gglBlendFunc)(GLenum sfactor, GLenum dfactor);
void (APIENTRY *gglCallList)(GLuint list);
void (APIENTRY *gglCallLists)(GLsizei n, GLenum type, const GLvoid *lists);
void (APIENTRY *gglClear)(GLbitfield mask);
void (APIENTRY *gglClearAccum)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
void (APIENTRY *gglClearColor)(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
void (APIENTRY *gglClearDepth)(GLclampd depth);
void (APIENTRY *gglClearIndex)(GLfloat c);
void (APIENTRY *gglClearStencil)(GLint s);
void (APIENTRY *gglClipPlane)(GLenum plane, const GLdouble *equation);
void (APIENTRY *gglColor3b)(GLbyte red, GLbyte green, GLbyte blue);
void (APIENTRY *gglColor3bv)(const GLbyte *v);
void (APIENTRY *gglColor3d)(GLdouble red, GLdouble green, GLdouble blue);
void (APIENTRY *gglColor3dv)(const GLdouble *v);
void (APIENTRY *gglColor3f)(GLfloat red, GLfloat green, GLfloat blue);
void (APIENTRY *gglColor3fv)(const GLfloat *v);
void (APIENTRY *gglColor3i)(GLint red, GLint green, GLint blue);
void (APIENTRY *gglColor3iv)(const GLint *v);
void (APIENTRY *gglColor3s)(GLshort red, GLshort green, GLshort blue);
void (APIENTRY *gglColor3sv)(const GLshort *v);
void (APIENTRY *gglColor3ub)(GLubyte red, GLubyte green, GLubyte blue);
void (APIENTRY *gglColor3ubv)(const GLubyte *v);
void (APIENTRY *gglColor3ui)(GLuint red, GLuint green, GLuint blue);
void (APIENTRY *gglColor3uiv)(const GLuint *v);
void (APIENTRY *gglColor3us)(GLushort red, GLushort green, GLushort blue);
void (APIENTRY *gglColor3usv)(const GLushort *v);
void (APIENTRY *gglColor4b)(GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha);
void (APIENTRY *gglColor4bv)(const GLbyte *v);
void (APIENTRY *gglColor4d)(GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha);
void (APIENTRY *gglColor4dv)(const GLdouble *v);
void (APIENTRY *gglColor4f)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
void (APIENTRY *gglColor4fv)(const GLfloat *v);
void (APIENTRY *gglColor4i)(GLint red, GLint green, GLint blue, GLint alpha);
void (APIENTRY *gglColor4iv)(const GLint *v);
void (APIENTRY *gglColor4s)(GLshort red, GLshort green, GLshort blue, GLshort alpha);
void (APIENTRY *gglColor4sv)(const GLshort *v);
void (APIENTRY *gglColor4ub)(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
void (APIENTRY *gglColor4ubv)(const GLubyte *v);
void (APIENTRY *gglColor4ui)(GLuint red, GLuint green, GLuint blue, GLuint alpha);
void (APIENTRY *gglColor4uiv)(const GLuint *v);
void (APIENTRY *gglColor4us)(GLushort red, GLushort green, GLushort blue, GLushort alpha);
void (APIENTRY *gglColor4usv)(const GLushort *v);
void (APIENTRY *gglColorMask)(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
void (APIENTRY *gglColorMaterial)(GLenum face, GLenum mode);
void (APIENTRY *gglCopyPixels)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum type);
void (APIENTRY *gglCullFace)(GLenum mode);
void (APIENTRY *gglDeleteLists)(GLuint list, GLsizei range);
void (APIENTRY *gglDepthFunc)(GLenum func);
void (APIENTRY *gglDepthMask)(GLboolean flag);
void (APIENTRY *gglDepthRange)(GLclampd zNear, GLclampd zFar);
void (APIENTRY *gglDisable)(GLenum cap);
void (APIENTRY *gglDrawBuffer)(GLenum mode);
void (APIENTRY *gglDrawPixels)(GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
void (APIENTRY *gglEdgeFlag)(GLboolean flag);
void (APIENTRY *gglEdgeFlagv)(const GLboolean *flag);
void (APIENTRY *gglEnable)(GLenum cap);
void (APIENTRY *gglEnd)(void);
void (APIENTRY *gglEndList)(void);
void (APIENTRY *gglEvalCoord1d)(GLdouble u);
void (APIENTRY *gglEvalCoord1dv)(const GLdouble *u);
void (APIENTRY *gglEvalCoord1f)(GLfloat u);
void (APIENTRY *gglEvalCoord1fv)(const GLfloat *u);
void (APIENTRY *gglEvalCoord2d)(GLdouble u, GLdouble v);
void (APIENTRY *gglEvalCoord2dv)(const GLdouble *u);
void (APIENTRY *gglEvalCoord2f)(GLfloat u, GLfloat v);
void (APIENTRY *gglEvalCoord2fv)(const GLfloat *u);
void (APIENTRY *gglEvalMesh1)(GLenum mode, GLint i1, GLint i2);
void (APIENTRY *gglEvalMesh2)(GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2);
void (APIENTRY *gglEvalPoint1)(GLint i);
void (APIENTRY *gglEvalPoint2)(GLint i, GLint j);
void (APIENTRY *gglFeedbackBuffer)(GLsizei size, GLenum type, GLfloat *buffer);
void (APIENTRY *gglFinish)(void);
void (APIENTRY *gglFlush)(void);
void (APIENTRY *gglFogf)(GLenum pname, GLfloat param);
void (APIENTRY *gglFogfv)(GLenum pname, const GLfloat *params);
void (APIENTRY *gglFogi)(GLenum pname, GLint param);
void (APIENTRY *gglFogiv)(GLenum pname, const GLint *params);
void (APIENTRY *gglFrontFace)(GLenum mode);
void (APIENTRY *gglFrustum)(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
GLuint (APIENTRY *gglGenLists)(GLsizei range);
void (APIENTRY *gglGetBooleanv)(GLenum pname, GLboolean *params);
void (APIENTRY *gglGetClipPlane)(GLenum plane, GLdouble *equation);
void (APIENTRY *gglGetDoublev)(GLenum pname, GLdouble *params);
GLenum (APIENTRY *gglGetError)(void);
void (APIENTRY *gglGetFloatv)(GLenum pname, GLfloat *params);
void (APIENTRY *gglGetIntegerv)(GLenum pname, GLint *params);
void (APIENTRY *gglGetLightfv)(GLenum light, GLenum pname, GLfloat *params);
void (APIENTRY *gglGetLightiv)(GLenum light, GLenum pname, GLint *params);
void (APIENTRY *gglGetMapdv)(GLenum target, GLenum query, GLdouble *v);
void (APIENTRY *gglGetMapfv)(GLenum target, GLenum query, GLfloat *v);
void (APIENTRY *gglGetMapiv)(GLenum target, GLenum query, GLint *v);
void (APIENTRY *gglGetMaterialfv)(GLenum face, GLenum pname, GLfloat *params);
void (APIENTRY *gglGetMaterialiv)(GLenum face, GLenum pname, GLint *params);
void (APIENTRY *gglGetPixelMapfv)(GLenum map, GLfloat *values);
void (APIENTRY *gglGetPixelMapuiv)(GLenum map, GLuint *values);
void (APIENTRY *gglGetPixelMapusv)(GLenum map, GLushort *values);
void (APIENTRY *gglGetPolygonStipple)(GLubyte *mask);
const GLubyte *(APIENTRY *gglGetString)(GLenum name);
void (APIENTRY *gglGetTexEnvfv)(GLenum target, GLenum pname, GLfloat *params);
void (APIENTRY *gglGetTexEnviv)(GLenum target, GLenum pname, GLint *params);
void (APIENTRY *gglGetTexGendv)(GLenum coord, GLenum pname, GLdouble *params);
void (APIENTRY *gglGetTexGenfv)(GLenum coord, GLenum pname, GLfloat *params);
void (APIENTRY *gglGetTexGeniv)(GLenum coord, GLenum pname, GLint *params);
void (APIENTRY *gglGetTexImage)(GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels);
void (APIENTRY *gglGetTexLevelParameterfv)(GLenum target, GLint level, GLenum pname, GLfloat *params);
void (APIENTRY *gglGetTexLevelParameteriv)(GLenum target, GLint level, GLenum pname, GLint *params);
void (APIENTRY *gglGetTexParameterfv)(GLenum target, GLenum pname, GLfloat *params);
void (APIENTRY *gglGetTexParameteriv)(GLenum target, GLenum pname, GLint *params);
void (APIENTRY *gglHint)(GLenum target, GLenum mode);
void (APIENTRY *gglIndexMask)(GLuint mask);
void (APIENTRY *gglIndexd)(GLdouble c);
void (APIENTRY *gglIndexdv)(const GLdouble *c);
void (APIENTRY *gglIndexf)(GLfloat c);
void (APIENTRY *gglIndexfv)(const GLfloat *c);
void (APIENTRY *gglIndexi)(GLint c);
void (APIENTRY *gglIndexiv)(const GLint *c);
void (APIENTRY *gglIndexs)(GLshort c);
void (APIENTRY *gglIndexsv)(const GLshort *c);
void (APIENTRY *gglInitNames)(void);
GLboolean (APIENTRY *gglIsEnabled)(GLenum cap);
GLboolean (APIENTRY *gglIsList)(GLuint list);
void (APIENTRY *gglLightModelf)(GLenum pname, GLfloat param);
void (APIENTRY *gglLightModelfv)(GLenum pname, const GLfloat *params);
void (APIENTRY *gglLightModeli)(GLenum pname, GLint param);
void (APIENTRY *gglLightModeliv)(GLenum pname, const GLint *params);
void (APIENTRY *gglLightf)(GLenum light, GLenum pname, GLfloat param);
void (APIENTRY *gglLightfv)(GLenum light, GLenum pname, const GLfloat *params);
void (APIENTRY *gglLighti)(GLenum light, GLenum pname, GLint param);
void (APIENTRY *gglLightiv)(GLenum light, GLenum pname, const GLint *params);
void (APIENTRY *gglLineStipple)(GLint factor, GLushort pattern);
void (APIENTRY *gglLineWidth)(GLfloat width);
void (APIENTRY *gglListBase)(GLuint base);
void (APIENTRY *gglLoadIdentity)(void);
void (APIENTRY *gglLoadMatrixd)(const GLdouble *m);
void (APIENTRY *gglLoadMatrixf)(const GLfloat *m);
void (APIENTRY *gglLoadName)(GLuint name);
void (APIENTRY *gglLogicOp)(GLenum opcode);
void (APIENTRY *gglMap1d)(GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble *points);
void (APIENTRY *gglMap1f)(GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points);
void (APIENTRY *gglMap2d)(GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble *points);
void (APIENTRY *gglMap2f)(GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat *points);
void (APIENTRY *gglMapGrid1d)(GLint un, GLdouble u1, GLdouble u2);
void (APIENTRY *gglMapGrid1f)(GLint un, GLfloat u1, GLfloat u2);
void (APIENTRY *gglMapGrid2d)(GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2);
void (APIENTRY *gglMapGrid2f)(GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2);
void (APIENTRY *gglMaterialf)(GLenum face, GLenum pname, GLfloat param);
void (APIENTRY *gglMaterialfv)(GLenum face, GLenum pname, const GLfloat *params);
void (APIENTRY *gglMateriali)(GLenum face, GLenum pname, GLint param);
void (APIENTRY *gglMaterialiv)(GLenum face, GLenum pname, const GLint *params);
void (APIENTRY *gglMatrixMode)(GLenum mode);
void (APIENTRY *gglMultMatrixd)(const GLdouble *m);
void (APIENTRY *gglMultMatrixf)(const GLfloat *m);
void (APIENTRY *gglNewList)(GLuint list, GLenum mode);
void (APIENTRY *gglNormal3b)(GLbyte nx, GLbyte ny, GLbyte nz);
void (APIENTRY *gglNormal3bv)(const GLbyte *v);
void (APIENTRY *gglNormal3d)(GLdouble nx, GLdouble ny, GLdouble nz);
void (APIENTRY *gglNormal3dv)(const GLdouble *v);
void (APIENTRY *gglNormal3f)(GLfloat nx, GLfloat ny, GLfloat nz);
void (APIENTRY *gglNormal3fv)(const GLfloat *v);
void (APIENTRY *gglNormal3i)(GLint nx, GLint ny, GLint nz);
void (APIENTRY *gglNormal3iv)(const GLint *v);
void (APIENTRY *gglNormal3s)(GLshort nx, GLshort ny, GLshort nz);
void (APIENTRY *gglNormal3sv)(const GLshort *v);
void (APIENTRY *gglOrtho)(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
void (APIENTRY *gglPassThrough)(GLfloat token);
void (APIENTRY *gglPixelMapfv)(GLenum map, GLint mapsize, const GLfloat *values);
void (APIENTRY *gglPixelMapuiv)(GLenum map, GLint mapsize, const GLuint *values);
void (APIENTRY *gglPixelMapusv)(GLenum map, GLint mapsize, const GLushort *values);
void (APIENTRY *gglPixelStoref)(GLenum pname, GLfloat param);
void (APIENTRY *gglPixelStorei)(GLenum pname, GLint param);
void (APIENTRY *gglPixelTransferf)(GLenum pname, GLfloat param);
void (APIENTRY *gglPixelTransferi)(GLenum pname, GLint param);
void (APIENTRY *gglPixelZoom)(GLfloat xfactor, GLfloat yfactor);
void (APIENTRY *gglPointSize)(GLfloat size);
void (APIENTRY *gglPolygonMode)(GLenum face, GLenum mode);
void (APIENTRY *gglPolygonStipple)(const GLubyte *mask);
void (APIENTRY *gglPopAttrib)(void);
void (APIENTRY *gglPopMatrix)(void);
void (APIENTRY *gglPopName)(void);
void (APIENTRY *gglPushAttrib)(GLbitfield mask);
void (APIENTRY *gglPushMatrix)(void);
void (APIENTRY *gglPushName)(GLuint name);
void (APIENTRY *gglRasterPos2d)(GLdouble x, GLdouble y);
void (APIENTRY *gglRasterPos2dv)(const GLdouble *v);
void (APIENTRY *gglRasterPos2f)(GLfloat x, GLfloat y);
void (APIENTRY *gglRasterPos2fv)(const GLfloat *v);
void (APIENTRY *gglRasterPos2i)(GLint x, GLint y);
void (APIENTRY *gglRasterPos2iv)(const GLint *v);
void (APIENTRY *gglRasterPos2s)(GLshort x, GLshort y);
void (APIENTRY *gglRasterPos2sv)(const GLshort *v);
void (APIENTRY *gglRasterPos3d)(GLdouble x, GLdouble y, GLdouble z);
void (APIENTRY *gglRasterPos3dv)(const GLdouble *v);
void (APIENTRY *gglRasterPos3f)(GLfloat x, GLfloat y, GLfloat z);
void (APIENTRY *gglRasterPos3fv)(const GLfloat *v);
void (APIENTRY *gglRasterPos3i)(GLint x, GLint y, GLint z);
void (APIENTRY *gglRasterPos3iv)(const GLint *v);
void (APIENTRY *gglRasterPos3s)(GLshort x, GLshort y, GLshort z);
void (APIENTRY *gglRasterPos3sv)(const GLshort *v);
void (APIENTRY *gglRasterPos4d)(GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void (APIENTRY *gglRasterPos4dv)(const GLdouble *v);
void (APIENTRY *gglRasterPos4f)(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void (APIENTRY *gglRasterPos4fv)(const GLfloat *v);
void (APIENTRY *gglRasterPos4i)(GLint x, GLint y, GLint z, GLint w);
void (APIENTRY *gglRasterPos4iv)(const GLint *v);
void (APIENTRY *gglRasterPos4s)(GLshort x, GLshort y, GLshort z, GLshort w);
void (APIENTRY *gglRasterPos4sv)(const GLshort *v);
void (APIENTRY *gglReadBuffer)(GLenum mode);
void (APIENTRY *gglReadPixels)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
void (APIENTRY *gglRectd)(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2);
void (APIENTRY *gglRectdv)(const GLdouble *v1, const GLdouble *v2);
void (APIENTRY *gglRectf)(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
void (APIENTRY *gglRectfv)(const GLfloat *v1, const GLfloat *v2);
void (APIENTRY *gglRecti)(GLint x1, GLint y1, GLint x2, GLint y2);
void (APIENTRY *gglRectiv)(const GLint *v1, const GLint *v2);
void (APIENTRY *gglRects)(GLshort x1, GLshort y1, GLshort x2, GLshort y2);
void (APIENTRY *gglRectsv)(const GLshort *v1, const GLshort *v2);
GLint (APIENTRY *gglRenderMode)(GLenum mode);
void (APIENTRY *gglRotated)(GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
void (APIENTRY *gglRotatef)(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
void (APIENTRY *gglScaled)(GLdouble x, GLdouble y, GLdouble z);
void (APIENTRY *gglScalef)(GLfloat x, GLfloat y, GLfloat z);
void (APIENTRY *gglScissor)(GLint x, GLint y, GLsizei width, GLsizei height);
void (APIENTRY *gglSelectBuffer)(GLsizei size, GLuint *buffer);
void (APIENTRY *gglShadeModel)(GLenum mode);
void (APIENTRY *gglStencilFunc)(GLenum func, GLint ref, GLuint mask);
void (APIENTRY *gglStencilMask)(GLuint mask);
void (APIENTRY *gglStencilOp)(GLenum fail, GLenum zfail, GLenum zpass);
void (APIENTRY *gglTexCoord1d)(GLdouble s);
void (APIENTRY *gglTexCoord1dv)(const GLdouble *v);
void (APIENTRY *gglTexCoord1f)(GLfloat s);
void (APIENTRY *gglTexCoord1fv)(const GLfloat *v);
void (APIENTRY *gglTexCoord1i)(GLint s);
void (APIENTRY *gglTexCoord1iv)(const GLint *v);
void (APIENTRY *gglTexCoord1s)(GLshort s);
void (APIENTRY *gglTexCoord1sv)(const GLshort *v);
void (APIENTRY *gglTexCoord2d)(GLdouble s, GLdouble t);
void (APIENTRY *gglTexCoord2dv)(const GLdouble *v);
void (APIENTRY *gglTexCoord2f)(GLfloat s, GLfloat t);
void (APIENTRY *gglTexCoord2fv)(const GLfloat *v);
void (APIENTRY *gglTexCoord2i)(GLint s, GLint t);
void (APIENTRY *gglTexCoord2iv)(const GLint *v);
void (APIENTRY *gglTexCoord2s)(GLshort s, GLshort t);
void (APIENTRY *gglTexCoord2sv)(const GLshort *v);
void (APIENTRY *gglTexCoord3d)(GLdouble s, GLdouble t, GLdouble r);
void (APIENTRY *gglTexCoord3dv)(const GLdouble *v);
void (APIENTRY *gglTexCoord3f)(GLfloat s, GLfloat t, GLfloat r);
void (APIENTRY *gglTexCoord3fv)(const GLfloat *v);
void (APIENTRY *gglTexCoord3i)(GLint s, GLint t, GLint r);
void (APIENTRY *gglTexCoord3iv)(const GLint *v);
void (APIENTRY *gglTexCoord3s)(GLshort s, GLshort t, GLshort r);
void (APIENTRY *gglTexCoord3sv)(const GLshort *v);
void (APIENTRY *gglTexCoord4d)(GLdouble s, GLdouble t, GLdouble r, GLdouble q);
void (APIENTRY *gglTexCoord4dv)(const GLdouble *v);
void (APIENTRY *gglTexCoord4f)(GLfloat s, GLfloat t, GLfloat r, GLfloat q);
void (APIENTRY *gglTexCoord4fv)(const GLfloat *v);
void (APIENTRY *gglTexCoord4i)(GLint s, GLint t, GLint r, GLint q);
void (APIENTRY *gglTexCoord4iv)(const GLint *v);
void (APIENTRY *gglTexCoord4s)(GLshort s, GLshort t, GLshort r, GLshort q);
void (APIENTRY *gglTexCoord4sv)(const GLshort *v);
void (APIENTRY *gglTexEnvf)(GLenum target, GLenum pname, GLfloat param);
void (APIENTRY *gglTexEnvfv)(GLenum target, GLenum pname, const GLfloat *params);
void (APIENTRY *gglTexEnvi)(GLenum target, GLenum pname, GLint param);
void (APIENTRY *gglTexEnviv)(GLenum target, GLenum pname, const GLint *params);
void (APIENTRY *gglTexGend)(GLenum coord, GLenum pname, GLdouble param);
void (APIENTRY *gglTexGendv)(GLenum coord, GLenum pname, const GLdouble *params);
void (APIENTRY *gglTexGenf)(GLenum coord, GLenum pname, GLfloat param);
void (APIENTRY *gglTexGenfv)(GLenum coord, GLenum pname, const GLfloat *params);
void (APIENTRY *gglTexGeni)(GLenum coord, GLenum pname, GLint param);
void (APIENTRY *gglTexGeniv)(GLenum coord, GLenum pname, const GLint *params);
void (APIENTRY *gglTexImage1D)(GLenum target, GLint level, GLint components, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
void (APIENTRY *gglTexImage2D)(GLenum target, GLint level, GLint components, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
void (APIENTRY *gglTexParameterf)(GLenum target, GLenum pname, GLfloat param);
void (APIENTRY *gglTexParameterfv)(GLenum target, GLenum pname, const GLfloat *params);
void (APIENTRY *gglTexParameteri)(GLenum target, GLenum pname, GLint param);
void (APIENTRY *gglTexParameteriv)(GLenum target, GLenum pname, const GLint *params);
void (APIENTRY *gglTranslated)(GLdouble x, GLdouble y, GLdouble z);
void (APIENTRY *gglTranslatef)(GLfloat x, GLfloat y, GLfloat z);
void (APIENTRY *gglVertex2d)(GLdouble x, GLdouble y);
void (APIENTRY *gglVertex2dv)(const GLdouble *v);
void (APIENTRY *gglVertex2f)(GLfloat x, GLfloat y);
void (APIENTRY *gglVertex2fv)(const GLfloat *v);
void (APIENTRY *gglVertex2i)(GLint x, GLint y);
void (APIENTRY *gglVertex2iv)(const GLint *v);
void (APIENTRY *gglVertex2s)(GLshort x, GLshort y);
void (APIENTRY *gglVertex2sv)(const GLshort *v);
void (APIENTRY *gglVertex3d)(GLdouble x, GLdouble y, GLdouble z);
void (APIENTRY *gglVertex3dv)(const GLdouble *v);
void (APIENTRY *gglVertex3f)(GLfloat x, GLfloat y, GLfloat z);
void (APIENTRY *gglVertex3fv)(const GLfloat *v);
void (APIENTRY *gglVertex3i)(GLint x, GLint y, GLint z);
void (APIENTRY *gglVertex3iv)(const GLint *v);
void (APIENTRY *gglVertex3s)(GLshort x, GLshort y, GLshort z);
void (APIENTRY *gglVertex3sv)(const GLshort *v);
void (APIENTRY *gglVertex4d)(GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void (APIENTRY *gglVertex4dv)(const GLdouble *v);
void (APIENTRY *gglVertex4f)(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void (APIENTRY *gglVertex4fv)(const GLfloat *v);
void (APIENTRY *gglVertex4i)(GLint x, GLint y, GLint z, GLint w);
void (APIENTRY *gglVertex4iv)(const GLint *v);
void (APIENTRY *gglVertex4s)(GLshort x, GLshort y, GLshort z, GLshort w);
void (APIENTRY *gglVertex4sv)(const GLshort *v);
void (APIENTRY *gglViewport)(GLint x, GLint y, GLsizei width, GLsizei height);

/*
----------------
StringToken
----------------
*/
char *StringToken(const char *haystack, const char *needle)
{
	char *srcptr = (char *)haystack;
	char token[256];
	int i = 0;

	while(*srcptr != '\0')
	{
		//skip over spaces, newlines, commas, or tabs
		while(((*srcptr == ' ') || (*srcptr == '\n') || (*srcptr == ',') || (*srcptr == '\t')) && (*srcptr != '\0'))
			srcptr++;

		//parse the token
		while((*srcptr != ' ') && (*srcptr != '\n') && (*srcptr != ',') && (*srcptr != '\t') && (*srcptr != '\0') && (i < 256))
		{
			token[i] = *srcptr;
			srcptr++;
			i++;
		}

		//null-terminate the string
		token[i] = '\0';

		if(!strcmp(token, needle))
			return (char *)needle;

		i = 0;
	}

	//token was not found
	return NULL;
}

HINSTANCE	hInstOpenGL	= NULL;
HINSTANCE	hInstGDI	= NULL;
char *vendor, *renderer, *version, *extensions, *wglextensions = NULL;
int glversion[2];

/*
----------------
OpenGL_GetVersion
----------------
*/
void OpenGL_GetVersion(void)
{
	char *ptr = version;
	char number[16];
	int i;

	while((*ptr < '0') || (*ptr > '9'))
		ptr++;

	//parse the major version
	i = 0;
	while((*ptr != '.') && (*ptr != ' ') && (*ptr != '\0') && (i < 15))
	{
		number[i] = *ptr;
		ptr++;
		i++;
	}

	number[++i] = '\0';
	glversion[1] = atoi(number);

	while(((*ptr == '.') || (*ptr == ' ')) && (*ptr != '\0'))
		ptr++;

	//parse the minor version
	i = 0;
	while((*ptr != '.') && (*ptr != ' ') && (*ptr != '\0') && (i < 15))
	{
		number[i] = *ptr;
		ptr++;
		i++;
	}

	number[++i] = '\0';
	glversion[0] = atoi(number);
}

/*
----------------
OpenGL_Init
----------------
*/
const int OpenGL_Init(const int gldriver, void (*errHandler)(const char *error, ...))
{
	int retdriver;

	OpenGL_Error = errHandler;

	switch(gldriver)
	{
		case GLDRIVER_NONE:
			return GLDRIVER_NONE;

		case GLDRIVER_ANY:
			//try loading default OpenGL driver
			if(hInstOpenGL = LoadLibrary("opengl32.dll"))
				retdriver = GLDRIVER_DEFAULT;
			//try loading 3Dfx OpenGL driver
			else if(hInstOpenGL = LoadLibrary("3dfxvgl.dll"))
				retdriver = GLDRIVER_3DFX;
			else if(hInstOpenGL = LoadLibrary("3dfxogl.dll"))
				retdriver = GLDRIVER_3DFX;
			else
				return GLDRIVER_NONE;		//couldn't find an available OpenGL driver

			break;

		case GLDRIVER_DEFAULT:
			//try loading default OpenGL driver
			if(hInstOpenGL = LoadLibrary("opengl32.dll"))
				retdriver = GLDRIVER_DEFAULT;
			else
				return GLDRIVER_NONE;		//couldn't load default OpenGL driver

			break;

		case GLDRIVER_3DFX:
			//try loading 3Dfx OpenGL driver
			if(hInstOpenGL = LoadLibrary("3dfxvgl.dll"))
				retdriver = GLDRIVER_3DFX;
			else if(hInstOpenGL = LoadLibrary("3dfxogl.dll"))
				retdriver = GLDRIVER_3DFX;
			else
				return GLDRIVER_NONE;		//couldn't load 3dfx OpenGL driver

			break;

		default:
			return GLDRIVER_NONE;
	}

//==========================================================

	if(!(gglAccum = (void (APIENTRY *)(GLenum op, GLfloat value))GetProcAddress(hInstOpenGL, "glAccum")))
		OpenGL_Error("OpenGL_Init: Couldn't get glAccum proc address");

	if(!(gglAlphaFunc = (void (APIENTRY *)(GLenum func, GLclampf ref))GetProcAddress(hInstOpenGL, "glAlphaFunc")))
		OpenGL_Error("OpenGL_Init: Couldn't get glAlphaFunc proc address");

	if(!(gglBegin = (void (APIENTRY *)(GLenum mode))GetProcAddress(hInstOpenGL, "glBegin")))
		OpenGL_Error("OpenGL_Init: Couldn't get glBegin proc address");	

	if(!(gglBitmap = (void (APIENTRY *)(GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte *bitmap))GetProcAddress(hInstOpenGL, "glBitmap")))
		OpenGL_Error("OpenGL_Init: Couldn't get glBitmap proc address");

	if(!(gglBlendFunc = (void (APIENTRY *)(GLenum sfactor, GLenum dfactor))GetProcAddress(hInstOpenGL, "glBlendFunc")))
		OpenGL_Error("OpenGL_Init: Couldn't get glBlendFunc proc address");

	if(!(gglCallList = (void (APIENTRY *)(GLuint list))GetProcAddress(hInstOpenGL, "glCallList")))
		OpenGL_Error("OpenGL_Init: Couldn't get glCallList proc address");

	if(!(gglCallLists = (void (APIENTRY *)(GLsizei n, GLenum type, const GLvoid *lists))GetProcAddress(hInstOpenGL, "glCallLists")))
		OpenGL_Error("OpenGL_Init: Couldn't get glCallLists proc address");

	if(!(gglClear = (void (APIENTRY *)(GLbitfield mask))GetProcAddress(hInstOpenGL, "glClear")))
		OpenGL_Error("OpenGL_Init: Couldn't get glClear proc address");

	if(!(gglClearAccum = (void (APIENTRY *)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha))GetProcAddress(hInstOpenGL, "glClearAccum")))
		OpenGL_Error("OpenGL_Init: Couldn't get glClearAccum proc address");

	if(!(gglClearColor = (void (APIENTRY *)(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha))GetProcAddress(hInstOpenGL, "glClearColor")))
		OpenGL_Error("OpenGL_Init: Couldn't get glClearColor proc address");

	if(!(gglClearDepth = (void (APIENTRY *)(GLclampd depth))GetProcAddress(hInstOpenGL, "glClearDepth")))
		OpenGL_Error("OpenGL_Init: Couldn't get glClearDepth proc address");

	if(!(gglClearIndex = (void (APIENTRY *)(GLfloat c))GetProcAddress(hInstOpenGL, "glClearIndex")))
		OpenGL_Error("OpenGL_Init: Couldn't get glClearIndex proc address");

	if(!(gglClearStencil = (void (APIENTRY *)(GLint s))GetProcAddress(hInstOpenGL, "glClearStencil")))
		OpenGL_Error("OpenGL_Init: Couldn't get glClearStencil proc address");

	if(!(gglClipPlane = (void (APIENTRY *)(GLenum plane, const GLdouble *equation))GetProcAddress(hInstOpenGL, "glClipPlane")))
		OpenGL_Error("OpenGL_Init: Couldn't get glClipPlane proc address");

	if(!(gglColor3b = (void (APIENTRY *)(GLbyte red, GLbyte green, GLbyte blue))GetProcAddress(hInstOpenGL, "glColor3b")))
		OpenGL_Error("OpenGL_Init: Couldn't get glColor3b proc address");

	if(!(gglColor3bv = (void (APIENTRY *)(const GLbyte *v))GetProcAddress(hInstOpenGL, "glColor3bv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glColor3bv proc address");

	if(!(gglColor3d = (void (APIENTRY *)(GLdouble red, GLdouble green, GLdouble blue))GetProcAddress(hInstOpenGL, "glColor3d")))
		OpenGL_Error("OpenGL_Init: Couldn't get glColor3d proc address");

	if(!(gglColor3dv = (void (APIENTRY *)(const GLdouble *v))GetProcAddress(hInstOpenGL, "glColor3dv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glColor3dv proc address");

	if(!(gglColor3f = (void (APIENTRY *)(GLfloat red, GLfloat green, GLfloat blue))GetProcAddress(hInstOpenGL, "glColor3f")))
		OpenGL_Error("OpenGL_Init: Couldn't get glColor3f proc address");

	if(!(gglColor3fv = (void (APIENTRY *)(const GLfloat *v))GetProcAddress(hInstOpenGL, "glColor3fv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glColor3fv proc address");

	if(!(gglColor3i = (void (APIENTRY *)(GLint red, GLint green, GLint blue))GetProcAddress(hInstOpenGL, "glColor3i")))
		OpenGL_Error("OpenGL_Init: Couldn't get glColor3i proc address");

	if(!(gglColor3iv = (void (APIENTRY *)(const GLint *v))GetProcAddress(hInstOpenGL, "glColor3iv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glColor3iv proc address");

	if(!(gglColor3s = (void (APIENTRY *)(GLshort red, GLshort green, GLshort blue))GetProcAddress(hInstOpenGL, "glColor3s")))
		OpenGL_Error("OpenGL_Init: Couldn't get glColor3s proc address");

	if(!(gglColor3sv = (void (APIENTRY *)(const GLshort *v))GetProcAddress(hInstOpenGL, "glColor3sv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glColor3sv proc address");

	if(!(gglColor3ub = (void (APIENTRY *)(GLubyte red, GLubyte green, GLubyte blue))GetProcAddress(hInstOpenGL, "glColor3ub")))
		OpenGL_Error("OpenGL_Init: Couldn't get glColor3ub proc address");

	if(!(gglColor3ubv = (void (APIENTRY *)(const GLubyte *v))GetProcAddress(hInstOpenGL, "glColor3ubv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glColor3ubv proc address");

	if(!(gglColor3ui = (void (APIENTRY *)(GLuint red, GLuint green, GLuint blue))GetProcAddress(hInstOpenGL, "glColor3ui")))
		OpenGL_Error("OpenGL_Init: Couldn't get glColor3ui proc address");

	if(!(gglColor3uiv = (void (APIENTRY *)(const GLuint *v))GetProcAddress(hInstOpenGL, "glColor3uiv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glColor3uiv proc address");

	if(!(gglColor3us = (void (APIENTRY *)(GLushort red, GLushort green, GLushort blue))GetProcAddress(hInstOpenGL, "glColor3us")))
		OpenGL_Error("OpenGL_Init: Couldn't get glColor3us proc address");

	if(!(gglColor3usv = (void (APIENTRY *)(const GLushort *v))GetProcAddress(hInstOpenGL, "glColor3usv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glColor3usv proc address");

	if(!(gglColor4b = (void (APIENTRY *)(GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha))GetProcAddress(hInstOpenGL, "glColor4b")))
		OpenGL_Error("OpenGL_Init: Couldn't get glColor4b proc address");

	if(!(gglColor4bv = (void (APIENTRY *)(const GLbyte *v))GetProcAddress(hInstOpenGL, "glColor4bv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glColor4bv proc address");

	if(!(gglColor4d = (void (APIENTRY *)(GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha))GetProcAddress(hInstOpenGL, "glColor4d")))
		OpenGL_Error("OpenGL_Init: Couldn't get glColor4d proc address");

	if(!(gglColor4dv = (void (APIENTRY *)(const GLdouble *v))GetProcAddress(hInstOpenGL, "glColor4dv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glColor4dv proc address");

	if(!(gglColor4f = (void (APIENTRY *)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha))GetProcAddress(hInstOpenGL, "glColor4f")))
		OpenGL_Error("OpenGL_Init: Couldn't get glColor4f proc address");

	if(!(gglColor4fv = (void (APIENTRY *)(const GLfloat *v))GetProcAddress(hInstOpenGL, "glColor4fv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glColor4fv proc address");

	if(!(gglColor4i = (void (APIENTRY *)(GLint red, GLint green, GLint blue, GLint alpha))GetProcAddress(hInstOpenGL, "glColor4i")))
		OpenGL_Error("OpenGL_Init: Couldn't get glColor4i proc address");

	if(!(gglColor4iv = (void (APIENTRY *)(const GLint *v))GetProcAddress(hInstOpenGL, "glColor4iv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glColor4iv proc address");

	if(!(gglColor4s = (void (APIENTRY *)(GLshort red, GLshort green, GLshort blue, GLshort alpha))GetProcAddress(hInstOpenGL, "glColor4s")))
		OpenGL_Error("OpenGL_Init: Couldn't get glColor4s proc address");

	if(!(gglColor4sv = (void (APIENTRY *)(const GLshort *v))GetProcAddress(hInstOpenGL, "glColor4sv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glColor4sv proc address");

	if(!(gglColor4ub = (void (APIENTRY *)(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha))GetProcAddress(hInstOpenGL, "glColor4ub")))
		OpenGL_Error("OpenGL_Init: Couldn't get glColor4ub proc address");

	if(!(gglColor4ubv = (void (APIENTRY *)(const GLubyte *v))GetProcAddress(hInstOpenGL, "glColor4ubv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glColor4ubv proc address");

	if(!(gglColor4ui = (void (APIENTRY *)(GLuint red, GLuint green, GLuint blue, GLuint alpha))GetProcAddress(hInstOpenGL, "glColor4ui")))
		OpenGL_Error("OpenGL_Init: Couldn't get glColor4ui proc address");

	if(!(gglColor4uiv = (void (APIENTRY *)(const GLuint *v))GetProcAddress(hInstOpenGL, "glColor4uiv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glColor4uiv proc address");

	if(!(gglColor4us = (void (APIENTRY *)(GLushort red, GLushort green, GLushort blue, GLushort alpha))GetProcAddress(hInstOpenGL, "glColor4us")))
		OpenGL_Error("OpenGL_Init: Couldn't get glColor4us proc address");

	if(!(gglColor4usv = (void (APIENTRY *)(const GLushort *v))GetProcAddress(hInstOpenGL, "glColor4usv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glColor4usv proc address");

	if(!(gglColorMask = (void (APIENTRY *)(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha))GetProcAddress(hInstOpenGL, "glColorMask")))
		OpenGL_Error("OpenGL_Init: Couldn't get glColorMask proc address");

	if(!(gglColorMaterial = (void (APIENTRY *)(GLenum face, GLenum mode))GetProcAddress(hInstOpenGL, "glColorMaterial")))
		OpenGL_Error("OpenGL_Init: Couldn't get glColorMaterial proc address");

	if(!(gglCopyPixels = (void (APIENTRY *)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum type))GetProcAddress(hInstOpenGL, "glCopyPixels")))
		OpenGL_Error("OpenGL_Init: Couldn't get glCopyPixels proc address");

	if(!(gglCullFace = (void (APIENTRY *)(GLenum mode))GetProcAddress(hInstOpenGL, "glCullFace")))
		OpenGL_Error("OpenGL_Init: Couldn't get glCullFace proc address");

	if(!(gglDeleteLists = (void (APIENTRY *)(GLuint list, GLsizei range))GetProcAddress(hInstOpenGL, "glDeleteLists")))
		OpenGL_Error("OpenGL_Init: Couldn't get glDeleteLists proc address");

	if(!(gglDepthFunc = (void (APIENTRY *)(GLenum func))GetProcAddress(hInstOpenGL, "glDepthFunc")))
		OpenGL_Error("OpenGL_Init: Couldn't get glDepthFunc proc address");

	if(!(gglDepthMask = (void (APIENTRY *)(GLboolean flag))GetProcAddress(hInstOpenGL, "glDepthMask")))
		OpenGL_Error("OpenGL_Init: Couldn't get glDepthMask proc address");

	if(!(gglDepthRange = (void (APIENTRY *)(GLclampd zNear, GLclampd zFar))GetProcAddress(hInstOpenGL, "glDepthRange")))
		OpenGL_Error("OpenGL_Init: Couldn't get glDepthRange proc address");

	if(!(gglDisable = (void (APIENTRY *)(GLenum cap))GetProcAddress(hInstOpenGL, "glDisable")))
		OpenGL_Error("OpenGL_Init: Couldn't get glDisable proc address");

	if(!(gglDrawBuffer = (void (APIENTRY *)(GLenum mode))GetProcAddress(hInstOpenGL, "glDrawBuffer")))
		OpenGL_Error("OpenGL_Init: Couldn't get glDrawBuffer proc address");

	if(!(gglDrawPixels = (void (APIENTRY *)(GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels))GetProcAddress(hInstOpenGL, "glDrawPixels")))
		OpenGL_Error("OpenGL_Init: Couldn't get glDrawPixels proc address");

	if(!(gglEdgeFlag = (void (APIENTRY *)(GLboolean flag))GetProcAddress(hInstOpenGL, "glEdgeFlag")))
		OpenGL_Error("OpenGL_Init: Couldn't get glEdgeFlag proc address");

	if(!(gglEdgeFlagv = (void (APIENTRY *)(const GLboolean *flag))GetProcAddress(hInstOpenGL, "glEdgeFlagv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glEdgeFlagv proc address");

	if(!(gglEnable = (void (APIENTRY *)(GLenum cap))GetProcAddress(hInstOpenGL, "glEnable")))
		OpenGL_Error("OpenGL_Init: Couldn't get glEnable proc address");

	if(!(gglEnd = (void (APIENTRY *)(void))GetProcAddress(hInstOpenGL, "glEnd")))
		OpenGL_Error("OpenGL_Init: Couldn't get glEnd proc address");

	if(!(gglEndList = (void (APIENTRY *)(void))GetProcAddress(hInstOpenGL, "glEndList")))
		OpenGL_Error("OpenGL_Init: Couldn't get glEndList proc address");

	if(!(gglEvalCoord1d = (void (APIENTRY *)(GLdouble u))GetProcAddress(hInstOpenGL, "glEvalCoord1d")))
		OpenGL_Error("OpenGL_Init: Couldn't get glEvalCoord1d proc address");

	if(!(gglEvalCoord1dv = (void (APIENTRY *)(const GLdouble *u))GetProcAddress(hInstOpenGL, "glEvalCoord1dv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glEvalCoord1dv proc address");

	if(!(gglEvalCoord1f = (void (APIENTRY *)(GLfloat u))GetProcAddress(hInstOpenGL, "glEvalCoord1f")))
		OpenGL_Error("OpenGL_Init: Couldn't get glEvalCoord1f proc address");

	if(!(gglEvalCoord1fv = (void (APIENTRY *)(const GLfloat *u))GetProcAddress(hInstOpenGL, "glEvalCoord1fv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glEvalCoord1fv proc address");

	if(!(gglEvalCoord2d = (void (APIENTRY *)(GLdouble u, GLdouble v))GetProcAddress(hInstOpenGL, "glEvalCoord2d")))
		OpenGL_Error("OpenGL_Init: Couldn't get glEvalCoord2d proc address");

	if(!(gglEvalCoord2dv = (void (APIENTRY *)(const GLdouble *u))GetProcAddress(hInstOpenGL, "glEvalCoord2dv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glEvalCoord2dv proc address");

	if(!(gglEvalCoord2f = (void (APIENTRY *)(GLfloat u, GLfloat v))GetProcAddress(hInstOpenGL, "glEvalCoord2f")))
		OpenGL_Error("OpenGL_Init: Couldn't get glEvalCoord2f proc address");

	if(!(gglEvalCoord2fv = (void (APIENTRY *)(const GLfloat *u))GetProcAddress(hInstOpenGL, "glEvalCoord2fv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glEvalCoord2fv proc address");

	if(!(gglEvalMesh1 = (void (APIENTRY *)(GLenum mode, GLint i1, GLint i2))GetProcAddress(hInstOpenGL, "glEvalMesh1")))
		OpenGL_Error("OpenGL_Init: Couldn't get glEvalMesh1 proc address");

	if(!(gglEvalMesh2 = (void (APIENTRY *)(GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2))GetProcAddress(hInstOpenGL, "glEvalMesh2")))
		OpenGL_Error("OpenGL_Init: Couldn't get glEvalMesh2 proc address");

	if(!(gglEvalPoint1 = (void (APIENTRY *)(GLint i))GetProcAddress(hInstOpenGL, "glEvalPoint1")))
		OpenGL_Error("OpenGL_Init: Couldn't get glEvalPoint1 proc address");

	if(!(gglEvalPoint2 = (void (APIENTRY *)(GLint i, GLint j))GetProcAddress(hInstOpenGL, "glEvalPoint2")))
		OpenGL_Error("OpenGL_Init: Couldn't get glEvalPoint2 proc address");

	if(!(gglFeedbackBuffer = (void (APIENTRY *)(GLsizei size, GLenum type, GLfloat *buffer))GetProcAddress(hInstOpenGL, "glFeedbackBuffer")))
		OpenGL_Error("OpenGL_Init: Couldn't get glFeedbackBuffer proc address");

	if(!(gglFinish = (void (APIENTRY *)(void))GetProcAddress(hInstOpenGL, "glFinish")))
		OpenGL_Error("OpenGL_Init: Couldn't get glFinish proc address");

	if(!(gglFlush = (void (APIENTRY *)(void))GetProcAddress(hInstOpenGL, "glFlush")))
		OpenGL_Error("OpenGL_Init: Couldn't get glFlush proc address");

	if(!(gglFogf = (void (APIENTRY *)(GLenum pname, GLfloat param))GetProcAddress(hInstOpenGL, "glFogf")))
		OpenGL_Error("OpenGL_Init: Couldn't get glFogf proc address");

	if(!(gglFogfv = (void (APIENTRY *)(GLenum pname, const GLfloat *params))GetProcAddress(hInstOpenGL, "glFogfv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glFogfv proc address");

	if(!(gglFogi = (void (APIENTRY *)(GLenum pname, GLint param))GetProcAddress(hInstOpenGL, "glFogi")))
		OpenGL_Error("OpenGL_Init: Couldn't get glFogi proc address");

	if(!(gglFogiv = (void (APIENTRY *)(GLenum pname, const GLint *params))GetProcAddress(hInstOpenGL, "glFogiv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glFogiv proc address");

	if(!(gglFrontFace = (void (APIENTRY *)(GLenum mode))GetProcAddress(hInstOpenGL, "glFrontFace")))
		OpenGL_Error("OpenGL_Init: Couldn't get glFrontFace proc address");

	if(!(gglFrustum = (void (APIENTRY *)(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar))GetProcAddress(hInstOpenGL, "glFrustum")))
		OpenGL_Error("OpenGL_Init: Couldn't get glFrustum proc address");

	if(!(gglGenLists = (GLuint (APIENTRY *)(GLsizei range))GetProcAddress(hInstOpenGL, "glGenLists")))
		OpenGL_Error("OpenGL_Init: Couldn't get glGenLists proc address");

	if(!(gglGetBooleanv = (void (APIENTRY *)(GLenum pname, GLboolean *params))GetProcAddress(hInstOpenGL, "glGetBooleanv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glGetBooleanv proc address");

	if(!(gglGetClipPlane = (void (APIENTRY *)(GLenum plane, GLdouble *equation))GetProcAddress(hInstOpenGL, "glGetClipPlane")))
		OpenGL_Error("OpenGL_Init: Couldn't get glGetClipPlane proc address");

	if(!(gglGetDoublev = (void (APIENTRY *)(GLenum pname, GLdouble *params))GetProcAddress(hInstOpenGL, "glGetDoublev")))
		OpenGL_Error("OpenGL_Init: Couldn't get glGetDoublev proc address");

	if(!(gglGetError = (GLenum (APIENTRY *)(void))GetProcAddress(hInstOpenGL, "glGetError")))
		OpenGL_Error("OpenGL_Init: Couldn't get glGetError proc address");

	if(!(gglGetFloatv = (void (APIENTRY *)(GLenum pname, GLfloat *params))GetProcAddress(hInstOpenGL, "glGetFloatv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glGetFloatv proc address");

	if(!(gglGetIntegerv = (void (APIENTRY *)(GLenum pname, GLint *params))GetProcAddress(hInstOpenGL, "glGetIntegerv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glGetIntegerv proc address");

	if(!(gglGetLightfv = (void (APIENTRY *)(GLenum light, GLenum pname, GLfloat *params))GetProcAddress(hInstOpenGL, "glGetLightfv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glGetLightfv proc address");

	if(!(gglGetLightiv = (void (APIENTRY *)(GLenum light, GLenum pname, GLint *params))GetProcAddress(hInstOpenGL, "glGetLightiv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glGetLightiv proc address");

	if(!(gglGetMapdv = (void (APIENTRY *)(GLenum target, GLenum query, GLdouble *v))GetProcAddress(hInstOpenGL, "glGetMapdv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glGetMapdv proc address");

	if(!(gglGetMapfv = (void (APIENTRY *)(GLenum target, GLenum query, GLfloat *v))GetProcAddress(hInstOpenGL, "glGetMapfv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glGetMapfv proc address");

	if(!(gglGetMapiv = (void (APIENTRY *)(GLenum target, GLenum query, GLint *v))GetProcAddress(hInstOpenGL, "glGetMapiv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glGetMapiv proc address");

	if(!(gglGetMaterialfv = (void (APIENTRY *)(GLenum face, GLenum pname, GLfloat *params))GetProcAddress(hInstOpenGL, "glGetMaterialfv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glGetMaterialfv proc address");

	if(!(gglGetMaterialiv = (void (APIENTRY *)(GLenum face, GLenum pname, GLint *params))GetProcAddress(hInstOpenGL, "glGetMaterialiv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glGetMaterialiv proc address");

	if(!(gglGetPixelMapfv = (void (APIENTRY *)(GLenum map, GLfloat *values))GetProcAddress(hInstOpenGL, "glGetPixelMapfv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glGetPixelMapfv proc address");

	if(!(gglGetPixelMapuiv = (void (APIENTRY *)(GLenum map, GLuint *values))GetProcAddress(hInstOpenGL, "glGetPixelMapuiv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glGetPixelMapuiv proc address");

	if(!(gglGetPixelMapusv = (void (APIENTRY *)(GLenum map, GLushort *values))GetProcAddress(hInstOpenGL, "glGetPixelMapusv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glGetPixelMapusv proc address");

	if(!(gglGetPolygonStipple = (void (APIENTRY *)(GLubyte *mask))GetProcAddress(hInstOpenGL, "glGetPolygonStipple")))
		OpenGL_Error("OpenGL_Init: Couldn't get glGetPolygonStipple proc address");

	if(!(gglGetString = (const GLubyte *(APIENTRY *)(GLenum name))GetProcAddress(hInstOpenGL, "glGetString")))
		OpenGL_Error("OpenGL_Init: Couldn't get glGetString proc address");

	if(!(gglGetTexEnvfv = (void (APIENTRY *)(GLenum target, GLenum pname, GLfloat *params))GetProcAddress(hInstOpenGL, "glGetTexEnvfv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glGetTexEnvfv proc address");

	if(!(gglGetTexEnviv = (void (APIENTRY *)(GLenum target, GLenum pname, GLint *params))GetProcAddress(hInstOpenGL, "glGetTexEnviv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glGetTexEnviv proc address");

	if(!(gglGetTexGendv = (void (APIENTRY *)(GLenum coord, GLenum pname, GLdouble *params))GetProcAddress(hInstOpenGL, "glGetTexGendv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glGetTexGendv proc address");

	if(!(gglGetTexGenfv = (void (APIENTRY *)(GLenum coord, GLenum pname, GLfloat *params))GetProcAddress(hInstOpenGL, "glGetTexGenfv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glGetTexGenfv proc address");

	if(!(gglGetTexGeniv = (void (APIENTRY *)(GLenum coord, GLenum pname, GLint *params))GetProcAddress(hInstOpenGL, "glGetTexGeniv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glGetTexGeniv proc address");

	if(!(gglGetTexImage = (void (APIENTRY *)(GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels))GetProcAddress(hInstOpenGL, "glGetTexImage")))
		OpenGL_Error("OpenGL_Init: Couldn't get glGetTexImage proc address");

	if(!(gglGetTexLevelParameterfv = (void (APIENTRY *)(GLenum target, GLint level, GLenum pname, GLfloat *params))GetProcAddress(hInstOpenGL, "glGetTexLevelParameterfv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glGetTexLevelParameterfv proc address");

	if(!(gglGetTexLevelParameteriv = (void (APIENTRY *)(GLenum target, GLint level, GLenum pname, GLint *params))GetProcAddress(hInstOpenGL, "glGetTexLevelParameteriv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glGetTexLevelParameteriv proc address");

	if(!(gglGetTexParameterfv = (void (APIENTRY *)(GLenum target, GLenum pname, GLfloat *params))GetProcAddress(hInstOpenGL, "glGetTexParameterfv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glGetTexParameterfv proc address");

	if(!(gglGetTexParameteriv = (void (APIENTRY *)(GLenum target, GLenum pname, GLint *params))GetProcAddress(hInstOpenGL, "glGetTexParameteriv")))
		OpenGL_Error("OpenGL_Init: Couldn't get  proc address");

	if(!(gglHint = (void (APIENTRY *)(GLenum target, GLenum mode))GetProcAddress(hInstOpenGL, "glHint")))
		OpenGL_Error("OpenGL_Init: Couldn't get glHint proc address");

	if(!(gglIndexMask = (void (APIENTRY *)(GLuint mask))GetProcAddress(hInstOpenGL, "glIndexMask")))
		OpenGL_Error("OpenGL_Init: Couldn't get glIndexMask proc address");

	if(!(gglIndexd = (void (APIENTRY *)(GLdouble c))GetProcAddress(hInstOpenGL, "glIndexd")))
		OpenGL_Error("OpenGL_Init: Couldn't get glIndexd proc address");

	if(!(gglIndexdv = (void (APIENTRY *)(const GLdouble *c))GetProcAddress(hInstOpenGL, "glIndexdv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glIndexdv proc address");

	if(!(gglIndexf = (void (APIENTRY *)(GLfloat c))GetProcAddress(hInstOpenGL, "glIndexf")))
		OpenGL_Error("OpenGL_Init: Couldn't get glIndexf proc address");

	if(!(gglIndexfv = (void (APIENTRY *)(const GLfloat *c))GetProcAddress(hInstOpenGL, "glIndexfv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glIndexfv proc address");

	if(!(gglIndexi = (void (APIENTRY *)(GLint c))GetProcAddress(hInstOpenGL, "glIndexi")))
		OpenGL_Error("OpenGL_Init: Couldn't get glIndexi proc address");

	if(!(gglIndexiv = (void (APIENTRY *)(const GLint *c))GetProcAddress(hInstOpenGL, "glIndexiv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glIndexiv proc address");

	if(!(gglIndexs = (void (APIENTRY *)(GLshort c))GetProcAddress(hInstOpenGL, "glIndexs")))
		OpenGL_Error("OpenGL_Init: Couldn't get glIndexs proc address");

	if(!(gglIndexsv = (void (APIENTRY *)(const GLshort *c))GetProcAddress(hInstOpenGL, "glIndexsv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glIndexsv proc address");

	if(!(gglInitNames = (void (APIENTRY *)(void))GetProcAddress(hInstOpenGL, "glInitNames")))
		OpenGL_Error("OpenGL_Init: Couldn't get glInitNames proc address");

	if(!(gglIsEnabled = (GLboolean (APIENTRY *)(GLenum cap))GetProcAddress(hInstOpenGL, "glIsEnabled")))
		OpenGL_Error("OpenGL_Init: Couldn't get glIsEnabled proc address");

	if(!(gglIsList = (GLboolean (APIENTRY *)(GLuint list))GetProcAddress(hInstOpenGL, "glIsList")))
		OpenGL_Error("OpenGL_Init: Couldn't get glIsList proc address");

	if(!(gglLightModelf = (void (APIENTRY *)(GLenum pname, GLfloat param))GetProcAddress(hInstOpenGL, "glLightModelf")))
		OpenGL_Error("OpenGL_Init: Couldn't get glLightModelf proc address");

	if(!(gglLightModelfv = (void (APIENTRY *)(GLenum pname, const GLfloat *params))GetProcAddress(hInstOpenGL, "glLightModelfv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glLightModelfv proc address");

	if(!(gglLightModeli = (void (APIENTRY *)(GLenum pname, GLint param))GetProcAddress(hInstOpenGL, "glLightModeli")))
		OpenGL_Error("OpenGL_Init: Couldn't get glLightModeli proc address");

	if(!(gglLightModeliv = (void (APIENTRY *)(GLenum pname, const GLint *params))GetProcAddress(hInstOpenGL, "glLightModeliv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glLightModeliv proc address");

	if(!(gglLightf = (void (APIENTRY *)(GLenum light, GLenum pname, GLfloat param))GetProcAddress(hInstOpenGL, "glLightf")))
		OpenGL_Error("OpenGL_Init: Couldn't get glLightf proc address");

	if(!(gglLightfv = (void (APIENTRY *)(GLenum light, GLenum pname, const GLfloat *params))GetProcAddress(hInstOpenGL, "glLightfv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glLightfv proc address");

	if(!(gglLighti = (void (APIENTRY *)(GLenum light, GLenum pname, GLint param))GetProcAddress(hInstOpenGL, "glLighti")))
		OpenGL_Error("OpenGL_Init: Couldn't get glLighti proc address");

	if(!(gglLightiv = (void (APIENTRY *)(GLenum light, GLenum pname, const GLint *params))GetProcAddress(hInstOpenGL, "glLightiv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glLightiv proc address");

	if(!(gglLineStipple = (void (APIENTRY *)(GLint factor, GLushort pattern))GetProcAddress(hInstOpenGL, "glLineStipple")))
		OpenGL_Error("OpenGL_Init: Couldn't get glLineStipple proc address");

	if(!(gglLineWidth = (void (APIENTRY *)(GLfloat width))GetProcAddress(hInstOpenGL, "glLineWidth")))
		OpenGL_Error("OpenGL_Init: Couldn't get glLineWidth proc address");

	if(!(gglListBase = (void (APIENTRY *)(GLuint base))GetProcAddress(hInstOpenGL, "glListBase")))
		OpenGL_Error("OpenGL_Init: Couldn't get glListBase proc address");

	if(!(gglLoadIdentity = (void (APIENTRY *)(void))GetProcAddress(hInstOpenGL, "glLoadIdentity")))
		OpenGL_Error("OpenGL_Init: Couldn't get glLoadIdentity proc address");

	if(!(gglLoadMatrixd = (void (APIENTRY *)(const GLdouble *m))GetProcAddress(hInstOpenGL, "glLoadMatrixd")))
		OpenGL_Error("OpenGL_Init: Couldn't get glLoadMatrixd proc address");

	if(!(gglLoadMatrixf = (void (APIENTRY *)(const GLfloat *m))GetProcAddress(hInstOpenGL, "glLoadMatrixf")))
		OpenGL_Error("OpenGL_Init: Couldn't get glLoadMatrixf proc address");

	if(!(gglLoadName = (void (APIENTRY *)(GLuint name))GetProcAddress(hInstOpenGL, "glLoadName")))
		OpenGL_Error("OpenGL_Init: Couldn't get glLoadName proc address");

	if(!(gglLogicOp = (void (APIENTRY *)(GLenum opcode))GetProcAddress(hInstOpenGL, "glLogicOp")))
		OpenGL_Error("OpenGL_Init: Couldn't get glLogicOp proc address");

	if(!(gglMap1d = (void (APIENTRY *)(GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble *points))GetProcAddress(hInstOpenGL, "glMap1d")))
		OpenGL_Error("OpenGL_Init: Couldn't get glMap1d proc address");

	if(!(gglMap1f = (void (APIENTRY *)(GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points))GetProcAddress(hInstOpenGL, "glMap1f")))
		OpenGL_Error("OpenGL_Init: Couldn't get glMap1f proc address");

	if(!(gglMap2d = (void (APIENTRY *)(GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble *points))GetProcAddress(hInstOpenGL, "glMap2d")))
		OpenGL_Error("OpenGL_Init: Couldn't get glMap2d proc address");

	if(!(gglMap2f = (void (APIENTRY *)(GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat *points))GetProcAddress(hInstOpenGL, "glMap2f")))
		OpenGL_Error("OpenGL_Init: Couldn't get glMap2f proc address");

	if(!(gglMapGrid1d = (void (APIENTRY *)(GLint un, GLdouble u1, GLdouble u2))GetProcAddress(hInstOpenGL, "glMapGrid1d")))
		OpenGL_Error("OpenGL_Init: Couldn't get glMapGrid1d proc address");

	if(!(gglMapGrid1f = (void (APIENTRY *)(GLint un, GLfloat u1, GLfloat u2))GetProcAddress(hInstOpenGL, "glMapGrid1f")))
		OpenGL_Error("OpenGL_Init: Couldn't get glMapGrid1f proc address");

	if(!(gglMapGrid2d = (void (APIENTRY *)(GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2))GetProcAddress(hInstOpenGL, "glMapGrid2d")))
		OpenGL_Error("OpenGL_Init: Couldn't get glMapGrid2d proc address");

	if(!(gglMapGrid2f = (void (APIENTRY *)(GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2))GetProcAddress(hInstOpenGL, "glMapGrid2f")))
		OpenGL_Error("OpenGL_Init: Couldn't get glMapGrid2f proc address");

	if(!(gglMaterialf = (void (APIENTRY *)(GLenum face, GLenum pname, GLfloat param))GetProcAddress(hInstOpenGL, "glMaterialf")))
		OpenGL_Error("OpenGL_Init: Couldn't get glMaterialf proc address");

	if(!(gglMaterialfv = (void (APIENTRY *)(GLenum face, GLenum pname, const GLfloat *params))GetProcAddress(hInstOpenGL, "glMaterialfv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glMaterialfv proc address");

	if(!(gglMateriali = (void (APIENTRY *)(GLenum face, GLenum pname, GLint param))GetProcAddress(hInstOpenGL, "glMateriali")))
		OpenGL_Error("OpenGL_Init: Couldn't get glMateriali proc address");

	if(!(gglMaterialiv = (void (APIENTRY *)(GLenum face, GLenum pname, const GLint *params))GetProcAddress(hInstOpenGL, "glMaterialiv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glMaterialiv proc address");

	if(!(gglMatrixMode = (void (APIENTRY *)(GLenum mode))GetProcAddress(hInstOpenGL, "glMatrixMode")))
		OpenGL_Error("OpenGL_Init: Couldn't get glMatrixMode proc address");

	if(!(gglMultMatrixd = (void (APIENTRY *)(const GLdouble *m))GetProcAddress(hInstOpenGL, "glMultMatrixd")))
		OpenGL_Error("OpenGL_Init: Couldn't get glMultMatrixd proc address");

	if(!(gglMultMatrixf = (void (APIENTRY *)(const GLfloat *m))GetProcAddress(hInstOpenGL, "glMultMatrixf")))
		OpenGL_Error("OpenGL_Init: Couldn't get glMultMatrixf proc address");

	if(!(gglNewList = (void (APIENTRY *)(GLuint list, GLenum mode))GetProcAddress(hInstOpenGL, "glNewList")))
		OpenGL_Error("OpenGL_Init: Couldn't get glNewList proc address");

	if(!(gglNormal3b = (void (APIENTRY *)(GLbyte nx, GLbyte ny, GLbyte nz))GetProcAddress(hInstOpenGL, "glNormal3b")))
		OpenGL_Error("OpenGL_Init: Couldn't get glNormal3b proc address");

	if(!(gglNormal3bv = (void (APIENTRY *)(const GLbyte *v))GetProcAddress(hInstOpenGL, "glNormal3bv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glNormal3bv proc address");

	if(!(gglNormal3d = (void (APIENTRY *)(GLdouble nx, GLdouble ny, GLdouble nz))GetProcAddress(hInstOpenGL, "glNormal3d")))
		OpenGL_Error("OpenGL_Init: Couldn't get glNormal3d proc address");

	if(!(gglNormal3dv = (void (APIENTRY *)(const GLdouble *v))GetProcAddress(hInstOpenGL, "glNormal3dv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glNormal3dv proc address");

	if(!(gglNormal3f = (void (APIENTRY *)(GLfloat nx, GLfloat ny, GLfloat nz))GetProcAddress(hInstOpenGL, "glNormal3f")))
		OpenGL_Error("OpenGL_Init: Couldn't get glNormal3f proc address");

	if(!(gglNormal3fv = (void (APIENTRY *)(const GLfloat *v))GetProcAddress(hInstOpenGL, "glNormal3fv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glNormal3fv proc address");

	if(!(gglNormal3i = (void (APIENTRY *)(GLint nx, GLint ny, GLint nz))GetProcAddress(hInstOpenGL, "glNormal3i")))
		OpenGL_Error("OpenGL_Init: Couldn't get glNormal3i proc address");

	if(!(gglNormal3iv = (void (APIENTRY *)(const GLint *v))GetProcAddress(hInstOpenGL, "glNormal3iv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glNormal3iv proc address");

	if(!(gglNormal3s = (void (APIENTRY *)(GLshort nx, GLshort ny, GLshort nz))GetProcAddress(hInstOpenGL, "glNormal3s")))
		OpenGL_Error("OpenGL_Init: Couldn't get glNormal3s proc address");

	if(!(gglNormal3sv = (void (APIENTRY *)(const GLshort *v))GetProcAddress(hInstOpenGL, "glNormal3sv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glNormal3sv proc address");

	if(!(gglOrtho = (void (APIENTRY *)(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar))GetProcAddress(hInstOpenGL, "glOrtho")))
		OpenGL_Error("OpenGL_Init: Couldn't get glOrtho proc address");

	if(!(gglPassThrough = (void (APIENTRY *)(GLfloat token))GetProcAddress(hInstOpenGL, "glPassThrough")))
		OpenGL_Error("OpenGL_Init: Couldn't get glPassThrough proc address");

	if(!(gglPixelMapfv = (void (APIENTRY *)(GLenum map, GLint mapsize, const GLfloat *values))GetProcAddress(hInstOpenGL, "glPixelMapfv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glPixelMapfv proc address");

	if(!(gglPixelMapuiv = (void (APIENTRY *)(GLenum map, GLint mapsize, const GLuint *values))GetProcAddress(hInstOpenGL, "glPixelMapuiv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glPixelMapuiv proc address");

	if(!(gglPixelMapusv = (void (APIENTRY *)(GLenum map, GLint mapsize, const GLushort *values))GetProcAddress(hInstOpenGL, "glPixelMapusv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glPixelMapusv proc address");

	if(!(gglPixelStoref = (void (APIENTRY *)(GLenum pname, GLfloat param))GetProcAddress(hInstOpenGL, "glPixelStoref")))
		OpenGL_Error("OpenGL_Init: Couldn't get glPixelStoref proc address");

	if(!(gglPixelStorei = (void (APIENTRY *)(GLenum pname, GLint param))GetProcAddress(hInstOpenGL, "glPixelStorei")))
		OpenGL_Error("OpenGL_Init: Couldn't get glPixelStorei proc address");

	if(!(gglPixelTransferf = (void (APIENTRY *)(GLenum pname, GLfloat param))GetProcAddress(hInstOpenGL, "glPixelTransferf")))
		OpenGL_Error("OpenGL_Init: Couldn't get glPixelTransferf proc address");

	if(!(gglPixelTransferi = (void (APIENTRY *)(GLenum pname, GLint param))GetProcAddress(hInstOpenGL, "glPixelTransferi")))
		OpenGL_Error("OpenGL_Init: Couldn't get glPixelTransferi proc address");

	if(!(gglPixelZoom = (void (APIENTRY *)(GLfloat xfactor, GLfloat yfactor))GetProcAddress(hInstOpenGL, "glPixelZoom")))
		OpenGL_Error("OpenGL_Init: Couldn't get glPixelZoom proc address");

	if(!(gglPointSize = (void (APIENTRY *)(GLfloat size))GetProcAddress(hInstOpenGL, "glPointSize")))
		OpenGL_Error("OpenGL_Init: Couldn't get glPointSize proc address");

	if(!(gglPolygonMode = (void (APIENTRY *)(GLenum face, GLenum mode))GetProcAddress(hInstOpenGL, "glPolygonMode")))
		OpenGL_Error("OpenGL_Init: Couldn't get glPolygonMode proc address");

	if(!(gglPolygonStipple = (void (APIENTRY *)(const GLubyte *mask))GetProcAddress(hInstOpenGL, "glPolygonStipple")))
		OpenGL_Error("OpenGL_Init: Couldn't get glPolygonStipple proc address");

	if(!(gglPopAttrib = (void (APIENTRY *)(void))GetProcAddress(hInstOpenGL, "glPopAttrib")))
		OpenGL_Error("OpenGL_Init: Couldn't get glPopAttrib proc address");

	if(!(gglPopMatrix = (void (APIENTRY *)(void))GetProcAddress(hInstOpenGL, "glPopMatrix")))
		OpenGL_Error("OpenGL_Init: Couldn't get glPopMatrix proc address");

	if(!(gglPopName = (void (APIENTRY *)(void))GetProcAddress(hInstOpenGL, "glPopName")))
		OpenGL_Error("OpenGL_Init: Couldn't get glPopName proc address");

	if(!(gglPushAttrib = (void (APIENTRY *)(GLbitfield mask))GetProcAddress(hInstOpenGL, "glPushAttrib")))
		OpenGL_Error("OpenGL_Init: Couldn't get glPushAttrib proc address");

	if(!(gglPushMatrix = (void (APIENTRY *)(void))GetProcAddress(hInstOpenGL, "glPushMatrix")))
		OpenGL_Error("OpenGL_Init: Couldn't get glPushMatrix proc address");

	if(!(gglPushName = (void (APIENTRY *)(GLuint name))GetProcAddress(hInstOpenGL, "glPushName")))
		OpenGL_Error("OpenGL_Init: Couldn't get glPushName proc address");

	if(!(gglRasterPos2d = (void (APIENTRY *)(GLdouble x, GLdouble y))GetProcAddress(hInstOpenGL, "glRasterPos2d")))
		OpenGL_Error("OpenGL_Init: Couldn't get glRasterPos2d proc address");

	if(!(gglRasterPos2dv = (void (APIENTRY *)(const GLdouble *v))GetProcAddress(hInstOpenGL, "glRasterPos2dv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glRasterPos2dv proc address");

	if(!(gglRasterPos2f = (void (APIENTRY *)(GLfloat x, GLfloat y))GetProcAddress(hInstOpenGL, "glRasterPos2f")))
		OpenGL_Error("OpenGL_Init: Couldn't get glRasterPos2f proc address");

	if(!(gglRasterPos2fv = (void (APIENTRY *)(const GLfloat *v))GetProcAddress(hInstOpenGL, "glRasterPos2fv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glRasterPos2fv proc address");

	if(!(gglRasterPos2i = (void (APIENTRY *)(GLint x, GLint y))GetProcAddress(hInstOpenGL, "glRasterPos2i")))
		OpenGL_Error("OpenGL_Init: Couldn't get glRasterPos2i proc address");

	if(!(gglRasterPos2iv = (void (APIENTRY *)(const GLint *v))GetProcAddress(hInstOpenGL, "glRasterPos2iv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glRasterPos2iv proc address");

	if(!(gglRasterPos2s = (void (APIENTRY *)(GLshort x, GLshort y))GetProcAddress(hInstOpenGL, "glRasterPos2s")))
		OpenGL_Error("OpenGL_Init: Couldn't get glRasterPos2s proc address");

	if(!(gglRasterPos2sv = (void (APIENTRY *)(const GLshort *v))GetProcAddress(hInstOpenGL, "glRasterPos2sv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glRasterPos2sv proc address");

	if(!(gglRasterPos3d = (void (APIENTRY *)(GLdouble x, GLdouble y, GLdouble z))GetProcAddress(hInstOpenGL, "glRasterPos3d")))
		OpenGL_Error("OpenGL_Init: Couldn't get glRasterPos3d proc address");

	if(!(gglRasterPos3dv = (void (APIENTRY *)(const GLdouble *v))GetProcAddress(hInstOpenGL, "glRasterPos3dv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glRasterPos3dv proc address");

	if(!(gglRasterPos3f = (void (APIENTRY *)(GLfloat x, GLfloat y, GLfloat z))GetProcAddress(hInstOpenGL, "glRasterPos3f")))
		OpenGL_Error("OpenGL_Init: Couldn't get glRasterPos3f proc address");

	if(!(gglRasterPos3fv = (void (APIENTRY *)(const GLfloat *v))GetProcAddress(hInstOpenGL, "glRasterPos3fv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glRasterPos3fv proc address");

	if(!(gglRasterPos3i = (void (APIENTRY *)(GLint x, GLint y, GLint z))GetProcAddress(hInstOpenGL, "glRasterPos3i")))
		OpenGL_Error("OpenGL_Init: Couldn't get glRasterPos3i proc address");

	if(!(gglRasterPos3iv = (void (APIENTRY *)(const GLint *v))GetProcAddress(hInstOpenGL, "glRasterPos3iv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glRasterPos3iv proc address");

	if(!(gglRasterPos3s = (void (APIENTRY *)(GLshort x, GLshort y, GLshort z))GetProcAddress(hInstOpenGL, "glRasterPos3s")))
		OpenGL_Error("OpenGL_Init: Couldn't get glRasterPos3s proc address");

	if(!(gglRasterPos3sv = (void (APIENTRY *)(const GLshort *v))GetProcAddress(hInstOpenGL, "glRasterPos3sv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glRasterPos3sv proc address");

	if(!(gglRasterPos4d = (void (APIENTRY *)(GLdouble x, GLdouble y, GLdouble z, GLdouble w))GetProcAddress(hInstOpenGL, "glRasterPos4d")))
		OpenGL_Error("OpenGL_Init: Couldn't get glRasterPos4d proc address");

	if(!(gglRasterPos4dv = (void (APIENTRY *)(const GLdouble *v))GetProcAddress(hInstOpenGL, "glRasterPos4dv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glRasterPos4dv proc address");

	if(!(gglRasterPos4f = (void (APIENTRY *)(GLfloat x, GLfloat y, GLfloat z, GLfloat w))GetProcAddress(hInstOpenGL, "glRasterPos4f")))
		OpenGL_Error("OpenGL_Init: Couldn't get glRasterPos4f proc address");

	if(!(gglRasterPos4fv = (void (APIENTRY *)(const GLfloat *v))GetProcAddress(hInstOpenGL, "glRasterPos4fv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glRasterPos4fv proc address");

	if(!(gglRasterPos4i = (void (APIENTRY *)(GLint x, GLint y, GLint z, GLint w))GetProcAddress(hInstOpenGL, "glRasterPos4i")))
		OpenGL_Error("OpenGL_Init: Couldn't get glRasterPos4i proc address");

	if(!(gglRasterPos4iv = (void (APIENTRY *)(const GLint *v))GetProcAddress(hInstOpenGL, "glRasterPos4iv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glRasterPos4iv proc address");

	if(!(gglRasterPos4s = (void (APIENTRY *)(GLshort x, GLshort y, GLshort z, GLshort w))GetProcAddress(hInstOpenGL, "glRasterPos4s")))
		OpenGL_Error("OpenGL_Init: Couldn't get glRasterPos4s proc address");

	if(!(gglRasterPos4sv = (void (APIENTRY *)(const GLshort *v))GetProcAddress(hInstOpenGL, "glRasterPos4sv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glRasterPos4sv proc address");

	if(!(gglReadBuffer = (void (APIENTRY *)(GLenum mode))GetProcAddress(hInstOpenGL, "glReadBuffer")))
		OpenGL_Error("OpenGL_Init: Couldn't get glReadBuffer proc address");

	if(!(gglReadPixels = (void (APIENTRY *)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels))GetProcAddress(hInstOpenGL, "glReadPixels")))
		OpenGL_Error("OpenGL_Init: Couldn't get glReadPixels proc address");

	if(!(gglRectd = (void (APIENTRY *)(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2))GetProcAddress(hInstOpenGL, "glRectd")))
		OpenGL_Error("OpenGL_Init: Couldn't get glRectd proc address");

	if(!(gglRectdv = (void (APIENTRY *)(const GLdouble *v1, const GLdouble *v2))GetProcAddress(hInstOpenGL, "glRectdv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glRectdv proc address");

	if(!(gglRectf = (void (APIENTRY *)(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2))GetProcAddress(hInstOpenGL, "glRectf")))
		OpenGL_Error("OpenGL_Init: Couldn't get glRectf proc address");

	if(!(gglRectfv = (void (APIENTRY *)(const GLfloat *v1, const GLfloat *v2))GetProcAddress(hInstOpenGL, "glRectfv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glRectfv proc address");

	if(!(gglRecti = (void (APIENTRY *)(GLint x1, GLint y1, GLint x2, GLint y2))GetProcAddress(hInstOpenGL, "glRecti")))
		OpenGL_Error("OpenGL_Init: Couldn't get glRecti proc address");

	if(!(gglRectiv = (void (APIENTRY *)(const GLint *v1, const GLint *v2))GetProcAddress(hInstOpenGL, "glRectiv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glRectiv proc address");

	if(!(gglRects = (void (APIENTRY *)(GLshort x1, GLshort y1, GLshort x2, GLshort y2))GetProcAddress(hInstOpenGL, "glRects")))
		OpenGL_Error("OpenGL_Init: Couldn't get glRects proc address");

	if(!(gglRectsv = (void (APIENTRY *)(const GLshort *v1, const GLshort *v2))GetProcAddress(hInstOpenGL, "glRectsv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glRectsv proc address");

	if(!(gglRenderMode = (GLint (APIENTRY *)(GLenum mode))GetProcAddress(hInstOpenGL, "glRenderMode")))
		OpenGL_Error("OpenGL_Init: Couldn't get glRenderMode proc address");

	if(!(gglRotated = (void (APIENTRY *)(GLdouble angle, GLdouble x, GLdouble y, GLdouble z))GetProcAddress(hInstOpenGL, "glRotated")))
		OpenGL_Error("OpenGL_Init: Couldn't get glRotated proc address");

	if(!(gglRotatef = (void (APIENTRY *)(GLfloat angle, GLfloat x, GLfloat y, GLfloat z))GetProcAddress(hInstOpenGL, "glRotatef")))
		OpenGL_Error("OpenGL_Init: Couldn't get glRotatef proc address");

	if(!(gglScaled = (void (APIENTRY *)(GLdouble x, GLdouble y, GLdouble z))GetProcAddress(hInstOpenGL, "glScaled")))
		OpenGL_Error("OpenGL_Init: Couldn't get glScaled proc address");

	if(!(gglScalef = (void (APIENTRY *)(GLfloat x, GLfloat y, GLfloat z))GetProcAddress(hInstOpenGL, "glScalef")))
		OpenGL_Error("OpenGL_Init: Couldn't get glScalef proc address");

	if(!(gglScissor = (void (APIENTRY *)(GLint x, GLint y, GLsizei width, GLsizei height))GetProcAddress(hInstOpenGL, "glScissor")))
		OpenGL_Error("OpenGL_Init: Couldn't get glScissor proc address");

	if(!(gglSelectBuffer = (void (APIENTRY *)(GLsizei size, GLuint *buffer))GetProcAddress(hInstOpenGL, "glSelectBuffer")))
		OpenGL_Error("OpenGL_Init: Couldn't get glSelectBuffer proc address");

	if(!(gglShadeModel = (void (APIENTRY *)(GLenum mode))GetProcAddress(hInstOpenGL, "glShadeModel")))
		OpenGL_Error("OpenGL_Init: Couldn't get glShadeModel proc address");

	if(!(gglStencilFunc = (void (APIENTRY *)(GLenum func, GLint ref, GLuint mask))GetProcAddress(hInstOpenGL, "glStencilFunc")))
		OpenGL_Error("OpenGL_Init: Couldn't get glStencilFunc proc address");

	if(!(gglStencilMask = (void (APIENTRY *)(GLuint mask))GetProcAddress(hInstOpenGL, "glStencilMask")))
		OpenGL_Error("OpenGL_Init: Couldn't get glStencilMask proc address");

	if(!(gglStencilOp = (void (APIENTRY *)(GLenum fail, GLenum zfail, GLenum zpass))GetProcAddress(hInstOpenGL, "glStencilOp")))
		OpenGL_Error("OpenGL_Init: Couldn't get glStencilOp proc address");

	if(!(gglTexCoord1d = (void (APIENTRY *)(GLdouble s))GetProcAddress(hInstOpenGL, "glTexCoord1d")))
		OpenGL_Error("OpenGL_Init: Couldn't get glTexCoord1d proc address");

	if(!(gglTexCoord1dv = (void (APIENTRY *)(const GLdouble *v))GetProcAddress(hInstOpenGL, "glTexCoord1dv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glTexCoord1dv proc address");

	if(!(gglTexCoord1f = (void (APIENTRY *)(GLfloat s))GetProcAddress(hInstOpenGL, "glTexCoord1f")))
		OpenGL_Error("OpenGL_Init: Couldn't get glTexCoord1f proc address");

	if(!(gglTexCoord1fv = (void (APIENTRY *)(const GLfloat *v))GetProcAddress(hInstOpenGL, "glTexCoord1fv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glTexCoord1fv proc address");

	if(!(gglTexCoord1i = (void (APIENTRY *)(GLint s))GetProcAddress(hInstOpenGL, "glTexCoord1i")))
		OpenGL_Error("OpenGL_Init: Couldn't get glTexCoord1i proc address");

	if(!(gglTexCoord1iv = (void (APIENTRY *)(const GLint *v))GetProcAddress(hInstOpenGL, "glTexCoord1iv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glTexCoord1iv proc address");

	if(!(gglTexCoord1s = (void (APIENTRY *)(GLshort s))GetProcAddress(hInstOpenGL, "glTexCoord1s")))
		OpenGL_Error("OpenGL_Init: Couldn't get glTexCoord1s proc address");

	if(!(gglTexCoord1sv = (void (APIENTRY *)(const GLshort *v))GetProcAddress(hInstOpenGL, "glTexCoord1sv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glTexCoord1sv proc address");

	if(!(gglTexCoord2d = (void (APIENTRY *)(GLdouble s, GLdouble t))GetProcAddress(hInstOpenGL, "glTexCoord2d")))
		OpenGL_Error("OpenGL_Init: Couldn't get glTexCoord2d proc address");

	if(!(gglTexCoord2dv = (void (APIENTRY *)(const GLdouble *v))GetProcAddress(hInstOpenGL, "glTexCoord2dv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glTexCoord2dv proc address");

	if(!(gglTexCoord2f = (void (APIENTRY *)(GLfloat s, GLfloat t))GetProcAddress(hInstOpenGL, "glTexCoord2f")))
		OpenGL_Error("OpenGL_Init: Couldn't get glTexCoord2f proc address");

	if(!(gglTexCoord2fv = (void (APIENTRY *)(const GLfloat *v))GetProcAddress(hInstOpenGL, "glTexCoord2fv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glTexCoord2fv proc address");

	if(!(gglTexCoord2i = (void (APIENTRY *)(GLint s, GLint t))GetProcAddress(hInstOpenGL, "glTexCoord2i")))
		OpenGL_Error("OpenGL_Init: Couldn't get glTexCoord2i proc address");

	if(!(gglTexCoord2iv = (void (APIENTRY *)(const GLint *v))GetProcAddress(hInstOpenGL, "glTexCoord2iv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glTexCoord2iv proc address");

	if(!(gglTexCoord2s = (void (APIENTRY *)(GLshort s, GLshort t))GetProcAddress(hInstOpenGL, "glTexCoord2s")))
		OpenGL_Error("OpenGL_Init: Couldn't get glTexCoord2s proc address");

	if(!(gglTexCoord2sv = (void (APIENTRY *)(const GLshort *v))GetProcAddress(hInstOpenGL, "glTexCoord2sv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glTexCoord2sv proc address");

	if(!(gglTexCoord3d = (void (APIENTRY *)(GLdouble s, GLdouble t, GLdouble r))GetProcAddress(hInstOpenGL, "glTexCoord3d")))
		OpenGL_Error("OpenGL_Init: Couldn't get glTexCoord3d proc address");

	if(!(gglTexCoord3dv = (void (APIENTRY *)(const GLdouble *v))GetProcAddress(hInstOpenGL, "glTexCoord3dv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glTexCoord3dv proc address");

	if(!(gglTexCoord3f = (void (APIENTRY *)(GLfloat s, GLfloat t, GLfloat r))GetProcAddress(hInstOpenGL, "glTexCoord3f")))
		OpenGL_Error("OpenGL_Init: Couldn't get glTexCoord3f proc address");

	if(!(gglTexCoord3fv = (void (APIENTRY *)(const GLfloat *v))GetProcAddress(hInstOpenGL, "glTexCoord3fv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glTexCoord3fv proc address");

	if(!(gglTexCoord3i = (void (APIENTRY *)(GLint s, GLint t, GLint r))GetProcAddress(hInstOpenGL, "glTexCoord3i")))
		OpenGL_Error("OpenGL_Init: Couldn't get glTexCoord3i proc address");

	if(!(gglTexCoord3iv = (void (APIENTRY *)(const GLint *v))GetProcAddress(hInstOpenGL, "glTexCoord3iv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glTexCoord3iv proc address");

	if(!(gglTexCoord3s = (void (APIENTRY *)(GLshort s, GLshort t, GLshort r))GetProcAddress(hInstOpenGL, "glTexCoord3s")))
		OpenGL_Error("OpenGL_Init: Couldn't get glTexCoord3s proc address");

	if(!(gglTexCoord3sv = (void (APIENTRY *)(const GLshort *v))GetProcAddress(hInstOpenGL, "glTexCoord3sv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glTexCoord3sv proc address");

	if(!(gglTexCoord4d = (void (APIENTRY *)(GLdouble s, GLdouble t, GLdouble r, GLdouble q))GetProcAddress(hInstOpenGL, "glTexCoord4d")))
		OpenGL_Error("OpenGL_Init: Couldn't get glTexCoord4d proc address");

	if(!(gglTexCoord4dv = (void (APIENTRY *)(const GLdouble *v))GetProcAddress(hInstOpenGL, "glTexCoord4dv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glTexCoord4dv proc address");

	if(!(gglTexCoord4f = (void (APIENTRY *)(GLfloat s, GLfloat t, GLfloat r, GLfloat q))GetProcAddress(hInstOpenGL, "glTexCoord4f")))
		OpenGL_Error("OpenGL_Init: Couldn't get glTexCoord4f proc address");

	if(!(gglTexCoord4fv = (void (APIENTRY *)(const GLfloat *v))GetProcAddress(hInstOpenGL, "glTexCoord4fv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glTexCoord4fv proc address");

	if(!(gglTexCoord4i = (void (APIENTRY *)(GLint s, GLint t, GLint r, GLint q))GetProcAddress(hInstOpenGL, "glTexCoord4i")))
		OpenGL_Error("OpenGL_Init: Couldn't get glTexCoord4i proc address");

	if(!(gglTexCoord4iv = (void (APIENTRY *)(const GLint *v))GetProcAddress(hInstOpenGL, "glTexCoord4iv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glTexCoord4iv proc address");

	if(!(gglTexCoord4s = (void (APIENTRY *)(GLshort s, GLshort t, GLshort r, GLshort q))GetProcAddress(hInstOpenGL, "glTexCoord4s")))
		OpenGL_Error("OpenGL_Init: Couldn't get glTexCoord4s proc address");

	if(!(gglTexCoord4sv = (void (APIENTRY *)(const GLshort *v))GetProcAddress(hInstOpenGL, "glTexCoord4sv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glTexCoord4sv proc address");

	if(!(gglTexEnvf = (void (APIENTRY *)(GLenum target, GLenum pname, GLfloat param))GetProcAddress(hInstOpenGL, "glTexEnvf")))
		OpenGL_Error("OpenGL_Init: Couldn't get glTexEnvf proc address");

	if(!(gglTexEnvfv = (void (APIENTRY *)(GLenum target, GLenum pname, const GLfloat *params))GetProcAddress(hInstOpenGL, "glTexEnvfv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glTexEnvfv proc address");

	if(!(gglTexEnvi = (void (APIENTRY *)(GLenum target, GLenum pname, GLint param))GetProcAddress(hInstOpenGL, "glTexEnvi")))
		OpenGL_Error("OpenGL_Init: Couldn't get glTexEnvi proc address");

	if(!(gglTexEnviv = (void (APIENTRY *)(GLenum target, GLenum pname, const GLint *params))GetProcAddress(hInstOpenGL, "glTexEnviv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glTexEnviv proc address");

	if(!(gglTexGend = (void (APIENTRY *)(GLenum coord, GLenum pname, GLdouble param))GetProcAddress(hInstOpenGL, "glTexGend")))
		OpenGL_Error("OpenGL_Init: Couldn't get glTexGend proc address");

	if(!(gglTexGendv = (void (APIENTRY *)(GLenum coord, GLenum pname, const GLdouble *params))GetProcAddress(hInstOpenGL, "glTexGendv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glTexGendv proc address");

	if(!(gglTexGenf = (void (APIENTRY *)(GLenum coord, GLenum pname, GLfloat param))GetProcAddress(hInstOpenGL, "glTexGenf")))
		OpenGL_Error("OpenGL_Init: Couldn't get glTexGenf proc address");

	if(!(gglTexGenfv = (void (APIENTRY *)(GLenum coord, GLenum pname, const GLfloat *params))GetProcAddress(hInstOpenGL, "glTexGenfv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glTexGenfv proc address");

	if(!(gglTexGeni = (void (APIENTRY *)(GLenum coord, GLenum pname, GLint param))GetProcAddress(hInstOpenGL, "glTexGeni")))
		OpenGL_Error("OpenGL_Init: Couldn't get glTexGeni proc address");

	if(!(gglTexGeniv = (void (APIENTRY *)(GLenum coord, GLenum pname, const GLint *params))GetProcAddress(hInstOpenGL, "glTexGeniv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glTexGeniv proc address");

	if(!(gglTexImage1D = (void (APIENTRY *)(GLenum target, GLint level, GLint components, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels))GetProcAddress(hInstOpenGL, "glTexImage1D")))
		OpenGL_Error("OpenGL_Init: Couldn't get glTexImage1D proc address");

	if(!(gglTexImage2D = (void (APIENTRY *)(GLenum target, GLint level, GLint components, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels))GetProcAddress(hInstOpenGL, "glTexImage2D")))
		OpenGL_Error("OpenGL_Init: Couldn't get glTexImage2D proc address");

	if(!(gglTexParameterf = (void (APIENTRY *)(GLenum target, GLenum pname, GLfloat param))GetProcAddress(hInstOpenGL, "glTexParameterf")))
		OpenGL_Error("OpenGL_Init: Couldn't get glTexParameterf proc address");

	if(!(gglTexParameterfv = (void (APIENTRY *)(GLenum target, GLenum pname, const GLfloat *params))GetProcAddress(hInstOpenGL, "glTexParameterfv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glTexParameterfv proc address");

	if(!(gglTexParameteri = (void (APIENTRY *)(GLenum target, GLenum pname, GLint param))GetProcAddress(hInstOpenGL, "glTexParameteri")))
		OpenGL_Error("OpenGL_Init: Couldn't get glTexParameteri proc address");

	if(!(gglTexParameteriv = (void (APIENTRY *)(GLenum target, GLenum pname, const GLint *params))GetProcAddress(hInstOpenGL, "glTexParameteriv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glTexParameteriv proc address");

	if(!(gglTranslated = (void (APIENTRY *)(GLdouble x, GLdouble y, GLdouble z))GetProcAddress(hInstOpenGL, "glTranslated")))
		OpenGL_Error("OpenGL_Init: Couldn't get glTranslated proc address");

	if(!(gglTranslatef = (void (APIENTRY *)(GLfloat x, GLfloat y, GLfloat z))GetProcAddress(hInstOpenGL, "glTranslatef")))
		OpenGL_Error("OpenGL_Init: Couldn't get glTranslatef proc address");

	if(!(gglVertex2d = (void (APIENTRY *)(GLdouble x, GLdouble y))GetProcAddress(hInstOpenGL, "glVertex2d")))
		OpenGL_Error("OpenGL_Init: Couldn't get glVertex2d proc address");

	if(!(gglVertex2dv = (void (APIENTRY *)(const GLdouble *v))GetProcAddress(hInstOpenGL, "glVertex2dv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glVertex2dv proc address");

	if(!(gglVertex2f = (void (APIENTRY *)(GLfloat x, GLfloat y))GetProcAddress(hInstOpenGL, "glVertex2f")))
		OpenGL_Error("OpenGL_Init: Couldn't get glVertex2f proc address");

	if(!(gglVertex2fv = (void (APIENTRY *)(const GLfloat *v))GetProcAddress(hInstOpenGL, "glVertex2fv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glVertex2fv proc address");

	if(!(gglVertex2i = (void (APIENTRY *)(GLint x, GLint y))GetProcAddress(hInstOpenGL, "glVertex2i")))
		OpenGL_Error("OpenGL_Init: Couldn't get glVertex2i proc address");

	if(!(gglVertex2iv = (void (APIENTRY *)(const GLint *v))GetProcAddress(hInstOpenGL, "glVertex2iv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glVertex2iv proc address");

	if(!(gglVertex2s = (void (APIENTRY *)(GLshort x, GLshort y))GetProcAddress(hInstOpenGL, "glVertex2s")))
		OpenGL_Error("OpenGL_Init: Couldn't get glVertex2s proc address");

	if(!(gglVertex2sv = (void (APIENTRY *)(const GLshort *v))GetProcAddress(hInstOpenGL, "glVertex2sv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glVertex2sv proc address");

	if(!(gglVertex3d = (void (APIENTRY *)(GLdouble x, GLdouble y, GLdouble z))GetProcAddress(hInstOpenGL, "glVertex3d")))
		OpenGL_Error("OpenGL_Init: Couldn't get glVertex3d proc address");

	if(!(gglVertex3dv = (void (APIENTRY *)(const GLdouble *v))GetProcAddress(hInstOpenGL, "glVertex3dv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glVertex3dv proc address");

	if(!(gglVertex3f = (void (APIENTRY *)(GLfloat x, GLfloat y, GLfloat z))GetProcAddress(hInstOpenGL, "glVertex3f")))
		OpenGL_Error("OpenGL_Init: Couldn't get glVertex3f proc address");

	if(!(gglVertex3fv = (void (APIENTRY *)(const GLfloat *v))GetProcAddress(hInstOpenGL, "glVertex3fv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glVertex3fv proc address");

	if(!(gglVertex3i = (void (APIENTRY *)(GLint x, GLint y, GLint z))GetProcAddress(hInstOpenGL, "glVertex3i")))
		OpenGL_Error("OpenGL_Init: Couldn't get glVertex3i proc address");

	if(!(gglVertex3iv = (void (APIENTRY *)(const GLint *v))GetProcAddress(hInstOpenGL, "glVertex3iv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glVertex3iv proc address");

	if(!(gglVertex3s = (void (APIENTRY *)(GLshort x, GLshort y, GLshort z))GetProcAddress(hInstOpenGL, "glVertex3s")))
		OpenGL_Error("OpenGL_Init: Couldn't get glVertex3s proc address");

	if(!(gglVertex3sv = (void (APIENTRY *)(const GLshort *v))GetProcAddress(hInstOpenGL, "glVertex3sv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glVertex3sv proc address");

	if(!(gglVertex4d = (void (APIENTRY *)(GLdouble x, GLdouble y, GLdouble z, GLdouble w))GetProcAddress(hInstOpenGL, "glVertex4d")))
		OpenGL_Error("OpenGL_Init: Couldn't get glVertex4d proc address");

	if(!(gglVertex4dv = (void (APIENTRY *)(const GLdouble *v))GetProcAddress(hInstOpenGL, "glVertex4dv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glVertex4dv proc address");

	if(!(gglVertex4f = (void (APIENTRY *)(GLfloat x, GLfloat y, GLfloat z, GLfloat w))GetProcAddress(hInstOpenGL, "glVertex4f")))
		OpenGL_Error("OpenGL_Init: Couldn't get glVertex4f proc address");

	if(!(gglVertex4fv = (void (APIENTRY *)(const GLfloat *v))GetProcAddress(hInstOpenGL, "glVertex4fv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glVertex4fv proc address");

	if(!(gglVertex4i = (void (APIENTRY *)(GLint x, GLint y, GLint z, GLint w))GetProcAddress(hInstOpenGL, "glVertex4i")))
		OpenGL_Error("OpenGL_Init: Couldn't get glVertex4i proc address");

	if(!(gglVertex4iv = (void (APIENTRY *)(const GLint *v))GetProcAddress(hInstOpenGL, "glVertex4iv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glVertex4iv proc address");

	if(!(gglVertex4s = (void (APIENTRY *)(GLshort x, GLshort y, GLshort z, GLshort w))GetProcAddress(hInstOpenGL, "glVertex4s")))
		OpenGL_Error("OpenGL_Init: Couldn't get glVertex4s proc address");

	if(!(gglVertex4sv = (void (APIENTRY *)(const GLshort *v))GetProcAddress(hInstOpenGL, "glVertex4sv")))
		OpenGL_Error("OpenGL_Init: Couldn't get glVertex4sv proc address");

	if(!(gglViewport = (void (APIENTRY *)(GLint x, GLint y, GLsizei width, GLsizei height))GetProcAddress(hInstOpenGL, "glViewport")))
		OpenGL_Error("OpenGL_Init: Couldn't get glViewport proc address");

//==========================================================

	if(!(gwglCopyContext = (BOOL (WINAPI *)(HGLRC, HGLRC, UINT))GetProcAddress(hInstOpenGL, "wglCopyContext")))
		OpenGL_Error("OpenGL_Init: Couldn't get wglCopyContext proc address");

	if(!(gwglCreateContext = (HGLRC (WINAPI *)(HDC))GetProcAddress(hInstOpenGL, "wglCreateContext")))
		OpenGL_Error("OpenGL_Init: Couldn't get wglCreateContext proc address");

	if(!(gwglCreateLayerContext = (HGLRC (WINAPI *)(HDC, int))GetProcAddress(hInstOpenGL, "wglCreateLayerContext")))
		OpenGL_Error("OpenGL_Init: Couldn't get wglCreateLayerContext proc address");

	if(!(gwglDeleteContext = (BOOL (WINAPI *)(HGLRC))GetProcAddress(hInstOpenGL, "wglDeleteContext")))
		OpenGL_Error("OpenGL_Init: Couldn't get wglDeleteContext proc address");

	if(!(gwglGetCurrentContext = (HGLRC (WINAPI *)(VOID))GetProcAddress(hInstOpenGL, "wglGetCurrentContext")))
		OpenGL_Error("OpenGL_Init: Couldn't get wglGetCurrentContext proc address");

	if(!(gwglGetCurrentDC = (HDC (WINAPI *)(VOID))GetProcAddress(hInstOpenGL, "wglGetCurrentDC")))
		OpenGL_Error("OpenGL_Init: Couldn't get wglGetCurrentDC proc address");

	if(!(gwglGetProcAddress = (FARPROC (WINAPI *)(LPCSTR))GetProcAddress(hInstOpenGL, "wglGetProcAddress")))
		OpenGL_Error("OpenGL_Init: Couldn't get wglGetProcAddress proc address");

	if(!(gwglMakeCurrent = (BOOL (WINAPI *)(HDC, HGLRC))GetProcAddress(hInstOpenGL, "wglMakeCurrent")))
		OpenGL_Error("OpenGL_Init: Couldn't get wglMakeCurrent proc address");

	if(!(gwglShareLists = (BOOL (WINAPI *)(HGLRC, HGLRC))GetProcAddress(hInstOpenGL, "wglShareLists")))
		OpenGL_Error("OpenGL_Init: Couldn't get wglShareLists proc address");

	if(!(gwglUseFontBitmaps = (BOOL (WINAPI *)(HDC, DWORD, DWORD, DWORD))GetProcAddress(hInstOpenGL, "wglUseFontBitmapsA")))
		OpenGL_Error("OpenGL_Init: Couldn't get wglUseFontBitmaps proc address");

	if(!(gwglUseFontOutlines = (BOOL (WINAPI *)(HDC, DWORD, DWORD, DWORD, FLOAT, FLOAT, int, LPGLYPHMETRICSFLOAT))GetProcAddress(hInstOpenGL, "wglUseFontOutlinesA")))
		OpenGL_Error("OpenGL_Init: Couldn't get wglUseFontOutlines proc address");

	if(!(gwglDescribeLayerPlane = (BOOL (WINAPI *)(HDC, int, int, UINT, LPLAYERPLANEDESCRIPTOR))GetProcAddress(hInstOpenGL, "wglDescribeLayerPlane")))
		OpenGL_Error("OpenGL_Init: Couldn't get wglDescribeLayerPlane proc address");

	if(!(gwglSetLayerPaletteEntries = (int (WINAPI *)(HDC, int, int, int, COLORREF *))GetProcAddress(hInstOpenGL, "wglSetLayerPaletteEntries")))
		OpenGL_Error("OpenGL_Init: Couldn't get wglSetLayerPaletteEntries proc address");

	if(!(gwglGetLayerPaletteEntries = (int (WINAPI *)(HDC, int, int, int, COLORREF *))GetProcAddress(hInstOpenGL, "wglGetLayerPaletteEntries")))
		OpenGL_Error("OpenGL_Init: Couldn't get wglGetLayerPaletteEntries proc address");

	if(!(gwglRealizeLayerPalette = (BOOL (WINAPI *)(HDC, int, BOOL))GetProcAddress(hInstOpenGL, "wglRealizeLayerPalette")))
		OpenGL_Error("OpenGL_Init: Couldn't get wglRealizeLayerPalette proc address");

	if(!(gwglSwapLayerBuffers = (BOOL (WINAPI *)(HDC, UINT))GetProcAddress(hInstOpenGL, "wglSwapLayerBuffers")))
		OpenGL_Error("OpenGL_Init: Couldn't get wglSwapLayerBuffers proc address");

//==========================================================

	if(retdriver == GLDRIVER_DEFAULT)
	{
		if(!hInstGDI)
		{
			if(!(hInstGDI = LoadLibrary("gdi32.dll")))
				OpenGL_Error("OpenGL_Init: Couldn't load gdi32.dll");
		}

		if(!(gwglChoosePixelFormat = (int (WINAPI *)(HDC, CONST PIXELFORMATDESCRIPTOR *))GetProcAddress(hInstGDI, "ChoosePixelFormat")))
			OpenGL_Error("OpenGL_Init: Couldn't get ChoosePixelFormat proc address");

		if(!(gwglDescribePixelFormat = (int (WINAPI *) (HDC, int, UINT, LPPIXELFORMATDESCRIPTOR))GetProcAddress(hInstGDI, "DescribePixelFormat")))
			OpenGL_Error("OpenGL_Init: Couldn't get DescribePixelFormat proc address");

		if(!(gwglGetPixelFormat = (int (WINAPI *)(HDC))GetProcAddress(hInstGDI, "GetPixelFormat")))
			OpenGL_Error("OpenGL_Init: Couldn't get GetPixelFormat proc address");

		if(!(gwglSetPixelFormat = (BOOL (WINAPI *)(HDC, int, CONST PIXELFORMATDESCRIPTOR *))GetProcAddress(hInstGDI, "SetPixelFormat")))
			OpenGL_Error("OpenGL_Init: Couldn't get SetPixelFormat proc address");

		if(!(gwglSwapBuffers = (BOOL (WINAPI *)(HDC))GetProcAddress(hInstGDI, "SwapBuffers")))
			OpenGL_Error("OpenGL_Init: Couldn't get SwapBuffers proc address");
	}
	else
	{
		if(!(gwglChoosePixelFormat = (int (WINAPI *)(HDC, CONST PIXELFORMATDESCRIPTOR *))GetProcAddress(hInstOpenGL, "wglChoosePixelFormat")))
			OpenGL_Error("OpenGL_Init: Couldn't get wglChoosePixelFormat proc address");

		if(!(gwglDescribePixelFormat = (int (WINAPI *) (HDC, int, UINT, LPPIXELFORMATDESCRIPTOR))GetProcAddress(hInstOpenGL, "wglDescribePixelFormat")))
			OpenGL_Error("OpenGL_Init: Couldn't get wglDescribePixelFormat proc address");

		if(!(gwglGetPixelFormat = (int (WINAPI *)(HDC))GetProcAddress(hInstOpenGL, "wglGetPixelFormat")))
			OpenGL_Error("OpenGL_Init: Couldn't get wglGetPixelFormat proc address");

		if(!(gwglSetPixelFormat = (BOOL (WINAPI *)(HDC, int, CONST PIXELFORMATDESCRIPTOR *))GetProcAddress(hInstOpenGL, "wglSetPixelFormat")))
			OpenGL_Error("OpenGL_Init: Couldn't get wglSetPixelFormat proc address");

		if(!(gwglSwapBuffers = (BOOL (WINAPI *)(HDC))GetProcAddress(hInstOpenGL, "wglSwapBuffers")))
			OpenGL_Error("OpenGL_Init: Couldn't get wglSwapBuffers proc address");
	}

	return retdriver;		//successfully dynloaded an OpenGL driver
}

/*
----------------
OpenGL_Shutdown
----------------
*/
void OpenGL_Shutdown(void)
{
	if(hInstGDI)
	{
		FreeLibrary(hInstGDI);
		hInstGDI = NULL;
	}

	if(hInstOpenGL)
	{
		FreeLibrary(hInstOpenGL);
		hInstOpenGL = NULL;
	}

	gwglCopyContext = NULL;
	gwglCreateContext = NULL;
	gwglCreateLayerContext = NULL;
	gwglDeleteContext = NULL;
	gwglGetCurrentContext = NULL;
	gwglGetCurrentDC = NULL;
	gwglGetProcAddress = NULL;
	gwglMakeCurrent = NULL;
	gwglShareLists = NULL;
	gwglUseFontBitmaps = NULL;
	gwglUseFontOutlines = NULL;
	gwglDescribeLayerPlane = NULL;
	gwglSetLayerPaletteEntries = NULL;
	gwglGetLayerPaletteEntries = NULL;
	gwglRealizeLayerPalette = NULL;
	gwglSwapLayerBuffers = NULL;

	gwglChoosePixelFormat = NULL;
	gwglDescribePixelFormat = NULL;
	gwglGetPixelFormat = NULL;
	gwglSetPixelFormat = NULL;
	gwglSwapBuffers = NULL;

	gglAccum = NULL;
	gglAlphaFunc = NULL;
	gglAreTexturesResident = NULL;
	gglArrayElement = NULL;
	gglBegin = NULL;
	gglBindTexture = NULL;
	gglBitmap = NULL;
	gglBlendFunc = NULL;
	gglCallList = NULL;
	gglCallLists = NULL;
	gglClear = NULL;
	gglClearAccum = NULL;
	gglClearColor = NULL;
	gglClearDepth = NULL;
	gglClearIndex = NULL;
	gglClearStencil = NULL;
	gglClipPlane = NULL;
	gglColor3b = NULL;
	gglColor3bv = NULL;
	gglColor3d = NULL;
	gglColor3dv = NULL;
	gglColor3f = NULL;
	gglColor3fv = NULL;
	gglColor3i = NULL;
	gglColor3iv = NULL;
	gglColor3s = NULL;
	gglColor3sv = NULL;
	gglColor3ub = NULL;
	gglColor3ubv = NULL;
	gglColor3ui = NULL;
	gglColor3uiv = NULL;
	gglColor3us = NULL;
	gglColor3usv = NULL;
	gglColor4b = NULL;
	gglColor4bv = NULL;
	gglColor4d = NULL;
	gglColor4dv = NULL;
	gglColor4f = NULL;
	gglColor4fv = NULL;
	gglColor4i = NULL;
	gglColor4iv = NULL;
	gglColor4s = NULL;
	gglColor4sv = NULL;
	gglColor4ub = NULL;
	gglColor4ubv = NULL;
	gglColor4ui = NULL;
	gglColor4uiv = NULL;
	gglColor4us = NULL;
	gglColor4usv = NULL;
	gglColorMask = NULL;
	gglColorMaterial = NULL;
	gglColorPointer = NULL;
	gglCopyPixels = NULL;
	gglCopyTexImage1D = NULL;
	gglCopyTexImage2D = NULL;
	gglCopyTexSubImage1D = NULL;
	gglCopyTexSubImage2D = NULL;
	gglCullFace = NULL;
	gglDeleteLists = NULL;
	gglDeleteTextures = NULL;
	gglDepthFunc = NULL;
	gglDepthMask = NULL;
	gglDepthRange = NULL;
	gglDisable = NULL;
	gglDrawArrays = NULL;
	gglDrawBuffer = NULL;
	gglDrawPixels = NULL;
	gglEdgeFlag = NULL;
	gglEdgeFlagPointer = NULL;
	gglEdgeFlagv = NULL;
	gglEnable = NULL;
	gglEnd = NULL;
	gglEndList = NULL;
	gglEvalCoord1d = NULL;
	gglEvalCoord1dv = NULL;
	gglEvalCoord1f = NULL;
	gglEvalCoord1fv = NULL;
	gglEvalCoord2d = NULL;
	gglEvalCoord2dv = NULL;
	gglEvalCoord2f = NULL;
	gglEvalCoord2fv = NULL;
	gglEvalMesh1 = NULL;
	gglEvalMesh2 = NULL;
	gglEvalPoint1 = NULL;
	gglEvalPoint2 = NULL;
	gglFeedbackBuffer = NULL;
	gglFinish = NULL;
	gglFlush = NULL;
	gglFogf = NULL;
	gglFogfv = NULL;
	gglFogi = NULL;
	gglFogiv = NULL;
	gglFrontFace = NULL;
	gglFrustum = NULL;
	gglGenLists = NULL;
	gglGenTextures = NULL;
	gglGetBooleanv = NULL;
	gglGetClipPlane = NULL;
	gglGetDoublev = NULL;
	gglGetError = NULL;
	gglGetFloatv = NULL;
	gglGetIntegerv = NULL;
	gglGetLightfv = NULL;
	gglGetLightiv = NULL;
	gglGetMapdv = NULL;
	gglGetMapfv = NULL;
	gglGetMapiv = NULL;
	gglGetMaterialfv = NULL;
	gglGetMaterialiv = NULL;
	gglGetPixelMapfv = NULL;
	gglGetPixelMapuiv = NULL;
	gglGetPixelMapusv = NULL;
	gglGetPointerv = NULL;
	gglGetPolygonStipple = NULL;
	gglGetString = NULL;
	gglGetTexEnvfv = NULL;
	gglGetTexEnviv = NULL;
	gglGetTexGendv = NULL;
	gglGetTexGenfv = NULL;
	gglGetTexGeniv = NULL;
	gglGetTexImage = NULL;
	gglGetTexLevelParameterfv = NULL;
	gglGetTexLevelParameteriv = NULL;
	gglGetTexParameterfv = NULL;
	gglGetTexParameteriv = NULL;
	gglHint = NULL;
	gglIndexMask = NULL;
	gglIndexPointer = NULL;
	gglIndexd = NULL;
	gglIndexdv = NULL;
	gglIndexf = NULL;
	gglIndexfv = NULL;
	gglIndexi = NULL;
	gglIndexiv = NULL;
	gglIndexs = NULL;
	gglIndexsv = NULL;
	gglInitNames = NULL;
	gglIsEnabled = NULL;
	gglIsList = NULL;
	gglIsTexture = NULL;
	gglLightModelf = NULL;
	gglLightModelfv = NULL;
	gglLightModeli = NULL;
	gglLightModeliv = NULL;
	gglLightf = NULL;
	gglLightfv = NULL;
	gglLighti = NULL;
	gglLightiv = NULL;
	gglLineStipple = NULL;
	gglLineWidth = NULL;
	gglListBase = NULL;
	gglLoadIdentity = NULL;
	gglLoadMatrixd = NULL;
	gglLoadMatrixf = NULL;
	gglLoadName = NULL;
	gglLogicOp = NULL;
	gglMap1d = NULL;
	gglMap1f = NULL;
	gglMap2d = NULL;
	gglMap2f = NULL;
	gglMapGrid1d = NULL;
	gglMapGrid1f = NULL;
	gglMapGrid2d = NULL;
	gglMapGrid2f = NULL;
	gglMaterialf = NULL;
	gglMaterialfv = NULL;
	gglMateriali = NULL;
	gglMaterialiv = NULL;
	gglMatrixMode = NULL;
	gglMultMatrixd = NULL;
	gglMultMatrixf = NULL;
	gglNewList = NULL;
	gglNormal3b = NULL;
	gglNormal3bv = NULL;
	gglNormal3d = NULL;
	gglNormal3dv = NULL;
	gglNormal3f = NULL;
	gglNormal3fv = NULL;
	gglNormal3i = NULL;
	gglNormal3iv = NULL;
	gglNormal3s = NULL;
	gglNormal3sv = NULL;
	gglNormalPointer = NULL;
	gglOrtho = NULL;
	gglPassThrough = NULL;
	gglPixelMapfv = NULL;
	gglPixelMapuiv = NULL;
	gglPixelMapusv = NULL;
	gglPixelStoref = NULL;
	gglPixelStorei = NULL;
	gglPixelTransferf = NULL;
	gglPixelTransferi = NULL;
	gglPixelZoom = NULL;
	gglPointSize = NULL;
	gglPolygonMode = NULL;
	gglPolygonStipple = NULL;
	gglPopAttrib = NULL;
	gglPopMatrix = NULL;
	gglPopName = NULL;
	gglPrioritizeTextures = NULL;
	gglPushAttrib = NULL;
	gglPushMatrix = NULL;
	gglPushName = NULL;
	gglRasterPos2d = NULL;
	gglRasterPos2dv = NULL;
	gglRasterPos2f = NULL;
	gglRasterPos2fv = NULL;
	gglRasterPos2i = NULL;
	gglRasterPos2iv = NULL;
	gglRasterPos2s = NULL;
	gglRasterPos2sv = NULL;
	gglRasterPos3d = NULL;
	gglRasterPos3dv = NULL;
	gglRasterPos3f = NULL;
	gglRasterPos3fv = NULL;
	gglRasterPos3i = NULL;
	gglRasterPos3iv = NULL;
	gglRasterPos3s = NULL;
	gglRasterPos3sv = NULL;
	gglRasterPos4d = NULL;
	gglRasterPos4dv = NULL;
	gglRasterPos4f = NULL;
	gglRasterPos4fv = NULL;
	gglRasterPos4i = NULL;
	gglRasterPos4iv = NULL;
	gglRasterPos4s = NULL;
	gglRasterPos4sv = NULL;
	gglReadBuffer = NULL;
	gglReadPixels = NULL;
	gglRectd = NULL;
	gglRectdv = NULL;
	gglRectf = NULL;
	gglRectfv = NULL;
	gglRecti = NULL;
	gglRectiv = NULL;
	gglRects = NULL;
	gglRectsv = NULL;
	gglRenderMode = NULL;
	gglRotated = NULL;
	gglRotatef = NULL;
	gglScaled = NULL;
	gglScalef = NULL;
	gglScissor = NULL;
	gglSelectBuffer = NULL;
	gglShadeModel = NULL;
	gglStencilFunc = NULL;
	gglStencilMask = NULL;
	gglStencilOp = NULL;
	gglTexCoord1d = NULL;
	gglTexCoord1dv = NULL;
	gglTexCoord1f = NULL;
	gglTexCoord1fv = NULL;
	gglTexCoord1i = NULL;
	gglTexCoord1iv = NULL;
	gglTexCoord1s = NULL;
	gglTexCoord1sv = NULL;
	gglTexCoord2d = NULL;
	gglTexCoord2dv = NULL;
	gglTexCoord2f = NULL;
	gglTexCoord2fv = NULL;
	gglTexCoord2i = NULL;
	gglTexCoord2iv = NULL;
	gglTexCoord2s = NULL;
	gglTexCoord2sv = NULL;
	gglTexCoord3d = NULL;
	gglTexCoord3dv = NULL;
	gglTexCoord3f = NULL;
	gglTexCoord3fv = NULL;
	gglTexCoord3i = NULL;
	gglTexCoord3iv = NULL;
	gglTexCoord3s = NULL;
	gglTexCoord3sv = NULL;
	gglTexCoord4d = NULL;
	gglTexCoord4dv = NULL;
	gglTexCoord4f = NULL;
	gglTexCoord4fv = NULL;
	gglTexCoord4i = NULL;
	gglTexCoord4iv = NULL;
	gglTexCoord4s = NULL;
	gglTexCoord4sv = NULL;
	gglTexCoordPointer = NULL;
	gglTexEnvf = NULL;
	gglTexEnvfv = NULL;
	gglTexEnvi = NULL;
	gglTexEnviv = NULL;
	gglTexGend = NULL;
	gglTexGendv = NULL;
	gglTexGenf = NULL;
	gglTexGenfv = NULL;
	gglTexGeni = NULL;
	gglTexGeniv = NULL;
	gglTexImage1D = NULL;
	gglTexImage2D = NULL;
	gglTexParameterf = NULL;
	gglTexParameterfv = NULL;
	gglTexParameteri = NULL;
	gglTexParameteriv = NULL;
	gglTexSubImage1D = NULL;
	gglTexSubImage2D = NULL;
	gglTranslated = NULL;
	gglTranslatef = NULL;
	gglVertex2d = NULL;
	gglVertex2dv = NULL;
	gglVertex2f = NULL;
	gglVertex2fv = NULL;
	gglVertex2i = NULL;
	gglVertex2iv = NULL;
	gglVertex2s = NULL;
	gglVertex2sv = NULL;
	gglVertex3d = NULL;
	gglVertex3dv = NULL;
	gglVertex3f = NULL;
	gglVertex3fv = NULL;
	gglVertex3i = NULL;
	gglVertex3iv = NULL;
	gglVertex3s = NULL;
	gglVertex3sv = NULL;
	gglVertex4d = NULL;
	gglVertex4dv = NULL;
	gglVertex4f = NULL;
	gglVertex4fv = NULL;
	gglVertex4i = NULL;
	gglVertex4iv = NULL;
	gglVertex4s = NULL;
	gglVertex4sv = NULL;
	gglVertexPointer = NULL;
	gglViewport = NULL;

	OpenGL_Error = NULL;
}

/*
----------------
OpenGL_TempExtensionsCallback

- placeholder callback function if none is specified
----------------
*/
static int OpenGL_TempExtensionsCallback(const char *name, gl_extensions_t extension, void *element)
{
	return GLENUMERATION_TRUE;
}

/*
----------------
OpenGL_EnableExtensions

- assumes the OpenGL subsystem has been properly initialized
----------------
*/
void OpenGL_EnableExtensions(HDC hDC, int (*OpenGL_ExtensionsCallback)(const char *name, gl_extensions_t extension, void *reserved), void *element)
{
	int (*OpenGL_ExtensionsEnumeration)(const char *name, gl_extensions_t extension, void *reserved);

	if(!hDC)
	{
		OpenGL_Error("OpenGL_EnableExtensions: OpenGL subsystem hasn't been initialized yet");
		return;
	}

	if(!OpenGL_ExtensionsCallback)
		OpenGL_ExtensionsEnumeration = OpenGL_TempExtensionsCallback;
	else
		OpenGL_ExtensionsEnumeration = OpenGL_ExtensionsCallback;

	//reset all extensions
	OpenGL_DisableExtensions();

	if(!gglGetString)
	{
		OpenGL_Error("OpenGL_EnableExtensions: OpenGL driver hasn't been loaded yet");
		return;
	}

#pragma warning(push)
#pragma warning(disable: 4996)

	vendor = strdup((char *)gglGetString(GL_VENDOR));
	renderer = strdup((char *)gglGetString(GL_RENDERER));
	version = strdup((char *)gglGetString(GL_VERSION));
	extensions = strdup((char *)gglGetString(GL_EXTENSIONS));

	OpenGL_GetVersion();

	if(gwglGetExtensionsStringARB = (const char *(WINAPI *)(HDC hdc))gwglGetProcAddress("wglGetExtensionsStringARB"))
		wglextensions = strdup((char *)gwglGetExtensionsStringARB(hDC));

	if((gwglGetExtensionsStringEXT = (const char *(WINAPI *)(void))gwglGetProcAddress("wglGetExtensionsStringEXT")) && !gwglGetExtensionsStringARB)
		wglextensions = strdup((char *)gwglGetExtensionsStringEXT());

#pragma warning(pop)

	if(!wglextensions)
		wglextensions = extensions;

	if(StringToken(wglextensions, "WGL_ARB_buffer_region") /* && gwglGetExtensionsStringEXT*/)
	{
		gwglCreateBufferRegionARB = (HANDLE (WINAPI *)(HDC hDC, int iLayerPlane, UINT uType))gwglGetProcAddress("wglCreateBufferRegionARB");
		gwglDeleteBufferRegionARB = (VOID (WINAPI *)(HANDLE hRegion))gwglGetProcAddress("wglDeleteBufferRegionARB");
		gwglSaveBufferRegionARB = (BOOL (WINAPI *)(HANDLE hRegion, int x, int y, int width, int height))gwglGetProcAddress("wglSaveBufferRegionARB");
		gwglRestoreBufferRegionARB = (BOOL (WINAPI *)(HANDLE hRegion, int x, int y, int width, int height, int xSrc, int ySrc))gwglGetProcAddress("wglRestoreBufferRegionARB");

		if(OpenGL_ExtensionsEnumeration("WGL_ARB_buffer_region", GWGL_ARB_buffer_region, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(wglextensions, "WGL_ARB_multisample") /* && StringToken(wglextensions, "WGL_EXT_pixel_format") && gwglGetExtensionsStringEXT*/)
	{
		if(OpenGL_ExtensionsEnumeration("WGL_ARB_multisample", GWGL_ARB_multisample, element) == GLENUMERATION_FALSE)
			return;
	}

	if(gwglGetExtensionsStringARB)
	{
		if(OpenGL_ExtensionsEnumeration("WGL_ARB_extensions_string", GWGL_ARB_extensions_string, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(wglextensions, "WGL_ARB_pixel_format") /* && gwglGetExtensionsStringARB*/)
	{
		gwglGetPixelFormatAttribivARB = (BOOL (WINAPI *)(HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int *piAttributes, int *piValues))gwglGetProcAddress("wglGetPixelFormatAttribivARB");
		gwglGetPixelFormatAttribfvARB = (BOOL (WINAPI *)(HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int *piAttributes, FLOAT *pfValues))gwglGetProcAddress("wglGetPixelFormatAttribfvARB");
		gwglChoosePixelFormatARB = (BOOL (WINAPI *)(HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats))gwglGetProcAddress("wglChoosePixelFormatARB");

		if(OpenGL_ExtensionsEnumeration("WGL_ARB_pixel_format", GWGL_ARB_pixel_format, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(wglextensions, "WGL_ARB_make_current_read") /* && gwglGetExtensionsStringARB*/)
	{
		gwglMakeContextCurrentARB = (BOOL (WINAPI *)(HDC hDrawDC, HDC hReadDC, HGLRC hglrc))gwglGetProcAddress("wglMakeContextCurrentARB");
		gwglGetCurrentReadDCARB = (HDC (WINAPI *)(void))gwglGetProcAddress("wglGetCurrentReadDCARB");

		if(OpenGL_ExtensionsEnumeration("WGL_ARB_make_current_read", GWGL_ARB_make_current_read, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(wglextensions, "WGL_ARB_pbuffer") /* && gwglGetExtensionsStringARB && StringToken(wglextensions, "WGL_ARB_pixel_format") && StringToken(wglextensions, "WGL_ARB_make_current_read")*/)
	{
		gwglCreatePbufferARB = (HPBUFFERARB (WINAPI *)(HDC hDC, int iPixelFormat, int iWidth, int iHeight, const int *piAttribList))gwglGetProcAddress("wglCreatePbufferARB");
		gwglGetPbufferDCARB = (HDC (WINAPI *)(HPBUFFERARB hPbuffer))gwglGetProcAddress("wglGetPbufferDCARB");
		gwglReleasePbufferDCARB = (int (WINAPI *)(HPBUFFERARB hPbuffer, HDC hDC))gwglGetProcAddress("wglReleasePbufferDCARB");
		gwglDestroyPbufferARB = (BOOL (WINAPI *)(HPBUFFERARB hPbuffer))gwglGetProcAddress("wglDestroyPbufferARB");
		gwglQueryPbufferARB = (BOOL (WINAPI *)(HPBUFFERARB hPbuffer, int iAttribute, int *piValue))gwglGetProcAddress("wglQueryPbufferARB");

		if(OpenGL_ExtensionsEnumeration("WGL_ARB_pbuffer", GWGL_ARB_pbuffer, element) == GLENUMERATION_FALSE)
			return;
	}

	//WGL_ARB_make_current_read && GL_ARB_texture_cube_map are not required
	if(/*atof(version) >= 1.1 && */StringToken(wglextensions, "WGL_ARB_render_texture") /* && gwglGetExtensionsStringARB && StringToken(wglextensions, "WGL_ARB_pixel_format") && StringToken(wglextensions, "WGL_ARB_pbuffer")*//* && StringToken(wglextensions, "WGL_ARB_make_current_read") && StringToken(extensions, "GL_ARB_texture_cube_map")*/)
	{
		gwglBindTexImageARB = (BOOL (WINAPI *)(HPBUFFERARB hPbuffer, int iBuffer))gwglGetProcAddress("wglBindTexImageARB");
		gwglReleaseTexImageARB = (BOOL (WINAPI *)(HPBUFFERARB hPbuffer, int iBuffer))gwglGetProcAddress("wglReleaseTexImageARB");
		gwglSetPbufferAttribARB = (BOOL (WINAPI *)(HPBUFFERARB hPbuffer, const int *piAttribList))gwglGetProcAddress("wglSetPbufferAttribARB");

		if(OpenGL_ExtensionsEnumeration("WGL_ARB_render_texture", GWGL_ARB_render_texture, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(wglextensions, "WGL_ARB_pixel_format_float"))
	{
		if(OpenGL_ExtensionsEnumeration("WGL_ARB_pixel_format_float", GWGL_ARB_pixel_format_float, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(wglextensions, "WGL_ARB_framebuffer_sRGB"))
	{
		if(OpenGL_ExtensionsEnumeration("WGL_ARB_framebuffer_sRGB", GWGL_ARB_framebuffer_sRGB, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(wglextensions, "WGL_ARB_create_context"))
	{
		gwglCreateContextAttribsARB = (HGLRC (WINAPI *)(HDC hDC, HGLRC hShareContext, const int *attribList))gwglGetProcAddress("wglCreateContextAttribsARB");

		if(OpenGL_ExtensionsEnumeration("WGL_ARB_create_context", GWGL_ARB_create_context, element) == GLENUMERATION_FALSE)
			return;
	}

	//XXX: not complete yet
	if(StringToken(wglextensions, "WGL_EXT_display_color_table"))
	{
		gwglCreateDisplayColorTableEXT = (GLboolean (WINAPI *)(GLushort id))gwglGetProcAddress("wglCreateDisplayColorTableEXT");
		gwglLoadDisplayColorTableEXT = (GLboolean (WINAPI *)(const GLushort *table, GLuint length))gwglGetProcAddress("wglLoadDisplayColorTableEXT");
		gwglBindDisplayColorTableEXT = (GLboolean (WINAPI *)(GLushort id))gwglGetProcAddress("wglBindDisplayColorTableEXT");
		gwglDestroyDisplayColorTableEXT = (VOID (WINAPI *)(GLushort id))gwglGetProcAddress("wglDestroyDisplayColorTableEXT");

		if(OpenGL_ExtensionsEnumeration("WGL_EXT_display_color_table", GWGL_EXT_display_color_table, element) == GLENUMERATION_FALSE)
			return;
	}

	if(gwglGetExtensionsStringEXT)
	{
		if(OpenGL_ExtensionsEnumeration("WGL_EXT_extensions_string", GWGL_EXT_extensions_string, element) == GLENUMERATION_FALSE)
			return;
	}
	
	if(StringToken(wglextensions, "WGL_EXT_make_current_read") /* && gwglGetExtensionsStringEXT*/)
	{
		gwglMakeContextCurrentEXT = (BOOL (WINAPI *)(HDC hDrawDC, HDC hReadDC, HGLRC hglrc))gwglGetProcAddress("wglMakeContextCurrentEXT");
		gwglGetCurrentReadDCEXT = (HDC (WINAPI *)(void))gwglGetProcAddress("wglGetCurrentReadDCEXT");

		if(OpenGL_ExtensionsEnumeration("WGL_EXT_make_current_read", GWGL_EXT_make_current_read, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(wglextensions, "WGL_EXT_pixel_format") /* && gwglGetExtensionsStringEXT*/)
	{
		gwglGetPixelFormatAttribivEXT = (BOOL (WINAPI *)(HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, int *piAttributes, int *piValues))gwglGetProcAddress("wglGetPixelFormatAttribivEXT");
		gwglGetPixelFormatAttribfvEXT = (BOOL (WINAPI *)(HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, int *piAttributes, FLOAT *pfValues))gwglGetProcAddress("wglGetPixelFormatAttribfvEXT");
		gwglChoosePixelFormatEXT = (BOOL (WINAPI *)(HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats))gwglGetProcAddress("wglChoosePixelFormatEXT");

		if(OpenGL_ExtensionsEnumeration("WGL_EXT_pixel_format", GWGL_EXT_pixel_format, element) == GLENUMERATION_FALSE)
			return;
	}

	//WGL_EXT_make_current_read is not required
	if(StringToken(wglextensions, "WGL_EXT_pbuffer") /* && gwglGetExtensionsStringEXT && StringToken(wglextensions, "WGL_EXT_pixel_format")*//* && StringToken(wglextensions, "WGL_EXT_make_current_read")*/)
	{
		gwglCreatePbufferEXT = (HPBUFFEREXT (WINAPI *)(HDC hDC, int iPixelFormat, int iWidth, int iHeight, const int *piAttribList))gwglGetProcAddress("wglCreatePbufferEXT");
		gwglGetPbufferDCEXT = (HDC (WINAPI *)(HPBUFFEREXT hPbuffer))gwglGetProcAddress("wglGetPbufferDCEXT");
		gwglReleasePbufferDCEXT = (int (WINAPI *)(HPBUFFEREXT hPbuffer, HDC hDC))gwglGetProcAddress("wglReleasePbufferDCEXT");
		gwglDestroyPbufferEXT = (BOOL (WINAPI *)(HPBUFFEREXT hPbuffer))gwglGetProcAddress("wglDestroyPbufferEXT");
		gwglQueryPbufferEXT = (BOOL (WINAPI *)(HPBUFFEREXT hPbuffer, int iAttribute, int *piValue))gwglGetProcAddress("wglQueryPbufferEXT");

		if(OpenGL_ExtensionsEnumeration("WGL_EXT_pbuffer", GWGL_EXT_pbuffer, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(wglextensions, "WGL_EXT_swap_control") /* && gwglGetExtensionsStringEXT*/)
	{
		gwglSwapIntervalEXT = (BOOL (WINAPI *)(int interval))gwglGetProcAddress("wglSwapIntervalEXT");
		gwglGetSwapIntervalEXT = (int (WINAPI *)(void))gwglGetProcAddress("wglGetSwapIntervalEXT");

		if(OpenGL_ExtensionsEnumeration("WGL_EXT_swap_control", GWGL_EXT_swap_control, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(wglextensions, "WGL_EXT_depth_float") /* && gwglGetExtensionsStringEXT && StringToken(wglextensions, "WGL_EXT_pixel_format")*/)
	{
		if(OpenGL_ExtensionsEnumeration("WGL_EXT_depth_float", GWGL_EXT_depth_float, element) == GLENUMERATION_FALSE)
			return;
	}

	//TODO: not a WGL extension
	if(StringToken(wglextensions, "WGL_NV_vertex_array_range"))
	{
		gwglAllocateMemoryNV = (void *(WINAPI *)(GLsizei size, GLfloat readfreq, GLfloat writefreq, GLfloat priority))gwglGetProcAddress("wglAllocateMemoryNV");
		gwglFreeMemoryNV = (void (WINAPI *)(void *pointer))gwglGetProcAddress("wglFreeMemoryNV");

		if(OpenGL_ExtensionsEnumeration("WGL_NV_vertex_array_range", GWGL_NV_vertex_array_range, element) == GLENUMERATION_FALSE)
			return;
	}

	//TODO: requires GLX 1.2
	if(/*atof(version) >= 1.1 && */StringToken(wglextensions, "WGL_3DFX_multisample") /* && gwglGetExtensionsStringARB && StringToken(wglextensions, "WGL_ARB_pixel_format")*/)
	{
		if(OpenGL_ExtensionsEnumeration("WGL_3DFX_multisample", GWGL_3DFX_multisample, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(wglextensions, "WGL_EXT_multisample") /* && gwglGetExtensionsStringEXT && StringToken(wglextensions, "WGL_EXT_pixel_format")*/)
	{
		gwglSampleMaskEXT = (void (WINAPI *)(GLclampf value, GLboolean invert))gwglGetProcAddress("wglSampleMaskEXT");
		gwglSamplePatternEXT = (void (WINAPI *)(GLenum pattern))gwglGetProcAddress("wglSamplePatternEXT");

		if(OpenGL_ExtensionsEnumeration("WGL_EXT_multisample", GWGL_EXT_multisample, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(wglextensions, "WGL_OML_sync_control") /* && gwglGetExtensionsStringARB*/)
	{
		gwglGetSyncValuesOML = (BOOL (WINAPI *)(HDC hdc, INT64 *ust, INT64 *msc, INT64 *sbc))gwglGetProcAddress("wglGetSyncValuesOML");
		gwglGetMscRateOML = (BOOL (WINAPI *)(HDC hdc, INT32 *numerator, INT32 *denominator))gwglGetProcAddress("wglGetMscRateOML");
		gwglSwapBuffersMscOML = (INT64 (WINAPI *)(HDC hdc, INT64 target_msc, INT64 divisor, INT64 remainder))gwglGetProcAddress("wglSwapBuffersMscOML");
		gwglSwapLayerBuffersMscOML = (INT64 (WINAPI *)(HDC hdc, int fuPlanes, INT64 target_msc, INT64 divisor, INT64 remainder))gwglGetProcAddress("wglSwapLayerBuffersMscOML");
		gwglWaitForMscOML = (BOOL (WINAPI *)(HDC hdc, INT64 target_msc, INT64 divisor, INT64 remainder, INT64 *ust, INT64 *msc, INT64 *sbc))gwglGetProcAddress("wglWaitForMscOML");
		gwglWaitForSbcOML = (BOOL (WINAPI *)(HDC hdc, INT64 target_sbc, INT64 *ust, INT64 *msc, INT64 *sbc))gwglGetProcAddress("wglWaitForSbcOML");

		if(OpenGL_ExtensionsEnumeration("WGL_OML_sync_control", GWGL_OML_sync_control, element) == GLENUMERATION_FALSE)
			return;
	}

	//WGL_I3D_gamma isn't required
	if(StringToken(wglextensions, "WGL_I3D_digital_video_control") /* && gwglGetExtensionsStringEXT*//* && StringToken(wglextensions, "WGL_I3D_gamma")*/)
	{
		gwglGetDigitalVideoParametersI3D = (BOOL (WINAPI *)(HDC hDC, int iAttribute, int *piValue))gwglGetProcAddress("wglGetDigitalVideoParametersI3D");
		gwglSetDigitalVideoParametersI3D = (BOOL (WINAPI *)(HDC hDC, int iAttribute, const int *piValue))gwglGetProcAddress("wglSetDigitalVideoParametersI3D");

		if(OpenGL_ExtensionsEnumeration("WGL_I3D_digital_video_control", GWGL_I3D_digital_video_control, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(wglextensions, "WGL_I3D_gamma") /* && gwglGetExtensionsStringEXT*/)
	{
		gwglGetGammaTableParametersI3D = (BOOL (WINAPI *)(HDC hDC, int iAttribute, int *piValue))gwglGetProcAddress("wglGetGammaTableParametersI3D");
		gwglSetGammaTableParametersI3D = (BOOL (WINAPI *)(HDC hDC, int iAttribute, const int *piValue))gwglGetProcAddress("wglSetGammaTableParametersI3D");
		gwglGetGammaTableI3D = (BOOL (WINAPI *)(HDC hDC, int iEntries, USHORT *puRed, USHORT *puGreen, USHORT *puBlue))gwglGetProcAddress("wglGetGammaTableI3D");
		gwglSetGammaTableI3D = (BOOL (WINAPI *)(HDC hDC, int iEntries, const USHORT *puRed, const USHORT *puGreen, const USHORT *puBlue))gwglGetProcAddress("wglSetGammaTableI3D");

		if(OpenGL_ExtensionsEnumeration("WGL_I3D_gamma", GWGL_I3D_gamma, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(wglextensions, "WGL_I3D_genlock") /* && gwglGetExtensionsStringEXT*/)
	{
		gwglEnableGenlockI3D = (BOOL (WINAPI *)(HDC hDC))gwglGetProcAddress("wglEnableGenlockI3D");
		gwglDisableGenlockI3D = (BOOL (WINAPI *)(HDC hDC))gwglGetProcAddress("wglDisableGenlockI3D");
		gwglIsEnabledGenlockI3D = (BOOL (WINAPI *)(HDC hDC, BOOL *pFlag))gwglGetProcAddress("wglIsEnabledGenlockI3D");
		gwglGenlockSourceI3D = (BOOL (WINAPI *)(HDC hDC, UINT uSource))gwglGetProcAddress("wglGenlockSourceI3D");
		gwglGetGenlockSourceI3D = (BOOL (WINAPI *)(HDC hDC, UINT *uSource))gwglGetProcAddress("wglGetGenlockSourceI3D");
		gwglGenlockSourceEdgeI3D = (BOOL (WINAPI *)(HDC hDC, UINT uEdge))gwglGetProcAddress("wglGenlockSourceEdgeI3D");
		gwglGetGenlockSourceEdgeI3D = (BOOL (WINAPI *)(HDC hDC, UINT *uEdge))gwglGetProcAddress("wglGetGenlockSourceEdgeI3D");
		gwglGenlockSampleRateI3D = (BOOL (WINAPI *)(HDC hDC, UINT uRate))gwglGetProcAddress("wglGenlockSampleRateI3D");
		gwglGetGenlockSampleRateI3D = (BOOL (WINAPI *)(HDC hDC, UINT *uRate))gwglGetProcAddress("wglGetGenlockSampleRateI3D");
		gwglGenlockSourceDelayI3D = (BOOL (WINAPI *)(HDC hDC, UINT uDelay))gwglGetProcAddress("wglGenlockSourceDelayI3D");
		gwglGetGenlockSourceDelayI3D = (BOOL (WINAPI *)(HDC hDC, UINT *uDelay))gwglGetProcAddress("wglGetGenlockSourceDelayI3D");
		gwglQueryGenlockMaxSourceDelayI3D = (BOOL (WINAPI *)(HDC hDC, UINT *uMaxLineDelay, UINT *uMaxPixelDelay))gwglGetProcAddress("wglQueryGenlockMaxSourceDelayI3D");

		if(OpenGL_ExtensionsEnumeration("WGL_I3D_genlock", GWGL_I3D_genlock, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(wglextensions, "WGL_I3D_image_buffer") /* && gwglGetExtensionsStringEXT*/)
	{
		gwglCreateImageBufferI3D = (LPVOID (WINAPI *)(HDC hDC, DWORD dwSize, UINT uFlags))gwglGetProcAddress("wglCreateImageBufferI3D");
		gwglDestroyImageBufferI3D = (BOOL (WINAPI *)(HDC hDC, LPVOID pAddress))gwglGetProcAddress("wglDestroyImageBufferI3D");
		gwglAssociateImageBufferEventsI3D = (BOOL (WINAPI *)(HDC hDC, const HANDLE *pEvent, const LPVOID *pAddress, const DWORD *pSize, UINT count))gwglGetProcAddress("wglAssociateImageBufferEventsI3D");
		gwglReleaseImageBufferEventsI3D = (BOOL (WINAPI *)(HDC hDC, const LPVOID *pAddress, UINT count))gwglGetProcAddress("wglReleaseImageBufferEventsI3D");

		if(OpenGL_ExtensionsEnumeration("WGL_I3D_image_buffer", GWGL_I3D_image_buffer, element) == GLENUMERATION_FALSE)
			return;
	}

	//WGL_EXT_swap_control is not required
	if(StringToken(wglextensions, "WGL_I3D_swap_frame_lock") /* && gwglGetExtensionsStringEXT*//* && StringToken(wglextensions, "WGL_EXT_swap_control")*/)
	{
		gwglEnableFrameLockI3D = (BOOL (WINAPI *)(void))gwglGetProcAddress("wglEnableFrameLockI3D");
		gwglDisableFrameLockI3D = (BOOL (WINAPI *)(void))gwglGetProcAddress("wglDisableFrameLockI3D");
		gwglIsEnabledFrameLockI3D = (BOOL (WINAPI *)(BOOL *pFlag))gwglGetProcAddress("wglIsEnabledFrameLockI3D");
		gwglQueryFrameLockMasterI3D = (BOOL (WINAPI *)(BOOL *pFlag))gwglGetProcAddress("wglQueryFrameLockMasterI3D");

		if(OpenGL_ExtensionsEnumeration("WGL_I3D_swap_frame_lock", GWGL_I3D_swap_frame_lock, element) == GLENUMERATION_FALSE)
			return;
	}

	//WGL_EXT_swap_control is not required
	if(StringToken(wglextensions, "WGL_I3D_swap_frame_usage") /* && gwglGetExtensionsStringEXT*//* && StringToken(wglextensions, "WGL_EXT_swap_control")*/)
	{
		gwglGetFrameUsageI3D = (BOOL (WINAPI *)(float *pUsage))gwglGetProcAddress("wglGetFrameUsageI3D");
		gwglBeginFrameTrackingI3D = (BOOL (WINAPI *)(void))gwglGetProcAddress("wglBeginFrameTrackingI3D");
		gwglEndFrameTrackingI3D = (BOOL (WINAPI *)(void))gwglGetProcAddress("wglEndFrameTrackingI3D");
		gwglQueryFrameTrackingI3D = (BOOL (WINAPI *)(DWORD *pFrameCount, DWORD *pMissedFrames, float *pLastMissedUsage))gwglGetProcAddress("wglQueryFrameTrackingI3D");

		if(OpenGL_ExtensionsEnumeration("WGL_I3D_swap_frame_usage", GWGL_I3D_swap_frame_usage, element) == GLENUMERATION_FALSE)
			return;
	}

	//WGL_NV_render_texture_rectangle is not required
	if(/*atof(version) >= 1.1 && */StringToken(wglextensions, "WGL_NV_render_depth_texture") /* && StringToken(wglextensions, "WGL_ARB_render_texture") && StringToken(wglextensions, "GL_SGIX_depth_texture")*//* && StringToken(wglextensions, "WGL_NV_render_texture_rectangle")*/)
	{
		if(OpenGL_ExtensionsEnumeration("WGL_NV_render_depth_texture", GWGL_NV_render_depth_texture, element) == GLENUMERATION_FALSE)
			return;
	}

	if(/*atof(version) >= 1.1 && */StringToken(wglextensions, "WGL_NV_render_texture_rectangle") /* && StringToken(wglextensions, "WGL_ARB_render_texture") && StringToken(wglextensions, "GL_NV_texture_rectangle")*/)
	{
		if(OpenGL_ExtensionsEnumeration("WGL_NV_render_texture_rectangle", GWGL_NV_render_texture_rectangle, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(wglextensions, "WGL_ATI_pixel_format_float") /* && StringToken(wglextensions, "WGL_ARB_pixel_format")*/)
	{
		if(OpenGL_ExtensionsEnumeration("WGL_ATI_pixel_format_float", GWGL_ATI_pixel_format_float, element) == GLENUMERATION_FALSE)
			return;
	}

	//GL_EXT_paletted_texture && GL_SGIX_depth_texture &&
	//GL_NV_texture_shader && GL_NV_half_float are not required
	if(StringToken(wglextensions, "WGL_NV_float_buffer") /* && StringToken(wglextensions, "GL_NV_fragment_program") && StringToken(wglextensions, "GL_NV_texture_rectangle") && StringToken(wglextensions, "WGL_ARB_pixel_format") && StringToken(wglextensions, "WGL_ARB_render_texture") && StringToken(wglextensions, "WGL_NV_render_texture_rectangle")*//* && StringToken(wglextensions, "GL_EXT_paletted_texture") && StringToken(wglextensions, "GL_SGIX_depth_texture") && StringToken(wglextensions, "GL_NV_texture_shader") && StringToken(wglextensions, "GL_NV_half_float")*/)
	{
		if(OpenGL_ExtensionsEnumeration("WGL_NV_float_buffer", GWGL_NV_float_buffer, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(wglextensions, "WGL_ATI_render_texture_rectangle"))
	{
		if(OpenGL_ExtensionsEnumeration("WGL_ATI_render_texture_rectangle", GWGL_ATI_render_texture_rectangle, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(wglextensions, "WGL_3DL_stereo_control"))
	{
		gwglSetStereoEmitterState3DL = (BOOL (WINAPI *)(HDC hDC, UINT uState))gwglGetProcAddress("wglSetStereoEmitterState3DL");

		if(OpenGL_ExtensionsEnumeration("WGL_3DL_stereo_control", GWGL_3DL_stereo_control, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(wglextensions, "WGL_EXT_pixel_format_packed_float"))
	{
		if(OpenGL_ExtensionsEnumeration("WGL_EXT_pixel_format_packed_float", GWGL_EXT_pixel_format_packed_float, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(wglextensions, "WGL_EXT_framebuffer_sRGB"))
	{
		if(OpenGL_ExtensionsEnumeration("WGL_EXT_framebuffer_sRGB", GWGL_EXT_framebuffer_sRGB, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(wglextensions, "WGL_NV_present_video"))
	{
		gwglEnumerateVideoDevicesNV = (int (WINAPI *)(HDC hDC, HVIDEOOUTPUTDEVICENV *phDeviceList))gwglGetProcAddress("wglEnumerateVideoDevicesNV");
		gwglBindVideoDeviceNV = (BOOL (WINAPI *)(HDC hDC, unsigned int uVideoSlot, HVIDEOOUTPUTDEVICENV hVideoDevice, const int *piAttribList))gwglGetProcAddress("wglBindVideoDeviceNV");
		gwglQueryCurrentContextNV = (BOOL (WINAPI *)(int iAttribute, int *piValue))gwglGetProcAddress("wglQueryCurrentContextNV");

		if(OpenGL_ExtensionsEnumeration("WGL_NV_present_video", GWGL_NV_present_video, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(wglextensions, "WGL_NV_video_out"))
	{
		gwglGetVideoDeviceNV = (BOOL (WINAPI *)(HDC hDC, int numDevices, HPVIDEODEV *hVideoDevice))gwglGetProcAddress("wglGetVideoDeviceNV");
		gwglReleaseVideoDeviceNV = (BOOL (WINAPI *)(HPVIDEODEV hVideoDevice))gwglGetProcAddress("wglReleaseVideoDeviceNV");
		gwglBindVideoImageNV = (BOOL (WINAPI *)(HPVIDEODEV hVideoDevice, HPBUFFERARB hPbuffer, int iVideoBuffer))gwglGetProcAddress("wglBindVideoImageNV");
		gwglReleaseVideoImageNV = (BOOL (WINAPI *)(HPBUFFERARB hPbuffer, int iVideoBuffer))gwglGetProcAddress("wglReleaseVideoImageNV");
		gwglSendPbufferToVideoNV = (BOOL (WINAPI *)(HPBUFFERARB hPbuffer, int iBufferType, unsigned long *pulCounterPbuffer, BOOL bBlock))gwglGetProcAddress("wglSendPbufferToVideoNV");
		gwglGetVideoInfoNV = (BOOL (WINAPI *)(HPVIDEODEV hpVideoDevice, unsigned long *pulCounterOutputPbuffer, unsigned long *pulCounterOutputVideo))gwglGetProcAddress("wglGetVideoInfoNV");

		if(OpenGL_ExtensionsEnumeration("WGL_NV_video_out", GWGL_NV_video_out, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(wglextensions, "WGL_NV_swap_group"))
	{
		gwglJoinSwapGroupNV = (BOOL (WINAPI *)(HDC hDC, GLuint group))gwglGetProcAddress("wglJoinSwapGroupNV");
		gwglBindSwapBarrierNV = (BOOL (WINAPI *)(GLuint group, GLuint barrier))gwglGetProcAddress("wglBindSwapBarrierNV");
		gwglQuerySwapGroupNV = (BOOL (WINAPI *)(HDC hDC, GLuint *group, GLuint *barrier))gwglGetProcAddress("wglQuerySwapGroupNV");
		gwglQueryMaxSwapGroupsNV = (BOOL (WINAPI *)(HDC hDC, GLuint *maxGroups, GLuint *maxBarriers))gwglGetProcAddress("wglQueryMaxSwapGroupsNV");
		gwglQueryFrameCountNV = (BOOL (WINAPI *)(HDC hDC, GLuint *count))gwglGetProcAddress("wglQueryFrameCountNV");
		gwglResetFrameCountNV = (BOOL (WINAPI *)(HDC hDC))gwglGetProcAddress("wglResetFrameCountNV");

		if(OpenGL_ExtensionsEnumeration("WGL_NV_swap_group", GWGL_NV_swap_group, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(wglextensions, "WGL_NV_gpu_affinity"))
	{
		gwglEnumGpusNV = (BOOL (WINAPI *)(UINT iGpuIndex, HGPUNV *phGpu))gwglGetProcAddress("wglEnumGpusNV");
		gwglEnumGpuDevicesNV = (BOOL (WINAPI *)(HGPUNV hGpu, UINT iDeviceIndex, PGPU_DEVICE lpGpuDevice))gwglGetProcAddress("wglEnumGpuDevicesNV");
		gwglCreateAffinityDCNV = (HDC (WINAPI *)(const HGPUNV *phGpuList))gwglGetProcAddress("wglCreateAffinityDCNV");
		gwglEnumGpusFromAffinityDCNV = (BOOL (WINAPI *)(HDC hAffinityDC, UINT iGpuIndex, HGPUNV *hGpu))gwglGetProcAddress("wglEnumGpusFromAffinityDCNV");
		gwglDeleteDCNV = (BOOL (WINAPI *)(HDC hdc))gwglGetProcAddress("wglDeleteDCNV");

		if(OpenGL_ExtensionsEnumeration("WGL_NV_gpu_affinity", GWGL_NV_gpu_affinity, element) == GLENUMERATION_FALSE)
			return;
	}

//==========================================================

	//TODO: this is only for OpenGL 1.0 through 1.2
	if(/*atof(version) <= 1.2 && */StringToken(extensions, "GL_ARB_multitexture"))
	{
		gglActiveTextureARB = (void (APIENTRY *)(GLenum texture))gwglGetProcAddress("glActiveTextureARB");
		gglClientActiveTextureARB = (void (APIENTRY *)(GLenum texture))gwglGetProcAddress("glClientActiveTextureARB");
		gglMultiTexCoord1dARB = (void (APIENTRY *)(GLenum target, GLdouble s))gwglGetProcAddress("glMultiTexCoord1dARB");
		gglMultiTexCoord1dvARB = (void (APIENTRY *)(GLenum target, const GLdouble *v))gwglGetProcAddress("glMultiTexCoord1dvARB");
		gglMultiTexCoord1fARB = (void (APIENTRY *)(GLenum target, GLfloat s))gwglGetProcAddress("glMultiTexCoord1fARB");
		gglMultiTexCoord1fvARB = (void (APIENTRY *)(GLenum target, const GLfloat *v))gwglGetProcAddress("glMultiTexCoord1fvARB");
		gglMultiTexCoord1iARB = (void (APIENTRY *)(GLenum target, GLint s))gwglGetProcAddress("glMultiTexCoord1iARB");
		gglMultiTexCoord1ivARB = (void (APIENTRY *)(GLenum target, const GLint *v))gwglGetProcAddress("glMultiTexCoord1ivARB");
		gglMultiTexCoord1sARB = (void (APIENTRY *)(GLenum target, GLshort s))gwglGetProcAddress("glMultiTexCoord1sARB");
		gglMultiTexCoord1svARB = (void (APIENTRY *)(GLenum target, const GLshort *v))gwglGetProcAddress("glMultiTexCoord1svARB");
		gglMultiTexCoord2dARB = (void (APIENTRY *)(GLenum target, GLdouble s, GLdouble t))gwglGetProcAddress("glMultiTexCoord2dARB");
		gglMultiTexCoord2dvARB = (void (APIENTRY *)(GLenum target, const GLdouble *v))gwglGetProcAddress("glMultiTexCoord2dvARB");
		gglMultiTexCoord2fARB = (void (APIENTRY *)(GLenum target, GLfloat s, GLfloat t))gwglGetProcAddress("glMultiTexCoord2fARB");
		gglMultiTexCoord2fvARB = (void (APIENTRY *)(GLenum target, const GLfloat *v))gwglGetProcAddress("glMultiTexCoord2fvARB");
		gglMultiTexCoord2iARB = (void (APIENTRY *)(GLenum target, GLint s, GLint t))gwglGetProcAddress("glMultiTexCoord2iARB");
		gglMultiTexCoord2ivARB = (void (APIENTRY *)(GLenum target, const GLint *v))gwglGetProcAddress("glMultiTexCoord2ivARB");
		gglMultiTexCoord2sARB = (void (APIENTRY *)(GLenum target, GLshort s, GLshort t))gwglGetProcAddress("glMultiTexCoord2sARB");
		gglMultiTexCoord2svARB = (void (APIENTRY *)(GLenum target, const GLshort *v))gwglGetProcAddress("glMultiTexCoord2svARB");
		gglMultiTexCoord3dARB = (void (APIENTRY *)(GLenum target, GLdouble s, GLdouble t, GLdouble r))gwglGetProcAddress("glMultiTexCoord3dARB");
		gglMultiTexCoord3dvARB = (void (APIENTRY *)(GLenum target, const GLdouble *v))gwglGetProcAddress("glMultiTexCoord3dvARB");
		gglMultiTexCoord3fARB = (void (APIENTRY *)(GLenum target, GLfloat s, GLfloat t, GLfloat r))gwglGetProcAddress("glMultiTexCoord3fARB");
		gglMultiTexCoord3fvARB = (void (APIENTRY *)(GLenum target, const GLfloat *v))gwglGetProcAddress("glMultiTexCoord3fvARB");
		gglMultiTexCoord3iARB = (void (APIENTRY *)(GLenum target, GLint s, GLint t, GLint r))gwglGetProcAddress("glMultiTexCoord3iARB");
		gglMultiTexCoord3ivARB = (void (APIENTRY *)(GLenum target, const GLint *v))gwglGetProcAddress("glMultiTexCoord3ivARB");
		gglMultiTexCoord3sARB = (void (APIENTRY *)(GLenum target, GLshort s, GLshort t, GLshort r))gwglGetProcAddress("glMultiTexCoord3sARB");
		gglMultiTexCoord3svARB = (void (APIENTRY *)(GLenum target, const GLshort *v))gwglGetProcAddress("glMultiTexCoord3svARB");
		gglMultiTexCoord4dARB = (void (APIENTRY *)(GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q))gwglGetProcAddress("glMultiTexCoord4dARB");
		gglMultiTexCoord4dvARB = (void (APIENTRY *)(GLenum target, const GLdouble *v))gwglGetProcAddress("glMultiTexCoord4dvARB");
		gglMultiTexCoord4fARB = (void (APIENTRY *)(GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q))gwglGetProcAddress("glMultiTexCoord4fARB");
		gglMultiTexCoord4fvARB = (void (APIENTRY *)(GLenum target, const GLfloat *v))gwglGetProcAddress("glMultiTexCoord4fvARB");
		gglMultiTexCoord4iARB = (void (APIENTRY *)(GLenum target, GLint s, GLint t, GLint r, GLint q))gwglGetProcAddress("glMultiTexCoord4iARB");
		gglMultiTexCoord4ivARB = (void (APIENTRY *)(GLenum target, const GLint *v))gwglGetProcAddress("glMultiTexCoord4ivARB");
		gglMultiTexCoord4sARB = (void (APIENTRY *)(GLenum target, GLshort s, GLshort t, GLshort r, GLshort q))gwglGetProcAddress("glMultiTexCoord4sARB");
		gglMultiTexCoord4svARB = (void (APIENTRY *)(GLenum target, const GLshort *v))gwglGetProcAddress("glMultiTexCoord4svARB");

		if(OpenGL_ExtensionsEnumeration("GL_ARB_multitexture", GGL_ARB_multitexture, element) == GLENUMERATION_FALSE)
			return;
	}

	//TODO: this is only for OpenGL 1.0 through 1.2
	if(/*atof(version) <= 1.2 && */StringToken(extensions, "GL_ARB_transpose_matrix"))
	{
		gglLoadTransposeMatrixfARB = (void (APIENTRY *)(const GLfloat *m))gwglGetProcAddress("glLoadTransposeMatrixfARB");
		gglLoadTransposeMatrixdARB = (void (APIENTRY *)(const GLdouble *m))gwglGetProcAddress("glLoadTransposeMatrixfARB");
		gglMultTransposeMatrixfARB = (void (APIENTRY *)(const GLfloat *m))gwglGetProcAddress("glLoadTransposeMatrixfARB");
		gglMultTransposeMatrixdARB = (void (APIENTRY *)(const GLdouble *m))gwglGetProcAddress("glLoadTransposeMatrixfARB");

		if(OpenGL_ExtensionsEnumeration("GL_ARB_transpose_matrix", GGL_ARB_transpose_matrix, element) == GLENUMERATION_FALSE)
			return;
	}

	//TODO: this is only for OpenGL 1.0 through 1.2
	if(/*atof(version) <= 1.2 && */StringToken(extensions, "GL_ARB_multisample") /* && gwglGetExtensionsStringEXT && StringToken(extensions, "WGL_EXT_pixel_format")*/)
	{
		gglSampleCoverageARB = (void (APIENTRY *)(GLclampf value, GLboolean invert))gwglGetProcAddress("glSampleCoverageARB");

		if(OpenGL_ExtensionsEnumeration("GL_ARB_multisample", GGL_ARB_multisample, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_ARB_texture_env_add"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_ARB_texture_env_add", GGL_ARB_texture_env_add, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_ARB_texture_cube_map"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_ARB_texture_cube_map", GGL_ARB_texture_cube_map, element) == GLENUMERATION_FALSE)
			return;
	}

	//TODO: this is only for OpenGL 1.0 through 1.2
	//needs GL_ARB_texture_cube_map if cube maps are being compressed
	if(/*atof(version) <= 1.2 && */ /*atof(version) >= 1.1 && */StringToken(extensions, "GL_ARB_texture_compression")/* && StringToken(extensions, "GL_ARB_texture_cube_map")*/)
	{
		gglCompressedTexImage3DARB = (void (APIENTRY *)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid *data))gwglGetProcAddress("glCompressedTexImage3DARB");
		gglCompressedTexImage2DARB = (void (APIENTRY *)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data))gwglGetProcAddress("glCompressedTexImage2DARB");
		gglCompressedTexImage1DARB = (void (APIENTRY *)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid *data))gwglGetProcAddress("glCompressedTexImage1DARB");
		gglCompressedTexSubImage3DARB = (void (APIENTRY *)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid *data))gwglGetProcAddress("glCompressedTexSubImage3DARB");
		gglCompressedTexSubImage2DARB = (void (APIENTRY *)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data))gwglGetProcAddress("glCompressedTexSubImage2DARB");
		gglCompressedTexSubImage1DARB = (void (APIENTRY *)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid *data))gwglGetProcAddress("glCompressedTexSubImage1DARB");
		gglGetCompressedTexImageARB = (void (APIENTRY *)(GLenum target, GLint level, void *img))gwglGetProcAddress("glGetCompressedTexImageARB");

		if(OpenGL_ExtensionsEnumeration("GL_ARB_texture_compression", GGL_ARB_texture_compression, element) == GLENUMERATION_FALSE)
			return;
	}

	if(/*atof(version) >= 1.0 && */StringToken(extensions, "GL_ARB_texture_border_clamp"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_ARB_texture_border_clamp", GGL_ARB_texture_border_clamp, element) == GLENUMERATION_FALSE)
			return;
	}

	//TODO: this is only for OpenGL 1.0 through 1.3
	//ARB_multisample is not required
	if(/*atof(version) >= 1.0 && */StringToken(extensions, "GL_ARB_point_parameters")/* && StringToken(extensions, "ARB_multisample")*/)
	{
		gglPointParameterfARB = (void (APIENTRY *)(GLenum pname, GLfloat param))gwglGetProcAddress("glPointParameterfARB");
		gglPointParameterfvARB = (void (APIENTRY *)(GLenum pname, const GLfloat *params))gwglGetProcAddress("glPointParameterfvARB");

		if(OpenGL_ExtensionsEnumeration("GL_ARB_point_parameters", GGL_ARB_point_parameters, element) == GLENUMERATION_FALSE)
			return;
	}

	if(/*atof(version) >= 1.0 && */StringToken(extensions, "GL_ARB_vertex_blend"))
	{
		gglWeightbvARB = (void (APIENTRY *)(GLint size, const GLbyte *weights))gwglGetProcAddress("glWeightbvARB");
		gglWeightsvARB = (void (APIENTRY *)(GLint size, const GLshort *weights))gwglGetProcAddress("glWeightsvARB");
		gglWeightivARB = (void (APIENTRY *)(GLint size, const GLint *weights))gwglGetProcAddress("glWeightivARB");
		gglWeightfvARB = (void (APIENTRY *)(GLint size, const GLfloat *weights))gwglGetProcAddress("glWeightfvARB");
		gglWeightdvARB = (void (APIENTRY *)(GLint size, const GLdouble *weights))gwglGetProcAddress("glWeightdvARB");
		gglWeightubvARB = (void (APIENTRY *)(GLint size, const GLubyte *weights))gwglGetProcAddress("glWeightubvARB");
		gglWeightusvARB = (void (APIENTRY *)(GLint size, const GLushort *weights))gwglGetProcAddress("glWeightusvARB");
		gglWeightuivARB = (void (APIENTRY *)(GLint size, const GLuint *weights))gwglGetProcAddress("glWeightuivARB");
		gglWeightPointerARB = (void (APIENTRY *)(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer))gwglGetProcAddress("glWeightPointerARB");
		gglVertexBlendARB = (void (APIENTRY *)(GLint count))gwglGetProcAddress("glVertexBlendARB");

		if(OpenGL_ExtensionsEnumeration("GL_ARB_vertex_blend", GGL_ARB_vertex_blend, element) == GLENUMERATION_FALSE)
			return;
	}

	if(/*atof(version) >= 1.0 && */StringToken(extensions, "GL_ARB_matrix_palette") /* && StringToken(extensions, "GL_ARB_vertex_blend")*/)
	{
		gglCurrentPaletteMatrixARB = (void (APIENTRY *)(GLint index))gwglGetProcAddress("glCurrentPaletteMatrixARB");
		gglMatrixIndexubvARB = (void (APIENTRY *)(GLint size, const GLubyte *indices))gwglGetProcAddress("glMatrixIndexubvARB");
		gglMatrixIndexusvARB = (void (APIENTRY *)(GLint size, const GLushort *indices))gwglGetProcAddress("glMatrixIndexusvARB");
		gglMatrixIndexuivARB = (void (APIENTRY *)(GLint size, const GLuint *indices))gwglGetProcAddress("glMatrixIndexuivARB");
		gglMatrixIndexPointerARB = (void (APIENTRY *)(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer))gwglGetProcAddress("glMatrixIndexPointerARB");

		if(OpenGL_ExtensionsEnumeration("GL_ARB_matrix_palette", GGL_ARB_matrix_palette, element) == GLENUMERATION_FALSE)
			return;
	}

	if(/*atof(version) >= 1.1 && */StringToken(extensions, "GL_ARB_texture_env_combine") /* && StringToken(extensions, "GL_ARB_multitexture")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_ARB_texture_env_combine", GGL_ARB_texture_env_combine, element) == GLENUMERATION_FALSE)
			return;
	}

	if(/*atof(version) >= 1.1 && */StringToken(extensions, "GL_ARB_texture_env_crossbar") /* && StringToken(extensions, "GL_ARB_multitexture") && StringToken(extensions, "GL_ARB_texture_env_combine")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_ARB_texture_env_crossbar", GGL_ARB_texture_env_crossbar, element) == GLENUMERATION_FALSE)
			return;
	}

	if(/*atof(version) >= 1.1 && */StringToken(extensions, "GL_ARB_texture_env_dot3") /* && StringToken(extensions, "GL_ARB_multitexture") && StringToken(extensions, "GL_ARB_texture_env_combine")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_ARB_texture_env_dot3", GGL_ARB_texture_env_dot3, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_ARB_texture_mirrored_repeat"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_ARB_texture_mirrored_repeat", GGL_ARB_texture_mirrored_repeat, element) == GLENUMERATION_FALSE)
			return;
	}

	if(/*atof(version) >= 1.1 && */StringToken(extensions, "GL_ARB_depth_texture"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_ARB_depth_texture", GGL_ARB_depth_texture, element) == GLENUMERATION_FALSE)
			return;
	}

	if(/*atof(version) >= 1.1 && */StringToken(extensions, "GL_ARB_shadow") /* && StringToken(extensions, "GL_ARB_depth_texture")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_ARB_shadow", GGL_ARB_shadow, element) == GLENUMERATION_FALSE)
			return;
	}

	if(/*atof(version) >= 1.1 && */StringToken(extensions, "GL_ARB_shadow_ambient") /* && StringToken(extensions, "GL_ARB_shadow") && StringToken(extensions, "GL_ARB_depth_texture")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_ARB_shadow_ambient", GGL_ARB_shadow_ambient, element) == GLENUMERATION_FALSE)
			return;
	}

	//TODO: this is only for OpenGL 1.0 through 1.3
	//GL_EXT_fog_coordinate && GL_EXT_secondary_color are not required
	if(/*atof(version) >= 1.0 && */StringToken(extensions, "GL_ARB_window_pos")/* && StringToken(extensions, "GL_EXT_fog_coordinate") && StringToken(extensions, "GL_EXT_secondary_color")*/)
	{
		gglWindowPos2dARB = (void (APIENTRY *)(GLdouble x, GLdouble y))gwglGetProcAddress("glWindowPos2dARB");
		gglWindowPos2dvARB = (void (APIENTRY *)(const GLdouble *v))gwglGetProcAddress("glWindowPos2dvARB");
		gglWindowPos2fARB = (void (APIENTRY *)(GLfloat x, GLfloat y))gwglGetProcAddress("glWindowPos2fARB");
		gglWindowPos2fvARB = (void (APIENTRY *)(const GLfloat *v))gwglGetProcAddress("glWindowPos2fvARB");
		gglWindowPos2iARB = (void (APIENTRY *)(GLint x, GLint y))gwglGetProcAddress("glWindowPos2iARB");
		gglWindowPos2ivARB = (void (APIENTRY *)(const GLint *v))gwglGetProcAddress("glWindowPos2ivARB");
		gglWindowPos2sARB = (void (APIENTRY *)(GLshort x, GLshort y))gwglGetProcAddress("glWindowPos2sARB");
		gglWindowPos2svARB = (void (APIENTRY *)(const GLshort *v))gwglGetProcAddress("glWindowPos2svARB");
		gglWindowPos3dARB = (void (APIENTRY *)(GLdouble x, GLdouble y, GLdouble z))gwglGetProcAddress("glWindowPos3dARB");
		gglWindowPos3dvARB = (void (APIENTRY *)(const GLdouble *v))gwglGetProcAddress("glWindowPos3dvARB");
		gglWindowPos3fARB = (void (APIENTRY *)(GLfloat x, GLfloat y, GLfloat z))gwglGetProcAddress("glWindowPos3fARB");
		gglWindowPos3fvARB = (void (APIENTRY *)(const GLfloat *v))gwglGetProcAddress("glWindowPos3fvARB");
		gglWindowPos3iARB = (void (APIENTRY *)(GLint x, GLint y, GLint z))gwglGetProcAddress("glWindowPos3iARB");
		gglWindowPos3ivARB = (void (APIENTRY *)(const GLint *v))gwglGetProcAddress("glWindowPos3ivARB");
		gglWindowPos3sARB = (void (APIENTRY *)(GLshort x, GLshort y, GLshort z))gwglGetProcAddress("glWindowPos3sARB");
		gglWindowPos3svARB = (void (APIENTRY *)(const GLshort *v))gwglGetProcAddress("glWindowPos3svARB");

		if(OpenGL_ExtensionsEnumeration("GL_ARB_window_pos", GGL_ARB_window_pos, element) == GLENUMERATION_FALSE)
			return;
	}

	//GL_ARB_vertex_blend && GL_EXT_vertex_weighting && GL_ARB_matrix_palette &&
	//GL_ARB_point_parameters && GL_EXT_point_parameters && GL_EXT_secondary_color &&
	//GL_EXT_fog_coord && GL_ARB_transpose_matrix are not required
	//NV_vertex_program && EXT_vertex_shader interact with the extension
	if(/*atof(version) >= 1.3 && */StringToken(extensions, "GL_ARB_vertex_program")/* && StringToken(extneions, "GL_ARB_vertex_blend") && StringToken(extensions, "GL_EXT_vertex_weighting") && StringToken(extensions, "GL_ARB_matrix_palette") && StringToken(extensions, "GL_ARB_point_parameters") && StringToken(extensions, "GL_EXT_point_parameters") && StringToken(extensions, "GL_EXT_secondary_color") && StringToken(extensions, "GL_EXT_fog_coord") && StringToken(extensions, "GL_ARB_transpose_matrix") && StringToken(extensions, "GL_NV_vertex_program") && StringToken(extensions, "GL_EXT_vertex_shader")*/)
	{
		gglVertexAttrib1sARB = (void (APIENTRY *)(GLuint index, GLshort x))gwglGetProcAddress("glVertexAttrib1sARB");
		gglVertexAttrib1fARB = (void (APIENTRY *)(GLuint index, GLfloat x))gwglGetProcAddress("glVertexAttrib1fARB");
		gglVertexAttrib1dARB = (void (APIENTRY *)(GLuint index, GLdouble x))gwglGetProcAddress("glVertexAttrib1dARB");
		gglVertexAttrib2sARB = (void (APIENTRY *)(GLuint index, GLshort x, GLshort y))gwglGetProcAddress("glVertexAttrib2sARB");
		gglVertexAttrib2fARB = (void (APIENTRY *)(GLuint index, GLfloat x, GLfloat y))gwglGetProcAddress("glVertexAttrib2fARB");
		gglVertexAttrib2dARB = (void (APIENTRY *)(GLuint index, GLdouble x, GLdouble y))gwglGetProcAddress("glVertexAttrib2dARB");
		gglVertexAttrib3sARB = (void (APIENTRY *)(GLuint index, GLshort x, GLshort y, GLshort z))gwglGetProcAddress("glVertexAttrib3sARB");
		gglVertexAttrib3fARB = (void (APIENTRY *)(GLuint index, GLfloat x, GLfloat y, GLfloat z))gwglGetProcAddress("glVertexAttrib3fARB");
		gglVertexAttrib3dARB = (void (APIENTRY *)(GLuint index, GLdouble x, GLdouble y, GLdouble z))gwglGetProcAddress("glVertexAttrib3dARB");
		gglVertexAttrib4sARB = (void (APIENTRY *)(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w))gwglGetProcAddress("glVertexAttrib4sARB");
		gglVertexAttrib4fARB = (void (APIENTRY *)(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w))gwglGetProcAddress("glVertexAttrib4fARB");
		gglVertexAttrib4dARB = (void (APIENTRY *)(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w))gwglGetProcAddress("glVertexAttrib4dARB");
		gglVertexAttrib4NubARB = (void (APIENTRY *)(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w))gwglGetProcAddress("glVertexAttrib4NubARB");
		gglVertexAttrib1svARB = (void (APIENTRY *)(GLuint index, const GLshort *v))gwglGetProcAddress("glVertexAttrib1svARB");
		gglVertexAttrib1fvARB = (void (APIENTRY *)(GLuint index, const GLfloat *v))gwglGetProcAddress("glVertexAttrib1fvARB");
		gglVertexAttrib1dvARB = (void (APIENTRY *)(GLuint index, const GLdouble *v))gwglGetProcAddress("glVertexAttrib1dvARB");
		gglVertexAttrib2svARB = (void (APIENTRY *)(GLuint index, const GLshort *v))gwglGetProcAddress("glVertexAttrib2svARB");
		gglVertexAttrib2fvARB = (void (APIENTRY *)(GLuint index, const GLfloat *v))gwglGetProcAddress("glVertexAttrib2fvARB");
		gglVertexAttrib2dvARB = (void (APIENTRY *)(GLuint index, const GLdouble *v))gwglGetProcAddress("glVertexAttrib2dvARB");
		gglVertexAttrib3svARB = (void (APIENTRY *)(GLuint index, const GLshort *v))gwglGetProcAddress("glVertexAttrib3svARB");
		gglVertexAttrib3fvARB = (void (APIENTRY *)(GLuint index, const GLfloat *v))gwglGetProcAddress("glVertexAttrib3fvARB");
		gglVertexAttrib3dvARB = (void (APIENTRY *)(GLuint index, const GLdouble *v))gwglGetProcAddress("glVertexAttrib3dvARB");
		gglVertexAttrib4bvARB = (void (APIENTRY *)(GLuint index, const GLbyte *v))gwglGetProcAddress("glVertexAttrib4bvARB");
		gglVertexAttrib4svARB = (void (APIENTRY *)(GLuint index, const GLshort *v))gwglGetProcAddress("glVertexAttrib4svARB");
		gglVertexAttrib4ivARB = (void (APIENTRY *)(GLuint index, const GLint *v))gwglGetProcAddress("glVertexAttrib4ivARB");
		gglVertexAttrib4ubvARB = (void (APIENTRY *)(GLuint index, const GLubyte *v))gwglGetProcAddress("glVertexAttrib4ubvARB");
		gglVertexAttrib4usvARB = (void (APIENTRY *)(GLuint index, const GLushort *v))gwglGetProcAddress("glVertexAttrib4usvARB");
		gglVertexAttrib4uivARB = (void (APIENTRY *)(GLuint index, const GLuint *v))gwglGetProcAddress("glVertexAttrib4uivARB");
		gglVertexAttrib4fvARB = (void (APIENTRY *)(GLuint index, const GLfloat *v))gwglGetProcAddress("glVertexAttrib4fvARB");
		gglVertexAttrib4dvARB = (void (APIENTRY *)(GLuint index, const GLdouble *v))gwglGetProcAddress("glVertexAttrib4dvARB");
		gglVertexAttrib4NbvARB = (void (APIENTRY *)(GLuint index, const GLbyte *v))gwglGetProcAddress("glVertexAttrib4NbvARB");
		gglVertexAttrib4NsvARB = (void (APIENTRY *)(GLuint index, const GLshort *v))gwglGetProcAddress("glVertexAttrib4NsvARB");
		gglVertexAttrib4NivARB = (void (APIENTRY *)(GLuint index, const GLint *v))gwglGetProcAddress("glVertexAttrib4NivARB");
		gglVertexAttrib4NubvARB = (void (APIENTRY *)(GLuint index, const GLubyte *v))gwglGetProcAddress("glVertexAttrib4NubvARB");
		gglVertexAttrib4NusvARB = (void (APIENTRY *)(GLuint index, const GLushort *v))gwglGetProcAddress("glVertexAttrib4NusvARB");
		gglVertexAttrib4NuivARB = (void (APIENTRY *)(GLuint index, const GLuint *v))gwglGetProcAddress("glVertexAttrib4NuivARB");
		gglVertexAttribPointerARB = (void (APIENTRY *)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer))gwglGetProcAddress("glVertexAttribPointerARB");
		gglEnableVertexAttribArrayARB = (void (APIENTRY *)(GLuint index))gwglGetProcAddress("glEnableVertexAttribArrayARB");
		gglDisableVertexAttribArrayARB = (void (APIENTRY *)(GLuint index))gwglGetProcAddress("glDisableVertexAttribArrayARB");
		gglProgramStringARB = (void (APIENTRY *)(GLenum target, GLenum format, GLsizei len, const GLvoid *string))gwglGetProcAddress("glProgramStringARB");
		gglBindProgramARB = (void (APIENTRY *)(GLenum target, GLuint program))gwglGetProcAddress("glBindProgramARB");
		gglDeleteProgramsARB = (void (APIENTRY *)(GLsizei n, const GLuint *programs))gwglGetProcAddress("glDeleteProgramsARB");
		gglGenProgramsARB = (void (APIENTRY *)(GLsizei n, GLuint *programs))gwglGetProcAddress("glGenProgramsARB");
		gglProgramEnvParameter4dARB = (void (APIENTRY *)(GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w))gwglGetProcAddress("glProgramEnvParameter4dARB");
		gglProgramEnvParameter4dvARB = (void (APIENTRY *)(GLenum target, GLuint index, const GLdouble *params))gwglGetProcAddress("glProgramEnvParameter4dvARB");
		gglProgramEnvParameter4fARB = (void (APIENTRY *)(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w))gwglGetProcAddress("glProgramEnvParameter4fARB");
		gglProgramEnvParameter4fvARB = (void (APIENTRY *)(GLenum target, GLuint index, const GLfloat *params))gwglGetProcAddress("glProgramEnvParameter4fvARB");
		gglProgramLocalParameter4dARB = (void (APIENTRY *)(GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w))gwglGetProcAddress("glProgramLocalParameter4dARB");
		gglProgramLocalParameter4dvARB = (void (APIENTRY *)(GLenum target, GLuint index, const GLdouble *params))gwglGetProcAddress("glProgramLocalParameter4dvARB");
		gglProgramLocalParameter4fARB = (void (APIENTRY *)(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w))gwglGetProcAddress("glProgramLocalParameter4fARB");
		gglProgramLocalParameter4fvARB = (void (APIENTRY *)(GLenum target, GLuint index, const GLfloat *params))gwglGetProcAddress("glProgramLocalParameter4fvARB");
		gglGetProgramEnvParameterdvARB = (void (APIENTRY *)(GLenum target, GLuint index, GLdouble *params))gwglGetProcAddress("glGetProgramEnvParameterdvARB");
		gglGetProgramEnvParameterfvARB = (void (APIENTRY *)(GLenum target, GLuint index, GLfloat *params))gwglGetProcAddress("glGetProgramEnvParameterfvARB");
		gglGetProgramLocalParameterdvARB = (void (APIENTRY *)(GLenum target, GLuint index, GLdouble *params))gwglGetProcAddress("glGetProgramLocalParameterdvARB");
		gglGetProgramLocalParameterfvARB = (void (APIENTRY *)(GLenum target, GLuint index, GLfloat *params))gwglGetProcAddress("glGetProgramLocalParameterfvARB");
		gglGetProgramivARB = (void (APIENTRY *)(GLenum target, GLenum pname, GLint *params))gwglGetProcAddress("glGetProgramivARB");
		gglGetProgramStringARB = (void (APIENTRY *)(GLenum target, GLenum pname, GLvoid *string))gwglGetProcAddress("glGetProgramStringARB");
		gglGetVertexAttribdvARB = (void (APIENTRY *)(GLuint index, GLenum pname, GLdouble *params))gwglGetProcAddress("glGetVertexAttribdvARB");
		gglGetVertexAttribfvARB = (void (APIENTRY *)(GLuint index, GLenum pname, GLfloat *params))gwglGetProcAddress("glGetVertexAttribfvARB");
		gglGetVertexAttribivARB = (void (APIENTRY *)(GLuint index, GLenum pname, GLint *params))gwglGetProcAddress("glGetVertexAttribivARB");
		gglGetVertexAttribPointervARB = (void (APIENTRY *)(GLuint index, GLenum pname, GLvoid **pointer))gwglGetProcAddress("glGetVertexAttribPointervARB");
		gglIsProgramARB = (GLboolean (APIENTRY *)(GLuint program))gwglGetProcAddress("glIsProgramARB");

		if(OpenGL_ExtensionsEnumeration("GL_ARB_vertex_program", GGL_ARB_vertex_program, element) == GLENUMERATION_FALSE)
			return;
	}

	//OpenGL 1.4 affects the definition of this extension
	//GL_ARB_vertex_blend && GL_EXT_vertex_weighting && GL_ARB_matrix_palette &&
	//GL_ARB_transpose_matrix && GL_EXT_fog_coord && GL_EXT_texture_rectangle are not required
	//GL_ARB_shadow && GL_ARB_vertex_program && GL_ATI_fragment_shader &&
	//GL_NV_fragment_program interact with the extension
	if(/*atof(version) >= 1.3 && */StringToken(extensions, "GL_ARB_fragment_program") /* && (atof(version) >= 1.4 || StringToken(extensions, "GL_EXT_texture_lod_bias"))*//* && atof(version) >= 1.4 && StringToken(extensions, "GL_ARB_vertex_blend") && StringToken(extensions, "GL_EXT_vertex_weighting") && StringToken(extensions, "GL_ARB_matrix_palette") && StringToken(extensions, "GL_ARB_transpose_matrix") && StringToken(extensions, "GL_EXT_fog_coord") && StringToken(extensions, "GL_EXT_texture_rectangle") && StringToken(extensions, "GL_ARB_shadow") && StringToken(extensions, "GL_ARB_vertex_program") && StringToken(extensions, "GL_ATI_fragment_shader") && StringToken(extensions, "GL_NV_fragment_program")*/)
	{
		gglProgramStringARB = (void (APIENTRY *)(GLenum target, GLenum format, GLsizei len, const GLvoid *string))gwglGetProcAddress("glProgramStringARB");
		gglBindProgramARB = (void (APIENTRY *)(GLenum target, GLuint program))gwglGetProcAddress("glBindProgramARB");
		gglDeleteProgramsARB = (void (APIENTRY *)(GLsizei n, const GLuint *programs))gwglGetProcAddress("glDeleteProgramsARB");
		gglGenProgramsARB = (void (APIENTRY *)(GLsizei n, GLuint *programs))gwglGetProcAddress("glGenProgramsARB");
		gglProgramEnvParameter4dARB = (void (APIENTRY *)(GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w))gwglGetProcAddress("glProgramEnvParameter4dARB");
		gglProgramEnvParameter4dvARB = (void (APIENTRY *)(GLenum target, GLuint index, const GLdouble *params))gwglGetProcAddress("glProgramEnvParameter4dvARB");
		gglProgramEnvParameter4fARB = (void (APIENTRY *)(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w))gwglGetProcAddress("glProgramEnvParameter4fARB");
		gglProgramEnvParameter4fvARB = (void (APIENTRY *)(GLenum target, GLuint index, const GLfloat *params))gwglGetProcAddress("glProgramEnvParameter4fvARB");
		gglProgramLocalParameter4dARB = (void (APIENTRY *)(GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w))gwglGetProcAddress("glProgramLocalParameter4dARB");
		gglProgramLocalParameter4dvARB = (void (APIENTRY *)(GLenum target, GLuint index, const GLdouble *params))gwglGetProcAddress("glProgramLocalParameter4dvARB");
		gglProgramLocalParameter4fARB = (void (APIENTRY *)(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w))gwglGetProcAddress("glProgramLocalParameter4fARB");
		gglProgramLocalParameter4fvARB = (void (APIENTRY *)(GLenum target, GLuint index, const GLfloat *params))gwglGetProcAddress("glProgramLocalParameter4fvARB");
		gglGetProgramEnvParameterdvARB = (void (APIENTRY *)(GLenum target, GLuint index, GLdouble *params))gwglGetProcAddress("glGetProgramEnvParameterdvARB");
		gglGetProgramEnvParameterfvARB = (void (APIENTRY *)(GLenum target, GLuint index, GLfloat *params))gwglGetProcAddress("glGetProgramEnvParameterfvARB");
		gglGetProgramLocalParameterdvARB = (void (APIENTRY *)(GLenum target, GLuint index, GLdouble *params))gwglGetProcAddress("glGetProgramLocalParameterdvARB");
		gglGetProgramLocalParameterfvARB = (void (APIENTRY *)(GLenum target, GLuint index, GLfloat *params))gwglGetProcAddress("glGetProgramLocalParameterfvARB");
		gglGetProgramivARB = (void (APIENTRY *)(GLenum target, GLenum pname, GLint *params))gwglGetProcAddress("glGetProgramivARB");
		gglGetProgramStringARB = (void (APIENTRY *)(GLenum target, GLenum pname, GLvoid *string))gwglGetProcAddress("glGetProgramStringARB");
		gglIsProgramARB = (GLboolean (APIENTRY *)(GLuint program))gwglGetProcAddress("glIsProgramARB");

		if(OpenGL_ExtensionsEnumeration("GL_ARB_fragment_program", GGL_ARB_fragment_program, element) == GLENUMERATION_FALSE)
			return;
	}

	//GL_ARB_vertex_blend && GL_ARB_vertex_program && GL_EXT_vertex_shader are not required
	if(StringToken(extensions, "GL_ARB_vertex_buffer_object")/* && StringToken(extensions, "GL_ARB_vertex_blend") && StringToken(extensions, "GL_ARB_vertex_program") && StringToken(extensions, "GL_EXT_vertex_shader")*/)
	{
		gglBindBufferARB = (void (APIENTRY *)(GLenum target, GLuint buffer))gwglGetProcAddress("glBindBufferARB");
		gglDeleteBuffersARB = (void (APIENTRY *)(GLsizei n, const GLuint *buffers))gwglGetProcAddress("glDeleteBuffersARB");
		gglGenBuffersARB = (void (APIENTRY *)(GLsizei n, GLuint *buffers))gwglGetProcAddress("glGenBuffersARB");
		gglIsBufferARB = (GLboolean (APIENTRY *)(GLuint buffer))gwglGetProcAddress("glIsBufferARB");
		gglBufferDataARB = (void (APIENTRY *)(GLenum target, GLsizeiptrARB size, const GLvoid *data, GLenum usage))gwglGetProcAddress("glBufferDataARB");
		gglBufferSubDataARB = (void (APIENTRY *)(GLenum target, GLintptrARB offset, GLsizeiptrARB size, const GLvoid *data))gwglGetProcAddress("glBufferSubDataARB");
		gglGetBufferSubDataARB = (void (APIENTRY *)(GLenum target, GLintptrARB offset, GLsizeiptrARB size, GLvoid *data))gwglGetProcAddress("glGetBufferSubDataARB");
		gglMapBufferARB = (void *(APIENTRY *)(GLenum target, GLenum access))gwglGetProcAddress("glMapBufferARB");
		gglUnmapBufferARB = (GLboolean (APIENTRY *)(GLenum target))gwglGetProcAddress("glUnmapBufferARB");
		gglGetBufferParameterivARB = (void (APIENTRY *)(GLenum target, GLenum pname, GLint *params))gwglGetProcAddress("glGetBufferParameterivARB");
		gglGetBufferPointervARB = (void (APIENTRY *)(GLenum target, GLenum pname, GLvoid **params))gwglGetProcAddress("glGetBufferPointervARB");

		if(OpenGL_ExtensionsEnumeration("GL_ARB_vertex_buffer_object", GGL_ARB_vertex_buffer_object, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_ARB_occlusion_query"))
	{
		gglGenQueriesARB = (void (APIENTRY *)(GLsizei n, GLuint *ids))gwglGetProcAddress("glGenQueriesARB");
		gglDeleteQueriesARB = (void (APIENTRY *)(GLsizei n, const GLuint *ids))gwglGetProcAddress("glDeleteQueriesARB");
		gglIsQueryARB = (GLboolean (APIENTRY *)(GLuint id))gwglGetProcAddress("glIsQueryARB");
		gglBeginQueryARB = (void (APIENTRY *)(GLenum target, GLuint id))gwglGetProcAddress("glBeginQueryARB");
		gglEndQueryARB = (void (APIENTRY *)(GLenum target))gwglGetProcAddress("glEndQueryARB");
		gglGetQueryivARB = (void (APIENTRY *)(GLenum target, GLenum pname, GLint *params))gwglGetProcAddress("glGetQueryivARB");
		gglGetQueryObjectivARB = (void (APIENTRY *)(GLuint id, GLenum pname, GLint *params))gwglGetProcAddress("glGetQueryObjectivARB");
		gglGetQueryObjectuivARB = (void (APIENTRY *)(GLuint id, GLenum pname, GLuint *params))gwglGetProcAddress("glGetQueryObjectuivARB");

		if(OpenGL_ExtensionsEnumeration("GL_ARB_occlusion_query", GGL_ARB_occlusion_query, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_ARB_shader_objects"))
	{
		gglDeleteObjectARB = (void (APIENTRY *)(GLhandleARB obj))gwglGetProcAddress("glDeleteObjectARB");
		gglGetHandleARB = (GLhandleARB (APIENTRY *)(GLenum pname))gwglGetProcAddress("glGetHandleARB");
		gglDetachObjectARB = (void (APIENTRY *)(GLhandleARB containerObj, GLhandleARB attachedObj))gwglGetProcAddress("glDetachObjectARB");
		gglCreateShaderObjectARB = (GLhandleARB (APIENTRY *)(GLenum shaderType))gwglGetProcAddress("glCreateShaderObjectARB");
		gglShaderSourceARB = (void (APIENTRY *)(GLhandleARB shaderObj, GLsizei count, const GLcharARB* *string, const GLint *length))gwglGetProcAddress("glShaderSourceARB");
		gglCompileShaderARB = (void (APIENTRY *)(GLhandleARB shaderObj))gwglGetProcAddress("glCompileShaderARB");
		gglCreateProgramObjectARB = (GLhandleARB (APIENTRY *)(void))gwglGetProcAddress("glCreateProgramObjectARB");
		gglAttachObjectARB = (void (APIENTRY *)(GLhandleARB containerObj, GLhandleARB obj))gwglGetProcAddress("glAttachObjectARB");
		gglLinkProgramARB = (void (APIENTRY *)(GLhandleARB programObj))gwglGetProcAddress("glLinkProgramARB");
		gglUseProgramObjectARB = (void (APIENTRY *)(GLhandleARB programObj))gwglGetProcAddress("glUseProgramObjectARB");
		gglValidateProgramARB = (void (APIENTRY *)(GLhandleARB programObj))gwglGetProcAddress("glValidateProgramARB");
		gglUniform1fARB = (void (APIENTRY *)(GLint location, GLfloat v0))gwglGetProcAddress("glUniform1fARB");
		gglUniform2fARB = (void (APIENTRY *)(GLint location, GLfloat v0, GLfloat v1))gwglGetProcAddress("glUniform2fARB");
		gglUniform3fARB = (void (APIENTRY *)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2))gwglGetProcAddress("glUniform3fARB");
		gglUniform4fARB = (void (APIENTRY *)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3))gwglGetProcAddress("glUniform4fARB");
		gglUniform1iARB = (void (APIENTRY *)(GLint location, GLint v0))gwglGetProcAddress("glUniform1iARB");
		gglUniform2iARB = (void (APIENTRY *)(GLint location, GLint v0, GLint v1))gwglGetProcAddress("glUniform2iARB");
		gglUniform3iARB = (void (APIENTRY *)(GLint location, GLint v0, GLint v1, GLint v2))gwglGetProcAddress("glUniform3iARB");
		gglUniform4iARB = (void (APIENTRY *)(GLint location, GLint v0, GLint v1, GLint v2, GLint v3))gwglGetProcAddress("glUniform4iARB");
		gglUniform1fvARB = (void (APIENTRY *)(GLint location, GLsizei count, const GLfloat *value))gwglGetProcAddress("glUniform1fvARB");
		gglUniform2fvARB = (void (APIENTRY *)(GLint location, GLsizei count, const GLfloat *value))gwglGetProcAddress("glUniform2fvARB");
		gglUniform3fvARB = (void (APIENTRY *)(GLint location, GLsizei count, const GLfloat *value))gwglGetProcAddress("glUniform3fvARB");
		gglUniform4fvARB = (void (APIENTRY *)(GLint location, GLsizei count, const GLfloat *value))gwglGetProcAddress("glUniform4fvARB");
		gglUniform1ivARB = (void (APIENTRY *)(GLint location, GLsizei count, const GLint *value))gwglGetProcAddress("glUniform1ivARB");
		gglUniform2ivARB = (void (APIENTRY *)(GLint location, GLsizei count, const GLint *value))gwglGetProcAddress("glUniform2ivARB");
		gglUniform3ivARB = (void (APIENTRY *)(GLint location, GLsizei count, const GLint *value))gwglGetProcAddress("glUniform3ivARB");
		gglUniform4ivARB = (void (APIENTRY *)(GLint location, GLsizei count, const GLint *value))gwglGetProcAddress("glUniform4ivARB");
		gglUniformMatrix2fvARB = (void (APIENTRY *)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value))gwglGetProcAddress("glUniformMatrix2fvARB");
		gglUniformMatrix3fvARB = (void (APIENTRY *)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value))gwglGetProcAddress("glUniformMatrix3fvARB");
		gglUniformMatrix4fvARB = (void (APIENTRY *)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value))gwglGetProcAddress("glUniformMatrix4fvARB");
		gglGetObjectParameterfvARB = (void (APIENTRY *)(GLhandleARB obj, GLenum pname, GLfloat *params))gwglGetProcAddress("glGetObjectParameterfvARB");
		gglGetObjectParameterivARB = (void (APIENTRY *)(GLhandleARB obj, GLenum pname, GLint *params))gwglGetProcAddress("glGetObjectParameterivARB");
		gglGetInfoLogARB = (void (APIENTRY *)(GLhandleARB obj, GLsizei maxLength, GLsizei *length, GLcharARB *infoLog))gwglGetProcAddress("glGetInfoLogARB");
		gglGetAttachedObjectsARB = (void (APIENTRY *)(GLhandleARB containerObj, GLsizei maxCount, GLsizei *count, GLhandleARB *obj))gwglGetProcAddress("glGetAttachedObjectsARB");
		gglGetUniformLocationARB = (GLint (APIENTRY *)(GLhandleARB programObj, const GLcharARB *name))gwglGetProcAddress("glGetUniformLocationARB");
		gglGetActiveUniformARB = (void (APIENTRY *)(GLhandleARB programObj, GLuint index, GLsizei maxLength, GLsizei *length, GLint *size, GLenum *type, GLcharARB *name))gwglGetProcAddress("glGetActiveUniformARB");
		gglGetUniformfvARB = (void (APIENTRY *)(GLhandleARB programObj, GLint location, GLfloat *params))gwglGetProcAddress("glGetUniformfvARB");
		gglGetUniformivARB = (void (APIENTRY *)(GLhandleARB programObj, GLint location, GLint *params))gwglGetProcAddress("glGetUniformivARB");
		gglGetShaderSourceARB = (void (APIENTRY *)(GLhandleARB obj, GLsizei maxLength, GLsizei *length, GLcharARB *source))gwglGetProcAddress("glGetShaderSourceARB");

		if(OpenGL_ExtensionsEnumeration("GL_ARB_shader_objects", GGL_ARB_shader_objects, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_ARB_vertex_shader"))
	{
		gglBindAttribLocationARB = (void (APIENTRY *)(GLhandleARB programObj, GLuint index, const GLcharARB *name))gwglGetProcAddress("glBindAttribLocationARB");
		gglGetActiveAttribARB = (void (APIENTRY *)(GLhandleARB programObj, GLuint index, GLsizei maxLength, GLsizei *length, GLint *size, GLenum *type, GLcharARB *name))gwglGetProcAddress("glGetActiveAttribARB");
		gglGetAttribLocationARB = (GLint (APIENTRY *)(GLhandleARB programObj, const GLcharARB *name))gwglGetProcAddress("glGetAttribLocationARB");

		if(OpenGL_ExtensionsEnumeration("GL_ARB_vertex_shader", GGL_ARB_vertex_shader, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_ARB_fragment_shader"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_ARB_fragment_shader", GGL_ARB_fragment_shader, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_ARB_shading_language_100"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_ARB_shading_language_100", GGL_ARB_shading_language_100, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_ARB_texture_non_power_of_two"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_ARB_texture_non_power_of_two", GGL_ARB_texture_non_power_of_two, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_ARB_point_sprite"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_ARB_point_sprite", GGL_ARB_point_sprite, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_ARB_fragment_program_shadow"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_ARB_fragment_program_shadow", GGL_ARB_fragment_program_shadow, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_ARB_draw_buffers"))
	{
		gglDrawBuffersARB = (void (APIENTRY *)(GLsizei n, const GLenum *bufs))gwglGetProcAddress("glDrawBuffersARB");

		if(OpenGL_ExtensionsEnumeration("GL_ARB_draw_buffers", GGL_ARB_draw_buffers, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_ARB_texture_rectangle"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_ARB_texture_rectangle", GGL_ARB_texture_rectangle, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_ARB_color_buffer_float"))
	{
		gglClampColorARB = (void (APIENTRY *)(GLenum target, GLenum clamp))gwglGetProcAddress("glClampColorARB");

		if(OpenGL_ExtensionsEnumeration("GL_ARB_color_buffer_float", GGL_ARB_color_buffer_float, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_ARB_half_float_pixel"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_ARB_half_float_pixel", GGL_ARB_half_float_pixel, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_ARB_texture_float"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_ARB_texture_float", GGL_ARB_texture_float, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_ARB_pixel_buffer_object"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_ARB_pixel_buffer_object", GGL_ARB_pixel_buffer_object, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_ARB_depth_buffer_float"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_ARB_depth_buffer_float", GGL_ARB_depth_buffer_float, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_ARB_draw_instanced"))
	{
		gglDrawArraysInstancedARB = (void (APIENTRY *)(GLenum mode, GLint first, GLsizei count, GLsizei primcount))gwglGetProcAddress("glDrawArraysInstancedARB");
		gglDrawElementsInstancedARB = (void (APIENTRY *)(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei primcount))gwglGetProcAddress("glDrawElementsInstancedARB");

		if(OpenGL_ExtensionsEnumeration("GL_ARB_draw_instanced", GGL_ARB_draw_instanced, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_ARB_framebuffer_object"))
	{
		gglIsRenderbuffer = (GLboolean (APIENTRY *)(GLuint renderbuffer))gwglGetProcAddress("glIsRenderbuffer");
		gglBindRenderbuffer = (void (APIENTRY *)(GLenum target, GLuint renderbuffer))gwglGetProcAddress("glBindRenderbuffer");
		gglDeleteRenderbuffers = (void (APIENTRY *)(GLsizei n, const GLuint *renderbuffers))gwglGetProcAddress("glDeleteRenderbuffers");
		gglGenRenderbuffers = (void (APIENTRY *)(GLsizei n, GLuint *renderbuffers))gwglGetProcAddress("glGenRenderbuffers");
		gglRenderbufferStorage = (void (APIENTRY *)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height))gwglGetProcAddress("glRenderbufferStorage");
		gglGetRenderbufferParameteriv = (void (APIENTRY *)(GLenum target, GLenum pname, GLint *params))gwglGetProcAddress("glGetRenderbufferParameteriv");
		gglIsFramebuffer = (GLboolean (APIENTRY *)(GLuint framebuffer))gwglGetProcAddress("glIsFramebuffer");
		gglBindFramebuffer = (void (APIENTRY *)(GLenum target, GLuint framebuffer))gwglGetProcAddress("glBindFramebuffer");
		gglDeleteFramebuffers = (void (APIENTRY *)(GLsizei n, const GLuint *framebuffers))gwglGetProcAddress("glDeleteFramebuffers");
		gglGenFramebuffers = (void (APIENTRY *)(GLsizei n, GLuint *framebuffers))gwglGetProcAddress("glGenFramebuffers");
		gglCheckFramebufferStatus = (GLenum (APIENTRY *)(GLenum target))gwglGetProcAddress("glCheckFramebufferStatus");
		gglFramebufferTexture1D = (void (APIENTRY *)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level))gwglGetProcAddress("glFramebufferTexture1D");
		gglFramebufferTexture2D = (void (APIENTRY *)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level))gwglGetProcAddress("glFramebufferTexture2D");
		gglFramebufferTexture3D = (void (APIENTRY *)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset))gwglGetProcAddress("glFramebufferTexture3D");
		gglFramebufferRenderbuffer = (void (APIENTRY *)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer))gwglGetProcAddress("glFramebufferRenderbuffer");
		gglGetFramebufferAttachmentParameteriv = (void (APIENTRY *)(GLenum target, GLenum attachment, GLenum pname, GLint *params))gwglGetProcAddress("glGetFramebufferAttachmentParameteriv");
		gglGenerateMipmap = (void (APIENTRY *)(GLenum target))gwglGetProcAddress("glGenerateMipmap");
		gglBlitFramebuffer = (void (APIENTRY *)(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter))gwglGetProcAddress("glBlitFramebuffer");
		gglRenderbufferStorageMultisample = (void (APIENTRY *)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height))gwglGetProcAddress("glRenderbufferStorageMultisample");
		gglFramebufferTextureLayer = (void (APIENTRY *)(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer))gwglGetProcAddress("glFramebufferTextureLayer");

		if(OpenGL_ExtensionsEnumeration("GL_ARB_framebuffer_object", GGL_ARB_framebuffer_object, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_ARB_framebuffer_sRGB"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_ARB_framebuffer_sRGB", GGL_ARB_framebuffer_sRGB, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_ARB_geometry_shader4"))
	{
		gglProgramParameteriARB = (void (APIENTRY *)(GLuint program, GLenum pname, GLint value))gwglGetProcAddress("glProgramParameteriARB");
		gglFramebufferTextureARB = (void (APIENTRY *)(GLenum target, GLenum attachment, GLuint texture, GLint level))gwglGetProcAddress("glFramebufferTextureARB");
		gglFramebufferTextureLayerARB = (void (APIENTRY *)(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer))gwglGetProcAddress("glFramebufferTextureLayerARB");
		gglFramebufferTextureFaceARB = (void (APIENTRY *)(GLenum target, GLenum attachment, GLuint texture, GLint level, GLenum face))gwglGetProcAddress("glFramebufferTextureFaceARB");

		if(OpenGL_ExtensionsEnumeration("GL_ARB_geometry_shader4", GGL_ARB_geometry_shader4, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_ARB_half_float_vertex"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_ARB_half_float_vertex", GGL_ARB_half_float_vertex, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_ARB_instanced_arrays"))
	{
		gglVertexAttribDivisor = (void (APIENTRYP *)(GLuint index, GLuint divisor))gwglGetProcAddress("glVertexAttribDivisor");

		if(OpenGL_ExtensionsEnumeration("GL_ARB_instanced_arrays", GGL_ARB_instanced_arrays, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_ARB_map_buffer_range"))
	{
		gglMapBufferRange = (void (APIENTRY *)(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access))gwglGetProcAddress("glMapBufferRange");
		gglFlushMappedBufferRange = (void (APIENTRY *)(GLenum target, GLintptr offset, GLsizeiptr length))gwglGetProcAddress("glFlushMappedBufferRange");

		if(OpenGL_ExtensionsEnumeration("GL_ARB_map_buffer_range", GGL_ARB_map_buffer_range, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_ARB_texture_buffer_object"))
	{
		gglTexBufferARB = (void (APIENTRY *)(GLenum target, GLenum internalformat, GLuint buffer))gwglGetProcAddress("glTexBufferARB");

		if(OpenGL_ExtensionsEnumeration("GL_ARB_texture_buffer_object", GGL_ARB_texture_buffer_object, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_ARB_texture_compression_rgtc"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_ARB_texture_compression_rgtc", GGL_ARB_texture_compression_rgtc, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_ARB_texture_rg"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_ARB_texture_rg", GGL_ARB_texture_rg, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_ARB_vertex_array_object"))
	{
		gglBindVertexArray = (void (APIENTRY *)(GLuint array))gwglGetProcAddress("glBindVertexArray");
		gglDeleteVertexArrays = (void (APIENTRY *)(GLsizei n, const GLuint *arrays))gwglGetProcAddress("glDeleteVertexArrays");
		gglGenVertexArrays = (void (APIENTRY *)(GLsizei n, GLuint *arrays))gwglGetProcAddress("glGenVertexArrays");
		gglIsVertexArray = (GLboolean (APIENTRY *)(GLuint array))gwglGetProcAddress("glIsVertexArray");

		if(OpenGL_ExtensionsEnumeration("GL_ARB_vertex_array_object", GGL_ARB_vertex_array_object, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_EXT_abgr"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_EXT_abgr", GGL_EXT_abgr, element) == GLENUMERATION_FALSE)
			return;
	}

	//TODO: this is only for OpenGL 1.0 && 1.1
	if(/*atof(version) <= 1.1 && */StringToken(extensions, "GL_EXT_blend_color"))
	{
		gglBlendColorEXT = (void (APIENTRY *)(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha))gwglGetProcAddress("glBlendColorEXT");

		if(OpenGL_ExtensionsEnumeration("GL_EXT_blend_color", GGL_EXT_blend_color, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_EXT_polygon_offset"))
	{
		gglPolygonOffsetEXT = (void (APIENTRY *)(GLfloat factor, GLfloat bias))gwglGetProcAddress("glPolygonOffsetEXT");

		if(OpenGL_ExtensionsEnumeration("GL_EXT_polygon_offset", GGL_EXT_polygon_offset, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_EXT_texture"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_EXT_texture", GGL_EXT_texture, element) == GLENUMERATION_FALSE)
			return;
	}

	//TODO: this is only for OpenGL 1.0 && 1.1
	//GL_EXT_abgr is not required
	if(/*atof(version) <= 1.1 && */StringToken(extensions, "GL_EXT_texture3D")/* && StringToken(extensions, "GL_EXT_abgr")*/ /* && StringToken(extensions, "GL_EXT_texture")*/)
	{
		gglTexImage3DEXT = (void (APIENTRY *)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels))gwglGetProcAddress("glTexImage3DEXT");
		//XXX: this is also in GL_EXT_subtexture
		gglTexSubImage3DEXT = (void (APIENTRY *)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels))gwglGetProcAddress("glTexSubImage3DEXT");

		if(OpenGL_ExtensionsEnumeration("GL_EXT_texture3D", GGL_EXT_texture3D, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_SGIS_texture_filter4"))
	{
		gglGetTexFilterFuncSGIS = (void (APIENTRY *)(GLenum target, GLenum filter, GLfloat *weights))gwglGetProcAddress("glGetTexFilterFuncSGIS");
		gglTexFilterFuncSGIS = (void (APIENTRY *)(GLenum target, GLenum filter, GLsizei n, const GLfloat *weights))gwglGetProcAddress("glTexFilterFuncSGIS");

		if(OpenGL_ExtensionsEnumeration("GL_SGIS_texture_filter4", GGL_SGIS_texture_filter4, element) == GLENUMERATION_FALSE)
			return;
	}

	//TODO: this is only for OpenGL 1.0
	//GL_EXT_abgr && GL_EXT_texture3D are not required
	if(/*atof(version) <= 1.0 && */StringToken(extensions, "GL_EXT_subtexture")/* && StringToken(extensions, "GL_EXT_abgr")*/ /* && StringToken(extensions, "GL_EXT_texture")*//* && StringToken(extensions, "GL_EXT_texture3D")*/)
	{
		gglTexSubImage1DEXT = (void (APIENTRY *)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels))gwglGetProcAddress("glTexSubImage1DEXT");
		gglTexSubImage2DEXT = (void (APIENTRY *)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels))gwglGetProcAddress("glTexSubImage2DEXT");
		//XXX: this is also in GL_EXT_texture3d
		gglTexSubImage3DEXT = (void (APIENTRY *)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels))gwglGetProcAddress("glTexSubImage3DEXT");

		if(OpenGL_ExtensionsEnumeration("GL_EXT_subtexture", GGL_EXT_subtexture, element) == GLENUMERATION_FALSE)
			return;
	}

	//TODO: this is only for OpenGL 1.0 (gglCopyTexSubImage3DEXT is used for OpenGL 1.1 as well)
	//GL_EXT_texture3D && GL_SGIS_texture_filter4 && GL_EXT_subtexture are not required
	if(/*atof(version) <= 1.1 && */StringToken(extensions, "GL_EXT_copy_texture") /* && StringToken(extensions, "GL_EXT_texture")*//* && StringToken(extensions, "GL_EXT_texture3D") && StringToken(extensions, "GL_SGIS_texture_filter4") && StringToken(extensions, "GL_EXT_subtexture")*/)
	{
		gglCopyTexImage1DEXT = (void (APIENTRY *)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border))gwglGetProcAddress("glCopyTexImage1DEXT");
		gglCopyTexImage2DEXT = (void (APIENTRY *)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border))gwglGetProcAddress("glCopyTexImage2DEXT");
		gglCopyTexSubImage1DEXT = (void (APIENTRY *)(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width))gwglGetProcAddress("glCopyTexSubImage1DEXT");
		gglCopyTexSubImage2DEXT = (void (APIENTRY *)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height))gwglGetProcAddress("glCopyTexSubImage2DEXT");
		gglCopyTexSubImage3DEXT = (void (APIENTRY *)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height))gwglGetProcAddress("glCopyTexSubImage3DEXT");

		if(OpenGL_ExtensionsEnumeration("GL_EXT_copy_texture", GGL_EXT_copy_texture, element) == GLENUMERATION_FALSE)
			return;
	}

	//TODO: this is only for OpenGL 1.0 && 1.1
	//GL_EXT_abgr is not required
	if(/*atof(version) <= 1.1 && */StringToken(extensions, "GL_EXT_histogram")/* && StringToken(extensions, "GL_EXT_abgr")*/ /* && StringToken(extensions, "GL_EXT_texture")*/)
	{
		gglGetHistogramEXT = (void (APIENTRY *)(GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid *values))gwglGetProcAddress("glGetHistogramEXT");
		gglGetHistogramParameterfvEXT = (void (APIENTRY *)(GLenum target, GLenum pname, GLfloat *params))gwglGetProcAddress("glGetHistogramParameterfvEXT");
		gglGetHistogramParameterivEXT = (void (APIENTRY *)(GLenum target, GLenum pname, GLint *params))gwglGetProcAddress("glGetHistogramParameterivEXT");
		gglGetMinmaxEXT = (void (APIENTRY *)(GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid *values))gwglGetProcAddress("glGetMinmaxEXT");
		gglGetMinmaxParameterfvEXT = (void (APIENTRY *)(GLenum target, GLenum pname, GLfloat *params))gwglGetProcAddress("glGetMinmaxParameterfvEXT");
		gglGetMinmaxParameterivEXT = (void (APIENTRY *)(GLenum target, GLenum pname, GLint *params))gwglGetProcAddress("glGetMinmaxParameterivEXT");
		gglHistogramEXT = (void (APIENTRY *)(GLenum target, GLsizei width, GLenum internalformat, GLboolean sink))gwglGetProcAddress("glHistogramEXT");
		gglMinmaxEXT = (void (APIENTRY *)(GLenum target, GLenum internalformat, GLboolean sink))gwglGetProcAddress("glMinmaxEXT");
		gglResetHistogramEXT = (void (APIENTRY *)(GLenum target))gwglGetProcAddress("glResetHistogramEXT");
		gglResetMinmaxEXT = (void (APIENTRY *)(GLenum target))gwglGetProcAddress("glResetMinmaxEXT");

		if(OpenGL_ExtensionsEnumeration("GL_EXT_histogram", GGL_EXT_histogram, element) == GLENUMERATION_FALSE)
			return;
	}

	//TODO: this is only for OpenGL 1.0 && 1.1
	//GL_EXT_abgr && GL_EXT_copy_texture are not required
	if(/*atof(version) <= 1.1 && */StringToken(extensions, "GL_EXT_convolution")/* && StringToken(extensions, "GL_EXT_abgr")*/ /* && StringToken(extensions, "GL_EXT_texture")*//* && StringToken(extensions, "GL_EXT_copy_texture")*/)
	{
		gglConvolutionFilter1DEXT = (void (APIENTRY *)(GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *image))gwglGetProcAddress("glConvolutionFilter1DEXT");
		gglConvolutionFilter2DEXT = (void (APIENTRY *)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *image))gwglGetProcAddress("glConvolutionFilter2DEXT");
		gglConvolutionParameterfEXT = (void (APIENTRY *)(GLenum target, GLenum pname, GLfloat params))gwglGetProcAddress("glConvolutionParameterfEXT");
		gglConvolutionParameterfvEXT = (void (APIENTRY *)(GLenum target, GLenum pname, const GLfloat *params))gwglGetProcAddress("glConvolutionParameterfvEXT");
		gglConvolutionParameteriEXT = (void (APIENTRY *)(GLenum target, GLenum pname, GLint params))gwglGetProcAddress("glConvolutionParameteriEXT");
		gglConvolutionParameterivEXT = (void (APIENTRY *)(GLenum target, GLenum pname, const GLint *params))gwglGetProcAddress("glConvolutionParameterivEXT");
		gglCopyConvolutionFilter1DEXT = (void (APIENTRY *)(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width))gwglGetProcAddress("glCopyConvolutionFilter1DEXT");
		gglCopyConvolutionFilter2DEXT = (void (APIENTRY *)(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height))gwglGetProcAddress("glCopyConvolutionFilter2DEXT");
		gglGetConvolutionFilterEXT = (void (APIENTRY *)(GLenum target, GLenum format, GLenum type, GLvoid *image))gwglGetProcAddress("glGetConvolutionFilterEXT");
		gglGetConvolutionParameterfvEXT = (void (APIENTRY *)(GLenum target, GLenum pname, GLfloat *params))gwglGetProcAddress("glGetConvolutionParameterfvEXT");
		gglGetConvolutionParameterivEXT = (void (APIENTRY *)(GLenum target, GLenum pname, GLint *params))gwglGetProcAddress("glGetConvolutionParameterivEXT");
		gglGetSeparableFilterEXT = (void (APIENTRY *)(GLenum target, GLenum format, GLenum type, GLvoid *row, GLvoid *column, GLvoid *span))gwglGetProcAddress("glGetSeparableFilterEXT");
		gglSeparableFilter2DEXT = (void (APIENTRY *)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *row, const GLvoid *column))gwglGetProcAddress("glSeparableFilter2DEXT");

		if(OpenGL_ExtensionsEnumeration("GL_EXT_convolution", GGL_EXT_convolution, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_SGI_color_matrix"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_SGI_color_matrix", GGL_SGI_color_matrix, element) == GLENUMERATION_FALSE)
			return;
	}

	//TODO: this is only for OpenGL 1.0 && 1.1
	//GL_EXT_abgr && GL_EXT_texture3D && GL_EXT_subtexture &&
	//GL_EXT_copy_texture && GL_EXT_convolution && GL_SGI_color_matrix are not required
	if(/*atof(version) <= 1.1 && */StringToken(extensions, "GL_SGI_color_table")/* && StringToken(extensions, "GL_EXT_abgr")*/ /* && StringToken(extensions, "GL_EXT_texture")*//* && StringToken(extensions, "GL_EXT_texture3D") && StringToken(extensions, "GL_EXT_subtexture") && StringToken(extensions, "GL_EXT_copy_texture") && StringToken(extensions, "GL_EXT_convolution") && StringToken(extensions, "GL_SGI_color_matrix")*/)
	{
		gglColorTableSGI = (void (APIENTRY *)(GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *table))gwglGetProcAddress("glColorTableSGI");
		gglColorTableParameterfvSGI = (void (APIENTRY *)(GLenum target, GLenum pname, const GLfloat *params))gwglGetProcAddress("glColorTableParameterfvSGI");
		gglColorTableParameterivSGI = (void (APIENTRY *)(GLenum target, GLenum pname, const GLint *params))gwglGetProcAddress("glColorTableParameterivSGI");
		gglCopyColorTableSGI = (void (APIENTRY *)(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width))gwglGetProcAddress("glCopyColorTableSGI");
		gglGetColorTableSGI = (void (APIENTRY *)(GLenum target, GLenum format, GLenum type, GLvoid *table))gwglGetProcAddress("glGetColorTableSGI");
		gglGetColorTableParameterfvSGI = (void (APIENTRY *)(GLenum target, GLenum pname, GLfloat *params))gwglGetProcAddress("glGetColorTableParameterfvSGI");
		gglGetColorTableParameterivSGI = (void (APIENTRY *)(GLenum target, GLenum pname, GLint *params))gwglGetProcAddress("glGetColorTableParameterivSGI");

		if(OpenGL_ExtensionsEnumeration("GL_SGI_color_table", GGL_SGI_color_table, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_SGIX_pixel_texture"))
	{
		gglPixelTexGenSGIX = (void (APIENTRY *)(GLenum mode))gwglGetProcAddress("glPixelTexGenSGIX");

		if(OpenGL_ExtensionsEnumeration("GL_SGIX_pixel_texture", GGL_SGIX_pixel_texture, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_SGIS_pixel_texture"))
	{
		gglPixelTexGenParameteriSGIS = (void (APIENTRY *)(GLenum pname, GLint param))gwglGetProcAddress("glPixelTexGenParameteriSGIS");
		gglPixelTexGenParameterivSGIS = (void (APIENTRY *)(GLenum pname, const GLint *params))gwglGetProcAddress("glPixelTexGenParameterivSGIS");
		gglPixelTexGenParameterfSGIS = (void (APIENTRY *)(GLenum pname, GLfloat param))gwglGetProcAddress("glPixelTexGenParameterfSGIS");
		gglPixelTexGenParameterfvSGIS = (void (APIENTRY *)(GLenum pname, const GLfloat *params))gwglGetProcAddress("glPixelTexGenParameterfvSGIS");
		gglGetPixelTexGenParameterivSGIS = (void (APIENTRY *)(GLenum pname, GLint *params))gwglGetProcAddress("glGetPixelTexGenParameterivSGIS");
		gglGetPixelTexGenParameterfvSGIS = (void (APIENTRY *)(GLenum pname, GLfloat *params))gwglGetProcAddress("glGetPixelTexGenParameterfvSGIS");

		if(OpenGL_ExtensionsEnumeration("GL_SGIS_pixel_texture", GGL_SGIS_pixel_texture, element) == GLENUMERATION_FALSE)
			return;
	}

	//XXX: not complete
	//GL_EXT_abgr && GL_EXT_subtexture are not required
	if(StringToken(extensions, "GL_SGIS_texture4D")/* && StringToken(extensions, "GL_EXT_abgr")*/ /* && StringToken(extensions, "GL_EXT_texture") && StringToken(extensions, "GL_EXT_texture3D")*//* && StringToken(extensions, "GL_EXT_subtexture")*/)
	{
		gglTexImage4DSGIS = (void (APIENTRY *)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLsizei size4d, GLint border, GLenum format, GLenum type, const GLvoid *pixels))gwglGetProcAddress("glTexImage4DSGIS");
		gglTexSubImage4DSGIS = (void (APIENTRY *)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint woffset, GLsizei width, GLsizei height, GLsizei depth, GLsizei size4d, GLenum format, GLenum type, const GLvoid *pixels))gwglGetProcAddress("glTexSubImage4DSGIS");

		if(OpenGL_ExtensionsEnumeration("GL_SGIS_texture4D", GGL_SGIS_texture4D, element) == GLENUMERATION_FALSE)
			return;
	}

	//GL_EXT_abgr && GL_EXT_copy_texture are not required
	if(StringToken(extensions, "GL_SGI_texture_color_table")/* && StringToken(extensions, "GL_EXT_abgr")*/ /* && StringToken(extensions, "GL_EXT_texture")*//* && StringToken(extensions, "GL_EXT_copy_texture")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_SGI_texture_color_table", GGL_SGI_texture_color_table, element) == GLENUMERATION_FALSE)
			return;
	}

	//GL_EXT_abgr && GL_EXT_texture3D && GL_EXT_subtexture && GL_EXT_histogram &&
	//GL_EXT_convolution && GL_SGI_color_table && GL_SGIS_texture4D are not required
	if(StringToken(extensions, "GL_EXT_cmyka")/* && StringToken(extensions, "GL_EXT_abgr") && StringToken(extensions, "GL_EXT_texture3D") && StringToken(extensions, "GL_EXT_subtexture") && StringToken(extensions, "GL_EXT_histogram") && StringToken(extensions, "GL_EXT_convolution") && StringToken(extensions, "GL_SGI_color_table") && StringToken(extensions, "GL_SGIS_texture4D")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_EXT_cmyka", GGL_EXT_cmyka, element) == GLENUMERATION_FALSE)
			return;
	}

	//TODO: this is only for OpenGL 1.0
	//GL_EXT_texture3D is not required
	if(/*atof(version) <= 1.0 && */StringToken(extensions, "GL_EXT_texture_object")/* && StringToken(extensions, "GL_EXT_texture3D")*/)
	{
		gglAreTexturesResidentEXT = (GLboolean (APIENTRY *)(GLsizei n, const GLuint *textures, GLboolean *residences))gwglGetProcAddress("glAreTexturesResidentEXT");
		gglBindTextureEXT = (void (APIENTRY *)(GLenum target, GLuint texture))gwglGetProcAddress("glBindTextureEXT");
		gglDeleteTexturesEXT = (void (APIENTRY *)(GLsizei n, const GLuint *textures))gwglGetProcAddress("glDeleteTexturesEXT");
		gglGenTexturesEXT = (void (APIENTRY *)(GLsizei n, GLuint *textures))gwglGetProcAddress("glGenTexturesEXT");
		gglIsTextureEXT = (GLboolean (APIENTRY *)(GLuint texture))gwglGetProcAddress("glIsTextureEXT");
		gglPrioritizeTexturesEXT = (void (APIENTRY *)(GLsizei n, const GLuint *textures, const GLclampf *priorities))gwglGetProcAddress("glPrioritizeTexturesEXT");

		if(OpenGL_ExtensionsEnumeration("GL_EXT_texture_object", GGL_EXT_texture_object, element) == GLENUMERATION_FALSE)
			return;
	}

	//GL_EXT_subtexture && GL_EXT_copy_texture && GL_EXT_texture_object are not required
	if(StringToken(extensions, "GL_SGIS_detail_texture") /* && StringToken(extensions, "GL_EXT_texture")*//* && StringToken(extensions, "GL_EXT_subtexture") && StringToken(extensions, "GL_EXT_copy_texture") && StringToken(extensions, "GL_EXT_texture_object")*/)
	{
		gglDetailTexFuncSGIS = (void (APIENTRY *)(GLenum target, GLsizei n, const GLfloat *points))gwglGetProcAddress("glDetailTexFuncSGIS");
		gglGetDetailTexFuncSGIS = (void (APIENTRY *)(GLenum target, GLfloat *points))gwglGetProcAddress("glGetDetailTexFuncSGIS");

		if(OpenGL_ExtensionsEnumeration("GL_SGIS_detail_texture", GGL_SGIS_detail_texture, element) == GLENUMERATION_FALSE)
			return;
	}

	//GL_EXT_texture3D && GL_EXT_texture_object are not required
	if(StringToken(extensions, "GL_SGIS_sharpen_texture") /* && StringToken(extensions, "GL_EXT_texture")*//* && StringToken(extensions, "GL_EXT_texture3D") && StringToken(extensions, "GL_EXT_texture_object")*/)
	{
		gglSharpenTexFuncSGIS = (void (APIENTRY *)(GLenum target, GLsizei n, const GLfloat *points))gwglGetProcAddress("glSharpenTexFuncSGIS");
		gglGetSharpenTexFuncSGIS = (void (APIENTRY *)(GLenum target, GLfloat *points))gwglGetProcAddress("glGetSharpenTexFuncSGIS");

		if(OpenGL_ExtensionsEnumeration("GL_SGIS_sharpen_texture", GGL_SGIS_sharpen_texture, element) == GLENUMERATION_FALSE)
			return;
	}

	//GL_EXT_abgr && GL_EXT_texture3D && GL_EXT_subtexture &&
	//GL_EXT_histogram && GL_EXT_convolution && GL_SGI_color_table &&
	//GL_SGIS_texture4D && GL_EXT_cmyka are not required
	if(StringToken(extensions, "GL_EXT_packed_pixels")/* && StringToken(extensions, "GL_EXT_abgr") && StringToken(extensions, "GL_EXT_texture3D") && StringToken(extensions, "GL_EXT_subtexture") && StringToken(extensions, "GL_EXT_histogram") && StringToken(extensions, "GL_EXT_convolution") && StringToken(extensions, "GL_SGI_color_table") && StringToken(extensions, "GL_SGIS_texture4D") && StringToken(extensions, "GL_EXT_cmyka")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_EXT_packed_pixels", GGL_EXT_packed_pixels, element) == GLENUMERATION_FALSE)
			return;
	}

	//GL_EXT_texture3D && GL_EXT_texture_object &&
	//GL_SGI_detail_texture && GL_SGI_sharpen_texture are not required
	if(StringToken(extensions, "GL_SGIS_texture_lod") /* && StringToken(extensions, "GL_EXT_texture")*//* && StringToken(extensions, "GL_EXT_texture3D") && StringToken(extensions, "GL_EXT_texture_object") && StringToken(extensions, "GL_SGI_detail_texture") && StringToken(extensions, "GL_SGI_sharpen_texture")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_SGIS_texture_lod", GGL_SGIS_texture_lod, element) == GLENUMERATION_FALSE)
			return;
	}

	//GL_SGIX_pixel_texture is not required
	if(StringToken(extensions, "GL_SGIS_multisample")/* && StringToken(extensions, "GL_SGIX_pixel_texture")*/)
	{
		gglSampleMaskSGIS = (void (APIENTRY *)(GLclampf value, GLboolean invert))gwglGetProcAddress("glSampleMaskSGIS");
		gglSamplePatternSGIS = (void (APIENTRY *)(GLenum pattern))gwglGetProcAddress("glSamplePatternSGIS");

		if(OpenGL_ExtensionsEnumeration("GL_SGIS_multisample", GGL_SGIS_multisample, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_EXT_rescale_normal"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_EXT_rescale_normal", GGL_EXT_rescale_normal, element) == GLENUMERATION_FALSE)
			return;
	}

	//TODO: this is only for OpenGL 1.0
	if(/*atof(version) <= 1.0 && */StringToken(extensions, "GL_EXT_vertex_array"))
	{
		gglArrayElementEXT = (void (APIENTRY *)(GLint i))gwglGetProcAddress("glArrayElementEXT");
		gglColorPointerEXT = (void (APIENTRY *)(GLint size, GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer))gwglGetProcAddress("glColorPointerEXT");
		gglDrawArraysEXT = (void (APIENTRY *)(GLenum mode, GLint first, GLsizei count))gwglGetProcAddress("glDrawArraysEXT");
		gglEdgeFlagPointerEXT = (void (APIENTRY *)(GLsizei stride, GLsizei count, const GLboolean *pointer))gwglGetProcAddress("glEdgeFlagPointerEXT");
		gglGetPointervEXT = (void (APIENTRY *)(GLenum pname, GLvoid* *params))gwglGetProcAddress("glGetPointervEXT");
		gglIndexPointerEXT = (void (APIENTRY *)(GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer))gwglGetProcAddress("glIndexPointerEXT");
		gglNormalPointerEXT = (void (APIENTRY *)(GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer))gwglGetProcAddress("glNormalPointerEXT");
		gglTexCoordPointerEXT = (void (APIENTRY *)(GLint size, GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer))gwglGetProcAddress("glTexCoordPointerEXT");
		gglVertexPointerEXT = (void (APIENTRY *)(GLint size, GLenum type, GLsizei stride, GLsizei count, const GLvoid *pointer))gwglGetProcAddress("glVertexPointerEXT");

		if(OpenGL_ExtensionsEnumeration("GL_EXT_vertex_array", GGL_EXT_vertex_array, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_EXT_misc_attribute"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_EXT_misc_attribute", GGL_EXT_misc_attribute, element) == GLENUMERATION_FALSE)
			return;
	}

	//GL_EXT_texture3D && GL_EXT_texture_object && GL_SGIS_texture_lod are not required
	if(StringToken(extensions, "GL_SGIS_generate_mipmap") /* && StringToken(extensions, "GL_EXT_texture")*//* && StringToken(extensions, "GL_EXT_texture3D") && StringToken(extensions, "GL_EXT_texture_object") && StringToken(extensions, "GL_SGIS_texture_lod")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_SGIS_generate_mipmap", GGL_SGIS_generate_mipmap, element) == GLENUMERATION_FALSE)
			return;
	}

	//XXX: not complete
	//GL_EXT_texture_object && GL_SGIS_texture_lod &&
	//GL_SGI_detail_texture && GL_SGI_sharpen_texture are not required
	if(StringToken(extensions, "GL_SGIX_clipmap") /* && StringToken(extensions, "GL_EXT_texture")*//* && StringToken(extensions, "GL_EXT_texture_object") && StringToken(extensions, "GL_SGIS_texture_lod") && StringToken(extensions, "GL_SGI_detail_texture") && StringToken(extensions, "GL_SGI_sharpen_texture")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_SGIX_clipmap", GGL_SGIX_clipmap, element) == GLENUMERATION_FALSE)
			return;
	}

	//XXX: not complete
	if(StringToken(extensions, "GL_SGIX_shadow"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_SGIX_shadow", GGL_SGIX_shadow, element) == GLENUMERATION_FALSE)
			return;
	}

	//GL_EXT_texture3D && GL_SGIS_texture_filter4 are not required
	if(StringToken(extensions, "GL_SGIS_texture_edge_clamp")/* && StringToken(extensions, "GL_EXT_texture3D") && StringToken(extensions, "GL_SGIS_texture_filter4")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_SGIS_texture_edge_clamp", GGL_SGIS_texture_edge_clamp, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_EXT_texture_edge_clamp"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_EXT_texture_edge_clamp", GGL_EXT_texture_edge_clamp, element) == GLENUMERATION_FALSE)
			return;
	}

	//GL_EXT_texture3D && GL_SGIS_texture_filter4 are not required
	if(StringToken(extensions, "GL_SGIS_texture_border_clamp")/* && StringToken(extensions, "GL_EXT_texture3D") && StringToken(extensions, "GL_SGIS_texture_filter4")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_SGIS_texture_border_clamp", GGL_SGIS_texture_border_clamp, element) == GLENUMERATION_FALSE)
			return;
	}
	
	//TODO: this is only for OpenGL 1.0 && 1.1
	if(/*atof(version) <= 1.1 && */StringToken(extensions, "GL_EXT_blend_minmax"))
	{
		gglBlendEquationEXT = (void (APIENTRY *)(GLenum mode))gwglGetProcAddress("glBlendEquationEXT");

		if(OpenGL_ExtensionsEnumeration("GL_EXT_blend_minmax", GGL_EXT_blend_minmax, element) == GLENUMERATION_FALSE)
			return;
	}

	//GL_EXT_blend_minmax is not required
	if(StringToken(extensions, "GL_EXT_blend_subtract")/* && StringToken(extensions, "GL_EXT_blend_minmax")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_EXT_blend_subtract", GGL_EXT_blend_subtract, element) == GLENUMERATION_FALSE)
			return;
	}

	//GL_EXT_blend_minmax is not required
	if(StringToken(extensions, "GL_EXT_blend_logic_op")/* && StringToken(extensions, "GL_EXT_blend_minmax")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_EXT_blend_logic_op", GGL_EXT_blend_logic_op, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_SGIX_interlace"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_SGIX_interlace", GGL_SGIX_interlace, element) == GLENUMERATION_FALSE)
			return;
	}

	//TODO: don't know requirements
	if(StringToken(extensions, "GL_SGIX_pixel_tiles"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_SGIX_pixel_tiles", GGL_SGIX_pixel_tiles, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_SGIX_texture_select") /* && StringToken(extensions, "GL_EXT_texture") && StringToken(extensions, "GL_EXT_texture3D") && StringToken(extensions, "GL_EXT_texture_object") && StringToken(extensions, "GL_SGIS_detail_texture")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_SGIX_texture_select", GGL_SGIX_texture_select, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_SGIX_sprite"))
	{
		gglSpriteParameterfSGIX = (void (APIENTRY *)(GLenum pname, GLfloat param))gwglGetProcAddress("glSpriteParameterfSGIX");
		gglSpriteParameterfvSGIX = (void (APIENTRY *)(GLenum pname, const GLfloat *params))gwglGetProcAddress("glSpriteParameterfvSGIX");
		gglSpriteParameteriSGIX = (void (APIENTRY *)(GLenum pname, GLint param))gwglGetProcAddress("glSpriteParameteriSGIX");
		gglSpriteParameterivSGIX = (void (APIENTRY *)(GLenum pname, const GLint *params))gwglGetProcAddress("glSpriteParameterivSGIX");

		if(OpenGL_ExtensionsEnumeration("GL_SGIX_sprite", GGL_SGIX_sprite, element) == GLENUMERATION_FALSE)
			return;
	}

	//GL_EXT_texture_object is not required
	if(StringToken(extensions, "GL_SGIX_texture_multi_buffer")/* && StringToken(extensions, "GL_EXT_texture_object")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_SGIX_texture_multi_buffer", GGL_SGIX_texture_multi_buffer, element) == GLENUMERATION_FALSE)
			return;
	}

	//TODO: this is only for OpenGL 1.0 through 1.3
	//GL_SGIS_multisample is not required
	if(StringToken(extensions, "GL_EXT_point_parameters")/* && StringToken(extensions, "GL_SGIS_multisample")*/)
	{
		gglPointParameterfEXT = (void (APIENTRY *)(GLenum pname, GLfloat param))gwglGetProcAddress("glPointParameterfEXT");
		gglPointParameterfvEXT = (void (APIENTRY *)(GLenum pname, const GLfloat *params))gwglGetProcAddress("glPointParameterfvEXT");

		if(OpenGL_ExtensionsEnumeration("GL_EXT_point_parameters", GGL_EXT_point_parameters, element) == GLENUMERATION_FALSE)
			return;
	}

	//TODO: don't know requirements: (assumed)
	//TODO: this is only for OpenGL 1.0 through 1.3
	//GL_SGIS_multisample is not required
	if(StringToken(extensions, "GL_SGIS_point_parameters")/* && StringToken(extensions, "GL_SGIS_multisample")*/)
	{
		gglPointParameterfSGIS = (void (APIENTRY *)(GLenum pname, GLfloat param))gwglGetProcAddress("glPointParameterfSGIS");
		gglPointParameterfvSGIS = (void (APIENTRY *)(GLenum pname, const GLfloat *params))gwglGetProcAddress("glPointParameterfvSGIS");

		if(OpenGL_ExtensionsEnumeration("GL_SGIS_point_parameters", GGL_SGIS_point_parameters, element) == GLENUMERATION_FALSE)
			return;
	}

	//XXX: not complete; needs GLX protocol
	if(StringToken(extensions, "GL_SGIX_instruments"))
	{
		gglGetInstrumentsSGIX = (GLint (APIENTRY *)(void))gwglGetProcAddress("glGetInstrumentsSGIX");
		gglInstrumentsBufferSGIX = (void (APIENTRY *)(GLsizei size, GLint *buffer))gwglGetProcAddress("glInstrumentsBufferSGIX");
		gglPollInstrumentsSGIX = (GLint (APIENTRY *)(GLint *marker_p))gwglGetProcAddress("glPollInstrumentsSGIX");
		gglReadInstrumentsSGIX = (void (APIENTRY *)(GLint marker))gwglGetProcAddress("glReadInstrumentsSGIX");
		gglStartInstrumentsSGIX = (void (APIENTRY *)(void))gwglGetProcAddress("glStartInstrumentsSGIX");
		gglStopInstrumentsSGIX = (void (APIENTRY *)(GLint marker))gwglGetProcAddress("glStopInstrumentsSGIX");

		if(OpenGL_ExtensionsEnumeration("GL_SGIX_instruments", GGL_SGIX_instruments, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_SGIX_texture_scale_bias") /* && StringToken(extensions, "GL_EXT_texture") && StringToken(extensions, "GL_EXT_texture3D") && StringToken(extensions, "GL_EXT_texture_object")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_SGIX_texture_scale_bias", GGL_SGIX_texture_scale_bias, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_SGIX_framezoom"))
	{
		gglFrameZoomSGIX = (void (APIENTRY *)(GLint factor))gwglGetProcAddress("glFrameZoomSGIX");

		if(OpenGL_ExtensionsEnumeration("GL_SGIX_framezoom", GGL_SGIX_framezoom, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_SGIX_tag_sample_buffer") /* && StringToken(extensions, "GL_SGIS_multisample")*/)
	{
		gglTagSampleBufferSGIX = (void (APIENTRY *)(void))gwglGetProcAddress("glTagSampleBufferSGIX");

		if(OpenGL_ExtensionsEnumeration("GL_SGIX_tag_sample_buffer", GGL_SGIX_tag_sample_buffer, element) == GLENUMERATION_FALSE)
			return;
	}

	//TODO: don't know requirements
	if(StringToken(extensions, "GL_SGIX_polynomial_ffd"))
	{
		gglDeformationMap3dSGIX = (void (APIENTRY *)(GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, GLdouble w1, GLdouble w2, GLint wstride, GLint worder, const GLdouble *points))gwglGetProcAddress("glDeformationMap3dSGIX");
		gglDeformationMap3fSGIX = (void (APIENTRY *)(GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, GLfloat w1, GLfloat w2, GLint wstride, GLint worder, const GLfloat *points))gwglGetProcAddress("glDeformationMap3fSGIX");
		gglDeformSGIX = (void (APIENTRY *)(GLbitfield mask))gwglGetProcAddress("glDeformSGIX");
		gglLoadIdentityDeformationMapSGIX = (void (APIENTRY *)(GLbitfield mask))gwglGetProcAddress("glLoadIdentityDeformationMapSGIX");

		if(OpenGL_ExtensionsEnumeration("GL_SGIX_polynomial_ffd", GGL_SGIX_polynomial_ffd, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_SGIX_reference_plane"))
	{
		gglReferencePlaneSGIX = (void (APIENTRY *)(const GLdouble *equation))gwglGetProcAddress("glReferencePlaneSGIX");

		if(OpenGL_ExtensionsEnumeration("GL_SGIX_reference_plane", GGL_SGIX_reference_plane, element) == GLENUMERATION_FALSE)
			return;
	}

	//XXX: not complete
	if(StringToken(extensions, "GL_SGIX_flush_raster"))
	{
		gglFlushRasterSGIX = (void (APIENTRY *)(void))gwglGetProcAddress("glFlushRasterSGIX");

		if(OpenGL_ExtensionsEnumeration("GL_SGIX_flush_raster", GGL_SGIX_flush_raster, element) == GLENUMERATION_FALSE)
			return;
	}

	//GL_EXT_subtexture && GL_EXT_copy_texture are not required
	if(StringToken(extensions, "GL_SGIX_depth_texture") /* && StringToken(extensions, "GL_EXT_texture")*//* && StringToken(extensions, "GL_EXT_subtexture") && StringToken(extensions, "GL_EXT_copy_texture")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_SGIX_depth_texture", GGL_SGIX_depth_texture, element) == GLENUMERATION_FALSE)
			return;
	}

	if(/*atof(version) >= 1.1 && */StringToken(extensions, "GL_SGIS_fog_function"))
	{
		gglFogFuncSGIS = (void (APIENTRY *)(GLsizei n, const GLfloat *points))gwglGetProcAddress("glFogFuncSGIS");
		gglGetFogFuncSGIS = (void (APIENTRY *)(GLfloat *points))gwglGetProcAddress("glGetFogFuncSGIS");

		if(OpenGL_ExtensionsEnumeration("GL_SGIS_fog_function", GGL_SGIS_fog_function, element) == GLENUMERATION_FALSE)
			return;
	}

	if(/*atof(version) >= 1.0 && */StringToken(extensions, "GL_SGIX_fog_offset"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_SGIX_fog_offset", GGL_SGIX_fog_offset, element) == GLENUMERATION_FALSE)
			return;
	}

	//GL_EXT_convolution is not required
	if(StringToken(extensions, "GL_HP_image_transform") /* && StringToken(extensions, "GL_EXT_texture")*//* && StringToken(extensions, "GL_EXT_convolution")*/ /* && StringToken(extensions, "GL_SGI_color_table")*/)
	{
		gglImageTransformParameteriHP = (void (APIENTRY *)(GLenum target, GLenum pname, GLint param))gwglGetProcAddress("glImageTransformParameteriHP");
		gglImageTransformParameterfHP = (void (APIENTRY *)(GLenum target, GLenum pname, GLfloat param))gwglGetProcAddress("glImageTransformParameterfHP");
		gglImageTransformParameterivHP = (void (APIENTRY *)(GLenum target, GLenum pname, const GLint *params))gwglGetProcAddress("glImageTransformParameterivHP");
		gglImageTransformParameterfvHP = (void (APIENTRY *)(GLenum target, GLenum pname, const GLfloat *params))gwglGetProcAddress("glImageTransformParameterfvHP");
		gglGetImageTransformParameterivHP = (void (APIENTRY *)(GLenum target, GLenum pname, GLint *params))gwglGetProcAddress("glGetImageTransformParameterivHP");
		gglGetImageTransformParameterfvHP = (void (APIENTRY *)(GLenum target, GLenum pname, GLfloat *params))gwglGetProcAddress("glGetImageTransformParameterfvHP");

		if(OpenGL_ExtensionsEnumeration("GL_HP_image_transform", GGL_HP_image_transform, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_HP_convolution_border_modes") /* && StringToken(extensions, "GL_EXT_convolution")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_HP_convolution_border_modes", GGL_HP_convolution_border_modes, element) == GLENUMERATION_FALSE)
			return;
	}

	//TODO: don't know requirements
	if(StringToken(extensions, "GL_INGR_palette_buffer"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_INGR_palette_buffer", GGL_INGR_palette_buffer, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_SGIX_texture_add_env"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_SGIX_texture_add_env", GGL_SGIX_texture_add_env, element) == GLENUMERATION_FALSE)
			return;
	}

	//TODO: this is only for OpenGL 1.0 && 1.1
	//GL_EXT_color_table && GL_EXT_texture_color_table && GL_EXT_copy_texture are not required
	if(/*atof(version) <= 1.1 && */StringToken(extensions, "GL_EXT_color_subtable")/* && StringToken(extensions, "GL_EXT_color_table") && StringToken(extensions, "GL_EXT_texture_color_table") && StringToken(extensions, "GL_EXT_copy_texture")*/)
	{
		gglColorSubTableEXT = (void (APIENTRY *)(GLenum target, GLsizei start, GLsizei count, GLenum format, GLenum type, const GLvoid *data))gwglGetProcAddress("glColorSubTableEXT");
		gglCopyColorSubTableEXT = (void (APIENTRY *)(GLenum target, GLsizei start, GLint x, GLint y, GLsizei width))gwglGetProcAddress("glCopyColorSubTableEXT");

		if(OpenGL_ExtensionsEnumeration("GL_EXT_color_subtable", GGL_EXT_color_subtable, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_PGI_vertex_hints"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_PGI_vertex_hints", GGL_PGI_vertex_hints, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_PGI_misc_hints"))
	{
		gglHintPGI = (void (APIENTRY *)(GLenum target, GLint mode))gwglGetProcAddress("glHintPGI");

		if(OpenGL_ExtensionsEnumeration("GL_PGI_misc_hints", GGL_PGI_misc_hints, element) == GLENUMERATION_FALSE)
			return;
	}

	//TODO: this is only for OpenGL 1.0 && 1.1
	//GL_SGI_color_table && GL_EXT_texture3D && GL_ARB_texture_cube_map are not required
	if(/*atof(version) <= 1.1 && */StringToken(extensions, "GL_EXT_paletted_texture")/* && StringToken(extensions, "GL_SGI_color_table") && StringToken(extensions, "GL_EXT_texture3D") && StringToken(extensions, "GL_ARB_texture_cube_map")*/)
	{
		gglColorTableEXT = (void (APIENTRY *)(GLenum target, GLenum internalFormat, GLsizei width, GLenum format, GLenum type, const GLvoid *table))gwglGetProcAddress("glColorTableEXT");
		gglGetColorTableEXT = (void (APIENTRY *)(GLenum target, GLenum format, GLenum type, GLvoid *data))gwglGetProcAddress("glGetColorTableEXT");
		gglGetColorTableParameterivEXT = (void (APIENTRY *)(GLenum target, GLenum pname, GLint *params))gwglGetProcAddress("glGetColorTableParameterivEXT");
		gglGetColorTableParameterfvEXT = (void (APIENTRY *)(GLenum target, GLenum pname, GLfloat *params))gwglGetProcAddress("glGetColorTableParameterfvEXT");

		if(OpenGL_ExtensionsEnumeration("GL_EXT_paletted_texture", GGL_EXT_paletted_texture, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_EXT_clip_volume_hint"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_EXT_clip_volume_hint", GGL_EXT_clip_volume_hint, element) == GLENUMERATION_FALSE)
			return;
	}

	//XXX: not complete
	if(StringToken(extensions, "GL_SGIX_list_priority"))
	{
		gglGetListParameterfvSGIX = (void (APIENTRY *)(GLuint list, GLenum pname, GLfloat *params))gwglGetProcAddress("glGetListParameterfvSGIX");
		gglGetListParameterivSGIX = (void (APIENTRY *)(GLuint list, GLenum pname, GLint *params))gwglGetProcAddress("glGetListParameterivSGIX");
		gglListParameterfSGIX = (void (APIENTRY *)(GLuint list, GLenum pname, GLfloat param))gwglGetProcAddress("glListParameterfSGIX");
		gglListParameterfvSGIX = (void (APIENTRY *)(GLuint list, GLenum pname, const GLfloat *params))gwglGetProcAddress("glListParameterfvSGIX");
		gglListParameteriSGIX = (void (APIENTRY *)(GLuint list, GLenum pname, GLint param))gwglGetProcAddress("glListParameteriSGIX");
		gglListParameterivSGIX = (void (APIENTRY *)(GLuint list, GLenum pname, const GLint *params))gwglGetProcAddress("glListParameterivSGIX");

		if(OpenGL_ExtensionsEnumeration("GL_SGIX_list_priority", GGL_SGIX_list_priority, element) == GLENUMERATION_FALSE)
			return;
	}

	//XXX: not complete
	if(StringToken(extensions, "GL_SGIX_ir_instrument1") /* && StringToken(extensions, "GL_SGIX_instruments")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_SGIX_ir_instrument1", GGL_SGIX_ir_instrument1, element) == GLENUMERATION_FALSE)
			return;
	}

	//TODO: don't know requirements
	if(StringToken(extensions, "GL_SGIX_calligraphic_fragment"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_SGIX_calligraphic_fragment", GGL_SGIX_calligraphic_fragment, element) == GLENUMERATION_FALSE)
			return;
	}

	//GL_EXT_texture3D && GL_EXT_texture_object are not required
	if(StringToken(extensions, "GL_SGIX_texture_lod_bias") /* && StringToken(extensions, "GL_EXT_texture")*//* && StringToken(extensions, "GL_EXT_texture3D") && StringToken(extensions, "GL_EXT_texture_object")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_SGIX_texture_lod_bias", GGL_SGIX_texture_lod_bias, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_SGIX_shadow_ambient"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_SGIX_shadow_ambient", GGL_SGIX_shadow_ambient, element) == GLENUMERATION_FALSE)
			return;
	}

	//XXX: not complete
	if(StringToken(extensions, "GL_EXT_index_texture") /* && StringToken(extensions, "GL_EXT_paletted_texture")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_EXT_index_texture", GGL_EXT_index_texture, element) == GLENUMERATION_FALSE)
			return;
	}

	//XXX: not complete
	if(StringToken(extensions, "GL_EXT_index_material"))
	{
		gglIndexMaterialEXT = (void (APIENTRY *)(GLenum face, GLenum mode))gwglGetProcAddress("glIndexMaterialEXT");

		if(OpenGL_ExtensionsEnumeration("GL_EXT_index_material", GGL_EXT_index_material, element) == GLENUMERATION_FALSE)
			return;
	}

	//XXX:not complete
	if(StringToken(extensions, "GL_EXT_index_func"))
	{
		gglIndexFuncEXT = (void (APIENTRY *)(GLenum func, GLclampf ref))gwglGetProcAddress("glIndexFuncEXT");

		if(OpenGL_ExtensionsEnumeration("GL_EXT_index_func", GGL_EXT_index_func, element) == GLENUMERATION_FALSE)
			return;
	}

	//XXX:not complete
	if(StringToken(extensions, "GL_EXT_index_array_formats"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_EXT_index_array_formats", GGL_EXT_index_array_formats, element) == GLENUMERATION_FALSE)
			return;
	}

	//XXX: not complete
	if(StringToken(extensions, "GL_EXT_compiled_vertex_array"))
	{
		gglLockArraysEXT = (void (APIENTRY *)(GLint first, GLsizei count))gwglGetProcAddress("glLockArraysEXT");
		gglUnlockArraysEXT = (void (APIENTRY *)(void))gwglGetProcAddress("glUnlockArraysEXT");

		if(OpenGL_ExtensionsEnumeration("GL_EXT_compiled_vertex_array", GGL_EXT_compiled_vertex_array, element) == GLENUMERATION_FALSE)
			return;
	}

	//XXX: not complete
	if(StringToken(extensions, "GL_EXT_cull_vertex"))
	{
		gglCullParameterdvEXT = (void (APIENTRY *)(GLenum pname, GLdouble *params))gwglGetProcAddress("glCullParameterdvEXT");
		gglCullParameterfvEXT = (void (APIENTRY *)(GLenum pname, GLfloat *params))gwglGetProcAddress("glCullParameterfvEXT");

		if(OpenGL_ExtensionsEnumeration("GL_EXT_cull_vertex", GGL_EXT_cull_vertex, element) == GLENUMERATION_FALSE)
			return;
	}

	//XXX: not complete
	//GL_EXT_texture3D && GL_EXT_subtexture && GL_EXT_histogram &&
	//GL_EXT_convolution && GL_SGI_color_table are not required
	if(StringToken(extensions, "GL_SGIX_ycrcb")/* && StringToken(extensions, "GL_EXT_texture3D") && StringToken(extensions, "GL_EXT_subtexture") && StringToken(extensions, "GL_EXT_histogram") && StringToken(extensions, "GL_EXT_convolution") && StringToken(extensions, "GL_SGI_color_table")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_SGIX_ycrcb", GGL_SGIX_ycrcb, element) == GLENUMERATION_FALSE)
			return;
	}

	//TODO: don't know requirements: (assumed)
	if(/*atof(version) >= 1.1 && */StringToken(extensions, "GL_SGIX_fragment_lighting"))
	{
		gglFragmentColorMaterialSGIX = (void (APIENTRY *)(GLenum face, GLenum mode))gwglGetProcAddress("glFragmentColorMaterialSGIX");
		gglFragmentLightfSGIX = (void (APIENTRY *)(GLenum light, GLenum pname, GLfloat param))gwglGetProcAddress("glFragmentLightfSGIX");
		gglFragmentLightfvSGIX = (void (APIENTRY *)(GLenum light, GLenum pname, const GLfloat *params))gwglGetProcAddress("glFragmentLightfvSGIX");
		gglFragmentLightiSGIX = (void (APIENTRY *)(GLenum light, GLenum pname, GLint param))gwglGetProcAddress("glFragmentLightiSGIX");
		gglFragmentLightivSGIX = (void (APIENTRY *)(GLenum light, GLenum pname, const GLint *params))gwglGetProcAddress("glFragmentLightivSGIX");
		gglFragmentLightModelfSGIX = (void (APIENTRY *)(GLenum pname, GLfloat param))gwglGetProcAddress("glFragmentLightModelfSGIX");
		gglFragmentLightModelfvSGIX = (void (APIENTRY *)(GLenum pname, const GLfloat *params))gwglGetProcAddress("glFragmentLightModelfvSGIX");
		gglFragmentLightModeliSGIX = (void (APIENTRY *)(GLenum pname, GLint param))gwglGetProcAddress("glFragmentLightModeliSGIX");
		gglFragmentLightModelivSGIX = (void (APIENTRY *)(GLenum pname, const GLint *params))gwglGetProcAddress("glFragmentLightModelivSGIX");
		gglFragmentMaterialfSGIX = (void (APIENTRY *)(GLenum face, GLenum pname, GLfloat param))gwglGetProcAddress("glFragmentMaterialfSGIX");
		gglFragmentMaterialfvSGIX = (void (APIENTRY *)(GLenum face, GLenum pname, const GLfloat *params))gwglGetProcAddress("glFragmentMaterialfvSGIX");
		gglFragmentMaterialiSGIX = (void (APIENTRY *)(GLenum face, GLenum pname, GLint param))gwglGetProcAddress("glFragmentMaterialiSGIX");
		gglFragmentMaterialivSGIX = (void (APIENTRY *)(GLenum face, GLenum pname, const GLint *params))gwglGetProcAddress("glFragmentMaterialivSGIX");
		gglGetFragmentLightfvSGIX = (void (APIENTRY *)(GLenum light, GLenum pname, GLfloat *params))gwglGetProcAddress("glGetFragmentLightfvSGIX");
		gglGetFragmentLightivSGIX = (void (APIENTRY *)(GLenum light, GLenum pname, GLint *params))gwglGetProcAddress("glGetFragmentLightivSGIX");
		gglGetFragmentMaterialfvSGIX = (void (APIENTRY *)(GLenum face, GLenum pname, GLfloat *params))gwglGetProcAddress("glGetFragmentMaterialfvSGIX");
		gglGetFragmentMaterialivSGIX = (void (APIENTRY *)(GLenum face, GLenum pname, GLint *params))gwglGetProcAddress("glGetFragmentMaterialivSGIX");
		gglLightEnviSGIX = (void (APIENTRY *)(GLenum pname, GLint param))gwglGetProcAddress("glLightEnviSGIX");

		if(OpenGL_ExtensionsEnumeration("GL_SGIX_fragment_lighting", GGL_SGIX_fragment_lighting, element) == GLENUMERATION_FALSE)
			return;
	}

	//XXX: do not implement this extension
	if(/*atof(version) >= 1.1 && */StringToken(extensions, "GL_EXT_fragment_lighting"))
	{
		gglFragmentColorMaterialEXT = (void (APIENTRY *)(GLenum face, GLenum mode))gwglGetProcAddress("glFragmentColorMaterialEXT");
		gglFragmentLightfEXT = (void (APIENTRY *)(GLenum light, GLenum pname, GLfloat param))gwglGetProcAddress("glFragmentLightfEXT");
		gglFragmentLightfvEXT = (void (APIENTRY *)(GLenum light, GLenum pname, const GLfloat *params))gwglGetProcAddress("glFragmentLightfvEXT");
		gglFragmentLightiEXT = (void (APIENTRY *)(GLenum light, GLenum pname, GLint param))gwglGetProcAddress("glFragmentLightiEXT");
		gglFragmentLightivEXT = (void (APIENTRY *)(GLenum light, GLenum pname, const GLint *params))gwglGetProcAddress("glFragmentLightivEXT");
		gglFragmentLightModelfEXT = (void (APIENTRY *)(GLenum pname, GLfloat param))gwglGetProcAddress("glFragmentLightModelfEXT");
		gglFragmentLightModelfvEXT = (void (APIENTRY *)(GLenum pname, const GLfloat *params))gwglGetProcAddress("glFragmentLightModelfvEXT");
		gglFragmentLightModeliEXT = (void (APIENTRY *)(GLenum pname, GLint param))gwglGetProcAddress("glFragmentLightModeliEXT");
		gglFragmentLightModelivEXT = (void (APIENTRY *)(GLenum pname, const GLint *params))gwglGetProcAddress("glFragmentLightModelivEXT");
		gglFragmentMaterialfEXT = (void (APIENTRY *)(GLenum face, GLenum pname, GLfloat param))gwglGetProcAddress("glFragmentMaterialfEXT");
		gglFragmentMaterialfvEXT = (void (APIENTRY *)(GLenum face, GLenum pname, const GLfloat *params))gwglGetProcAddress("glFragmentMaterialfvEXT");
		gglFragmentMaterialiEXT = (void (APIENTRY *)(GLenum face, GLenum pname, GLint param))gwglGetProcAddress("glFragmentMaterialiEXT");
		gglFragmentMaterialivEXT = (void (APIENTRY *)(GLenum face, GLenum pname, const GLint *params))gwglGetProcAddress("glFragmentMaterialivEXT");
		gglGetFragmentLightfvEXT = (void (APIENTRY *)(GLenum light, GLenum pname, GLfloat *params))gwglGetProcAddress("glGetFragmentLightfvEXT");
		gglGetFragmentLightivEXT = (void (APIENTRY *)(GLenum light, GLenum pname, GLint *params))gwglGetProcAddress("glGetFragmentLightivEXT");
		gglGetFragmentMaterialfvEXT = (void (APIENTRY *)(GLenum face, GLenum pname, GLfloat *params))gwglGetProcAddress("glGetFragmentMaterialfvEXT");
		gglGetFragmentMaterialivEXT = (void (APIENTRY *)(GLenum face, GLenum pname, GLint *params))gwglGetProcAddress("glGetFragmentMaterialivEXT");
		gglLightEnviEXT = (void (APIENTRY *)(GLenum pname, GLint param))gwglGetProcAddress("glLightEnviEXT");

		if(OpenGL_ExtensionsEnumeration("GL_EXT_fragment_lighting", GGL_EXT_fragment_lighting, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_IBM_rasterpos_clip"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_IBM_rasterpos_clip", GGL_IBM_rasterpos_clip, element) == GLENUMERATION_FALSE)
			return;
	}

	//XXX: not complete
	if(/*atof(version) >= 1.1 && */StringToken(extensions, "GL_HP_texture_lighting"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_HP_texture_lighting", GGL_HP_texture_lighting, element) == GLENUMERATION_FALSE)
			return;
	}

	//TODO: this is only for OpenGL 1.0 && 1.1
	if(/*atof(version) <= 1.1 && */StringToken(extensions, "GL_EXT_draw_range_elements"))
	{
		gglDrawRangeElementsEXT = (void (APIENTRY *)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices))gwglGetProcAddress("glDrawRangeElementsEXT");

		if(OpenGL_ExtensionsEnumeration("GL_EXT_draw_range_elements", GGL_EXT_draw_range_elements, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_WIN_phong_shading"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_WIN_phong_shading", GGL_WIN_phong_shading, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_WIN_specular_fog"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_WIN_specular_fog", GGL_WIN_specular_fog, element) == GLENUMERATION_FALSE)
			return;
	}

	//GL_EXT_multitexture && GL_SGIX_shadow are not required
	if(/*atof(version) >= 1.1 && */StringToken(extensions, "GL_EXT_light_texture") /* && StringToken(extensions, "GL_EXT_fragment_lighting")*//* && StringToken(extensions, "GL_EXT_multitexture") && StringToken(extensions, "GL_SGIX_shadow")*/)
	{
		gglApplyTextureEXT = (void (APIENTRY *)(GLenum mode))gwglGetProcAddress("glApplyTextureEXT");
		gglTextureLightEXT = (void (APIENTRY *)(GLenum pname))gwglGetProcAddress("glTextureLightEXT");
		gglTextureMaterialEXT = (void (APIENTRY *)(GLenum face, GLenum mode))gwglGetProcAddress("glTextureMaterialEXT");

		if(OpenGL_ExtensionsEnumeration("GL_EXT_light_texture", GGL_EXT_light_texture, element) == GLENUMERATION_FALSE)
			return;
	}

	//XXX: not complete
	//GL_EXT_blend_subtract is not required
	if(StringToken(extensions, "GL_SGIX_blend_alpha_minmax") /* && StringToken(extensions, "GL_EXT_blend_minmax")*//* && StringToken(extensions, "GL_EXT_blend_subtract")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_SGIX_blend_alpha_minmax", GGL_SGIX_blend_alpha_minmax, element) == GLENUMERATION_FALSE)
			return;
	}

	if(/*atof(version) >= 1.1 && */StringToken(extensions, "GL_EXT_scene_marker"))
	{
		gglBeginSceneEXT = (void (APIENTRY *)(GLvoid))gwglGetProcAddress("glBeginSceneEXT");
	    gglEndSceneEXT = (void (APIENTRY *)(GLvoid))gwglGetProcAddress("glEndSceneEXT");

		if(OpenGL_ExtensionsEnumeration("GL_EXT_scene_marker", GGL_EXT_scene_marker, element) == GLENUMERATION_FALSE)
			return;
	}

	//GL_SGIS_pixel_texture && GL_SGIX_pixel_texture_lod are not required
	if(StringToken(extensions, "GL_SGIX_pixel_texture_bits")/* && StringToken(extensions, "GL_SGIS_pixel_texture") && StringToken(extensions, "GL_SGIX_pixel_texture_lod")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_SGIX_pixel_texture_bits", GGL_SGIX_pixel_texture_bits, element) == GLENUMERATION_FALSE)
			return;
	}

	//TODO: don't know requirements
	if(StringToken(extensions, "GL_SGIX_impact_pixel_texture"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_SGIX_impact_pixel_texture", GGL_SGIX_impact_pixel_texture, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_EXT_bgra"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_EXT_bgra", GGL_EXT_bgra, element) == GLENUMERATION_FALSE)
			return;
	}
	
	//XXX: not complete
	if(StringToken(extensions, "GL_SGIX_async"))
	{
		gglAsyncMarkerSGIX = (void (APIENTRY *)(GLuint marker))gwglGetProcAddress("glAsyncMarkerSGIX");
		gglFinishAsyncSGIX = (GLint (APIENTRY *)(GLuint *markerp))gwglGetProcAddress("glFinishAsyncSGIX");
		gglPollAsyncSGIX = (GLint (APIENTRY *)(GLuint *markerp))gwglGetProcAddress("glPollAsyncSGIX");
		gglGenAsyncMarkersSGIX = (GLuint (APIENTRY *)(GLsizei range))gwglGetProcAddress("glGenAsyncMarkersSGIX");
		gglDeleteAsyncMarkersSGIX = (void (APIENTRY *)(GLuint marker, GLsizei range))gwglGetProcAddress("glDeleteAsyncMarkersSGIX");
		gglIsAsyncMarkerSGIX = (GLboolean (APIENTRY *)(GLuint marker))gwglGetProcAddress("glIsAsyncMarkerSGIX");

		if(OpenGL_ExtensionsEnumeration("GL_SGIX_async", GGL_SGIX_async, element) == GLENUMERATION_FALSE)
			return;
	}

	//XXX: not complete
	//GL_EXT_texture3D && GL_SGIS_texture4D are not required
	if(StringToken(extensions, "GL_SGIX_async_pixel") /* && StringToken(extensions, "GL_SGIX_async")*//* && StringToken(extensions, "GL_EXT_texture3D") && StringToken(extensions, "GL_SGIS_texture4D")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_SGIX_async_pixel", GGL_SGIX_async_pixel, element) == GLENUMERATION_FALSE)
			return;
	}

	//XXX: not complete
	if(StringToken(extensions, "GL_SGIX_async_histogram") /* && StringToken(extensions, "GL_SGIX_async") && StringToken(extensions, "GL_EXT_histogram")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_SGIX_async_histogram", GGL_SGIX_async_histogram, element) == GLENUMERATION_FALSE)
			return;
	}

	//GL_EXT_texture3D is not required
	if(StringToken(extensions, "GL_INTEL_texture_scissor")/* && StringToken(extensions, "GL_EXT_texture3D")*/ /* && StringToken(extensions, "GL_EXT_texture")*/)
	{
		gglTexScissorINTEL = (void (APIENTRY *)(GLenum target, GLclampf tlow, GLclampf thigh))gwglGetProcAddress("glTexScissorINTEL");
		gglTexScissorFuncINTEL = (void (APIENTRY *)(GLenum target, GLenum lfunc, GLenum hfunc))gwglGetProcAddress("glTexScissorFuncINTEL");

		if(OpenGL_ExtensionsEnumeration("GL_INTEL_texture_scissor", GGL_INTEL_texture_scissor, element) == GLENUMERATION_FALSE)
			return;
	}

	if(/*atof(version) >= 1.1 && */StringToken(extensions, "GL_INTEL_parallel_arrays"))
	{
		gglVertexPointervINTEL = (void (APIENTRY *)(GLint size, GLenum type, const GLvoid* *pointer))gwglGetProcAddress("glVertexPointervINTEL");
		gglNormalPointervINTEL = (void (APIENTRY *)(GLenum type, const GLvoid* *pointer))gwglGetProcAddress("glNormalPointervINTEL");
		gglColorPointervINTEL = (void (APIENTRY *)(GLint size, GLenum type, const GLvoid* *pointer))gwglGetProcAddress("glColorPointervINTEL");
		gglTexCoordPointervINTEL = (void (APIENTRY *)(GLint size, GLenum type, const GLvoid* *pointer))gwglGetProcAddress("glTexCoordPointervINTEL");

		if(OpenGL_ExtensionsEnumeration("GL_INTEL_parallel_arrays", GGL_INTEL_parallel_arrays, element) == GLENUMERATION_FALSE)
			return;
	}

	//XXX: not complete: preliminary
	if(StringToken(extensions, "GL_HP_occlusion_test"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_HP_occlusion_test", GGL_HP_occlusion_test, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_EXT_pixel_transform"))
	{
		gglPixelTransformParameteriEXT = (void (APIENTRY *)(GLenum target, GLenum pname, GLint param))gwglGetProcAddress("glPixelTransformParameteriEXT");
		gglPixelTransformParameterfEXT = (void (APIENTRY *)(GLenum target, GLenum pname, GLfloat param))gwglGetProcAddress("glPixelTransformParameterfEXT");
		gglPixelTransformParameterivEXT = (void (APIENTRY *)(GLenum target, GLenum pname, const GLint *params))gwglGetProcAddress("glPixelTransformParameterivEXT");
		gglPixelTransformParameterfvEXT = (void (APIENTRY *)(GLenum target, GLenum pname, const GLfloat *params))gwglGetProcAddress("glPixelTransformParameterfvEXT");

		if(OpenGL_ExtensionsEnumeration("GL_EXT_pixel_transform", GGL_EXT_pixel_transform, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_EXT_pixel_transform_color_table") /* && StringToken(extensions, "GL_SGI_color_table") && StringToken(extensions, "GL_EXT_pixel_transform")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_EXT_pixel_transform_color_table", GGL_EXT_pixel_transform_color_table, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_EXT_shared_texture_palette") /* && StringToken(extensions, "GL_EXT_paletted_texture")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_EXT_shared_texture_palette", GGL_EXT_shared_texture_palette, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_EXT_separate_specular_color"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_EXT_separate_specular_color", GGL_EXT_separate_specular_color, element) == GLENUMERATION_FALSE)
			return;
	}

	//TODO: this is only for OpenGL 1.0 through 1.3
	if(StringToken(extensions, "GL_EXT_secondary_color") /* && (atof(version) >= 1.2 || StringToken(extensions, "GL_EXT_separate_specular_color"))*/)
	{
		gglSecondaryColor3bEXT = (void (APIENTRY *)(GLbyte red, GLbyte green, GLbyte blue))gwglGetProcAddress("glSecondaryColor3bEXT");
		gglSecondaryColor3bvEXT = (void (APIENTRY *)(const GLbyte *v))gwglGetProcAddress("glSecondaryColor3bvEXT");
		gglSecondaryColor3dEXT = (void (APIENTRY *)(GLdouble red, GLdouble green, GLdouble blue))gwglGetProcAddress("glSecondaryColor3dEXT");
		gglSecondaryColor3dvEXT = (void (APIENTRY *)(const GLdouble *v))gwglGetProcAddress("glSecondaryColor3dvEXT");
		gglSecondaryColor3fEXT = (void (APIENTRY *)(GLfloat red, GLfloat green, GLfloat blue))gwglGetProcAddress("glSecondaryColor3fEXT");
		gglSecondaryColor3fvEXT = (void (APIENTRY *)(const GLfloat *v))gwglGetProcAddress("glSecondaryColor3fvEXT");
		gglSecondaryColor3iEXT = (void (APIENTRY *)(GLint red, GLint green, GLint blue))gwglGetProcAddress("glSecondaryColor3iEXT");
		gglSecondaryColor3ivEXT = (void (APIENTRY *)(const GLint *v))gwglGetProcAddress("glSecondaryColor3ivEXT");
		gglSecondaryColor3sEXT = (void (APIENTRY *)(GLshort red, GLshort green, GLshort blue))gwglGetProcAddress("glSecondaryColor3sEXT");
		gglSecondaryColor3svEXT = (void (APIENTRY *)(const GLshort *v))gwglGetProcAddress("glSecondaryColor3svEXT");
		gglSecondaryColor3ubEXT = (void (APIENTRY *)(GLubyte red, GLubyte green, GLubyte blue))gwglGetProcAddress("glSecondaryColor3ubEXT");
		gglSecondaryColor3ubvEXT = (void (APIENTRY *)(const GLubyte *v))gwglGetProcAddress("glSecondaryColor3ubvEXT");
		gglSecondaryColor3uiEXT = (void (APIENTRY *)(GLuint red, GLuint green, GLuint blue))gwglGetProcAddress("glSecondaryColor3uiEXT");
		gglSecondaryColor3uivEXT = (void (APIENTRY *)(const GLuint *v))gwglGetProcAddress("glSecondaryColor3uivEXT");
		gglSecondaryColor3usEXT = (void (APIENTRY *)(GLushort red, GLushort green, GLushort blue))gwglGetProcAddress("glSecondaryColor3usEXT");
		gglSecondaryColor3usvEXT = (void (APIENTRY *)(const GLushort *v))gwglGetProcAddress("glSecondaryColor3usvEXT");
		gglSecondaryColorPointerEXT = (void (APIENTRY *)(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer))gwglGetProcAddress("glSecondaryColorPointerEXT");

		if(OpenGL_ExtensionsEnumeration("GL_EXT_secondary_color", GGL_EXT_secondary_color, element) == GLENUMERATION_FALSE)
			return;
	}

	//OpenGL 1.2 && GL_EXT_multitexture are not required
	if(/*atof(version) >= 1.1 && */StringToken(extensions, "GL_EXT_texture_env")/* && atof(version) >= 1.2 && StringToken(extensions, "GL_EXT_multitexture")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_EXT_texture_env", GGL_EXT_texture_env, element) == GLENUMERATION_FALSE)
			return;
	}

	if(/*atof(version) >= 1.1 && */StringToken(extensions, "GL_EXT_texture_perturb_normal") /* && StringToken(extensions, "GL_EXT_coordinate_frame") && StringToken(extensions, "GL_EXT_light_texture")*/)
	{
		gglTextureNormalEXT = (void (APIENTRY *)(GLenum mode))gwglGetProcAddress("glTextureNormalEXT");

		if(OpenGL_ExtensionsEnumeration("GL_EXT_texture_perturb_normal", GGL_EXT_texture_perturb_normal, element) == GLENUMERATION_FALSE)
			return;
	}

	//TODO: this is only for OpenGL 1.1 && 1.2 && 1.3
	if(/*atof(version) >= 1.1 && */StringToken(extensions, "GL_EXT_multi_draw_arrays") || StringToken(extensions, "GL_SUN_multi_draw_arrays"))
	{
		if(!(gglMultiDrawArraysEXT = (void (APIENTRY *)(GLenum mode, GLint *first, GLsizei *count, GLsizei primcount))gwglGetProcAddress("glMultiDrawArraysEXT")))
			gglMultiDrawArraysEXT = (void (APIENTRY *)(GLenum mode, GLint *first, GLsizei *count, GLsizei primcount))gwglGetProcAddress("glMultiDrawArraysSUN");

		if(!(gglMultiDrawElementsEXT = (void (APIENTRY *)(GLenum mode, const GLsizei *count, GLenum type, const GLvoid* *indices, GLsizei primcount))gwglGetProcAddress("glMultiDrawElementsEXT")))
			gglMultiDrawElementsEXT = (void (APIENTRY *)(GLenum mode, const GLsizei *count, GLenum type, const GLvoid* *indices, GLsizei primcount))gwglGetProcAddress("glMultiDrawElementsSUN");

		if(OpenGL_ExtensionsEnumeration("GL_EXT_multi_draw_arrays", GGL_EXT_multi_draw_arrays, element) == GLENUMERATION_FALSE)
			return;
	}

	//TODO: This is only for OpenGL 1.1 && 1.2 && 1.3
	if(/*atof(version) >= 1.1 && */StringToken(extensions, "GL_EXT_fog_coord"))
	{
		gglFogCoordfEXT = (void (APIENTRY *)(GLfloat coord))gwglGetProcAddress("glFogCoordfEXT");
		gglFogCoordfvEXT = (void (APIENTRY *)(const GLfloat *coord))gwglGetProcAddress("glFogCoordfvEXT");
		gglFogCoorddEXT = (void (APIENTRY *)(GLdouble coord))gwglGetProcAddress("glFogCoorddEXT");
		gglFogCoorddvEXT = (void (APIENTRY *)(const GLdouble *coord))gwglGetProcAddress("glFogCoorddvEXT");
		gglFogCoordPointerEXT = (void (APIENTRY *)(GLenum type, GLsizei stride, const GLvoid *pointer))gwglGetProcAddress("glFogCoordPointerEXT");

		if(OpenGL_ExtensionsEnumeration("GL_EXT_fog_coord", GGL_EXT_fog_coord, element) == GLENUMERATION_FALSE)
			return;
	}

	//XXX: not complete
	//GL_EXT_fog_coord && GL_EXT_cull_vertex are not required
	if(/*atof(version) >= 1.1 && */StringToken(extensions, "GL_REND_screen_coordinates")/* && StringToken(extensions, "GL_EXT_fog_coord") && StringToken(extensions, "GL_EXT_cull_vertex")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_REND_screen_coordinates", GGL_REND_screen_coordinates, element) == GLENUMERATION_FALSE)
			return;
	}

	//XXX: incomplete; don't implement this extension
	if(/*atof(version) >= 1.1 && */StringToken(extensions, "GL_EXT_coordinate_frame"))
	{
/*		gglTangent3bEXT = (void (APIENTRY *)(GLbyte tx, GLbyte ty, GLbyte tz))gwglGetProcAddress("glTangent3bEXT");
		gglTangent3bvEXT = (void (APIENTRY *)(const GLbyte *v))gwglGetProcAddress("glTangent3bvEXT");
		gglTangent3dEXT = (void (APIENTRY *)(GLdouble tx, GLdouble ty, GLdouble tz))gwglGetProcAddress("glTangent3dEXT");
		gglTangent3dvEXT = (void (APIENTRY *)(const GLdouble *v))gwglGetProcAddress("glTangent3dvEXT");
		gglTangent3fEXT = (void (APIENTRY *)(GLfloat tx, GLfloat ty, GLfloat tz))gwglGetProcAddress("glTangent3fEXT");
		gglTangent3fvEXT = (void (APIENTRY *)(const GLfloat *v))gwglGetProcAddress("glTangent3fvEXT");
		gglTangent3iEXT = (void (APIENTRY *)(GLint tx, GLint ty, GLint tz))gwglGetProcAddress("glTangent3iEXT");
		gglTangent3ivEXT = (void (APIENTRY *)(const GLint *v))gwglGetProcAddress("glTangent3ivEXT");
		gglTangent3sEXT = (void (APIENTRY *)(GLshort tx, GLshort ty, GLshort tz))gwglGetProcAddress("glTangent3sEXT");
		gglTangent3svEXT = (void (APIENTRY *)(const GLshort *v))gwglGetProcAddress("glTangent3svEXT");
		gglBinormal3bEXT = (void (APIENTRY *)(GLbyte bx, GLbyte by, GLbyte bz))gwglGetProcAddress("glBinormal3bEXT");
		gglBinormal3bvEXT = (void (APIENTRY *)(const GLbyte *v))gwglGetProcAddress("glBinormal3bvEXT");
		gglBinormal3dEXT = (void (APIENTRY *)(GLdouble bx, GLdouble by, GLdouble bz))gwglGetProcAddress("glBinormal3dEXT");
		gglBinormal3dvEXT = (void (APIENTRY *)(const GLdouble *v))gwglGetProcAddress("glBinormal3dvEXT");
		gglBinormal3fEXT = (void (APIENTRY *)(GLfloat bx, GLfloat by, GLfloat bz))gwglGetProcAddress("glBinormal3fEXT");
		gglBinormal3fvEXT = (void (APIENTRY *)(const GLfloat *v))gwglGetProcAddress("glBinormal3fvEXT");
		gglBinormal3iEXT = (void (APIENTRY *)(GLint bx, GLint by, GLint bz))gwglGetProcAddress("glBinormal3iEXT");
		gglBinormal3ivEXT = (void (APIENTRY *)(const GLint *v))gwglGetProcAddress("glBinormal3ivEXT");
		gglBinormal3sEXT = (void (APIENTRY *)(GLshort bx, GLshort by, GLshort bz))gwglGetProcAddress("glBinormal3sEXT");
		gglBinormal3svEXT = (void (APIENTRY *)(const GLshort *v))gwglGetProcAddress("glBinormal3svEXT");
		gglTangentPointerEXT = (void (APIENTRY *)(GLenum type, GLsizei stride, const GLvoid *pointer))gwglGetProcAddress("glTangentPointerEXT");
		gglBinormalPointerEXT = (void (APIENTRY *)(GLenum type, GLsizei stride, const GLvoid *pointer))gwglGetProcAddress("glBinormalPointerEXT");
*/
		if(OpenGL_ExtensionsEnumeration("GL_EXT_coordinate_frame", GGL_EXT_coordinate_frame, element) == GLENUMERATION_FALSE)
			return;
	}

	//GL_SGI_texture_color_table && GL_SGIX_texture_scale_bias are not required
	if(StringToken(extensions, "GL_EXT_texture_env_combine")/* && StringToken(extensions, "GL_SGI_texture_color_table") && StringToken(extensions, "GL_SGIX_texture_scale_bias")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_EXT_texture_env_combine", GGL_EXT_texture_env_combine, element) == GLENUMERATION_FALSE)
			return;
	}

	//XXX: not complete
	if(StringToken(extensions, "GL_APPLE_specular_vector"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_APPLE_specular_vector", GGL_APPLE_specular_vector, element) == GLENUMERATION_FALSE)
			return;
	}

	//XXX: not complete
	if(StringToken(extensions, "GL_APPLE_transform_hint"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_APPLE_transform_hint", GGL_APPLE_transform_hint, element) == GLENUMERATION_FALSE)
			return;
	}

	//TODO: don't know requirements
	if(StringToken(extensions, "GL_SGIX_fog_scale"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_SGIX_fog_scale", GGL_SGIX_fog_scale, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_SUNX_constant_data"))
	{
		gglFinishTextureSUNX = (void (APIENTRY *)(void))gwglGetProcAddress("glFinishTextureSUNX");

		if(OpenGL_ExtensionsEnumeration("GL_SUNX_constant_data", GGL_SUNX_constant_data, element) == GLENUMERATION_FALSE)
			return;
	}

	if(/*atof(version) >= 1.2 && */StringToken(extensions, "GL_SUN_global_alpha"))
	{
		gglGlobalAlphaFactorbSUN = (void (APIENTRY *)(GLbyte factor))gwglGetProcAddress("glGlobalAlphaFactorbSUN");
		gglGlobalAlphaFactorsSUN = (void (APIENTRY *)(GLshort factor))gwglGetProcAddress("glGlobalAlphaFactorsSUN");
		gglGlobalAlphaFactoriSUN = (void (APIENTRY *)(GLint factor))gwglGetProcAddress("glGlobalAlphaFactoriSUN");
		gglGlobalAlphaFactorfSUN = (void (APIENTRY *)(GLfloat factor))gwglGetProcAddress("glGlobalAlphaFactorfSUN");
		gglGlobalAlphaFactordSUN = (void (APIENTRY *)(GLdouble factor))gwglGetProcAddress("glGlobalAlphaFactordSUN");
		gglGlobalAlphaFactorubSUN = (void (APIENTRY *)(GLubyte factor))gwglGetProcAddress("glGlobalAlphaFactorubSUN");
		gglGlobalAlphaFactorusSUN = (void (APIENTRY *)(GLushort factor))gwglGetProcAddress("glGlobalAlphaFactorusSUN");
		gglGlobalAlphaFactoruiSUN = (void (APIENTRY *)(GLuint factor))gwglGetProcAddress("glGlobalAlphaFactoruiSUN");

		if(OpenGL_ExtensionsEnumeration("GL_SUN_global_alpha", GGL_SUN_global_alpha, element) == GLENUMERATION_FALSE)
			return;
	}

	if(/*atof(version) >= 1.1 && */StringToken(extensions, "GL_SUN_triangle_list"))
	{
		gglReplacementCodeuiSUN = (void (APIENTRY *)(GLuint code))gwglGetProcAddress("glReplacementCodeuiSUN");
		gglReplacementCodeusSUN = (void (APIENTRY *)(GLushort code))gwglGetProcAddress("glReplacementCodeusSUN");
		gglReplacementCodeubSUN = (void (APIENTRY *)(GLubyte code))gwglGetProcAddress("glReplacementCodeubSUN");
		gglReplacementCodeuivSUN = (void (APIENTRY *)(const GLuint *code))gwglGetProcAddress("glReplacementCodeuivSUN");
		gglReplacementCodeusvSUN = (void (APIENTRY *)(const GLushort *code))gwglGetProcAddress("glReplacementCodeusvSUN");
		gglReplacementCodeubvSUN = (void (APIENTRY *)(const GLubyte *code))gwglGetProcAddress("glReplacementCodeubvSUN");
		gglReplacementCodePointerSUN = (void (APIENTRY *)(GLenum type, GLsizei stride, const GLvoid* *pointer))gwglGetProcAddress("glReplacementCodePointerSUN");

		if(OpenGL_ExtensionsEnumeration("GL_SUN_triangle_list", GGL_SUN_triangle_list, element) == GLENUMERATION_FALSE)
			return;
	}

	//GL_SUN_triangle_list is not required
	if(StringToken(extensions, "GL_SUN_vertex")/* && StringToken(extensions, "GL_SUN_triangle_list")*/)
	{
		gglColor4ubVertex2fSUN = (void (APIENTRY *)(GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y))gwglGetProcAddress("glColor4ubVertex2fSUN");
		gglColor4ubVertex2fvSUN = (void (APIENTRY *)(const GLubyte *c, const GLfloat *v))gwglGetProcAddress("glColor4ubVertex2fvSUN");
		gglColor4ubVertex3fSUN = (void (APIENTRY *)(GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y, GLfloat z))gwglGetProcAddress("glColor4ubVertex3fSUN");
		gglColor4ubVertex3fvSUN = (void (APIENTRY *)(const GLubyte *c, const GLfloat *v))gwglGetProcAddress("glColor4ubVertex3fvSUN");
		gglColor3fVertex3fSUN = (void (APIENTRY *)(GLfloat r, GLfloat g, GLfloat b, GLfloat x, GLfloat y, GLfloat z))gwglGetProcAddress("glColor3fVertex3fSUN");
		gglColor3fVertex3fvSUN = (void (APIENTRY *)(const GLfloat *c, const GLfloat *v))gwglGetProcAddress("glColor3fVertex3fvSUN");
		gglNormal3fVertex3fSUN = (void (APIENTRY *)(GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z))gwglGetProcAddress("glNormal3fVertex3fSUN");
		gglNormal3fVertex3fvSUN = (void (APIENTRY *)(const GLfloat *n, const GLfloat *v))gwglGetProcAddress("glNormal3fVertex3fvSUN");
		gglColor4fNormal3fVertex3fSUN = (void (APIENTRY *)(GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z))gwglGetProcAddress("glColor4fNormal3fVertex3fSUN");
		gglColor4fNormal3fVertex3fvSUN = (void (APIENTRY *)(const GLfloat *c, const GLfloat *n, const GLfloat *v))gwglGetProcAddress("glColor4fNormal3fVertex3fvSUN");
		gglTexCoord2fVertex3fSUN = (void (APIENTRY *)(GLfloat s, GLfloat t, GLfloat x, GLfloat y, GLfloat z))gwglGetProcAddress("glTexCoord2fVertex3fSUN");
		gglTexCoord2fVertex3fvSUN = (void (APIENTRY *)(const GLfloat *tc, const GLfloat *v))gwglGetProcAddress("glTexCoord2fVertex3fvSUN");
		gglTexCoord4fVertex4fSUN = (void (APIENTRY *)(GLfloat s, GLfloat t, GLfloat p, GLfloat q, GLfloat x, GLfloat y, GLfloat z, GLfloat w))gwglGetProcAddress("glTexCoord4fVertex4fSUN");
		gglTexCoord4fVertex4fvSUN = (void (APIENTRY *)(const GLfloat *tc, const GLfloat *v))gwglGetProcAddress("glTexCoord4fVertex4fvSUN");
		gglTexCoord2fColor4ubVertex3fSUN = (void (APIENTRY *)(GLfloat s, GLfloat t, GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y, GLfloat z))gwglGetProcAddress("glTexCoord2fColor4ubVertex3fSUN");
		gglTexCoord2fColor4ubVertex3fvSUN = (void (APIENTRY *)(const GLfloat *tc, const GLubyte *c, const GLfloat *v))gwglGetProcAddress("glTexCoord2fColor4ubVertex3fvSUN");
		gglTexCoord2fColor3fVertex3fSUN = (void (APIENTRY *)(GLfloat s, GLfloat t, GLfloat r, GLfloat g, GLfloat b, GLfloat x, GLfloat y, GLfloat z))gwglGetProcAddress("glTexCoord2fColor3fVertex3fSUN");
		gglTexCoord2fColor3fVertex3fvSUN = (void (APIENTRY *)(const GLfloat *tc, const GLfloat *c, const GLfloat *v))gwglGetProcAddress("glTexCoord2fColor3fVertex3fvSUN");
		gglTexCoord2fNormal3fVertex3fSUN = (void (APIENTRY *)(GLfloat s, GLfloat t, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z))gwglGetProcAddress("glTexCoord2fNormal3fVertex3fSUN");
		gglTexCoord2fNormal3fVertex3fvSUN = (void (APIENTRY *)(const GLfloat *tc, const GLfloat *n, const GLfloat *v))gwglGetProcAddress("glTexCoord2fNormal3fVertex3fvSUN");
		gglTexCoord2fColor4fNormal3fVertex3fSUN = (void (APIENTRY *)(GLfloat s, GLfloat t, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z))gwglGetProcAddress("glTexCoord2fColor4fNormal3fVertex3fSUN");
		gglTexCoord2fColor4fNormal3fVertex3fvSUN = (void (APIENTRY *)(const GLfloat *tc, const GLfloat *c, const GLfloat *n, const GLfloat *v))gwglGetProcAddress("glTexCoord2fColor4fNormal3fVertex3fvSUN");
		gglTexCoord4fColor4fNormal3fVertex4fSUN = (void (APIENTRY *)(GLfloat s, GLfloat t, GLfloat p, GLfloat q, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z, GLfloat w))gwglGetProcAddress("glTexCoord4fColor4fNormal3fVertex4fSUN");
		gglTexCoord4fColor4fNormal3fVertex4fvSUN = (void (APIENTRY *)(const GLfloat *tc, const GLfloat *c, const GLfloat *n, const GLfloat *v))gwglGetProcAddress("glTexCoord4fColor4fNormal3fVertex4fvSUN");
		gglReplacementCodeuiVertex3fSUN = (void (APIENTRY *)(GLuint rc, GLfloat x, GLfloat y, GLfloat z))gwglGetProcAddress("glReplacementCodeuiVertex3fSUN");
		gglReplacementCodeuiVertex3fvSUN = (void (APIENTRY *)(const GLuint *rc, const GLfloat *v))gwglGetProcAddress("glReplacementCodeuiVertex3fvSUN");
		gglReplacementCodeuiColor4ubVertex3fSUN = (void (APIENTRY *)(GLuint rc, GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y, GLfloat z))gwglGetProcAddress("glReplacementCodeuiColor4ubVertex3fSUN");
		gglReplacementCodeuiColor4ubVertex3fvSUN = (void (APIENTRY *)(const GLuint *rc, const GLubyte *c, const GLfloat *v))gwglGetProcAddress("glReplacementCodeuiColor4ubVertex3fvSUN");
		gglReplacementCodeuiColor3fVertex3fSUN = (void (APIENTRY *)(GLuint rc, GLfloat r, GLfloat g, GLfloat b, GLfloat x, GLfloat y, GLfloat z))gwglGetProcAddress("glReplacementCodeuiColor3fVertex3fSUN");
		gglReplacementCodeuiColor3fVertex3fvSUN = (void (APIENTRY *)(const GLuint *rc, const GLfloat *c, const GLfloat *v))gwglGetProcAddress("glReplacementCodeuiColor3fVertex3fvSUN");
		gglReplacementCodeuiNormal3fVertex3fSUN = (void (APIENTRY *)(GLuint rc, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z))gwglGetProcAddress("glReplacementCodeuiNormal3fVertex3fSUN");
		gglReplacementCodeuiNormal3fVertex3fvSUN = (void (APIENTRY *)(const GLuint *rc, const GLfloat *n, const GLfloat *v))gwglGetProcAddress("glReplacementCodeuiNormal3fVertex3fvSUN");
		gglReplacementCodeuiColor4fNormal3fVertex3fSUN = (void (APIENTRY *)(GLuint rc, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z))gwglGetProcAddress("glReplacementCodeuiColor4fNormal3fVertex3fSUN");
		gglReplacementCodeuiColor4fNormal3fVertex3fvSUN = (void (APIENTRY *)(const GLuint *rc, const GLfloat *c, const GLfloat *n, const GLfloat *v))gwglGetProcAddress("glReplacementCodeuiColor4fNormal3fVertex3fvSUN");
		gglReplacementCodeuiTexCoord2fVertex3fSUN = (void (APIENTRY *)(GLuint rc, GLfloat s, GLfloat t, GLfloat x, GLfloat y, GLfloat z))gwglGetProcAddress("glReplacementCodeuiTexCoord2fVertex3fSUN");
		gglReplacementCodeuiTexCoord2fVertex3fvSUN = (void (APIENTRY *)(const GLuint *rc, const GLfloat *tc, const GLfloat *v))gwglGetProcAddress("glReplacementCodeuiTexCoord2fVertex3fvSUN");
		gglReplacementCodeuiTexCoord2fNormal3fVertex3fSUN = (void (APIENTRY *)(GLuint rc, GLfloat s, GLfloat t, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z))gwglGetProcAddress("glReplacementCodeuiTexCoord2fNormal3fVertex3fSUN");
		gglReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN = (void (APIENTRY *)(const GLuint *rc, const GLfloat *tc, const GLfloat *n, const GLfloat *v))gwglGetProcAddress("glReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN");
		gglReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN = (void (APIENTRY *)(GLuint rc, GLfloat s, GLfloat t, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z))gwglGetProcAddress("glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN");
		gglReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN = (void (APIENTRY *)(const GLuint *rc, const GLfloat *tc, const GLfloat *c, const GLfloat *n, const GLfloat *v))gwglGetProcAddress("glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN");

		if(OpenGL_ExtensionsEnumeration("GL_SUN_vertex", GGL_SUN_vertex, element) == GLENUMERATION_FALSE)
			return;
	}

	//TODO: this is only for OpenGL 1.0 through 1.3
	if(StringToken(extensions, "GL_EXT_blend_func_separate"))
	{
		gglBlendFuncSeparateEXT = (void (APIENTRY *)(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha))gwglGetProcAddress("glBlendFuncSeparateEXT");

		if(OpenGL_ExtensionsEnumeration("GL_EXT_blend_func_separate", GGL_EXT_blend_func_separate, element) == GLENUMERATION_FALSE)
			return;
	}
	else if(StringToken(extensions, "GL_INGR_blend_func_separate"))
	{
		gglBlendFuncSeparateEXT = (void (APIENTRY *)(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha))gwglGetProcAddress("glBlendFuncSeparateINGR");

		if(OpenGL_ExtensionsEnumeration("GL_EXT_blend_func_separate", GGL_EXT_blend_func_separate, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_INGR_color_clamp"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_INGR_color_clamp", GGL_INGR_color_clamp, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_INGR_interlace_read"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_INGR_interlace_read", GGL_INGR_interlace_read, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_EXT_stencil_wrap"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_EXT_stencil_wrap", GGL_EXT_stencil_wrap, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_EXT_422_pixels"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_EXT_422_pixels", GGL_EXT_422_pixels, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_NV_texgen_reflection"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_NV_texgen_reflection", GGL_NV_texgen_reflection, element) == GLENUMERATION_FALSE)
			return;
	}

	//TODO: don't know requirements
	if(StringToken(extensions, "GL_EXT_texture_cube_map"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_EXT_texture_cube_map", GGL_EXT_texture_cube_map, element) == GLENUMERATION_FALSE)
			return;
	}

	//GL_SGIS_texture4D is not required
	if(/*atof(version) >= 1.2 && */StringToken(extensions, "GL_SGIX_texture_range") /* && StringToken(extensions, "GL_SGI_color_range")*//* && StringToken(extensions, "GL_SGIS_texture4D")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_SGIX_texture_range", GGL_SGIX_texture_range, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_SUN_convolution_border_modes") /* && StringToken(extensions, "GL_EXT_convolution")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_SUN_convolution_border_modes", GGL_SUN_convolution_border_modes, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_EXT_texture_env_add"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_EXT_texture_env_add", GGL_EXT_texture_env_add, element) == GLENUMERATION_FALSE)
			return;
	}

	//GL_ARB_multitexture is not required
	if(StringToken(extensions, "GL_EXT_texture_lod_bias")/* && StringToken(extensions, "GL_ARB_multitexture")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_EXT_texture_lod_bias", GGL_EXT_texture_lod_bias, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_EXT_texture_filter_anisotropic"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_EXT_texture_filter_anisotropic", GGL_EXT_texture_filter_anisotropic, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_EXT_vertex_weighting"))
	{
		gglVertexWeightfEXT = (void (APIENTRY *)(GLfloat weight))gwglGetProcAddress("glVertexWeightfEXT");
		gglVertexWeightfvEXT = (void (APIENTRY *)(const GLfloat *weight))gwglGetProcAddress("glVertexWeightfvEXT");
		gglVertexWeightPointerEXT = (void (APIENTRY *)(GLsizei size, GLenum type, GLsizei stride, const GLvoid *pointer))gwglGetProcAddress("glVertexWeightPointerEXT");

		if(OpenGL_ExtensionsEnumeration("GL_EXT_vertex_weighting", GGL_EXT_vertex_weighting, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_NV_light_max_exponent"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_NV_light_max_exponent", GGL_NV_light_max_exponent, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_NV_vertex_array_range"))
	{
		gglFlushVertexArrayRangeNV = (void (APIENTRY *)(void))gwglGetProcAddress("glFlushVertexArrayRangeNV");
		gglVertexArrayRangeNV = (void (APIENTRY *)(GLsizei length, const GLvoid *pointer))gwglGetProcAddress("glVertexArrayRangeNV");

		if(OpenGL_ExtensionsEnumeration("GL_NV_vertex_array_range", GGL_NV_vertex_array_range, element) == GLENUMERATION_FALSE)
			return;
	}

	//GL_ARB_multitexture has to do with this extension
	//GL_NV_texture_shader && GL_ARB_depth_texture && GL_ARB_shadow &&
	//GL_SGIX_depth_texture && GL_SGIX_shadow are not required
	if(StringToken(extensions, "GL_NV_register_combiners")/* && StringToken(extensions, "GL_NV_texture_shader") && ((StringToken(extensions, "GL_ARB_depth_texture") && StringToken(extensions, "GL_ARB_shadow")) || (StringToken(extensions, "GL_SGIX_depth_texture") && StringToken(extensions, "GL_SGIX_shadow")))*/)
	{
		gglCombinerParameterfvNV = (void (APIENTRY *)(GLenum pname, const GLfloat *params))gwglGetProcAddress("glCombinerParameterfvNV");
		gglCombinerParameterfNV = (void (APIENTRY *)(GLenum pname, GLfloat param))gwglGetProcAddress("glCombinerParameterfNV");
		gglCombinerParameterivNV = (void (APIENTRY *)(GLenum pname, const GLint *params))gwglGetProcAddress("glCombinerParameterivNV");
		gglCombinerParameteriNV = (void (APIENTRY *)(GLenum pname, GLint param))gwglGetProcAddress("glCombinerParameteriNV");
		gglCombinerInputNV = (void (APIENTRY *)(GLenum stage, GLenum portion, GLenum variable, GLenum input, GLenum mapping, GLenum componentUsage))gwglGetProcAddress("glCombinerInputNV");
		gglCombinerOutputNV = (void (APIENTRY *)(GLenum stage, GLenum portion, GLenum abOutput, GLenum cdOutput, GLenum sumOutput, GLenum scale, GLenum bias, GLboolean abDotProduct, GLboolean cdDotProduct, GLboolean muxSum))gwglGetProcAddress("glCombinerOutputNV");
		gglFinalCombinerInputNV = (void (APIENTRY *)(GLenum variable, GLenum input, GLenum mapping, GLenum componentUsage))gwglGetProcAddress("glFinalCombinerInputNV");
		gglGetCombinerInputParameterfvNV = (void (APIENTRY *)(GLenum stage, GLenum portion, GLenum variable, GLenum pname, GLfloat *params))gwglGetProcAddress("glGetCombinerInputParameterfvNV");
		gglGetCombinerInputParameterivNV = (void (APIENTRY *)(GLenum stage, GLenum portion, GLenum variable, GLenum pname, GLint *params))gwglGetProcAddress("glGetCombinerInputParameterivNV");
		gglGetCombinerOutputParameterfvNV = (void (APIENTRY *)(GLenum stage, GLenum portion, GLenum pname, GLfloat *params))gwglGetProcAddress("glGetCombinerOutputParameterfvNV");
		gglGetCombinerOutputParameterivNV = (void (APIENTRY *)(GLenum stage, GLenum portion, GLenum pname, GLint *params))gwglGetProcAddress("glGetCombinerOutputParameterivNV");
		gglGetFinalCombinerInputParameterfvNV = (void (APIENTRY *)(GLenum variable, GLenum pname, GLfloat *params))gwglGetProcAddress("glGetFinalCombinerInputParameterfvNV");
		gglGetFinalCombinerInputParameterivNV = (void (APIENTRY *)(GLenum variable, GLenum pname, GLint *params))gwglGetProcAddress("glGetFinalCombinerInputParameterivNV");

		if(OpenGL_ExtensionsEnumeration("GL_NV_register_combiners", GGL_NV_register_combiners, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_NV_fog_distance"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_NV_fog_distance", GGL_NV_fog_distance, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_NV_texgen_emboss") /* && StringToken(extensions, "GL_ARB_multitexture")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_NV_texgen_emboss", GGL_NV_texgen_emboss, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_NV_blend_square"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_NV_blend_square", GGL_NV_blend_square, element) == GLENUMERATION_FALSE)
			return;
	}

	//GL_ARB_multitexture is not required
	if(StringToken(extensions, "GL_NV_texture_env_combine4") /* && StringToken(extensions, "GL_EXT_texture_env_combine")*//* && StringToken(extensions, "GL_ARB_multitexture")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_NV_texture_env_combine4", GGL_NV_texture_env_combine4, element) == GLENUMERATION_FALSE)
			return;
	}

	//Mesa 2.2 or later is required
	if(StringToken(extensions, "GL_MESA_resize_buffers"))
	{
		gglResizeBuffersMESA = (void (APIENTRY *)(void))gwglGetProcAddress("glResizeBuffersMESA");

		if(OpenGL_ExtensionsEnumeration("GL_MESA_resize_buffers", GGL_MESA_resize_buffers, element) == GLENUMERATION_FALSE)
			return;
	}

	//TODO: glWindowPos2 and glWindowPos3 functions are only for OpenGL 1.0 through 1.3
	if(/*atof(version) >= 1.0 && */StringToken(extensions, "GL_MESA_window_pos"))
	{
		gglWindowPos2dMESA = (void (APIENTRY *)(GLdouble x, GLdouble y))gwglGetProcAddress("glWindowPos2dMESA");
		gglWindowPos2dvMESA = (void (APIENTRY *)(const GLdouble *v))gwglGetProcAddress("glWindowPos2dvMESA");
		gglWindowPos2fMESA = (void (APIENTRY *)(GLfloat x, GLfloat y))gwglGetProcAddress("glWindowPos2fMESA");
		gglWindowPos2fvMESA = (void (APIENTRY *)(const GLfloat *v))gwglGetProcAddress("glWindowPos2fvMESA");
		gglWindowPos2iMESA = (void (APIENTRY *)(GLint x, GLint y))gwglGetProcAddress("glWindowPos2iMESA");
		gglWindowPos2ivMESA = (void (APIENTRY *)(const GLint *v))gwglGetProcAddress("glWindowPos2ivMESA");
		gglWindowPos2sMESA = (void (APIENTRY *)(GLshort x, GLshort y))gwglGetProcAddress("glWindowPos2sMESA");
		gglWindowPos2svMESA = (void (APIENTRY *)(const GLshort *v))gwglGetProcAddress("glWindowPos2svMESA");
		gglWindowPos3dMESA = (void (APIENTRY *)(GLdouble x, GLdouble y, GLdouble z))gwglGetProcAddress("glWindowPos3dMESA");
		gglWindowPos3dvMESA = (void (APIENTRY *)(const GLdouble *v))gwglGetProcAddress("glWindowPos3dvMESA");
		gglWindowPos3fMESA = (void (APIENTRY *)(GLfloat x, GLfloat y, GLfloat z))gwglGetProcAddress("glWindowPos3fMESA");
		gglWindowPos3fvMESA = (void (APIENTRY *)(const GLfloat *v))gwglGetProcAddress("glWindowPos3fvMESA");
		gglWindowPos3iMESA = (void (APIENTRY *)(GLint x, GLint y, GLint z))gwglGetProcAddress("glWindowPos3iMESA");
		gglWindowPos3ivMESA = (void (APIENTRY *)(const GLint *v))gwglGetProcAddress("glWindowPos3ivMESA");
		gglWindowPos3sMESA = (void (APIENTRY *)(GLshort x, GLshort y, GLshort z))gwglGetProcAddress("glWindowPos3sMESA");
		gglWindowPos3svMESA = (void (APIENTRY *)(const GLshort *v))gwglGetProcAddress("glWindowPos3svMESA");
		gglWindowPos4dMESA = (void (APIENTRY *)(GLdouble x, GLdouble y, GLdouble z, GLdouble w))gwglGetProcAddress("glWindowPos4dMESA");
		gglWindowPos4dvMESA = (void (APIENTRY *)(const GLdouble *v))gwglGetProcAddress("glWindowPos4dvMESA");
		gglWindowPos4fMESA = (void (APIENTRY *)(GLfloat x, GLfloat y, GLfloat z, GLfloat w))gwglGetProcAddress("glWindowPos4fMESA");
		gglWindowPos4fvMESA = (void (APIENTRY *)(const GLfloat *v))gwglGetProcAddress("glWindowPos4fvMESA");
		gglWindowPos4iMESA = (void (APIENTRY *)(GLint x, GLint y, GLint z, GLint w))gwglGetProcAddress("glWindowPos4iMESA");
		gglWindowPos4ivMESA = (void (APIENTRY *)(const GLint *v))gwglGetProcAddress("glWindowPos4ivMESA");
		gglWindowPos4sMESA = (void (APIENTRY *)(GLshort x, GLshort y, GLshort z, GLshort w))gwglGetProcAddress("glWindowPos4sMESA");
		gglWindowPos4svMESA = (void (APIENTRY *)(const GLshort *v))gwglGetProcAddress("glWindowPos4svMESA");

		if(OpenGL_ExtensionsEnumeration("GL_MESA_window_pos", GGL_MESA_window_pos, element) == GLENUMERATION_FALSE)
			return;
	}

	if(/*atof(version) >= 1.1 && */StringToken(extensions, "GL_EXT_texture_compression_s3tc") /* && StringToken(extensions, "GL_ARB_texture_compression")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_EXT_texture_compression_s3tc", GGL_EXT_texture_compression_s3tc, element) == GLENUMERATION_FALSE)
			return;
	}

	//GL_EXT_cull_vertex is not required
	if(StringToken(extensions, "GL_IBM_cull_vertex")/* && StringToken(extensions, "GL_EXT_cull_vertex")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_IBM_cull_vertex", GGL_IBM_cull_vertex, element) == GLENUMERATION_FALSE)
			return;
	}

	if(/*atof(version) >= 1.1 && */StringToken(extensions, "GL_IBM_multimode_draw_arrays"))
	{
		gglMultiModeDrawArraysIBM = (void (APIENTRY *)(GLenum mode, const GLint *first, const GLsizei *count, GLsizei primcount, GLint modestride))gwglGetProcAddress("glMultiModeDrawArraysIBM");
		gglMultiModeDrawElementsIBM = (void (APIENTRY *)(const GLenum *mode, const GLsizei *count, GLenum type, const GLvoid* *indices, GLsizei primcount, GLint modestride))gwglGetProcAddress("glMultiModeDrawElementsIBM");

		if(OpenGL_ExtensionsEnumeration("GL_IBM_multimode_draw_arrays", GGL_IBM_multimode_draw_arrays, element) == GLENUMERATION_FALSE)
			return;
	}

	//GL_EXT_multi_draw_arrays && GL_IBM_multimode_draw_arrays are not required
	//supports GL_EXT_fog_coord && EXT_secondary_color (not required)
	if(/*atof(version) >= 1.1 && */StringToken(extensions, "GL_IBM_vertex_array_lists")/* && StringToken(extensions, "GL_EXT_multi_draw_arrays") && StringToken(extensions, "GL_IBM_multimode_draw_arrays") && StringToken(extensions, "GL_EXT_fog_coord") && StringToken(extensions, "GL_EXT_secondary_color")*/)
	{
		gglColorPointerListIBM = (void (APIENTRY *)(GLint size, GLenum type, GLint stride, const GLvoid* *pointer, GLint ptrstride))gwglGetProcAddress("glColorPointerListIBM");
		gglSecondaryColorPointerListIBM = (void (APIENTRY *)(GLint size, GLenum type, GLint stride, const GLvoid* *pointer, GLint ptrstride))gwglGetProcAddress("glSecondaryColorPointerListIBM");
		gglEdgeFlagPointerListIBM = (void (APIENTRY *)(GLint stride, const GLboolean* *pointer, GLint ptrstride))gwglGetProcAddress("glEdgeFlagPointerListIBM");
		gglFogCoordPointerListIBM = (void (APIENTRY *)(GLenum type, GLint stride, const GLvoid* *pointer, GLint ptrstride))gwglGetProcAddress("glFogCoordPointerListIBM");
		gglIndexPointerListIBM = (void (APIENTRY *)(GLenum type, GLint stride, const GLvoid* *pointer, GLint ptrstride))gwglGetProcAddress("glIndexPointerListIBM");
		gglNormalPointerListIBM = (void (APIENTRY *)(GLenum type, GLint stride, const GLvoid* *pointer, GLint ptrstride))gwglGetProcAddress("glNormalPointerListIBM");
		gglTexCoordPointerListIBM = (void (APIENTRY *)(GLint size, GLenum type, GLint stride, const GLvoid* *pointer, GLint ptrstride))gwglGetProcAddress("glTexCoordPointerListIBM");
		gglVertexPointerListIBM = (void (APIENTRY *)(GLint size, GLenum type, GLint stride, const GLvoid* *pointer, GLint ptrstride))gwglGetProcAddress("glVertexPointerListIBM");

		if(OpenGL_ExtensionsEnumeration("GL_IBM_vertex_array_lists", GGL_IBM_vertex_array_lists, element) == GLENUMERATION_FALSE)
			return;
	}

	//TODO: don't know requirements
	if(StringToken(extensions, "GL_SGIX_subsample"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_SGIX_subsample", GGL_SGIX_subsample, element) == GLENUMERATION_FALSE)
			return;
	}

	//TODO: don't know requirements
	if(StringToken(extensions, "GL_SGIX_ycrcba"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_SGIX_ycrcba", GGL_SGIX_ycrcba, element) == GLENUMERATION_FALSE)
			return;
	}

	//TODO: don't know requirements
	if(StringToken(extensions, "GL_SGIX_ycrcb_subsample"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_SGIX_ycrcb_subsample", GGL_SGIX_ycrcb_subsample, element) == GLENUMERATION_FALSE)
			return;
	}

	//TODO: don't know requirements
	if(StringToken(extensions, "GL_SGIX_depth_pass_instrument"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_SGIX_depth_pass_instrument", GGL_SGIX_depth_pass_instrument, element) == GLENUMERATION_FALSE)
			return;
	}

	if(/*atof(version) >= 1.1 && */StringToken(extensions, "GL_3DFX_texture_compression_FXT1") /* && StringToken(extensions, "GL_ARB_texture_compression")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_3DFX_texture_compression_FXT1", GGL_3DFX_texture_compression_FXT1, element) == GLENUMERATION_FALSE)
			return;
	}

	//GLX 1.2 is required
	//GLX_EXT_fbconfig && GLX 1.3 are not required
	if(/*atof(version) >= 1.1 && */StringToken(extensions, "GL_3DFX_multisample") /* && gwglGetExtensionsStringARB && StringToken(extensions, "WGL_ARB_pixel_format")*//* && StringToken(extensions, "GLX_EXT_fbconfig")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_3DFX_multisample", GGL_3DFX_multisample, element) == GLENUMERATION_FALSE)
			return;
	}

	if(/*atof(version) >= 1.1 && */StringToken(extensions, "GL_3DFX_tbuffer") /* && (StringToken(extensions, "GL_SGIS_multisample") || StringToken(extensions, "GL_3DFX_multisample"))*/)
	{
		gglTbufferMask3DFX = (void (APIENTRY *)(GLuint mask))gwglGetProcAddress("glTbufferMask3DFX");

			if(OpenGL_ExtensionsEnumeration("GL_3DFX_tbuffer", GGL_3DFX_tbuffer, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_EXT_multisample") /* && gwglGetExtensionsStringEXT && StringToken(wglextensions, "WGL_EXT_pixel_format")*/)
	{
		gglSampleMaskEXT = (void (APIENTRY *)(GLclampf value, GLboolean invert))gwglGetProcAddress("glSampleMaskEXT");
		gglSamplePatternEXT = (void (APIENTRY *)(GLenum pattern))gwglGetProcAddress("glSamplePatternEXT");

		if(OpenGL_ExtensionsEnumeration("GL_EXT_multisample", GGL_EXT_multisample, element) == GLENUMERATION_FALSE)
			return;
	}

	//XXX: not complete
	if(/*atof(version) >= 1.2 && */(StringToken(extensions, "GL_SGIX_vertex_preclip") || StringToken(extensions, "GL_SGIX_vertex_preclip_hint")))
	{
			if(OpenGL_ExtensionsEnumeration("GL_SGIX_vertex_preclip", GGL_SGIX_vertex_preclip, element) == GLENUMERATION_FALSE)
			return;
	}

	//TODO: don't know requirements
	if(StringToken(extensions, "GL_SGIX_convolution_accuracy"))
	{
			if(OpenGL_ExtensionsEnumeration("GL_SGIX_convolution_accuracy", GGL_SGIX_convolution_accuracy, element) == GLENUMERATION_FALSE)
			return;
	}

	//XXX: not complete
	//GL_SGIX_subsample && GL_EXT_texture3D && GL_EXT_subtexture &&
	//GL_EXT_histogram && GL_EXT_convolution && GL_SGI_color_table are not required
	if(StringToken(extensions, "GL_SGIX_resample")/* && StringToken(extensions, "GL_SGIX_subsample") && StringToken(extensions, "GL_EXT_texture3D") && StringToken(extensions, "GL_EXT_subtexture") && StringToken(extensions, "GL_EXT_histogram") && StringToken(extensions, "GL_EXT_convolution") && StringToken(extensions, "GL_SGI_color_table")*/)
	{
			if(OpenGL_ExtensionsEnumeration("GL_SGIX_resample", GGL_SGIX_resample, element) == GLENUMERATION_FALSE)
			return;
	}

	//TODO: don't know requirements
	if(StringToken(extensions, "GL_SGIS_point_line_texgen"))
	{
			if(OpenGL_ExtensionsEnumeration("GL_SGIS_point_line_texgen", GGL_SGIS_point_line_texgen, element) == GLENUMERATION_FALSE)
			return;
	}

	//ARB_texture_compression is required
	if(StringToken(extensions, "GL_SGIS_texture_color_mask")/* && StringToken(extensions, "ARB_texture_compression")*/)
	{
		gglTextureColorMaskSGIS = (void (APIENTRY *)(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha))gwglGetProcAddress("glTextureColorMaskSGIS");

		if(OpenGL_ExtensionsEnumeration("GL_SGIS_texture_color_mask", GGL_SGIS_texture_color_mask, element) == GLENUMERATION_FALSE)
			return;
	}

	//TODO: don't know requirements
	if(StringToken(extensions, "GL_SGIX_igloo_interface"))
	{
		gglIglooInterfaceSGIX = (void (APIENTRY *)(GLenum pname, const GLvoid *params))gwglGetProcAddress("glIglooInterfaceSGIX");

		if(OpenGL_ExtensionsEnumeration("GL_SGIX_igloo_interface", GGL_SGIX_igloo_interface, element) == GLENUMERATION_FALSE)
			return;
	}

	//GL_ARB_multitexture is not required
	if(StringToken(extensions, "GL_EXT_texture_env_dot3") /* && StringToken(extensions, "GL_EXT_texture_env_combine")*//* && StringToken(extensions, "GL_ARB_multitexture")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_EXT_texture_env_dot3", GGL_EXT_texture_env_dot3, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_ATI_texture_mirror_once") /* && StringToken(extensions, "GL_EXT_texture3D")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_ATI_texture_mirror_once", GGL_ATI_texture_mirror_once, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_NV_fence"))
	{
		gglDeleteFencesNV = (void (APIENTRY *)(GLsizei n, const GLuint *fences))gwglGetProcAddress("glDeleteFencesNV");
		gglGenFencesNV = (void (APIENTRY *)(GLsizei n, GLuint *fences))gwglGetProcAddress("glGenFencesNV");
		gglIsFenceNV = (GLboolean (APIENTRY *)(GLuint fence))gwglGetProcAddress("glIsFenceNV");
		gglTestFenceNV = (GLboolean (APIENTRY *)(GLuint fence))gwglGetProcAddress("glTestFenceNV");
		gglGetFenceivNV = (void (APIENTRY *)(GLuint fence, GLenum pname, GLint *params))gwglGetProcAddress("glGetFenceivNV");
		gglFinishFenceNV = (void (APIENTRY *)(GLuint fence))gwglGetProcAddress("glFinishFenceNV");
		gglSetFenceNV = (void (APIENTRY *)(GLuint fence, GLenum condition))gwglGetProcAddress("glSetFenceNV");

		if(OpenGL_ExtensionsEnumeration("GL_NV_fence", GGL_NV_fence, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_IBM_static_data"))
	{
		gglFlushStaticDataIBM = (void (APIENTRY *)(GLenum target))gwglGetProcAddress("glFlushStaticDataIBM");

		if(OpenGL_ExtensionsEnumeration("GL_IBM_static_data", GGL_IBM_static_data, element) == GLENUMERATION_FALSE)
			return;
	}

	//TODO: GL_EXT_texture_3D && GL_IBM_texture_edge_clamp are not necessarily required
	if(StringToken(extensions, "GL_IBM_texture_mirrored_repeat")/* && StringToken(extensions, "GL_EXT_texture_3D") && StringToken(extensions, "GL_IBM_texture_edge_clamp")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_IBM_texture_mirrored_repeat", GGL_IBM_texture_mirrored_repeat, element) == GLENUMERATION_FALSE)
			return;
	}

	//GL_NV_vertex_program is not required
	if(StringToken(extensions, "GL_NV_evaluators") /* && StringToken(extensions, "GL_ARB_multitexture")*//* && StringToken(extensions, "GL_NV_vertex_program")*/)
	{
		gglMapControlPointsNV = (void (APIENTRY *)(GLenum target, GLuint index, GLenum type, GLsizei ustride, GLsizei vstride, GLint uorder, GLint vorder, GLboolean packed, const GLvoid *points))gwglGetProcAddress("glMapControlPointsNV");
		gglMapParameterivNV = (void (APIENTRY *)(GLenum target, GLenum pname, const GLint *params))gwglGetProcAddress("glMapParameterivNV");
		gglMapParameterfvNV = (void (APIENTRY *)(GLenum target, GLenum pname, const GLfloat *params))gwglGetProcAddress("glMapParameterfvNV");
		gglGetMapControlPointsNV = (void (APIENTRY *)(GLenum target, GLuint index, GLenum type, GLsizei ustride, GLsizei vstride, GLboolean packed, GLvoid *points))gwglGetProcAddress("glGetMapControlPointsNV");
		gglGetMapParameterivNV = (void (APIENTRY *)(GLenum target, GLenum pname, GLint *params))gwglGetProcAddress("glGetMapParameterivNV");
		gglGetMapParameterfvNV = (void (APIENTRY *)(GLenum target, GLenum pname, GLfloat *params))gwglGetProcAddress("glGetMapParameterfvNV");
		gglGetMapAttribParameterivNV = (void (APIENTRY *)(GLenum target, GLuint index, GLenum pname, GLint *params))gwglGetProcAddress("glGetMapAttribParameterivNV");
		gglGetMapAttribParameterfvNV = (void (APIENTRY *)(GLenum target, GLuint index, GLenum pname, GLfloat *params))gwglGetProcAddress("glGetMapAttribParameterfvNV");
		gglEvalMapsNV = (void (APIENTRY *)(GLenum target, GLenum mode))gwglGetProcAddress("glEvalMapsNV");

		if(OpenGL_ExtensionsEnumeration("GL_NV_evaluators", GGL_NV_evaluators, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_NV_packed_depth_stencil")/* && StringToken(extensions, "GL_SGIX_depth_texture")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_NV_packed_depth_stencil", GGL_NV_packed_depth_stencil, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_NV_register_combiners2") /* && StringToken(extensions, "GL_NV_register_combiners")*/)
	{
		gglCombinerStageParameterfvNV = (void (APIENTRY *)(GLenum stage, GLenum pname, const GLfloat *params))gwglGetProcAddress("glCombinerStageParameterfvNV");
		gglGetCombinerStageParameterfvNV = (void (APIENTRY *)(GLenum stage, GLenum pname, GLfloat *params))gwglGetProcAddress("glGetCombinerStageParameterfvNV");

		if(OpenGL_ExtensionsEnumeration("GL_NV_register_combiners2", GGL_NV_register_combiners2, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_NV_texture_compression_vtc") /* && StringToken(extensions, "GL_ARB_texture_compression") && StringToken(extensions, "GL_EXT_texture_compression_s3tc")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_NV_texture_compression_vtc", GGL_NV_texture_compression_vtc, element) == GLENUMERATION_FALSE)
			return;
	}

	//GL_IBM_mirrored_repeat && GL_ARB_texture_border_clamp &&
	//GL_EXT_paletted_texture && GL_NV_texture_shader are not required
	if(StringToken(extensions, "GL_NV_texture_rectangle")/* && StringToken(extensions, "GL_IBM_mirrored_repeat") && StringToken(extensions, "GL_ARB_texture_border_clamp") && StringToken(extensions, "GL_EXT_paletted_texture") && StringToken(extensions, "GL_NV_texture_shader")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_NV_texture_rectangle", GGL_NV_texture_rectangle, element) == GLENUMERATION_FALSE)
			return;
	}

	//GL_NV_register_combiners && GL_EXT_texture_lod_bias &&
	//GL_ARB_texture_env_combine && GL_EXT_texture_env_combine &&
	//GL_NV_texture_env_combine4 && GL_ARB_texture_env_add && GL_EXT_texture_env_add &&
	//GL_NV_texture_rectangle && GL_NV_texture_shader2 are not required
	if(StringToken(extensions, "GL_NV_texture_shader") /* && StringToken(extensions, "GL_ARB_multitexture") && StringToken(extensions, "GL_ARB_texture_cube_map")*//* && StringToken(extensions, "GL_NV_register_combiners") && StringToken(extensions, "GL_EXT_texture_lod_bias") && StringToken(extensions, "GL_ARB_texture_env_combine") && StringToken(extensions, "GL_EXT_texture_env_combine") && StringToken(extensions, "GL_NV_texture_env_combine4") && StringToken(extensions, "GL_ARB_texture_env_add") && StringToken(extensions, "GL_EXT_texture_env_add") && StringToken(extensions, "GL_NV_texture_rectangle") && StringToken(extensions, "GL_NV_texture_shader2")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_NV_texture_shader", GGL_NV_texture_shader, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_NV_texture_shader2") /* && StringToken(extensions, "GL_NV_texture_shader")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_NV_texture_shader2", GGL_NV_texture_shader2, element) == GLENUMERATION_FALSE)
			return;
	}

	//GL_NV_fence is not required
	if(StringToken(extensions, "GL_NV_vertex_array_range2") /* && StringToken(extensions, "GL_NV_vertex_array_range")*//* && StringToken(extensions, "GL_NV_fence")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_NV_vertex_array_range2", GGL_NV_vertex_array_range2, element) == GLENUMERATION_FALSE)
			return;
	}

	//GL_EXT_point_parameters && GL_EXT_secondary_color && GL_EXT_fog_coord &&
	//GL_EXT_vertex_weighting && GL_ARB_imaging are not required
	//requires at least 2 texture units
	if(/*(!StringToken(version, "1.2.1")) && */StringToken(extensions, "GL_NV_vertex_program") /* && StringToken(extensions, "GL_ARB_multitexture")*//* && StringToken(extensions, "GL_EXT_point_parameters") && StringToken(extensions, "GL_EXT_secondary_color") && StringToken(extensions, "GL_EXT_fog_coord") && StringToken(extensions, "GL_EXT_vertex_weighting") && StringToken(extensions, "GL_ARB_imaging")*/)
	{
		gglAreProgramsResidentNV = (GLboolean (APIENTRY *)(GLsizei n, const GLuint *programs, GLboolean *residences))gwglGetProcAddress("glAreProgramsResidentNV");
		gglBindProgramNV = (void (APIENTRY *)(GLenum target, GLuint id))gwglGetProcAddress("glBindProgramNV");
		gglDeleteProgramsNV = (void (APIENTRY *)(GLsizei n, const GLuint *programs))gwglGetProcAddress("glDeleteProgramsNV");
		gglExecuteProgramNV = (void (APIENTRY *)(GLenum target, GLuint id, const GLfloat *params))gwglGetProcAddress("glExecuteProgramNV");
		gglGenProgramsNV = (void (APIENTRY *)(GLsizei n, GLuint *programs))gwglGetProcAddress("glGenProgramsNV");
		gglGetProgramParameterdvNV = (void (APIENTRY *)(GLenum target, GLuint index, GLenum pname, GLdouble *params))gwglGetProcAddress("glGetProgramParameterdvNV");
		gglGetProgramParameterfvNV = (void (APIENTRY *)(GLenum target, GLuint index, GLenum pname, GLfloat *params))gwglGetProcAddress("glGetProgramParameterfvNV");
		gglGetProgramivNV = (void (APIENTRY *)(GLuint id, GLenum pname, GLint *params))gwglGetProcAddress("glGetProgramivNV");
		gglGetProgramStringNV = (void (APIENTRY *)(GLuint id, GLenum pname, GLubyte *program))gwglGetProcAddress("glGetProgramStringNV");
		gglGetTrackMatrixivNV = (void (APIENTRY *)(GLenum target, GLuint address, GLenum pname, GLint *params))gwglGetProcAddress("glGetTrackMatrixivNV");
		gglGetVertexAttribdvNV = (void (APIENTRY *)(GLuint index, GLenum pname, GLdouble *params))gwglGetProcAddress("glGetVertexAttribdvNV");
		gglGetVertexAttribfvNV = (void (APIENTRY *)(GLuint index, GLenum pname, GLfloat *params))gwglGetProcAddress("glGetVertexAttribfvNV");
		gglGetVertexAttribivNV = (void (APIENTRY *)(GLuint index, GLenum pname, GLint *params))gwglGetProcAddress("glGetVertexAttribivNV");
		gglGetVertexAttribPointervNV = (void (APIENTRY *)(GLuint index, GLenum pname, GLvoid* *pointer))gwglGetProcAddress("glGetVertexAttribPointervNV");
		gglIsProgramNV = (GLboolean (APIENTRY *)(GLuint id))gwglGetProcAddress("glIsProgramNV");
		gglLoadProgramNV = (void (APIENTRY *)(GLenum target, GLuint id, GLsizei len, const GLubyte *program))gwglGetProcAddress("glLoadProgramNV");
		gglProgramParameter4dNV = (void (APIENTRY *)(GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w))gwglGetProcAddress("glProgramParameter4dNV");
		gglProgramParameter4dvNV = (void (APIENTRY *)(GLenum target, GLuint index, const GLdouble *v))gwglGetProcAddress("glProgramParameter4dvNV");
		gglProgramParameter4fNV = (void (APIENTRY *)(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w))gwglGetProcAddress("glProgramParameter4fNV");
		gglProgramParameter4fvNV = (void (APIENTRY *)(GLenum target, GLuint index, const GLfloat *v))gwglGetProcAddress("glProgramParameter4fvNV");
		gglProgramParameters4dvNV = (void (APIENTRY *)(GLenum target, GLuint index, GLuint count, const GLdouble *v))gwglGetProcAddress("glProgramParameters4dvNV");
		gglProgramParameters4fvNV = (void (APIENTRY *)(GLenum target, GLuint index, GLuint count, const GLfloat *v))gwglGetProcAddress("glProgramParameters4fvNV");
		gglRequestResidentProgramsNV = (void (APIENTRY *)(GLsizei n, const GLuint *programs))gwglGetProcAddress("glRequestResidentProgramsNV");
		gglTrackMatrixNV = (void (APIENTRY *)(GLenum target, GLuint address, GLenum matrix, GLenum transform))gwglGetProcAddress("glTrackMatrixNV");
		gglVertexAttribPointerNV = (void (APIENTRY *)(GLuint index, GLint fsize, GLenum type, GLsizei stride, const GLvoid *pointer))gwglGetProcAddress("glVertexAttribPointerNV");
		gglVertexAttrib1dNV = (void (APIENTRY *)(GLuint index, GLdouble x))gwglGetProcAddress("glVertexAttrib1dNV");
		gglVertexAttrib1dvNV = (void (APIENTRY *)(GLuint index, const GLdouble *v))gwglGetProcAddress("glVertexAttrib1dvNV");
		gglVertexAttrib1fNV = (void (APIENTRY *)(GLuint index, GLfloat x))gwglGetProcAddress("glVertexAttrib1fNV");
		gglVertexAttrib1fvNV = (void (APIENTRY *)(GLuint index, const GLfloat *v))gwglGetProcAddress("glVertexAttrib1fvNV");
		gglVertexAttrib1sNV = (void (APIENTRY *)(GLuint index, GLshort x))gwglGetProcAddress("glVertexAttrib1sNV");
		gglVertexAttrib1svNV = (void (APIENTRY *)(GLuint index, const GLshort *v))gwglGetProcAddress("glVertexAttrib1svNV");
		gglVertexAttrib2dNV = (void (APIENTRY *)(GLuint index, GLdouble x, GLdouble y))gwglGetProcAddress("glVertexAttrib2dNV");
		gglVertexAttrib2dvNV = (void (APIENTRY *)(GLuint index, const GLdouble *v))gwglGetProcAddress("glVertexAttrib2dvNV");
		gglVertexAttrib2fNV = (void (APIENTRY *)(GLuint index, GLfloat x, GLfloat y))gwglGetProcAddress("glVertexAttrib2fNV");
		gglVertexAttrib2fvNV = (void (APIENTRY *)(GLuint index, const GLfloat *v))gwglGetProcAddress("glVertexAttrib2fvNV");
		gglVertexAttrib2sNV = (void (APIENTRY *)(GLuint index, GLshort x, GLshort y))gwglGetProcAddress("glVertexAttrib2sNV");
		gglVertexAttrib2svNV = (void (APIENTRY *)(GLuint index, const GLshort *v))gwglGetProcAddress("glVertexAttrib2svNV");
		gglVertexAttrib3dNV = (void (APIENTRY *)(GLuint index, GLdouble x, GLdouble y, GLdouble z))gwglGetProcAddress("glVertexAttrib3dNV");
		gglVertexAttrib3dvNV = (void (APIENTRY *)(GLuint index, const GLdouble *v))gwglGetProcAddress("glVertexAttrib3dvNV");
		gglVertexAttrib3fNV = (void (APIENTRY *)(GLuint index, GLfloat x, GLfloat y, GLfloat z))gwglGetProcAddress("glVertexAttrib3fNV");
		gglVertexAttrib3fvNV = (void (APIENTRY *)(GLuint index, const GLfloat *v))gwglGetProcAddress("glVertexAttrib3fvNV");
		gglVertexAttrib3sNV = (void (APIENTRY *)(GLuint index, GLshort x, GLshort y, GLshort z))gwglGetProcAddress("glVertexAttrib3sNV");
		gglVertexAttrib3svNV = (void (APIENTRY *)(GLuint index, const GLshort *v))gwglGetProcAddress("glVertexAttrib3svNV");
		gglVertexAttrib4dNV = (void (APIENTRY *)(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w))gwglGetProcAddress("glVertexAttrib4dNV");
		gglVertexAttrib4dvNV = (void (APIENTRY *)(GLuint index, const GLdouble *v))gwglGetProcAddress("glVertexAttrib4dvNV");
		gglVertexAttrib4fNV = (void (APIENTRY *)(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w))gwglGetProcAddress("glVertexAttrib4fNV");
		gglVertexAttrib4fvNV = (void (APIENTRY *)(GLuint index, const GLfloat *v))gwglGetProcAddress("glVertexAttrib4fvNV");
		gglVertexAttrib4sNV = (void (APIENTRY *)(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w))gwglGetProcAddress("glVertexAttrib4sNV");
		gglVertexAttrib4svNV = (void (APIENTRY *)(GLuint index, const GLshort *v))gwglGetProcAddress("glVertexAttrib4svNV");
		gglVertexAttrib4ubNV = (void (APIENTRY *)(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w))gwglGetProcAddress("glVertexAttrib4ubNV");
		gglVertexAttrib4ubvNV = (void (APIENTRY *)(GLuint index, const GLubyte *v))gwglGetProcAddress("glVertexAttrib4ubvNV");
		gglVertexAttribs1dvNV = (void (APIENTRY *)(GLuint index, GLsizei count, const GLdouble *v))gwglGetProcAddress("glVertexAttribs1dvNV");
		gglVertexAttribs1fvNV = (void (APIENTRY *)(GLuint index, GLsizei count, const GLfloat *v))gwglGetProcAddress("glVertexAttribs1fvNV");
		gglVertexAttribs1svNV = (void (APIENTRY *)(GLuint index, GLsizei count, const GLshort *v))gwglGetProcAddress("glVertexAttribs1svNV");
		gglVertexAttribs2dvNV = (void (APIENTRY *)(GLuint index, GLsizei count, const GLdouble *v))gwglGetProcAddress("glVertexAttribs2dvNV");
		gglVertexAttribs2fvNV = (void (APIENTRY *)(GLuint index, GLsizei count, const GLfloat *v))gwglGetProcAddress("glVertexAttribs2fvNV");
		gglVertexAttribs2svNV = (void (APIENTRY *)(GLuint index, GLsizei count, const GLshort *v))gwglGetProcAddress("glVertexAttribs2svNV");
		gglVertexAttribs3dvNV = (void (APIENTRY *)(GLuint index, GLsizei count, const GLdouble *v))gwglGetProcAddress("glVertexAttribs3dvNV");
		gglVertexAttribs3fvNV = (void (APIENTRY *)(GLuint index, GLsizei count, const GLfloat *v))gwglGetProcAddress("glVertexAttribs3fvNV");
		gglVertexAttribs3svNV = (void (APIENTRY *)(GLuint index, GLsizei count, const GLshort *v))gwglGetProcAddress("glVertexAttribs3svNV");
		gglVertexAttribs4dvNV = (void (APIENTRY *)(GLuint index, GLsizei count, const GLdouble *v))gwglGetProcAddress("glVertexAttribs4dvNV");
		gglVertexAttribs4fvNV = (void (APIENTRY *)(GLuint index, GLsizei count, const GLfloat *v))gwglGetProcAddress("glVertexAttribs4fvNV");
		gglVertexAttribs4svNV = (void (APIENTRY *)(GLuint index, GLsizei count, const GLshort *v))gwglGetProcAddress("glVertexAttribs4svNV");
		gglVertexAttribs4ubvNV = (void (APIENTRY *)(GLuint index, GLsizei count, const GLubyte *v))gwglGetProcAddress("glVertexAttribs4ubvNV");

		if(OpenGL_ExtensionsEnumeration("GL_NV_vertex_program", GGL_NV_vertex_program, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_SGIX_texture_coordinate_clamp") /* && StringToken(extensions, "GL_EXT_texture3D")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_SGIX_texture_coordinate_clamp", GGL_SGIX_texture_coordinate_clamp, element) == GLENUMERATION_FALSE)
			return;
	}

	//TODO: don't know requirements
	if(StringToken(extensions, "GL_SGIX_scalebias_hint"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_SGIX_scalebias_hint", GGL_SGIX_scalebias_hint, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_OML_interlace"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_OML_interlace", GGL_OML_interlace, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_OML_subsample"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_OML_subsample", GGL_OML_subsample, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_OML_resample") /* && StringToken(extensions, "GL_OML_subsample")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_OML_resample", GGL_OML_resample, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_NV_copy_depth_to_color") /* && StringToken(extensions, "GL_NV_packed_depth_stencil")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_NV_copy_depth_to_color", GGL_NV_copy_depth_to_color, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_ATI_envmap_bumpmap") /* && StringToken(extensions, "GL_EXT_texture_env_combine") && StringToken(extensions, "GL_ARB_multitexture")*/)
	{
		gglTexBumpParameterivATI = (void (APIENTRY *)(GLenum pname, const GLint *param))gwglGetProcAddress("glTexBumpParameterivATI");
		gglTexBumpParameterfvATI = (void (APIENTRY *)(GLenum pname, const GLfloat *param))gwglGetProcAddress("glTexBumpParameterfvATI");
		gglGetTexBumpParameterivATI = (void (APIENTRY *)(GLenum pname, GLint *param))gwglGetProcAddress("glGetTexBumpParameterivATI");
		gglGetTexBumpParameterfvATI = (void (APIENTRY *)(GLenum pname, GLfloat *param))gwglGetProcAddress("glGetTexBumpParameterfvATI");

		if(OpenGL_ExtensionsEnumeration("GL_ATI_envmap_bumpmap", GGL_ATI_envmap_bumpmap, element) == GLENUMERATION_FALSE)
			return;
	}

	//GL_ARB_shadow interacts with this extension
	if(StringToken(extensions, "GL_ATI_fragment_shader") /* && StringToken(extensions, "GL_ARB_multitexture")*//* && StringToken(extensions, "GL_ARB_shadow")*/)
	{
		gglGenFragmentShadersATI = (GLuint (APIENTRY *)(GLuint range))gwglGetProcAddress("glGenFragmentShadersATI");
		gglBindFragmentShaderATI = (void (APIENTRY *)(GLuint id))gwglGetProcAddress("glBindFragmentShaderATI");
		gglDeleteFragmentShaderATI = (void (APIENTRY *)(GLuint id))gwglGetProcAddress("glDeleteFragmentShaderATI");
		gglBeginFragmentShaderATI = (void (APIENTRY *)(void))gwglGetProcAddress("glBeginFragmentShaderATI");
		gglEndFragmentShaderATI = (void (APIENTRY *)(void))gwglGetProcAddress("glEndFragmentShaderATI");
		gglPassTexCoordATI = (void (APIENTRY *)(GLuint dst, GLuint coord, GLenum swizzle))gwglGetProcAddress("glPassTexCoordATI");
		gglSampleMapATI = (void (APIENTRY *)(GLuint dst, GLuint interp, GLenum swizzle))gwglGetProcAddress("glSampleMapATI");
		gglColorFragmentOp1ATI = (void (APIENTRY *)(GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod))gwglGetProcAddress("glColorFragmentOp1ATI");
		gglColorFragmentOp2ATI = (void (APIENTRY *)(GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod))gwglGetProcAddress("glColorFragmentOp2ATI");
		gglColorFragmentOp3ATI = (void (APIENTRY *)(GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod, GLuint arg3, GLuint arg3Rep, GLuint arg3Mod))gwglGetProcAddress("glColorFragmentOp3ATI");
		gglAlphaFragmentOp1ATI = (void (APIENTRY *)(GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod))gwglGetProcAddress("glAlphaFragmentOp1ATI");
		gglAlphaFragmentOp2ATI = (void (APIENTRY *)(GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod))gwglGetProcAddress("glAlphaFragmentOp2ATI");
		gglAlphaFragmentOp3ATI = (void (APIENTRY *)(GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod, GLuint arg3, GLuint arg3Rep, GLuint arg3Mod))gwglGetProcAddress("glAlphaFragmentOp3ATI");
		gglSetFragmentShaderConstantATI = (void (APIENTRY *)(GLuint dst, const GLfloat *value))gwglGetProcAddress("glSetFragmentShaderConstantATI");

		if(OpenGL_ExtensionsEnumeration("GL_ATI_fragment_shader", GGL_ATI_fragment_shader, element) == GLENUMERATION_FALSE)
			return;
	}

	//GL_ARB_vertex_program && GL_EXT_vertex_shader are not required
	if(StringToken(extensions, "GL_ATI_pn_triangles") || StringToken(extensions, "GL_ATIX_pn_triangles")/* && StringToken(extensions, "GL_ARB_vertex_program") && StringToken(extensions, "GL_EXT_vertex_shader")*/)
	{
		gglPNTrianglesiATI = (void (APIENTRY *)(GLenum pname, GLint param))gwglGetProcAddress("glPNTrianglesiATI");
		gglPNTrianglesfATI = (void (APIENTRY *)(GLenum pname, GLfloat param))gwglGetProcAddress("glPNTrianglesfATI");

		if(OpenGL_ExtensionsEnumeration("GL_ATI_pn_triangles", GGL_ATI_pn_triangles, element) == GLENUMERATION_FALSE)
			return;
	}

	//GL_EXT_vertex_shader is not required
	if(/*atof(version) >= 1.1 && */StringToken(extensions, "GL_ATI_vertex_array_object")/* && StringToken(extensions, "GL_EXT_vertex_shader")*/)
	{
		gglNewObjectBufferATI = (GLuint (APIENTRY *)(GLsizei size, const GLvoid *pointer, GLenum usage))gwglGetProcAddress("glNewObjectBufferATI");
		gglIsObjectBufferATI = (GLboolean (APIENTRY *)(GLuint buffer))gwglGetProcAddress("glIsObjectBufferATI");
		gglUpdateObjectBufferATI = (void (APIENTRY *)(GLuint buffer, GLuint offset, GLsizei size, const GLvoid *pointer, GLenum preserve))gwglGetProcAddress("glUpdateObjectBufferATI");
		gglGetObjectBufferfvATI = (void (APIENTRY *)(GLuint buffer, GLenum pname, GLfloat *params))gwglGetProcAddress("glGetObjectBufferfvATI");
		gglGetObjectBufferivATI = (void (APIENTRY *)(GLuint buffer, GLenum pname, GLint *params))gwglGetProcAddress("glGetObjectBufferivATI");
		gglDeleteObjectBufferATI = (void (APIENTRY *)(GLuint buffer))gwglGetProcAddress("glDeleteObjectBufferATI");
		gglArrayObjectATI = (void (APIENTRY *)(GLenum array, GLint size, GLenum type, GLsizei stride, GLuint buffer, GLuint offset))gwglGetProcAddress("glArrayObjectATI");
		gglGetArrayObjectfvATI = (void (APIENTRY *)(GLenum array, GLenum pname, GLfloat *params))gwglGetProcAddress("glGetArrayObjectfvATI");
		gglGetArrayObjectivATI = (void (APIENTRY *)(GLenum array, GLenum pname, GLint *params))gwglGetProcAddress("glGetArrayObjectivATI");
		gglVariantArrayObjectATI = (void (APIENTRY *)(GLuint id, GLenum type, GLsizei stride, GLuint buffer, GLuint offset))gwglGetProcAddress("glVariantArrayObjectATI");
		gglGetVariantArrayObjectfvATI = (void (APIENTRY *)(GLuint id, GLenum pname, GLfloat *params))gwglGetProcAddress("glGetVariantArrayObjectfvATI");
		gglGetVariantArrayObjectivATI = (void (APIENTRY *)(GLuint id, GLenum pname, GLint *params))gwglGetProcAddress("glGetVariantArrayObjectivATI");

		if(OpenGL_ExtensionsEnumeration("GL_ATI_vertex_array_object", GGL_ATI_vertex_array_object, element) == GLENUMERATION_FALSE)
			return;
	}

	//GL_ARB_imaging is not required
	if(StringToken(extensions, "GL_EXT_vertex_shader")/* && StringToken(extensions, "GL_ARB_imaging")*/)
	{
		gglBeginVertexShaderEXT = (void (APIENTRY *)(void))gwglGetProcAddress("glBeginVertexShaderEXT");
		gglEndVertexShaderEXT = (void (APIENTRY *)(void))gwglGetProcAddress("glEndVertexShaderEXT");
		gglBindVertexShaderEXT = (void (APIENTRY *)(GLuint id))gwglGetProcAddress("glBindVertexShaderEXT");
		gglGenVertexShadersEXT = (GLuint (APIENTRY *)(GLuint range))gwglGetProcAddress("glGenVertexShadersEXT");
		gglDeleteVertexShaderEXT = (void (APIENTRY *)(GLuint id))gwglGetProcAddress("glDeleteVertexShaderEXT");
		gglShaderOp1EXT = (void (APIENTRY *)(GLenum op, GLuint res, GLuint arg1))gwglGetProcAddress("glShaderOp1EXT");
		gglShaderOp2EXT = (void (APIENTRY *)(GLenum op, GLuint res, GLuint arg1, GLuint arg2))gwglGetProcAddress("glShaderOp2EXT");
		gglShaderOp3EXT = (void (APIENTRY *)(GLenum op, GLuint res, GLuint arg1, GLuint arg2, GLuint arg3))gwglGetProcAddress("glShaderOp3EXT");
		gglSwizzleEXT = (void (APIENTRY *)(GLuint res, GLuint in, GLenum outX, GLenum outY, GLenum outZ, GLenum outW))gwglGetProcAddress("glSwizzleEXT");
		gglWriteMaskEXT = (void (APIENTRY *)(GLuint res, GLuint in, GLenum outX, GLenum outY, GLenum outZ, GLenum outW))gwglGetProcAddress("glWriteMaskEXT");
		gglInsertComponentEXT = (void (APIENTRY *)(GLuint res, GLuint src, GLuint num))gwglGetProcAddress("glInsertComponentEXT");
		gglExtractComponentEXT = (void (APIENTRY *)(GLuint res, GLuint src, GLuint num))gwglGetProcAddress("glExtractComponentEXT");
		gglGenSymbolsEXT = (GLuint (APIENTRY *)(GLenum datatype, GLenum storagetype, GLenum range, GLuint components))gwglGetProcAddress("glGenSymbolsEXT");
		gglSetInvariantEXT = (void (APIENTRY *)(GLuint id, GLenum type, const void *addr))gwglGetProcAddress("glSetInvariantEXT");
		gglSetLocalConstantEXT = (void (APIENTRY *)(GLuint id, GLenum type, const void *addr))gwglGetProcAddress("glSetLocalConstantEXT");
		gglVariantbvEXT = (void (APIENTRY *)(GLuint id, const GLbyte *addr))gwglGetProcAddress("glVariantbvEXT");
		gglVariantsvEXT = (void (APIENTRY *)(GLuint id, const GLshort *addr))gwglGetProcAddress("glVariantsvEXT");
		gglVariantivEXT = (void (APIENTRY *)(GLuint id, const GLint *addr))gwglGetProcAddress("glVariantivEXT");
		gglVariantfvEXT = (void (APIENTRY *)(GLuint id, const GLfloat *addr))gwglGetProcAddress("glVariantfvEXT");
		gglVariantdvEXT = (void (APIENTRY *)(GLuint id, const GLdouble *addr))gwglGetProcAddress("glVariantdvEXT");
		gglVariantubvEXT = (void (APIENTRY *)(GLuint id, const GLubyte *addr))gwglGetProcAddress("glVariantubvEXT");
		gglVariantusvEXT = (void (APIENTRY *)(GLuint id, const GLushort *addr))gwglGetProcAddress("glVariantusvEXT");
		gglVariantuivEXT = (void (APIENTRY *)(GLuint id, const GLuint *addr))gwglGetProcAddress("glVariantuivEXT");
		gglVariantPointerEXT = (void (APIENTRY *)(GLuint id, GLenum type, GLuint stride, const void *addr))gwglGetProcAddress("glVariantPointerEXT");
		gglEnableVariantClientStateEXT = (void (APIENTRY *)(GLuint id))gwglGetProcAddress("glEnableVariantClientStateEXT");
		gglDisableVariantClientStateEXT = (void (APIENTRY *)(GLuint id))gwglGetProcAddress("glDisableVariantClientStateEXT");
		gglBindLightParameterEXT = (GLuint (APIENTRY *)(GLenum light, GLenum value))gwglGetProcAddress("glBindLightParameterEXT");
		gglBindMaterialParameterEXT = (GLuint (APIENTRY *)(GLenum face, GLenum value))gwglGetProcAddress("glBindMaterialParameterEXT");
		gglBindTexGenParameterEXT = (GLuint (APIENTRY *)(GLenum unit, GLenum coord, GLenum value))gwglGetProcAddress("glBindTexGenParameterEXT");
		gglBindTextureUnitParameterEXT = (GLuint (APIENTRY *)(GLenum unit, GLenum value))gwglGetProcAddress("glBindTextureUnitParameterEXT");
		gglBindParameterEXT = (GLuint (APIENTRY *)(GLenum value))gwglGetProcAddress("glBindParameterEXT");
		gglIsVariantEnabledEXT = (GLboolean (APIENTRY *)(GLuint id, GLenum cap))gwglGetProcAddress("glIsVariantEnabledEXT");
		gglGetVariantBooleanvEXT = (void (APIENTRY *)(GLuint id, GLenum value, GLboolean *data))gwglGetProcAddress("glGetVariantBooleanvEXT");
		gglGetVariantIntegervEXT = (void (APIENTRY *)(GLuint id, GLenum value, GLint *data))gwglGetProcAddress("glGetVariantIntegervEXT");
		gglGetVariantFloatvEXT = (void (APIENTRY *)(GLuint id, GLenum value, GLfloat *data))gwglGetProcAddress("glGetVariantFloatvEXT");
		gglGetVariantPointervEXT = (void (APIENTRY *)(GLuint id, GLenum value, GLvoid* *data))gwglGetProcAddress("glGetVariantPointervEXT");
		gglGetInvariantBooleanvEXT = (void (APIENTRY *)(GLuint id, GLenum value, GLboolean *data))gwglGetProcAddress("glGetInvariantBooleanvEXT");
		gglGetInvariantIntegervEXT = (void (APIENTRY *)(GLuint id, GLenum value, GLint *data))gwglGetProcAddress("glGetInvariantIntegervEXT");
		gglGetInvariantFloatvEXT = (void (APIENTRY *)(GLuint id, GLenum value, GLfloat *data))gwglGetProcAddress("glGetInvariantFloatvEXT");
		gglGetLocalConstantBooleanvEXT = (void (APIENTRY *)(GLuint id, GLenum value, GLboolean *data))gwglGetProcAddress("glGetLocalConstantBooleanvEXT");
		gglGetLocalConstantIntegervEXT = (void (APIENTRY *)(GLuint id, GLenum value, GLint *data))gwglGetProcAddress("glGetLocalConstantIntegervEXT");
		gglGetLocalConstantFloatvEXT = (void (APIENTRY *)(GLuint id, GLenum value, GLfloat *data))gwglGetProcAddress("glGetLocalConstantFloatvEXT");

		if(OpenGL_ExtensionsEnumeration("GL_EXT_vertex_shader", GGL_EXT_vertex_shader, element) == GLENUMERATION_FALSE)
			return;
	}

	if(/*atof(version) >= 1.0 && */StringToken(extensions, "GL_ATI_vertex_streams") /* && StringToken(extensions, "GL_ARB_vertex_blend")*/)
	{
		gglVertexStream1sATI = (void (APIENTRY *)(GLenum stream, GLshort x))gwglGetProcAddress("glVertexStream1sATI");
		gglVertexStream1svATI = (void (APIENTRY *)(GLenum stream, const GLshort *coords))gwglGetProcAddress("glVertexStream1svATI");
		gglVertexStream1iATI = (void (APIENTRY *)(GLenum stream, GLint x))gwglGetProcAddress("glVertexStream1iATI");
		gglVertexStream1ivATI = (void (APIENTRY *)(GLenum stream, const GLint *coords))gwglGetProcAddress("glVertexStream1ivATI");
		gglVertexStream1fATI = (void (APIENTRY *)(GLenum stream, GLfloat x))gwglGetProcAddress("glVertexStream1fATI");
		gglVertexStream1fvATI = (void (APIENTRY *)(GLenum stream, const GLfloat *coords))gwglGetProcAddress("glVertexStream1fvATI");
		gglVertexStream1dATI = (void (APIENTRY *)(GLenum stream, GLdouble x))gwglGetProcAddress("glVertexStream1dATI");
		gglVertexStream1dvATI = (void (APIENTRY *)(GLenum stream, const GLdouble *coords))gwglGetProcAddress("glVertexStream1dvATI");
		gglVertexStream2sATI = (void (APIENTRY *)(GLenum stream, GLshort x, GLshort y))gwglGetProcAddress("glVertexStream2sATI");
		gglVertexStream2svATI = (void (APIENTRY *)(GLenum stream, const GLshort *coords))gwglGetProcAddress("glVertexStream2svATI");
		gglVertexStream2iATI = (void (APIENTRY *)(GLenum stream, GLint x, GLint y))gwglGetProcAddress("glVertexStream2iATI");
		gglVertexStream2ivATI = (void (APIENTRY *)(GLenum stream, const GLint *coords))gwglGetProcAddress("glVertexStream2ivATI");
		gglVertexStream2fATI = (void (APIENTRY *)(GLenum stream, GLfloat x, GLfloat y))gwglGetProcAddress("glVertexStream2fATI");
		gglVertexStream2fvATI = (void (APIENTRY *)(GLenum stream, const GLfloat *coords))gwglGetProcAddress("glVertexStream2fvATI");
		gglVertexStream2dATI = (void (APIENTRY *)(GLenum stream, GLdouble x, GLdouble y))gwglGetProcAddress("glVertexStream2dATI");
		gglVertexStream2dvATI = (void (APIENTRY *)(GLenum stream, const GLdouble *coords))gwglGetProcAddress("glVertexStream2dvATI");
		gglVertexStream3sATI = (void (APIENTRY *)(GLenum stream, GLshort x, GLshort y, GLshort z))gwglGetProcAddress("glVertexStream3sATI");
		gglVertexStream3svATI = (void (APIENTRY *)(GLenum stream, const GLshort *coords))gwglGetProcAddress("glVertexStream3svATI");
		gglVertexStream3iATI = (void (APIENTRY *)(GLenum stream, GLint x, GLint y, GLint z))gwglGetProcAddress("glVertexStream3iATI");
		gglVertexStream3ivATI = (void (APIENTRY *)(GLenum stream, const GLint *coords))gwglGetProcAddress("glVertexStream3ivATI");
		gglVertexStream3fATI = (void (APIENTRY *)(GLenum stream, GLfloat x, GLfloat y, GLfloat z))gwglGetProcAddress("glVertexStream3fATI");
		gglVertexStream3fvATI = (void (APIENTRY *)(GLenum stream, const GLfloat *coords))gwglGetProcAddress("glVertexStream3fvATI");
		gglVertexStream3dATI = (void (APIENTRY *)(GLenum stream, GLdouble x, GLdouble y, GLdouble z))gwglGetProcAddress("glVertexStream3dATI");
		gglVertexStream3dvATI = (void (APIENTRY *)(GLenum stream, const GLdouble *coords))gwglGetProcAddress("glVertexStream3dvATI");
		gglVertexStream4sATI = (void (APIENTRY *)(GLenum stream, GLshort x, GLshort y, GLshort z, GLshort w))gwglGetProcAddress("glVertexStream4sATI");
		gglVertexStream4svATI = (void (APIENTRY *)(GLenum stream, const GLshort *coords))gwglGetProcAddress("glVertexStream4svATI");
		gglVertexStream4iATI = (void (APIENTRY *)(GLenum stream, GLint x, GLint y, GLint z, GLint w))gwglGetProcAddress("glVertexStream4iATI");
		gglVertexStream4ivATI = (void (APIENTRY *)(GLenum stream, const GLint *coords))gwglGetProcAddress("glVertexStream4ivATI");
		gglVertexStream4fATI = (void (APIENTRY *)(GLenum stream, GLfloat x, GLfloat y, GLfloat z, GLfloat w))gwglGetProcAddress("glVertexStream4fATI");
		gglVertexStream4fvATI = (void (APIENTRY *)(GLenum stream, const GLfloat *coords))gwglGetProcAddress("glVertexStream4fvATI");
		gglVertexStream4dATI = (void (APIENTRY *)(GLenum stream, GLdouble x, GLdouble y, GLdouble z, GLdouble w))gwglGetProcAddress("glVertexStream4dATI");
		gglVertexStream4dvATI = (void (APIENTRY *)(GLenum stream, const GLdouble *coords))gwglGetProcAddress("glVertexStream4dvATI");
		gglNormalStream3bATI = (void (APIENTRY *)(GLenum stream, GLbyte nx, GLbyte ny, GLbyte nz))gwglGetProcAddress("glNormalStream3bATI");
		gglNormalStream3bvATI = (void (APIENTRY *)(GLenum stream, const GLbyte *coords))gwglGetProcAddress("glNormalStream3bvATI");
		gglNormalStream3sATI = (void (APIENTRY *)(GLenum stream, GLshort nx, GLshort ny, GLshort nz))gwglGetProcAddress("glNormalStream3sATI");
		gglNormalStream3svATI = (void (APIENTRY *)(GLenum stream, const GLshort *coords))gwglGetProcAddress("glNormalStream3svATI");
		gglNormalStream3iATI = (void (APIENTRY *)(GLenum stream, GLint nx, GLint ny, GLint nz))gwglGetProcAddress("glNormalStream3iATI");
		gglNormalStream3ivATI = (void (APIENTRY *)(GLenum stream, const GLint *coords))gwglGetProcAddress("glNormalStream3ivATI");
		gglNormalStream3fATI = (void (APIENTRY *)(GLenum stream, GLfloat nx, GLfloat ny, GLfloat nz))gwglGetProcAddress("glNormalStream3fATI");
		gglNormalStream3fvATI = (void (APIENTRY *)(GLenum stream, const GLfloat *coords))gwglGetProcAddress("glNormalStream3fvATI");
		gglNormalStream3dATI = (void (APIENTRY *)(GLenum stream, GLdouble nx, GLdouble ny, GLdouble nz))gwglGetProcAddress("glNormalStream3dATI");
		gglNormalStream3dvATI = (void (APIENTRY *)(GLenum stream, const GLdouble *coords))gwglGetProcAddress("glNormalStream3dvATI");
		gglClientActiveVertexStreamATI = (void (APIENTRY *)(GLenum stream))gwglGetProcAddress("glClientActiveVertexStreamATI");
		gglVertexBlendEnviATI = (void (APIENTRY *)(GLenum pname, GLint param))gwglGetProcAddress("glVertexBlendEnviATI");
		gglVertexBlendEnvfATI = (void (APIENTRY *)(GLenum pname, GLfloat param))gwglGetProcAddress("glVertexBlendEnvfATI");

		if(OpenGL_ExtensionsEnumeration("GL_ATI_vertex_streams", GGL_ATI_vertex_streams, element) == GLENUMERATION_FALSE)
			return;
	}

	//GL_ATI_vertex_array_object is not required
	if(/*atof(version) >= 1.1 && */StringToken(extensions, "GL_ATI_element_array")/* && StringToken(extensions, "GL_ATI_vertex_array_object")*/)
	{
		gglElementPointerATI = (void (APIENTRY *)(GLenum type, const GLvoid *pointer))gwglGetProcAddress("glElementPointerATI");
		gglDrawElementArrayATI = (void (APIENTRY *)(GLenum mode, GLsizei count))gwglGetProcAddress("glDrawElementArrayATI");
		gglDrawRangeElementArrayATI = (void (APIENTRY *)(GLenum mode, GLuint start, GLuint end, GLsizei count))gwglGetProcAddress("glDrawRangeElementArrayATI");

		if(OpenGL_ExtensionsEnumeration("GL_ATI_element_array", GGL_ATI_element_array, element) == GLENUMERATION_FALSE)
			return;
	}

	if(/*atof(version) >= 1.1 && */StringToken(extensions, "GL_SUN_mesh_array"))
	{
		gglDrawMeshArraysSUN = (void (APIENTRY *)(GLenum mode, GLint first, GLsizei count, GLsizei width))gwglGetProcAddress("glDrawMeshArraysSUN");

		if(OpenGL_ExtensionsEnumeration("GL_SUN_mesh_array", GGL_SUN_mesh_array, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_SUN_slice_accum"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_SUN_slice_accum", GGL_SUN_slice_accum, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_NV_multisample_filter_hint") /* && StringToken(extensions, "GL_ARB_multisample")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_NV_multisample_filter_hint", GGL_NV_multisample_filter_hint, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_NV_depth_clamp"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_NV_depth_clamp", GGL_NV_depth_clamp, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_NV_occlusion_query") /* && StringToken(extensions, "GL_HP_occlusion_test")*/)
	{
		gglGenOcclusionQueriesNV = (void (APIENTRY *)(GLsizei n, GLuint *ids))gwglGetProcAddress("glGenOcclusionQueriesNV");
		gglDeleteOcclusionQueriesNV = (void (APIENTRY *)(GLsizei n, const GLuint *ids))gwglGetProcAddress("glDeleteOcclusionQueriesNV");
		gglIsOcclusionQueryNV = (GLboolean (APIENTRY *)(GLuint id))gwglGetProcAddress("glIsOcclusionQueryNV");
		gglBeginOcclusionQueryNV = (void (APIENTRY *)(GLuint id))gwglGetProcAddress("glBeginOcclusionQueryNV");
		gglEndOcclusionQueryNV = (void (APIENTRY *)(void))gwglGetProcAddress("glEndOcclusionQueryNV");
		gglGetOcclusionQueryivNV = (void (APIENTRY *)(GLuint id, GLenum pname, GLint *params))gwglGetProcAddress("glGetOcclusionQueryivNV");
		gglGetOcclusionQueryuivNV = (void (APIENTRY *)(GLuint id, GLenum pname, GLuint *params))gwglGetProcAddress("glGetOcclusionQueryuivNV");

		if(OpenGL_ExtensionsEnumeration("GL_NV_occlusion_query", GGL_NV_occlusion_query, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_NV_point_sprite") /* && StringToken(extensions, "GL_EXT_point_parameters")*/)
	{
		gglPointParameteriNV = (void (APIENTRY *)(GLenum pname, GLint param))gwglGetProcAddress("glPointParameteriNV");
		gglPointParameterivNV = (void (APIENTRY *)(GLenum pname, const GLint *params))gwglGetProcAddress("glPointParameterivNV");

		if(OpenGL_ExtensionsEnumeration("GL_NV_point_sprite", GGL_NV_point_sprite, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_NV_texture_shader3") /* && StringToken(extensions, "GL_NV_texture_shader") && StringToken(extensions, "GL_NV_texture_shader2")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_NV_texture_shader3", GGL_NV_texture_shader3, element) == GLENUMERATION_FALSE)
			return;
	}

	if(/*(!StringToken(version, "1.2.1")) && */StringToken(extensions, "GL_NV_vertex_program1_1") /* && StringToken(extensions, "GL_NV_vertex_program")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_NV_vertex_program1_1", GGL_NV_vertex_program1_1, element) == GLENUMERATION_FALSE)
			return;
	}

	if(/*atof(version) >= 1.1 && */StringToken(extensions, "GL_EXT_shadow_funcs") /* && StringToken(extensions, "GL_ARB_depth_texture") && StringToken(extensions, "GL_ARB_shadow")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_EXT_shadow_funcs", GGL_EXT_shadow_funcs, element) == GLENUMERATION_FALSE)
			return;
	}

	//GL_NV_packed_depth_stencil is not required
	if(StringToken(extensions, "GL_EXT_stencil_two_side")/* && StringToken(extensions, "GL_NV_packed_depth_stencil")*/)
	{
		gglActiveStencilFaceEXT = (void (APIENTRY *)(GLenum face))gwglGetProcAddress("glActiveStencilFaceEXT");

		if(OpenGL_ExtensionsEnumeration("GL_EXT_stencil_two_side", GGL_EXT_stencil_two_side, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_ATI_text_fragment_shader") /* && StringToken(extensions, "GL_ARB_multitexture") && StringToken(extensions, "GL_ARB_shadow") && StringToken(extensions, "GL_ARB_vertex_program") && StringToken(extensions, "GL_ATI_fragment_shader")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_ATI_text_fragment_shader", GGL_ATI_text_fragment_shader, element) == GLENUMERATION_FALSE)
			return;
	}

	if(/*atof(version) >= 1.1 && */StringToken(extensions, "GL_APPLE_client_storage"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_APPLE_client_storage", GGL_APPLE_client_storage, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_APPLE_element_array") /* && StringToken(extensions, "GL_APPLE_vertex_array_range")*/)
	{
		gglElementPointerAPPLE = (void (APIENTRY *)(GLenum type, const GLvoid *pointer))gwglGetProcAddress("glElementPointerAPPLE");
		gglDrawElementArrayAPPLE = (void (APIENTRY *)(GLenum mode, GLint first, GLsizei count))gwglGetProcAddress("glDrawElementArrayAPPLE");
		gglDrawRangeElementArrayAPPLE = (void (APIENTRY *)(GLenum mode, GLuint start, GLuint end, GLint first, GLsizei count))gwglGetProcAddress("glDrawRangeElementArrayAPPLE");
		gglMultiDrawElementArrayAPPLE = (void (APIENTRY *)(GLenum mode, const GLint *first, const GLsizei *count, GLsizei primcount))gwglGetProcAddress("glMultiDrawElementArrayAPPLE");
		gglMultiDrawRangeElementArrayAPPLE = (void (APIENTRY *)(GLenum mode, GLuint start, GLuint end, const GLint *first, const GLsizei *count, GLsizei primcount))gwglGetProcAddress("glMultiDrawRangeElementArrayAPPLE");

		if(OpenGL_ExtensionsEnumeration("GL_APPLE_element_array", GGL_APPLE_element_array, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_APPLE_fence"))
	{
		gglGenFencesAPPLE = (void (APIENTRY *)(GLsizei n, GLuint *fences))gwglGetProcAddress("glGenFencesAPPLE");
		gglDeleteFencesAPPLE = (void (APIENTRY *)(GLsizei n, const GLuint *fences))gwglGetProcAddress("glDeleteFencesAPPLE");
		gglSetFenceAPPLE = (void (APIENTRY *)(GLuint fence))gwglGetProcAddress("glSetFenceAPPLE");
		gglIsFenceAPPLE = (GLboolean (APIENTRY *)(GLuint fence))gwglGetProcAddress("glIsFenceAPPLE");
		gglTestFenceAPPLE = (GLboolean (APIENTRY *)(GLuint fence))gwglGetProcAddress("glTestFenceAPPLE");
		gglFinishFenceAPPLE = (void (APIENTRY *)(GLuint fence))gwglGetProcAddress("glFinishFenceAPPLE");
		gglTestObjectAPPLE = (GLboolean (APIENTRY *)(GLenum object, GLuint name))gwglGetProcAddress("glTestObjectAPPLE");
		gglFinishObjectAPPLE = (void (APIENTRY *)(GLenum object, GLint name))gwglGetProcAddress("glFinishObjectAPPLE");

		if(OpenGL_ExtensionsEnumeration("GL_APPLE_fence", GGL_APPLE_fence, element) == GLENUMERATION_FALSE)
			return;
	}

	//GL_APPLE_vertex_array_range && GL_APPLE_element_array && GL_ARB_vertex_program &&
	//GL_EXT_secondary_color && GL_EXT_fog_coord && GL_ARB_vertex_blend are not required
	if(StringToken(extensions, "GL_APPLE_vertex_array_object")/* && StringToken(extensions, "GL_APPLE_vertex_array_range") && StringToken(extensions, "GL_APPLE_element_array") && StringToken(extensions, "GL_ARB_vertex_program") && StringToken(extensions, "GL_EXT_secondary_color") && StringToken(extensions, "GL_EXT_fog_coord") && StringToken(extensions, "GL_ARB_vertex_blend")*/)
	{
		gglBindVertexArrayAPPLE = (void (APIENTRY *)(GLuint array))gwglGetProcAddress("glBindVertexArrayAPPLE");
		gglDeleteVertexArraysAPPLE = (void (APIENTRY *)(GLsizei n, const GLuint *arrays))gwglGetProcAddress("glDeleteVertexArraysAPPLE");
		gglGenVertexArraysAPPLE = (void (APIENTRY *)(GLsizei n, const GLuint *arrays))gwglGetProcAddress("glGenVertexArraysAPPLE");
		gglIsVertexArrayAPPLE = (GLboolean (APIENTRY *)(GLuint array))gwglGetProcAddress("glIsVertexArrayAPPLE");

		if(OpenGL_ExtensionsEnumeration("GL_APPLE_vertex_array_object", GGL_APPLE_vertex_array_object, element) == GLENUMERATION_FALSE)
			return;
	}

	//GL_APPLE_fence is not required
	if(StringToken(extensions, "GL_APPLE_vertex_array_range")/* && StringToken(extensions, "GL_APPLE_fence")*/)
	{
		gglVertexArrayRangeAPPLE = (void (APIENTRY *)(GLsizei length, GLvoid *pointer))gwglGetProcAddress("glVertexArrayRangeAPPLE");
		gglFlushVertexArrayRangeAPPLE = (void (APIENTRY *)(GLsizei length, GLvoid *pointer))gwglGetProcAddress("glFlushVertexArrayRangeAPPLE");
		gglVertexArrayParameteriAPPLE = (void (APIENTRY *)(GLenum pname, GLint param))gwglGetProcAddress("glVertexArrayParameteriAPPLE");

		if(OpenGL_ExtensionsEnumeration("GL_APPLE_vertex_array_range", GGL_APPLE_vertex_array_range, element) == GLENUMERATION_FALSE)
			return;
	}

	if(/*atof(version) >= 1.1 && */StringToken(extensions, "GL_APPLE_ycbcr_422") /* && (StringToken(extensions, "GL_APPLE_packed_pixels") || (atof(version) >= 1.2))*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_APPLE_ycbcr_422", GGL_APPLE_ycbcr_422, element) == GLENUMERATION_FALSE)
			return;
	}

	//XXX: not complete: dependencies not specified
	if(/*atof(version) >= 1.1 && */StringToken(extensions, "GL_S3_s3tc"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_S3_s3tc", GGL_S3_s3tc, element) == GLENUMERATION_FALSE)
			return;
	}

	if(/*atof(version) >= 1.3 && */StringToken(extensions, "GL_ATI_draw_buffers") /* && StringToken(extensions, "GL_ARB_fragment_program")*/)
	{
		gglDrawBuffersATI = (void (APIENTRY *)(GLsizei n, const GLenum *bufs))gwglGetProcAddress("glDrawBuffersATI");

		if(OpenGL_ExtensionsEnumeration("GL_ATI_draw_buffers", GGL_ATI_draw_buffers, element) == GLENUMERATION_FALSE)
			return;
	}

	//GL_ARB_multitexture is not required
	if(StringToken(extensions, "GL_ATI_texture_env_combine3") || StringToken(extensions, "GL_ATIX_texture_env_combine3") /* && StringToken(extensions, "GL_ARB_texture_env_combine")*//* && StringToken(extensions, "GL_ARB_multitexture")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_ATI_texture_env_combine3", GGL_ATI_texture_env_combine3, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_ATI_texture_float") /* && ((atof(version) >= 1.1) || StringToken(extensions, "GL_EXT_texture"))*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_ATI_texture_float", GGL_ATI_texture_float, element) == GLENUMERATION_FALSE)
			return;
	}

	//GL_EXT_paletted_texture && GL_SGIX_depth_texture &&
	//GL_NV_texture_shader && GL_NV_half_float are not required
	if(StringToken(extensions, "GL_NV_float_buffer") /* && StringToken(extensions, "GL_NV_fragment_program") && StringToken(extensions, "GL_NV_texture_rectangle") && StringToken(extensions, "WGL_ARB_pixel_format") && StringToken(extensions, "WGL_ARB_render_texture") && StringToken(extensions, "WGL_NV_render_texture_rectangle")*//* && StringToken(extensions, "GL_EXT_paletted_texture") && StringToken(extensions, "GL_SGIX_depth_texture") && StringToken(extensions, "GL_NV_texture_shader") && StringToken(extensions, "GL_NV_half_float")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_NV_float_buffer", GGL_NV_float_buffer, element) == GLENUMERATION_FALSE)
			return;
	}

	//GL_NV_vertex_program && GL_NV_register_combiners && GL_NV_texture_shader &&
	//GL_NV_texture_rectangle && GL_ARB_texture_cube_map && GL_EXT_fog_coord &&
	//GL_NV_depth_clamp && GL_ARB_depth_texture && GL_SGIX_depth_texture &&
	//GL_NV_float_buffer && GL_ARB_vertex_program are not required
	if(/*(!StringToken(version, "1.2.1")) && */StringToken(extensions, "GL_NV_fragment_program") /* && StringToken(extensions, "GL_ARB_multitexture")*//* && StringToken(extensions, "GL_NV_vertex_program") && StringToken(extensions, "GL_NV_register_combiners") && StringToken(extensions, "GL_NV_texture_shader") && StringToken(extensions, "GL_NV_texture_rectangle") && StringToken(extensions, "GL_ARB_texture_cube_map") && StringToken(extensions, "GL_EXT_fog_coord") && StringToken(extensions, "GL_NV_depth_clamp") && StringToken(extensions, "GL_ARB_depth_texture") && StringToken(extensions, "GL_SGIX_depth_texture") && StringToken(extensions, "GL_NV_float_buffer") && StringToken(extensions, "GL_ARB_vertex_program")*/)
	{
		gglProgramNamedParameter4fNV = (void (APIENTRY *)(GLuint id, GLsizei len, const GLubyte *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w))gwglGetProcAddress("glProgramNamedParameter4fNV");
		gglProgramNamedParameter4dNV = (void (APIENTRY *)(GLuint id, GLsizei len, const GLubyte *name, GLdouble x, GLdouble y, GLdouble z, GLdouble w))gwglGetProcAddress("glProgramNamedParameter4dNV");
		gglProgramNamedParameter4fvNV = (void (APIENTRY *)(GLuint id, GLsizei len, const GLubyte *name, const GLfloat *v))gwglGetProcAddress("glProgramNamedParameter4fvNV");
		gglProgramNamedParameter4dvNV = (void (APIENTRY *)(GLuint id, GLsizei len, const GLubyte *name, const GLdouble *v))gwglGetProcAddress("glProgramNamedParameter4dvNV");
		gglGetProgramNamedParameterfvNV = (void (APIENTRY *)(GLuint id, GLsizei len, const GLubyte *name, GLfloat *params))gwglGetProcAddress("glGetProgramNamedParameterfvNV");
		gglGetProgramNamedParameterdvNV = (void (APIENTRY *)(GLuint id, GLsizei len, const GLubyte *name, GLdouble *params))gwglGetProcAddress("glGetProgramNamedParameterdvNV");

		if(OpenGL_ExtensionsEnumeration("GL_NV_fragment_program", GGL_NV_fragment_program, element) == GLENUMERATION_FALSE)
			return;
	}

	//GL_NV_float_buffer && GL_EXT_fog_coord && GL_EXT_secondary_color &&
	//GL_EXT_vertex_weighting && GL_NV_vertex_program are not required
	if(/*atof(version) >= 1.1 && */StringToken(extensions, "GL_NV_half_float")/* && StringToken(extensions, "GL_NV_float_buffer") && StringToken(extensions, "GL_EXT_fog_coord") && StringToken(extensions, "GL_EXT_secondary_color") && StringToken(extensions, "GL_EXT_vertex_weighting") && StringToken(extensions, "GL_NV_vertex_program")*/)
	{
		gglVertex2hNV = (void (APIENTRY *)(GLhalfNV x, GLhalfNV y))gwglGetProcAddress("glVertex2hNV");
		gglVertex2hvNV = (void (APIENTRY *)(const GLhalfNV *v))gwglGetProcAddress("glVertex2hvNV");
		gglVertex3hNV = (void (APIENTRY *)(GLhalfNV x, GLhalfNV y, GLhalfNV z))gwglGetProcAddress("glVertex3hNV");
		gglVertex3hvNV = (void (APIENTRY *)(const GLhalfNV *v))gwglGetProcAddress("glVertex3hvNV");
		gglVertex4hNV = (void (APIENTRY *)(GLhalfNV x, GLhalfNV y, GLhalfNV z, GLhalfNV w))gwglGetProcAddress("glVertex4hNV");
		gglVertex4hvNV = (void (APIENTRY *)(const GLhalfNV *v))gwglGetProcAddress("glVertex4hvNV");
		gglNormal3hNV = (void (APIENTRY *)(GLhalfNV nx, GLhalfNV ny, GLhalfNV nz))gwglGetProcAddress("glNormal3hNV");
		gglNormal3hvNV = (void (APIENTRY *)(const GLhalfNV *v))gwglGetProcAddress("glNormal3hvNV");
		gglColor3hNV = (void (APIENTRY *)(GLhalfNV red, GLhalfNV green, GLhalfNV blue))gwglGetProcAddress("glColor3hNV");
		gglColor3hvNV = (void (APIENTRY *)(const GLhalfNV *v))gwglGetProcAddress("glColor3hvNV");
		gglColor4hNV = (void (APIENTRY *)(GLhalfNV red, GLhalfNV green, GLhalfNV blue, GLhalfNV alpha))gwglGetProcAddress("glColor4hNV");
		gglColor4hvNV = (void (APIENTRY *)(const GLhalfNV *v))gwglGetProcAddress("glColor4hvNV");
		gglTexCoord1hNV = (void (APIENTRY *)(GLhalfNV s))gwglGetProcAddress("glTexCoord1hNV");
		gglTexCoord1hvNV = (void (APIENTRY *)(const GLhalfNV *v))gwglGetProcAddress("glTexCoord1hvNV");
		gglTexCoord2hNV = (void (APIENTRY *)(GLhalfNV s, GLhalfNV t))gwglGetProcAddress("glTexCoord2hNV");
		gglTexCoord2hvNV = (void (APIENTRY *)(const GLhalfNV *v))gwglGetProcAddress("glTexCoord2hvNV");
		gglTexCoord3hNV = (void (APIENTRY *)(GLhalfNV s, GLhalfNV t, GLhalfNV r))gwglGetProcAddress("glTexCoord3hNV");
		gglTexCoord3hvNV = (void (APIENTRY *)(const GLhalfNV *v))gwglGetProcAddress("glTexCoord3hvNV");
		gglTexCoord4hNV = (void (APIENTRY *)(GLhalfNV s, GLhalfNV t, GLhalfNV r, GLhalfNV q))gwglGetProcAddress("glTexCoord4hNV");
		gglTexCoord4hvNV = (void (APIENTRY *)(const GLhalfNV *v))gwglGetProcAddress("glTexCoord4hvNV");
		gglMultiTexCoord1hNV = (void (APIENTRY *)(GLenum target, GLhalfNV s))gwglGetProcAddress("glMultiTexCoord1hNV");
		gglMultiTexCoord1hvNV = (void (APIENTRY *)(GLenum target, const GLhalfNV *v))gwglGetProcAddress("glMultiTexCoord1hvNV");
		gglMultiTexCoord2hNV = (void (APIENTRY *)(GLenum target, GLhalfNV s, GLhalfNV t))gwglGetProcAddress("glMultiTexCoord2hNV");
		gglMultiTexCoord2hvNV = (void (APIENTRY *)(GLenum target, const GLhalfNV *v))gwglGetProcAddress("glMultiTexCoord2hvNV");
		gglMultiTexCoord3hNV = (void (APIENTRY *)(GLenum target, GLhalfNV s, GLhalfNV t, GLhalfNV r))gwglGetProcAddress("glMultiTexCoord3hNV");
		gglMultiTexCoord3hvNV = (void (APIENTRY *)(GLenum target, const GLhalfNV *v))gwglGetProcAddress("glMultiTexCoord3hvNV");
		gglMultiTexCoord4hNV = (void (APIENTRY *)(GLenum target, GLhalfNV s, GLhalfNV t, GLhalfNV r, GLhalfNV q))gwglGetProcAddress("glMultiTexCoord4hNV");
		gglMultiTexCoord4hvNV = (void (APIENTRY *)(GLenum target, const GLhalfNV *v))gwglGetProcAddress("glMultiTexCoord4hvNV");
		//XXX: if EXT_fog_coord isn't supported, these will be NULL
		gglFogCoordhNV = (void (APIENTRY *)(GLhalfNV fog))gwglGetProcAddress("glFogCoordhNV");
		gglFogCoordhvNV = (void (APIENTRY *)(const GLhalfNV *fog))gwglGetProcAddress("glFogCoordhvNV");
		//XXX: if EXT_secondary_color isn't supported, these will be NULL
		gglSecondaryColor3hNV = (void (APIENTRY *)(GLhalfNV red, GLhalfNV green, GLhalfNV blue))gwglGetProcAddress("glSecondaryColor3hNV");
		gglSecondaryColor3hvNV = (void (APIENTRY *)(const GLhalfNV *v))gwglGetProcAddress("glSecondaryColor3hvNV");
		//XXX: if EXT_vertex_weighting isn't supported, these will be NULL
		gglVertexWeighthNV = (void (APIENTRY *)(GLhalfNV weight))gwglGetProcAddress("glVertexWeighthNV");
		gglVertexWeighthvNV = (void (APIENTRY *)(const GLhalfNV *weight))gwglGetProcAddress("glVertexWeighthvNV");
		//XXX: if NV_vertex_program isn't supported, these wil be NULL
		gglVertexAttrib1hNV = (void (APIENTRY *)(GLuint index, GLhalfNV x))gwglGetProcAddress("glVertexAttrib1hNV");
		gglVertexAttrib1hvNV = (void (APIENTRY *)(GLuint index, const GLhalfNV *v))gwglGetProcAddress("glVertexAttrib1hvNV");
		gglVertexAttrib2hNV = (void (APIENTRY *)(GLuint index, GLhalfNV x, GLhalfNV y))gwglGetProcAddress("glVertexAttrib2hNV");
		gglVertexAttrib2hvNV = (void (APIENTRY *)(GLuint index, const GLhalfNV *v))gwglGetProcAddress("glVertexAttrib2hvNV");
		gglVertexAttrib3hNV = (void (APIENTRY *)(GLuint index, GLhalfNV x, GLhalfNV y, GLhalfNV z))gwglGetProcAddress("glVertexAttrib3hNV");
		gglVertexAttrib3hvNV = (void (APIENTRY *)(GLuint index, const GLhalfNV *v))gwglGetProcAddress("glVertexAttrib3hvNV");
		gglVertexAttrib4hNV = (void (APIENTRY *)(GLuint index, GLhalfNV x, GLhalfNV y, GLhalfNV z, GLhalfNV w))gwglGetProcAddress("glVertexAttrib4hNV");
		gglVertexAttrib4hvNV = (void (APIENTRY *)(GLuint index, const GLhalfNV *v))gwglGetProcAddress("glVertexAttrib4hvNV");
		gglVertexAttribs1hvNV = (void (APIENTRY *)(GLuint index, GLsizei n, const GLhalfNV *v))gwglGetProcAddress("glVertexAttribs1hvNV");
		gglVertexAttribs2hvNV = (void (APIENTRY *)(GLuint index, GLsizei n, const GLhalfNV *v))gwglGetProcAddress("glVertexAttribs2hvNV");
		gglVertexAttribs3hvNV = (void (APIENTRY *)(GLuint index, GLsizei n, const GLhalfNV *v))gwglGetProcAddress("glVertexAttribs3hvNV");
		gglVertexAttribs4hvNV = (void (APIENTRY *)(GLuint index, GLsizei n, const GLhalfNV *v))gwglGetProcAddress("glVertexAttribs4hvNV");

		if(OpenGL_ExtensionsEnumeration("GL_NV_half_float", GGL_NV_half_float, element) == GLENUMERATION_FALSE)
			return;
	}

	//GL_NV_vertex_array_range is not required
	if(StringToken(extensions, "GL_NV_pixel_data_range")/* && StringToken(extensions, "GL_NV_vertex_array_range")*/)
	{
		gglPixelDataRangeNV = (void (APIENTRY *)(GLenum target, GLsizei length, GLvoid *pointer))gwglGetProcAddress("glPixelDataRangeNV");
		gglFlushPixelDataRangeNV = (void (APIENTRY *)(GLenum target))gwglGetProcAddress("glFlushPixelDataRangeNV");

		if(OpenGL_ExtensionsEnumeration("GL_NV_pixel_data_range", GGL_NV_pixel_data_range, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_NV_primitive_restart"))
	{
		gglPrimitiveRestartNV = (void (APIENTRY *)(void))gwglGetProcAddress("glPrimitiveRestartNV");
		gglPrimitiveRestartIndexNV = (void (APIENTRY *)(GLuint index))gwglGetProcAddress("glPrimitiveRestartIndexNV");

		if(OpenGL_ExtensionsEnumeration("GL_NV_primitive_restart", GGL_NV_primitive_restart, element) == GLENUMERATION_FALSE)
			return;
	}

	if(/*atof(version) >= 1.1 && */StringToken(extensions, "GL_NV_texture_expand_normal"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_NV_texture_expand_normal", GGL_NV_texture_expand_normal, element) == GLENUMERATION_FALSE)
			return;
	}

	if(/*atof(version) >= 1.3 && */StringToken(extensions, "GL_NV_vertex_program2") /* && StringToken(extensions, "GL_NV_vertex_program")*/)
	{
		if(OpenGL_ExtensionsEnumeration("GL_NV_vertex_program2", GGL_NV_vertex_program2, element) == GLENUMERATION_FALSE)
			return;
	}

	if(/*atof(version) >= 1.1 && */StringToken(extensions, "GL_ATI_map_object_buffer") /* && StringToken(extensions, "GL_ATI_vertex_array_object")*/)
	{
		gglMapObjectBufferATI = (GLvoid* (APIENTRY *)(GLuint buffer))gwglGetProcAddress("glMapObjectBufferATI");
		gglUnmapObjectBufferATI = (void (APIENTRY *)(GLuint buffer))gwglGetProcAddress("glUnmapObjectBufferATI");

		if(OpenGL_ExtensionsEnumeration("GL_ATI_map_object_buffer", GGL_ATI_map_object_buffer, element) == GLENUMERATION_FALSE)
			return;
	}

	if(/*atof(version) >= 1.0 && */StringToken(extensions, "GL_ATI_separate_stencil"))
	{
		gglStencilOpSeparateATI = (void (APIENTRY *)(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass))gwglGetProcAddress("glStencilOpSeparateATI");
		gglStencilFuncSeparateATI = (void (APIENTRY *)(GLenum frontfunc, GLenum backfunc, GLint ref, GLuint mask))gwglGetProcAddress("glStencilFuncSeparateATI");

		if(OpenGL_ExtensionsEnumeration("GL_ATI_separate_stencil", GGL_ATI_separate_stencil, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_ATI_vertex_attrib_array_object") /* && StringToken(extensions, "GL_ARB_vertex_program") && StringToken(extensions, "GL_ATI_vertex_array_object")*/)
	{
		gglVertexAttribArrayObjectATI = (void (APIENTRY *)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLuint buffer, GLuint offset))gwglGetProcAddress("glVertexAttribArrayObjectATI");
		gglGetVertexAttribArrayObjectfvATI = (void (APIENTRY *)(GLuint index, GLenum pname, GLfloat *params))gwglGetProcAddress("glGetVertexAttribArrayObjectfvATI");
		gglGetVertexAttribArrayObjectivATI = (void (APIENTRY *)(GLuint index, GLenum pname, GLint *params))gwglGetProcAddress("glGetVertexAttribArrayObjectivATI");

		if(OpenGL_ExtensionsEnumeration("GL_ATI_vertex_attrib_array_object", GGL_ATI_vertex_attrib_array_object, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_OES_fixed_point"))
	{
		gglAlphaFuncxOES = (void (APIENTRY *)(GLenum func, GLclampx ref))gwglGetProcAddress("glAlphaFuncxOES");
		gglClearColorxOES = (void (APIENTRY *)(GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha))gwglGetProcAddress("glClearColorxOES");
		gglClearDepthxOES = (void (APIENTRY *)(GLclampx depth))gwglGetProcAddress("glClearDepthxOES");
		gglClipPlanexOES = (void (APIENTRY *)(GLenum plane, const GLfixed *equation))gwglGetProcAddress("glClipPlanexOES");
		gglColor4xOES = (void (APIENTRY *)(GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha))gwglGetProcAddress("glColor4xOES");
		gglDepthRangexOES = (void (APIENTRY *)(GLclampx zNear, GLclampx zFar))gwglGetProcAddress("glDepthRangexOES");
		gglFogxOES = (void (APIENTRY *)(GLenum pname, GLfixed param))gwglGetProcAddress("glFogxOES");
		gglFogxvOES = (void (APIENTRY *)(GLenum pname, const GLfixed *params))gwglGetProcAddress("glFogxvOES");
		gglFrustumxOES = (void (APIENTRY *)(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar))gwglGetProcAddress("glFrustumxOES");
		gglGetClipPlanexOES = (void (APIENTRY *)(GLenum pname, GLfixed eqn[4]))gwglGetProcAddress("glGetClipPlanexOES");
		gglGetFixedvOES = (void (APIENTRY *)(GLenum pname, GLfixed *params))gwglGetProcAddress("glGetFixedvOES");
		gglGetLightxvOES = (void (APIENTRY *)(GLenum light, GLenum pname, GLfixed *params))gwglGetProcAddress("glGetLightxvOES");
		gglGetMaterialxvOES = (void (APIENTRY *)(GLenum face, GLenum pname, GLfixed *params))gwglGetProcAddress("glGetMaterialxvOES");
		gglGetTexEnvxvOES = (void (APIENTRY *)(GLenum env, GLenum pname, GLfixed *params))gwglGetProcAddress("glGetTexEnvxvOES");
		gglGetTexParameterxvOES = (void (APIENTRY *)(GLenum target, GLenum pname, GLfixed *params))gwglGetProcAddress("glGetTexParameterxvOES");
		gglLightModelxOES = (void (APIENTRY *)(GLenum pname, GLfixed param))gwglGetProcAddress("glLightModelxOES");
		gglLightModelxvOES = (void (APIENTRY *)(GLenum pname, const GLfixed *params))gwglGetProcAddress("glLightModelxvOES");
		gglLightxOES = (void (APIENTRY *)(GLenum light, GLenum pname, GLfixed param))gwglGetProcAddress("glLightxOES");
		gglLightxvOES = (void (APIENTRY *)(GLenum light, GLenum pname, const GLfixed *params))gwglGetProcAddress("glLightxvOES");
		gglLineWidthxOES = (void (APIENTRY *)(GLfixed width))gwglGetProcAddress("glLineWidthxOES");
		gglLoadMatrixxOES = (void (APIENTRY *)(const GLfixed *m))gwglGetProcAddress("glLoadMatrixxOES");
		gglMaterialxOES = (void (APIENTRY *)(GLenum face, GLenum pname, GLfixed param))gwglGetProcAddress("glMaterialxOES");
		gglMaterialxvOES = (void (APIENTRY *)(GLenum face, GLenum pname, const GLfixed *params))gwglGetProcAddress("glMaterialxvOES");
		gglMultMatrixxOES = (void (APIENTRY *)(const GLfixed *m))gwglGetProcAddress("glMultMatrixxOES");
		gglMultiTexCoord4xOES = (void (APIENTRY *)(GLenum target, GLfixed s, GLfixed t, GLfixed r, GLfixed q))gwglGetProcAddress("glMultiTexCoord4xOES");
		gglNormal3xOES = (void (APIENTRY *)(GLfixed nx, GLfixed ny, GLfixed nz))gwglGetProcAddress("glNormal3xOES");
		gglOrthoxOES = (void (APIENTRY *)(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar))gwglGetProcAddress("glOrthoxOES");
		gglPointParameterxOES = (void (APIENTRY *)(GLenum pname, GLfixed param))gwglGetProcAddress("glPointParameterxOES");
		gglPointParameterxvOES = (void (APIENTRY *)(GLenum pname, const GLfixed *params))gwglGetProcAddress("glPointParameterxvOES");
		gglPointSizexOES = (void (APIENTRY *)(GLfixed size))gwglGetProcAddress("glPointSizexOES");
		gglPolygonOffsetxOES = (void (APIENTRY *)(GLfixed factor, GLfixed units))gwglGetProcAddress("glPolygonOffsetxOES");
		gglRotatexOES = (void (APIENTRY *)(GLfixed angle, GLfixed x, GLfixed y, GLfixed z))gwglGetProcAddress("glRotatexOES");
		gglSampleCoveragexOES = (void (APIENTRY *)(GLclampx value, GLboolean invert))gwglGetProcAddress("glSampleCoveragexOES");
		gglScalexOES = (void (APIENTRY *)(GLfixed x, GLfixed y, GLfixed z))gwglGetProcAddress("glScalexOES");
		gglTexEnvxOES = (void (APIENTRY *)(GLenum target, GLenum pname, GLfixed param))gwglGetProcAddress("glTexEnvxOES");
		gglTexEnvxvOES = (void (APIENTRY *)(GLenum target, GLenum pname, const GLfixed *params))gwglGetProcAddress("glTexEnvxvOES");
		gglTexParameterxOES = (void (APIENTRY *)(GLenum target, GLenum pname, GLfixed param))gwglGetProcAddress("glTexParameterxOES");
		gglTexParameterxvOES = (void (APIENTRY *)(GLenum target, GLenum pname, const GLfixed *params))gwglGetProcAddress("glTexParameterxvOES");
		gglTranslatexOES = (void (APIENTRY *)(GLfixed x, GLfixed y, GLfixed z))gwglGetProcAddress("glTranslatexOES");

		if(OpenGL_ExtensionsEnumeration("GL_OES_fixed_point", GGL_OES_fixed_point, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_OES_single_precision"))
	{
		gglDepthRangefOES = (void (APIENTRY *)(GLclampf zNear, GLclampf zFar))gwglGetProcAddress("glDepthRangefOES");
		gglFrustumfOES = (void (APIENTRY *)(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar))gwglGetProcAddress("glFrustumfOES");
		gglOrthofOES = (void (APIENTRY *)(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar))gwglGetProcAddress("glOrthofOES");
		gglClipPlanefOES = (void (APIENTRY *)(GLenum plane, const GLfloat *equation))gwglGetProcAddress("glClipPlanefOES");
		gglGetClipPlanefOES = (void (APIENTRY *)(GLenum pname, GLfloat eqn[4]))gwglGetProcAddress("glGetClipPlanefOES");
		gglClearDepthfOES = (void (APIENTRY *)(GLclampf depth))gwglGetProcAddress("glClearDepthfOES");

		if(OpenGL_ExtensionsEnumeration("GL_OES_single_precision", GGL_OES_single_precision, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_OES_compressed_paletted_texture"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_OES_compressed_paletted_texture", GGL_OES_compressed_paletted_texture, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_OES_byte_coordinates"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_OES_byte_coordinates", GGL_OES_byte_coordinates, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_OES_read_format"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_OES_read_format", GGL_OES_read_format, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_OES_query_matrix"))
	{
		gglQueryMatrixxOES = (GLbitfield (APIENTRY * )(GLfixed mantissa[16], GLint exponent[16]))gwglGetProcAddress("glQueryMatrixxOES");

		if(OpenGL_ExtensionsEnumeration("GL_OES_query_matrix", GGL_OES_query_matrix, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_EXT_depth_bounds_test"))
	{
		gglDepthBoundsEXT = (void (APIENTRY *)(GLclampd zmin, GLclampd zmax))gwglGetProcAddress("glDepthBoundsEXT");

		if(OpenGL_ExtensionsEnumeration("GL_EXT_depth_bounds_test", GGL_EXT_depth_bounds_test, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_EXT_texture_mirror_clamp"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_EXT_texture_mirror_clamp", GGL_EXT_texture_mirror_clamp, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_EXT_blend_equation_separate"))
	{
		gglBlendEquationSeparateEXT = (void (APIENTRY *)(GLenum modeRGB, GLenum modeAlpha))gwglGetProcAddress("glBlendEquationSeparateEXT");

		if(OpenGL_ExtensionsEnumeration("GL_EXT_blend_equation_separate", GGL_EXT_blend_equation_separate, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_MESA_pack_invert"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_MESA_pack_invert", GGL_MESA_pack_invert, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_MESA_ycbcr_texture"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_MESA_ycbcr_texture", GGL_MESA_ycbcr_texture, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_EXT_pixel_buffer_object"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_EXT_pixel_buffer_object", GGL_EXT_pixel_buffer_object, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_NV_fragment_program_option"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_NV_fragment_program_option", GGL_NV_fragment_program_option, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_NV_fragment_program2"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_NV_fragment_program2", GGL_NV_fragment_program2, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_NV_vertex_program2_option"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_NV_vertex_program2_option", GGL_NV_vertex_program2_option, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_NV_vertex_program3"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_NV_vertex_program3", GGL_NV_vertex_program3, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_EXT_texture_compression_dxt1"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_EXT_texture_compression_dxt1", GGL_EXT_texture_compression_dxt1, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_EXT_framebuffer_object"))
	{
		gglIsRenderbufferEXT = (GLboolean (APIENTRY *)(GLuint renderbuffer))gwglGetProcAddress("glIsRenderbufferEXT");
		gglBindRenderbufferEXT = (void (APIENTRY *)(GLenum target, GLuint renderbuffer))gwglGetProcAddress("glBindRenderbufferEXT");
		gglDeleteRenderbuffersEXT = (void (APIENTRY *)(GLsizei n, const GLuint *renderbuffers))gwglGetProcAddress("glDeleteRenderbuffersEXT");
		gglGenRenderbuffersEXT = (void (APIENTRY *)(GLsizei n, GLuint *renderbuffers))gwglGetProcAddress("glGenRenderbuffersEXT");
		gglRenderbufferStorageEXT = (void (APIENTRY *)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height))gwglGetProcAddress("glRenderbufferStorageEXT");
		gglGetRenderbufferParameterivEXT = (void (APIENTRY *)(GLenum target, GLenum pname, GLint *params))gwglGetProcAddress("glGetRenderbufferParameterivEXT");
		gglIsFramebufferEXT = (GLboolean (APIENTRY *)(GLuint framebuffer))gwglGetProcAddress("glIsFramebufferEXT");
		gglBindFramebufferEXT = (void (APIENTRY *)(GLenum target, GLuint framebuffer))gwglGetProcAddress("glBindFramebufferEXT");
		gglDeleteFramebuffersEXT = (void (APIENTRY *)(GLsizei n, const GLuint *framebuffers))gwglGetProcAddress("glDeleteFramebuffersEXT");
		gglGenFramebuffersEXT = (void (APIENTRY *)(GLsizei n, GLuint *framebuffers))gwglGetProcAddress("glGenFramebuffersEXT");
		gglCheckFramebufferStatusEXT = (GLenum (APIENTRY *)(GLenum target))gwglGetProcAddress("glCheckFramebufferStatusEXT");
		gglFramebufferTexture1DEXT = (void (APIENTRY *)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level))gwglGetProcAddress("glFramebufferTexture1DEXT");
		gglFramebufferTexture2DEXT = (void (APIENTRY *)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level))gwglGetProcAddress("glFramebufferTexture2DEXT");
		gglFramebufferTexture3DEXT = (void (APIENTRY *)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset))gwglGetProcAddress("glFramebufferTexture3DEXT");
		gglFramebufferRenderbufferEXT = (void (APIENTRY *)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer))gwglGetProcAddress("glFramebufferRenderbufferEXT");
		gglGetFramebufferAttachmentParameterivEXT = (void (APIENTRY *)(GLenum target, GLenum attachment, GLenum pname, GLint *params))gwglGetProcAddress("glGetFramebufferAttachmentParameterivEXT");
		gglGenerateMipmapEXT = (void (APIENTRY *)(GLenum target))gwglGetProcAddress("glGenerateMipmapEXT");

		if(OpenGL_ExtensionsEnumeration("GL_EXT_framebuffer_object", GGL_EXT_framebuffer_object, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_GREMEDY_string_marker"))
	{
		gglStringMarkerGREMEDY = (void (APIENTRY *)(GLsizei len, const GLvoid *string))gwglGetProcAddress("glStringMarkerGREMEDY");

		if(OpenGL_ExtensionsEnumeration("GL_GREMEDY_string_marker", GGL_GREMEDY_string_marker, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_EXT_packed_depth_stencil"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_EXT_packed_depth_stencil", GGL_EXT_packed_depth_stencil, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_EXT_stencil_clear_tag"))
	{
		gglStencilClearTagEXT = (void (APIENTRY *)(GLsizei stencilTagBits, GLuint stencilClearTag))gwglGetProcAddress("glStencilClearTagEXT");

		if(OpenGL_ExtensionsEnumeration("GL_EXT_stencil_clear_tag", GGL_EXT_stencil_clear_tag, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_EXT_texture_sRGB"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_EXT_texture_sRGB", GGL_EXT_texture_sRGB, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_EXT_framebuffer_blit"))
	{
		gglBlitFramebufferEXT = (void (APIENTRY *)(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter))gwglGetProcAddress("glBlitFramebufferEXT");

		if(OpenGL_ExtensionsEnumeration("GL_EXT_framebuffer_blit", GGL_EXT_framebuffer_blit, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_EXT_framebuffer_multisample"))
	{
		gglRenderbufferStorageMultisampleEXT = (void (APIENTRY *)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height))gwglGetProcAddress("glRenderbufferStorageMultisampleEXT");

		if(OpenGL_ExtensionsEnumeration("GL_EXT_framebuffer_multisample", GGL_EXT_framebuffer_multisample, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_MESAX_texture_stack"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_MESAX_texture_stack", GGL_MESAX_texture_stack, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_KTX_buffer_region"))
	{
		gglBufferRegionEnabledEXT = (GLuint (APIENTRY *)(void))gwglGetProcAddress("glBufferRegionEnabledKTX");
		gglDeleteBufferRegionEXT = (void (APIENTRY *)(GLenum region))gwglGetProcAddress("glDeleteBufferRegionKTX");
		gglDrawBufferRegionEXT = (void (APIENTRY *)(GLuint region, GLint x, GLint y, GLsizei width, GLsizei height, GLint xDest, GLint yDest))gwglGetProcAddress("glDrawBufferRegionKTX");
		gglNewBufferRegionEXT = (GLuint (APIENTRY *)(GLenum region))gwglGetProcAddress("glNewBufferRegionKTX");
		gglReadBufferRegionEXT = (void (APIENTRY *)(GLuint region, GLint x, GLint y, GLsizei width, GLsizei height))gwglGetProcAddress("glReadBufferRegionKTX");

		if(OpenGL_ExtensionsEnumeration("GL_KTX_buffer_region", GGL_KTX_buffer_region, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_SUN_read_video_pixels"))
	{
		gglReadVideoPixelsSUN = (void (APIENTRY *)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid* pixels))gwglGetProcAddress("glReadVideoPixelsSUN");

		if(OpenGL_ExtensionsEnumeration("GL_SUN_read_video_pixels", GGL_SUN_read_video_pixels, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_EXT_Cg_shader"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_EXT_Cg_shader", GGL_EXT_Cg_shader, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_ATIX_point_sprites"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_ATIX_point_sprites", GGL_ATIX_point_sprites, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_ATIX_texture_env_route"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_ATIX_texture_env_route", GGL_ATIX_texture_env_route, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_ATIX_vertex_shader_output_point_size"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_ATIX_vertex_shader_output_point_size", GGL_ATIX_vertex_shader_output_point_size, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_ATI_texture_compression_3dc"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_ATI_texture_compression_3dc", GGL_ATI_texture_compression_3dc, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_EXT_texture_rectangle"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_EXT_texture_rectangle", GGL_EXT_texture_rectangle, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_APPLE_float_pixels"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_APPLE_float_pixels", GGL_APPLE_float_pixels, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_APPLE_pixel_buffer"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_APPLE_pixel_buffer", GGL_APPLE_pixel_buffer, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_APPLE_texture_range"))
	{
		if(OpenGL_ExtensionsEnumeration("GL_APPLE_texture_range", GGL_APPLE_texture_range, element) == GLENUMERATION_FALSE)
			return;
	}

	if(StringToken(extensions, "GL_WIN_swap_hint"))
	{
		gglAddSwapHintRectWIN = (void (APIENTRY *)(GLint x, GLint y, GLsizei width, GLsizei height))gwglGetProcAddress("glAddSwapHintRectWIN");

		if(OpenGL_ExtensionsEnumeration("GL_WIN_swap_hint", GGL_WIN_swap_hint, element) == GLENUMERATION_FALSE)
			return;
	}

//==========================================================

	if(atof(version) >= 1.1)
	{
		if(!(gglAreTexturesResident = (GLboolean (APIENTRY *)(GLsizei n, const GLuint *textures, GLboolean *residences))GetProcAddress(hInstOpenGL, "glAreTexturesResident")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glAreTexturesResident proc address");

		if(!(gglArrayElement = (void (APIENTRY *)(GLint i))GetProcAddress(hInstOpenGL, "glArrayElement")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glArrayElement proc address");

		if(!(gglBindTexture = (void (APIENTRY *)(GLenum target, GLuint texture))GetProcAddress(hInstOpenGL, "glBindTexture")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glBindTexture proc address");

		if(!(gglColorPointer = (void (APIENTRY *)(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer))GetProcAddress(hInstOpenGL, "glColorPointer")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glColorPointer proc address");

		if(!(gglCopyTexImage1D = (void (APIENTRY *)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border))GetProcAddress(hInstOpenGL, "glCopyTexImage1D")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glCopyTexImage1D proc address");

		if(!(gglCopyTexImage2D = (void (APIENTRY *)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border))GetProcAddress(hInstOpenGL, "glCopyTexImage2D")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glCopyTexImage2D proc address");

		if(!(gglCopyTexSubImage1D = (void (APIENTRY *)(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width))GetProcAddress(hInstOpenGL, "glCopyTexSubImage1D")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glCopyTexSubImage1D proc address");

		if(!(gglCopyTexSubImage2D = (void (APIENTRY *)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height))GetProcAddress(hInstOpenGL, "glCopyTexSubImage2D")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glCopyTexSubImage2D proc address");

		//glCopyTexSubImage3D is not available in OGL 1.1 (must use glCopyTexSubImage3DEXT)

		if(!(gglDeleteTextures = (void (APIENTRY *)(GLsizei n, const GLuint *textures))GetProcAddress(hInstOpenGL, "glDeleteTextures")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glDeleteTextures proc address");

		if(!(gglDisableClientState = (void (APIENTRY *)(GLenum array))GetProcAddress(hInstOpenGL, "glDisableClientState")))
			OpenGL_Error("OpenGL_Init: Couldn't get glDisableClientState proc address");

		if(!(gglDrawArrays = (void (APIENTRY *)(GLenum mode, GLint first, GLsizei count))GetProcAddress(hInstOpenGL, "glDrawArrays")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't getglDrawArrays  proc address");

		if(!(gglDrawElements = (void (APIENTRY *)(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices))GetProcAddress(hInstOpenGL, "glDrawElements")))
			OpenGL_Error("OpenGL_Init: Couldn't get glDrawElements proc address");

		if(!(gglEdgeFlagPointer = (void (APIENTRY *)(GLsizei stride, const GLboolean *pointer))GetProcAddress(hInstOpenGL, "glEdgeFlagPointer")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glEdgeFlagPointer proc address");

		if(!(gglEnableClientState = (void (APIENTRY *)(GLenum array))GetProcAddress(hInstOpenGL, "glEnableClientState")))
			OpenGL_Error("OpenGL_Init: Couldn't get glEnableClientState proc address");

		if(!(gglGenTextures = (void (APIENTRY *)(GLsizei n, GLuint *textures))GetProcAddress(hInstOpenGL, "glGenTextures")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glGenTextures proc address");

		if(!(gglGetPointerv = (void (APIENTRY *)(GLenum pname, GLvoid* *params))GetProcAddress(hInstOpenGL, "glGetPointerv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glGetPointerv proc address");

		if(!(gglIndexub = (void (APIENTRY *)(GLubyte c))GetProcAddress(hInstOpenGL, "glIndexub")))
			OpenGL_Error("OpenGL_Init: Couldn't get glIndexub proc address");

		if(!(gglIndexubv = (void (APIENTRY *)(const GLubyte *c))GetProcAddress(hInstOpenGL, "glIndexubv")))
			OpenGL_Error("OpenGL_Init: Couldn't get glIndexubv proc address");

		if(!(gglIndexPointer = (void (APIENTRY *)(GLenum type, GLsizei stride, const GLvoid *pointer))GetProcAddress(hInstOpenGL, "glIndexPointer")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glIndexPointer proc address");

		if(!(gglInterleavedArrays = (void (APIENTRY *)(GLenum format, GLsizei stride, const GLvoid *pointer))GetProcAddress(hInstOpenGL, "glInterleavedArrays")))
			OpenGL_Error("OpenGL_Init: Couldn't get glInterleavedArrays proc address");

		if(!(gglIsTexture = (GLboolean (APIENTRY *)(GLuint texture))GetProcAddress(hInstOpenGL, "glIsTexture")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glIsTexture proc address");

		if(!(gglNormalPointer = (void (APIENTRY *)(GLenum type, GLsizei stride, const GLvoid *pointer))GetProcAddress(hInstOpenGL, "glNormalPointer")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glNormalPointer proc address");

		if(!(gglPolygonOffset = (void (APIENTRY *)(GLfloat factor, GLfloat units))GetProcAddress(hInstOpenGL, "glPolygonOffset")))
			OpenGL_Error("OpenGL_Init: Couldn't get glPolygonOffset proc address");

		if(!(gglPopClientAttrib = (void (APIENTRY *)(void))GetProcAddress(hInstOpenGL, "glPopClientAttrib")))
			OpenGL_Error("OpenGL_Init: Couldn't get glPopClientAttrib proc address");

		if(!(gglPrioritizeTextures = (void (APIENTRY *)(GLsizei n, const GLuint *textures, const GLclampf *priorities))GetProcAddress(hInstOpenGL, "glPrioritizeTextures")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glPrioritizeTextures proc address");

		if(!(gglPushClientAttrib = (void (APIENTRY *)(GLbitfield mask))GetProcAddress(hInstOpenGL, "glPushClientAttrib")))
			OpenGL_Error("OpenGL_Init: Couldn't get glPushClientAttrib proc address");

		if(!(gglTexCoordPointer = (void (APIENTRY *)(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer))GetProcAddress(hInstOpenGL, "glTexCoordPointer")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glTexCoordPointer proc address");

		if(!(gglTexSubImage1D = (void (APIENTRY *)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels))GetProcAddress(hInstOpenGL, "glTexSubImage1D")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glTexSubImage1D proc address");

		if(!(gglTexSubImage2D = (void (APIENTRY *)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels))GetProcAddress(hInstOpenGL, "glTexSubImage2D")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glTexSubImage2D proc address");

		if(!(gglVertexPointer = (void (APIENTRY *)(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer))GetProcAddress(hInstOpenGL, "glVertexPointer")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexPointer proc address");
	}

	if(atof(version) >= 1.2)
	{
		if(!(gglDrawRangeElements = (void (APIENTRY *)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices))gwglGetProcAddress("glDrawRangeElements")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glDrawRangeElements proc address");

		if(!(gglTexImage3D = (void (APIENTRY *)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels))gwglGetProcAddress("glTexImage3D")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glTexImage3D proc address");

		if(!(gglTexSubImage3D = (void (APIENTRY *)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels))gwglGetProcAddress("glTexSubImage3D")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glTexSubImage3D proc address");

		if(!(gglCopyTexSubImage3D = (void (APIENTRY *)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height))gwglGetProcAddress("glCopyTexSubImage3D")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glCopyTexSubImage3D proc address");
	}

	if(StringToken(extensions, "GL_ARB_imaging"))
	{
		if(!(gglBlendColor = (void (APIENTRY *)(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha))gwglGetProcAddress("glBlendColor")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glBlendColor proc address");

		if(!(gglBlendEquation = (void (APIENTRY *)(GLenum mode))gwglGetProcAddress("glBlendEquation")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glBlendEquation proc address");

		if(!(gglColorTable = (void (APIENTRY *)(GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *table))gwglGetProcAddress("glColorTable")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glColorTable proc address");

		if(!(gglColorTableParameterfv = (void (APIENTRY *)(GLenum target, GLenum pname, const GLfloat *params))gwglGetProcAddress("glColorTableParameterfv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glColorTableParameterfv proc address");

		if(!(gglColorTableParameteriv = (void (APIENTRY *)(GLenum target, GLenum pname, const GLint *params))gwglGetProcAddress("glColorTableParameteriv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glColorTableParameteriv proc address");

		if(!(gglCopyColorTable = (void (APIENTRY *)(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width))gwglGetProcAddress("glCopyColorTable")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glCopyColorTable proc address");

		if(!(gglGetColorTable = (void (APIENTRY *)(GLenum target, GLenum format, GLenum type, GLvoid *table))gwglGetProcAddress("glGetColorTable")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glGetColorTable proc address");

		if(!(gglGetColorTableParameterfv = (void (APIENTRY *)(GLenum target, GLenum pname, GLfloat *params))gwglGetProcAddress("glGetColorTableParameterfv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glGetColorTableParameterfv proc address");

		if(!(gglGetColorTableParameteriv = (void (APIENTRY *)(GLenum target, GLenum pname, GLint *params))gwglGetProcAddress("glGetColorTableParameteriv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glGetColorTableParameteriv proc address");

		if(!(gglColorSubTable = (void (APIENTRY *)(GLenum target, GLsizei start, GLsizei count, GLenum format, GLenum type, const GLvoid *data))gwglGetProcAddress("glColorSubTable")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glColorSubTable proc address");

		if(!(gglCopyColorSubTable = (void (APIENTRY *)(GLenum target, GLsizei start, GLint x, GLint y, GLsizei width))gwglGetProcAddress("glCopyColorSubTable")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glCopyColorSubTable proc address");

		if(!(gglConvolutionFilter1D = (void (APIENTRY *)(GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *image))gwglGetProcAddress("glConvolutionFilter1D")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glConvolutionFilter1D proc address");

		if(!(gglConvolutionFilter2D = (void (APIENTRY *)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *image))gwglGetProcAddress("glConvolutionFilter2D")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glConvolutionFilter2D proc address");

		if(!(gglConvolutionParameterf = (void (APIENTRY *)(GLenum target, GLenum pname, GLfloat params))gwglGetProcAddress("glConvolutionParameterf")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glConvolutionParameterf proc address");

		if(!(gglConvolutionParameterfv = (void (APIENTRY *)(GLenum target, GLenum pname, const GLfloat *params))gwglGetProcAddress("glConvolutionParameterfv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glConvolutionParameterfv proc address");

		if(!(gglConvolutionParameteri = (void (APIENTRY *)(GLenum target, GLenum pname, GLint params))gwglGetProcAddress("glConvolutionParameteri")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glConvolutionParameteri proc address");

		if(!(gglConvolutionParameteriv = (void (APIENTRY *)(GLenum target, GLenum pname, const GLint *params))gwglGetProcAddress("glConvolutionParameteriv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glConvolutionParameteriv proc address");

		if(!(gglCopyConvolutionFilter1D = (void (APIENTRY *)(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width))gwglGetProcAddress("glCopyConvolutionFilter1D")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glCopyConvolutionFilter1D proc address");

		if(!(gglCopyConvolutionFilter2D = (void (APIENTRY *)(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height))gwglGetProcAddress("glCopyConvolutionFilter2D")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glCopyConvolutionFilter2D proc address");

		if(!(gglGetConvolutionFilter = (void (APIENTRY *)(GLenum target, GLenum format, GLenum type, GLvoid *image))gwglGetProcAddress("glGetConvolutionFilter")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glGetConvolutionFilter proc address");

		if(!(gglGetConvolutionParameterfv = (void (APIENTRY *)(GLenum target, GLenum pname, GLfloat *params))gwglGetProcAddress("glGetConvolutionParameterfv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glGetConvolutionParameterfv proc address");

		if(!(gglGetConvolutionParameteriv = (void (APIENTRY *)(GLenum target, GLenum pname, GLint *params))gwglGetProcAddress("glGetConvolutionParameteriv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glGetConvolutionParameteriv proc address");

		if(!(gglGetSeparableFilter = (void (APIENTRY *)(GLenum target, GLenum format, GLenum type, GLvoid *row, GLvoid *column, GLvoid *span))gwglGetProcAddress("glGetSeparableFilter")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glGetSeparableFilter proc address");

		if(!(gglSeparableFilter2D = (void (APIENTRY *)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *row, const GLvoid *column))gwglGetProcAddress("glSeparableFilter2D")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glSeparableFilter2D proc address");

		if(!(gglGetHistogram = (void (APIENTRY *)(GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid *values))gwglGetProcAddress("glGetHistogram")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glGetHistogram proc address");

		if(!(gglGetHistogramParameterfv = (void (APIENTRY *)(GLenum target, GLenum pname, GLfloat *params))gwglGetProcAddress("glGetHistogramParameterfv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glGetHistogramParameterfv proc address");

		if(!(gglGetHistogramParameteriv = (void (APIENTRY *)(GLenum target, GLenum pname, GLint *params))gwglGetProcAddress("glGetHistogramParameteriv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glGetHistogramParameteriv proc address");

		if(!(gglGetMinmax = (void (APIENTRY *)(GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid *values))gwglGetProcAddress("glGetMinmax")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glGetMinmax proc address");

		if(!(gglGetMinmaxParameterfv = (void (APIENTRY *)(GLenum target, GLenum pname, GLfloat *params))gwglGetProcAddress("glGetMinmaxParameterfv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glGetMinmaxParameterfv proc address");

		if(!(gglGetMinmaxParameteriv = (void (APIENTRY *)(GLenum target, GLenum pname, GLint *params))gwglGetProcAddress("glGetMinmaxParameteriv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glGetMinmaxParameteriv proc address");

		if(!(gglHistogram = (void (APIENTRY *)(GLenum target, GLsizei width, GLenum internalformat, GLboolean sink))gwglGetProcAddress("glHistogram")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glHistogram proc address");

		if(!(gglMinmax = (void (APIENTRY *)(GLenum target, GLenum internalformat, GLboolean sink))gwglGetProcAddress("glMinmax")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glMinmax proc address");

		if(!(gglResetHistogram = (void (APIENTRY *)(GLenum target))gwglGetProcAddress("glResetHistogram")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glResetHistogram proc address");

		if(!(gglResetMinmax = (void (APIENTRY *)(GLenum target))gwglGetProcAddress("glResetMinmax")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glResetMinmax proc address");

		if(OpenGL_ExtensionsEnumeration("GL_ARB_imaging", GGL_ARB_imaging, element) == GLENUMERATION_FALSE)
			return;
	}

	if(atof(version) >= 1.3)
	{
		if(!(gglActiveTexture = (void (APIENTRY *)(GLenum texture))gwglGetProcAddress("glActiveTexture")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glActiveTexture proc address");

		if(!(gglClientActiveTexture = (void (APIENTRY *)(GLenum texture))gwglGetProcAddress("glClientActiveTexture")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glClientActiveTexture proc address");

		if(!(gglMultiTexCoord1d = (void (APIENTRY *)(GLenum target, GLdouble s))gwglGetProcAddress("glMultiTexCoord1d")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glMultiTexCoord1d proc address");

		if(!(gglMultiTexCoord1dv  =(void (APIENTRY *)(GLenum target, const GLdouble *v))gwglGetProcAddress("glMultiTexCoord1dv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glMultiTexCoord1dv proc address");

		if(!(gglMultiTexCoord1f = (void (APIENTRY *)(GLenum target, GLfloat s))gwglGetProcAddress("glMultiTexCoord1f")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glMultiTexCoord1f proc address");

		if(!(gglMultiTexCoord1fv = (void (APIENTRY *)(GLenum target, const GLfloat *v))gwglGetProcAddress("glMultiTexCoord1fv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glMultiTexCoord1fv proc address");

		if(!(gglMultiTexCoord1i = (void (APIENTRY *)(GLenum target, GLint s))gwglGetProcAddress("glMultiTexCoord1i")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glMultiTexCoord1i proc address");

		if(!(gglMultiTexCoord1iv = (void (APIENTRY *)(GLenum target, const GLint *v))gwglGetProcAddress("glMultiTexCoord1iv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glMultiTexCoord1iv proc address");

		if(!(gglMultiTexCoord1s = (void (APIENTRY *)(GLenum target, GLshort s))gwglGetProcAddress("glMultiTexCoord1s")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glMultiTexCoord1s proc address");

		if(!(gglMultiTexCoord1sv = (void (APIENTRY *)(GLenum target, const GLshort *v))gwglGetProcAddress("glMultiTexCoord1sv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glMultiTexCoord1sv proc address");

		if(!(gglMultiTexCoord2d = (void (APIENTRY *)(GLenum target, GLdouble s, GLdouble t))gwglGetProcAddress("glMultiTexCoord2d")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glMultiTexCoord2d proc address");

		if(!(gglMultiTexCoord2dv = (void (APIENTRY *)(GLenum target, const GLdouble *v))gwglGetProcAddress("glMultiTexCoord2dv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glMultiTexCoord2dv proc address");

		if(!(gglMultiTexCoord2f = (void (APIENTRY *)(GLenum target, GLfloat s, GLfloat t))gwglGetProcAddress("glMultiTexCoord2f")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glMultiTexCoord2f proc address");

		if(!(gglMultiTexCoord2fv = (void (APIENTRY *)(GLenum target, const GLfloat *v))gwglGetProcAddress("glMultiTexCoord2fv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glMultiTexCoord2fv proc address");

		if(!(gglMultiTexCoord2i = (void (APIENTRY *)(GLenum target, GLint s, GLint t))gwglGetProcAddress("glMultiTexCoord2i")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glMultiTexCoord2i proc address");

		if(!(gglMultiTexCoord2iv = (void (APIENTRY *)(GLenum target, const GLint *v))gwglGetProcAddress("glMultiTexCoord2iv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glMultiTexCoord2iv proc address");

		if(!(gglMultiTexCoord2s = (void (APIENTRY *)(GLenum target, GLshort s, GLshort t))gwglGetProcAddress("glMultiTexCoord2s")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glMultiTexCoord2s proc address");

		if(!(gglMultiTexCoord2sv = (void (APIENTRY *)(GLenum target, const GLshort *v))gwglGetProcAddress("glMultiTexCoord2sv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glMultiTexCoord2sv proc address");

		if(!(gglMultiTexCoord3d = (void (APIENTRY *)(GLenum target, GLdouble s, GLdouble t, GLdouble r))gwglGetProcAddress("glMultiTexCoord3d")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glMultiTexCoord3d proc address");

		if(!(gglMultiTexCoord3dv = (void (APIENTRY *)(GLenum target, const GLdouble *v))gwglGetProcAddress("glMultiTexCoord3dv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glMultiTexCoord3dv proc address");

		if(!(gglMultiTexCoord3f = (void (APIENTRY *)(GLenum target, GLfloat s, GLfloat t, GLfloat r))gwglGetProcAddress("glMultiTexCoord3f")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glMultiTexCoord3f proc address");

		if(!(gglMultiTexCoord3fv = (void (APIENTRY *)(GLenum target, const GLfloat *v))gwglGetProcAddress("glMultiTexCoord3fv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glMultiTexCoord3fv proc address");

		if(!(gglMultiTexCoord3i = (void (APIENTRY *)(GLenum target, GLint s, GLint t, GLint r))gwglGetProcAddress("glMultiTexCoord3i")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glMultiTexCoord3i proc address");

		if(!(gglMultiTexCoord3iv = (void (APIENTRY *)(GLenum target, const GLint *v))gwglGetProcAddress("glMultiTexCoord3iv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glMultiTexCoord3iv proc address");

		if(!(gglMultiTexCoord3s = (void (APIENTRY *)(GLenum target, GLshort s, GLshort t, GLshort r))gwglGetProcAddress("glMultiTexCoord3s")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glMultiTexCoord3s proc address");

		if(!(gglMultiTexCoord3sv = (void (APIENTRY *)(GLenum target, const GLshort *v))gwglGetProcAddress("glMultiTexCoord3sv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glMultiTexCoord3sv proc address");

		if(!(gglMultiTexCoord4d = (void (APIENTRY *)(GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q))gwglGetProcAddress("glMultiTexCoord4d")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glMultiTexCoord4d proc address");

		if(!(gglMultiTexCoord4dv = (void (APIENTRY *)(GLenum target, const GLdouble *v))gwglGetProcAddress( "glMultiTexCoord4dv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glMultiTexCoord4dv proc address");

		if(!(gglMultiTexCoord4f = (void (APIENTRY *)(GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q))gwglGetProcAddress("glMultiTexCoord4f")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glMultiTexCoord4f proc address");

		if(!(gglMultiTexCoord4fv = (void (APIENTRY *)(GLenum target, const GLfloat *v))gwglGetProcAddress("glMultiTexCoord4fv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glMultiTexCoord4fv proc address");

		if(!(gglMultiTexCoord4i = (void (APIENTRY *)(GLenum target, GLint s, GLint t, GLint r, GLint q))gwglGetProcAddress("glMultiTexCoord4i")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glMultiTexCoord4i proc address");

		if(!(gglMultiTexCoord4iv = (void (APIENTRY *)(GLenum target, const GLint *v))gwglGetProcAddress( "glMultiTexCoord4iv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glMultiTexCoord4iv proc address");

		if(!(gglMultiTexCoord4s = (void (APIENTRY *)(GLenum target, GLshort s, GLshort t, GLshort r, GLshort q))gwglGetProcAddress("glMultiTexCoord4s")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glMultiTexCoord4s proc address");

		if(!(gglMultiTexCoord4sv = (void (APIENTRY *)(GLenum target, const GLshort *v))gwglGetProcAddress("glMultiTexCoord4sv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glMultiTexCoord4sv proc address");

		if(!(gglLoadTransposeMatrixf = (void (APIENTRY *)(const GLfloat *m))gwglGetProcAddress("glLoadTransposeMatrixf")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glLoadTransposeMatrixf proc address");

		if(!(gglLoadTransposeMatrixd = (void (APIENTRY *)(const GLdouble *m))gwglGetProcAddress("glLoadTransposeMatrixd")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glLoadTransposeMatrixd proc address");

		if(!(gglMultTransposeMatrixf = (void (APIENTRY *)(const GLfloat *m))gwglGetProcAddress("glMultTransposeMatrixf")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glMultTransposeMatrixf proc address");

		if(!(gglMultTransposeMatrixd = (void (APIENTRY *)(const GLdouble *m))gwglGetProcAddress("glMultTransposeMatrixd")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glMultTransposeMatrixd proc address");

		if(!(gglSampleCoverage = (void (APIENTRY *)(GLclampf value, GLboolean invert))gwglGetProcAddress("glSampleCoverage")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glSampleCoverage proc address");

		if(!(gglCompressedTexImage3D = (void (APIENTRY *)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid *data))gwglGetProcAddress("glCompressedTexImage3D")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glCompressedTexImage3D proc address");

		if(!(gglCompressedTexImage2D = (void (APIENTRY *)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data))gwglGetProcAddress("glCompressedTexImage2D")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glCompressedTexImage2D proc address");

		if(!(gglCompressedTexImage1D = (void (APIENTRY *)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid *data))gwglGetProcAddress("glCompressedTexImage1D")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glCompressedTexImage1D proc address");

		if(!(gglCompressedTexSubImage3D = (void (APIENTRY *)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid *data))gwglGetProcAddress("glCompressedTexSubImage3D")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glCompressedTexSubImage3D proc address");

		if(!(gglCompressedTexSubImage2D = (void (APIENTRY *)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data))gwglGetProcAddress("glCompressedTexSubImage2D")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glCompressedTexSubImage2D proc address");

		if(!(gglCompressedTexSubImage1D = (void (APIENTRY *)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid *data))gwglGetProcAddress("glCompressedTexSubImage1D")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glCompressedTexSubImage1D proc address");

		if(!(gglGetCompressedTexImage = (void (APIENTRY *)(GLenum target, GLint level, void *img))gwglGetProcAddress("glGetCompressedTexImage")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glGetCompressedTexImage proc address");
	}

	if(atof(version) >= 1.4)
	{
		if(!(gglBlendFuncSeparate = (void (APIENTRY *)(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha))gwglGetProcAddress("glBlendFuncSeparate")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glBlendFuncSeparate proc address");

		if(!(gglFogCoordf = (void (APIENTRY *)(GLfloat coord))gwglGetProcAddress("glFogCoordf")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glFogCoordf proc address");

		if(!(gglFogCoordfv = (void (APIENTRY *)(const GLfloat *coord))gwglGetProcAddress("glFogCoordfv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glFogCoordfv proc address");

		if(!(gglFogCoordd = (void (APIENTRY *)(GLdouble coord))gwglGetProcAddress("glFogCoordd")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glFogCoordd proc address");

		if(!(gglFogCoorddv = (void (APIENTRY *)(const GLdouble *coord))gwglGetProcAddress("glFogCoorddv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glFogCoorddv proc address");

		if(!(gglFogCoordPointer = (void (APIENTRY *)(GLenum type, GLsizei stride, const GLvoid *pointer))gwglGetProcAddress("glFogCoordPointer")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glFogCoordPointer proc address");

		if(!(gglMultiDrawArrays = (void (APIENTRY *)(GLenum mode, GLint *first, GLsizei *count, GLsizei primcount))gwglGetProcAddress("glMultiDrawArrays")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glMultiDrawArrays proc address");

		if(!(gglMultiDrawElements = (void (APIENTRY *)(GLenum mode, const GLsizei *count, GLenum type, const GLvoid **indices, GLsizei primcount))gwglGetProcAddress("glMultiDrawElements")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glMultiDrawElements proc address");

		if(!(gglPointParameterf = (void (APIENTRY *)(GLenum pname, GLfloat param))gwglGetProcAddress("glPointParameterf")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glPointParameterf proc address");

		if(!(gglPointParameterfv = (void (APIENTRY *)(GLenum pname, const GLfloat *params))gwglGetProcAddress("glPointParameterfv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glPointParameterfv proc address");

		if(!(gglPointParameteri = (void (APIENTRY *)(GLenum pname, GLint param))gwglGetProcAddress("glPointParameteri")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glPointParameteri proc address");

		if(!(gglPointParameteriv = (void (APIENTRY *)(GLenum pname, const GLint *params))gwglGetProcAddress("glPointParameteriv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glPointParameteriv proc address");
		
		if(!(gglSecondaryColor3b = (void (APIENTRY *)(GLbyte red, GLbyte green, GLbyte blue))gwglGetProcAddress("glSecondaryColor3b")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glSecondaryColor3b proc address");

		if(!(gglSecondaryColor3bv = (void (APIENTRY *)(const GLbyte *v))gwglGetProcAddress("glSecondaryColor3bv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glSecondaryColor3bv proc address");

		if(!(gglSecondaryColor3d = (void (APIENTRY *)(GLdouble red, GLdouble green, GLdouble blue))gwglGetProcAddress("glSecondaryColor3d")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glSecondaryColor3d proc address");

		if(!(gglSecondaryColor3dv = (void (APIENTRY *)(const GLdouble *v))gwglGetProcAddress("glSecondaryColor3dv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glSecondaryColor3dv proc address");

		if(!(gglSecondaryColor3f = (void (APIENTRY *)(GLfloat red, GLfloat green, GLfloat blue))gwglGetProcAddress("glSecondaryColor3f")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glSecondaryColor3f proc address");

		if(!(gglSecondaryColor3fv = (void (APIENTRY *)(const GLfloat *v))gwglGetProcAddress("glSecondaryColor3fv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glSecondaryColor3fv proc address");

		if(!(gglSecondaryColor3i = (void (APIENTRY *)(GLint red, GLint green, GLint blue))gwglGetProcAddress("glSecondaryColor3i")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glSecondaryColor3i proc address");

		if(!(gglSecondaryColor3iv = (void (APIENTRY *)(const GLint *v))gwglGetProcAddress("glSecondaryColor3iv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glSecondaryColor3iv proc address");

		if(!(gglSecondaryColor3s = (void (APIENTRY *)(GLshort red, GLshort green, GLshort blue))gwglGetProcAddress("glSecondaryColor3s")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glSecondaryColor3s proc address");

		if(!(gglSecondaryColor3sv = (void (APIENTRY *)(const GLshort *v))gwglGetProcAddress("glSecondaryColor3sv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glSecondaryColor3sv proc address");

		if(!(gglSecondaryColor3ub = (void (APIENTRY *)(GLubyte red, GLubyte green, GLubyte blue))gwglGetProcAddress("glSecondaryColor3ub")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glSecondaryColor3ub proc address");

		if(!(gglSecondaryColor3ubv = (void (APIENTRY *)(const GLubyte *v))gwglGetProcAddress("glSecondaryColor3ubv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glSecondaryColor3ubv proc address");

		if(!(gglSecondaryColor3ui = (void (APIENTRY *)(GLuint red, GLuint green, GLuint blue))gwglGetProcAddress("glSecondaryColor3ui")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glSecondaryColor3ui proc address");

		if(!(gglSecondaryColor3uiv = (void (APIENTRY *)(const GLuint *v))gwglGetProcAddress("glSecondaryColor3uiv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glSecondaryColor3uiv proc address");

		if(!(gglSecondaryColor3us = (void (APIENTRY *)(GLushort red, GLushort green, GLushort blue))gwglGetProcAddress("glSecondaryColor3us")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glSecondaryColor3us proc address");

		if(!(gglSecondaryColor3usv = (void (APIENTRY *)(const GLushort *v))gwglGetProcAddress("glSecondaryColor3usv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glSecondaryColor3usv proc address");

		if(!(gglSecondaryColorPointer = (void (APIENTRY *)(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer))gwglGetProcAddress("glSecondaryColorPointer")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glSecondaryColorPointer proc address");

		if(!(gglWindowPos2d = (void (APIENTRY *)(GLdouble x, GLdouble y))gwglGetProcAddress("glWindowPos2d")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glWindowPos2d proc address");

		if(!(gglWindowPos2dv = (void (APIENTRY *)(const GLdouble *p))gwglGetProcAddress("glWindowPos2dv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glWindowPos2dv proc address");

		if(!(gglWindowPos2f = (void (APIENTRY *)(GLfloat x, GLfloat y))gwglGetProcAddress("glWindowPos2f")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glWindowPos2f proc address");

		if(!(gglWindowPos2fv = (void (APIENTRY *)(const GLfloat *p))gwglGetProcAddress("glWindowPos2fv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glWindowPos2fv proc address");

		if(!(gglWindowPos2i = (void (APIENTRY *)(GLint x, GLint y))gwglGetProcAddress("glWindowPos2i")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glWindowPos2i proc address");

		if(!(gglWindowPos2iv = (void (APIENTRY *)(const GLint *p))gwglGetProcAddress("glWindowPos2iv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glWindowPos2iv proc address");

		if(!(gglWindowPos2s = (void (APIENTRY *)(GLshort x, GLshort y))gwglGetProcAddress("glWindowPos2s")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glWindowPos2s proc address");

		if(!(gglWindowPos2sv = (void (APIENTRY *)(const GLshort *p))gwglGetProcAddress("glWindowPos2sv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glWindowPos2sv proc address");

		if(!(gglWindowPos3d = (void (APIENTRY *)(GLdouble x, GLdouble y, GLdouble z))gwglGetProcAddress("glWindowPos3d")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glWindowPos3d proc address");

		if(!(gglWindowPos3dv = (void (APIENTRY *)(const GLdouble *p))gwglGetProcAddress("glWindowPos3dv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glWindowPos3dv proc address");

		if(!(gglWindowPos3f = (void (APIENTRY *)(GLfloat x, GLfloat y, GLfloat z))gwglGetProcAddress("glWindowPos3f")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glWindowPos3f proc address");

		if(!(gglWindowPos3fv = (void (APIENTRY *)(const GLfloat *p))gwglGetProcAddress("glWindowPos3fv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glWindowPos3fv proc address");

		if(!(gglWindowPos3i = (void (APIENTRY *)(GLint x, GLint y, GLint z))gwglGetProcAddress("glWindowPos3i")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glWindowPos3i proc address");

		if(!(gglWindowPos3iv = (void (APIENTRY *)(const GLint *p))gwglGetProcAddress("glWindowPos3iv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glWindowPos3iv proc address");

		if(!(gglWindowPos3s = (void (APIENTRY *)(GLshort x, GLshort y, GLshort z))gwglGetProcAddress("glWindowPos3s")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glWindowPos3s proc address");

		if(!(gglWindowPos3sv = (void (APIENTRY *)(const GLshort *p))gwglGetProcAddress("glWindowPos3sv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glWindowPos3sv proc address");
	}

	if(atof(version) >= 1.5)
	{
		if(!(gglGenQueries = (void (APIENTRY *)(GLsizei n, GLuint *ids))gwglGetProcAddress("glGenQueries")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glGenQueries proc address");

		if(!(gglDeleteQueries = (void (APIENTRY *)(GLsizei n, const GLuint *ids))gwglGetProcAddress("glDeleteQueries")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glDeleteQueries proc address");

		if(!(gglIsQuery = (GLboolean (APIENTRY *)(GLuint id))gwglGetProcAddress("glIsQuery")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glIsQuery proc address");

		if(!(gglBeginQuery = (void (APIENTRY *)(GLenum target, GLuint id))gwglGetProcAddress("glBeginQuery")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glBeginQuery proc address");

		if(!(gglEndQuery = (void (APIENTRY *)(GLenum target))gwglGetProcAddress("glEndQuery")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glEndQuery proc address");

		if(!(gglGetQueryiv = (void (APIENTRY *)(GLenum target, GLenum pname, GLint *params))gwglGetProcAddress("glGetQueryiv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glGetQueryiv proc address");

		if(!(gglGetQueryObjectiv = (void (APIENTRY *)(GLuint id, GLenum pname, GLint *params))gwglGetProcAddress("glGetQueryObjectiv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glGetQueryObjectiv proc address");

		if(!(gglGetQueryObjectuiv = (void (APIENTRY *)(GLuint id, GLenum pname, GLuint *params))gwglGetProcAddress("glGetQueryObjectuiv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glGetQueryObjectuiv proc address");

		if(!(gglBindBuffer = (void (APIENTRY *)(GLenum target, GLuint buffer))gwglGetProcAddress("glBindBuffer")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glBindBuffer proc address");

		if(!(gglDeleteBuffers = (void (APIENTRY *)(GLsizei n, const GLuint *buffers))gwglGetProcAddress("glDeleteBuffers")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glDeleteBuffers proc address");

		if(!(gglGenBuffers = (void (APIENTRY *)(GLsizei n, GLuint *buffers))gwglGetProcAddress("glGenBuffers")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glGenBuffers proc address");

		if(!(gglIsBuffer = (GLboolean (APIENTRY *)(GLuint buffer))gwglGetProcAddress("glIsBuffer")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glIsBuffer proc address");

		if(!(gglBufferData = (void (APIENTRY *)(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage))gwglGetProcAddress("glBufferData")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glBufferData proc address");

		if(!(gglBufferSubData = (void (APIENTRY *)(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data))gwglGetProcAddress("glBufferSubData")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glBufferSubData proc address");

		if(!(gglGetBufferSubData = (void (APIENTRY *)(GLenum target, GLintptr offset, GLsizeiptr size, GLvoid *data))gwglGetProcAddress("glGetBufferSubData")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glGetBufferSubData proc address");

		if(!(gglMapBuffer = (GLvoid* (APIENTRY *)(GLenum target, GLenum access))gwglGetProcAddress("glMapBuffer")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glMapBuffer proc address");

		if(!(gglUnmapBuffer = (GLboolean (APIENTRY *)(GLenum target))gwglGetProcAddress("glUnmapBuffer")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glUnmapBuffer proc address");

		if(!(gglGetBufferParameteriv = (void (APIENTRY *)(GLenum target, GLenum pname, GLint *params))gwglGetProcAddress("glGetBufferParameteriv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glGetBufferParameteriv proc address");

		if(!(gglGetBufferPointerv = (void (APIENTRY *)(GLenum target, GLenum pname, GLvoid* *params))gwglGetProcAddress("glGetBufferPointerv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glGetBufferPointerv proc address");
	}

	if(atof(version) >= 2.0)
	{
		if(!(gglBlendEquationSeparate = (void (APIENTRY *)(GLenum modeRGB, GLenum modeAlpha))gwglGetProcAddress("glBlendEquationSeparate")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glBlendEquationSeparate proc address");

		if(!(gglDrawBuffers = (void (APIENTRY *)(GLsizei n, const GLenum *bufs))gwglGetProcAddress("glDrawBuffers")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glDrawBuffers proc address");

		if(!(gglStencilOpSeparate = (void (APIENTRY *)(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass))gwglGetProcAddress("glStencilOpSeparate")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glStencilOpSeparate proc address");

		if(!(gglStencilFuncSeparate = (void (APIENTRY *)(GLenum frontfunc, GLenum backfunc, GLint ref, GLuint mask))gwglGetProcAddress("glStencilFuncSeparate")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glStencilFuncSeparate proc address");

		if(!(gglStencilMaskSeparate = (void (APIENTRY *)(GLenum face, GLuint mask))gwglGetProcAddress("glStencilMaskSeparate")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glStencilMaskSeparate proc address");

		if(!(gglAttachShader = (void (APIENTRY *)(GLuint program, GLuint shader))gwglGetProcAddress("glAttachShader")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glAttachShader proc address");

		if(!(gglBindAttribLocation = (void (APIENTRY *)(GLuint program, GLuint index, const GLchar *name))gwglGetProcAddress("glBindAttribLocation")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glBindAttribLocation proc address");

		if(!(gglCompileShader = (void (APIENTRY *)(GLuint shader))gwglGetProcAddress("glCompileShader")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glCompileShader proc address");

		if(!(gglCreateProgram = (GLuint (APIENTRY *)(void))gwglGetProcAddress("glCreateProgram")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glCreateProgram proc address");

		if(!(gglCreateShader = (GLuint (APIENTRY *)(GLenum type))gwglGetProcAddress("glCreateShader")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glCreateShader proc address");

		if(!(gglDeleteProgram = (void (APIENTRY *)(GLuint program))gwglGetProcAddress("glDeleteProgram")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glDeleteProgram proc address");

		if(!(gglDeleteShader = (void (APIENTRY *)(GLuint shader))gwglGetProcAddress("glDeleteShader")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glDeleteShader proc address");

		if(!(gglDetachShader = (void (APIENTRY *)(GLuint program, GLuint shader))gwglGetProcAddress("glDetachShader")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glDetachShader proc address");

		if(!(gglDisableVertexAttribArray = (void (APIENTRY *)(GLuint index))gwglGetProcAddress("glDisableVertexAttribArray")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glDisableVertexAttribArray proc address");

		if(!(gglEnableVertexAttribArray = (void (APIENTRY *)(GLuint index))gwglGetProcAddress("glEnableVertexAttribArray")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glEnableVertexAttribArray proc address");

		if(!(gglGetActiveAttrib = (void (APIENTRY *)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name))gwglGetProcAddress("glGetActiveAttrib")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glGetActiveAttrib proc address");

		if(!(gglGetActiveUniform = (void (APIENTRY *)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name))gwglGetProcAddress("glGetActiveUniform")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glGetActiveUniform proc address");

		if(!(gglGetAttachedShaders = (void (APIENTRY *)(GLuint program, GLsizei maxCount, GLsizei *count, GLuint *obj))gwglGetProcAddress("glGetAttachedShaders")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glGetAttachedShaders proc address");

		if(!(gglGetAttribLocation = (GLint (APIENTRY *)(GLuint program, const GLchar *name))gwglGetProcAddress("glGetAttribLocation")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glGetAttribLocation proc address");

		if(!(gglGetProgramiv = (void (APIENTRY *)(GLuint program, GLenum pname, GLint *params))gwglGetProcAddress("glGetProgramiv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glGetProgramiv proc address");

		if(!(gglGetProgramInfoLog = (void (APIENTRY *)(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog))gwglGetProcAddress("glGetProgramInfoLog")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glGetProgramInfoLog proc address");

		if(!(gglGetShaderiv = (void (APIENTRY *)(GLuint shader, GLenum pname, GLint *params))gwglGetProcAddress("glGetShaderiv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glGetShaderiv proc address");

		if(!(gglGetShaderInfoLog = (void (APIENTRY *)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog))gwglGetProcAddress("glGetShaderInfoLog")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glGetShaderInfoLog proc address");

		if(!(gglGetShaderSource = (void (APIENTRY *)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source))gwglGetProcAddress("glGetShaderSource")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glGetShaderSource proc address");

		if(!(gglGetUniformLocation = (GLint (APIENTRY *)(GLuint program, const GLchar *name))gwglGetProcAddress("glGetUniformLocation")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glGetUniformLocation proc address");

		if(!(gglGetUniformfv = (void (APIENTRY *)(GLuint program, GLint location, GLfloat *params))gwglGetProcAddress("glGetUniformfv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glGetUniformfv proc address");

		if(!(gglGetUniformiv = (void (APIENTRY *)(GLuint program, GLint location, GLint *params))gwglGetProcAddress("glGetUniformiv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glGetUniformiv proc address");

		if(!(gglGetVertexAttribdv = (void (APIENTRY *)(GLuint index, GLenum pname, GLdouble *params))gwglGetProcAddress("glGetVertexAttribdv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glGetVertexAttribdv proc address");

		if(!(gglGetVertexAttribfv = (void (APIENTRY *)(GLuint index, GLenum pname, GLfloat *params))gwglGetProcAddress("glGetVertexAttribfv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glGetVertexAttribfv proc address");

		if(!(gglGetVertexAttribiv = (void (APIENTRY *)(GLuint index, GLenum pname, GLint *params))gwglGetProcAddress("glGetVertexAttribiv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glGetVertexAttribiv proc address");

		if(!(gglGetVertexAttribPointerv = (void (APIENTRY *)(GLuint index, GLenum pname, GLvoid* *pointer))gwglGetProcAddress("glGetVertexAttribPointerv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glGetVertexAttribPointerv proc address");

		if(!(gglIsProgram = (GLboolean (APIENTRY *)(GLuint program))gwglGetProcAddress("glIsProgram")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glIsProgram proc address");

		if(!(gglIsShader = (GLboolean (APIENTRY *)(GLuint shader))gwglGetProcAddress("glIsShader")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glIsShader proc address");

		if(!(gglLinkProgram = (void (APIENTRY *)(GLuint program))gwglGetProcAddress("glLinkProgram")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glLinkProgram proc address");

		if(!(gglShaderSource = (void (APIENTRY *)(GLuint shader, GLsizei count, const GLchar* *string, const GLint *length))gwglGetProcAddress("glShaderSource")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glShaderSource proc address");

		if(!(gglUseProgram = (void (APIENTRY *)(GLuint program))gwglGetProcAddress("glUseProgram")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glUseProgram proc address");

		if(!(gglUniform1f = (void (APIENTRY *)(GLint location, GLfloat v0))gwglGetProcAddress("glUniform1f")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glUniform1f proc address");

		if(!(gglUniform2f = (void (APIENTRY *)(GLint location, GLfloat v0, GLfloat v1))gwglGetProcAddress("glUniform2f")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glUniform2f proc address");

		if(!(gglUniform3f = (void (APIENTRY *)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2))gwglGetProcAddress("glUniform3f")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glUniform3f proc address");

		if(!(gglUniform4f = (void (APIENTRY *)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3))gwglGetProcAddress("glUniform4f")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glUniform4f proc address");

		if(!(gglUniform1i = (void (APIENTRY *)(GLint location, GLint v0))gwglGetProcAddress("glUniform1i")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glUniform1i proc address");

		if(!(gglUniform2i = (void (APIENTRY *)(GLint location, GLint v0, GLint v1))gwglGetProcAddress("glUniform2i")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glUniform2i proc address");

		if(!(gglUniform3i = (void (APIENTRY *)(GLint location, GLint v0, GLint v1, GLint v2))gwglGetProcAddress("glUniform3i")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glUniform3i proc address");

		if(!(gglUniform4i = (void (APIENTRY *)(GLint location, GLint v0, GLint v1, GLint v2, GLint v3))gwglGetProcAddress("glUniform4i")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glUniform4i proc address");

		if(!(gglUniform1fv = (void (APIENTRY *)(GLint location, GLsizei count, const GLfloat *value))gwglGetProcAddress("glUniform1fv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glUniform1fv proc address");

		if(!(gglUniform2fv = (void (APIENTRY *)(GLint location, GLsizei count, const GLfloat *value))gwglGetProcAddress("glUniform2fv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glUniform2fv proc address");

		if(!(gglUniform3fv = (void (APIENTRY *)(GLint location, GLsizei count, const GLfloat *value))gwglGetProcAddress("glUniform3fv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glUniform3fv proc address");

		if(!(gglUniform4fv = (void (APIENTRY *)(GLint location, GLsizei count, const GLfloat *value))gwglGetProcAddress("glUniform4fv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glUniform4fv proc address");

		if(!(gglUniform1iv = (void (APIENTRY *)(GLint location, GLsizei count, const GLint *value))gwglGetProcAddress("glUniform1iv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glUniform1iv proc address");

		if(!(gglUniform2iv = (void (APIENTRY *)(GLint location, GLsizei count, const GLint *value))gwglGetProcAddress("glUniform2iv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glUniform2iv proc address");

		if(!(gglUniform3iv = (void (APIENTRY *)(GLint location, GLsizei count, const GLint *value))gwglGetProcAddress("glUniform3iv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glUniform3iv proc address");

		if(!(gglUniform4iv = (void (APIENTRY *)(GLint location, GLsizei count, const GLint *value))gwglGetProcAddress("glUniform4iv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glUniform4iv proc address");

		if(!(gglUniformMatrix2fv = (void (APIENTRY *)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value))gwglGetProcAddress("glUniformMatrix2fv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glUniformMatrix2fv proc address");

		if(!(gglUniformMatrix3fv = (void (APIENTRY *)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value))gwglGetProcAddress("glUniformMatrix3fv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glUniformMatrix3fv proc address");

		if(!(gglUniformMatrix4fv = (void (APIENTRY *)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value))gwglGetProcAddress("glUniformMatrix4fv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glUniformMatrix4fv proc address");

		if(!(gglValidateProgram = (void (APIENTRY *)(GLuint program))gwglGetProcAddress("glValidateProgram")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glValidateProgram proc address");

		if(!(gglVertexAttrib1d = (void (APIENTRY *)(GLuint index, GLdouble x))gwglGetProcAddress("glVertexAttrib1d")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttrib1d proc address");

		if(!(gglVertexAttrib1dv = (void (APIENTRY *)(GLuint index, const GLdouble *v))gwglGetProcAddress("glVertexAttrib1dv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttrib1dv proc address");

		if(!(gglVertexAttrib1f = (void (APIENTRY *)(GLuint index, GLfloat x))gwglGetProcAddress("glVertexAttrib1f")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttrib1f proc address");

		if(!(gglVertexAttrib1fv = (void (APIENTRY *)(GLuint index, const GLfloat *v))gwglGetProcAddress("glVertexAttrib1fv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttrib1fv proc address");

		if(!(gglVertexAttrib1s = (void (APIENTRY *)(GLuint index, GLshort x))gwglGetProcAddress("glVertexAttrib1s")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttrib1s proc address");

		if(!(gglVertexAttrib1sv = (void (APIENTRY *)(GLuint index, const GLshort *v))gwglGetProcAddress("glVertexAttrib1sv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttrib1sv proc address");

		if(!(gglVertexAttrib2d = (void (APIENTRY *)(GLuint index, GLdouble x, GLdouble y))gwglGetProcAddress("glVertexAttrib2d")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttrib2d proc address");

		if(!(gglVertexAttrib2dv = (void (APIENTRY *)(GLuint index, const GLdouble *v))gwglGetProcAddress("glVertexAttrib2dv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttrib2dv proc address");

		if(!(gglVertexAttrib2f = (void (APIENTRY *)(GLuint index, GLfloat x, GLfloat y))gwglGetProcAddress("glVertexAttrib2f")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttrib2f proc address");

		if(!(gglVertexAttrib2fv = (void (APIENTRY *)(GLuint index, const GLfloat *v))gwglGetProcAddress("glVertexAttrib2fv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttrib2fv proc address");

		if(!(gglVertexAttrib2s = (void (APIENTRY *)(GLuint index, GLshort x, GLshort y))gwglGetProcAddress("glVertexAttrib2s")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttrib2s proc address");

		if(!(gglVertexAttrib2sv = (void (APIENTRY *)(GLuint index, const GLshort *v))gwglGetProcAddress("glVertexAttrib2sv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttrib2sv proc address");

		if(!(gglVertexAttrib3d = (void (APIENTRY *)(GLuint index, GLdouble x, GLdouble y, GLdouble z))gwglGetProcAddress("glVertexAttrib3d")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttrib3d proc address");

		if(!(gglVertexAttrib3dv = (void (APIENTRY *)(GLuint index, const GLdouble *v))gwglGetProcAddress("glVertexAttrib3dv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttrib3dv proc address");

		if(!(gglVertexAttrib3f = (void (APIENTRY *)(GLuint index, GLfloat x, GLfloat y, GLfloat z))gwglGetProcAddress("glVertexAttrib3f")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttrib3f proc address");

		if(!(gglVertexAttrib3fv = (void (APIENTRY *)(GLuint index, const GLfloat *v))gwglGetProcAddress("glVertexAttrib3fv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttrib3fv proc address");

		if(!(gglVertexAttrib3s = (void (APIENTRY *)(GLuint index, GLshort x, GLshort y, GLshort z))gwglGetProcAddress("glVertexAttrib3s")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttrib3s proc address");

		if(!(gglVertexAttrib3sv = (void (APIENTRY *)(GLuint index, const GLshort *v))gwglGetProcAddress("glVertexAttrib3sv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttrib3sv proc address");

		if(!(gglVertexAttrib4Nbv = (void (APIENTRY *)(GLuint index, const GLbyte *v))gwglGetProcAddress("glVertexAttrib4Nbv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttrib4Nbv proc address");

		if(!(gglVertexAttrib4Niv = (void (APIENTRY *)(GLuint index, const GLint *v))gwglGetProcAddress("glVertexAttrib4Niv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttrib4Niv proc address");

		if(!(gglVertexAttrib4Nsv = (void (APIENTRY *)(GLuint index, const GLshort *v))gwglGetProcAddress("glVertexAttrib4Nsv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttrib4Nsv proc address");

		if(!(gglVertexAttrib4Nub = (void (APIENTRY *)(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w))gwglGetProcAddress("glVertexAttrib4Nub")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttrib4Nub proc address");

		if(!(gglVertexAttrib4Nubv = (void (APIENTRY *)(GLuint index, const GLubyte *v))gwglGetProcAddress("glVertexAttrib4Nubv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttrib4Nubv proc address");

		if(!(gglVertexAttrib4Nuiv = (void (APIENTRY *)(GLuint index, const GLuint *v))gwglGetProcAddress("glVertexAttrib4Nuiv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttrib4Nuiv proc address");

		if(!(gglVertexAttrib4Nusv = (void (APIENTRY *)(GLuint index, const GLushort *v))gwglGetProcAddress("glVertexAttrib4Nusv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttrib4Nusv proc address");

		if(!(gglVertexAttrib4bv = (void (APIENTRY *)(GLuint index, const GLbyte *v))gwglGetProcAddress("glVertexAttrib4bv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttrib4bv proc address");

		if(!(gglVertexAttrib4d = (void (APIENTRY *)(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w))gwglGetProcAddress("glVertexAttrib4d")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttrib4d proc address");

		if(!(gglVertexAttrib4dv = (void (APIENTRY *)(GLuint index, const GLdouble *v))gwglGetProcAddress("glVertexAttrib4dv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttrib4dv proc address");

		if(!(gglVertexAttrib4f = (void (APIENTRY *)(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w))gwglGetProcAddress("glVertexAttrib4f")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttrib4f proc address");

		if(!(gglVertexAttrib4fv = (void (APIENTRY *)(GLuint index, const GLfloat *v))gwglGetProcAddress("glVertexAttrib4fv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttrib4fv proc address");

		if(!(gglVertexAttrib4iv = (void (APIENTRY *)(GLuint index, const GLint *v))gwglGetProcAddress("glVertexAttrib4iv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttrib4iv proc address");

		if(!(gglVertexAttrib4s = (void (APIENTRY *)(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w))gwglGetProcAddress("glVertexAttrib4s")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttrib4s proc address");

		if(!(gglVertexAttrib4sv = (void (APIENTRY *)(GLuint index, const GLshort *v))gwglGetProcAddress("glVertexAttrib4sv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttrib4sv proc address");

		if(!(gglVertexAttrib4ubv = (void (APIENTRY *)(GLuint index, const GLubyte *v))gwglGetProcAddress("glVertexAttrib4ubv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttrib4ubv proc address");

		if(!(gglVertexAttrib4uiv = (void (APIENTRY *)(GLuint index, const GLuint *v))gwglGetProcAddress("glVertexAttrib4uiv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttrib4uiv proc address");

		if(!(gglVertexAttrib4usv = (void (APIENTRY *)(GLuint index, const GLushort *v))gwglGetProcAddress("glVertexAttrib4usv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttrib4usv proc address");

		if(!(gglVertexAttribPointer = (void (APIENTRY *)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer))gwglGetProcAddress("glVertexAttribPointer")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttribPointer proc address");
	}

	if(atof(version) >= 2.1)
	{
		if(!(gglUniformMatrix2x3fv = (void (APIENTRY *)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value))gwglGetProcAddress("glUniformMatrix2x3fv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glUniformMatrix2x3fv proc address");

		if(!(gglUniformMatrix3x2fv = (void (APIENTRY *)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value))gwglGetProcAddress("glUniformMatrix3x2fv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glUniformMatrix3x2fv proc address");

		if(!(gglUniformMatrix2x4fv = (void (APIENTRY *)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value))gwglGetProcAddress("glUniformMatrix2x4fv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glUniformMatrix2x4fv proc address");

		if(!(gglUniformMatrix4x2fv = (void (APIENTRY *)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value))gwglGetProcAddress("glUniformMatrix4x2fv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glUniformMatrix4x2fv proc address");

		if(!(gglUniformMatrix3x4fv = (void (APIENTRY *)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value))gwglGetProcAddress("glUniformMatrix3x4fv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glUniformMatrix3x4fv proc address");

		if(!(gglUniformMatrix4x3fv = (void (APIENTRY *)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value))gwglGetProcAddress("glUniformMatrix4x3fv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glUniformMatrix4x3fv proc address");
	}

	if(atof(version) >= 3.0)
	{
		if(!(gglColorMaski = (void (APIENTRY *)(GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a))gwglGetProcAddress("glColorMaski")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glColorMaski proc address");

		if(!(gglGetBooleani_v = (void (APIENTRY *)(GLenum target, GLuint index, GLboolean *data))gwglGetProcAddress("glGetBooleani_v")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glGetBooleani_v proc address");

		if(!(gglGetIntegeri_v = (void (APIENTRY *)(GLenum target, GLuint index, GLint *data))gwglGetProcAddress("glGetIntegeri_v")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glGetIntegeri_v proc address");

		if(!(gglEnablei = (void (APIENTRY *)(GLenum target, GLuint index))gwglGetProcAddress("glEnablei")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glEnablei proc address");

		if(!(gglDisablei = (void (APIENTRY *)(GLenum target, GLuint index))gwglGetProcAddress("glDisablei")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glDisablei proc address");

		if(!(gglIsEnabledi = (GLboolean (APIENTRY *)(GLenum target, GLuint index))gwglGetProcAddress("glIsEnabledi")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glIsEnabledi proc address");

		if(!(gglBeginTransformFeedback = (void (APIENTRY *)(GLenum primitiveMode))gwglGetProcAddress("glBeginTransformFeedback")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glBeginTransformFeedback proc address");

		if(!(gglEndTransformFeedback = (void (APIENTRY *)(void))gwglGetProcAddress("glEndTransformFeedback")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glEndTransformFeedback proc address");

		if(!(gglBindBufferRange = (void (APIENTRY *)(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size))gwglGetProcAddress("glBindBufferRange")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glBindBufferRange proc address");

		if(!(gglBindBufferBase = (void (APIENTRY *)(GLenum target, GLuint index, GLuint buffer))gwglGetProcAddress("glBindBufferBase")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glBindBufferBase proc address");

		if(!(gglTransformFeedbackVaryings = (void (APIENTRY *)(GLuint program, GLsizei count, const GLint *locations, GLenum bufferMode))gwglGetProcAddress("glTransformFeedbackVaryings")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glTransformFeedbackVaryings proc address");

		if(!(gglGetTransformFeedbackVarying = (void (APIENTRY *)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name))gwglGetProcAddress("glGetTransformFeedbackVarying")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glGetTransformFeedbackVarying proc address");

		if(!(gglClampColor = (void (APIENTRY *)(GLenum target, GLenum clamp))gwglGetProcAddress("glClampColor")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glClampColor proc address");

		if(!(gglBeginConditionalRender = (void (APIENTRY *)(GLuint id, GLenum mode))gwglGetProcAddress("glBeginConditionalRender")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glBeginConditionalRender proc address");

		if(!(gglEndConditionalRender = (void (APIENTRY *)(void))gwglGetProcAddress("glEndConditionalRender")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glEndConditionalRender proc address");

		if(!(gglVertexAttribI1i = (void (APIENTRY *)(GLuint index, GLint x))gwglGetProcAddress("glVertexAttribI1i")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttribI1i proc address");

		if(!(gglVertexAttribI2i = (void (APIENTRY *)(GLuint index, GLint x, GLint y))gwglGetProcAddress("glVertexAttribI2i")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttribI2i proc address");

		if(!(gglVertexAttribI3i = (void (APIENTRY *)(GLuint index, GLint x, GLint y, GLint z))gwglGetProcAddress("glVertexAttribI3i")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttribI3i proc address");

		if(!(gglVertexAttribI4i = (void (APIENTRY *)(GLuint index, GLint x, GLint y, GLint z, GLint w))gwglGetProcAddress("glVertexAttribI4i")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttribI4i proc address");

		if(!(gglVertexAttribI1ui = (void (APIENTRY *)(GLuint index, GLuint x))gwglGetProcAddress("glVertexAttribI1ui")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttribI1ui proc address");

		if(!(gglVertexAttribI2ui = (void (APIENTRY *)(GLuint index, GLuint x, GLuint y))gwglGetProcAddress("glVertexAttribI2ui")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttribI2ui proc address");

		if(!(gglVertexAttribI3ui = (void (APIENTRY *)(GLuint index, GLuint x, GLuint y, GLuint z))gwglGetProcAddress("glVertexAttribI3ui")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttribI3ui proc address");

		if(!(gglVertexAttribI4ui = (void (APIENTRY *)(GLuint index, GLuint x, GLuint y, GLuint z, GLuint w))gwglGetProcAddress("glVertexAttribI4ui")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttribI4ui proc address");

		if(!(gglVertexAttribI1iv = (void (APIENTRY *)(GLuint index, const GLint *v))gwglGetProcAddress("glVertexAttribI1iv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttribI1iv proc address");

		if(!(gglVertexAttribI2iv = (void (APIENTRY *)(GLuint index, const GLint *v))gwglGetProcAddress("glVertexAttribI2iv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttribI2iv proc address");

		if(!(gglVertexAttribI3iv = (void (APIENTRY *)(GLuint index, const GLint *v))gwglGetProcAddress("glVertexAttribI3iv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttribI3iv proc address");

		if(!(gglVertexAttribI4iv = (void (APIENTRY *)(GLuint index, const GLint *v))gwglGetProcAddress("glVertexAttribI4iv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttribI4iv proc address");

		if(!(gglVertexAttribI1uiv = (void (APIENTRY *)(GLuint index, const GLuint *v))gwglGetProcAddress("glVertexAttribI1uiv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttribI1uiv proc address");

		if(!(gglVertexAttribI2uiv = (void (APIENTRY *)(GLuint index, const GLuint *v))gwglGetProcAddress("glVertexAttribI2uiv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttribI2uiv proc address");

		if(!(gglVertexAttribI3uiv = (void (APIENTRY *)(GLuint index, const GLuint *v))gwglGetProcAddress("glVertexAttribI3uiv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttribI3uiv proc address");

		if(!(gglVertexAttribI4uiv = (void (APIENTRY *)(GLuint index, const GLuint *v))gwglGetProcAddress("glVertexAttribI4uiv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttribI4uiv proc address");

		if(!(gglVertexAttribI4bv = (void (APIENTRY *)(GLuint index, const GLbyte *v))gwglGetProcAddress("glVertexAttribI4bv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttribI4bv proc address");

		if(!(gglVertexAttribI4sv = (void (APIENTRY *)(GLuint index, const GLshort *v))gwglGetProcAddress("glVertexAttribI4sv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttribI4sv proc address");

		if(!(gglVertexAttribI4ubv = (void (APIENTRY *)(GLuint index, const GLubyte *v))gwglGetProcAddress("glVertexAttribI4ubv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttribI4ubv proc address");

		if(!(gglVertexAttribI4usv = (void (APIENTRY *)(GLuint index, const GLushort *v))gwglGetProcAddress("glVertexAttribI4usv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttribI4usv proc address");

		if(!(gglVertexAttribIPointer = (void (APIENTRY *)(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer))gwglGetProcAddress("glVertexAttribIPointer")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glVertexAttribIPointer proc address");

		if(!(gglGetVertexAttribIiv = (void (APIENTRY *)(GLuint index, GLenum pname, GLint *params))gwglGetProcAddress("glGetVertexAttribIiv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glGetVertexAttribIiv proc address");

		if(!(gglGetVertexAttribIuiv = (void (APIENTRY *)(GLuint index, GLenum pname, GLuint *params))gwglGetProcAddress("glGetVertexAttribIuiv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glGetVertexAttribIuiv proc address");

		if(!(gglGetUniformuiv = (void (APIENTRY *)(GLuint program, GLint location, GLuint *params))gwglGetProcAddress("glGetUniformuiv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glGetUniformuiv proc address");

		if(!(gglBindFragDataLocation = (void (APIENTRY *)(GLuint program, GLuint color, const GLchar *name))gwglGetProcAddress("glBindFragDataLocation")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glBindFragDataLocation proc address");

		if(!(gglGetFragDataLocation = (GLint (APIENTRY *)(GLuint program, const GLchar *name))gwglGetProcAddress("glGetFragDataLocation")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glGetFragDataLocation proc address");

		if(!(gglUniform1ui = (void (APIENTRY *)(GLint location, GLuint v0))gwglGetProcAddress("glUniform1ui")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glUniform1ui proc address");

		if(!(gglUniform2ui = (void (APIENTRY *)(GLint location, GLuint v0, GLuint v1))gwglGetProcAddress("glUniform2ui")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glUniform2ui proc address");

		if(!(gglUniform3ui = (void (APIENTRY *)(GLint location, GLuint v0, GLuint v1, GLuint v2))gwglGetProcAddress("glUniform3ui")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glUniform3ui proc address");

		if(!(gglUniform4ui = (void (APIENTRY *)(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3))gwglGetProcAddress("glUniform4ui")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glUniform4ui proc address");

		if(!(gglUniform1uiv = (void (APIENTRY *)(GLint location, GLsizei count, const GLuint *value))gwglGetProcAddress("glUniform1uiv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glUniform1uiv proc address");

		if(!(gglUniform2uiv = (void (APIENTRY *)(GLint location, GLsizei count, const GLuint *value))gwglGetProcAddress("glUniform2uiv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glUniform2uiv proc address");

		if(!(gglUniform3uiv = (void (APIENTRY *)(GLint location, GLsizei count, const GLuint *value))gwglGetProcAddress("glUniform3uiv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glUniform3uiv proc address");

		if(!(gglUniform4uiv = (void (APIENTRY *)(GLint location, GLsizei count, const GLuint *value))gwglGetProcAddress("glUniform4uiv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glUniform4uiv proc address");

		if(!(gglTexParameterIiv = (void (APIENTRY *)(GLenum target, GLenum pname, const GLint *params))gwglGetProcAddress("glTexParameterIiv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glTexParameterIiv proc address");

		if(!(gglTexParameterIuiv = (void (APIENTRY *)(GLenum target, GLenum pname, const GLuint *params))gwglGetProcAddress("glTexParameterIuiv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glTexParameterIuiv proc address");

		if(!(gglGetTexParameterIiv = (void (APIENTRY *)(GLenum target, GLenum pname, GLint *params))gwglGetProcAddress("glGetTexParameterIiv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glGetTexParameterIiv proc address");

		if(!(gglGetTexParameterIuiv = (void (APIENTRY *)(GLenum target, GLenum pname, GLuint *params))gwglGetProcAddress("glGetTexParameterIuiv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glGetTexParameterIuiv proc address");

		if(!(gglClearBufferiv = (void (APIENTRY *)(GLenum buffer, GLint drawbuffer, const GLint *value))gwglGetProcAddress("glClearBufferiv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glClearBufferiv proc address");

		if(!(gglClearBufferuiv = (void (APIENTRY *)(GLenum buffer, GLint drawbuffer, const GLuint *value))gwglGetProcAddress("glClearBufferuiv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glClearBufferuiv proc address");

		if(!(gglClearBufferfv = (void (APIENTRY *)(GLenum buffer, GLint drawbuffer, const GLfloat *value))gwglGetProcAddress("glClearBufferfv")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glClearBufferfv proc address");

		if(!(gglClearBufferfi = (void (APIENTRY *)(GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil))gwglGetProcAddress("glClearBufferfi")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glClearBufferfi proc address");

		if(!(gglGetStringi = (const GLubyte *(APIENTRY *)(GLenum name, GLuint index))gwglGetProcAddress("glGetStringi")))
			OpenGL_Error("OpenGL_EnableExtensions: Couldn't get glGetStringi proc address");
	}
}

/*
----------------
OpenGL_DisableExtensions
----------------
*/
void OpenGL_DisableExtensions(void)
{
	gwglCreateBufferRegionARB = NULL;
	gwglDeleteBufferRegionARB = NULL;
	gwglSaveBufferRegionARB = NULL;
	gwglRestoreBufferRegionARB = NULL;
	gwglGetExtensionsStringARB = NULL;
	gwglGetPixelFormatAttribivARB = NULL;
	gwglGetPixelFormatAttribfvARB = NULL;
	gwglChoosePixelFormatARB = NULL;
	gwglMakeContextCurrentARB = NULL;
	gwglGetCurrentReadDCARB = NULL;
	gwglCreatePbufferARB = NULL;
	gwglGetPbufferDCARB = NULL;
	gwglReleasePbufferDCARB = NULL;
	gwglDestroyPbufferARB = NULL;
	gwglQueryPbufferARB = NULL;
	gwglBindTexImageARB = NULL;
	gwglReleaseTexImageARB = NULL;
	gwglSetPbufferAttribARB = NULL;
	gwglCreateContextAttribsARB = NULL;
	gwglCreateDisplayColorTableEXT = NULL;
	gwglLoadDisplayColorTableEXT = NULL;
	gwglBindDisplayColorTableEXT = NULL;
	gwglDestroyDisplayColorTableEXT = NULL;
	gwglGetExtensionsStringEXT = NULL;
	gwglMakeContextCurrentEXT = NULL;
	gwglGetCurrentReadDCEXT = NULL;
	gwglGetPixelFormatAttribivEXT = NULL;
	gwglGetPixelFormatAttribfvEXT = NULL;
	gwglChoosePixelFormatEXT = NULL;
	gwglCreatePbufferEXT = NULL;
	gwglGetPbufferDCEXT = NULL;
	gwglReleasePbufferDCEXT = NULL;
	gwglDestroyPbufferEXT = NULL;
	gwglQueryPbufferEXT = NULL;
	gwglSwapIntervalEXT = NULL;
	gwglGetSwapIntervalEXT = NULL;
	gwglAllocateMemoryNV = NULL;
	gwglFreeMemoryNV = NULL;
	gwglGetSyncValuesOML = NULL;
	gwglGetMscRateOML = NULL;
	gwglSwapBuffersMscOML = NULL;
	gwglSwapLayerBuffersMscOML = NULL;
	gwglWaitForMscOML = NULL;
	gwglWaitForSbcOML = NULL;
	gwglGetDigitalVideoParametersI3D = NULL;
	gwglSetDigitalVideoParametersI3D = NULL;
	gwglGetGammaTableParametersI3D = NULL;
	gwglSetGammaTableParametersI3D = NULL;
	gwglGetGammaTableI3D = NULL;
	gwglSetGammaTableI3D = NULL;
	gwglEnableGenlockI3D = NULL;
	gwglDisableGenlockI3D = NULL;
	gwglIsEnabledGenlockI3D = NULL;
	gwglGenlockSourceI3D = NULL;
	gwglGetGenlockSourceI3D = NULL;
	gwglGenlockSourceEdgeI3D = NULL;
	gwglGetGenlockSourceEdgeI3D = NULL;
	gwglGenlockSampleRateI3D = NULL;
	gwglGetGenlockSampleRateI3D = NULL;
	gwglGenlockSourceDelayI3D = NULL;
	gwglGetGenlockSourceDelayI3D = NULL;
	gwglQueryGenlockMaxSourceDelayI3D = NULL;
	gwglCreateImageBufferI3D = NULL;
	gwglDestroyImageBufferI3D = NULL;
	gwglAssociateImageBufferEventsI3D = NULL;
	gwglReleaseImageBufferEventsI3D = NULL;
	gwglEnableFrameLockI3D = NULL;
	gwglDisableFrameLockI3D = NULL;
	gwglIsEnabledFrameLockI3D = NULL;
	gwglQueryFrameLockMasterI3D = NULL;
	gwglGetFrameUsageI3D = NULL;
	gwglBeginFrameTrackingI3D = NULL;
	gwglEndFrameTrackingI3D = NULL;
	gwglQueryFrameTrackingI3D = NULL;
	gwglSetStereoEmitterState3DL = NULL;

	gglAreTexturesResident = NULL;
	gglArrayElement = NULL;
	gglBindTexture = NULL;
	gglColorPointer = NULL;
	gglCopyTexImage1D = NULL;
	gglCopyTexImage2D = NULL;
	gglCopyTexSubImage1D = NULL;
	gglCopyTexSubImage2D = NULL;
	gglDeleteTextures = NULL;
	gglDisableClientState = NULL;
	gglDrawArrays = NULL;
	gglDrawElements = NULL;
	gglEdgeFlagPointer = NULL;
	gglEnableClientState = NULL;
	gglGenTextures = NULL;
	gglGetPointerv = NULL;
	gglIndexub = NULL;
	gglIndexubv = NULL;
	gglIndexPointer = NULL;
	gglInterleavedArrays = NULL;
	gglIsTexture = NULL;
	gglNormalPointer = NULL;
	gglPolygonOffset = NULL;
	gglPopClientAttrib = NULL;
	gglPrioritizeTextures = NULL;
	gglPushClientAttrib = NULL;
	gglTexCoordPointer = NULL;
	gglTexSubImage1D = NULL;
	gglTexSubImage2D = NULL;
	gglVertexPointer = NULL;

	gglDrawRangeElements = NULL;
	gglTexImage3D = NULL;
	gglTexSubImage3D = NULL;
	gglCopyTexSubImage3D = NULL;
	
	gglBlendColor = NULL;
	gglBlendEquation = NULL;
	gglColorTable = NULL;
	gglColorTableParameterfv = NULL;
	gglColorTableParameteriv = NULL;
	gglCopyColorTable = NULL;
	gglGetColorTable = NULL;
	gglGetColorTableParameterfv = NULL;
	gglGetColorTableParameteriv = NULL;
	gglColorSubTable = NULL;
	gglCopyColorSubTable = NULL;
	gglConvolutionFilter1D = NULL;
	gglConvolutionFilter2D = NULL;
	gglConvolutionParameterf = NULL;
	gglConvolutionParameterfv = NULL;
	gglConvolutionParameteri = NULL;
	gglConvolutionParameteriv = NULL;
	gglCopyConvolutionFilter1D = NULL;
	gglCopyConvolutionFilter2D = NULL;
	gglGetConvolutionFilter = NULL;
	gglGetConvolutionParameterfv = NULL;
	gglGetConvolutionParameteriv = NULL;
	gglGetSeparableFilter = NULL;
	gglSeparableFilter2D = NULL;
	gglGetHistogram = NULL;
	gglGetHistogramParameterfv = NULL;
	gglGetHistogramParameteriv = NULL;
	gglGetMinmax = NULL;
	gglGetMinmaxParameterfv = NULL;
	gglGetMinmaxParameteriv = NULL;
	gglHistogram = NULL;
	gglMinmax = NULL;
	gglResetHistogram = NULL;
	gglResetMinmax = NULL;

	gglActiveTexture = NULL;
	gglClientActiveTexture = NULL;
	gglMultiTexCoord1d = NULL;
	gglMultiTexCoord1dv = NULL;
	gglMultiTexCoord1f = NULL;
	gglMultiTexCoord1fv = NULL;
	gglMultiTexCoord1i = NULL;
	gglMultiTexCoord1iv = NULL;
	gglMultiTexCoord1s = NULL;
	gglMultiTexCoord1sv = NULL;
	gglMultiTexCoord2d = NULL;
	gglMultiTexCoord2dv = NULL;
	gglMultiTexCoord2f = NULL;
	gglMultiTexCoord2fv = NULL;
	gglMultiTexCoord2i = NULL;
	gglMultiTexCoord2iv = NULL;
	gglMultiTexCoord2s = NULL;
	gglMultiTexCoord2sv = NULL;
	gglMultiTexCoord3d = NULL;
	gglMultiTexCoord3dv = NULL;
	gglMultiTexCoord3f = NULL;
	gglMultiTexCoord3fv = NULL;
	gglMultiTexCoord3i = NULL;
	gglMultiTexCoord3iv = NULL;
	gglMultiTexCoord3s = NULL;
	gglMultiTexCoord3sv = NULL;
	gglMultiTexCoord4d = NULL;
	gglMultiTexCoord4dv = NULL;
	gglMultiTexCoord4f = NULL;
	gglMultiTexCoord4fv = NULL;
	gglMultiTexCoord4i = NULL;
	gglMultiTexCoord4iv = NULL;
	gglMultiTexCoord4s = NULL;
	gglMultiTexCoord4sv = NULL;
	gglLoadTransposeMatrixf = NULL;
	gglLoadTransposeMatrixd = NULL;
	gglMultTransposeMatrixf = NULL;
	gglMultTransposeMatrixd = NULL;
	gglSampleCoverage = NULL;
	gglCompressedTexImage3D = NULL;
	gglCompressedTexImage2D = NULL;
	gglCompressedTexImage1D = NULL;
	gglCompressedTexSubImage3D = NULL;
	gglCompressedTexSubImage2D = NULL;
	gglCompressedTexSubImage1D = NULL;
	gglGetCompressedTexImage = NULL;

	gglFogCoordf = NULL;
	gglFogCoordfv = NULL;
	gglFogCoordd = NULL;
	gglFogCoorddv = NULL;
	gglFogCoordPointer = NULL;
	gglMultiDrawArrays = NULL;
	gglMultiDrawElements = NULL;
	gglPointParameterf = NULL;
	gglPointParameterfv = NULL;
	gglSecondaryColor3b = NULL;
	gglSecondaryColor3bv = NULL;
	gglSecondaryColor3d = NULL;
	gglSecondaryColor3dv = NULL;
	gglSecondaryColor3f = NULL;
	gglSecondaryColor3fv = NULL;
	gglSecondaryColor3i = NULL;
	gglSecondaryColor3iv = NULL;
	gglSecondaryColor3s = NULL;
	gglSecondaryColor3sv = NULL;
	gglSecondaryColor3ub = NULL;
	gglSecondaryColor3ubv = NULL;
	gglSecondaryColor3ui = NULL;
	gglSecondaryColor3uiv = NULL;
	gglSecondaryColor3us = NULL;
	gglSecondaryColor3usv = NULL;
	gglSecondaryColorPointer = NULL;
	gglBlendFuncSeparate = NULL;
	gglWindowPos2d = NULL;
	gglWindowPos2f = NULL;
	gglWindowPos2i = NULL;
	gglWindowPos2s = NULL;
	gglWindowPos2dv = NULL;
	gglWindowPos2fv = NULL;
	gglWindowPos2iv = NULL;
	gglWindowPos2sv = NULL;
	gglWindowPos3d = NULL;
	gglWindowPos3f = NULL;
	gglWindowPos3i = NULL;
	gglWindowPos3s = NULL;
	gglWindowPos3dv = NULL;
	gglWindowPos3fv = NULL;
	gglWindowPos3iv = NULL;
	gglWindowPos3sv = NULL;

	gglGenQueries = NULL;
	gglDeleteQueries = NULL;
	gglIsQuery = NULL;
	gglBeginQuery = NULL;
	gglEndQuery = NULL;
	gglGetQueryiv = NULL;
	gglGetQueryObjectiv = NULL;
	gglGetQueryObjectuiv = NULL;
	gglBindBuffer = NULL;
	gglDeleteBuffers = NULL;
	gglGenBuffers = NULL;
	gglIsBuffer = NULL;
	gglBufferData = NULL;
	gglBufferSubData = NULL;
	gglGetBufferSubData = NULL;
	gglMapBuffer = NULL;
	gglUnmapBuffer = NULL;
	gglGetBufferParameteriv = NULL;
	gglGetBufferPointerv = NULL;

	gglBlendEquationSeparate = NULL;
	gglDrawBuffers = NULL;
	gglStencilOpSeparate = NULL;
	gglStencilFuncSeparate = NULL;
	gglStencilMaskSeparate = NULL;
	gglAttachShader = NULL;
	gglBindAttribLocation = NULL;
	gglCompileShader = NULL;
	gglCreateProgram = NULL;
	gglCreateShader = NULL;
	gglDeleteProgram = NULL;
	gglDeleteShader = NULL;
	gglDetachShader = NULL;
	gglDisableVertexAttribArray = NULL;
	gglEnableVertexAttribArray = NULL;
	gglGetActiveAttrib = NULL;
	gglGetActiveUniform = NULL;
	gglGetAttachedShaders = NULL;
	gglGetAttribLocation = NULL;
	gglGetProgramiv = NULL;
	gglGetProgramInfoLog = NULL;
	gglGetShaderiv = NULL;
	gglGetShaderInfoLog = NULL;
	gglGetShaderSource = NULL;
	gglGetUniformLocation = NULL;
	gglGetUniformfv = NULL;
	gglGetUniformiv = NULL;
	gglGetVertexAttribdv = NULL;
	gglGetVertexAttribfv = NULL;
	gglGetVertexAttribiv = NULL;
	gglGetVertexAttribPointerv = NULL;
	gglIsProgram = NULL;
	gglIsShader = NULL;
	gglLinkProgram = NULL;
	gglShaderSource = NULL;
	gglUseProgram = NULL;
	gglUniform1f = NULL;
	gglUniform2f = NULL;
	gglUniform3f = NULL;
	gglUniform4f = NULL;
	gglUniform1i = NULL;
	gglUniform2i = NULL;
	gglUniform3i = NULL;
	gglUniform4i = NULL;
	gglUniform1fv = NULL;
	gglUniform2fv = NULL;
	gglUniform3fv = NULL;
	gglUniform4fv = NULL;
	gglUniform1iv = NULL;
	gglUniform2iv = NULL;
	gglUniform3iv = NULL;
	gglUniform4iv = NULL;
	gglUniformMatrix2fv = NULL;
	gglUniformMatrix3fv = NULL;
	gglUniformMatrix4fv = NULL;
	gglValidateProgram = NULL;
	gglVertexAttrib1d = NULL;
	gglVertexAttrib1dv = NULL;
	gglVertexAttrib1f = NULL;
	gglVertexAttrib1fv = NULL;
	gglVertexAttrib1s = NULL;
	gglVertexAttrib1sv = NULL;
	gglVertexAttrib2d = NULL;
	gglVertexAttrib2dv = NULL;
	gglVertexAttrib2f = NULL;
	gglVertexAttrib2fv = NULL;
	gglVertexAttrib2s = NULL;
	gglVertexAttrib2sv = NULL;
	gglVertexAttrib3d = NULL;
	gglVertexAttrib3dv = NULL;
	gglVertexAttrib3f = NULL;
	gglVertexAttrib3fv = NULL;
	gglVertexAttrib3s = NULL;
	gglVertexAttrib3sv = NULL;
	gglVertexAttrib4Nbv = NULL;
	gglVertexAttrib4Niv = NULL;
	gglVertexAttrib4Nsv = NULL;
	gglVertexAttrib4Nub = NULL;
	gglVertexAttrib4Nubv = NULL;
	gglVertexAttrib4Nuiv = NULL;
	gglVertexAttrib4Nusv = NULL;
	gglVertexAttrib4bv = NULL;
	gglVertexAttrib4d = NULL;
	gglVertexAttrib4dv = NULL;
	gglVertexAttrib4f = NULL;
	gglVertexAttrib4fv = NULL;
	gglVertexAttrib4iv = NULL;
	gglVertexAttrib4s = NULL;
	gglVertexAttrib4sv = NULL;
	gglVertexAttrib4ubv = NULL;
	gglVertexAttrib4uiv = NULL;
	gglVertexAttrib4usv = NULL;
	gglVertexAttribPointer = NULL;

	gglUniformMatrix2x3fv = NULL;
	gglUniformMatrix3x2fv = NULL;
	gglUniformMatrix2x4fv = NULL;
	gglUniformMatrix4x2fv = NULL;
	gglUniformMatrix3x4fv = NULL;
	gglUniformMatrix4x3fv = NULL;

	gglColorMaski = NULL;
	gglGetBooleani_v = NULL;
	gglGetIntegeri_v = NULL;
	gglEnablei = NULL;
	gglDisablei = NULL;
	gglIsEnabledi = NULL;
	gglBeginTransformFeedback = NULL;
	gglEndTransformFeedback = NULL;
	gglBindBufferRange = NULL;
	gglBindBufferBase = NULL;
	gglTransformFeedbackVaryings = NULL;
	gglGetTransformFeedbackVarying = NULL;
	gglClampColor = NULL;
	gglBeginConditionalRender = NULL;
	gglEndConditionalRender = NULL;
	gglVertexAttribI1i = NULL;
	gglVertexAttribI2i = NULL;
	gglVertexAttribI3i = NULL;
	gglVertexAttribI4i = NULL;
	gglVertexAttribI1ui = NULL;
	gglVertexAttribI2ui = NULL;
	gglVertexAttribI3ui = NULL;
	gglVertexAttribI4ui = NULL;
	gglVertexAttribI1iv = NULL;
	gglVertexAttribI2iv = NULL;
	gglVertexAttribI3iv = NULL;
	gglVertexAttribI4iv = NULL;
	gglVertexAttribI1uiv = NULL;
	gglVertexAttribI2uiv = NULL;
	gglVertexAttribI3uiv = NULL;
	gglVertexAttribI4uiv = NULL;
	gglVertexAttribI4bv = NULL;
	gglVertexAttribI4sv = NULL;
	gglVertexAttribI4ubv = NULL;
	gglVertexAttribI4usv = NULL;
	gglVertexAttribIPointer = NULL;
	gglGetVertexAttribIiv = NULL;
	gglGetVertexAttribIuiv = NULL;
	gglGetUniformuiv = NULL;
	gglBindFragDataLocation = NULL;
	gglGetFragDataLocation = NULL;
	gglUniform1ui = NULL;
	gglUniform2ui = NULL;
	gglUniform3ui = NULL;
	gglUniform4ui = NULL;
	gglUniform1uiv = NULL;
	gglUniform2uiv = NULL;
	gglUniform3uiv = NULL;
	gglUniform4uiv = NULL;
	gglTexParameterIiv = NULL;
	gglTexParameterIuiv = NULL;
	gglGetTexParameterIiv = NULL;
	gglGetTexParameterIuiv = NULL;
	gglClearBufferiv = NULL;
	gglClearBufferuiv = NULL;
	gglClearBufferfv = NULL;
	gglClearBufferfi = NULL;
	gglGetStringi = NULL;

	gglActiveTextureARB = NULL;
	gglClientActiveTextureARB = NULL;
	gglMultiTexCoord1dARB = NULL;
	gglMultiTexCoord1dvARB = NULL;
	gglMultiTexCoord1fARB = NULL;
	gglMultiTexCoord1fvARB = NULL;
	gglMultiTexCoord1iARB = NULL;
	gglMultiTexCoord1ivARB = NULL;
	gglMultiTexCoord1sARB = NULL;
	gglMultiTexCoord1svARB = NULL;
	gglMultiTexCoord2dARB = NULL;
	gglMultiTexCoord2dvARB = NULL;
	gglMultiTexCoord2fARB = NULL;
	gglMultiTexCoord2fvARB = NULL;
	gglMultiTexCoord2iARB = NULL;
	gglMultiTexCoord2ivARB = NULL;
	gglMultiTexCoord2sARB = NULL;
	gglMultiTexCoord2svARB = NULL;
	gglMultiTexCoord3dARB = NULL;
	gglMultiTexCoord3dvARB = NULL;
	gglMultiTexCoord3fARB = NULL;
	gglMultiTexCoord3fvARB = NULL;
	gglMultiTexCoord3iARB = NULL;
	gglMultiTexCoord3ivARB = NULL;
	gglMultiTexCoord3sARB = NULL;
	gglMultiTexCoord3svARB = NULL;
	gglMultiTexCoord4dARB = NULL;
	gglMultiTexCoord4dvARB = NULL;
	gglMultiTexCoord4fARB = NULL;
	gglMultiTexCoord4fvARB = NULL;
	gglMultiTexCoord4iARB = NULL;
	gglMultiTexCoord4ivARB = NULL;
	gglMultiTexCoord4sARB = NULL;
	gglMultiTexCoord4svARB = NULL;
	gglLoadTransposeMatrixfARB = NULL;
	gglLoadTransposeMatrixdARB = NULL;
	gglMultTransposeMatrixfARB = NULL;
	gglMultTransposeMatrixdARB = NULL;
	gglSampleCoverageARB = NULL;
	gglCompressedTexImage3DARB = NULL;
	gglCompressedTexImage2DARB = NULL;
	gglCompressedTexImage1DARB = NULL;
	gglCompressedTexSubImage3DARB = NULL;
	gglCompressedTexSubImage2DARB = NULL;
	gglCompressedTexSubImage1DARB = NULL;
	gglGetCompressedTexImageARB = NULL;
	gglPointParameterfARB = NULL;
	gglPointParameterfvARB = NULL;
	gglWeightbvARB = NULL;
	gglWeightsvARB = NULL;
	gglWeightivARB = NULL;
	gglWeightfvARB = NULL;
	gglWeightdvARB = NULL;
	gglWeightubvARB = NULL;
	gglWeightusvARB = NULL;
	gglWeightuivARB = NULL;
	gglWeightPointerARB = NULL;
	gglVertexBlendARB = NULL;
	gglCurrentPaletteMatrixARB = NULL;
	gglMatrixIndexubvARB = NULL;
	gglMatrixIndexusvARB = NULL;
	gglMatrixIndexuivARB = NULL;
	gglMatrixIndexPointerARB = NULL;
	gglWindowPos2dARB = NULL;
	gglWindowPos2dvARB = NULL;
	gglWindowPos2fARB = NULL;
	gglWindowPos2fvARB = NULL;
	gglWindowPos2iARB = NULL;
	gglWindowPos2ivARB = NULL;
	gglWindowPos2sARB = NULL;
	gglWindowPos2svARB = NULL;
	gglWindowPos3dARB = NULL;
	gglWindowPos3dvARB = NULL;
	gglWindowPos3fARB = NULL;
	gglWindowPos3fvARB = NULL;
	gglWindowPos3iARB = NULL;
	gglWindowPos3ivARB = NULL;
	gglWindowPos3sARB = NULL;
	gglWindowPos3svARB = NULL;
	gglVertexAttrib1sARB = NULL;
	gglVertexAttrib1fARB = NULL;
	gglVertexAttrib1dARB = NULL;
	gglVertexAttrib2sARB = NULL;
	gglVertexAttrib2fARB = NULL;
	gglVertexAttrib2dARB = NULL;
	gglVertexAttrib3sARB = NULL;
	gglVertexAttrib3fARB = NULL;
	gglVertexAttrib3dARB = NULL;
	gglVertexAttrib4sARB = NULL;
	gglVertexAttrib4fARB = NULL;
	gglVertexAttrib4dARB = NULL;
	gglVertexAttrib4NubARB = NULL;
	gglVertexAttrib1svARB = NULL;
	gglVertexAttrib1fvARB = NULL;
	gglVertexAttrib1dvARB = NULL;
	gglVertexAttrib2svARB = NULL;
	gglVertexAttrib2fvARB = NULL;
	gglVertexAttrib2dvARB = NULL;
	gglVertexAttrib3svARB = NULL;
	gglVertexAttrib3fvARB = NULL;
	gglVertexAttrib3dvARB = NULL;
	gglVertexAttrib4bvARB = NULL;
	gglVertexAttrib4svARB = NULL;
	gglVertexAttrib4ivARB = NULL;
	gglVertexAttrib4ubvARB = NULL;
	gglVertexAttrib4usvARB = NULL;
	gglVertexAttrib4uivARB = NULL;
	gglVertexAttrib4fvARB = NULL;
	gglVertexAttrib4dvARB = NULL;
	gglVertexAttrib4NbvARB = NULL;
	gglVertexAttrib4NsvARB = NULL;
	gglVertexAttrib4NivARB = NULL;
	gglVertexAttrib4NubvARB = NULL;
	gglVertexAttrib4NusvARB = NULL;
	gglVertexAttrib4NuivARB = NULL;
	gglVertexAttribPointerARB = NULL;
	gglEnableVertexAttribArrayARB = NULL;
	gglDisableVertexAttribArrayARB = NULL;
	gglProgramStringARB = NULL;
	gglBindProgramARB = NULL;
	gglDeleteProgramsARB = NULL;
	gglGenProgramsARB = NULL;
	gglProgramEnvParameter4dARB = NULL;
	gglProgramEnvParameter4dvARB = NULL;
	gglProgramEnvParameter4fARB = NULL;
	gglProgramEnvParameter4fvARB = NULL;
	gglProgramLocalParameter4dARB = NULL;
	gglProgramLocalParameter4dvARB = NULL;
	gglProgramLocalParameter4fARB = NULL;
	gglProgramLocalParameter4fvARB = NULL;
	gglGetProgramEnvParameterdvARB = NULL;
	gglGetProgramEnvParameterfvARB = NULL;
	gglGetProgramLocalParameterdvARB = NULL;
	gglGetProgramLocalParameterfvARB = NULL;
	gglGetProgramivARB = NULL;
	gglGetProgramStringARB = NULL;
	gglGetVertexAttribdvARB = NULL;
	gglGetVertexAttribfvARB = NULL;
	gglGetVertexAttribivARB = NULL;
	gglGetVertexAttribPointervARB = NULL;
	gglIsProgramARB = NULL;
	gglProgramStringARB = NULL;
	gglBindProgramARB = NULL;
	gglDeleteProgramsARB = NULL;
	gglGenProgramsARB = NULL;
	gglProgramEnvParameter4dARB = NULL;
	gglProgramEnvParameter4dvARB = NULL;
	gglProgramEnvParameter4fARB = NULL;
	gglProgramEnvParameter4fvARB = NULL;
	gglProgramLocalParameter4dARB = NULL;
	gglProgramLocalParameter4dvARB = NULL;
	gglProgramLocalParameter4fARB = NULL;
	gglProgramLocalParameter4fvARB = NULL;
	gglGetProgramEnvParameterdvARB = NULL;
	gglGetProgramEnvParameterfvARB = NULL;
	gglGetProgramLocalParameterdvARB = NULL;
	gglGetProgramLocalParameterfvARB = NULL;
	gglGetProgramivARB = NULL;
	gglGetProgramStringARB = NULL;
	gglIsProgramARB = NULL;
	gglBindBufferARB = NULL;
	gglDeleteBuffersARB = NULL;
	gglGenBuffersARB = NULL;
	gglIsBufferARB = NULL;
	gglBufferDataARB = NULL;
	gglBufferSubDataARB = NULL;
	gglGetBufferSubDataARB = NULL;
	gglMapBufferARB = NULL;
	gglUnmapBufferARB = NULL;
	gglGetBufferParameterivARB = NULL;
	gglGetBufferPointervARB = NULL;
	gglGenQueriesARB = NULL;
	gglDeleteQueriesARB = NULL;
	gglIsQueryARB = NULL;
	gglBeginQueryARB = NULL;
	gglEndQueryARB = NULL;
	gglGetQueryivARB = NULL;
	gglGetQueryObjectivARB = NULL;
	gglGetQueryObjectuivARB = NULL;
	gglDeleteObjectARB = NULL;
	gglGetHandleARB = NULL;
	gglDetachObjectARB = NULL;
	gglCreateShaderObjectARB = NULL;
	gglShaderSourceARB = NULL;
	gglCompileShaderARB = NULL;
	gglCreateProgramObjectARB = NULL;
	gglAttachObjectARB = NULL;
	gglLinkProgramARB = NULL;
	gglUseProgramObjectARB = NULL;
	gglValidateProgramARB = NULL;
	gglUniform1fARB = NULL;
	gglUniform2fARB = NULL;
	gglUniform3fARB = NULL;
	gglUniform4fARB = NULL;
	gglUniform1iARB = NULL;
	gglUniform2iARB = NULL;
	gglUniform3iARB = NULL;
	gglUniform4iARB = NULL;
	gglUniform1fvARB = NULL;
	gglUniform2fvARB = NULL;
	gglUniform3fvARB = NULL;
	gglUniform4fvARB = NULL;
	gglUniform1ivARB = NULL;
	gglUniform2ivARB = NULL;
	gglUniform3ivARB = NULL;
	gglUniform4ivARB = NULL;
	gglUniformMatrix2fvARB = NULL;
	gglUniformMatrix3fvARB = NULL;
	gglUniformMatrix4fvARB = NULL;
	gglGetObjectParameterfvARB = NULL;
	gglGetObjectParameterivARB = NULL;
	gglGetInfoLogARB = NULL;
	gglGetAttachedObjectsARB = NULL;
	gglGetUniformLocationARB = NULL;
	gglGetActiveUniformARB = NULL;
	gglGetUniformfvARB = NULL;
	gglGetUniformivARB = NULL;
	gglGetShaderSourceARB = NULL;
	gglBindAttribLocationARB = NULL;
	gglGetActiveAttribARB = NULL;
	gglGetAttribLocationARB = NULL;
	gglDrawBuffersARB = NULL;
	gglClampColorARB = NULL;
	gglDrawArraysInstancedARB = NULL;
	gglDrawElementsInstancedARB = NULL;
	gglIsRenderbuffer = NULL;
	gglBindRenderbuffer = NULL;
	gglDeleteRenderbuffers = NULL;
	gglGenRenderbuffers = NULL;
	gglRenderbufferStorage = NULL;
	gglGetRenderbufferParameteriv = NULL;
	gglIsFramebuffer = NULL;
	gglBindFramebuffer = NULL;
	gglDeleteFramebuffers = NULL;
	gglGenFramebuffers = NULL;
	gglCheckFramebufferStatus = NULL;
	gglFramebufferTexture1D = NULL;
	gglFramebufferTexture2D = NULL;
	gglFramebufferTexture3D = NULL;
	gglFramebufferRenderbuffer = NULL;
	gglGetFramebufferAttachmentParameteriv = NULL;
	gglGenerateMipmap = NULL;
	gglBlitFramebuffer = NULL;
	gglRenderbufferStorageMultisample = NULL;
	gglFramebufferTextureLayer = NULL;
	gglProgramParameteriARB = NULL;
	gglFramebufferTextureARB = NULL;
	gglFramebufferTextureLayerARB = NULL;
	gglFramebufferTextureFaceARB = NULL;
	gglVertexAttribDivisor = NULL;
	gglMapBufferRange = NULL;
	gglFlushMappedBufferRange = NULL;
	gglTexBufferARB = NULL;
	gglBindVertexArray = NULL;
	gglDeleteVertexArrays = NULL;
	gglGenVertexArrays = NULL;
	gglIsVertexArray = NULL;
	gglBlendColorEXT = NULL;
	gglPolygonOffsetEXT = NULL;
	gglTexImage3DEXT = NULL;
	gglTexSubImage3DEXT = NULL;
	gglGetTexFilterFuncSGIS = NULL;
	gglTexFilterFuncSGIS = NULL;
	gglTexSubImage1DEXT = NULL;
	gglTexSubImage2DEXT = NULL;
	gglCopyTexImage1DEXT = NULL;
	gglCopyTexImage2DEXT = NULL;
	gglCopyTexSubImage1DEXT = NULL;
	gglCopyTexSubImage2DEXT = NULL;
	gglCopyTexSubImage3DEXT = NULL;
	gglGetHistogramEXT = NULL;
	gglGetHistogramParameterfvEXT = NULL;
	gglGetHistogramParameterivEXT = NULL;
	gglGetMinmaxEXT = NULL;
	gglGetMinmaxParameterfvEXT = NULL;
	gglGetMinmaxParameterivEXT = NULL;
	gglHistogramEXT = NULL;
	gglMinmaxEXT = NULL;
	gglResetHistogramEXT = NULL;
	gglResetMinmaxEXT = NULL;
	gglConvolutionFilter1DEXT = NULL;
	gglConvolutionFilter2DEXT = NULL;
	gglConvolutionParameterfEXT = NULL;
	gglConvolutionParameterfvEXT = NULL;
	gglConvolutionParameteriEXT = NULL;
	gglConvolutionParameterivEXT = NULL;
	gglCopyConvolutionFilter1DEXT = NULL;
	gglCopyConvolutionFilter2DEXT = NULL;
	gglGetConvolutionFilterEXT = NULL;
	gglGetConvolutionParameterfvEXT = NULL;
	gglGetConvolutionParameterivEXT = NULL;
	gglGetSeparableFilterEXT = NULL;
	gglSeparableFilter2DEXT = NULL;
	gglColorTableSGI = NULL;
	gglColorTableParameterfvSGI = NULL;
	gglColorTableParameterivSGI = NULL;
	gglCopyColorTableSGI = NULL;
	gglGetColorTableSGI = NULL;
	gglGetColorTableParameterfvSGI = NULL;
	gglGetColorTableParameterivSGI = NULL;
	gglPixelTexGenSGIX = NULL;
	gglPixelTexGenParameteriSGIS = NULL;
	gglPixelTexGenParameterivSGIS = NULL;
	gglPixelTexGenParameterfSGIS = NULL;
	gglPixelTexGenParameterfvSGIS = NULL;
	gglGetPixelTexGenParameterivSGIS = NULL;
	gglGetPixelTexGenParameterfvSGIS = NULL;
	gglTexImage4DSGIS = NULL;
	gglTexSubImage4DSGIS = NULL;
	gglAreTexturesResidentEXT = NULL;
	gglBindTextureEXT = NULL;
	gglDeleteTexturesEXT = NULL;
	gglGenTexturesEXT = NULL;
	gglIsTextureEXT = NULL;
	gglPrioritizeTexturesEXT = NULL;
	gglDetailTexFuncSGIS = NULL;
	gglGetDetailTexFuncSGIS = NULL;
	gglSharpenTexFuncSGIS = NULL;
	gglGetSharpenTexFuncSGIS = NULL;
	gglSampleMaskSGIS = NULL;
	gglSamplePatternSGIS = NULL;
	gglArrayElementEXT = NULL;
	gglColorPointerEXT = NULL;
	gglDrawArraysEXT = NULL;
	gglEdgeFlagPointerEXT = NULL;
	gglGetPointervEXT = NULL;
	gglIndexPointerEXT = NULL;
	gglNormalPointerEXT = NULL;
	gglTexCoordPointerEXT = NULL;
	gglVertexPointerEXT = NULL;
	gglBlendEquationEXT = NULL;
	gglSpriteParameterfSGIX = NULL;
	gglSpriteParameterfvSGIX = NULL;
	gglSpriteParameteriSGIX = NULL;
	gglSpriteParameterivSGIX = NULL;
	gglPointParameterfEXT = NULL;
	gglPointParameterfvEXT = NULL;
	gglPointParameterfSGIS = NULL;
	gglPointParameterfvSGIS = NULL;
	gglGetInstrumentsSGIX = NULL;
	gglInstrumentsBufferSGIX = NULL;
	gglPollInstrumentsSGIX = NULL;
	gglReadInstrumentsSGIX = NULL;
	gglStartInstrumentsSGIX = NULL;
	gglStopInstrumentsSGIX = NULL;
	gglFrameZoomSGIX = NULL;
	gglTagSampleBufferSGIX = NULL;
	gglDeformationMap3dSGIX = NULL;
	gglDeformationMap3fSGIX = NULL;
	gglDeformSGIX = NULL;
	gglLoadIdentityDeformationMapSGIX = NULL;
	gglReferencePlaneSGIX = NULL;
	gglFlushRasterSGIX = NULL;
	gglFogFuncSGIS = NULL;
	gglGetFogFuncSGIS = NULL;
	gglImageTransformParameteriHP = NULL;
	gglImageTransformParameterfHP = NULL;
	gglImageTransformParameterivHP = NULL;
	gglImageTransformParameterfvHP = NULL;
	gglGetImageTransformParameterivHP = NULL;
	gglGetImageTransformParameterfvHP = NULL;
	gglColorSubTableEXT = NULL;
	gglCopyColorSubTableEXT = NULL;
	gglHintPGI = NULL;
	gglColorTableEXT = NULL;
	gglGetColorTableEXT = NULL;
	gglGetColorTableParameterivEXT = NULL;
	gglGetColorTableParameterfvEXT = NULL;
	gglGetListParameterfvSGIX = NULL;
	gglGetListParameterivSGIX = NULL;
	gglListParameterfSGIX = NULL;
	gglListParameterfvSGIX = NULL;
	gglListParameteriSGIX = NULL;
	gglListParameterivSGIX = NULL;
	gglIndexMaterialEXT = NULL;
	gglIndexFuncEXT = NULL;
	gglLockArraysEXT = NULL;
	gglUnlockArraysEXT = NULL;
	gglCullParameterdvEXT = NULL;
	gglCullParameterfvEXT = NULL;
	gglFragmentColorMaterialSGIX = NULL;
	gglFragmentLightfSGIX = NULL;
	gglFragmentLightfvSGIX = NULL;
	gglFragmentLightiSGIX = NULL;
	gglFragmentLightivSGIX = NULL;
	gglFragmentLightModelfSGIX = NULL;
	gglFragmentLightModelfvSGIX = NULL;
	gglFragmentLightModeliSGIX = NULL;
	gglFragmentLightModelivSGIX = NULL;
	gglFragmentMaterialfSGIX = NULL;
	gglFragmentMaterialfvSGIX = NULL;
	gglFragmentMaterialiSGIX = NULL;
	gglFragmentMaterialivSGIX = NULL;
	gglGetFragmentLightfvSGIX = NULL;
	gglGetFragmentLightivSGIX = NULL;
	gglGetFragmentMaterialfvSGIX = NULL;
	gglGetFragmentMaterialivSGIX = NULL;
	gglLightEnviSGIX = NULL;
	gglFragmentColorMaterialEXT = NULL;
	gglFragmentLightfEXT = NULL;
	gglFragmentLightfvEXT = NULL;
	gglFragmentLightiEXT = NULL;
	gglFragmentLightivEXT = NULL;
	gglFragmentLightModelfEXT = NULL;
	gglFragmentLightModelfvEXT = NULL;
	gglFragmentLightModeliEXT = NULL;
	gglFragmentLightModelivEXT = NULL;
	gglFragmentMaterialfEXT = NULL;
	gglFragmentMaterialfvEXT = NULL;
	gglFragmentMaterialiEXT = NULL;
	gglFragmentMaterialivEXT = NULL;
	gglGetFragmentLightfvEXT = NULL;
	gglGetFragmentLightivEXT = NULL;
	gglGetFragmentMaterialfvEXT = NULL;
	gglGetFragmentMaterialivEXT = NULL;
	gglLightEnviEXT = NULL;
	gglDrawRangeElementsEXT = NULL;
	gglApplyTextureEXT = NULL;
	gglTextureLightEXT = NULL;
	gglTextureMaterialEXT = NULL;
	gglBeginSceneEXT = NULL;
	gglEndSceneEXT = NULL;
	gglAsyncMarkerSGIX = NULL;
	gglFinishAsyncSGIX = NULL;
	gglPollAsyncSGIX = NULL;
	gglGenAsyncMarkersSGIX = NULL;
	gglDeleteAsyncMarkersSGIX = NULL;
	gglIsAsyncMarkerSGIX = NULL;
	gglVertexPointervINTEL = NULL;
	gglNormalPointervINTEL = NULL;
	gglColorPointervINTEL = NULL;
	gglTexCoordPointervINTEL = NULL;
	gglPixelTransformParameteriEXT = NULL;
	gglPixelTransformParameterfEXT = NULL;
	gglPixelTransformParameterivEXT = NULL;
	gglPixelTransformParameterfvEXT = NULL;
	gglSecondaryColor3bEXT = NULL;
	gglSecondaryColor3bvEXT = NULL;
	gglSecondaryColor3dEXT = NULL;
	gglSecondaryColor3dvEXT = NULL;
	gglSecondaryColor3fEXT = NULL;
	gglSecondaryColor3fvEXT = NULL;
	gglSecondaryColor3iEXT = NULL;
	gglSecondaryColor3ivEXT = NULL;
	gglSecondaryColor3sEXT = NULL;
	gglSecondaryColor3svEXT = NULL;
	gglSecondaryColor3ubEXT = NULL;
	gglSecondaryColor3ubvEXT = NULL;
	gglSecondaryColor3uiEXT = NULL;
	gglSecondaryColor3uivEXT = NULL;
	gglSecondaryColor3usEXT = NULL;
	gglSecondaryColor3usvEXT = NULL;
	gglSecondaryColorPointerEXT = NULL;
	gglTextureNormalEXT = NULL;
	gglMultiDrawArraysEXT = NULL;
	gglMultiDrawElementsEXT = NULL;
	gglFogCoordfEXT = NULL;
	gglFogCoordfvEXT = NULL;
	gglFogCoorddEXT = NULL;
	gglFogCoorddvEXT = NULL;
	gglFogCoordPointerEXT = NULL;
	gglTangent3bEXT = NULL;
	gglTangent3bvEXT = NULL;
	gglTangent3dEXT = NULL;
	gglTangent3dvEXT = NULL;
	gglTangent3fEXT = NULL;
	gglTangent3fvEXT = NULL;
	gglTangent3iEXT = NULL;
	gglTangent3ivEXT = NULL;
	gglTangent3sEXT = NULL;
	gglTangent3svEXT = NULL;
	gglBinormal3bEXT = NULL;
	gglBinormal3bvEXT = NULL;
	gglBinormal3dEXT = NULL;
	gglBinormal3dvEXT = NULL;
	gglBinormal3fEXT = NULL;
	gglBinormal3fvEXT = NULL;
	gglBinormal3iEXT = NULL;
	gglBinormal3ivEXT = NULL;
	gglBinormal3sEXT = NULL;
	gglBinormal3svEXT = NULL;
	gglTangentPointerEXT = NULL;
	gglBinormalPointerEXT = NULL;
	gglFinishTextureSUNX = NULL;
	gglGlobalAlphaFactorbSUN = NULL;
	gglGlobalAlphaFactorsSUN = NULL;
	gglGlobalAlphaFactoriSUN = NULL;
	gglGlobalAlphaFactorfSUN = NULL;
	gglGlobalAlphaFactordSUN = NULL;
	gglGlobalAlphaFactorubSUN = NULL;
	gglGlobalAlphaFactorusSUN = NULL;
	gglGlobalAlphaFactoruiSUN = NULL;
	gglReplacementCodeuiSUN = NULL;
	gglReplacementCodeusSUN = NULL;
	gglReplacementCodeubSUN = NULL;
	gglReplacementCodeuivSUN = NULL;
	gglReplacementCodeusvSUN = NULL;
	gglReplacementCodeubvSUN = NULL;
	gglReplacementCodePointerSUN = NULL;
	gglColor4ubVertex2fSUN = NULL;
	gglColor4ubVertex2fvSUN = NULL;
	gglColor4ubVertex3fSUN = NULL;
	gglColor4ubVertex3fvSUN = NULL;
	gglColor3fVertex3fSUN = NULL;
	gglColor3fVertex3fvSUN = NULL;
	gglNormal3fVertex3fSUN = NULL;
	gglNormal3fVertex3fvSUN = NULL;
	gglColor4fNormal3fVertex3fSUN = NULL;
	gglColor4fNormal3fVertex3fvSUN = NULL;
	gglTexCoord2fVertex3fSUN = NULL;
	gglTexCoord2fVertex3fvSUN = NULL;
	gglTexCoord4fVertex4fSUN = NULL;
	gglTexCoord4fVertex4fvSUN = NULL;
	gglTexCoord2fColor4ubVertex3fSUN = NULL;
	gglTexCoord2fColor4ubVertex3fvSUN = NULL;
	gglTexCoord2fColor3fVertex3fSUN = NULL;
	gglTexCoord2fColor3fVertex3fvSUN = NULL;
	gglTexCoord2fNormal3fVertex3fSUN = NULL;
	gglTexCoord2fNormal3fVertex3fvSUN = NULL;
	gglTexCoord2fColor4fNormal3fVertex3fSUN = NULL;
	gglTexCoord2fColor4fNormal3fVertex3fvSUN = NULL;
	gglTexCoord4fColor4fNormal3fVertex4fSUN = NULL;
	gglTexCoord4fColor4fNormal3fVertex4fvSUN = NULL;
	gglReplacementCodeuiVertex3fSUN = NULL;
	gglReplacementCodeuiVertex3fvSUN = NULL;
	gglReplacementCodeuiColor4ubVertex3fSUN = NULL;
	gglReplacementCodeuiColor4ubVertex3fvSUN = NULL;
	gglReplacementCodeuiColor3fVertex3fSUN = NULL;
	gglReplacementCodeuiColor3fVertex3fvSUN = NULL;
	gglReplacementCodeuiNormal3fVertex3fSUN = NULL;
	gglReplacementCodeuiNormal3fVertex3fvSUN = NULL;
	gglReplacementCodeuiColor4fNormal3fVertex3fSUN = NULL;
	gglReplacementCodeuiColor4fNormal3fVertex3fvSUN = NULL;
	gglReplacementCodeuiTexCoord2fVertex3fSUN = NULL;
	gglReplacementCodeuiTexCoord2fVertex3fvSUN = NULL;
	gglReplacementCodeuiTexCoord2fNormal3fVertex3fSUN = NULL;
	gglReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN = NULL;
	gglReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN = NULL;
	gglReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN = NULL;
	gglBlendFuncSeparateEXT = NULL;
	gglVertexWeightfEXT = NULL;
	gglVertexWeightfvEXT = NULL;
	gglVertexWeightPointerEXT = NULL;
	gglFlushVertexArrayRangeNV = NULL;
	gglVertexArrayRangeNV = NULL;
	gglCombinerParameterfvNV = NULL;
	gglCombinerParameterfNV = NULL;
	gglCombinerParameterivNV = NULL;
	gglCombinerParameteriNV = NULL;
	gglCombinerInputNV = NULL;
	gglCombinerOutputNV = NULL;
	gglFinalCombinerInputNV = NULL;
	gglGetCombinerInputParameterfvNV = NULL;
	gglGetCombinerInputParameterivNV = NULL;
	gglGetCombinerOutputParameterfvNV = NULL;
	gglGetCombinerOutputParameterivNV = NULL;
	gglGetFinalCombinerInputParameterfvNV = NULL;
	gglGetFinalCombinerInputParameterivNV = NULL;
	gglResizeBuffersMESA = NULL;
	gglWindowPos2dMESA = NULL;
	gglWindowPos2dvMESA = NULL;
	gglWindowPos2fMESA = NULL;
	gglWindowPos2fvMESA = NULL;
	gglWindowPos2iMESA = NULL;
	gglWindowPos2ivMESA = NULL;
	gglWindowPos2sMESA = NULL;
	gglWindowPos2svMESA = NULL;
	gglWindowPos3dMESA = NULL;
	gglWindowPos3dvMESA = NULL;
	gglWindowPos3fMESA = NULL;
	gglWindowPos3fvMESA = NULL;
	gglWindowPos3iMESA = NULL;
	gglWindowPos3ivMESA = NULL;
	gglWindowPos3sMESA = NULL;
	gglWindowPos3svMESA = NULL;
	gglWindowPos4dMESA = NULL;
	gglWindowPos4dvMESA = NULL;
	gglWindowPos4fMESA = NULL;
	gglWindowPos4fvMESA = NULL;
	gglWindowPos4iMESA = NULL;
	gglWindowPos4ivMESA = NULL;
	gglWindowPos4sMESA = NULL;
	gglWindowPos4svMESA = NULL;
	gglMultiModeDrawArraysIBM = NULL;
	gglMultiModeDrawElementsIBM = NULL;
	gglColorPointerListIBM = NULL;
	gglSecondaryColorPointerListIBM = NULL;
	gglEdgeFlagPointerListIBM = NULL;
	gglFogCoordPointerListIBM = NULL;
	gglIndexPointerListIBM = NULL;
	gglNormalPointerListIBM = NULL;
	gglTexCoordPointerListIBM = NULL;
	gglVertexPointerListIBM = NULL;
	gglTbufferMask3DFX = NULL;
	gglSampleMaskEXT = NULL;
	gglSamplePatternEXT = NULL;
	gglTextureColorMaskSGIS = NULL;
	gglIglooInterfaceSGIX = NULL;
	gglDeleteFencesNV = NULL;
	gglGenFencesNV = NULL;
	gglIsFenceNV = NULL;
	gglTestFenceNV = NULL;
	gglGetFenceivNV = NULL;
	gglFinishFenceNV = NULL;
	gglSetFenceNV = NULL;
	gglFlushStaticDataIBM = NULL;
	gglMapControlPointsNV = NULL;
	gglMapParameterivNV = NULL;
	gglMapParameterfvNV = NULL;
	gglGetMapControlPointsNV = NULL;
	gglGetMapParameterivNV = NULL;
	gglGetMapParameterfvNV = NULL;
	gglGetMapAttribParameterivNV = NULL;
	gglGetMapAttribParameterfvNV = NULL;
	gglEvalMapsNV = NULL;
	gglCombinerStageParameterfvNV = NULL;
	gglGetCombinerStageParameterfvNV = NULL;
	gglAreProgramsResidentNV = NULL;
	gglBindProgramNV = NULL;
	gglDeleteProgramsNV = NULL;
	gglExecuteProgramNV = NULL;
	gglGenProgramsNV = NULL;
	gglGetProgramParameterdvNV = NULL;
	gglGetProgramParameterfvNV = NULL;
	gglGetProgramivNV = NULL;
	gglGetProgramStringNV = NULL;
	gglGetTrackMatrixivNV = NULL;
	gglGetVertexAttribdvNV = NULL;
	gglGetVertexAttribfvNV = NULL;
	gglGetVertexAttribivNV = NULL;
	gglGetVertexAttribPointervNV = NULL;
	gglIsProgramNV = NULL;
	gglLoadProgramNV = NULL;
	gglProgramParameter4dNV = NULL;
	gglProgramParameter4dvNV = NULL;
	gglProgramParameter4fNV = NULL;
	gglProgramParameter4fvNV = NULL;
	gglProgramParameters4dvNV = NULL;
	gglProgramParameters4fvNV = NULL;
	gglRequestResidentProgramsNV = NULL;
	gglTrackMatrixNV = NULL;
	gglVertexAttribPointerNV = NULL;
	gglVertexAttrib1dNV = NULL;
	gglVertexAttrib1dvNV = NULL;
	gglVertexAttrib1fNV = NULL;
	gglVertexAttrib1fvNV = NULL;
	gglVertexAttrib1sNV = NULL;
	gglVertexAttrib1svNV = NULL;
	gglVertexAttrib2dNV = NULL;
	gglVertexAttrib2dvNV = NULL;
	gglVertexAttrib2fNV = NULL;
	gglVertexAttrib2fvNV = NULL;
	gglVertexAttrib2sNV = NULL;
	gglVertexAttrib2svNV = NULL;
	gglVertexAttrib3dNV = NULL;
	gglVertexAttrib3dvNV = NULL;
	gglVertexAttrib3fNV = NULL;
	gglVertexAttrib3fvNV = NULL;
	gglVertexAttrib3sNV = NULL;
	gglVertexAttrib3svNV = NULL;
	gglVertexAttrib4dNV = NULL;
	gglVertexAttrib4dvNV = NULL;
	gglVertexAttrib4fNV = NULL;
	gglVertexAttrib4fvNV = NULL;
	gglVertexAttrib4sNV = NULL;
	gglVertexAttrib4svNV = NULL;
	gglVertexAttrib4ubNV = NULL;
	gglVertexAttrib4ubvNV = NULL;
	gglVertexAttribs1dvNV = NULL;
	gglVertexAttribs1fvNV = NULL;
	gglVertexAttribs1svNV = NULL;
	gglVertexAttribs2dvNV = NULL;
	gglVertexAttribs2fvNV = NULL;
	gglVertexAttribs2svNV = NULL;
	gglVertexAttribs3dvNV = NULL;
	gglVertexAttribs3fvNV = NULL;
	gglVertexAttribs3svNV = NULL;
	gglVertexAttribs4dvNV = NULL;
	gglVertexAttribs4fvNV = NULL;
	gglVertexAttribs4svNV = NULL;
	gglVertexAttribs4ubvNV = NULL;
	gglTexBumpParameterivATI = NULL;
	gglTexBumpParameterfvATI = NULL;
	gglGetTexBumpParameterivATI = NULL;
	gglGetTexBumpParameterfvATI = NULL;
	gglGenFragmentShadersATI = NULL;
	gglBindFragmentShaderATI = NULL;
	gglDeleteFragmentShaderATI = NULL;
	gglBeginFragmentShaderATI = NULL;
	gglEndFragmentShaderATI = NULL;
	gglPassTexCoordATI = NULL;
	gglSampleMapATI = NULL;
	gglColorFragmentOp1ATI = NULL;
	gglColorFragmentOp2ATI = NULL;
	gglColorFragmentOp3ATI = NULL;
	gglAlphaFragmentOp1ATI = NULL;
	gglAlphaFragmentOp2ATI = NULL;
	gglAlphaFragmentOp3ATI = NULL;
	gglSetFragmentShaderConstantATI = NULL;
	gglPNTrianglesiATI = NULL;
	gglPNTrianglesfATI = NULL;
	gglNewObjectBufferATI = NULL;
	gglIsObjectBufferATI = NULL;
	gglUpdateObjectBufferATI = NULL;
	gglGetObjectBufferfvATI = NULL;
	gglGetObjectBufferivATI = NULL;
	gglDeleteObjectBufferATI = NULL;
	gglArrayObjectATI = NULL;
	gglGetArrayObjectfvATI = NULL;
	gglGetArrayObjectivATI = NULL;
	gglVariantArrayObjectATI = NULL;
	gglGetVariantArrayObjectfvATI = NULL;
	gglGetVariantArrayObjectivATI = NULL;
	gglBeginVertexShaderEXT = NULL;
	gglEndVertexShaderEXT = NULL;
	gglBindVertexShaderEXT = NULL;
	gglGenVertexShadersEXT = NULL;
	gglDeleteVertexShaderEXT = NULL;
	gglShaderOp1EXT = NULL;
	gglShaderOp2EXT = NULL;
	gglShaderOp3EXT = NULL;
	gglSwizzleEXT = NULL;
	gglWriteMaskEXT = NULL;
	gglInsertComponentEXT = NULL;
	gglExtractComponentEXT = NULL;
	gglGenSymbolsEXT = NULL;
	gglSetInvariantEXT = NULL;
	gglSetLocalConstantEXT = NULL;
	gglVariantbvEXT = NULL;
	gglVariantsvEXT = NULL;
	gglVariantivEXT = NULL;
	gglVariantfvEXT = NULL;
	gglVariantdvEXT = NULL;
	gglVariantubvEXT = NULL;
	gglVariantusvEXT = NULL;
	gglVariantuivEXT = NULL;
	gglVariantPointerEXT = NULL;
	gglEnableVariantClientStateEXT = NULL;
	gglDisableVariantClientStateEXT = NULL;
	gglBindLightParameterEXT = NULL;
	gglBindMaterialParameterEXT = NULL;
	gglBindTexGenParameterEXT = NULL;
	gglBindTextureUnitParameterEXT = NULL;
	gglBindParameterEXT = NULL;
	gglIsVariantEnabledEXT = NULL;
	gglGetVariantBooleanvEXT = NULL;
	gglGetVariantIntegervEXT = NULL;
	gglGetVariantFloatvEXT = NULL;
	gglGetVariantPointervEXT = NULL;
	gglGetInvariantBooleanvEXT = NULL;
	gglGetInvariantIntegervEXT = NULL;
	gglGetInvariantFloatvEXT = NULL;
	gglGetLocalConstantBooleanvEXT = NULL;
	gglGetLocalConstantIntegervEXT = NULL;
	gglGetLocalConstantFloatvEXT = NULL;
	gglVertexStream1sATI = NULL;
	gglVertexStream1svATI = NULL;
	gglVertexStream1iATI = NULL;
	gglVertexStream1ivATI = NULL;
	gglVertexStream1fATI = NULL;
	gglVertexStream1fvATI = NULL;
	gglVertexStream1dATI = NULL;
	gglVertexStream1dvATI = NULL;
	gglVertexStream2sATI = NULL;
	gglVertexStream2svATI = NULL;
	gglVertexStream2iATI = NULL;
	gglVertexStream2ivATI = NULL;
	gglVertexStream2fATI = NULL;
	gglVertexStream2fvATI = NULL;
	gglVertexStream2dATI = NULL;
	gglVertexStream2dvATI = NULL;
	gglVertexStream3sATI = NULL;
	gglVertexStream3svATI = NULL;
	gglVertexStream3iATI = NULL;
	gglVertexStream3ivATI = NULL;
	gglVertexStream3fATI = NULL;
	gglVertexStream3fvATI = NULL;
	gglVertexStream3dATI = NULL;
	gglVertexStream3dvATI = NULL;
	gglVertexStream4sATI = NULL;
	gglVertexStream4svATI = NULL;
	gglVertexStream4iATI = NULL;
	gglVertexStream4ivATI = NULL;
	gglVertexStream4fATI = NULL;
	gglVertexStream4fvATI = NULL;
	gglVertexStream4dATI = NULL;
	gglVertexStream4dvATI = NULL;
	gglNormalStream3bATI = NULL;
	gglNormalStream3bvATI = NULL;
	gglNormalStream3sATI = NULL;
	gglNormalStream3svATI = NULL;
	gglNormalStream3iATI = NULL;
	gglNormalStream3ivATI = NULL;
	gglNormalStream3fATI = NULL;
	gglNormalStream3fvATI = NULL;
	gglNormalStream3dATI = NULL;
	gglNormalStream3dvATI = NULL;
	gglClientActiveVertexStreamATI = NULL;
	gglVertexBlendEnviATI = NULL;
	gglVertexBlendEnvfATI = NULL;
	gglElementPointerATI = NULL;
	gglDrawElementArrayATI = NULL;
	gglDrawRangeElementArrayATI = NULL;
	gglDrawMeshArraysSUN = NULL;
	gglGenOcclusionQueriesNV = NULL;
	gglDeleteOcclusionQueriesNV = NULL;
	gglIsOcclusionQueryNV = NULL;
	gglBeginOcclusionQueryNV = NULL;
	gglEndOcclusionQueryNV = NULL;
	gglGetOcclusionQueryivNV = NULL;
	gglGetOcclusionQueryuivNV = NULL;
	gglPointParameteriNV = NULL;
	gglPointParameterivNV = NULL;
	gglActiveStencilFaceEXT = NULL;
	gglElementPointerAPPLE = NULL;
	gglDrawElementArrayAPPLE = NULL;
	gglDrawRangeElementArrayAPPLE = NULL;
	gglMultiDrawElementArrayAPPLE = NULL;
	gglMultiDrawRangeElementArrayAPPLE = NULL;
	gglGenFencesAPPLE = NULL;
	gglDeleteFencesAPPLE = NULL;
	gglSetFenceAPPLE = NULL;
	gglIsFenceAPPLE = NULL;
	gglTestFenceAPPLE = NULL;
	gglFinishFenceAPPLE = NULL;
	gglTestObjectAPPLE = NULL;
	gglFinishObjectAPPLE = NULL;
	gglBindVertexArrayAPPLE = NULL;
	gglDeleteVertexArraysAPPLE = NULL;
	gglGenVertexArraysAPPLE = NULL;
	gglIsVertexArrayAPPLE = NULL;
	gglVertexArrayRangeAPPLE = NULL;
	gglFlushVertexArrayRangeAPPLE = NULL;
	gglVertexArrayParameteriAPPLE = NULL;
	gglDrawBuffersATI = NULL;
	gglProgramNamedParameter4fNV = NULL;
	gglProgramNamedParameter4dNV = NULL;
	gglProgramNamedParameter4fvNV = NULL;
	gglProgramNamedParameter4dvNV = NULL;
	gglGetProgramNamedParameterfvNV = NULL;
	gglGetProgramNamedParameterdvNV = NULL;
	gglVertex2hNV = NULL;
	gglVertex2hvNV = NULL;
	gglVertex3hNV = NULL;
	gglVertex3hvNV = NULL;
	gglVertex4hNV = NULL;
	gglVertex4hvNV = NULL;
	gglNormal3hNV = NULL;
	gglNormal3hvNV = NULL;
	gglColor3hNV = NULL;
	gglColor3hvNV = NULL;
	gglColor4hNV = NULL;
	gglColor4hvNV = NULL;
	gglTexCoord1hNV = NULL;
	gglTexCoord1hvNV = NULL;
	gglTexCoord2hNV = NULL;
	gglTexCoord2hvNV = NULL;
	gglTexCoord3hNV = NULL;
	gglTexCoord3hvNV = NULL;
	gglTexCoord4hNV = NULL;
	gglTexCoord4hvNV = NULL;
	gglMultiTexCoord1hNV = NULL;
	gglMultiTexCoord1hvNV = NULL;
	gglMultiTexCoord2hNV = NULL;
	gglMultiTexCoord2hvNV = NULL;
	gglMultiTexCoord3hNV = NULL;
	gglMultiTexCoord3hvNV = NULL;
	gglMultiTexCoord4hNV = NULL;
	gglMultiTexCoord4hvNV = NULL;
	gglFogCoordhNV = NULL;
	gglFogCoordhvNV = NULL;
	gglSecondaryColor3hNV = NULL;
	gglSecondaryColor3hvNV = NULL;
	gglVertexWeighthNV = NULL;
	gglVertexWeighthvNV = NULL;
	gglVertexAttrib1hNV = NULL;
	gglVertexAttrib1hvNV = NULL;
	gglVertexAttrib2hNV = NULL;
	gglVertexAttrib2hvNV = NULL;
	gglVertexAttrib3hNV = NULL;
	gglVertexAttrib3hvNV = NULL;
	gglVertexAttrib4hNV = NULL;
	gglVertexAttrib4hvNV = NULL;
	gglVertexAttribs1hvNV = NULL;
	gglVertexAttribs2hvNV = NULL;
	gglVertexAttribs3hvNV = NULL;
	gglVertexAttribs4hvNV = NULL;
	gglPixelDataRangeNV = NULL;
	gglFlushPixelDataRangeNV = NULL;
	gglPrimitiveRestartNV = NULL;
	gglPrimitiveRestartIndexNV = NULL;
	gglMapObjectBufferATI = NULL;
	gglUnmapObjectBufferATI = NULL;
	gglStencilOpSeparateATI = NULL;
	gglStencilFuncSeparateATI = NULL;
	gglVertexAttribArrayObjectATI = NULL;
	gglGetVertexAttribArrayObjectfvATI = NULL;
	gglGetVertexAttribArrayObjectivATI = NULL;
	gglAlphaFuncxOES = NULL;
	gglClearColorxOES = NULL;
	gglClearDepthxOES = NULL;
	gglClipPlanexOES = NULL;
	gglColor4xOES = NULL;
	gglDepthRangexOES = NULL;
	gglFogxOES = NULL;
	gglFogxvOES = NULL;
	gglFrustumxOES = NULL;
	gglGetClipPlanexOES = NULL;
	gglGetFixedvOES = NULL;
	gglGetLightxvOES = NULL;
	gglGetMaterialxvOES = NULL;
	gglGetTexEnvxvOES = NULL;
	gglGetTexParameterxvOES = NULL;
	gglLightModelxOES = NULL;
	gglLightModelxvOES = NULL;
	gglLightxOES = NULL;
	gglLightxvOES = NULL;
	gglLineWidthxOES = NULL;
	gglLoadMatrixxOES = NULL;
	gglMaterialxOES = NULL;
	gglMaterialxvOES = NULL;
	gglMultMatrixxOES = NULL;
	gglMultiTexCoord4xOES = NULL;
	gglNormal3xOES = NULL;
	gglOrthoxOES = NULL;
	gglPointParameterxOES = NULL;
	gglPointParameterxvOES = NULL;
	gglPointSizexOES = NULL;
	gglPolygonOffsetxOES = NULL;
	gglRotatexOES = NULL;
	gglSampleCoveragexOES = NULL;
	gglScalexOES = NULL;
	gglTexEnvxOES = NULL;
	gglTexEnvxvOES = NULL;
	gglTexParameterxOES = NULL;
	gglTexParameterxvOES = NULL;
	gglTranslatexOES = NULL;
	gglDepthRangefOES = NULL;
	gglFrustumfOES = NULL;
	gglOrthofOES = NULL;
	gglClipPlanefOES = NULL;
	gglGetClipPlanefOES = NULL;
	gglClearDepthfOES = NULL;
	gglQueryMatrixxOES = NULL;
	gglDepthBoundsEXT = NULL;
	gglBlendEquationSeparateEXT = NULL;
	gglIsRenderbufferEXT = NULL;
	gglBindRenderbufferEXT = NULL;
	gglDeleteRenderbuffersEXT = NULL;
	gglGenRenderbuffersEXT = NULL;
	gglRenderbufferStorageEXT = NULL;
	gglGetRenderbufferParameterivEXT = NULL;
	gglIsFramebufferEXT = NULL;
	gglBindFramebufferEXT = NULL;
	gglDeleteFramebuffersEXT = NULL;
	gglGenFramebuffersEXT = NULL;
	gglCheckFramebufferStatusEXT = NULL;
	gglFramebufferTexture1DEXT = NULL;
	gglFramebufferTexture2DEXT = NULL;
	gglFramebufferTexture3DEXT = NULL;
	gglFramebufferRenderbufferEXT = NULL;
	gglGetFramebufferAttachmentParameterivEXT = NULL;
	gglGenerateMipmapEXT = NULL;
	gglStringMarkerGREMEDY = NULL;
	gglStencilClearTagEXT = NULL;
	gglBlitFramebufferEXT = NULL;
	gglRenderbufferStorageMultisampleEXT = NULL;
	gglBufferRegionEnabledEXT = NULL;
	gglDeleteBufferRegionEXT = NULL;
	gglDrawBufferRegionEXT = NULL;
	gglNewBufferRegionEXT = NULL;
	gglReadBufferRegionEXT = NULL;
	gglReadVideoPixelsSUN = NULL;
	gglAddSwapHintRectWIN = NULL;
}
