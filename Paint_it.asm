.data
num real8 255.0
one real8 1.0
.code
paint_valAsm proc 

;export char paint_val(double ballc, double balla, double blockc, double blocka)

;	double sc = ballc / 255;//source
;	double sa = balla / 255;
;	double dc = blockc / 255;//destination
;	double da = blocka / 255;

	divsd xmm0, num
	divsd xmm1, num
	divsd xmm2, num
	divsd xmm3, num

	movsd xmm4, one
	subsd xmm4, xmm1 ; (1 - sa)
	mulsd xmm0, xmm1 ; sc * sa
	mulsd xmm2, xmm3 ; dc * da
	mulsd xmm2, xmm4 ; dc * da * (1 - sa)
	addsd xmm0, xmm2
	

;	double color = sc * sa + dc * da * (1 - sa);
;	return color * 255;

mulsd xmm0, num
cvtsd2si rax, xmm0
ret
paint_valAsm endp
end