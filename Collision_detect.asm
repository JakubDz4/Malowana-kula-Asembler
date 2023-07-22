.data
proj real8 0.0
.code
collisionAsm proc
	;double Ax = dx - Wx;//A wektor od wierzcholka sciany do potencjalnej pozycji kuli
	movsd xmm4, xmm0; 
	subsd xmm4, xmm2
	;double Ay = dy - Wy;
	movsd xmm5, xmm1
	subsd xmm5, xmm3
	;double Bx = W2x - Wx;//B wektor od 1 wierzcholka do 2 wierzcholka sciany
	movsd xmm6, real8 ptr[rsp+40]
	subsd xmm6, xmm2
	;double By = W2y - Wy;
	movsd xmm7, real8 ptr[rsp+48]
	subsd xmm7, xmm3

	;dx = xmm0, dy = xmm1, Wx = xmm2, Wy = xmm3
	;Ax = xmm4, Ay = xmm5, Bx = xmm6, By = xmm7

	;//projection A on B
	;double proj = (Ax * Bx + Ay * By) / (Bx * Bx + By * By);
	mulsd xmm4, xmm6 ; Ax * Bx
	mulsd xmm5, xmm7 ; Ay * By
	addsd xmm4, xmm5 ; Ax * Bx + Ay * By
	movsd proj, xmm4 
	;xmm5 i xmm4 free

	movsd xmm5, xmm6
	movsd xmm4, xmm7
	mulsd xmm5, xmm5 ; Bx * Bx
	mulsd xmm4, xmm4 ; By * By
	addsd xmm4, xmm5 ; Bx * Bx + By * By
	movsd xmm5, proj
	divsd xmm5, xmm4
	movsd xmm4, xmm5

	;dx = xmm0, dy = xmm1, Wx = xmm2, Wy = xmm3
	;proj = xmm4, proj = xmm5, Bx = xmm6, By = xmm7

	;//POint closest to ball
	;double Cx = proj * Bx + Wx;
	;double Cy = proj * By + Wy;
	mulsd xmm4, xmm6
	addsd xmm4, xmm2 ; Cx
	mulsd xmm5, xmm7
	addsd xmm5, xmm3 ; Cy

	;dx = xmm0, dy = xmm1, Wx = xmm2, Wy = xmm3
	;Cx = xmm4, Cy = xmm5, free = xmm6, free = xmm7

	;//make sure taht C point is on line
	;if (Cx >= std::max(Wx, W2x))
	;{
	;	Cx = std::max(Wx, W2x);
	;}
	;else if (Cx <= std::min(Wx, W2x))
	;{
	;	Cx = std::min(Wx, W2x);
	;}
	movsd xmm6, xmm2
	maxsd xmm6, real8 ptr[rsp+40] ; std::max(Wx, W2x)
	movsd xmm7, xmm2
	minsd xmm7, real8 ptr[rsp+40] ; std::min(Wx, W2x)

	;go to etykieta if arg 1 < arg 2
	comisd xmm6, xmm4 ; if(std::max(Wx, W2x) < Cx)
	jc MaxWxSmallerCx
	comisd xmm4, xmm7 ; Cx < std::min(Wx, W2x)
	jc CxSmallerMinWx
	jmp NextIfs

	MaxWxSmallerCx:
	movsd xmm4, xmm6; Cx = std::max(Wx, W2x);
	jmp NextIfs

	CxSmallerMinWx:
	movsd xmm4, xmm7; Cx = std::min(Wx, W2x);

	NextIfs:
	;if (Cy >= std::max(Wy, W2y))
	;{
	;	Cy = std::max(Wy, W2y);
	;}
	;else if (Cy <= std::min(Wy, W2y))
	;{
	;	Cy = std::min(Wy, W2y);
	;}
	;
	;free = xmm6, free = xmm7
	;
	movsd xmm6, xmm3
	maxsd xmm6, real8 ptr[rsp+48] ; std::max(Wy, W2y)
	movsd xmm7, xmm3
	minsd xmm7, real8 ptr[rsp+48] ; std::min(Wy, W2y);

	;go to etykieta if arg 1 < arg 2
	comisd xmm6, xmm5 ; if (std::max(Wy, W2y) < Cy)
	jc MaxWySmallerCy
	comisd xmm5, xmm7 ; Cy < std::min(Wy, W2y)
	jc CySmallerMinWy
	jmp FinalIf

	MaxWySmallerCy:
	movsd xmm5, xmm6; Cy = std::max(Wy, W2y);
	jmp FinalIf

	CySmallerMinWy:
	movsd xmm5, xmm7; Cy = std::min(Wy, W2y);

	;dx = xmm0, dy = xmm1, free = xmm2, free = xmm3
	;Cx = xmm4, Cy = xmm5, free = xmm6, free = xmm7
	FinalIf:

	;//Make sure that distance between pos and C is lower than radius 
	;if (r * r >= (Cx - dx) * (Cx - dx) + (Cy - dy) * (Cy - dy))
	;{
	;	finalx = Cx;
	;	finaly = Cy;
	;	return true;
	;}
	;else return false;	

	movsd xmm2, xmm4
	subsd xmm2, xmm0 ;Cx - dx
	mulsd xmm2, xmm2 ;(Cx - dx) * (Cx - dx)

	movsd xmm3, xmm5
	subsd xmm3, xmm1
	mulsd xmm3, xmm3 ;(Cy - dy) * (Cy - dy)

	addsd xmm2, xmm3 ;(Cx - dx) * (Cx - dx) + (Cy - dy) * (Cy - dy)

	mov rax, qword ptr[rsp+72] 
	mul rax ; r*r
	cvtsi2sd xmm0, rax ; xmm0 = r*r
	xor al, al ; rax = false

	ucomisd xmm2, xmm0
	jc rWiekRow
	jz rWiekRow

	jmp koniec

	rWiekRow:
	mov rcx, qword ptr[rsp+56]
	movsd real8 ptr [rcx], xmm4
	mov rcx, qword ptr[rsp+64]
	movsd real8 ptr [rcx], xmm5
	inc al ; rax = true

koniec:
ret
collisionAsm endp
end