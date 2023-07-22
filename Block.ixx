module;
#include <SDL.h>
#include "ASMLibrary.h"
export module M_Block;
import M_Vec2;
import <vector>;

export class Block
{
private:
	Color color = NULL;
	int x, y, w, h;
public:
	std::vector<Pixel> pixels;
	double angle;
	Vec2d wall[4];


	Block(int _x = 200, int _y = 400, int _w = 200, int _h = 75, double _angle = 0, uint8_t r = 0xff, uint8_t g = 0xff, uint8_t b = 0xff, uint8_t a = 0xff)
	{
		x = _x;
		y = _y;
		w = _w;
		h = _h;
		color.init(r, g, b, a);
		angle = _angle * M_PI / 180;

		double w2 = w / 2;
		double h2 = h / 2;

		wall[0].x = (-w2) * cos(angle) - (-h2) * sin(angle);//LewyGorny
		wall[0].y = (-h2) * cos(angle) + (-w2) * sin(angle);

		wall[1].x = (w2)*cos(angle) - (-h2) * sin(angle);//PrawyGorny
		wall[1].y = (-h2) * cos(angle) + (w2)*sin(angle);

		wall[2].x = (w2)*cos(angle) - (h2)*sin(angle);//PrawyDolny
		wall[2].y = (h2)*cos(angle) + (w2)*sin(angle);

		wall[3].x = (-w2) * cos(angle) - (h2)*sin(angle);//LewyDolny
		wall[3].y = (h2) * cos(angle) + (-w2) * sin(angle);

		wall[0].x += x; wall[0].y += y;
		wall[1].x += x; wall[1].y += y;
		wall[2].x += x; wall[2].y += y;
		wall[3].x += x; wall[3].y += y;

		pixels.resize(w * h);
		for (int i = 0; i < h; i++)
		{
			int iw = i * w;
			for (int j = 0; j < w; j++)
			{
				double px = (-w2 + j) * cos(angle) - (-h2 + i) * sin(angle) + x;
				double py = (-h2 + i) * cos(angle) + (-w2 + j) * sin(angle) + y;
				pixels[iw + j].init(px, py, r, g, b, a);
			}

		}
	}

	void show(SDL_Renderer* renderer)
	{
		SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0, color.a);
		/*SDL_RenderDrawLine(renderer, wall[0].x, wall[0].y, wall[1].x, wall[1].y);
		SDL_RenderDrawLine(renderer, wall[1].x, wall[1].y, wall[2].x, wall[2].y);
		SDL_RenderDrawLine(renderer, wall[2].x, wall[2].y, wall[3].x, wall[3].y);
		SDL_RenderDrawLine(renderer, wall[3].x, wall[3].y, wall[0].x, wall[0].y);*/

		for (int i = 0; i < pixels.size(); i++)
		{
			SDL_SetRenderDrawColor(renderer, pixels[i].color.r, pixels[i].color.g, pixels[i].color.b, pixels[i].color.a);
			SDL_RenderDrawPoint(renderer, pixels[i].x, pixels[i].y);
		}
	}

	void paint(Color ball_color, size_t r, double cx, double cy)
	{
		for (int i = 0; i < pixels.size(); i++)
		{
			double lenx = cx - pixels[i].x;
			double leny = cy - pixels[i].y;
			if (r * r >= lenx * lenx + leny * leny)
			{
				pixels[i].color.r = paint_val(static_cast<double>(ball_color.r), static_cast<double>(ball_color.a), static_cast<double>(pixels[i].color.r), static_cast<double>(pixels[i].color.a));
				pixels[i].color.g = paint_val(static_cast<double>(ball_color.g), static_cast<double>(ball_color.a), static_cast<double>(pixels[i].color.g), static_cast<double>(pixels[i].color.a));
				pixels[i].color.b = paint_val(static_cast<double>(ball_color.b), static_cast<double>(ball_color.a), static_cast<double>(pixels[i].color.b), static_cast<double>(pixels[i].color.a));
			}
		}
	}
};