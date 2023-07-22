export module M_Collision;
import <iostream>;

export bool collision(double dx, double dy, double Wx, double Wy, double W2x, double W2y, double &finalx, double &finaly, size_t r)//return collision point with dest/pos line and block 1 line
{
	//dx zamiast px bo chcemy zapobiegac

	//Wektor A i B
	double Ax = dx - Wx;//A wektor od wierzcholka sciany do potencjalnej pozycji kuli
	double Ay = dy - Wy;
	double Bx = W2x - Wx;//B wektor od 1 wierzcholka do 2 wierzcholka sciany
	double By = W2y - Wy;

	//projection A on B
	double proj = (Ax * Bx + Ay * By) / (Bx * Bx + By * By);
	//POint closest to ball
	double Cx = proj * Bx + Wx;
	double Cy = proj * By + Wy;

	//make sure taht C point is on line
	if (Cx >= std::max(Wx, W2x))
	{
		Cx = std::max(Wx, W2x);
	}
	else if (Cx <= std::min(Wx, W2x))
	{
		Cx = std::min(Wx, W2x);
	}
	if (Cy >= std::max(Wy, W2y))
	{
		Cy = std::max(Wy, W2y);
	}
	else if (Cy <= std::min(Wy, W2y))
	{
		Cy = std::min(Wy, W2y);
	}

	//Make sure that distance between pos and C is lower than radius 
	if (sqrt(r * r) >=sqrt( (Cx - dx) * (Cx - dx) + (Cy - dy) * (Cy - dy)))
	{
		finalx = Cx;
		finaly = Cy;
		return true;
	}
	else return false;

}