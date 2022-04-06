#include <math.h>
#include "opengl.h"
#include "main.h"
#include "project.h"
#include "cfd.h"

//#define	DEBUG_MATH					//uses generic math functions only
//#define	FPU_MATH_ONLY				//uses FPU versions of the math functions only

//TODO: go thru the asm code and benchmark different instruction uses for speed
//TODO: use MONITOR/MWAIT for HT cpus during spins
//TODO: fix the stupid gcc aligment problem and get rid of the declarations in the asm file

//TODO: make NV versions of HDR fragment programs
//TODO: fix CreatePlane plane[3] computation in the ASM versions
//NOTE: when using NV_float_buffer, rectangle textures must be used all the time
//NOTE: if ARB_texture_float or ATI_texture_float exist, NV_float_buffer can use texture filtering,
//otherwise no filtering can be applied
//NOTE: compiler with optimizations on will allocate space for variables defined outside of the
//function on the stack, therefore won't align it properly in memory; use static to force it to
//allocate these outside the stack space
//NOTE: when compiler optimization is enabled, spins can be completely skipped in the code if
//they aren't defined properly
//NOTE: when using VisualC++ compiler, longs are still 32-bits wide in x64 (LLP64), otherwise
//they are assumed 64-bits wide (LP64) : use _long instead

/*
 *---------------------------------------
 * coordinate system usage:
 *
 * y
 * ^  z (neg. into screen)
 * | ^
 * |/
 * o-----> x
 *---------------------------------------
*/

/*
 *---------------------------------------
 *  positive rotation:
 *
 *  x axis - up
 *  y axis - left
 *  z axis - clockwise??? //TODO: check this
 *---------------------------------------
*/

//TODO: should this be swapped??? .obj file format uses opposite as default
/*
 *---------------------------------------
 * positive winding:
 * (normal points out of screen)
 *
 *     2
 *    / \
 *   1 - 3
 *---------------------------------------
*/

/*
 *-----------------------------------
 * matricies are defined as:
 * 		0  1  2  3
 * 		4  5  6  7
 * 		8  9  10 11
 * 		12 13 14 15
 *-----------------------------------
*/

#define NUM_BENCH_FRAMES 150
#define TRIG_GRAN 512

int fbuffers[NUM_FBUFFERS];
int fbtexs[NUM_FBUFFERS];
int rdbuffers[NUM_FBUFFERS];
int bufwidth, bufheight;

vec4 zero = {0.0f, 0.0f, 0.0f, 0.0f};

bool usefxaa = true;
bool benchmark = false;
bool end_threads = false;			//set this to true to terminate all the threads
void *rmutex = NULL, *glmutex = NULL;
void *rendersig = NULL;
bool anim = false;					//animate the transient results
bool capture = false;

texture_t screenshot;
char sname[42];
int srcount = 0;
byte *screen_buffer = NULL;

#define FONT_Y_INC 14.0f
#define BINDINGS_LEFT_ALIGN 480.0f
float fontpos;
GLuint list_font = 0;
GLuint vertex_shaders[NUM_VP];
GLuint fragment_shaders[NUM_FP];
GLuint glslprograms[NUM_GP];
//TODO:
int uniloc[3];
int pixels;
float *gmat = NULL;					//gmat[16]

float v_width, v_height;
int _width;							//same as width (to avoid name mangling in MSVC x64)

bool clipped_filled;
int currentbuf = 0;					//active buffer
_long sbufi[2];						//screen buffer (pointers; front, back) for fast switching
byte *sbuf = NULL;					//pointer to active buffer (for compatibility)
byte *sbuf_front = NULL;			//front buffer
byte *sbuf_back = NULL;				//back buffer
threads_t *thread_info = NULL;

int counter = 0, timer = 0, endtimer = 0;
int tmp_time;

//replacement math functions
double (__cdecl *Sqroot)(const double value);
double (__cdecl *RSqroot)(const double value);
double (__cdecl *Sine)(const double theta);
double (__cdecl *Cosine)(const double theta);
double (__cdecl *Tangent)(const double theta);
void (__cdecl *VectorMove)(float *dest, const float *src);
void (__cdecl *VectorMoveScalar)(float *dest, const float src);
void (__cdecl *VectorAdd)(float *dest, const float *src1, const float *src2);
void (__cdecl *VectorAddScalar)(float *dest, const float *src1, const float src2);
void (__cdecl *VectorSubtract)(float *dest, const float *src1, const float *src2);
void (__cdecl *VectorSubtractScalar)(float *dest, const float *src1, const float src2);
void (__cdecl *VectorMultiply)(float *dest, const float *src1, const float *src2);
void (__cdecl *VectorMultiplyScalar)(float *dest, const float *src1, const float src2);
void (__cdecl *VectorMultiplyAdd)(float *dest, const float *src1, const float *src2, const float *src3);
void (__cdecl *VectorMultiplyAddScalar)(float *dest, const float *src1, const float src2, const float *src3);
float (__cdecl *VectorDotProduct3)(const float *src1, const float *src2);
float (__cdecl *VectorDotProduct4)(const float *src1, const float *src2);
float (__cdecl *VectorDotProductH)(const float *src1, const float *src2);
void (__cdecl *VectorCrossProduct)(float *dest, const float *src1, const float *src2);
void (__cdecl *VectorNormalize)(float *vec);
void (__cdecl *CreatePlane)(float *plane, const float *v1, const float *v2, const float *v3);
void (__cdecl *VectorMatrixMultiply3)(float *out, const float *vec, const float *mat);
void (__cdecl *VectorMatrixMultiply4)(float *out, const float *vec, const float *mat);
void (__cdecl *VectorMatrixMultiplyH)(float *out, const float *vec, const float *mat);
void (__cdecl *MatrixCopy3x3)(float *out, const float *mat);
void (__cdecl *MatrixCopy4x4)(float *out, const float *mat);
void (__cdecl *MatrixTranspose3x3)(float *out, const float *mat);
void (__cdecl *MatrixTranspose4x4)(float *out, const float *mat);
void (__cdecl *MatrixMultiply3x3)(float *out, const float *mat1, const float *mat2);
void (__cdecl *MatrixMultiply4x4)(float *out, const float *mat1, const float *mat2);
void (__cdecl *MatrixLoadIdentity)(void);
void (__cdecl *MatrixTranslate)(const float *vpos);
void (__cdecl *MatrixScale)(const float *sfac);
void (__cdecl *MatrixRotate)(const float *angles);

/*
----------------
ReadShader

- name:			path and name for the shader to be read in
- return:		allocates and returns a pointer to the containing buffer
----------------
*/
byte *ReadShader(const char *name)
{
	int length;
	dword numbytes;
	byte *buffer;
	sysfile_handle_t *fp;

	if(!(fp = Sys_OpenFile(name, FM_READ)))
		Sys_Error("ReadShader: Failed to open shader %s", name);

	length = (int)Sys_GetFileLength(fp);

	if(!(buffer = (byte *)calloc(length + 1, sizeof(byte))))
		Sys_Error("ReadShader: Failed to allocate shader buffer");

	numbytes = Sys_ReadFile(fp, length, buffer);
	buffer[numbytes] = '\0';
	Sys_CloseFile(fp);
	return buffer;
}

