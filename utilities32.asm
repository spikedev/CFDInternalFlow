;32-bit FPU optimized math routines

	bits 32
	global _StartTimer
	global _StopTimer
	global _ThreadSpinWait
	global _GetCPUInfo_Util
	global _GetCPUInfo
	global _Sqroot_ASM
	global _RSqroot_ASM
	global _Sine_ASM
	global _Cosine_ASM
	global _Tangent_ASM
	global _VectorMove_ASM
	global _VectorMoveScalar_ASM
	global _VectorAdd_ASM
	global _VectorAddScalar_ASM
	global _VectorSubtract_ASM
	global _VectorSubtractScalar_ASM
	global _VectorMultiply_ASM
	global _VectorMultiplyScalar_ASM
	global _VectorMultiplyAdd_ASM
	global _VectorMultiplyAddScalar_ASM
	global _VectorDotProduct3_ASM
	global _VectorDotProduct4_ASM
	global _VectorDotProductH_ASM
	global _VectorCrossProduct_ASM
	global _VectorNormalize_ASM
	global _CreatePlane_ASM
	global _VectorMatrixMultiply3_ASM
	global _VectorMatrixMultiply4_ASM
	global _VectorMatrixMultiplyH_ASM
	global _MatrixCopy3x3_ASM
	global _MatrixCopy4x4_ASM
	global _MatrixTranspose3x3_ASM
	global _MatrixTranspose4x4_ASM
	global _MatrixMultiply3x3_ASM
	global _MatrixMultiply4x4_ASM
	global _MatrixLoadIdentity_ASM
	global _MatrixTranslate_ASM
	global _MatrixScale_ASM
	global _MatrixRotate_ASM

	global _cpu_vendor_id
	global _cpu_model
	global _cpu_family
	global _cpu_ext_family
	global _extcpu_model
	global _extcpu_family
	global _extcpu_ext_family
	global _cpu_count
	global _cpu_hyperthreading
	global _cpu_64_bit
	global _cpu_mmx
	global _cpu_sse
	global _cpu_sse2
	global _cpu_sse3
	global _cpu_3dnow
	global _cpu_3dnow2
	global _cpu_rdtsc
	global _cpu_fpu
	global _cpu_fcmov
	global _cpu_extmmx
	global _cpu_ext_vendorid
	global _cpu_extended_info
	global _cpu_ht_thread
	global _cpu_ssse3
	global _cpu_sse4_1
	global _cpu_sse4_2
	global _cpu_sse4a
	global _cpu_sse5a

	extern _gmat
	extern _thread_info
	extern _end_threads

	section .text

%define THREAD_POT				6						;power of 2 for indexing thread_info

;
;----------------
;StartTimer
;----------------
;
_StartTimer:
	rdtsc
	mov			dword [clock_tics], eax
	cld
	times		8 nop
	ret

;
;----------------
;StopTimer
;----------------
;
_StopTimer:
	clc
	rdtsc
	sub			eax, dword [clock_tics]
	sub			eax, 15
	ret

;
;----------------
;ThreadSpinWait
;
;return 1 if end_threads was triggered
;----------------
;
_ThreadSpinWait:
	mov			edx, dword [esp + 4]					;int thread_num
	mov			ecx, dword [_thread_info]
	shl			edx, THREAD_POT
TSWLoop:
	cmp			dword [_end_threads], 0
	jne			TSWEndLoop1
	cmp			dword [ecx + edx + 40], 0
	je			TSWEndLoop0
	jmp			TSWLoop
TSWEndLoop1:
	mov			eax, 1
	ret													;int
TSWEndLoop0:
	xor			eax, eax
	ret													;int

;
;----------------
;GetCPUInfo_Util
;----------------
;
_GetCPUInfo_Util:
	mov			edx, dword [esp + 4]					;int *regs4
	push		ebx
	mov			eax, dword [edx]
	mov			ebx, dword [edx + 4]
	mov			ecx, dword [edx + 8]
	mov			edx, dword [edx + 12]
	cpuid
	mov			dword [edx], eax
	mov			dword [edx + 4], ebx
	mov			dword [edx + 8], ecx
	mov			dword [edx + 12], edx
	pop			ebx
	ret

;
;----------------
;GetCPUInfo
;----------------
;
_GetCPUInfo:
	push		edi
	mov			edi, _cpu_vendor_id
	pushad
	pushfd
	pop			eax
	mov			ebx, eax
	xor			eax, 0x200000
	push		eax
	popfd
	xor			eax, ebx
	jz			NoCPUID
	xor			eax, eax
	cpuid
	test		eax, 0xffffffff
	jz			NoCPUID
	mov			dword [edi], ebx
	mov			dword [edi + 4], edx
	mov			dword [edi + 8], ecx
	mov			byte [edi + 12], 0
	mov			eax, 1
	cpuid
	mov			esi, eax
	shr			eax, 4
	and			eax, 0xf
	mov			dword [_cpu_model], eax
	mov			eax, esi
	shr			eax, 8
	and			eax, 0xf
	mov			dword [_cpu_family], eax
	mov			eax, esi
	shr			eax, 20
	and			eax, 0xff
	mov			dword [_cpu_ext_family], eax
	shr			ebx, 16
	and			ebx, 0xff
	mov			dword [_cpu_count], ebx
	test		edx, 0x10000000
	jz			NoHyperThreading
	mov			dword [_cpu_hyperthreading], 1
NoHyperThreading:
	test		ecx, 0x8
	jz			NoHTThreading
	mov			dword [_cpu_ht_thread], 1
NoHTThreading:
	test		edx, 0x40000000
	jz			No64Bit
	mov			dword [_cpu_64_bit], 1
No64Bit:
	test		edx, 0x800000
	jz			NoMMX
	mov			dword [_cpu_mmx], 1
NoMMX:
	test		edx, 0x2000000
	jz			NoSSE
	mov			dword [_cpu_sse], 1
NoSSE:
	test		edx, 0x4000000
	jz			NoSSE2
	mov			dword [_cpu_sse2], 1
NoSSE2:
	test		ecx, 1
	jz			NoSSE3
	mov			dword [_cpu_sse3], 1
NoSSE3:
	test		ecx, 0x200
	jz			NoSSSE3
	mov			dword [_cpu_ssse3], 1
NoSSSE3:
	test		ecx, 0x80000
	jz			NoSSE4_1
	mov			dword [_cpu_sse4_1], 1
NoSSE4_1:
	test		ecx, 0x100000
	jz			NoSSE4_2
	mov			dword [_cpu_sse4_2], 1
NoSSE4_2:
	test		edx, 0x10
	jz			NoRDTSC
	mov			dword [_cpu_rdtsc], 1
NoRDTSC:
	test		edx, 1
	jz			NoFPU
	mov			dword [_cpu_fpu], 1
	test		edx, 0x8000
	jz			NoFPU
	mov			dword [_cpu_fcmov], 1
