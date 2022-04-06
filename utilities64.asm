;64-bit FPU optimized math routines

	bits 64
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
	default rel

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
;Win64
_ThreadSpinWait:
;	rcx													;int thread_num
	mov			rdx, qword [_thread_info]
	shl			rcx, THREAD_POT
TSWLoop:
	cmp			dword [_end_threads], 0
	jne			TSWEndLoop1
	cmp			dword [rdx + rcx + 48], 0
	je			TSWEndLoop0
	jmp			TSWLoop
TSWEndLoop1:
	mov			rax, 1
	ret													;int
TSWEndLoop0:
	xor			rax, rax
	ret													;int

;
;----------------
;GetCPUInfo_Util
;----------------
;
;Win64
_GetCPUInfo_Util:
;	rcx													;int *regs4
	mov			r8, rcx
	push		rbx
	mov			eax, dword [r8]
	mov			ebx, dword [r8 + 4]
	mov			ecx, dword [r8 + 8]
	mov			edx, dword [r8 + 12]
	cpuid
	mov			dword [r8], eax
	mov			dword [r8 + 4], ebx
	mov			dword [r8 + 8], ecx
	mov			dword [r8 + 12], edx
	pop			rbx
	ret

;
;----------------
;GetCPUInfo
;----------------
;
_GetCPUInfo:
	mov			r8, qword _cpu_vendor_id
	push		rbx
	push		rsi
	push		rdi
	pushfq
	pop			rax
	mov			ebx, eax
	xor			eax, 0x200000
	push		rax
	popfq
	xor			eax, ebx
	jz			NoCPUID
	xor			eax, eax
	cpuid
	test		eax, 0xffffffff
	jz			NoCPUID
	mov			dword [r8], ebx
	mov			dword [r8 + 4], edx
	mov			dword [r8 + 8], ecx
	mov			byte [r8 + 12], 0
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
	mov			rsi, qword _cpu_ext_vendorid
	mov			dword [rsi], eax
	mov			dword [rsi + 4], ebx
	mov			dword [rsi + 8], ecx
	mov			dword [rsi + 12], edx
	mov			eax, 0x80000003
	cpuid
	mov			dword [rsi + 16], eax
	mov			dword [rsi + 20], ebx
	mov			dword [rsi + 24], ecx
	mov			dword [rsi + 28], edx
	mov			eax, 0x80000004
	cpuid
	mov			dword [rsi + 32], eax
	mov			dword [rsi + 36], ebx
	mov			dword [rsi + 40], ecx
	mov			dword [rsi + 44], edx
	mov			byte [rsi + 48], 0
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
	pop			rdi
	pop			rsi
	pop			rbx
	mov			eax, 1
	ret
NoCPUID:
	pop			rdi
	pop			rsi
	pop			rbx
	xor			eax, eax
	ret

;
;----------------
;Sqroot_ASM
;----------------
;
;Win64 Linux64
_Sqroot_ASM:
	;xmm0												;double value
	sqrtsd		xmm0, xmm0
	ret													;double

;
;----------------
;RSqroot_ASM
;----------------
;
;Win64 Linux64
_RSqroot_ASM:
	;xmm0												;double value
	sqrtsd		xmm1, xmm0
	movsd		xmm0, [constant_one_pd]
	divsd		xmm0, xmm1
	ret													;double

;
;----------------
;Sine_ASM
;----------------
;
;Win64 Linux64
_Sine_ASM:
	;xmm0												;double theta
	cvtsd2ss	xmm0, xmm0
	movss		xmm2, xmm0
	movss		xmm3, xmm0
	mulss		xmm2, [constant_inv_360_ps]
	cmpltss		xmm3, [constant_zero_ps]
	cvttss2si	edx, xmm2								;x = floor(a / 360) * 360
	andps		xmm3, [constant_one_ps]					;a < 0 ? 1 : 0
	cvtsi2ss	xmm2, edx
	subss		xmm2, xmm3								;if a < 0, x -= 1
	mulss		xmm2, [constant_360_ps]
	subss		xmm0, xmm2
	movss		xmm1, [constant_180_ps]
	subss		xmm1, xmm0
	movss		xmm0, xmm1
	movss		xmm2, xmm1
	andps		xmm0, [constant_signmsk_ps]
	xorps		xmm2, xmm0								;fabs(180 - a)
	cmpnltss	xmm2, [constant_90_ps]
	movss		xmm3, [constant_180_ps]
	xorps		xmm3, xmm0								;if a < 0, -180
	andps		xmm3, xmm2
	andps		xmm2, [constant_signmsk_ps]
	xorps		xmm1, xmm2
	addps		xmm1, xmm3

	movss		xmm0, [aconvert_ps]
	mulss		xmm1, xmm0
	movss		xmm2, xmm1
	mulss		xmm1, xmm1
;	movss		xmm0, [sine_constant1_ps]
;	mulss		xmm0, xmm1
;	addss		xmm0, [sine_constant2_ps]
;	mulss		xmm0, xmm1
;	addss		xmm0, [sine_constant3_ps]
;	mulss		xmm0, xmm1
	movss		xmm0, [sine_constant4_ps]
	mulss		xmm0, xmm1
	addss		xmm0, [sine_constant5_ps]
	mulss		xmm0, xmm1
	addss		xmm0, [constant_one_ps]
	mulss		xmm0, xmm2
	cvtss2sd	xmm0, xmm0
	ret													;double

