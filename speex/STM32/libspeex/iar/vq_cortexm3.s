/* Copyright (C) 2008 STMicroelectronics, MCD */
/*

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:
   
   - Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
   
   - Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
   
   - Neither the name of the Xiph.org Foundation nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.
   
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
  
  SECTION .text:CODE(2)
  
  EXPORT vq_nbest

#define t          r0
#define resp2      r1
#define E          r2

#define dist       r3

#define E_k        r4

#define t_0        r4
#define t_1        r5
#define t_2        r4
#define t_3        r5
#define t_4        r4
#define t_5        r5
#define t_6        r4
#define t_7        r5
#define t_8        r4
#define t_9        r5

#define codebook_1   r7
#define codebook_2   r8

#define best_index r10
#define best_dist  lr

#define k          r6





vq_nbest

	PUSH   {r4-r10,lr}
	MOV    k, #0x00 ; 32
	LDR    E, [sp, #32]
Loop
		
	LDRSH  codebook_1, [resp2], #2
	LDRSH  codebook_2, [resp2], #2
	LDRSH  t_0, [t, #0x0]
	LDRSH  t_1, [t, #0x2]	
	MUL    dist, t_0, codebook_1	
        MLA    dist, t_1, codebook_2, dist

	LDRSH  codebook_1, [resp2], #2
	LDRSH  t_2, [t, #0x4]
	LDRSH  codebook_2, [resp2], #2
	LDRSH  t_3, [t, #0x6]    
	MLA    dist, t_2, codebook_1, dist
	MLA    dist, t_3, codebook_2, dist

	LDRSH  codebook_1, [resp2], #2
	LDRSH  t_4, [t, #0x8]   
	LDRSH  codebook_2, [resp2], #2
	LDRSH  t_5, [t, #0xA]
        MLA    dist, t_4, codebook_1, dist
	MLA    dist, t_5, codebook_2, dist

	LDRSH  codebook_1, [resp2], #2
	LDRSH  t_6, [t, #0xC]
        LDRSH  codebook_2, [resp2], #2
	LDRSH  t_7, [t, #0xE]
	MLA    dist, t_6, codebook_1, dist
        MLA    dist, t_7, codebook_2, dist

	LDRSH  codebook_1, [resp2], #2
	LDRSH  t_8, [t, #0x10]    
	LDRSH  codebook_2, [resp2], #2
	LDRSH  t_9, [t, #0x12]
        MLA    dist, t_8, codebook_1, dist
	MLA    dist, t_9, codebook_2, dist
	
	LDR    E_k, [E], #4
	RSB    dist, dist, E_k, ASR #1

	CMP      k,#0x01
	ITEE     GE
	CMPGE    dist, best_dist
	MOVLT    best_dist, dist
	MOVLT    best_index, k
	 
        ADDS  k, k, #1
	CMP   k, #0x20
	BLT   Loop

        LDR   dist, [sp,#40]
	STR   best_index, [dist]
	LDR   dist, [sp,#44]
	STR   best_dist, [dist]

	POP   {r4-r10,pc}

   END