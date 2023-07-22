#include <SDL.h>
#include <SDL_ttf.h>
import M_MenuCPP;
import M_MenuASM;
import M_Ball;
import M_Block;
import <iostream>;
import <iomanip>;
import <ctime>;
import <vector>;
import <string>;

//extern "C" double __stdcall foo(double);
//
//void foo2(double a, double b, double c, double d, double e, double f, double& g, double h, size_t r)
//{
//	g += 10;
//}

struct Option
{
	SDL_Texture* text;
	SDL_Rect rect;
	TTF_Font* font;
	SDL_Color color;
	SDL_Renderer* renderer;
	std::string stext;
	bool changeable;

	void init(int x, int y, std::string textureText, SDL_Renderer* _renderer, bool _changeable = false)
	{
		renderer = _renderer;
		changeable = _changeable;

		//Free texture if it exists
		if (text != NULL)
		{
			SDL_DestroyTexture(text);
			text = NULL;
		}

		font = TTF_OpenFont("D:\\Wlasne\\SDL2\\Ja_Ball\\ALGER.TTF", 30);
		color = { 255,255,255,255 };

		//Render text surface
		SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText.c_str(), color);
		if (textSurface == NULL)
		{
			printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
		}
		else
		{
			//Create texture from surface pixels
			text = SDL_CreateTextureFromSurface(renderer, textSurface);
			if (text == NULL)
			{
				printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
			}
			else
			{
				stext = textureText;
				rect.x = x;
				rect.y = y;
				rect.w = textSurface->w;
				rect.h = textSurface->h;
			}

			SDL_SetTextureColorMod(text, 255, 255, 0);

			//Get rid of old surface
			SDL_FreeSurface(textSurface);
		}
	}

	void clear()
	{
		if (text != NULL)
			SDL_DestroyTexture(text);
		TTF_CloseFont(font);
		font = NULL;
	}

	void show()
	{
		if (text != NULL)
			SDL_RenderCopy(renderer, text, NULL, &rect);
	}

	bool attach(int &x, int &y)
	{
		if (x <= rect.x + rect.w && x >= rect.x && y <= rect.y + rect.h && y >= rect.y)
			return true;
		return false;
	}
};

class Main_menu
{
	SDL_Event e;
	SDL_Window* window = NULL;
	SDL_Renderer* renderer;
	std::vector<Ball> balls;
	std::vector<Block> blocks;
	size_t frames = 1000, W = 1200, H = 800;
	clock_t start, stop;
	double czasA=0, czasC=0;
	int wybor;
	bool menu = true;
	std::vector<Option> option;
public:
	bool init()
	{

		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
			return false;
		}

		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{

		}

		if (TTF_Init() < 0)
		{
			return false;
		}

		window = SDL_CreateWindow("Ball", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, W, H, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			return false;
		}
		
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (renderer == NULL)
		{
			return false;
		}

		blocks.resize(4);
		balls.resize(1);

