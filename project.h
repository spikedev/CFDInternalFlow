#ifndef __PROJECT_H__
#define __PROJECT_H__

#define MINF(a, b) (a < b ? a : b)
#define MAXF(a, b) (a > b ? a : b)

#if !defined(__GNUC__) && defined(RT_X64) && (defined(WIN32) || defined(_WIN32))
//this is to get around the name mangling MSVC does in x64
#define cpu_vendor_id					_cpu_vendor_id
#define cpu_model						_cpu_model
#define cpu_family						_cpu_family
#define cpu_ext_family					_cpu_ext_family
#define extcpu_model					_extcpu_model
#define extcpu_family					_extcpu_family
#define extcpu_ext_family				_extcpu_ext_family
#define cpu_count						_cpu_count
#define cpu_hyperthreading				_cpu_hyperthreading
#define cpu_64_bit						_cpu_64_bit
#define cpu_mmx							_cpu_mmx
#define cpu_sse							_cpu_sse
#define cpu_sse2						_cpu_sse2
#define cpu_sse3						_cpu_sse3
#define cpu_3dnow						_cpu_3dnow
#define cpu_3dnow2						_cpu_3dnow2
#define cpu_rdtsc						_cpu_rdtsc
#define cpu_fpu							_cpu_fpu
#define cpu_fcmov						_cpu_fcmov
#define cpu_extmmx						_cpu_extmmx
#define cpu_ext_vendorid				_cpu_ext_vendorid
#define cpu_extended_info				_cpu_extended_info
#define cpu_ht_thread					_cpu_ht_thread
#define cpu_ssse3						_cpu_ssse3
#define cpu_sse4_1						_cpu_sse4_1
#define cpu_sse4_2						_cpu_sse4_2
#define cpu_sse4a						_cpu_sse4a
#define cpu_sse5a						_cpu_sse5a

#define sbuf							_sbuf
#define thread_info						_thread_info
#define end_threads						_end_threads
#define num_threads						_num_threads
#define gmat							_gmat

#define StartTimer						_StartTimer
#define StopTimer						_StopTimer
#define ThreadSpinWait					_ThreadSpinWait
#define GetCPUInfo						_GetCPUInfo
#define Sqroot_ASM						_Sqroot_ASM
#define RSqroot_ASM						_RSqroot_ASM
#define Sine_ASM						_Sine_ASM
#define Cosine_ASM						_Cosine_ASM
#define Tangent_ASM						_Tangent_ASM
#define VectorMove_ASM					_VectorMove_ASM
#define VectorMoveScalar_ASM			_VectorMoveScalar_ASM
#define VectorAdd_ASM					_VectorAdd_ASM
#define VectorAddScalar_ASM				_VectorAddScalar_ASM
#define VectorSubtract_ASM				_VectorSubtract_ASM
#define VectorSubtractScalar_ASM		_VectorSubtractScalar_ASM
#define VectorMultiply_ASM				_VectorMultiply_ASM
#define VectorMultiplyScalar_ASM		_VectorMultiplyScalar_ASM
#define VectorMultiplyAdd_ASM			_VectorMultiplyAdd_ASM
#define VectorMultiplyAddScalar_ASM		_VectorMultiplyAddScalar_ASM
#define VectorDotProduct3_ASM			_VectorDotProduct3_ASM
#define VectorDotProduct4_ASM			_VectorDotProduct4_ASM
#define VectorDotProductH_ASM			_VectorDotProductH_ASM
#define VectorCrossProduct_ASM			_VectorCrossProduct_ASM
#define VectorNormalize_ASM				_VectorNormalize_ASM
#define CreatePlane_ASM					_CreatePlane_ASM
#define VectorMatrixMultiply3_ASM		_VectorMatrixMultiply3_ASM
#define VectorMatrixMultiply4_ASM		_VectorMatrixMultiply4_ASM
#define VectorMatrixMultiplyH_ASM		_VectorMatrixMultiplyH_ASM
#define MatrixCopy3x3_ASM				_MatrixCopy3x3_ASM
#define MatrixCopy4x4_ASM				_MatrixCopy4x4_ASM
#define MatrixTranspose3x3_ASM			_MatrixTranspose3x3_ASM
#define MatrixTranspose4x4_ASM			_MatrixTranspose4x4_ASM
#define MatrixMultiply3x3_ASM			_MatrixMultiply3x3_ASM
#define MatrixMultiply4x4_ASM			_MatrixMultiply4x4_ASM
#define MatrixLoadIdentity_ASM			_MatrixLoadIdentity_ASM
#define MatrixTranslate_ASM				_MatrixTranslate_ASM
#define MatrixScale_ASM					_MatrixScale_ASM
#define MatrixRotate_ASM				_MatrixRotate_ASM
#endif