NoFPU:
	mov			eax, 0x80000000
	cpuid
	mov			esi, eax
	test		eax, 0xffffffff
	jz			NoExtendedCPUID
	mov			dword [_cpu_extended_info], 1
	mov			dword [extcpu_max_level], esi
	mov			eax, 0x80000001
	cpuid
	mov			esi, eax
	shr			eax, 4
	and			eax, 0xf
	mov			dword [_extcpu_model], eax
	mov			eax, esi
	shr			eax, 8
	and			eax, 0xf
	mov			dword [_extcpu_family], eax
	mov			eax, esi
	shr			eax, 20
	and			eax, 0xff
	mov			dword [_extcpu_ext_family], eax
	test		edx, 0x80000000
	jz			No3DNow
	mov			dword [_cpu_3dnow], 1
No3DNow:
	test		edx, 0x40000000
	jz			No3DNow2
	mov			dword [_cpu_3dnow2], 1
No3DNow2:
	test		edx, 0x20000000
	jz			No64BitExt
	mov			dword [_cpu_64_bit], 1
No64BitExt:
	test		edx, 0x800000
	jz			NoMMXExt
	mov			dword [_cpu_mmx], 1
NoMMXExt:
	test		ecx, 0x40
	jz			NoSSE4A
	mov			dword [_cpu_sse4a], 1
NoSSE4A:
	test		ecx, 0x800
	jz			NoSSE5A
	mov			dword [_cpu_sse5a], 1
NoSSE5A:
	test		edx, 0x10000
	jz			NoFCMOV
	mov			dword [_cpu_fcmov], 1
NoFCMOV:
	test		edx, 0x400000
	jz			NoExtMMX
	mov			dword [_cpu_extmmx], 1
NoExtMMX:
	test		edx, 1
	jz			NoFPUExt
	mov			dword [_cpu_fpu], 1
NoFPUExt:
	cmp			dword [extcpu_max_level], 0x80000004
	jl			NoExtVendorID
	mov			eax, 0x80000002
	cpuid
	mov			esi, _cpu_ext_vendorid
	mov			dword [esi], eax
	mov			dword [esi + 4], ebx
	mov			dword [esi + 8], ecx
	mov			dword [esi + 12], edx
	mov			eax, 0x80000003
	cpuid
	mov			dword [esi + 16], eax
	mov			dword [esi + 20], ebx
	mov			dword [esi + 24], ecx
	mov			dword [esi + 28], edx
	mov			eax, 0x80000004
	cpuid
	mov			dword [esi + 32], eax
	mov			dword [esi + 36], ebx
	mov			dword [esi + 40], ecx
	mov			dword [esi + 44], edx
	mov			byte [esi + 48], 0
NoExtVendorID:
	mov			ebx, dword [_cpu_count]
	test		ebx, ebx
	jnz			LogicalCPUCountFound
	mov			eax, 0x80000008
	cpuid
	and			ecx, 0xff
	inc			ecx
	mov			dword [_cpu_count], ecx
;	mov			ebx, dword [_cpu_hyperthreading]
;	test		ebx, ebx
;	jz			LogicalCPUCountFound
;	shr			dword [_cpu_count], 1
LogicalCPUCountFound:
NoExtendedCPUID:
	popad
	mov			eax, 1
	pop			edi
	ret
NoCPUID:
	popad
	xor			eax, eax
	pop			edi
	ret

;
;----------------
;Sqroot_ASM
;----------------
;
_Sqroot_ASM:
	sqrtsd		xmm0, [esp + 4]							;double value
	movsd		[esp - 8], xmm0
	fld			qword [esp - 8]
	ret													;double

;
;----------------
;RSqroot_ASM
;----------------
;
_RSqroot_ASM:
	sqrtsd		xmm0, [esp + 4]							;double value
	movsd		xmm1, [constant_one_pd]
	divsd		xmm1, xmm0
	movsd		[esp - 8], xmm1
	fld			qword [esp - 8]
	ret													;double

;
;----------------
;Sine_ASM
;----------------
;
;TODO: fix the rounding mode here
_Sine_ASM:
	fld			qword [esp + 4]							;double theta
	fld			dword [constant_inv_360_ps]
	fmul		st0, st1
	fsub		dword [constant_half_ps]
	fistp		dword [esp - 4]							;x = floor(a / 360) * 360
	fild		dword [esp - 4]
	fmul		dword [constant_360_ps]
	fsubp		st1
	fld			dword [constant_180_ps]
	fsubrp		st1

	fld			st0
	fabs
	fcomp		dword [constant_90_ps]
	fnstsw		ax
	test		ah, 5									;if a < 90, jump to ContinueSinFull
	jnp			ContinueSinFull
	fcom		dword [constant_zero_ps]
	fnstsw		ax
	test		ah, 5									;if a < 0
	jnp			SinNeg180
	fld			dword [constant_180_ps]
	fsubrp		st1
	jmp			ContinueSinFull
SinNeg180:
	fld			dword [constant_neg_180_ps]
	fsubrp		st1
ContinueSinFull:
	fld			dword [aconvert_ps]
	fmulp		st1
	fld			st0
	fmul		st1
	fld			dword [sine_constant4_ps]
	fmul		st1
	fadd		dword [sine_constant5_ps]
	fmulp		st1
	fadd		dword [constant_one_ps]
	fmulp		st1
;	fld			qword [esp + 4]
;	fld			dword [aconvert_ps]
;	fmulp		st1
;	fsin
	ret													;double

;
;----------------
;Cosine_ASM
;----------------
;
;TODO: fix the rounding mode here
_Cosine_ASM:
	fld			qword [esp + 4]							;double theta
	fadd		dword [constant_90_ps]
	fld			dword [constant_inv_360_ps]
	fmul		st0, st1
	fsub		dword [constant_half_ps]
	fistp		dword [esp - 4]							;x = floor(a / 360) * 360
	fild		dword [esp - 4]
	fmul		dword [constant_360_ps]
	fsubp		st1
	fld			dword [constant_180_ps]
	fsubrp		st1

	fld			st0
	fabs
	fcomp		dword [constant_90_ps]
	fnstsw		ax
	test		ah, 5									;if a < 90, jump to ContinueCosFull
	jnp			ContinueCosFull
	fcom		dword [constant_zero_ps]
	fnstsw		ax
	test		ah, 5									;if a < 0
	jnp			CosNeg180
	fld			dword [constant_180_ps]
	fsubrp		st1
	jmp			ContinueCosFull
CosNeg180:
	fld			dword [constant_neg_180_ps]
	fsubrp		st1
ContinueCosFull:
	fld			dword [aconvert_ps]
	fmulp		st1
	fld			st0
	fmul		st1
	fld			dword [sine_constant4_ps]
	fmul		st1
	fadd		dword [sine_constant5_ps]
	fmulp		st1
	fadd		dword [constant_one_ps]
	fmulp		st1
;	fld			qword [esp + 4]
;	fld			dword [aconvert_ps]
;	fmulp		st1
;	fcos
	ret													;double