		return true;
	}

	void clear()
	{
		if(renderer!= NULL)
			SDL_DestroyRenderer(renderer);
		if (window != NULL)
			SDL_DestroyWindow(window);
		window = NULL;
		renderer = NULL;

		for (int i = 0; i < option.size(); i++)
			option[i].clear();

		SDL_Quit();
		TTF_Quit();
	}

	bool MenuSection()
	{
		int x, y, textid = -1;
		bool texting = false, running = true, saveOptions = false;
		while (running)
		{
			//event
			SDL_PollEvent(&e);
			if (e.type == SDL_QUIT)
			{
				menu = false;
				break;
			}
			else if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN)
			{
				SDL_GetMouseState(&x, &y);
				for (int i = 0; i < option.size(); i++)
				{
					if (option[i].attach(x, y))
					{
						if (i == option.size() - 1 || i == option.size() - 2 || (textid != i && option[i].changeable))
							SDL_SetTextureColorMod(option[i].text, 255, 0, 0);
						if (e.type == SDL_MOUSEBUTTONDOWN)
						{
							if (i == option.size() - 1)
							{
								running = false;
							}
							else if (i == option.size() - 2)
							{
								saveOptions = true;
								running = false;
							}
							else if (option[i].changeable)
							{
								if (texting && i == textid)
								{
									SDL_StopTextInput();
									texting = false;
									textid = -1;
								}
								else
								{
									option[i].stext = "";
									textid = i;
									SDL_StartTextInput();
									texting = true;
								}
							}
						}
					}
					else if (textid != i) SDL_SetTextureColorMod(option[i].text, 255, 255, 0);
				}
			}
			else if (e.type == SDL_KEYDOWN)
			{
				//Handle backspace
				if (e.key.keysym.sym == SDLK_BACKSPACE && option[textid].stext.length() > 0)
				{
					//lop off character
					option[textid].stext.pop_back();
					option[textid].init(option[textid].rect.x, option[textid].rect.y, option[textid].stext, renderer, option[textid].changeable);
				}
			}
			else if (e.type == SDL_TEXTINPUT)
			{
				option[textid].stext += e.text.text;
				option[textid].init(option[textid].rect.x, option[textid].rect.y, option[textid].stext, renderer, option[textid].changeable);
			}
			if (texting)
				SDL_SetTextureColorMod(option[textid].text, 0, 0, 255);
			//Clear screen
			SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
			SDL_RenderClear(renderer);

			for (int i = 0; i < option.size(); i++)
				option[i].show();

			//Update screen
			SDL_RenderPresent(renderer);

			SDL_Delay(16);
		}
		return saveOptions;
	}

	inline void SetOptions()
	{
		option.clear();
		option.resize(5);
		option[0].init(50, 50, "Start", renderer);
		option[1].init(50, 100, "Add Ball", renderer);
		option[2].init(50, 150, "Add Block", renderer);
		option[3].init(50, 200, "Options", renderer);
		option[4].init(50, 250, "Exit", renderer);
	}

	void MenuLoop()
	{
		int x, y;
		SetOptions();
		while (menu)
		{
			//event
			SDL_PollEvent(&e);
			if (e.type == SDL_QUIT)
			{
				break;
			}
			else if(e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN)
			{
				SDL_GetMouseState(&x, &y);
				for (int i = 0; i < option.size(); i++)
				{
					if (option[i].attach(x, y))
					{
						SDL_SetTextureColorMod(option[i].text, 255, 0, 0);
						if (e.type == SDL_MOUSEBUTTONDOWN)
						{
							switch (i)
							{
								case 0:
								{
									bool running = true;
									int nx, ny;
									option.clear();
									option.resize(5);

									option[2].init(50, 100, "Run Asembler Program:", renderer, true);

									option[0].init(50, 50, "Run C++ Program:", renderer, true);
									option[1].init(option[2].rect.w + 70, 50, "Time: "+std::to_string(czasC) + " [Seconds]", renderer);

									option[3].init(option[2].rect.w + 70, 100, "Time: " + std::to_string(czasA) + " [Seconds]", renderer);

									option[4].init(50, 150, "Return", renderer, true);

									//some hard created objects + 141 resize
									blocks[0] = Block(W / 2, 10, W, 20);
									blocks[1] = Block(10, H / 2, 20, H);
									blocks[2] = Block(W / 2, H - 10, W, 20);
									blocks[3] = Block(W - 10, H / 2, 20, H);

									//blocks[4] = Block(W / 2, H/2, W/3, H/15, 40);
									//blocks[5] = Block(W-W/4, H/3, W / 3, H / 15, 60);

									//balls[0] = Ball(W / 2, H / 2);
									//balls[1] = Ball(W / 3, H - H/5, 60, 2.4, 0, 255, 0, 156, 1, -2);
									//balls[2] = Ball(W - W/5, H - H/5, 54, 2.7, 0, 0, 255, 142, -1, 2);

									while (running)
									{
										SDL_PollEvent(&e);
										if (e.type == SDL_QUIT)
										{
											break;
										}
										else if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN)
										{
											SDL_GetMouseState(&nx, &ny);
											for (int i = 0; i < option.size(); i++)
											{
												if (option[i].attach(nx, ny))
												{
													if (option[i].changeable)
														SDL_SetTextureColorMod(option[i].text, 255, 0, 0);
													if (e.type == SDL_MOUSEBUTTONDOWN)
													{
														if (i == option.size() - 1)
														{
															running = false;
														}
														else if (i == 0)//c++
														{
															MenuCPP menu1(balls, blocks, window, renderer);
															start = clock();
															menu1.main_loop(frames);
															stop = clock();
															czasC = (double)(stop - start) / CLOCKS_PER_SEC;
															option[1].init(option[2].rect.w + 70, 50, "Time: " + std::to_string(czasC) + " [Seconds]", renderer);
															break;
														}
														else if (i == 2)//asm
														{
															MenuASM menu2(balls, blocks, window, renderer);
															start = clock();
															menu2.main_loop(frames);
															stop = clock();
															czasA = (double)(stop - start) / CLOCKS_PER_SEC;
															option[3].init(option[2].rect.w + 70, 100, "Time: " + std::to_string(czasA) + " [Seconds]", renderer);
															break;
														}
													}
												}
												else SDL_SetTextureColorMod(option[i].text, 255, 255, 0);
											}
										}
										//Clear screen
										SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
										SDL_RenderClear(renderer);

										for (int i = 0; i < option.size(); i++)
											option[i].show();

										//Update screen
										SDL_RenderPresent(renderer);

										SDL_Delay(16);
									}

									SetOptions();
									break;
								}
								case 1:
								{
									option.clear();
									option.resize(22);
									
									option[6].init(50, 200, "Ball Velocity:", renderer);

									option[0].init(50, 50, "Ball X:", renderer);
									option[1].init(option[6].rect.w + 70, 50, "100", renderer, true);

									option[2].init(50, 100, "Ball Y:", renderer);
									option[3].init(option[6].rect.w + 70, 100, "100", renderer, true);

									option[4].init(50, 150, "Ball Radius:", renderer);
									option[5].init(option[6].rect.w + 70, 150, "50", renderer, true);

									option[7].init(option[6].rect.w + 70, 200, "3", renderer, true);

									option[8].init(50, 250, "Ball Red:", renderer);
									option[9].init(option[6].rect.w + 70, 250, "255", renderer, true);

									option[10].init(50, 300, "Ball Green:", renderer);
									option[11].init(option[6].rect.w + 70, 300, "0", renderer, true);

									option[12].init(50, 350, "Ball Blue:", renderer);
									option[13].init(option[6].rect.w + 70, 350, "0", renderer, true);

									option[14].init(50, 400, "Ball Alpha:", renderer);
									option[15].init(option[6].rect.w + 70, 400, "255", renderer, true);

									option[16].init(50, 450, "Direction X:", renderer);
									option[17].init(option[6].rect.w + 70, 450, "1", renderer, true);

									option[18].init(50, 500, "Direction Y:", renderer);
									option[19].init(option[6].rect.w + 70, 500, "1", renderer, true);

									option[20].init(50, 550, "Add Ball", renderer);
									option[21].init(50, 600, "Return", renderer);

									if(MenuSection())
									balls.push_back(Ball(std::stoi(option[1].stext), std::stoi(option[3].stext), std::stoi(option[5].stext), std::stoi(option[7].stext)
										, std::stoi(option[9].stext), std::stoi(option[11].stext), std::stoi(option[13].stext), std::stoi(option[15].stext),
										std::stoi(option[17].stext), std::stoi(option[19].stext)));

									SetOptions();
									break;
								}
								case 2:
								{
									option.clear();
									option.resize(20);
									option[12].init(50, 350, "Block Green:", renderer);

									option[0].init(50, 50, "Block X:", renderer);
									option[1].init(option[12].rect.w + 70, 50, "0", renderer, true);

									option[2].init(50, 100, "Block Y:", renderer);
									option[3].init(option[12].rect.w + 70, 100, "0", renderer, true);

									option[4].init(50, 150, "Block W:", renderer);
									option[5].init(option[12].rect.w + 70, 150, "200", renderer, true);

									option[6].init(50, 200, "Block H:", renderer);
									option[7].init(option[12].rect.w + 70, 200, "75", renderer, true);

									option[8].init(50, 250, "Block Angle:", renderer);
									option[9].init(option[12].rect.w + 70, 250, "0", renderer, true);

									option[10].init(50, 300, "Block Red:", renderer);
									option[11].init(option[12].rect.w + 70, 300, "255", renderer, true);

									option[13].init(option[12].rect.w + 70, 350, "255", renderer, true);

									option[14].init(50, 400, "Block Blue:", renderer);
									option[15].init(option[12].rect.w + 70, 400, "255", renderer, true);

									option[16].init(50, 450, "Block Alpha:", renderer);
									option[17].init(option[12].rect.w + 70, 450, "255", renderer, true);

									option[18].init(50, 500, "Add Block", renderer);
									option[19].init(50, 550, "Return", renderer);

									if(MenuSection())
									blocks.push_back(Block(std::stoi(option[1].stext) ,std::stoi(option[3].stext) ,std::stoi(option[5].stext) ,std::stoi(option[7].stext)
										,std::stoi(option[9].stext) ,std::stoi(option[11].stext) ,std::stoi(option[13].stext) ,std::stoi(option[15].stext) ,
										std::stoi(option[17].stext)));

									SetOptions();
									break;
								}
								case 3:
								{
									option.clear();
									option.resize(8);
									option[4].init(50, 150, "Program Run Frames:", renderer);
									option[0].init(50, 50, "Window Width:", renderer);
									option[1].init(option[4].rect.w + 70, 50, std::to_string(W), renderer, true);
									option[2].init(50, 100, "Window Height:", renderer);
									option[3].init(option[4].rect.w + 70, 100, std::to_string(H), renderer, true);
									option[5].init(option[4].rect.w + 70, 150, std::to_string(frames), renderer, true);

									option[6].init(50, 200, "Save Options", renderer);
									option[7].init(50, 250, "Return", renderer);

									if (MenuSection())
									{
										W = std::stoi(option[1].stext);
										H = std::stoi(option[3].stext);
										frames = std::stoi(option[5].stext);
										SDL_SetWindowSize(window, W, H);
									}

									SetOptions();
									break;
								}
								case 4:
								{
									menu = false;
									break;
								}
							}
						}
					}
					else SDL_SetTextureColorMod(option[i].text, 255, 255, 0);
				}
			}
		
			//Clear screen
			SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
			SDL_RenderClear(renderer);

			for (int i = 0; i < option.size(); i++)
				option[i].show();

			//Update screen
			SDL_RenderPresent(renderer);

			SDL_Delay(16);
		}
	}
};