#define EPSILON 1e-4
#define DEGTORAD (3.14159265358979323846 / 180.0)		//deg -> rad
#define RADTODEG (180.0 / 3.14159265358979323846)		//rad -> deg

extern unsigned int modulo3[];

#define GOT_TOKEN				0
#define END_OF_LINE				1
#define END_OF_FILE				2

typedef enum
{
	FB_RENDER,
	FB_DSAMPLE,
	FB_HGAUSS,
	FB_HGAUSS2,
	FB_HGAUSS3,
	FB_HGAUSS4,
	FB_VGAUSS,
	FB_VGAUSS2,
	FB_VGAUSS3,
	FB_VGAUSS4,
	FB_RENDER_INT,
	NUM_FBUFFERS
} fbuffer_t;

typedef enum
{
	VP_GENERAL_ARB,
	VP_GENERAL_COLOR_ARB,
	VP_TONEMAP_ARB,
	VP_GENERAL_NV20,
	GLSLV_FXAA_ARB,
	GLSLV_GENERAL_ARB,
	GLSLV_TEXT_ARB,
	NUM_VP
} vp_t;

typedef enum
{
	FP_HORIZ_AA_ARB,
	FP_DOWN_SAMPLE_ARB,
	FP_HORIZ_GAUSS_ARB,
	FP_VERT_GAUSS_ARB,
	FP_TONEMAP_ARB,
	FP_SPRITE_ARB,
	FP_GENERAL_ARB,
	FP_GENERAL_COLOR_ARB,
	FP_FSAA_ARB,
	GLSLF_FXAA_ARB,
	GLSLF_GENERAL_ARB,
	GLSLF_TEXT_ARB,
	NUM_FP
} fp_t;

typedef enum
{
	GP_GENERAL,
	GP_FXAA,
	GP_TEXT,
	NUM_GP
} gp_t;

typedef int index_t[3];

extern byte cpu_vendor_id[13];
extern int cpu_model;
extern int cpu_family;
extern int cpu_ext_family;
extern int extcpu_model;
extern int extcpu_family;
extern int extcpu_ext_family;
extern int cpu_count;
extern bool cpu_hyperthreading;			//pause - this is totally bogus, don't rely on this
extern bool cpu_64_bit;
extern bool cpu_mmx;
extern bool cpu_sse;
extern bool cpu_sse2;
extern bool cpu_sse3;
extern bool cpu_3dnow;
extern bool cpu_3dnow2;
extern bool cpu_rdtsc;
extern bool cpu_fpu;
extern bool cpu_fcmov;
extern bool cpu_extmmx;
extern byte cpu_ext_vendorid[49];
extern bool cpu_extended_info;
extern bool cpu_ht_thread;				//monitor/mwait
extern bool cpu_ssse3;
extern bool cpu_sse4_1;
extern bool cpu_sse4_2;
extern bool cpu_sse4a;
extern bool cpu_sse5a;

typedef struct
{
	int width;
	int height;
	int bitdepth;
	byte *data;
} texture_t;

#pragma pack(push, 4)
//NOTE: keep this a power of 2 size (64 bytes)
typedef struct
{
//	int num;				//thread number
	void *mutex;			//XXX: in 64-bit mode, these are 8 bytes
	void *signal;			//XXX: in 64-bit mode, these are 8 bytes
	int pixel_start;		//pixel on the screen where this threads starts rendering

	bool done;
#ifdef BLOCK_SUBDIV_RENDER
	int num_bsh, num_bsv;	//number of block subdivs
//	int tmpps, tmpss;		//number of pixels/subdivs left in this block
	int tmpbs;				//number of pixels left in block
	int counter;			//pixel counter
	byte p[8];				//TEMP: filler
#else
	int block_size;			//number of pixels in block
	int tmpbs;				//number of pixels left in block
	int counter;			//pixel counter
	byte p[12];				//TEMP: filler
#endif

	int transdone;
	int transdone2;
	int renddone;
	int num_faces;			//number of faces visible from transform/clipping
#ifndef RT_X64
	byte padding[8];
#endif
} threads_t;

#pragma pack(pop)

extern threads_t *thread_info;

void Img_VerticalFlip(texture_t *image);
void Img_LoadBMP(FILE *imgfile, const char *path, texture_t *image, bool alpha);
void Img_WriteBMP(const char *path, texture_t *texture);
void Img_LoadTGA(FILE *fp, const char *path, texture_t *image, bool alpha);
int Img_TGAGetPixel(bool gray_scale, byte *colormap, int colormap_bytes, int bitdepth, FILE *fp);
void Img_LoadJPEG(FILE *fp, const char *path, texture_t *image, bool alpha);