static bool firstcompilelog = true;

/*
----------------
LoadGLSLProgram

- frag:			returns the fragment shader handle
- vert:			returns the vertex shader handle
- fpath:		path and name to the fragment shader
- vpath:		path and name to the vertex shader
- return:		GLSL program shader handle
----------------
*/
dword LoadGLSLProgram(dword *frag, dword *vert, const char *fpath, const char *vpath)
{
	dword glslprog;
	byte *shader_string;
	int length, maxlength, compiled;
	char *clog;
	FILE *flog;

	if(!(glslprog = gglCreateProgramObjectARB()))
		Sys_Error("LoadGLSLProgram: Failed to create GLSL program object");

	if(!(*frag = gglCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB)))
		Sys_Error("LoadGLSLProgram: Failed to create GLSL fragment shader object");

	if(!(*vert = gglCreateShaderObjectARB(GL_VERTEX_SHADER_ARB)))
		Sys_Error("LoadGLSLProgram: Failed to create GLSL vertex shader object");

	shader_string = ReadShader(fpath);
	gglShaderSourceARB(*frag, 1, &shader_string, NULL);
	gglCompileShaderARB(*frag);
	free(shader_string);
	gglGetObjectParameterivARB(*frag, GL_OBJECT_COMPILE_STATUS_ARB, &compiled);
	gglGetObjectParameterivARB(*frag, GL_OBJECT_INFO_LOG_LENGTH_ARB, &maxlength);
	if(!(clog = (char *)malloc(maxlength)))
		Sys_Error("LoadGLSLProgram: Failed to allocate memory for loading GLSL shaders");

	gglGetInfoLogARB(*frag, maxlength, &length, clog);
	if(firstcompilelog)
	{
		if(!(flog = fopen("compilelog.txt", "w")))
			Sys_Error("LoadGLSLProgram: Failed to open compilelog.txt file for writing");
	}
	else
	{
		if(!(flog = fopen("compilelog.txt", "a")))
			Sys_Error("LoadGLSLProgram: Failed to open compilelog.txt file for writing");
	}

	if(!firstcompilelog)
		fprintf(flog, "\n\n");
	else
		firstcompilelog = false;

	fprintf(flog, "==== Fragment Shader Log --%s-- ====\n", fpath);
	fprintf(flog, "%s", clog);
	fclose(flog);

	if(!compiled)
		Sys_Error("LoadGLSLProgram: Failed to compile fragment shader %s", fpath);

	shader_string = ReadShader(vpath);
	gglShaderSourceARB(*vert, 1, &shader_string, NULL);
	gglCompileShaderARB(*vert);
	free(shader_string);
	gglGetObjectParameterivARB(*vert, GL_OBJECT_COMPILE_STATUS_ARB, &compiled);
	gglGetObjectParameterivARB(*vert, GL_OBJECT_INFO_LOG_LENGTH_ARB, &maxlength);
	if(!(clog = (char *)malloc(maxlength)))
		Sys_Error("LoadGLSLProgram: Failed to allocate memory for loading GLSL shaders");

	gglGetInfoLogARB(*vert, maxlength, &length, clog);
	if(!(flog = fopen("compilelog.txt", "a")))
		Sys_Error("LoadGLSLProgram: Failed to open compilelog.txt file for writing");

	fprintf(flog, "\n\n");
	fprintf(flog, "==== Vertex Shader Log --%s-- ====\n", vpath);
	fprintf(flog, "%s", clog);
	fclose(flog);

	if(!compiled)
		Sys_Error("LoadGLSLProgram: Failed to compile vertex shader %s", vpath);

	gglAttachObjectARB(glslprog, *frag);
	gglAttachObjectARB(glslprog, *vert);
	gglLinkProgramARB(glslprog);

	gglDeleteObjectARB(*frag);
	gglDeleteObjectARB(*vert);

	gglGetObjectParameterivARB(glslprog, GL_OBJECT_LINK_STATUS_ARB, &compiled);

	if(!compiled)
		Sys_Error("LoadGLSLProgram: Failed to link shaders");

	return glslprog;
}

/*
----------------
CheckFramebufferStatus
----------------
*/
void CheckFramebufferStatus(void)
{
    int status;

    status = gglCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);

    switch(status)
	{
		case GL_FRAMEBUFFER_COMPLETE_EXT:
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
			Sys_Error("CheckFramebufferStatus: Framebuffer incomplete attachment");
			break;
				
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
			Sys_Error("CheckFramebufferStatus: Framebuffer incomplete, missing attachment\n");
			break;

			//XXX: ???
//		case GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT_EXT:
//			Sys_Error("CheckFramebufferStatus: Framebuffer incomplete, duplicate attachment\n");
//			break;

		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
			Sys_Error("CheckFramebufferStatus: Framebuffer incomplete, attached images must have same dimensions\n");
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
			Sys_Error("CheckFramebufferStatus: Framebuffer incomplete, attached images must have same format\n");
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
			Sys_Error("CheckFramebufferStatus: Framebuffer incomplete, missing draw buffer\n");
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
			Sys_Error("CheckFramebufferStatus: Framebuffer incomplete, missing read buffer\n");
			break;

		case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
            Sys_Error("CheckFramebufferStatus: Unsupported framebuffer format\n");
            break;

		default:
			Sys_Error("CheckFramebufferStatus: Unknown framebuffer status %x", status);
    }
}

/*
----------------
SetupCores
----------------
*/
void SetupCores(void)
{
	int i;

	GetCPUInfo();

	if(!cpu_fpu)
		Sys_Error("SetupCores: CFD requires a Floating-Point Processor to run");

	if(si.dumpcpuid)
	{
		FILE *cfp;

		if(!(cfp = fopen("cpuid.txt", "w")))
			Sys_Error("SetupCores: Unable to open file cpuid.txt");

		fprintf(cfp, "Vendor ID: %s\n", cpu_vendor_id);
		fprintf(cfp, "Extended Vendor ID: %s\n", cpu_ext_vendorid);
		fprintf(cfp, "Model: %d\n", cpu_model);
		fprintf(cfp, "Family: %d\n", cpu_family);
		fprintf(cfp, "Extended Family: %d\n", cpu_ext_family);
		fprintf(cfp, "Ext Model: %d\n", extcpu_model);
		fprintf(cfp, "Ext Family: %d\n", extcpu_family);
		fprintf(cfp, "Ext Extended Family: %d\n", extcpu_ext_family);
		fprintf(cfp, "CPU Count: %d\n", cpu_count);
		fprintf(cfp, "PAUSE: %d\n", cpu_hyperthreading);
		fprintf(cfp, "x64: %d\n", cpu_64_bit);
		fprintf(cfp, "MMX: %d\n", cpu_mmx);
		fprintf(cfp, "MMX+: %d\n", cpu_extmmx);
		fprintf(cfp, "SSE: %d\n", cpu_sse);
		fprintf(cfp, "SSE2: %d\n", cpu_sse2);
		fprintf(cfp, "SSE3: %d\n", cpu_sse3);
		fprintf(cfp, "SSSE3: %d\n", cpu_ssse3);
		fprintf(cfp, "SSE4.1: %d\n", cpu_sse4_1);
		fprintf(cfp, "SSE4.2: %d\n", cpu_sse4_2);
		fprintf(cfp, "SSE4a: %d\n", cpu_sse4a);
		fprintf(cfp, "SSE5a: %d\n", cpu_sse5a);
		fprintf(cfp, "3DNow: %d\n", cpu_3dnow);
		fprintf(cfp, "3DNow+: %d\n", cpu_3dnow2);
		fprintf(cfp, "RDTSC: %d\n", cpu_rdtsc);
		fprintf(cfp, "FCMOV: %d\n", cpu_fcmov);
		fprintf(cfp, "FPU: %d\n", cpu_fpu);
		fprintf(cfp, "MONITOR/MWAIT: %d\n", cpu_ht_thread);
		fclose(cfp);
	}

	//create the threads
	if(si.num_threads <= 0)
	{
		if(cpu_count)
			si.num_threads = cpu_count;
		else
			si.num_threads = 1;

//		if(cpu_hyperthreading)
//			num_threads *= 2;
	}

	if(!(thread_info = (threads_t *)_malloc_aligned(si.num_threads * sizeof(threads_t), 16)))
		Sys_Error("SetupCores: Failed to allocate memory for the threads");

	memset(thread_info, 0, si.num_threads * sizeof(threads_t));

	for(i = 0; i < si.num_threads; i++)
	{
		thread_info[i].transdone = thread_info[i].transdone2 = 0;
		thread_info[i].renddone = 1;
	}

	if(si.num_threads > 1)
	{
		if(!Sys_CreateThreads(si.num_threads - 1))
		{
			si.num_threads = 1;
			Sys_Log("-->Failed to create threads; using single threaded renderer");
		}
	}
}