;
;----------------
;Cosine_ASM
;----------------
;
;Win64 Linux64
_Cosine_ASM:
	;xmm0												;double theta
	cvtsd2ss	xmm0, xmm0
	addss		xmm0, [constant_90_ps]
	movss		xmm2, xmm0
	movss		xmm3, xmm0
	mulss		xmm2, [constant_inv_360_ps]
	cmpltss		xmm3, [constant_zero_ps]
	cvttss2si	edx, xmm2
	andps		xmm3, [constant_one_ps]
	cvtsi2ss	xmm2, edx
	subss		xmm2, xmm3
	mulss		xmm2, [constant_360_ps]
	subss		xmm0, xmm2
	movss		xmm1, [constant_180_ps]
	subss		xmm1, xmm0
	movss		xmm0, xmm1
	movss		xmm2, xmm1
	andps		xmm0, [constant_signmsk_ps]
	xorps		xmm2, xmm0
	cmpnltss	xmm2, [constant_90_ps]
	movss		xmm3, [constant_180_ps]
	xorps		xmm3, xmm0
	andps		xmm3, xmm2
	andps		xmm2, [constant_signmsk_ps]
	xorps		xmm1, xmm2
	addps		xmm1, xmm3

	movss		xmm0, [aconvert_ps]
	mulss		xmm1, xmm0
	movss		xmm2, xmm1
	mulss		xmm1, xmm1
;	movss		xmm0, [sine_constant1_ps]
;	mulss		xmm0, xmm1
;	addss		xmm0, [sine_constant2_ps]
;	mulss		xmm0, xmm1
;	addss		xmm0, [sine_constant3_ps]
;	mulss		xmm0, xmm1
	movss		xmm0, [sine_constant4_ps]
	mulss		xmm0, xmm1
	addss		xmm0, [sine_constant5_ps]
	mulss		xmm0, xmm1
	addss		xmm0, [constant_one_ps]
	mulss		xmm0, xmm2
	cvtss2sd	xmm0, xmm0
	ret													;double

;
;----------------
;Tangent_ASM
;----------------
;
;Win64 Linux64
_Tangent_ASM:
	;xmm0												;double theta
	cvtsd2ss	xmm0, xmm0
	movss		xmm1, xmm0
	movss		xmm2, xmm0
	mulss		xmm1, [constant_inv_180_ps]
	cmpltss		xmm0, [constant_zero_ps]
	cvttss2si	edx, xmm1
	andps		xmm0, [constant_one_ps]
	cvtsi2ss	xmm1, edx
	subss		xmm1, xmm0
	mulss		xmm1, [constant_180_ps]
	subss		xmm2, xmm1
	movss		xmm1, xmm2
	cmpnltss	xmm1, [constant_90_ps]
	andps		xmm1, [constant_one_ps]
	mulss		xmm1, [constant_180_ps]
	subss		xmm2, xmm1
	movss		xmm1, [aconvert_ps]
	mulss		xmm2, xmm1
	movss		xmm3, xmm2
	mulss		xmm2, xmm2								;cosine
	movss		xmm1, [cosine_constant1_ps]
	mulss		xmm1, xmm2
	addss		xmm1, [cosine_constant2_ps]
	mulss		xmm1, xmm2
	addss		xmm1, [cosine_constant3_ps]
	mulss		xmm1, xmm2
	addss		xmm1, [cosine_constant4_ps]
	mulss		xmm1, xmm2
	addss		xmm1, [cosine_constant5_ps]
	mulss		xmm1, xmm2
	addss		xmm1, [constant_one_ps]

	movss		xmm0, [sine_constant3_ps]				;sine
	mulss		xmm0, xmm2
	addss		xmm0, [sine_constant4_ps]
	mulss		xmm0, xmm2
	addss		xmm0, [sine_constant5_ps]
	mulss		xmm0, xmm2
	addss		xmm0, [constant_one_ps]
	mulss		xmm0, xmm3
	divss		xmm0, xmm1								;sine / cosine
	cvtss2sd	xmm0, xmm0
	ret													;double

;
;----------------
;VectorMove_ASM
;----------------
;
;Win64
_VectorMove_ASM:
	;rcx												;float *dest
	;rdx												;float *src
	movaps		xmm0, [rdx]
	movaps		[rcx], xmm0
	ret

;
;----------------
;VectorMoveScalar_ASM
;----------------
;
;Win64
_VectorMoveScalar_ASM:
	;rcx												;float *dest
	;xmm1												;float src
	shufps		xmm1, xmm1, 0
	movaps		[rcx], xmm1
	ret

;
;----------------
;VectorAdd_ASM
;----------------
;
;Win64
_VectorAdd_ASM:
	;rcx												;float *dest
	;rdx												;float *src1
	;r8													;float *src2
	movaps		xmm0, [rdx]
	addps		xmm0, [r8]
	movaps		[rcx], xmm0
	ret