;
;----------------
;Tangent_ASM
;----------------
;
;TODO: fix the rounding mode here
_Tangent_ASM:
	fld			qword [esp + 4]							;double theta
	fld			dword [constant_inv_180_ps]
	fmul		st0, st1
	fsub		dword [constant_half_ps]
	fistp		dword [esp - 4]							;x = floor(a / 360) * 360
	fild		dword [esp - 4]
	fmul		dword [constant_180_ps]
	fsubp		st1
	fcom		dword [constant_90_ps]
	fnstsw		ax
	test		ah, 5									;if a < 90, jump to ContinueTanFull
	jnp			ContinueTanFull
	fsub		dword [constant_180_ps]
ContinueTanFull:
	fld			dword [aconvert_ps]
	fmulp		st1
	fld			st0										;interleave sine and cosine computations
	fmul		st1
	fld			dword [cosine_constant1_ps]
	fmul		st1
	fxch		st1
	fld			dword [sine_constant3_ps]
	fmul		st1
	fxch		st2
	fadd		dword [cosine_constant2_ps]
	fxch		st2
	fadd		dword [sine_constant4_ps]
	fxch		st2
	fmul		st1
	fxch		st2
	fmul		st1
	fxch		st2
	fadd		dword [cosine_constant3_ps]
	fxch		st2
	fadd		dword [sine_constant5_ps]
	fxch		st2
	fmul		st1
	fxch		st2
	fmul		st1
	fxch		st2
	fadd		dword [cosine_constant4_ps]
	fxch		st2
	fadd		dword [constant_one_ps]
	fxch		st2
	fmul		st1
	fxch		st2
	fmulp		st3
	fxch		st1
	fadd		dword [cosine_constant5_ps]
	fmulp		st1
	fadd		dword [constant_one_ps]
	fdivp		st1										;sine / cosine
;	fld			qword [esp + 4]
;	fld			dword [aconvert_ps]
;	fmulp		st1
;	fptan
;	fstp		st0										;pop the stack
	ret													;double

;
;----------------
;VectorMove_ASM
;----------------
;
_VectorMove_ASM:
	mov			eax, dword [esp + 8]					;float *src
	mov			ecx, dword [esp + 4]					;float *dest
	movaps		xmm0, [eax]
	movaps		[ecx], xmm0
	ret

;
;----------------
;VectorMoveScalar_ASM
;----------------
;
_VectorMoveScalar_ASM:
	mov			ecx, dword [esp + 4]					;float *dest
	movss		xmm0, [esp + 8]							;float src
	shufps		xmm0, xmm0, 0
	movaps		[ecx], xmm0
	ret

;
;----------------
;VectorAdd_ASM
;----------------
;
_VectorAdd_ASM:
	mov			eax, dword [esp + 8]					;float *src1
	mov			ecx, dword [esp + 12]					;float *src2
	mov			edx, dword [esp + 4]					;float *dest
	movaps		xmm0, [eax]
	addps		xmm0, [ecx]
	movaps		[edx], xmm0
	ret

;
;----------------
;VectorAddScalar_ASM
;----------------
;
_VectorAddScalar_ASM:
	mov			eax, dword [esp + 8]					;float *src1
	mov			ecx, dword [esp + 4]					;float *dest
	movss		xmm0, [esp + 12]						;float src2
	shufps		xmm0, xmm0, 0
	addps		xmm0, [eax]
	movaps		[ecx], xmm0
	ret

;
;----------------
;VectorSubtract_ASM
;----------------
;
_VectorSubtract_ASM:
	mov			eax, dword [esp + 8]					;float *src1
	mov			ecx, dword [esp + 12]					;float *src2
	mov			edx, dword [esp + 4]					;float *dest
	movaps		xmm0, [eax]
	subps		xmm0, [ecx]
	movaps		[edx], xmm0
	ret

;
;----------------
;VectorSubtractScalar_ASM
;----------------
;
_VectorSubtractScalar_ASM:
	mov			eax, dword [esp + 8]					;float *src1
	mov			ecx, dword [esp + 4]					;float *dest
	movss		xmm0, [esp + 12]
	movaps		xmm1, [eax]
	shufps		xmm0, xmm0, 0
	subps		xmm1, xmm0
	movaps		[ecx], xmm1
	ret

;
;----------------
;VectorMultiply_ASM
;----------------
;
_VectorMultiply_ASM:
	mov			eax, dword [esp + 8]					;float *src1
	mov			ecx, dword [esp + 12]					;float *src2
	mov			edx, dword [esp + 4]					;float *dest
	movaps		xmm0, [eax]
	mulps		xmm0, [ecx]
	movaps		[edx], xmm0
	ret

;
;----------------
;VectorMultiplyScalar_ASM
;----------------
;
_VectorMultiplyScalar_ASM:
	mov			eax, dword [esp + 8]					;float *src1
	mov			ecx, dword [esp + 4]					;float *dest
	movss		xmm0, [esp + 12]						;float src2
	shufps		xmm0, xmm0, 0
	mulps		xmm0, [eax]
	movaps		[ecx], xmm0
	ret

;
;----------------
;VectorMultiplyAdd_ASM
;----------------
;
_VectorMultiplyAdd_ASM:
	mov			eax, dword [esp + 8]					;float *src1
	mov			ecx, dword [esp + 12]					;float *src2
	mov			edx, dword [esp + 16]					;float *src3
	movaps		xmm0, [eax]
	mov			eax, dword [esp + 4]					;float *dest
	mulps		xmm0, [ecx]
	addps		xmm0, [edx]
	movaps		[eax], xmm0
	ret

;
;----------------
;VectorMultiplyAddScalar_ASM
;----------------
;
_VectorMultiplyAddScalar_ASM:
	mov			eax, dword [esp + 8]					;float *src1
	mov			edx, dword [esp + 16]					;float *src3
	mov			ecx, dword [esp + 4]					;float *dest
	movss		xmm0, [esp + 12]						;float src2
	shufps		xmm0, xmm0, 0
	mulps		xmm0, [eax]
	addps		xmm0, [edx]
	movaps		[ecx], xmm0
	ret

;
;----------------
;VectorDotProduct3_ASM
;----------------
;
_VectorDotProduct3_ASM:
	mov			eax, dword [esp + 4]					;float *src1
	mov			ecx, dword [esp + 8]					;float *src2
	fld			dword [ecx]
	fmul		dword [eax]
	fld			dword [ecx + 4]
	fmul		dword [eax + 4]
	fld			dword [ecx + 8]
	fmul		dword [eax + 8]
	fxch		st1
	faddp		st2, st0
	faddp		st1, st0
	ret													;float

;
;----------------
;VectorDotProduct4_ASM
;----------------
;
_VectorDotProduct4_ASM:
	mov			eax, dword [esp + 4]					;float *src1
	mov			ecx, dword [esp + 8]					;float *src2
	movaps		xmm0, [eax]