/*
----------------
SetupRPath
----------------
*/
void SetupRPath(rpath_e path)
{
	switch(path)
	{
		case R_GENERIC:
			Sqroot = Sqroot_Generic;
			RSqroot = RSqroot_Generic;
			Sine = Sine_Generic;
			Cosine = Cosine_Generic;
			Tangent = Tangent_Generic;
			VectorMove = VectorMove_Generic;
			VectorMoveScalar = VectorMoveScalar_Generic;
			VectorAdd = VectorAdd_Generic;
			VectorAddScalar = VectorAddScalar_Generic;
			VectorSubtract = VectorSubtract_Generic;
			VectorSubtractScalar = VectorSubtractScalar_Generic;
			VectorMultiply = VectorMultiply_Generic;
			VectorMultiplyScalar = VectorMultiplyScalar_Generic;
			VectorMultiplyAdd = VectorMultiplyAdd_Generic;
			VectorMultiplyAddScalar = VectorMultiplyAddScalar_Generic;
			VectorDotProduct3 = VectorDotProduct3_Generic;
			VectorDotProduct4 = VectorDotProduct4_Generic;
			VectorDotProductH = VectorDotProductH_Generic;
			VectorCrossProduct = VectorCrossProduct_Generic;
			VectorNormalize = VectorNormalize_Generic;
			CreatePlane = CreatePlane_Generic;
			VectorMatrixMultiply3 = VectorMatrixMultiply3_Generic;
			VectorMatrixMultiply4 = VectorMatrixMultiply4_Generic;
			VectorMatrixMultiplyH = VectorMatrixMultiplyH_Generic;
			MatrixCopy3x3 = MatrixCopy3x3_Generic;
			MatrixCopy4x4 = MatrixCopy4x4_Generic;
			MatrixTranspose3x3 = MatrixTranspose3x3_Generic;
			MatrixTranspose4x4 = MatrixTranspose4x4_Generic;
			MatrixMultiply3x3 = MatrixMultiply3x3_Generic;
			MatrixMultiply4x4 = MatrixMultiply4x4_Generic;
			MatrixLoadIdentity = MatrixLoadIdentity_Generic;
			MatrixTranslate = MatrixTranslate_Generic;
			MatrixScale = MatrixScale_Generic;
			MatrixRotate = MatrixRotate_Generic;
			Sys_Log("-- Using GENERIC render path --");
			break;

		case R_ASM:
			Sqroot = Sqroot_ASM;
			RSqroot = RSqroot_ASM;
			Sine = Sine_ASM;
			Cosine = Cosine_ASM;
			Tangent = Tangent_ASM;
			VectorMove = VectorMove_ASM;
			VectorMoveScalar = VectorMoveScalar_ASM;
			VectorAdd = VectorAdd_ASM;
			VectorAddScalar = VectorAddScalar_ASM;
			VectorSubtract = VectorSubtract_ASM;
			VectorSubtractScalar = VectorSubtractScalar_ASM;
			VectorMultiply = VectorMultiply_ASM;
			VectorMultiplyScalar = VectorMultiplyScalar_ASM;
			VectorMultiplyAdd = VectorMultiplyAdd_ASM;
			VectorMultiplyAddScalar = VectorMultiplyAddScalar_ASM;
			VectorDotProduct3 = VectorDotProduct3_ASM;
			VectorDotProductH = VectorDotProductH_ASM;
			VectorDotProduct4 = VectorDotProduct4_ASM;
			VectorCrossProduct = VectorCrossProduct_ASM;
			VectorNormalize = VectorNormalize_ASM;
			CreatePlane = CreatePlane_ASM;
			VectorMatrixMultiply3 = VectorMatrixMultiply3_ASM;
			VectorMatrixMultiply4 = VectorMatrixMultiply4_ASM;
			VectorMatrixMultiplyH = VectorMatrixMultiplyH_ASM;
			MatrixCopy3x3 = MatrixCopy3x3_ASM;
			MatrixCopy4x4 = MatrixCopy4x4_ASM;
			MatrixTranspose3x3 = MatrixTranspose3x3_ASM;
			MatrixTranspose4x4 = MatrixTranspose4x4_ASM;
			MatrixMultiply3x3 = MatrixMultiply3x3_ASM;
			MatrixMultiply4x4 = MatrixMultiply4x4_ASM;
			MatrixLoadIdentity = MatrixLoadIdentity_ASM;
			MatrixTranslate = MatrixTranslate_ASM;
			MatrixScale = MatrixScale_ASM;
			MatrixRotate = MatrixRotate_ASM;
			Sys_Log("-- Using ASM render path --");
	}
}

