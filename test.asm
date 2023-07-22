;.data
;radius qword 18446744073709551614
;finx real8 11.0
;rax is an register, which c== reads as a return value
; In Windows, the first four integer parameters are passed in registers RCX, RDX, R8, R9 and floating point in XMM0, XMM1, XMM2, XMM3, 
;anything more than four parameters are passed on the stack above the shadow space.
;if first is double then first int is in rdx ; (
;double 5s parameter: mov ebx, DWORD PTR[rbp + 48] ; e
;Shadow space = 32 + saved rbp = 40 + 5th parameter = 48
;
;wyluskanie i powiekszenie int: foo(int&);
;.code
;foo proc
;	mov eax, DWORD ptr [rcx] 
;	inc rax
;	ret
;foo endp
;end
;
;return 5 int
;.code
;foo proc
;MOV RAX, QWORD PTR [RSP+40]
;ret
;foo endp
;end
;
;return 5 double
;.code
;foo proc
;movq xmm0, real8 ptr[rsp+40]
;ret
;foo endp
;end
;
;multiply double
;.code
;foo proc
;	mulpd xmm0, xmm0
;ret
;foo endp
;end
;
;go to hehe if xmm1 > xmm0
;
;wyluskanie i dodanie do 7 argumentu double
;double __stdcall foo(double a, double b, double c, double d, double e, double f, double &g, double h, size_t r);
;
;	mov rax, qword ptr[rsp+56]
;	movsd xmm7, real8 ptr [rax]
;	addsd xmm7, num
;	movsd real8 ptr [rax], xmm7
;
;foo(double&);
.data
num real8 3.3
.code
foo proc 
	fsin
ret
foo endp
end