;
;----------------
;VectorAddScalar_ASM
;----------------
;
;Win64
_VectorAddScalar_ASM:
	;rcx												;float *dest
	;rdx												;float *src1
	;xmm2												;float src2
	shufps		xmm2, xmm2, 0
	addps		xmm2, [rdx]
	movaps		[rcx], xmm2
	ret

;
;----------------
;VectorSubtract_ASM
;----------------
;
;Win64
_VectorSubtract_ASM:
	;rcx												;float *dest
	;rdx												;float *src1
	;r8													;float *src2
	movaps		xmm0, [rdx]
	subps		xmm0, [r8]
	movaps		[rcx], xmm0
	ret

;
;----------------
;VectorSubtractScalar_ASM
;----------------
;
;Win64
_VectorSubtractScalar_ASM:
	;rcx												;float *dest
	;rdx												;float *src1
	;xmm2												float src2
	shufps		xmm2, xmm2, 0
	movaps		xmm0, [rdx]
	subps		xmm0, xmm2
	movaps		[rcx], xmm0
	ret

;
;----------------
;VectorMultiply_ASM
;----------------
;
;Win64
_VectorMultiply_ASM:
	;rcx												;float *dest
	;rdx												;float *src1
	;r8													;float *src2
	movaps		xmm0, [rdx]
	mulps		xmm0, [r8]
	movaps		[rcx], xmm0
	ret

;
;----------------
;VectorMultiplyScalar_ASM
;----------------
;
;Win64
_VectorMultiplyScalar_ASM:
	;rcx												;float *dest
	;rdx												;float *src1
	;xmm2												;float src2
	shufps		xmm2, xmm2, 0
	mulps		xmm2, [rdx]
	movaps		[rcx], xmm2
	ret

;
;----------------
;VectorMultiplyAdd_ASM
;----------------
;
;Win64
_VectorMultiplyAdd_ASM:
	;rcx												;float *dest
	;rdx												;float *src1
	;r8													;float *src2
	;r9													;float *src3
	movaps		xmm0, [rdx]
	mulps		xmm0, [r8]
	addps		xmm0, [r9]
	movaps		[rcx], xmm0
	ret

;
;----------------
;VectorMultiplyAddScalar_ASM
;----------------
;
;Win64
_VectorMultiplyAddScalar_ASM:
	;rcx												;float *dest
	;rdx												;float *src1
	;xmm2												;float src2
	;r9													;float *src3
	shufps		xmm2, xmm2, 0
	mulps		xmm2, [rdx]
	addps		xmm2, [r9]
	movaps		[rcx], xmm2
	ret

;
;----------------
;VectorDotProduct3_ASM
;----------------
;
;Win64
_VectorDotProduct3_ASM:
	;rcx												;float *src1
	;rdx												;float *src2
	movaps		xmm0, [rcx]
	mulps		xmm0, [rdx]
	movhlps		xmm1, xmm0
	movaps		xmm2, xmm0
	shufps		xmm0, xmm0, 0x55
	addss		xmm1, xmm2
	addss		xmm0, xmm1
	ret													;float

;
;----------------
;VectorDotProduct4_ASM
;----------------
;
;Win64
_VectorDotProduct4_ASM:
	;rcx												;float *src1
	;rdx												;float *src2
	movaps		xmm0, [rcx]
	mulps		xmm0, [rdx]
	movhlps		xmm1, xmm0
	addps		xmm0, xmm1
	movaps		xmm2, xmm0
	shufps		xmm0, xmm0, 0x55
	addss		xmm0, xmm2
	ret													;float

;
;----------------
;VectorDotProductH_ASM
;----------------
;
;Win64
_VectorDotProductH_ASM:
	;rcx												;float *src1
	;rdx												;float *src2
	movaps		xmm0, [rcx]
	movaps		xmm1, [rdx]
	mulps		xmm0, xmm1
	movhlps		xmm2, xmm0
	movaps		xmm3, xmm0
	shufps		xmm0, xmm0, 0x55
	shufps		xmm1, xmm1, 0xff
	addss		xmm2, xmm3
	addss		xmm0, xmm1
	addss		xmm0, xmm2
	ret													;float

;
;----------------
;VectorCrossProduct_ASM
;----------------
;
;Win64
_VectorCrossProduct_ASM:
	;rcx												;float *dest
	;rdx												;float *src1
	;r8													;float *src2
	movaps		xmm0, [rdx]
	movaps		xmm1, [r8]
	movaps		xmm2, xmm0
	movaps		xmm3, xmm1
	shufps		xmm0, xmm0, 0xc9
	shufps		xmm1, xmm1, 0xd2
	shufps		xmm2, xmm2, 0xd2
	shufps		xmm3, xmm3, 0xc9
	mulps		xmm0, xmm1
	mulps		xmm2, xmm3
	subps		xmm0, xmm2
	movaps		[rcx], xmm0
	ret