/*
----------------
Init
----------------
*/
void Init(void)
{
	HFONT font, oldfont;
//	byte *shader_string;
	int i;

	_width = si.width;

	if(si.r_path == R_DETECT)
	{
		if(cpu_sse)
			si.r_path = R_ASM;
		else
			si.r_path = R_GENERIC;
	}

	SetupRPath(si.r_path);
	pixels = si.width * si.height;

	Sys_Log("Initializing render threads");

	//start positions on the screen for thread blocks
	if(si.num_threads > 1)
	{
		int n;

		for(i = 0; i < si.num_threads; i++)
		{
			n = si.height / si.num_threads;
			thread_info[i].pixel_start = i * n * si.width;
			thread_info[i].block_size = n * si.width;

			if(i == (si.num_threads - 1))
				thread_info[i].block_size += ((si.height % si.num_threads) * si.width);
		}
	}
	else
	{
		thread_info[0].pixel_start = 0;
		thread_info[0].block_size = pixels;
	}

	Sys_Log("Allocating front and back render buffers");

	if(!(sbuf_front = (byte *)_malloc_aligned(pixels * 4 * sizeof(byte), 16)))
		Sys_Error("Init: Failed to allocate memory for the front render buffer");

	if(!(sbuf_back = (byte *)_malloc_aligned(pixels * 4 * sizeof(byte), 16)))
		Sys_Error("Init: Failed to allocate memory for the back render buffer");

Sys_Log("-	Front/back buffer mem allocated = %d bytes", pixels * 4 * sizeof(byte) * 2);

	sbufi[0] = (_long)sbuf_front;
	sbufi[1] = (_long)sbuf_back;
//	currentbuf = 0;
	sbuf = (byte *)sbufi[currentbuf];

	memset(sbuf, 0, pixels * 4);

	if(!(screen_buffer = (byte *)calloc(si.width * si.height * 3, sizeof(byte))))
		Sys_Error("Init: Failed to allocate memory for the screenshot buffer");

Sys_Log("-	Screenshot buffer mem allocated = %d bytes", si.width * si.height * 3 * sizeof(byte));

	Sys_Log("Preparing the clipping memory block and thread states");

	v_width = (float)si.width;
	v_height = (float)si.height;

	for(i = 0; i < si.num_threads; i++)
		thread_info[i].renddone = 1;

	bufwidth = si.width;
	bufheight = si.height;

	//-----------------------------------------------------------------

	FinalProject();

	//-----------------------------------------------------------------

	Sys_Log("Loading framebuffer textures to video memory");

	//GL_ARB_shader_objects
	//GL_ARB_fragment_shader
	//GL_ARB_vertex_shader
	//
	//may use GL_ARB_texture_gather or GL_ARB_gpu_shader5 or GL_NV_gpu_shader5 (FXAA_GATHER4_ALPHA)
	//may use GL_EXT_gpu_shader4 or GL_ARB_gpu_shader4 (FXAA_FAST_PIXEL_OFFSET)

	//TODO: requires GL_ARB_shader_objects
	glslprograms[GP_GENERAL] = LoadGLSLProgram(&fragment_shaders[GLSLF_GENERAL_ARB], &vertex_shaders[GLSLV_GENERAL_ARB], "shaders/general_frag.glsl", "shaders/general_vert.glsl");
	glslprograms[GP_FXAA] = LoadGLSLProgram(&fragment_shaders[GLSLF_FXAA_ARB], &vertex_shaders[GLSLV_FXAA_ARB], "shaders/FXAA_frag.glsl", "shaders/FXAA_vert.glsl");
//	glslprograms[GP_FXAA] = LoadGLSLProgram(&fragment_shaders[GLSLF_FXAA_ARB], &vertex_shaders[GLSLV_FXAA_ARB], "shaders/DLAA_frag.glsl", "shaders/DLAA_vert.glsl");
	glslprograms[GP_TEXT] = LoadGLSLProgram(&fragment_shaders[GLSLF_TEXT_ARB], &vertex_shaders[GLSLV_TEXT_ARB], "shaders/text_frag.glsl", "shaders/text_vert.glsl");

	//TODO:
	uniloc[0] = gglGetUniformLocationARB(glslprograms[GP_FXAA], "intexture");
//	uniloc[1] = gglGetUniformLocationARB(glslprograms[GP_FXAA], "bgl_RenderedTextureWidth");
//	uniloc[2] = gglGetUniformLocationARB(glslprograms[GP_FXAA], "bgl_RenderedTextureHeight");

	//get shading language version
	//TODO: requires GL_ARB_shading_language_100
	Sys_Log("\tShader Language Version available: %s", gglGetString(GL_SHADING_LANGUAGE_VERSION_ARB));
	Sys_Log("Loading ASCII character list");

	//build ascii characters
	list_font = gglGenLists(96);

	font = CreateFont(-14, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH, "Arial");

	oldfont = (HFONT)SelectObject(si.hDC, font);
	gwglUseFontBitmaps(si.hDC, 32, 96, list_font);
	SelectObject(si.hDC, oldfont);
	DeleteObject(font);
}

/*
----------------
Render
----------------
*/
void Render(void)
{
	static int oldtime = 0;
	int time, curtime, i, j;
//	byte *tmpsbuf;

	//calculate the frame rate
	curtime = Sys_GetTime();

	time = curtime - oldtime;
	tmp_time = time;
	oldtime = curtime;

	//benchmarking stuff
	if(benchmark)
	{
		if(counter == 0)
			timer = Sys_GetTime();
		
		if(counter == (NUM_BENCH_FRAMES - 1))
		{
			FILE *fp;

			endtimer = Sys_GetTime();

			if(!(fp = fopen("benchmark.txt", "a")))
				Sys_Message("Unable to open benchmark output file benchmark.txt");
			else
			{
				fprintf(fp, "\n=========================================\n");
				fprintf(fp, "Benchmark Utility for CFD Internal Flow\n");
				fprintf(fp, "%s\n", Sys_Timestamp());
				fprintf(fp, "Threads: %d\n", si.num_threads);
				switch(si.r_path)
				{
					case R_GENERIC:
						fprintf(fp, "-- Using GENERIC render path --\n");
						break;

					case R_ASM:
						fprintf(fp, "-- Using ASM render path --\n");
				}

				fprintf(fp, "----------------------\n\n");
				fprintf(fp, "Total Frames: %d\n", counter + 1);
				fprintf(fp, "Total Time: %.3f seconds\n", (float)(endtimer - timer) / 1000.0f);
				fprintf(fp, "=========================================\n\n\n\n");
				fclose(fp);

				counter = -1;
				timer = 0;
				endtimer = 0;
				benchmark = false;
			}
		}

		counter++;
	}

	HandleInput(tmp_time);

	if(si.num_threads > 1)
	{
		while(1)
		{
			i = 0;
			for(j = 1; j < si.num_threads; j++)
			{
				if(!thread_info[j].renddone)
				{
					i = 1;
					break;
				}
			}

			if(!i)
				break;
		}
	}

	for(i = 0; i < si.num_threads; i++)
		thread_info[i].renddone = 0;

	//swap buffers
	currentbuf = !currentbuf;
	sbuf = (byte *)sbufi[currentbuf];

	if(si.num_threads > 1)
	{
		for(i = 0; i < (si.num_threads - 1); i++)
			Sys_SignalThread(thread_info[i + 1].signal);
	}

	RenderPixels(0);

	thread_info[0].renddone = 1;

	//-----------------------------------------------------------------
#if 0
	gglUseProgramObjectARB(glslprograms[GP_GENERAL]);

	//for FSAA
	if(usefxaa)
	{
		gglBindTexture(GL_TEXTURE_2D, 0);
		gglBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbuffers[FB_RENDER_INT]);
	}

	gglDisable(GL_DEPTH_TEST);
	gglDisable(GL_CULL_FACE);

	gglViewport(0, 0, (GLsizei)width, (GLsizei)height);
	gglMatrixMode(GL_PROJECTION);

	gglLoadIdentity();
	gglOrtho(0.0, (GLdouble)v_width, (GLdouble)v_height, 0.0, -1.0, 1.0);

	gglMatrixMode(GL_MODELVIEW);
	gglLoadIdentity();
	//-----------------------------------------------------------------

	//TODO: is clearcolor necessary?
	//set rendering modes
	gglClearColor(1.0f, 1.0f, 1.0f, 1.0f);
