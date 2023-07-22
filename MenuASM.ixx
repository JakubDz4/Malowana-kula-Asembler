module;
#include <SDL.h>
#include "ASMLibrary.h"
export module M_MenuASM;
import M_Ball;
import M_Block;
import <iostream>;//delete
import <vector>;

export class MenuASM
{
public:
	SDL_Event e;
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	std::vector<Ball> balls;
	std::vector<Block> blocks;

	MenuASM(std::vector<Ball>& ba, std::vector<Block>& bl, SDL_Window* _window, SDL_Renderer* _renderer)
	{
		init(ba, bl, _window, _renderer);
	}

	~MenuASM()
	{
		clear();
	}

	//delete wisualizuje
	void collision_Point_test(Ball& ball, double Wx, double Wy, double W2x, double W2y)
	{
		//Wektor A i B
		double Ax = ball.pos.x - Wx;
		double Ay = ball.pos.y - Wy;
		double Bx = W2x - Wx;
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
		if (ball.rad * ball.rad >= (Cx - ball.pos.x) * (Cx - ball.pos.x) + (Cy - ball.pos.y) * (Cy - ball.pos.y))
		{
			SDL_SetRenderDrawColor(renderer, 0xff, 0, 0, 0x00);
			for (int w = 0; w < 4 * 2; w++)
			{
				for (int h = 0; h < 4 * 2; h++)
				{
					int dx = 4 - w; // horizontal offset
					int dy = 4 - h; // vertical offset
					if ((dx * dx + dy * dy) <= (4 * 4))
					{
						SDL_RenderDrawPoint(renderer, Cx + dx, Cy + dy);
					}
				}
			}
		}
		else
		{
			SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0, 0x00);
			for (int w = 0; w < 10 * 2; w++)
			{
				for (int h = 0; h < 10 * 2; h++)
				{
					int dx = 10 - w; // horizontal offset
					int dy = 10 - h; // vertical offset
					if ((dx * dx + dy * dy) <= (10 * 10))
					{
						SDL_RenderDrawPoint(renderer, Cx + dx, Cy + dy);
					}
				}
			}
		}


	}

	bool init(std::vector<Ball>& ba, std::vector<Block>& bl, SDL_Window* _window, SDL_Renderer* _renderer)
	{
		balls = ba;
		blocks = bl;

		window = _window;
		if (window == NULL)
		{
			return false;
		}

		renderer = _renderer;
		if (renderer == NULL)
		{
			return false;
		}

		return true;
	}

	inline void render()
	{
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
		SDL_RenderClear(renderer);
		for (int i = 0; i < balls.size(); i++)
		{
			balls[i].show(renderer);
		}
		for (int i = 0; i < blocks.size(); i++)
		{
			blocks[i].show(renderer);
		}

		SDL_RenderPresent(renderer);
	}

	void main_loop(size_t frame_num = 1000000)
	{
		bool col;
		int x = 0, y = 0;
		while (frame_num--)
		{
			SDL_PollEvent(&e);
			if (e.type == SDL_QUIT)
			{
				break;
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				SDL_GetMouseState(&x, &y);
				balls[0].dest.y = y;
				balls[0].dest.x = x;
			}
			double fin_cx, fin_cy;
			for (int i = 0; i < balls.size(); i++)
			{
				for (int j = 0; j < blocks.size(); j++)
				{
					double cx, cy;
					int k2;
					bool Iscollision = false;
					for (int k = 0; k < 4; k++)
					{
						k2 = k + 1;
						if (k2 == 4)
							k2 = 0;

						col = collision(balls[i].dest.x, balls[i].dest.y, blocks[j].wall[k].x, blocks[j].wall[k].y, blocks[j].wall[k2].x, blocks[j].wall[k2].y, cx, cy, balls[i].rad);

						if (col)
						{
							if (!Iscollision)
							{
								fin_cx = cx; fin_cy = cy;
							}
							else
							{
								if (abs(balls[i].dest.x - cx) < abs(balls[i].dest.x - fin_cx))
									fin_cx = cx;
								if (abs(balls[i].dest.y - cy) < abs(balls[i].dest.y - fin_cy))
									fin_cy = cy;
							}
							Iscollision = true;
						}
					}

					if (Iscollision)
					{
						double len = collision_reaction_back(balls[i].dest.x, balls[i].dest.y, fin_cx, fin_cy, balls[i].rad);
						double backx = balls[i].dir.x * len;
						double backy = balls[i].dir.y * len;

						double alfa = atan2(fin_cx - balls[i].dest.x, fin_cy - balls[i].dest.y) - atan2(balls[i].dir.x, balls[i].dir.y);

						balls[i].dest.x -= backx;
						balls[i].dest.y -= backy;

						new_dir(balls[i].dir.x, balls[i].dir.y, alfa);

						double vel_left = balls[i].vel - len;
						balls[i].dest.x += vel_left * balls[i].dir.x;
						balls[i].dest.y += vel_left * balls[i].dir.y;

						blocks[j].paint(balls[i].color, balls[i].rad, fin_cx, fin_cy);
					}
				}
				balls[i].move();
			}
			render();
		}
	}

	void clear()
	{
		/*SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		window = NULL;
		renderer = NULL;*/

		//SDL_Quit();
	}
};