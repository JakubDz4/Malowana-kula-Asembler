module;
#include <SDL.h>
export module M_Vec2;

export struct Vec2d
{
	double x, y;
	Vec2d(double _x = 0, double _y = 0)
	{
		x = _x;
		y = _y;
	}
};

export struct Color
{
	uint8_t r, g, b, a;
	void init(uint8_t _r , uint8_t _g, uint8_t _b, uint8_t _a)
	{
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}
};

export struct Pixel
{
	Color color;
	int x, y;
	void init(int _x, int _y, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	{
		color.init(r, g, b, a);
		x = _x;
		y = _y;
	}


};