;	movaps		xmm1, [ecx]
	mulps		xmm0, [ecx]
	movhlps		xmm1, xmm0
	addps		xmm0, xmm1
	movaps		xmm2, xmm0
	shufps		xmm0, xmm0, 0x55
	addss		xmm0, xmm2
	movss		[esp - 8], xmm0
	fld			dword [esp -8]
	ret													;float

;
;----------------
;VectorDotProductH_ASM
;----------------
;
_VectorDotProductH_ASM:
	mov			eax, dword [esp + 4]					;float *src1
	mov			ecx, dword [esp + 8]					;float *src2
	fld			dword [ecx]
	fmul		dword [eax]
	fld			dword [ecx + 4]
	fmul		dword [eax + 4]
	fld			dword [ecx + 8]
	fmul		dword [eax + 8]
	fld			dword [ecx + 12]
	faddp		st3, st0
	faddp		st2, st0
	faddp		st1, st0
	ret													;float

;
;----------------
;VectorCrossProduct_ASM
;----------------
;
_VectorCrossProduct_ASM:
	mov			edx, dword [esp + 8]					;float *src1
	mov			ecx, dword [esp + 12]					;float *src2
	mov			eax, dword [esp + 4]					;float *dest
	movaps		xmm0, [edx]
	movaps		xmm1, [ecx]
	movaps		xmm2, xmm0
	movaps		xmm3, xmm1
	shufps		xmm0, xmm0, 0xc9
	shufps		xmm1, xmm1, 0xd2
	shufps		xmm2, xmm2, 0xd2
	shufps		xmm3, xmm3, 0xc9
	mulps		xmm0, xmm1
	mulps		xmm2, xmm3
	subps		xmm0, xmm2
	movaps		[eax], xmm0
	ret

;
;----------------
;VectorNormalize_ASM
;----------------
;
_VectorNormalize_ASM:
	mov			eax, dword [esp + 4]					;float *vec
	movaps		xmm0, [eax]
	movaps		xmm3, xmm0
	mulps		xmm0, xmm0
	movhlps		xmm1, xmm0
	movaps		xmm2, xmm0
	shufps		xmm0, xmm0, 0x55
	addss		xmm1, xmm2
	addss		xmm0, xmm1
	rsqrtss		xmm0, xmm0
	shufps		xmm0, xmm0, 0x0
;	movaps		xmm1, [eax]
	mulps		xmm0, xmm3
	movaps		[eax], xmm0
	ret

;
;----------------
;CreatePlane_ASM
;----------------
;
_CreatePlane_ASM:
	mov			eax, dword [esp + 8]					;float *v1
	mov			ecx, dword [esp + 12]					;float *v2
	mov			edx, dword [esp + 16]					;float *v3
	movaps		xmm0, [eax]
	movaps		xmm1, [edx]
	subps		xmm0, [ecx]								;v1 - v2
	subps		xmm1, [ecx]								;v3 - v2
	movaps		xmm2, xmm0								;cross product
	movaps		xmm3, xmm1
	shufps		xmm0, xmm0, 0xc9
	shufps		xmm1, xmm1, 0xd2
	shufps		xmm2, xmm2, 0xd2
	shufps		xmm3, xmm3, 0xc9
	mulps		xmm0, xmm1
	mulps		xmm2, xmm3
	subps		xmm0, xmm2
	movaps		xmm3, xmm0								;normalize
	mulps		xmm0, xmm0
	movhlps		xmm1, xmm0
	movaps		xmm2, xmm0
	shufps		xmm0, xmm0, 0x55
	addss		xmm1, xmm2
	addss		xmm0, xmm1
	rsqrtss		xmm0, xmm0
	shufps		xmm0, xmm0, 0x0
	movaps		xmm1, xmm3
	mulps		xmm0, xmm1
	mov			ecx, dword [esp + 4]					;float *plane
	movaps		[ecx], xmm0
	movaps		xmm1, [eax]
	mulps		xmm0, xmm1								;dot product
	movhlps		xmm1, xmm0
	movaps		xmm2, xmm0
	shufps		xmm0, xmm0, 0x55
	addss		xmm1, xmm2
	addss		xmm0, xmm1
	xorps		xmm0, [constant_signmsk_ps]
	movss		[ecx + 12], xmm0
	ret

;
;----------------
;VectorMatrixMultiply3_ASM
;----------------
;
_VectorMatrixMultiply3_ASM:
;	mov			ecx, dword [esp + 12]					;float *mat
;	mov			edx, dword [esp + 8]					;float *vec
;	mov			eax, dword [esp + 4]					;float *out
;	movaps		xmm3, [ecx + 32]
;	movlps		xmm0, [ecx]
;	movlps		xmm2, [ecx + 8]
;	movhps		xmm0, [ecx + 16]
;	movhps		xmm2, [ecx + 24]
;	movaps		xmm1, xmm0
;	shufps		xmm0, xmm3, 0xc8
;	shufps		xmm1, xmm3, 0xdd
;	shufps		xmm2, xmm3, 0xe8
;	movss		xmm4, [edx]
;	movss		xmm5, [edx + 4]
;	movss		xmm6, [edx + 8]
;	shufps		xmm4, xmm4, 0x0
;	shufps		xmm5, xmm5, 0x0
;	shufps		xmm6, xmm6, 0x0
;	mulps		xmm0, xmm4
;	mulps		xmm1, xmm5
;	mulps		xmm2, xmm6
;	addps		xmm0, xmm1
;	addps		xmm0, xmm2
;	movaps		[eax], xmm0
	mov			edx, dword [esp + 8]					;float *vec
	mov			ecx, dword [esp + 12]					;float *mat
	mov			eax, dword [esp + 4]					;float *out
	movaps		xmm0, [edx]
	movaps		xmm1, [ecx]
	movaps		xmm2, [ecx + 16]
	movaps		xmm3, [ecx + 32]
	mulps		xmm1, xmm0
	mulps		xmm2, xmm0
	mulps		xmm3, xmm0
	movhlps		xmm0, xmm1
	movaps		xmm4, xmm1
	shufps		xmm1, xmm1, 0x55
	addss		xmm0, xmm4
	addss		xmm1, xmm0
	movss		[eax], xmm1
	movhlps		xmm0, xmm2
	movaps		xmm4, xmm2
	shufps		xmm2, xmm2, 0x55
	addss		xmm0, xmm4
	addss		xmm2, xmm0
	movss		[eax + 4], xmm2
	movhlps		xmm0, xmm3
	movaps		xmm4, xmm3
	shufps		xmm3, xmm3, 0x55
	addss		xmm0, xmm4
	addss		xmm3, xmm0
	movss		[eax + 8], xmm3
	ret