;
;----------------
;VectorNormalize_ASM
;----------------
;
;Win64
_VectorNormalize_ASM:
	;rcx												;float *vec
	movaps		xmm0, [rcx]
	movaps		xmm3, xmm0
	mulps		xmm0, xmm0
	movhlps		xmm1, xmm0
	movaps		xmm2, xmm0
	shufps		xmm0, xmm0, 0x55
	addss		xmm1, xmm2
	addss		xmm0, xmm1
	rsqrtss		xmm0, xmm0
	shufps		xmm0, xmm0, 0x0
	mulps		xmm0, xmm3
	movaps		[rcx], xmm0
	ret

;
;----------------
;CreatePlane_ASM
;----------------
;
;Win64
_CreatePlane_ASM:
	;rcx												;float *plane
	;rdx												;float *v1
	;r8													;float *v2
	;r9													;float *v3
	movaps		xmm0, [rdx]
	movaps		xmm1, [r9]
	subps		xmm0, [r8]								;v1 - v2
	subps		xmm1, [r8]								;v3 - v2
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
	movaps		[rcx], xmm0
	movaps		xmm1, [rdx]
	mulps		xmm0, xmm1								;dot product
	movhlps		xmm1, xmm0
	movaps		xmm2, xmm0
	shufps		xmm0, xmm0, 0x55
	addss		xmm1, xmm2
	addss		xmm0, xmm1
	xorps		xmm0, [constant_signmsk_ps]
	movss		[rcx + 12], xmm0
	ret

;
;----------------
;VectorMatrixMultiply3_ASM
;----------------
;
;Win64
_VectorMatrixMultiply3_ASM:
	;rcx												;float *out
	;rdx												;float *vec
	;r8													;float *mat
	movaps		xmm0, [rdx]
	movaps		xmm1, [r8]
	movaps		xmm2, [r8 + 16]
	movaps		xmm3, [r8 + 32]
	mulps		xmm1, xmm0
	mulps		xmm2, xmm0
	mulps		xmm3, xmm0
	movhlps		xmm0, xmm1
	movaps		xmm4, xmm1
	shufps		xmm1, xmm1, 0x55
	addss		xmm0, xmm4
	addss		xmm1, xmm0
	movss		[rcx], xmm1
	movhlps		xmm0, xmm2
	movaps		xmm4, xmm2
	shufps		xmm2, xmm2, 0x55
	addss		xmm0, xmm4
	addss		xmm2, xmm0
	movss		[rcx + 4], xmm2
	movhlps		xmm0, xmm3
	movaps		xmm4, xmm3
	shufps		xmm3, xmm3, 0x55
	addss		xmm0, xmm4
	addss		xmm3, xmm0
	movss		[rcx + 8], xmm3
	ret

;
;----------------
;VectorMatrixMultiply4_ASM
;----------------
;
;Win64
_VectorMatrixMultiply4_ASM:
	;rcx												;float *out
	;rdx												;float *vec
	;r8													;float *mat
	movaps		xmm0, [rdx]
	movaps		xmm1, [r8]
	movaps		xmm2, [r8 + 16]
	movaps		xmm3, [r8 + 32]
	movaps		xmm4, [r8 + 48]
	mulps		xmm1, xmm0
	mulps		xmm2, xmm0
	mulps		xmm3, xmm0
	mulps		xmm4, xmm0
	movhlps		xmm0, xmm1
	addps		xmm1, xmm0
	movaps		xmm5, xmm1
	shufps		xmm1, xmm1, 0x55
	addss		xmm1, xmm5
	movss		[rcx], xmm1
	movhlps		xmm0, xmm2
	addps		xmm2, xmm0
	movaps		xmm5, xmm2
	shufps		xmm2, xmm2, 0x55
	addss		xmm2, xmm5
	movss		[rcx + 4], xmm2
	movhlps		xmm0, xmm3
	addps		xmm3, xmm0
	movaps		xmm5, xmm3
	shufps		xmm3, xmm3, 0x55
	addss		xmm3, xmm5
	movss		[rcx + 8], xmm3
	movhlps		xmm0, xmm4
	addps		xmm4, xmm0
	movaps		xmm5, xmm4
	shufps		xmm4, xmm4, 0x55
	addss		xmm4, xmm5
	movss		[rcx + 12], xmm4
	ret

;
;----------------
;VectorMatrixMultiplyH_ASM
;----------------
;
;Win64
_VectorMatrixMultiplyH_ASM:
	;rcx												;float *out
	;rdx												;float *vec
	;r8													;float *mat
	movaps		xmm0, [rdx]
	andps		xmm0, [constant_mat_mask_ps]
	orps		xmm0, [constant_vec_mask_ps]
	movaps		xmm1, [r8]
	movaps		xmm2, [r8 + 16]
	movaps		xmm3, [r8 + 32]
	mulps		xmm1, xmm0
	mulps		xmm2, xmm0
	mulps		xmm3, xmm0
	movhlps		xmm0, xmm1
	addps		xmm1, xmm0
	movaps		xmm4, xmm1
	shufps		xmm1, xmm1, 0x55
	addss		xmm1, xmm4
	movss		[rcx], xmm1
	movhlps		xmm0, xmm2
	addps		xmm2, xmm0
	movaps		xmm4, xmm2
	shufps		xmm2, xmm2, 0x55
	addss		xmm2, xmm4
	movss		[rcx + 4], xmm2
	movhlps		xmm0, xmm3
	addps		xmm3, xmm0
	movaps		xmm4, xmm3
	shufps		xmm3, xmm3, 0x55
	addss		xmm3, xmm4
	movss		[rcx + 8], xmm3
	ret

