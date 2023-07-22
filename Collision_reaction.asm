.data
negative real8 -1.0
PI real8 3.14159265358979323846
.code
collision_reaction_backAsm proc
;export double collision_reaction_back(double destx, double desty, double Cx, double Cy, size_t r)//return proper ball dest and dir

;	double tmpx = Cx - destx;
	subsd xmm2, xmm0
	mulsd xmm2, xmm2 ; tmpx * tmpx 

;	double tmpy = Cy - desty;
	subsd xmm3, xmm1
	mulsd xmm3, xmm3 ; tmpy * tmpy

;	double V = r - sqrt(tmpx * tmpx + tmpy * tmpy);
	addsd xmm2, xmm3
	sqrtsd xmm2, xmm2 ; sqrt(tmpx * tmpx + tmpy * tmpy);

	mov rax, qword ptr[rsp+40] 
	cvtsi2sd xmm0, rax ; xmm0 = r

	subsd xmm0, xmm2

ret
collision_reaction_backAsm endp

calc_cosalfaAsm proc
;export double calc_cosalfa(double cx, double cy, double dx, double dy, double dirx, double diry)

;	double Ax = cx-dx;//A wektor od pozycji do punktu c
;	double Ay = dy-cy;
;	double Bx = dirx;//B wektor kierunku
;	double By = -diry;

	subsd xmm0, xmm2 ; Ax
	subsd xmm3, xmm1 ; Ay
	movsd xmm1, real8 ptr[rsp+40]  ; Bx
	movsd xmm2, real8 ptr[rsp+48] 
	mulsd xmm2, negative ;By

;	double skalar = Ax * Bx + Ay * By;
;	double lenA = sqrt(Ax * Ax + Ay * Ay);
;	double lenB = sqrt(Bx * Bx + By * By);

	movsd xmm4, xmm0
	mulsd xmm4, xmm1 ; Ax * Bx
	movsd xmm5, xmm3
	mulsd xmm5, xmm2 ; Ay * By
	addsd xmm4, xmm5 ; skalar

	mulsd xmm0, xmm0
	mulsd xmm3, xmm3
	addsd xmm0, xmm3
	sqrtsd xmm0, xmm0 ; sqrt(Ax * Ax + Ay * Ay);

	mulsd xmm1, xmm1
	mulsd xmm2, xmm2
	addsd xmm1, xmm2
	sqrtsd xmm1, xmm1 ; sqrt(Bx * Bx + By * By);

	mulsd xmm1, xmm0 ; (lenA*lenB);
	movsd xmm0, xmm4 ; xmm0 = skalar

	divsd xmm0, xmm1


;	return skalar/(lenA*lenB);
ret
calc_cosalfaAsm endp

new_dirAsm proc
;export void new_dir(double &dirx, double &diry, double &alfa)

	;movsd xmm0, real8 ptr [rcx] 
	;addsd xmm0, num
	;movsd real8 ptr [rcx], xmm0

;	double x = dirx;
;	double y = diry;
;	double a = 1.57079632679 - alfa;	
;	alfa = 2 * a;
;	dirx = x * cos(alfa) - y * sin(alfa);
;	diry = y * cos(alfa) + x * sin(alfa);

	movsd xmm0, PI
	movsd xmm1, real8 ptr [r8] 
	subsd xmm0, xmm1
	addsd xmm0, xmm0 ;alfa
	movsd real8 ptr [r8], xmm0 ; save to alfa in manu
	fsin 

ret
new_dirAsm endp


end