;
;----------------
;VectorMatrixMultiply4_ASM
;----------------
;
_VectorMatrixMultiply4_ASM:
	mov			edx, dword [esp + 8]					;float *vec
	mov			ecx, dword [esp + 12]					;float *mat
	mov			eax, dword [esp + 4]					;float *out
	movaps		xmm0, [edx]
	movaps		xmm1, [ecx]
	movaps		xmm2, [ecx + 16]
	movaps		xmm3, [ecx + 32]
	movaps		xmm4, [ecx + 48]
	mulps		xmm1, xmm0
	mulps		xmm2, xmm0
	mulps		xmm3, xmm0
	mulps		xmm4, xmm0
	movhlps		xmm0, xmm1
	addps		xmm1, xmm0
	movaps		xmm5, xmm1
	shufps		xmm1, xmm1, 0x55
	addss		xmm1, xmm5
	movss		[eax], xmm1
	movhlps		xmm0, xmm2
	addps		xmm2, xmm0
	movaps		xmm5, xmm2
	shufps		xmm2, xmm2, 0x55
	addss		xmm2, xmm5
	movss		[eax + 4], xmm2
	movhlps		xmm0, xmm3
	addps		xmm3, xmm0
	movaps		xmm5, xmm3
	shufps		xmm3, xmm3, 0x55
	addss		xmm3, xmm5
	movss		[eax + 8], xmm3
	movhlps		xmm0, xmm4
	addps		xmm4, xmm0
	movaps		xmm5, xmm4
	shufps		xmm4, xmm4, 0x55
	addss		xmm4, xmm5
	movss		[eax + 12], xmm4
	ret

;
;----------------
;VectorMatrixMultiplyH_ASM
;----------------
;
_VectorMatrixMultiplyH_ASM:
	mov			edx, dword [esp + 8]					;float *vec
	mov			ecx, dword [esp + 12]					;float *mat
	mov			eax, dword [esp + 4]					;float *out
	movaps		xmm0, [edx]
	andps		xmm0, [constant_mat_mask_ps]
	orps		xmm0, [constant_vec_mask_ps]
	movaps		xmm1, [ecx]
	movaps		xmm2, [ecx + 16]
	movaps		xmm3, [ecx + 32]
	mulps		xmm1, xmm0
	mulps		xmm2, xmm0
	mulps		xmm3, xmm0
	movhlps		xmm0, xmm1
	addps		xmm1, xmm0
	movaps		xmm4, xmm1
	shufps		xmm1, xmm1, 0x55
	addss		xmm1, xmm4
	movss		[eax], xmm1
	movhlps		xmm0, xmm2
	addps		xmm2, xmm0
	movaps		xmm4, xmm2
	shufps		xmm2, xmm2, 0x55
	addss		xmm2, xmm4
	movss		[eax + 4], xmm2
	movhlps		xmm0, xmm3
	addps		xmm3, xmm0
	movaps		xmm4, xmm3
	shufps		xmm3, xmm3, 0x55
	addss		xmm3, xmm4
	movss		[eax + 8], xmm3
	ret

;
;----------------
;MatrixCopy3x3_ASM
;----------------
;
_MatrixCopy3x3_ASM:
	mov			ecx, dword [esp + 8]					;float *mat
	mov			eax, dword [esp + 4]					;float *dest
	movaps		xmm0, [ecx]
	movaps		xmm1, [ecx + 16]
	movaps		xmm2, [ecx + 32]
	movaps		[eax], xmm0
	movaps		[eax + 16], xmm1
	movaps		[eax + 32], xmm2
	ret

;
;----------------
;MatrixCopy4x4_ASM
;----------------
;
_MatrixCopy4x4_ASM:
	mov			ecx, dword [esp + 8]					;float *mat
	mov			eax, dword [esp + 4]					;float *dest
	movaps		xmm0, [ecx]
	movaps		xmm1, [ecx + 16]
	movaps		xmm2, [ecx + 32]
	movaps		xmm3, [ecx + 48]
	movaps		[eax], xmm0
	movaps		[eax + 16], xmm1
	movaps		[eax + 32], xmm2
	movaps		[eax + 48], xmm3
	ret

;
;----------------
;MatrixTranspose3x3_ASM
;----------------
;
_MatrixTranspose3x3_ASM:
	mov			ecx, dword [esp + 8]					;float *mat
	mov			eax, dword [esp + 4]					;float *dest
	movaps		xmm3, [ecx + 32]
	movlps		xmm0, [ecx]
	movlps		xmm2, [ecx + 8]
	movhps		xmm0, [ecx + 16]
	movhps		xmm2, [ecx + 24]
	movaps		xmm1, xmm0
	shufps		xmm0, xmm3, 0xc8
	shufps		xmm1, xmm3, 0xdd
	shufps		xmm2, xmm3, 0xe8
	movaps		[eax], xmm0
	movaps		[eax + 16], xmm1
	movaps		[eax + 32], xmm2
;	movlps		xmm1, [ecx]
;	movlps		xmm0, [ecx + 24]
;	movlps		xmm2, [ecx + 8]
;	movhps		xmm1, [ecx + 12]
;	movhps		xmm0, [ecx + 32]
;	movhps		xmm2, [ecx + 20]
;	movaps		xmm3, xmm1
;	shufps		xmm1, xmm0, 0xc8
;	shufps		xmm3, xmm0, 0xdd
;	shufps		xmm2, xmm0, 0xe8
;	movaps		[eax], xmm1
;	movlps		[eax + 12], xmm3
;	movhlps		xmm0, xmm2
;	movhps		[eax + 20], xmm3
;	movlps		[eax + 24], xmm2
;	movss		[eax + 32], xmm0

;
;----------------
;MatrixTranspose4x4_ASM
;----------------
;
_MatrixTranspose4x4_ASM:
	mov			ecx, dword [esp + 8]					;float *mat
	mov			eax, dword [esp + 4]					;float *dest
	movlps		xmm1, [ecx]
	movlps		xmm3, [ecx + 8]
	movhps		xmm1, [ecx + 16]
	movhps		xmm3, [ecx + 24]
	movlps		xmm2, [ecx + 32]
	movlps		xmm4, [ecx + 40]
	movhps		xmm2, [ecx + 48]
	movhps		xmm4, [ecx + 56]
	movaps		xmm0, xmm1
	shufps		xmm0, xmm2, 0x88
	shufps		xmm1, xmm2, 0xdd
	movaps		xmm2, xmm3
	shufps		xmm2, xmm4, 0x88
	shufps		xmm3, xmm4, 0xdd
	movaps		[eax], xmm0
	movaps		[eax + 16], xmm1
	movaps		[eax + 32], xmm2
	movaps		[eax + 48], xmm3
	ret