int main(int argc, char* args[])
{
	Main_menu Mmenu;
	Mmenu.init();
	Mmenu.MenuLoop();
	Mmenu.clear();
	/*
	SDL_Event e;
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	std::vector<Ball> balls;
	std::vector<Block> blocks;
	size_t frames = 1000, W = 1200, H = 800;
	clock_t start, stop;
	double czas;
	int wybor;
	bool menu = true;

	//Ball(double x, double y, size_t _rad, double _vel, uint8_t r, uint8_t g, uint8_t b, uint8_t a, double dx, double dy)

	std::vector<Ball> balls = { {100, 100, 60, 10, 255, 255, 0, 128, 1, 0.3}, {200, 600, 100, 3, 255, 255, 255, 255, 0.4, 0.3},
	{656, 500, 18, 11, 255, 100, 100, 50, 0.3, -3}, {430,430, 70, 8, 0,255,255,140,-0.3,1}, {340, 600, 40, 11, 255,0,255,128,1,0.3},
		{}, {150,150,60,10,0,255,0,128,1,1}, {150,150,60,10,0,0,255,128,-1,1} };



	std::vector<Block> blocks = { {400, 300, 400, 30, 23}, {800, 600, 400, 30, -73} };

	while (menu)
	{
		system("cls");
		std::cout << "Malowana Kula () " << std::endl;
		std::cout << "[1] Start "  << std::endl;
		std::cout << "[2] Opcje "  << std::endl;
		std::cout << "[3] Dodaj obiekt \"kula\" "  << std::endl;
		std::cout << "[4] Dodaj obiekt \"klocek\" " << std::endl;
		std::cout << "[5] Wyjdz " <<  std::endl;
		std::cin >> wybor;

		switch (wybor)
		{
		case 1:
		{
			//ramka
			blocks.push_back(Block(W/2, 5, W, 10));
			blocks.push_back(Block(5, H/2, 10, H));
			blocks.push_back(Block(W/2, H-5, W, 10));
			blocks.push_back(Block(W-5, H/2, 10, H));
			system("cls");
			menu = false;
			break;
		}

		case 2:
		{
			system("cls");
			std::cout << "czas odtwarzania programów: " << frames <<std::endl;
			std::cout << "Szerokosc okna: " << W << std::endl;
			std::cout << "Wysokosc okna: " << H << std::endl;

			std::cout << "Nowy czas odtwarzania programów: ";
			std::cin >> frames;
			std::cout << "Nowa szerokosc okna: ";
			std::cin >> W;
			std::cout << "Nowa wysokosc okna: ";
			std::cin >> H;
			break;
		}

		default:
		{
			return 0;
		}
			
		}
	}

	Menu menu1(balls, blocks);
	start = clock();
	//how many frames, asembler?
	menu1.main_loop(frames, false);
	stop = clock();
	czas = (double)(stop - start) / CLOCKS_PER_SEC;
	std::cout << "Pierwszy program czas: " << czas << std::endl;

	Menu menu2(balls, blocks);
	start = clock();
	menu2.main_loop(frames, true);
	stop = clock();
	czas = (double)(stop - start) / CLOCKS_PER_SEC;
	std::cout << "Drugi program czas: " << czas << std::endl;*/
	return 0;
}