;
;----------------
;MatrixCopy3x3_ASM
;----------------
;
;Win64
_MatrixCopy3x3_ASM:
	;rcx												;float *out
	;rdx												;float *mat
	movaps		xmm0, [rdx]
	movaps		xmm1, [rdx + 16]
	movaps		xmm2, [rdx + 32]
	movaps		[rcx], xmm0
	movaps		[rcx + 16], xmm1
	movaps		[rcx + 32], xmm2
	ret

;
;----------------
;MatrixCopy4x4_ASM
;----------------
;
;Win64
_MatrixCopy4x4_ASM:
	;rcx												;float *out
	;rdx												;float *mat
	movaps		xmm0, [rdx]
	movaps		xmm1, [rdx + 16]
	movaps		xmm2, [rdx + 32]
	movaps		xmm3, [rdx + 48]
	movaps		[rcx], xmm0
	movaps		[rcx + 16], xmm1
	movaps		[rcx + 32], xmm2
	movaps		[rcx + 48], xmm3
	ret

;
;----------------
;MatrixTranspose3x3_ASM
;----------------
;
;Win64
_MatrixTranspose3x3_ASM:
	;rcx												;float *out
	;rdx												;float *mat
	movaps		xmm3, [rdx + 32]
	movlps		xmm0, [rdx]
	movlps		xmm2, [rdx + 8]
	movhps		xmm0, [rdx + 16]
	movhps		xmm2, [rdx + 24]
	movaps		xmm1, xmm0
	shufps		xmm0, xmm3, 0xc8
	shufps		xmm1, xmm3, 0xdd
	shufps		xmm2, xmm3, 0xe8
	movaps		[rcx], xmm0
	movaps		[rcx + 16], xmm1
	movaps		[rcx + 32], xmm2
	ret

;
;----------------
;MatrixTranspose4x4_ASM
;----------------
;
;Win64
_MatrixTranspose4x4_ASM:
	;rcx												;float *out
	;rdx												;float *mat
	movlps		xmm1, [rdx]
	movlps		xmm3, [rdx + 8]
	movhps		xmm1, [rdx + 16]
	movhps		xmm3, [rdx + 24]
	movlps		xmm2, [rdx + 32]
	movlps		xmm4, [rdx + 40]
	movhps		xmm2, [rdx + 48]
	movhps		xmm4, [rdx + 56]
	movaps		xmm0, xmm1
	shufps		xmm0, xmm2, 0x88
	shufps		xmm1, xmm2, 0xdd
	movaps		xmm2, xmm3
	shufps		xmm2, xmm4, 0x88
	shufps		xmm3, xmm4, 0xdd
	movaps		[rcx], xmm0
	movaps		[rcx + 16], xmm1
	movaps		[rcx + 32], xmm2
	movaps		[rcx + 48], xmm3
	ret

;
;----------------
;MatrixMultiply3x3_ASM
;----------------
;
;Win64
_MatrixMultiply3x3_ASM:
	;rcx												;float *out
	;rdx												;float *mat1
	;r8													;float *mat2
	movaps		[rsp - 24], xmm6
	movss		xmm0, [rdx]
	movss		xmm1, [rdx + 4]
	movss		xmm2, [rdx + 8]
	shufps		xmm0, xmm0, 0x0
	shufps		xmm1, xmm1, 0x0
	shufps		xmm2, xmm2, 0x0
	movaps		xmm5, [r8]
	movaps		xmm6, [r8 + 16]
	movaps		xmm15, [r8 + 32]
	mulps		xmm0, xmm5
	mulps		xmm1, xmm6
	mulps		xmm2, xmm15
	addps		xmm0, xmm1
	addps		xmm0, xmm2
	movss		xmm1, [rdx + 16]
	movss		xmm2, [rdx + 20]
	movss		xmm3, [rdx + 24]
	shufps		xmm1, xmm1, 0x0
	shufps		xmm2, xmm2, 0x0
	shufps		xmm3, xmm3, 0x0
	mulps		xmm1, xmm5
	mulps		xmm2, xmm6
	mulps		xmm3, xmm15
	addps		xmm1, xmm2
	addps		xmm1, xmm3
	movss		xmm2, [rdx + 32]
	movss		xmm3, [rdx + 36]
	movss		xmm4, [rdx + 40]
	shufps		xmm2, xmm2, 0x0
	shufps		xmm3, xmm3, 0x0
	shufps		xmm4, xmm4, 0x0
	mulps		xmm2, xmm5
	mulps		xmm3, xmm6
	mulps		xmm4, xmm15
	addps		xmm2, xmm3
	addps		xmm2, xmm4
	movaps		[rcx], xmm0
	movaps		[rcx + 16], xmm1
	movaps		[rcx + 32], xmm2
	movaps		xmm6, [rsp - 24]
	ret