;
;----------------
;MatrixMultiply3x3_ASM
;----------------
;
_MatrixMultiply3x3_ASM:
	mov			edx, dword [esp + 8]					;float *mat1
	mov			ecx, dword [esp + 12]					;float *mat2
	mov			eax, dword [esp + 4]					;float *dest
	movss		xmm0, [edx]
	movss		xmm1, [edx + 4]
	movss		xmm2, [edx + 8]
	shufps		xmm0, xmm0, 0x0
	shufps		xmm1, xmm1, 0x0
	shufps		xmm2, xmm2, 0x0
	movaps		xmm5, [ecx]
	movaps		xmm6, [ecx + 16]
	movaps		xmm7, [ecx + 32]
	mulps		xmm0, xmm5
	mulps		xmm1, xmm6
	mulps		xmm2, xmm7
	addps		xmm0, xmm1
	addps		xmm0, xmm2
	movss		xmm1, [edx + 16]
	movss		xmm2, [edx + 20]
	movss		xmm3, [edx + 24]
	shufps		xmm1, xmm1, 0x0
	shufps		xmm2, xmm2, 0x0
	shufps		xmm3, xmm3, 0x0
	mulps		xmm1, xmm5
	mulps		xmm2, xmm6
	mulps		xmm3, xmm7
	addps		xmm1, xmm2
	addps		xmm1, xmm3
	movss		xmm2, [edx + 32]
	movss		xmm3, [edx + 36]
	movss		xmm4, [edx + 40]
	shufps		xmm2, xmm2, 0x0
	shufps		xmm3, xmm3, 0x0
	shufps		xmm4, xmm4, 0x0
	mulps		xmm2, xmm5
	mulps		xmm3, xmm6
	mulps		xmm4, xmm7
	addps		xmm2, xmm3
	addps		xmm2, xmm4
	movaps		[eax], xmm0
	movaps		[eax + 16], xmm1
	movaps		[eax + 32], xmm2
	ret

;
;----------------
;MatrixMultiply4x4_ASM
;----------------
;
_MatrixMultiply4x4_ASM:
	mov			edx, dword [esp + 8]					;float *mat1
	mov			ecx, dword [esp + 12]					;float *mat2
	mov			eax, dword [esp + 4]					;float *dest
	movss		xmm0, [edx]
	movss		xmm1, [edx + 4]
	movss		xmm2, [edx + 8]
	movss		xmm3, [edx + 12]
	shufps		xmm0, xmm0, 0x0
	shufps		xmm1, xmm1, 0x0
	shufps		xmm2, xmm2, 0x0
	shufps		xmm3, xmm3, 0x0
	movaps		xmm4, [ecx]
	movaps		xmm5, [ecx + 16]
	movaps		xmm6, [ecx + 32]
	movaps		xmm7, [ecx + 48]
	mulps		xmm0, xmm4
	mulps		xmm1, xmm5
	mulps		xmm2, xmm6
	mulps		xmm3, xmm7
	addps		xmm0, xmm1
	addps		xmm0, xmm2
	addps		xmm0, xmm3
	movaps		[eax], xmm0
	movss		xmm0, [edx + 16]
	movss		xmm1, [edx + 20]
	movss		xmm2, [edx + 24]
	movss		xmm3, [edx + 28]
	shufps		xmm0, xmm0, 0x0
	shufps		xmm1, xmm1, 0x0
	shufps		xmm2, xmm2, 0x0
	shufps		xmm3, xmm3, 0x0
	mulps		xmm0, xmm4
	mulps		xmm1, xmm5
	mulps		xmm2, xmm6
	mulps		xmm3, xmm7
	addps		xmm0, xmm1
	addps		xmm0, xmm2
	addps		xmm0, xmm3
	movaps		[eax + 16], xmm0
	movss		xmm0, [edx + 32]
	movss		xmm1, [edx + 36]
	movss		xmm2, [edx + 40]
	movss		xmm3, [edx + 44]
	shufps		xmm0, xmm0, 0x0
	shufps		xmm1, xmm1, 0x0
	shufps		xmm2, xmm2, 0x0
	shufps		xmm3, xmm3, 0x0
	mulps		xmm0, xmm4
	mulps		xmm1, xmm5
	mulps		xmm2, xmm6
	mulps		xmm3, xmm7
	addps		xmm0, xmm1
	addps		xmm0, xmm2
	addps		xmm0, xmm3
	movaps		[eax + 32], xmm0
	movss		xmm0, [edx + 48]
	movss		xmm1, [edx + 52]
	movss		xmm2, [edx + 56]
	movss		xmm3, [edx + 60]
	shufps		xmm0, xmm0, 0x0
	shufps		xmm1, xmm1, 0x0
	shufps		xmm2, xmm2, 0x0
	shufps		xmm3, xmm3, 0x0
	mulps		xmm0, xmm4
	mulps		xmm1, xmm5
	mulps		xmm2, xmm6
	mulps		xmm3, xmm7
	addps		xmm0, xmm1
	addps		xmm0, xmm2
	addps		xmm0, xmm3
	movaps		[eax + 48], xmm0
	ret

;
;----------------
;MatrixLoadIdentity_ASM
;----------------
;
_MatrixLoadIdentity_ASM:
	mov			eax, dword [_gmat]
	movaps		xmm0, [constant_ident_1_ps]
	movaps		xmm1, [constant_ident_2_ps]
	movaps		xmm2, [constant_ident_3_ps]
	movaps		xmm3, [constant_ident_4_ps]
	movaps		[eax], xmm0
	movaps		[eax + 16], xmm1
	movaps		[eax + 32], xmm2
	movaps		[eax + 48], xmm3
	ret

;
;----------------
;MatrixTranslate_ASM
;----------------
;
_MatrixTranslate_ASM:
	mov			ecx, dword [_gmat]
	mov			eax, dword [esp + 4]					;float *vpos
;	mov			ecx, tempvec
;	fld			dword [ebx + 12]
;	fld			dword [ebx + 28]
;	fld			dword [ebx + 44]
;fld			dword [ebx + 60]
;	fxch		st2
;	fstp		dword [ecx]
;fstp		dword [ecx + 12]
;	fstp		dword [ecx + 4]
;	fstp		dword [ecx + 8]
;	movaps		xmm0, [eax]
;	movaps		xmm1, [tempvec]
;	addps		xmm0, xmm1
;	movaps		[tempvec], xmm0
;	fld			dword [ecx]
;	fld			dword [ecx + 4]
;	fld			dword [ecx + 8]
;fld			dword [ecx + 12]
;	fxch		st2
;	fstp		dword [ebx + 12]
;fstp		dword [ebx + 60]
;	fstp		dword [ebx + 28]
;	fstp		dword [ebx + 44]
	movss		xmm0, [ecx + 12]
	movss		xmm2, [ecx + 28]
	movss		xmm4, [ecx + 44]
;movss		xmm6, [ebx + 60]
	movss		xmm1, [eax]
	movss		xmm3, [eax + 4]
	movss		xmm5, [eax + 8]
;movss		xmm7, [eax + 12]
	addss		xmm0, xmm1
	addss		xmm2, xmm3
	addss		xmm4, xmm5
;addss		xmm6, xmm7
	movss		[ecx + 12], xmm0
	movss		[ecx + 28], xmm2
	movss		[ecx + 44], xmm4
;movss		[ebx + 60], xmm6
	ret