//	gglClearDepth(1.0f);
	gglClear(GL_COLOR_BUFFER_BIT/* | GL_DEPTH_BUFFER_BIT*/);

	//-----------------------------------------------------------------
#endif

	if(anim)
		FinalProj_NextTS(tmp_time);
	else
		ci.elapsedtime = 0;

	FinalProj_Render(v_width, v_height);

/*
	tmpsbuf = (byte *)sbufi[!currentbuf];
	gglPointSize(1.0);
	gglBegin(GL_POINTS);
	for(i = 0; i < pixels; i++)
	{
		gglColor3ub((byte)tmpsbuf[i * 4], (byte)tmpsbuf[i * 4 + 1], (byte)tmpsbuf[i * 4 + 2]);
		gglVertex3f((float)(i % width), (float)(i / width), -1.0f);
	}

	gglEnd();
*/
#if 0
	//for FSAA
	if(usefxaa)
		gglBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	//
	// FSAA
	//
	if(usefxaa)//antialiasing > 0)
	{
		gglDisable(GL_DEPTH_TEST);
		gglDisable(GL_CULL_FACE);

		gglViewport(0, 0, (GLsizei)width, (GLsizei)height);
		gglMatrixMode(GL_PROJECTION);

		gglLoadIdentity();
		gglOrtho(0.0, (GLdouble)v_width, (GLdouble)v_height, 0.0, -1.0, 1.0);

		gglMatrixMode(GL_MODELVIEW);
		gglLoadIdentity();

		gglClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		gglClear(GL_COLOR_BUFFER_BIT);

		gglUseProgramObjectARB(glslprograms[GP_FXAA]);
		gglActiveTexture(GL_TEXTURE0);
		gglBindTexture(GL_TEXTURE_2D, fbtexs[FB_RENDER_INT]);
//		gglEnable(GL_TEXTURE_2D);
		gglUniform1iARB(uniloc[0], 0);

		gglBegin(GL_QUADS);
//		gglTexCoord2f(0.0f, 0.0f);
		gglMultiTexCoord2f(0, 0.0f, 0.0f);
		gglVertex2f(0.0f, v_height);
//		gglTexCoord2f(1.0f, 0.0f);
		gglMultiTexCoord2f(0, 1.0f, 0.0f);
		gglVertex2f(v_width, v_height);
//		gglTexCoord2f(1.0f, 1.0f);
		gglMultiTexCoord2f(0, 1.0f, 1.0f);
		gglVertex2f(v_width, 0.0f);
//		gglTexCoord2f(0.0f, 1.0f);
		gglMultiTexCoord2f(0, 0.0f, 1.0f);
		gglVertex2f(0.0f, 0.0f);
		gglEnd();
	}

	//-----------------------------------------------------------------
	//setup to draw the text to the screen in 2D mode

	gglUseProgramObjectARB(glslprograms[GP_TEXT]);
#endif
	gglColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	gglRasterPos3f(BINDINGS_LEFT_ALIGN, 15.0f, 0.0f);
	Print("Key Bindings:");
	gglRasterPos3f(BINDINGS_LEFT_ALIGN, 15.0f + FONT_Y_INC, 0.0f);
	Print("s - Change scheme");
	gglRasterPos3f(BINDINGS_LEFT_ALIGN, 15.0f + 2.0f * FONT_Y_INC, 0.0f);
	Print("m - Toggle smoothing");
	gglRasterPos3f(BINDINGS_LEFT_ALIGN, 15.0f + 3.0f * FONT_Y_INC, 0.0f);
	Print("c - Toggle capture");
	gglRasterPos3f(BINDINGS_LEFT_ALIGN, 15.0f + 4.0f * FONT_Y_INC, 0.0f);
	Print("x - Toggle results animation");
	gglRasterPos3f(BINDINGS_LEFT_ALIGN, 15.0f + 5.0f * FONT_Y_INC, 0.0f);
	Print("p - Screenshot");

	fontpos = 15.0f;
	gglRasterPos3f(0.0f, fontpos, 0.0f);
	//draw the currently visible scheme
	switch(scheme)
	{
		case OUT_SIMPLE_SS_U:
			Print("SIMPLE Steady-State u Solution:  Nx = %d  Ny = %d", ci.numxc, ci.numyc);
			break;

		case OUT_SIMPLE_SS_V:
			Print("SIMPLE Steady-State v Solution:  Nx = %d  Ny = %d", ci.numxc, ci.numyc);
			break;

		case OUT_SIMPLE_SS_P:
			Print("SIMPLE Steady-State pressure Solution:  Nx = %d  Ny = %d", ci.numxc, ci.numyc);
			break;

		case OUT_ANALYTICAL_U:
			Print("Analytical Steady-State u Solution:  Nx = %d  Ny = %d", ci.numxc, ci.numyc);
			break;
	}

	fontpos += FONT_Y_INC;

	gglRasterPos3f(0.0f, fontpos, 0.0f);
	if(smoothing)
		Print("Smooth Shading");
	else
		Print("Solid Shading");

	fontpos += FONT_Y_INC;

	gglRasterPos3f(0.0f, fontpos, 0.0f);
	if(anim)
		Print("Animation Time Scale: %.2f", ci.animscale);

	fontpos += FONT_Y_INC;

	if(benchmark)
	{
		gglRasterPos3f(0.0f, fontpos, 0.0f);
		Print("Benchmarking... %d", counter);
		fontpos += FONT_Y_INC;
	}

	if(capture)
	{
		gglRasterPos3f(0.0f, fontpos, 0.0f);
		Print("Capturing...");
		fontpos += FONT_Y_INC;

		gglFlush();
		gglReadPixels(0, 0, si.width, si.height, GL_RGB, GL_UNSIGNED_BYTE, screen_buffer);

		sprintf(sname, "screen%d.bmp", srcount++);
		screenshot.width = si.width;
		screenshot.height = si.height;
		screenshot.bitdepth = 24;
		screenshot.data = screen_buffer;

		Img_WriteBMP(sname, &screenshot);
	}

	//swap buffers
	Sys_UpdateScreen();

	//-next frame-
	//when a renderer changes, the initialization must be redone
}

/*
----------------
ThreadMain

- reserved		thread number (starting from 0)
----------------
*/
int ThreadMain(void *reserved)
{
	int n;

	n = (int)reserved + 1;
	Sys_Log("--->Initializing thread #%d", n);

//	thread_info[n].num = n;
//	thread_info[n].blocked = false;
	if(!(thread_info[n].mutex = Sys_CreateMutex()))
	{
		Sys_Log("Thread #%d: Failed to create mutex", n);
		return 0;
	}

	thread_info[n].signal = Sys_CreateSignal();

	while(1)
	{
//		thread_info[n].blocked = true;
		Sys_BlockThread(thread_info[n].signal, thread_info[n].mutex);
//		thread_info[n].blocked = false;

		if(end_threads)
			break;

		//render the scene
//		RenderPixels(n);

		thread_info[n].renddone = 1;
	}

	Sys_Log("--->Shutting down thread #%d", n);
	Sys_DestroyMutex(thread_info[n].mutex);
	Sys_DestroySignal(thread_info[n].signal);

	return 1;
}