;
;----------------
;MatrixMultiply4x4_ASM
;----------------
;
;Win64
_MatrixMultiply4x4_ASM:
	;rcx												;float *out
	;rdx												;float *mat1
	;r8													;float *mat2
	movaps		[rsp - 24], xmm6
	movss		xmm0, [rdx]
	movss		xmm1, [rdx + 4]
	movss		xmm2, [rdx + 8]
	movss		xmm3, [rdx + 12]
	shufps		xmm0, xmm0, 0x0
	shufps		xmm1, xmm1, 0x0
	shufps		xmm2, xmm2, 0x0
	shufps		xmm3, xmm3, 0x0
	movaps		xmm4, [r8]
	movaps		xmm5, [r8 + 16]
	movaps		xmm6, [r8 + 32]
	movaps		xmm15, [r8 + 48]
	mulps		xmm0, xmm4
	mulps		xmm1, xmm5
	mulps		xmm2, xmm6
	mulps		xmm3, xmm15
	addps		xmm0, xmm1
	addps		xmm0, xmm2
	addps		xmm0, xmm3
	movaps		[rcx], xmm0
	movss		xmm0, [rdx + 16]
	movss		xmm1, [rdx + 20]
	movss		xmm2, [rdx + 24]
	movss		xmm3, [rdx + 28]
	shufps		xmm0, xmm0, 0x0
	shufps		xmm1, xmm1, 0x0
	shufps		xmm2, xmm2, 0x0
	shufps		xmm3, xmm3, 0x0
	mulps		xmm0, xmm4
	mulps		xmm1, xmm5
	mulps		xmm2, xmm6
	mulps		xmm3, xmm15
	addps		xmm0, xmm1
	addps		xmm0, xmm2
	addps		xmm0, xmm3
	movaps		[rcx + 16], xmm0
	movss		xmm0, [rdx + 32]
	movss		xmm1, [rdx + 36]
	movss		xmm2, [rdx + 40]
	movss		xmm3, [rdx + 44]
	shufps		xmm0, xmm0, 0x0
	shufps		xmm1, xmm1, 0x0
	shufps		xmm2, xmm2, 0x0
	shufps		xmm3, xmm3, 0x0
	mulps		xmm0, xmm4
	mulps		xmm1, xmm5
	mulps		xmm2, xmm6
	mulps		xmm3, xmm15
	addps		xmm0, xmm1
	addps		xmm0, xmm2
	addps		xmm0, xmm3
	movaps		[rcx + 32], xmm0
	movss		xmm0, [rdx + 48]
	movss		xmm1, [rdx + 52]
	movss		xmm2, [rdx + 56]
	movss		xmm3, [rdx + 60]
	shufps		xmm0, xmm0, 0x0
	shufps		xmm1, xmm1, 0x0
	shufps		xmm2, xmm2, 0x0
	shufps		xmm3, xmm3, 0x0
	mulps		xmm0, xmm4
	mulps		xmm1, xmm5
	mulps		xmm2, xmm6
	mulps		xmm3, xmm15
	addps		xmm0, xmm1
	addps		xmm0, xmm2
	addps		xmm0, xmm3
	movaps		[rcx + 48], xmm0
	movaps		xmm6, [rsp - 24]
	ret

;
;----------------
;MatrixLoadIdentity_ASM
;----------------
;
;Win64 Linux64
_MatrixLoadIdentity_ASM:
	mov			rax, qword [_gmat]
	movaps		xmm0, [constant_ident_1_ps]
	movaps		xmm1, [constant_ident_2_ps]
	movaps		xmm2, [constant_ident_3_ps]
	movaps		xmm3, [constant_ident_4_ps]
	movaps		[rax], xmm0
	movaps		[rax + 16], xmm1
	movaps		[rax + 32], xmm2
	movaps		[rax + 48], xmm3
	ret

;
;----------------
;MatrixTranslate_ASM
;----------------
;
;Win64
_MatrixTranslate_ASM:
	;rcx												;float *vpos
	mov			rax, qword [_gmat]
	movss		xmm0, [rax + 12]
	movss		xmm2, [rax + 28]
	movss		xmm4, [rax + 44]
	movss		xmm1, [rcx]
	movss		xmm3, [rcx + 4]
	movss		xmm5, [rcx + 8]
	addss		xmm0, xmm1
	addss		xmm2, xmm3
	addss		xmm4, xmm5
	movss		[rax + 12], xmm0
	movss		[rax + 28], xmm2
	movss		[rax + 44], xmm4
	ret

;
;----------------
;MatrixScale_ASM
;----------------
;
;Win64
_MatrixScale_ASM:
	;rcx												;float *sfac
	mov			rax, qword [_gmat]
	movss		xmm0, [rax]
	movss		xmm2, [rax + 20]
	movss		xmm4, [rax + 40]
	movss		xmm1, [rcx]
	movss		xmm3, [rcx + 4]
	movss		xmm5, [rcx + 8]
	mulss		xmm0, xmm1
	mulss		xmm2, xmm3
	mulss		xmm4, xmm5
	movss		[rax], xmm0
	movss		[rax + 20], xmm2
	movss		[rax + 40], xmm4
	ret