byte *ReadShader(const char *name);
dword LoadGLSLProgram(dword *frag, dword *vert, const char *fpath, const char *vpath);
void CheckFramebufferStatus(void);
void SetupRPath(rpath_e path);
int ThreadMain(void *reserved);
void Print(const char *stream, ...);
void RenderPixels(const int thread_num);
void HandleInput(const int ticks);

//generic replacement math functions
double __cdecl Sqroot_Generic(const double value);
double __cdecl RSqroot_Generic(const double value);
double __cdecl Sine_Generic(const double theta);
double __cdecl Cosine_Generic(const double theta);
double __cdecl Tangent_Generic(const double theta);
void __cdecl VectorMove_Generic(float *dest, const float *src);
void __cdecl VectorMoveScalar_Generic(float *dest, const float src);
void __cdecl VectorAdd_Generic(float *dest, const float *src1, const float *src2);
void __cdecl VectorAddScalar_Generic(float *dest, const float *src1, const float src2);
void __cdecl VectorSubtract_Generic(float *dest, const float *src1, const float *src2);
void __cdecl VectorSubtractScalar_Generic(float *dest, const float *src1, const float src2);
void __cdecl VectorMultiply_Generic(float *dest, const float *src1, const float *src2);
void __cdecl VectorMultiplyScalar_Generic(float *dest, const float *src1, const float src2);
void __cdecl VectorMultiplyAdd_Generic(float *dest, const float *src1, const float *src2, const float *src3);
void __cdecl VectorMultiplyAddScalar_Generic(float *dest, const float *src1, const float src2, const float *src3);
float __cdecl VectorDotProduct3_Generic(const float *src1, const float *src2);
float __cdecl VectorDotProduct4_Generic(const float *src1, const float *src2);
float __cdecl VectorDotProductH_Generic(const float *src1, const float *src2);
void __cdecl VectorCrossProduct_Generic(float *dest, const float *src1, const float *src2);
void __cdecl VectorNormalize_Generic(float *vec);
void __cdecl CreatePlane_Generic(float *plane, const float *v1, const float *v2, const float *v3);
void __cdecl VectorMatrixMultiply3_Generic(float *out, const float *vec, const float *mat);
void __cdecl VectorMatrixMultiply4_Generic(float *out, const float *vec, const float *mat);
void __cdecl VectorMatrixMultiplyH_Generic(float *out, const float *vec, const float *mat);
void __cdecl MatrixCopy3x3_Generic(float *out, const float *mat);
void __cdecl MatrixCopy4x4_Generic(float *out, const float *mat);
void __cdecl MatrixTranspose3x3_Generic(float *out, const float *mat);
void __cdecl MatrixTranspose4x4_Generic(float *out, const float *mat);
void __cdecl MatrixMultiply3x3_Generic(float *out, const float *mat1, const float *mat2);
void __cdecl MatrixMultiply4x4_Generic(float *out, const float *mat1, const float *mat2);
void __cdecl MatrixLoadIdentity_Generic(void);
void __cdecl MatrixTranslate_Generic(const float *vpos);
void __cdecl MatrixScale_Generic(const float *sfac);
void __cdecl MatrixRotate_Generic(const float *angles);

void __cdecl StartTimer(void);
int __cdecl StopTimer(void);
int __cdecl ThreadSpinWait(const int thread_num);
int __cdecl GetCPUInfo(void);