//======================================================================================

/*
----------------
Shutdown
----------------
*/
void Shutdown(void)
{
	int i;

	FinalProj_Shutdown();

	//signal all the threads to shutdown
	if(si.num_threads > 1)
	{
		end_threads = true;
		for(i = 0; i < (si.num_threads - 1); i++)
			Sys_SignalThread(thread_info[i + 1].signal);
	}

	if(sbuf_front)
	{
		_free_aligned(sbuf_front);
		sbuf_front = NULL;
	}

	if(sbuf_back)
	{
		_free_aligned(sbuf_back);
		sbuf_back = NULL;
	}

	if(screen_buffer)
	{
		free(screen_buffer);
		screen_buffer = NULL;
	}

	for(i = 0; i < NUM_VP; i++)
	{
		if(vertex_shaders[i])
		{
			gglDeleteProgramsARB(1, (GLuint *)&vertex_shaders[i]);
			vertex_shaders[i] = 0;
		}
	}

	for(i = 0; i < NUM_FP; i++)
	{
		if(fragment_shaders[i])
		{
			gglDeleteProgramsARB(1, (GLuint *)&fragment_shaders[i]);
			fragment_shaders[i] = 0;
		}
	}

	//delete all lists
	if(list_font)
	{
		gglDeleteLists(list_font, 96);
		list_font = 0;
	}

	if(si.num_threads > 1)
		Sys_TerminateThreads(si.num_threads - 1);

	if(thread_info)
	{
		_free_aligned(thread_info);
		thread_info = NULL;
	}
}

/*
----------------
Print

- stream:		message to be printed to the screen
----------------
*/
void __cdecl Print(const char *stream, ...)
{
	va_list argptr;
	char string[1024];

	if(list_font)
	{
		va_start(argptr, stream);
		vsnprintf(string, sizeof(string), stream, argptr);
		va_end(argptr);

		gglPushAttrib(GL_LIST_BIT);
		gglListBase(list_font - 32);
		gglCallLists((GLsizei)strlen(string), GL_UNSIGNED_BYTE, string);
		gglPopAttrib();
	}
}

//=======================================================================

double __cdecl Sqroot_Generic(const double value)
{
	return sqrt(value);
}

double __cdecl RSqroot_Generic(const double value)
{
	return 1.0 / sqrt(value);
}

//all angles in degrees
double __cdecl Sine_Generic(const double theta)
{
	return sin(theta * DEGTORAD);
}

double __cdecl Cosine_Generic(const double theta)
{
	return cos(theta * DEGTORAD);
}

double __cdecl Tangent_Generic(const double theta)
{
	return tan(theta * DEGTORAD);
}

void __cdecl VectorMove_Generic(float *dest, const float *src)
{
	dest[0] = src[0];
	dest[1] = src[1];
	dest[2] = src[2];
	dest[3] = src[3];
}

void __cdecl VectorMoveScalar_Generic(float *dest, const float src)
{
	dest[0] = src;
	dest[1] = src;
	dest[2] = src;
	dest[3] = src;
}

void __cdecl VectorAdd_Generic(float *dest, const float *src1, const float *src2)
{
	dest[0] = src1[0] + src2[0];
	dest[1] = src1[1] + src2[1];
	dest[2] = src1[2] + src2[2];
	dest[3] = src1[3] + src2[3];
}

void __cdecl VectorAddScalar_Generic(float *dest, const float *src1, const float src2)
{
	dest[0] = src1[0] + src2;
	dest[1] = src1[1] + src2;
	dest[2] = src1[2] + src2;
	dest[3] = src1[3] + src2;
}

void __cdecl VectorSubtract_Generic(float *dest, const float *src1, const float *src2)
{
	dest[0] = src1[0] - src2[0];
	dest[1] = src1[1] - src2[1];
	dest[2] = src1[2] - src2[2];
	dest[3] = src1[3] - src2[3];
}

void __cdecl VectorSubtractScalar_Generic(float *dest, const float *src1, const float src2)
{
	dest[0] = src1[0] - src2;
	dest[1] = src1[1] - src2;
	dest[2] = src1[2] - src2;
	dest[3] = src1[3] - src2;
}

void __cdecl VectorMultiply_Generic(float *dest, const float *src1, const float *src2)
{
	dest[0] = src1[0] * src2[0];
	dest[1] = src1[1] * src2[1];
	dest[2] = src1[2] * src2[2];
	dest[3] = src1[3] * src2[3];
}

void __cdecl VectorMultiplyScalar_Generic(float *dest, const float *src1, const float src2)
{
	dest[0] = src1[0] * src2;
	dest[1] = src1[1] * src2;
	dest[2] = src1[2] * src2;
	dest[3] = src1[3] * src2;
}

void __cdecl VectorMultiplyAdd_Generic(float *dest, const float *src1, const float *src2, const float *src3)
{
	dest[0] = src1[0] * src2[0] + src3[0];
	dest[1] = src1[1] * src2[1] + src3[1];
	dest[2] = src1[2] * src2[2] + src3[2];
	dest[3] = src1[3] * src2[3] + src3[3];
}

void __cdecl VectorMultiplyAddScalar_Generic(float *dest, const float *src1, const float src2, const float *src3)
{
	dest[0] = src1[0] * src2 + src3[0];
	dest[1] = src1[1] * src2 + src3[1];
	dest[2] = src1[2] * src2 + src3[2];
	dest[3] = src1[3] * src2 + src3[3];
}

float __cdecl VectorDotProduct3_Generic(const float *src1, const float *src2)
{
	return src1[0] * src2[0] + src1[1] * src2[1] + src1[2] * src2[2];
}

float __cdecl VectorDotProduct4_Generic(const float *src1, const float *src2)
{
	return src1[0] * src2[0] + src1[1] * src2[1] + src1[2] * src2[2] + src1[3] * src2[3];
}

float __cdecl VectorDotProductH_Generic(const float *src1, const float *src2)
{
	return src1[0] * src2[0] + src1[1] * src2[1] + src1[2] * src2[2] + src2[3];
}

void __cdecl VectorCrossProduct_Generic(float *dest, const float *src1, const float *src2)
{
	dest[0] = src1[1] * src2[2] - src1[2] * src2[1];
	dest[1] = src1[2] * src2[0] - src1[0] * src2[2];
	dest[2] = src1[0] * src2[1] - src1[1] * src2[0];
}

void __cdecl VectorNormalize_Generic(float *vec)
{
	float invlength;

	invlength = (float)RSqroot_Generic(VectorDotProduct3_Generic(vec, vec));
	vec[0] *= invlength;
	vec[1] *= invlength;
	vec[2] *= invlength;
}

void __cdecl CreatePlane_Generic(float *plane, const float *v1, const float *v2, const float *v3)
{
	vec4 delta1, delta2;

	delta1[0] = v1[0] - v2[0];
	delta1[1] = v1[1] - v2[1];
	delta1[2] = v1[2] - v2[2];
	delta1[3] = v1[3] - v2[3];
	delta2[0] = v3[0] - v2[0];
	delta2[1] = v3[1] - v2[1];
	delta2[2] = v3[2] - v2[2];
	delta2[3] = v3[3] - v2[3];
	plane[0] = delta1[1] * delta2[2] - delta1[2] * delta2[1];
	plane[1] = delta1[2] * delta2[0] - delta1[0] * delta2[2];
	plane[2] = delta1[0] * delta2[1] - delta1[1] * delta2[0];

	VectorNormalize_Generic(plane);

	plane[3] = -VectorDotProduct3_Generic(v1, plane);
}