;
;----------------
;MatrixRotate_ASM
;----------------
;
;Win64
_MatrixRotate_ASM:
	;rcx												;float *angles
	movaps		xmm1, [rcx]								;parallel interleaved sines and cosines
	movaps		[rsp - 24], xmm6
	movaps		xmm0, xmm1
	movaps		xmm2, xmm1
	addps		xmm0, [constant_90_ps]
	movaps		xmm3, xmm1
	movaps		xmm5, xmm0
	movaps		xmm6, xmm0
	mulps		xmm2, [constant_inv_360_ps]
	mulps		xmm5, [constant_inv_360_ps]
	cmpltps		xmm3, [constant_zero_ps]
	cmpltps		xmm6, [constant_zero_ps]
	cvttps2dq	xmm4, xmm2								;x = floor(a / 360) * 360
	cvttps2dq	xmm15, xmm5
	andps		xmm3, [constant_one_ps]					;a < 0 ? 1 : 0
	andps		xmm6, [constant_one_ps]
	cvtdq2ps	xmm2, xmm4
	cvtdq2ps	xmm5, xmm15
	subps		xmm2, xmm3								;if a < 0, x -= 1
	subps		xmm5, xmm6
	mulps		xmm2, [constant_360_ps]
	mulps		xmm5, [constant_360_ps]
	subps		xmm1, xmm2
	subps		xmm0, xmm5
	movaps		xmm4, [constant_180_ps]
	movaps		xmm15, [constant_180_ps]
	subps		xmm4, xmm1
	subps		xmm15, xmm0
	movaps		xmm1, xmm4
	movaps		xmm2, xmm4
	movaps		xmm0, xmm15
	movaps		xmm5, xmm15
	andps		xmm1, [constant_signmsk_ps]
	andps		xmm0, [constant_signmsk_ps]
	xorps		xmm2, xmm1								;fabs(180 - a)
	xorps		xmm5, xmm0
	cmpnltps	xmm2, [constant_90_ps]
	cmpnltps	xmm5, [constant_90_ps]
	movaps		xmm3, [constant_180_ps]
	movaps		xmm6, [constant_180_ps]
	xorps		xmm3, xmm1								;if a < 0, -180
	xorps		xmm6, xmm0
	andps		xmm3, xmm2
	andps		xmm6, xmm5
	andps		xmm2, [constant_signmsk_ps]
	andps		xmm5, [constant_signmsk_ps]
	xorps		xmm4, xmm2
	xorps		xmm15, xmm5
	addps		xmm4, xmm3
	addps		xmm15, xmm6

	movaps		xmm1, [aconvert_ps]
	mulps		xmm4, xmm1
	mulps		xmm15, xmm1
	movaps		xmm2, xmm4
	movaps		xmm5, xmm15
	mulps		xmm4, xmm4
	mulps		xmm15, xmm15
	movaps		xmm1, [sine_constant4_ps]
	movaps		xmm0, [sine_constant4_ps]
	mulps		xmm1, xmm4
	mulps		xmm0, xmm15
	addps		xmm1, [sine_constant5_ps]
	addps		xmm0, [sine_constant5_ps]
	mulps		xmm1, xmm4
	mulps		xmm0, xmm15
	addps		xmm1, [constant_one_ps]
	addps		xmm0, [constant_one_ps]
	mulps		xmm1, xmm2								;sin(x, y, z)
	mulps		xmm0, xmm5								;cos(x, y, z)

	movaps		[rsp - 40], xmm1
	movaps		[rsp - 56], xmm0
;[cos(y)cos(r) ------------------ -cos(y)sin(r) ----------------- sin(y) --------------------- 0]
	movss		xmm2, [rsp - 56 + 4]
	movss		xmm0, [rsp - 56 + 4]
	movss		xmm1, [rsp - 56 + 8]
	xorps		xmm0, [constant_signmsk_ps]				;-cos(y)
	movss		xmm3, [rsp - 40 + 8]
	movss		xmm4, [rsp - 40 + 4]
	mulss		xmm1, xmm2								;cos(y)cos(r)
	mulss		xmm0, xmm3								;-cos(y)sin(r)
	movss		[rsp - 72 + 8], xmm4
	movss		[rsp - 72], xmm1
	movss		[rsp - 72 + 4], xmm0
;[cos(y)cos(r) ------------------ -cos(y)sin(r) ----------------- sin(y) --------------------- 0]
;[cos(r)sin(p)sin(y)+cos(p)sin(r) cos(p)cos(r)-sin(p)sin(y)sin(r) -cos(y)sin(p) -------------- 0]
	movss		xmm4, [rsp - 56]
	movss		xmm6, [rsp - 56 + 4]
	movss		xmm5, [rsp - 56 + 8]
	movss		xmm2, [rsp - 40]
	movss		xmm1, [rsp - 40 + 4]
	movss		xmm0, [rsp - 40 + 8]
	movss		xmm3, [rsp - 40 + 8]
	xorps		xmm6, [constant_signmsk_ps]				;-cos(y)
	mulss		xmm1, xmm2								;sin(p)sin(y)
	mulss		xmm0, xmm4								;cos(p)sin(r)
	mulss		xmm4, xmm5								;cos(p)cos(r)
	movss		xmm15, xmm1
	mulss		xmm15, xmm5								;cos(r)sin(p)sin(y)
	mulss		xmm1, xmm3								;sin(p)sin(y)sin(r)
	mulss		xmm2, xmm6								;-cos(y)sin(p)
	addss		xmm0, xmm15								;cos(r)sin(p)sin(y)+cos(p)sin(r)
	subss		xmm4, xmm1								;cos(p)cos(r)-sin(p)sin(y)sin(r)
	movss		[rsp - 88 + 8], xmm2
	movss		[rsp - 88], xmm0
	movss		[rsp - 88 + 4], xmm4