//SSE2 optimized replacement math functions
double __cdecl Sqroot_ASM(const double value);
double __cdecl RSqroot_ASM(const double value);
double __cdecl Sine_ASM(const double theta);
double __cdecl Cosine_ASM(const double theta);
double __cdecl Tangent_ASM(const double theta);
void __cdecl VectorMove_ASM(float *dest, const float *src);
void __cdecl VectorMoveScalar_ASM(float *dest, const float src);
void __cdecl VectorAdd_ASM(float *dest, const float *src1, const float *src2);
void __cdecl VectorAddScalar_ASM(float *dest, const float *src1, const float src2);
void __cdecl VectorSubtract_ASM(float *dest, const float *src1, const float *src2);
void __cdecl VectorSubtractScalar_ASM(float *dest, const float *src1, const float src2);
void __cdecl VectorMultiply_ASM(float *dest, const float *src1, const float *src2);
void __cdecl VectorMultiplyScalar_ASM(float *dest, const float *src1, const float src2);
void __cdecl VectorMultiplyAdd_ASM(float *dest, const float *src1, const float *src2, const float *src3);
void __cdecl VectorMultiplyAddScalar_ASM(float *dest, const float *src1, const float src2, const float *src3);
float __cdecl VectorDotProduct3_ASM(const float *src1, const float *src2);
float __cdecl VectorDotProduct4_ASM(const float *src1, const float *src2);
float __cdecl VectorDotProductH_ASM(const float *src1, const float *src2);
void __cdecl VectorCrossProduct_ASM(float *dest, const float *src1, const float *src2);
void __cdecl VectorNormalize_ASM(float *vec);
void __cdecl CreatePlane_ASM(float *plane, const float *v1, const float *v2, const float *v3);
void __cdecl VectorMatrixMultiply3_ASM(float *out, const float *vec, const float *mat);
void __cdecl VectorMatrixMultiply4_ASM(float *out, const float *vec, const float *mat);
void __cdecl VectorMatrixMultiplyH_ASM(float *out, const float *vec, const float *mat);
void __cdecl MatrixCopy3x3_ASM(float *out, const float *mat);
void __cdecl MatrixCopy4x4_ASM(float *out, const float *mat);
void __cdecl MatrixTranspose3x3_ASM(float *out, const float *mat);
void __cdecl MatrixTranspose4x4_ASM(float *out, const float *mat);
void __cdecl MatrixMultiply3x3_ASM(float *out, const float *mat1, const float *mat2);
void __cdecl MatrixMultiply4x4_ASM(float *out, const float *mat1, const float *mat2);
void __cdecl MatrixLoadIdentity_ASM(void);
void __cdecl MatrixTranslate_ASM(const float *vpos);
void __cdecl MatrixScale_ASM(const float *sfac);
void __cdecl MatrixRotate_ASM(const float *angles);

extern double (__cdecl *Sqroot)(const double value);
extern double (__cdecl *RSqroot)(const double value);
extern double (__cdecl *Sine)(const double theta);
extern double (__cdecl *Cosine)(const double theta);
extern double (__cdecl *Tangent)(const double theta);
extern void (__cdecl *VectorMove)(float *dest, const float *src);
extern void (__cdecl *VectorMoveScalar)(float *dest, const float src);
extern void (__cdecl *VectorAdd)(float *dest, const float *src1, const float *src2);
extern void (__cdecl *VectorAddScalar)(float *dest, const float *src1, const float src2);
extern void (__cdecl *VectorSubtract)(float *dest, const float *src1, const float *src2);
extern void (__cdecl *VectorSubtractScalar)(float *dest, const float *src1, const float src2);
extern void (__cdecl *VectorMultiply)(float *dest, const float *src1, const float *src2);
extern void (__cdecl *VectorMultiplyScalar)(float *dest, const float *src1, const float src2);
extern void (__cdecl *VectorMultiplyAdd)(float *dest, const float *src1, const float *src2, const float *src3);
extern void (__cdecl *VectorMultiplyAddScalar)(float *dest, const float *src1, const float src2, const float *src3);
extern float (__cdecl *VectorDotProduct3)(const float *src1, const float *src2);
extern float (__cdecl *VectorDotProduct4)(const float *src1, const float *src2);
extern float (__cdecl *VectorDotProductH)(const float *src1, const float *src2);
extern void (__cdecl *VectorCrossProduct)(float *dest, const float *src1, const float *src2);
extern void (__cdecl *VectorNormalize)(float *vec);
extern void (__cdecl *CreatePlane)(float *plane, const float *v1, const float *v2, const float *v3);
extern void (__cdecl *VectorMatrixMultiply3)(float *out, const float *vec, const float *mat);
extern void (__cdecl *VectorMatrixMultiply4)(float *out, const float *vec, const float *mat);
extern void (__cdecl *VectorMatrixMultiplyH)(float *out, const float *vec, const float *mat);
extern void (__cdecl *MatrixCopy3x3)(float *out, const float *mat);
extern void (__cdecl *MatrixCopy4x4)(float *out, const float *mat);
extern void (__cdecl *MatrixTranspose3x3)(float *out, const float *mat);
extern void (__cdecl *MatrixTranspose4x4)(float *out, const float *mat);
extern void (__cdecl *MatrixMultiply3x3)(float *out, const float *mat1, const float *mat2);
extern void (__cdecl *MatrixMultiply4x4)(float *out, const float *mat1, const float *mat2);
extern void (__cdecl *MatrixLoadIdentity)(void);
extern void (__cdecl *MatrixTranslate)(const float *vpos);
extern void (__cdecl *MatrixScale)(const float *sfac);
extern void (__cdecl *MatrixRotate)(const float *angles);

#endif