/*
 *-----------------------------------
 * matricies are defined as:
 * 		0  1  2  3
 * 		4  5  6  7
 * 		8  9  10 11
 * 		12 13 14 15
 *-----------------------------------
*/

void __cdecl VectorMatrixMultiply3_Generic(float *out, const float *vec, const float *mat)
{
	out[0] = mat[0] * vec[0] + mat[1] * vec[1] + mat[2] * vec[2];
	out[1] = mat[4] * vec[0] + mat[5] * vec[1] + mat[6] * vec[2];
	out[2] = mat[8] * vec[0] + mat[9] * vec[1] + mat[10] * vec[2];
}

void __cdecl VectorMatrixMultiply4_Generic(float *out, const float *vec, const float *mat)
{
	out[0] = mat[0] * vec[0] + mat[1] * vec[1] + mat[2] * vec[2] + mat[3] * vec[3];
	out[1] = mat[4] * vec[0] + mat[5] * vec[1] + mat[6] * vec[2] + mat[7] * vec[3];
	out[2] = mat[8] * vec[0] + mat[9] * vec[1] + mat[10] * vec[2] + mat[11] * vec[3];
	out[3] = mat[12] * vec[0] + mat[13] * vec[1] + mat[14] * vec[2] + mat[15] * vec[3];
}

void __cdecl VectorMatrixMultiplyH_Generic(float *out, const float *vec, const float *mat)
{
	out[0] = mat[0] * vec[0] + mat[1] * vec[1] + mat[2] * vec[2] + mat[3];
	out[1] = mat[4] * vec[0] + mat[5] * vec[1] + mat[6] * vec[2] + mat[7];
	out[2] = mat[8] * vec[0] + mat[9] * vec[1] + mat[10] * vec[2] + mat[11];
//	out[3] = mat[12] * vec[0] + mat[13] * vec[1] + mat[14] * vec[2] + mat[15];
}

void __cdecl MatrixCopy3x3_Generic(float *out, const float *mat)
{
	out[0] = mat[0];
	out[1] = mat[1];
	out[2] = mat[2];
//	out[3] = 0.0f;				//padding
	out[4] = mat[4];
	out[5] = mat[5];
	out[6] = mat[6];
//	out[7] = 0.0f;				//padding
	out[8] = mat[8];
	out[9] = mat[9];
	out[10] = mat[10];
//	out[11] = 0.0f;				//padding
}

void __cdecl MatrixCopy4x4_Generic(float *out, const float *mat)
{
	out[0] = mat[0];
	out[1] = mat[1];
	out[2] = mat[2];
	out[3] = mat[3];
	out[4] = mat[4];
	out[5] = mat[5];
	out[6] = mat[6];
	out[7] = mat[7];
	out[8] = mat[8];
	out[9] = mat[9];
	out[10] = mat[10];
	out[11] = mat[11];
	out[12] = mat[12];
	out[13] = mat[13];
	out[14] = mat[14];
	out[15] = mat[15];
}

void __cdecl MatrixTranspose3x3_Generic(float *out, const float *mat)
{
	out[0] = mat[0]; out[4] = mat[1]; out[8] = mat[2];
	out[1] = mat[4]; out[5] = mat[5]; out[9] = mat[6];
	out[2] = mat[8]; out[6] = mat[9]; out[10] = mat[10];
//	out[3] = 0.0f; out[7] = 0.0f; out[11] = 0.0f;			//padding
}

void __cdecl MatrixTranspose4x4_Generic(float *out, const float *mat)
{
	out[0] = mat[0]; out[4] = mat[1]; out[8] = mat[2]; out[12] = mat[3];
	out[1] = mat[4]; out[5] = mat[5]; out[9] = mat[6]; out[13] = mat[7];
	out[2] = mat[8]; out[6] = mat[9]; out[10] = mat[10]; out[14] = mat[11];
	out[3] = mat[12]; out[7] = mat[13]; out[11] = mat[14]; out[15] = mat[15];
}

void __cdecl MatrixMultiply3x3_Generic(float *out, const float *mat1, const float *mat2)
{
	out[0] = mat1[0] * mat2[0] + mat1[1] * mat2[4] + mat1[2] * mat2[8];
	out[1] = mat1[0] * mat2[1] + mat1[1] * mat2[5] + mat1[2] * mat2[9];
	out[2] = mat1[0] * mat2[2] + mat1[1] * mat2[6] + mat1[2] * mat2[10];
//	out[3] = 0.0f;				//padding
	out[4] = mat1[4] * mat2[0] + mat1[5] * mat2[4] + mat1[6] * mat2[8];
	out[5] = mat1[4] * mat2[1] + mat1[5] * mat2[5] + mat1[6] * mat2[9];
	out[6] = mat1[4] * mat2[2] + mat1[5] * mat2[6] + mat1[6] * mat2[10];
//	out[7] = 0.0f;				//padding
	out[8] = mat1[8] * mat2[0] + mat1[9] * mat2[4] + mat1[10] * mat2[8];
	out[9] = mat1[8] * mat2[1] + mat1[9] * mat2[5] + mat1[10] * mat2[9];
	out[10] = mat1[8] * mat2[2] + mat1[9] * mat2[6] + mat1[10] * mat2[10];
//	out[11] = 0.0f;				//padding
}

void __cdecl MatrixMultiply4x4_Generic(float *out, const float *mat1, const float *mat2)
{
	out[0] = mat1[0] * mat2[0] + mat1[1] * mat2[4] + mat1[2] * mat2[8] + mat1[3] * mat2[12];
	out[1] = mat1[0] * mat2[1] + mat1[1] * mat2[5] + mat1[2] * mat2[9] + mat1[3] * mat2[13];
	out[2] = mat1[0] * mat2[2] + mat1[1] * mat2[6] + mat1[2] * mat2[10] + mat1[3] * mat2[14];
	out[3] = mat1[0] * mat2[3] + mat1[1] * mat2[7] + mat1[2] * mat2[11] + mat1[3] * mat2[15];
	out[4] = mat1[4] * mat2[0] + mat1[5] * mat2[4] + mat1[6] * mat2[8] + mat1[7] * mat2[12];
	out[5] = mat1[4] * mat2[1] + mat1[5] * mat2[5] + mat1[6] * mat2[9] + mat1[7] * mat2[13];
	out[6] = mat1[4] * mat2[2] + mat1[5] * mat2[6] + mat1[6] * mat2[10] + mat1[7] * mat2[14];
	out[7] = mat1[4] * mat2[3] + mat1[5] * mat2[7] + mat1[6] * mat2[11] + mat1[7] * mat2[15];
	out[8] = mat1[8] * mat2[0] + mat1[9] * mat2[4] + mat1[10] * mat2[8] + mat1[11] * mat2[12];
	out[9] = mat1[8] * mat2[1] + mat1[9] * mat2[5] + mat1[10] * mat2[9] + mat1[11] * mat2[13];
	out[10] = mat1[8] * mat2[2] + mat1[9] * mat2[6] + mat1[10] * mat2[10] + mat1[11] * mat2[14];
	out[11] = mat1[8] * mat2[3] + mat1[9] * mat2[7] + mat1[10] * mat2[11] + mat1[11] * mat2[15];
	out[12] = mat1[12] * mat2[0] + mat1[13] * mat2[4] + mat1[14] * mat2[8] + mat1[15] * mat2[12];
	out[13] = mat1[12] * mat2[1] + mat1[13] * mat2[5] + mat1[14] * mat2[9] + mat1[15] * mat2[13];
	out[14] = mat1[12] * mat2[2] + mat1[13] * mat2[6] + mat1[14] * mat2[10] + mat1[15] * mat2[14];
	out[15] = mat1[12] * mat2[3] + mat1[13] * mat2[7] + mat1[14] * mat2[11] + mat1[15] * mat2[15];
}