;[cos(y)cos(r) ------------------ -cos(y)sin(r) ----------------- sin(y) --------------------- 0]
;[cos(r)sin(p)sin(y)+cos(p)sin(r) cos(p)cos(r)-sin(p)sin(y)sin(r) -cos(y)sin(p) -------------- 0]
;[sin(p)sin(r)-cos(p)cos(r)sin(y) cos(p)sin(y)sin(r)+cos(r)sin(p) cos(p)cos(y) --------------- 0]
;[0 ----------------------------- 0 ----------------------------- 0 -------------------------- 1]
	movss		xmm5, [rsp - 56]
	movss		xmm6, [rsp - 56 + 4]
	movss		xmm4, [rsp - 56 + 8]
	movss		xmm2, [rsp - 40]
	movss		xmm1, [rsp - 40 + 4]
	movss		xmm0, [rsp - 40 + 8]
	movss		xmm3, [rsp - 40 + 8]
	mulss		xmm1, xmm5								;cos(p)sin(y)
	mulss		xmm0, xmm2								;sin(p)sin(r)
	mulss		xmm2, xmm4								;cos(r)sin(p)
	movss		xmm15, xmm1
	mulss		xmm15, xmm4								;cos(p)cos(r)sin(y)
	mulss		xmm1, xmm3								;cos(p)sin(y)sin(r)
	mulss		xmm5, xmm6								;cos(p)cos(y)
	subss		xmm0, xmm15								;sin(p)sin(r)-cos(p)cos(r)sin(y)
	addss		xmm2, xmm1								;cos(p)sin(y)sin(r)+cos(r)sin(p)
	movss		[rsp - 104 + 8], xmm5
	movss		[rsp - 104], xmm0
	movss		[rsp - 104 + 4], xmm2

	mov			rax, qword [_gmat]
	movss		xmm0, [rsp - 72]
	movss		xmm1, [rsp - 72 + 4]
	movss		xmm2, [rsp - 72 + 8]
	shufps		xmm0, xmm0, 0x0
	shufps		xmm1, xmm1, 0x0
	shufps		xmm2, xmm2, 0x0
	movaps		xmm3, [rax]
	movaps		xmm4, [rax + 16]
	movaps		xmm5, [rax + 32]
	mulps		xmm0, xmm3
	mulps		xmm1, xmm4
	mulps		xmm2, xmm5
	addps		xmm0, xmm1
	addps		xmm0, xmm2
	movaps		[rax], xmm0
	movss		xmm0, [rsp - 88]
	movss		xmm1, [rsp - 88 + 4]
	movss		xmm2, [rsp - 88 + 8]
	shufps		xmm0, xmm0, 0x0
	shufps		xmm1, xmm1, 0x0
	shufps		xmm2, xmm2, 0x0
	mulps		xmm0, xmm3
	mulps		xmm1, xmm4
	mulps		xmm2, xmm5
	addps		xmm0, xmm1
	addps		xmm0, xmm2
	movaps		[rax + 16], xmm0
	movss		xmm0, [rsp - 104]
	movss		xmm1, [rsp - 104 + 4]
	movss		xmm2, [rsp - 104 + 8]
	shufps		xmm0, xmm0, 0x0
	shufps		xmm1, xmm1, 0x0
	shufps		xmm2, xmm2, 0x0
	mulps		xmm0, xmm3
	mulps		xmm1, xmm4
	mulps		xmm2, xmm5
	addps		xmm0, xmm1
	addps		xmm0, xmm2
	movaps		[rax + 32], xmm0
	movaps		xmm6, [rsp - 24]
	ret

	section .data data align=16

align 16
constant_one_pd			dq		1.0, 1.0
constant_signmsk_ps		dd		0x80000000, 0x80000000, 0x80000000, 0x80000000
constant_one_ps			dd		1.0, 1.0, 1.0, 1.0
constant_zero_ps		dd		0.0, 0.0, 0.0, 0.0
constant_360_ps			dd		360.0, 360.0, 360.0, 360.0
constant_180_ps			dd		180.0, 180.0, 180.0, 180.0
constant_90_ps			dd		90.0, 90.0, 90.0, 90.0
constant_inv_360_ps		dd		2.77777777777777777e-3, 2.77777777777777777e-3, 2.77777777777777777e-3, 2.77777777777777777e-3
constant_inv_180_ps		dd		5.55555555555555555e-3, 5.55555555555555555e-3, 5.55555555555555555e-3, 5.55555555555555555e-3
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