;
;----------------
;MatrixScale_ASM
;----------------
;
_MatrixScale_ASM:
	mov			ecx, dword [_gmat]
	mov			eax, dword [esp + 4]					;float sfac
	movss		xmm0, [ecx]
	movss		xmm2, [ecx + 20]
	movss		xmm4, [ecx + 40]
	movss		xmm1, [eax]
	movss		xmm3, [eax + 4]
	movss		xmm5, [eax + 8]
	mulss		xmm0, xmm1
	mulss		xmm2, xmm3
	mulss		xmm4, xmm5
	movss		[ecx], xmm0
	movss		[ecx + 20], xmm2
	movss		[ecx + 40], xmm4
	ret

;
;----------------
;MatrixRotate_ASM
;----------------
;
_MatrixRotate_ASM:
		mov			eax, dword [esp + 4]					;float *angles
;FIXME: do not store this in temp_esp; this won't work if multiple threads call this function!!!
	mov			dword [temp_esp], esp
	sub			esp, 110								;allocate temp 16-byte aligned stack space
	and			esp, 0xfffffff0
	movaps		xmm1, [eax]								;calculate parallel sines
	movaps		xmm2, xmm1
	movaps		xmm3, xmm1
	mulps		xmm2, [constant_inv_360_ps]
	movhlps		xmm0, xmm2
	cmpltps		xmm3, [constant_zero_ps]
	cvttss2si	edx, xmm2								;x = floor(a / 360) * 360
	shufps		xmm2, xmm2, 0x1
	cvttss2si	ecx, xmm0
	cvtsi2ss	xmm4, edx
	cvttss2si	edx, xmm2
	cvtsi2ss	xmm0, ecx
	cvtsi2ss	xmm2, edx
	unpcklps	xmm4, xmm2
	andps		xmm3, [constant_one_ps]					;a < 0 ? 1 : 0
	movlhps		xmm4, xmm0
	subps		xmm4, xmm3								;if a < 0, x -= 1
	mulps		xmm4, [constant_360_ps]
	subps		xmm1, xmm4
	movaps		xmm0, [constant_180_ps]
	subps		xmm0, xmm1
	movaps		xmm1, xmm0
	movaps		xmm2, xmm0
	andps		xmm1, [constant_signmsk_ps]
	xorps		xmm2, xmm1								;fabs(180 - a)
	cmpnltps	xmm2, [constant_90_ps]
	movaps		xmm3, [constant_180_ps]
	xorps		xmm3, xmm1								;if a < 0, -180
	andps		xmm3, xmm2
	andps		xmm2, [constant_signmsk_ps]
	xorps		xmm0, xmm2
	addps		xmm0, xmm3

	movaps		xmm1, [aconvert_ps]
	mulps		xmm0, xmm1
	movaps		xmm2, xmm0
	mulps		xmm0, xmm0
	movaps		xmm1, [sine_constant4_ps]
	mulps		xmm1, xmm0
	addps		xmm1, [sine_constant5_ps]
	mulps		xmm1, xmm0
	addps		xmm1, [constant_one_ps]
	mulps		xmm1, xmm2								;sin(x, y, z)

	movaps		xmm0, [eax]								;calculate parallel cosines
	addps		xmm0, [constant_90_ps]
	movaps		xmm2, xmm0
	movaps		xmm3, xmm0
	mulps		xmm2, [constant_inv_360_ps]
	movhlps		xmm4, xmm2
	cmpltps		xmm3, [constant_zero_ps]
	cvttss2si	edx, xmm2
	shufps		xmm2, xmm2, 0x1
	cvttss2si	ecx, xmm4
	cvtsi2ss	xmm5, edx
	cvttss2si	edx, xmm2
	cvtsi2ss	xmm4, ecx
	cvtsi2ss	xmm2, edx
	unpcklps	xmm5, xmm2
	andps		xmm3, [constant_one_ps]
	movlhps		xmm5, xmm4
	subps		xmm5, xmm3
	mulps		xmm5, [constant_360_ps]
	subps		xmm0, xmm5
	movaps		xmm5, [constant_180_ps]
	subps		xmm5, xmm0
	movaps		xmm0, xmm5
	movaps		xmm2, xmm5
	andps		xmm0, [constant_signmsk_ps]
	xorps		xmm2, xmm0
	cmpnltps	xmm2, [constant_90_ps]
	movaps		xmm3, [constant_180_ps]
	xorps		xmm3, xmm0
	andps		xmm3, xmm2
	andps		xmm2, [constant_signmsk_ps]
	xorps		xmm5, xmm2
	addps		xmm5, xmm3

	movaps		xmm0, [aconvert_ps]
	mulps		xmm5, xmm0
	movaps		xmm2, xmm5
	mulps		xmm5, xmm5
	movaps		xmm0, [sine_constant4_ps]
	mulps		xmm0, xmm5
	addps		xmm0, [sine_constant5_ps]
	mulps		xmm0, xmm5
	addps		xmm0, [constant_one_ps]
	mulps		xmm0, xmm2								;cos(x, y, z)
	movaps		[esp], xmm1
	movaps		[esp + 16], xmm0
;	mov			eax, temp_vec_1
;	mov			ecx, temp_vec_2
;	mov			ecx, storage_vec_1_ps
;[cos(y)cos(r) ------------------ -cos(y)sin(r) ----------------- sin(y) --------------------- 0]
	fld			dword [esp + 16 + 4]
	fmul		dword [esp + 16 + 8]
	fld			dword [esp + 16 + 4]
	fmul		dword [esp + 8]
	fld			dword [esp + 4]
	fxch		st2
	fstp		dword [esp + 32]						;temp storage of the rotation matrix
	fchs
;	mov			edx, storage_vec_2_ps
	fstp		dword [esp + 32 + 4]
	fstp		dword [esp + 32 + 8]
;[cos(y)cos(r) ------------------ -cos(y)sin(r) ----------------- sin(y) --------------------- 0]
;[cos(r)sin(p)sin(y)+cos(p)sin(r) cos(p)cos(r)-sin(p)sin(y)sin(r) -cos(y)sin(p) -------------- 0]
	fld			dword [esp]
	fmul		dword [esp + 16 + 8]
	fld			dword [esp + 16]
	fmul		dword [esp + 8]
	fxch		st1
	fmul		dword [esp + 4]
	fld			dword [esp]
	fmul		dword [esp + 4]
	fld			dword [esp + 16]
	fmul		dword [esp + 16 + 8]
	fxch		st1
	fmul		dword [esp + 8]
	fld			dword [esp + 16 + 4]
	fmul		dword [esp]
	fxch		st4
	faddp		st3
	fsubp		st1
	fxch		st2
	fchs
;	mov			edi, storage_vec_3_ps
	fstp		dword [esp + 48 + 8]
	fstp		dword [esp + 48]
	fstp		dword [esp + 48 + 4]
