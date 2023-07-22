module;
#include <SDL.h>
export module M_Ball;
import M_Vec2;


export class Ball
{
private:
public:
	Color color;
	double vel;
	size_t rad;
	Vec2d dest, pos, dir;
	Ball(double x = 100, double y = 300, size_t _rad = 50, double _vel = 3, uint8_t r = 0xff, uint8_t g = 0x00, uint8_t b = 0x00, uint8_t a = 128,
		double dx = 1, double dy = 3)
	{
		rad = _rad;
		vel = _vel;
		color.init(r, g, b, a);
		pos.x = x; pos.y = y;
		dir.x = dx; dir.y = dy;
		dest.x = pos.x + dir.x * vel; dest.y = pos.y + dir.y * vel;

	}

	void show(SDL_Renderer* renderer)
	{
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
		for (int w = 0; w < rad * 2; w++)
		{
			for (int h = 0; h < rad * 2; h++)
			{
				int dx = rad - w; // horizontal offset
				int dy = rad - h; // vertical offset
				if ((dx * dx + dy * dy) <= (rad * rad))
				{
					SDL_RenderDrawPoint(renderer, pos.x + dx, pos.y + dy);
				}
			}
		}
	}

	void move()
	{
		pos = dest;
		dest.x = pos.x + dir.x * vel; 
		dest.y = pos.y + dir.y * vel;	
	}
};