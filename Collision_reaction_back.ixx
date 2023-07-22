export module M_Collision_reaction;
import <iostream>;

export double collision_reaction_back(double destx, double desty, double Cx, double Cy, size_t r)//return proper ball dest and dir
{
	double tmpx = Cx - destx;
	double tmpy = Cy - desty;

	double V = r - sqrt(tmpx * tmpx + tmpy * tmpy);

	return V;
}

export void new_dir(double &dirx, double &diry, double &alfa)
{
	double x = dirx;
	double y = diry;
	double a = 1.57079632679 - alfa;
	alfa = 2 * a;
	dirx = x * cos(alfa) - y * sin(alfa);
	diry = y * cos(alfa) + x * sin(alfa);
}