;[cos(y)cos(r) ------------------ -cos(y)sin(r) ----------------- sin(y) --------------------- 0]
;[cos(r)sin(p)sin(y)+cos(p)sin(r) cos(p)cos(r)-sin(p)sin(y)sin(r) -cos(y)sin(p) -------------- 0]
;[sin(p)sin(r)-cos(p)cos(r)sin(y) cos(p)sin(y)sin(r)+cos(r)sin(p) cos(p)cos(y) --------------- 0]
;[0 ----------------------------- 0 ----------------------------- 0 -------------------------- 1]
	fld			dword [esp + 16]
	fmul		dword [esp + 16 + 8]
	fld			dword [esp]
	fmul		dword [esp + 8]
	fxch		st1
	fmul		dword [esp + 4]
	fld			dword [esp + 16]
	fmul		dword [esp + 4]
	fld			dword [esp + 16 + 8]
	fmul		dword [esp]
	fxch		st1
	fmul		dword [esp + 8]
	fld			dword [esp + 16]
	fmul		dword [esp + 16 + 4]
	fxch		st3
	fsubp		st4
	faddp		st1
	fxch		st2
	fstp		dword [esp + 64]
	fstp		dword [esp + 64 + 8]
	fstp		dword [esp + 64 + 4]
	mov			eax, dword [_gmat]
	movss		xmm0, [esp + 32]
	movss		xmm1, [esp + 32 + 4]
	movss		xmm2, [esp + 32 + 8]
	shufps		xmm0, xmm0, 0x0
	shufps		xmm1, xmm1, 0x0
	shufps		xmm2, xmm2, 0x0
	movaps		xmm3, [eax]
	movaps		xmm4, [eax + 16]
	movaps		xmm5, [eax + 32]
	mulps		xmm0, xmm3
	mulps		xmm1, xmm4
	mulps		xmm2, xmm5
	addps		xmm0, xmm1
	addps		xmm0, xmm2
	movaps		[eax], xmm0
	movss		xmm0, [esp + 48]
	movss		xmm1, [esp + 48 + 4]
	movss		xmm2, [esp + 48 + 8]
	shufps		xmm0, xmm0, 0x0
	shufps		xmm1, xmm1, 0x0
	shufps		xmm2, xmm2, 0x0
	mulps		xmm0, xmm3
	mulps		xmm1, xmm4
	mulps		xmm2, xmm5
	addps		xmm0, xmm1
	addps		xmm0, xmm2
	movaps		[eax + 16], xmm0
	movss		xmm0, [esp + 64]
	movss		xmm1, [esp + 64 + 4]
	movss		xmm2, [esp + 64 + 8]
	shufps		xmm0, xmm0, 0x0
	shufps		xmm1, xmm1, 0x0
	shufps		xmm2, xmm2, 0x0
	mulps		xmm0, xmm3
	mulps		xmm1, xmm4
	mulps		xmm2, xmm5
	addps		xmm0, xmm1
	addps		xmm0, xmm2
	movaps		[eax + 32], xmm0
	mov			esp, dword [temp_esp]
	ret

	section .data data align=16

align 16
constant_one_pd			dq		1.0, 1.0
constant_signmsk_ps		dd		0x80000000, 0x80000000, 0x80000000, 0x80000000
constant_one_ps			dd		1.0, 1.0, 1.0, 1.0
constant_half_ps		dd		0.5, 0.5, 0.5, 0.5
constant_zero_ps		dd		0.0, 0.0, 0.0, 0.0
constant_360_ps			dd		360.0, 360.0, 360.0, 360.0
constant_180_ps			dd		180.0, 180.0, 180.0, 180.0
constant_90_ps			dd		90.0, 90.0, 90.0, 90.0
constant_inv_360_ps		dd		2.77777777777777777e-3, 2.77777777777777777e-3, 2.77777777777777777e-3, 2.77777777777777777e-3
constant_inv_180_ps		dd		5.55555555555555555e-3, 5.55555555555555555e-3, 5.55555555555555555e-3, 5.55555555555555555e-3
constant_neg_180_ps		dd		-180.0, -180.0, -180.0, -180.0
aconvert_ps				dd		1.74532925199432957e-2, 1.74532925199432957e-2, 1.74532925199432957e-2, 1.74532925199432957e-2
sine_constant3_ps		dd		-1.98412698412698412e-4, -1.98412698412698412e-4, -1.98412698412698412e-4, -1.98412698412698412e-4
sine_constant4_ps		dd		8.33333333333333333e-3, 8.33333333333333333e-3, 8.33333333333333333e-3, 8.33333333333333333e-3
sine_constant5_ps		dd		-1.66666666666666667e-1, -1.66666666666666667e-1, -1.66666666666666667e-1, -1.66666666666666667e-1
cosine_constant1_ps		dd		-2.75573192239858906e-7, -2.75573192239858906e-7, -2.75573192239858906e-7, -2.75573192239858906e-7
cosine_constant2_ps		dd		2.48015873015873015e-5, 2.48015873015873015e-5, 2.48015873015873015e-5, 2.48015873015873015e-5
cosine_constant3_ps		dd		-1.38888888888888888e-3, -1.38888888888888888e-3, -1.38888888888888888e-3, -1.38888888888888888e-3
cosine_constant4_ps		dd		4.16666666666666666e-2, 4.16666666666666666e-2, 4.16666666666666666e-2, 4.16666666666666666e-2
cosine_constant5_ps		dd		-0.5, -0.5, -0.5, -0.5
constant_ident_1_ps		dd		1.0, 0.0, 0.0, 0.0
constant_ident_2_ps		dd		0.0, 1.0, 0.0, 0.0
constant_ident_3_ps		dd		0.0, 0.0, 1.0, 0.0
constant_ident_4_ps		dd		0.0, 0.0, 0.0, 1.0
constant_mat_mask_ps	dd		0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0
constant_vec_mask_ps	dd		0.0, 0.0, 0.0, 1.0

	section .bss bss align=16

align 16
temp_esp				resd	1

_cpu_vendor_id			resb	13
_cpu_model				resd	1
_cpu_family				resd	1
_cpu_ext_family			resd	1
_extcpu_model			resd	1
_extcpu_family			resd	1
_extcpu_ext_family		resd	1
_cpu_count				resd	1
_cpu_hyperthreading		resd	1
_cpu_64_bit				resd	1
_cpu_mmx				resd	1
_cpu_sse				resd	1
_cpu_sse2				resd	1
_cpu_sse3				resd	1
_cpu_3dnow				resd	1
_cpu_3dnow2				resd	1
_cpu_rdtsc				resd	1
_cpu_fpu				resd	1
_cpu_fcmov				resd	1
_cpu_extmmx				resd	1
_cpu_ext_vendorid		resb	49
_cpu_extended_info		resd	1
_cpu_ht_thread			resd	1
_cpu_ssse3				resd	1
_cpu_sse4_1				resd	1
_cpu_sse4_2				resd	1
_cpu_sse4a				resd	1
_cpu_sse5a				resd	1
extcpu_max_level		resd	1
clock_tics				resd	1