void __cdecl MatrixLoadIdentity_Generic(void)
{
	gmat[0] = gmat[5] = gmat[10] = gmat[15] = 1.0f;
	gmat[1] = gmat[2] = gmat[3] = gmat[4] = 0.0f;
	gmat[6] = gmat[7] = gmat[8] = gmat[9] = 0.0f;
	gmat[11] = gmat[12] = gmat[13] = gmat[14] = 0.0f;
}

void __cdecl MatrixTranslate_Generic(const float *vpos)
{
	gmat[3] += vpos[0];
	gmat[7] += vpos[1];
	gmat[11] += vpos[2];
}

void __cdecl MatrixScale_Generic(const float *sfac)
{
	gmat[0] *= sfac[0];
	gmat[5] *= sfac[1];
	gmat[10] *= sfac[2];
}

//NOTE: 3x3 matricies should be padded to 16 byte rows
_align16(float rmat1[12]);
_align16(float rmat2[12]);
_align16(float tmpmat[12]);
_align16(float tm41[16]);
_align16(float tm42[16]);

//angles in degrees
void __cdecl MatrixRotate_Generic(const float *angles)
{
	float c, s, d, t, e, u;

	c = (float)Cosine_Generic(angles[0]);
	s = (float)Sine_Generic(angles[0]);
	d = (float)Cosine_Generic(angles[1]);
	t = (float)Sine_Generic(angles[1]);
	e = (float)Cosine_Generic(angles[2]);
	u = (float)Sine_Generic(angles[2]);

	tm41[0] = d * e;
	tm41[1] = -(d * u);
	tm41[2] = t;
	tm41[3] = 0.0f;
	tm41[4] = e * s * t + c * u;
	tm41[5] = c * e - (s * t * u);
	tm41[6] = -(d * s);
	tm41[7] = 0.0f;
	tm41[8] = s * u - (c * e * t);
	tm41[9] = c * t * u + e * s;
	tm41[10] = c * d;
	tm41[11] = 0.0f;
	tm41[12] = 0.0f;
	tm41[13] = 0.0f;
	tm41[14] = 0.0f;
	tm41[15] = 1.0f;

	MatrixMultiply4x4_Generic(tm42, tm41, gmat);
	MatrixCopy4x4_Generic(gmat, tm42);
}

void RenderPixels(const int thread_num)
{
	int i, k;
	int tn;
	float r, g, b, a;

	r = g = b = a = 1.0f;
	tn = thread_num;

	//reset the counter
	thread_info[thread_num].done = false;
	thread_info[thread_num].counter = 0;
	thread_info[thread_num].tmpbs = thread_info[thread_num].block_size;

	while(1)
	{
		if(thread_info[tn].counter >= thread_info[tn].tmpbs)
		{
			thread_info[tn].done = true;

			//work on other threads' blocks that aren't complete
			for(k = 0; k < si.num_threads; k++)
			{
//				if((thread_info[k].counter < (thread_info[k].tmpbs - 1))/* && !thread_info[k].extra*/)
				if(!(thread_info[k].done))
				{
//					thread_info[k].extra = 1;
					tn = k;
					break;
				}
			}

			if(k == si.num_threads)
				break;		//all blocks complete
		}

		if(tn == thread_num)
		{
			i = thread_info[thread_num].counter + thread_info[thread_num].pixel_start;
			thread_info[thread_num].counter++;
		}
		else
		{
			i = (thread_info[tn].tmpbs - 1) + thread_info[tn].pixel_start;
			thread_info[tn].tmpbs--;
		}

		sbuf[i * 4] = (byte)(r * 255.0f);
		sbuf[i * 4 + 1] = (byte)(g * 255.0f);
		sbuf[i * 4 + 2] = (byte)(b * 255.0f);
		sbuf[i * 4 + 3] = (byte)(a * 255.0f);
	}
}

int scount = 0;
bool slowvel = false;

#define TIME_DELAY 200
#define TIMESCALE 0.008f

/*
----------------
HandleInput
----------------
*/
void HandleInput(const int ticks)
{
	static int bench_cl = 0, screen_cl = 0, scheme_cl = 0, smooth_cl = 0, animated_cl = 0, capture_cl = 0;
	int time;
	
	In_UpdateKeyboard();

	if(si.keyevents[GKEY_S])
	{
		time = Sys_GetTime();

		if((time - scheme_cl) > TIME_DELAY)
		{
			scheme++;
			if(scheme == OUT_MAX)
				scheme = 0;

			scheme_cl = time;
		}
	}

	if(si.keyevents[GKEY_M])
	{
		time = Sys_GetTime();

		if((time - smooth_cl) > TIME_DELAY)
		{
			smoothing = !smoothing;
			smooth_cl = time;
		}
	}

	if(si.keyevents[GKEY_C])
	{
		time = Sys_GetTime();

		if((time - capture_cl) > TIME_DELAY)
		{
			capture = !capture;
			capture_cl = time;
		}
	}

	if(si.keyevents[GKEY_X])
	{
		time = Sys_GetTime();

		if((time - animated_cl) > TIME_DELAY)
		{
			anim = !anim;
			animated_cl = time;
		}
	}

	//screenshot
	if(si.keyevents[GKEY_P])
	{
		time = Sys_GetTime();

		if((time - screen_cl) > TIME_DELAY)
		{
			gglFlush();
			gglReadPixels(0, 0, si.width, si.height, GL_RGB, GL_UNSIGNED_BYTE, screen_buffer);

			sprintf(sname, "screen%d.bmp", scount++);
			screenshot.width = si.width;
			screenshot.height = si.height;
			screenshot.bitdepth = 24;
			screenshot.data = screen_buffer;

			Img_WriteBMP(sname, &screenshot);

			screen_cl = time;
		}
	}

	if(si.keyevents[GKEY_EQUALS] || si.keyevents[GKEYPAD_ADD])
	{
		ci.animscale += (float)ticks * TIMESCALE;
		if(ci.animscale > 100000.0f)
			ci.animscale = 100000.0f;
	}
	
	if(si.keyevents[GKEY_MINUS] || si.keyevents[GKEYPAD_SUBTRACT])
	{
		ci.animscale -= (float)ticks * TIMESCALE;
		if(ci.animscale < 0.0f)
			ci.animscale = 0.0f;
	}
}
