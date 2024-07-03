#include <bits/stdc++.h>
#include"SDL.h"
#define CHIEUCAO 700
#define CHIEURONG 700
#define DOLONBANG 10
#define DORONGBANG 200
#define TOADO_X 50
#define TOADO_Y 50

struct playerScore{
	int endScore;
	double endTime;
	int endSize;
};

void DrawString(SDL_Surface *screen, int x, int y, const char *text, SDL_Surface *charset) {
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while(*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screen, &d);
		x += 8;
		text++;
		};
	};




void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 color) {
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
	*(Uint32 *)p = color;
};



void DrawLine(SDL_Surface *screen, int x, int y, int l, int dx, int dy, Uint32 color) {
	for(int i = 0; i < l; i++) {
		DrawPixel(screen, x, y, color);
		x += dx;
		y += dy;
		};
	};

void DrawRectangle(SDL_Surface *screen, int x, int y, int l, int k,
                   Uint32 outlineColor, Uint32 fillColor) {
	int i;
	DrawLine(screen, x, y, k, 0, 1, outlineColor);
	DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
	DrawLine(screen, x, y, l, 1, 0, outlineColor);
	DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
	for(i = y + 1; i < y + k - 1; i++)
		DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
	};

void DrawPoint(SDL_Surface *screen, int X, int Y, int width, const char *number, SDL_Surface *charset) {
	int length = strlen(number);
	DrawString(screen, (X + width / 2) - length*4, Y + width / 2, number, charset);
}


int get_power(int x) {
	int n = 0;
	while (x != 0) {
		x = x/2;
		n++;
	}

	return n;
}
void DrawBoard(SDL_Surface *screen, int **tab, int size, Uint32 outlineColor, Uint32 fillColor, SDL_Surface *charset) {
	int width = (DORONGBANG / size) - 2;
	int X, Y;
	char number[5];
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			X = TOADO_X + (DORONGBANG / size)*i;
			Y = TOADO_Y + (DORONGBANG / size)*j;
			int color_power = get_power(tab[i][j]);
			DrawRectangle(screen, Y, X, width, width, outlineColor, SDL_MapRGB(screen->format, 46, 204 + 10 * color_power, 113 + 10*color_power));
			if(tab[i][j] == 0) sprintf(number, "");
			else sprintf(number, "%d", tab[i][j]);
			DrawPoint(screen, Y, X, width, number, charset);
		}
	}
}

void DrawScore(SDL_Surface *screen, unsigned int score, SDL_Surface *charset) {
	int X = TOADO_X;
	int Y = TOADO_Y + DORONGBANG + 100;
	int width = 40;
	char number[18];
	sprintf(number, "Diem : %u", score);
	DrawPoint(screen, Y, X, width, number, charset);
}

int** createtab(int size) {
	int **tab;
	tab = new int *[size];
	for (int i = 0; i < size; ++i) tab[i] = new int[size];

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			tab[i][j] = 0;
		}
	}
	int X = rand() % size;
	int Y = rand() % size;
	tab[X][Y] = 2;
	X = rand() % size;
	Y = rand() % size;
	tab[X][Y] = 2;

	return tab;
}

bool checkWin(int **&tab, int size) {
	bool win = 0;

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (tab[i][j] == 2048) win = 1;
		}
	}

	return win;
}

bool checkPut(int **&tab, int size) {
	bool put = 0;

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (tab[i][j] == 0) put = 1;
		}
	}

	return put;
}

bool checkLose(int **&tab, int size) {
	bool lose = 1;

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size - 1; j++) {
			if (tab[i][j] == tab[i][j + 1]) lose = 0;
		}
	}
	for (int i = 0; i < size - 1; i++) {
		for (int j = 0; j < size; j++) {
			if (tab[i][j] == tab[i + 1][j]) lose = 0;
		}
	}

	if (checkPut(tab, size)) lose = 0;

	return lose;
}

bool checkToRandom(int **&tab, int **&backTab, int size) {
	int can_put = 0;
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			if (tab[i][j] != backTab[i][j]) can_put = 1;
		}
	}
	return can_put;
}

void randomOnBoard(int **&tab, int size) {

	if (checkPut(tab, size)) {
		int values[10] = { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 };
		int value = values[rand() % 10];

		int X = rand() % size;
		int Y = rand() % size;
		if (tab[X][Y] == 0) tab[X][Y] = 2;
		else {
			while (tab[X][Y] != 0) {
				X = rand() % size;
				Y = rand() % size;
			}
			tab[X][Y] = value;
		}
	}
}

void rushTop(int **&tab, int size) {
	for (int i = 1; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (tab[i][j] != 0) {
				for (int k = i; k > 0; --k) {
					if (tab[k - 1][j] == 0) {
						tab[k - 1][j] = tab[k][j];
						tab[k][j] = 0;
					}
				}
			}
		}
	}
}
void moveTop(int **&tab, int size, unsigned int &score) {
	rushTop(tab, size);

	for (int i = 1; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (tab[i][j] != 0) {
				if (tab[i - 1][j] == tab[i][j]) {
					tab[i - 1][j]+= tab[i][j];
					score += tab[i - 1][j];
					tab[i][j] = 0;

				}
			}
		}
	}

	rushTop(tab, size);
}

void rushBot(int **&tab, int size) {
	for (int i = 0; i < size; i++) {
		for (int j = size - 2; j >= 0; j--) {
			if (tab[j][i] != 0) {
				for (int k = j; k < size - 1; ++k) {
					if (tab[k + 1][i] == 0) {
						tab[k + 1][i] = tab[k][i];
						tab[k][i] = 0;
					}
				}
			}
		}
	}
}
void moveBot(int **&tab, int size, unsigned int &score) {
	rushBot(tab, size);

	for (int i = 0; i < size; i++) {
		for (int j = size - 2; j >= 0; j--) {
			if (tab[j][i] != 0) {
				if (tab[j][i] == tab[j + 1][i]) {
					tab[j + 1][i] += tab[j][i];
					score += tab[j + 1][i];
					tab[j][i] = 0;
				}
			}
		}
	}

	rushBot(tab, size);
}

void rushRight(int **&tab, int size) {

	for (int i = 0; i < size; i++) {
		for (int j = size - 2; j >= 0; j--) {
			if (tab[i][j] != 0) {
				for (int k = j; k < size - 1; ++k) {
					if (tab[i][k + 1] == 0) {
						tab[i][k + 1] = tab[i][k];
						tab[i][k] = 0;
					}
				}
			}
		}
	}
}
void moveRight(int **&tab,int size, unsigned int &score) {
	rushRight(tab, size);

	for (int i = 0; i < size; i++) {
		for (int j = size - 2; j >= 0; j--) {
			if (tab[i][j] != 0) {
				if (tab[i][j] == tab[i][j + 1]) {
					tab[i][j + 1] += tab[i][j];
					score += tab[i][j + 1];
					tab[i][j] = 0;
				}
			}
		}
	}

	rushRight(tab, size);
}

void rushLeft(int **&tab, int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 1; j < size; j++) {
			if (tab[i][j] != 0) {
				for (int k = j; k > 0; --k) {
					if (tab[i][k - 1] == 0) {
						tab[i][k - 1] = tab[i][k];
						tab[i][k] = 0;
					}
				}
			}
		}
	}
}
void moveLeft(int **&tab, int size, unsigned int &score) {

	rushLeft(tab, size);

	for(int i = 0; i < size; i++) {
		for (int j = 1; j < size; j++) {
			if (tab[i][j] != 0) {
				if (tab[i][j] == tab[i][j - 1]) {
					tab[i][j - 1] += tab[i][j];
					score += tab[i][j - 1];
					tab[i][j] = 0;
				}
			}
		}
	}

	rushLeft(tab, size);
}

void copyTab(int **&tab1,int **&tab2, int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) tab1[i][j] = tab2[i][j];
	}
}

void move(SDL_Event event, int **&tab, int size, int **&backtab, unsigned int &score, unsigned int &backScore) {
	int **checkTab = createtab(size);
	int checkScore = backScore;
	copyTab(checkTab, backtab, size);

	copyTab(backtab, tab, size);
	backScore = score;


	if (event.key.keysym.sym == SDLK_UP) moveTop(tab, size, score);
	else if (event.key.keysym.sym == SDLK_DOWN) moveBot(tab, size, score);
	else if (event.key.keysym.sym == SDLK_LEFT) moveLeft(tab, size, score);
	else if (event.key.keysym.sym == SDLK_RIGHT) moveRight(tab, size, score);

	if (checkToRandom(tab, backtab, size)) {
		randomOnBoard(tab, size);
	}
	else {
		copyTab(backtab, checkTab, size);
		backScore = checkScore;
	}

	for (int i = 0; i < size; ++i)
		delete[] checkTab[i];
	delete[] checkTab;
}

void sortByScore(playerScore *&tab, int size) {
	playerScore helper;
	for (int i = 0; i < size - 1; i++) {
		for (int j = 0; j < size - 1 - i; j++) {
			if (tab[j].endScore > tab[j + 1].endScore) {
				helper = tab[j];
				tab[j] = tab[j + 1];
				tab[j + 1] = helper;
			}
		}
	}

	for (int i = 0; i < size / 2; i++) {
		helper = tab[i];
		tab[i] = tab[size - i - 1];
		tab[size - i - 1] = helper;
	}
}

void sortByTime(playerScore *&tab, int size) {
	playerScore helper;
	for (int i = 0; i < size - 1; i++) {
		for (int j = 0; j < size - 1 - i; j++) {
			if (tab[j].endTime > tab[j + 1].endTime) {
				helper = tab[j];
				tab[j] = tab[j + 1];
				tab[j + 1] = helper;
			}
		}
	}
}

playerScore *createRanking(playerScore *tab, int amount, int size, int fileSize) {
	playerScore *ranking = new playerScore[amount];
	int j = 0;
	for (int i = 0; i < fileSize; i++) {
		if (tab[i].endSize == size) {
			ranking[j].endScore = tab[i].endScore;
			ranking[j].endTime = tab[i].endTime;
			ranking[j].endSize = tab[i].endSize;
			j++;
		}
	}
	return ranking;
}

void saveToFile(int score, double time, int size) {
	FILE *fileList = fopen("winnerList.txt", "a");
	fprintf(fileList, "%u %.1lf %d\n", score, time, size);

	fclose(fileList);
}
playerScore *openFile(int &lines){
	FILE *fileList = fopen("winnerList.txt", "r");
	int ch = 0;

	if (fileList == NULL)
	{
		return 0;
	}

	while (!feof(fileList))
	{
		ch = fgetc(fileList);
		if (ch == '\n')
		{
			lines++;
		}
	}

	playerScore *list = new playerScore [lines];
	fseek(fileList, 0, SEEK_SET);
	for (int i = 0; i < lines; i++) {
		fscanf(fileList, "%d %lf %d", &list[i].endScore, &list[i].endTime, &list[i].endSize);
	}
	fclose(fileList);

	return list;
}


int main(int argc, char **argv) {
	int size = 4;
	bool getStart = 0;
	unsigned int score = 0;
	unsigned int backScore = score;
	int showSize = 0;
	srand(time(NULL));
	int** tab;
	int** backtab;
	int fileSize = 0;
	playerScore *winnerList;

	int t1, t2, quit, frames, rc;
	double delta, worldTime, fpsTimer, fps, distance, etiSpeed;
	SDL_Event event;
	SDL_Surface *screen, *charset;
	SDL_Surface *eti;
	SDL_Texture *scrtex;
	SDL_Window *window;
	SDL_Renderer *renderer;

	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}


	rc = SDL_CreateWindowAndRenderer(CHIEUCAO, CHIEURONG, 0,
	                                 &window, &renderer);
	if(rc != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		return 1;
		};

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, CHIEUCAO, CHIEURONG);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	SDL_SetWindowTitle(window, "Game 2048 by Tuan dz");


	screen = SDL_CreateRGBSurface(0, CHIEUCAO, CHIEURONG, 32,
	                              0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
	                           SDL_TEXTUREACCESS_STREAMING,
	                           CHIEUCAO, CHIEURONG);


	//disable cursor visibility
	SDL_ShowCursor(SDL_DISABLE);

	charset = SDL_LoadBMP("./font.bmp");
	if(charset == NULL) {
		printf("SDL_LoadBMP(font.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};
	SDL_SetColorKey(charset, true, 0x000000);
	char text[128];
	int blue = SDL_MapRGB(screen->format, 52, 152, 219);
	int green = SDL_MapRGB(screen->format, 46, 204, 113);

	tab = createtab(size);
	backtab = createtab(size);
	copyTab(tab, backtab, size);
	winnerList = openFile(fileSize);

	frames = 0;
	fpsTimer = 0;
	fps = 0;
	quit = 0;
	worldTime = 0;
	distance = 0;
	etiSpeed = 1;





	while(!quit) {

		if (!getStart) {

			if (size >= 10) size = 10;
			if (size <= 3) size = 3;

			SDL_FillRect(screen, NULL, blue);
			DrawRectangle(screen, 4, 4, CHIEUCAO - 8, 36, blue, green);
			sprintf(text, "Chon kich thuoc ban do: \030 - Tang, \031 - Giam Enter - Xac nhan");
			DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 26, text, charset);

			DrawRectangle(screen, CHIEUCAO/2 - 100, CHIEURONG/2 - 36, 200, 36, blue, green);
			sprintf(text, "Kich thuc: %d", size);
			DrawString(screen, CHIEUCAO/2  -  strlen(text) * 4, CHIEURONG / 2 - 20, text, charset);


			while (SDL_PollEvent(&event)) {
				switch (event.type) {
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_ESCAPE) quit = 1;
					else if (event.key.keysym.sym == SDLK_UP) size++;
					else if (event.key.keysym.sym == SDLK_DOWN) size--;
					else if (event.key.keysym.sym == SDLK_RETURN) {
						getStart = 1;
						tab = createtab(size);
						backtab = createtab(size);
						copyTab(tab, backtab, size);
						t1 = SDL_GetTicks();
						backScore = 0;
						score = 0;
						worldTime = 0;
					}
					break;
				case SDL_QUIT:
					quit = 1;
					break;
				};
			};

			SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
			SDL_RenderCopy(renderer, scrtex, NULL, NULL);
			SDL_RenderPresent(renderer);
		}
		else if(getStart && !checkLose(tab, size) && !checkWin(tab, size)){

			t2 = SDL_GetTicks();
			delta = (t2 - t1) * 0.001;
			t1 = t2;
			worldTime += delta;
			fpsTimer += delta;
			if (fpsTimer > 0.5) {
				fps = frames * 2;
				frames = 0;
				fpsTimer -= 0.5;
			};

			SDL_FillRect(screen, NULL, blue);


			DrawBoard(screen, tab, size, blue, green, charset);
			DrawScore(screen, score, charset);

			DrawRectangle(screen, 4, 4, CHIEUCAO - 8, 36, blue, green);
			sprintf(text, "2048 game,      thoi gian = %.1lf s  %.0lf fps / s", worldTime, fps);
			DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 10, text, charset);
			sprintf(text, "Esc - Thoat, Arrows - Di chuyen, u - Di lai, n - Game moi");
			DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 26, text, charset);

			SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
			SDL_RenderCopy(renderer, scrtex, NULL, NULL);
			SDL_RenderPresent(renderer);

			while(SDL_PollEvent(&event)) {
				switch(event.type) {
					case SDL_KEYDOWN:
						if (event.key.keysym.sym == SDLK_ESCAPE) quit = 1;
						else if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_DOWN ||
							event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_RIGHT) move(event, tab, size, backtab, score, backScore);
						else if (event.key.keysym.sym == SDLK_u) {
							copyTab(tab, backtab, size);
							score = backScore;
						}
						else if (event.key.keysym.sym == SDLK_n) {
							for (int i = 0; i < size; ++i)
								delete[] tab[i];
							delete[] tab;

							for (int i = 0; i < size; ++i)
								delete[] backtab[i];
							delete[] backtab;
							getStart = 0;
						}
						else if (event.key.keysym.sym == SDLK_r) {

							bool show_ranking = 0;
							int amountBoard = 0;
							int startShow = 0;
							int limit = 10;
							for (int i = 0; i < fileSize; i++) if (size == winnerList[i].endSize) amountBoard++;
							playerScore *sizeRanking = createRanking(winnerList, amountBoard, size, fileSize);
							while (!show_ranking) {

								SDL_FillRect(screen, NULL, blue);
								DrawRectangle(screen, 4, 4, CHIEUCAO - 8, 36, blue, green);
								sprintf(text, "Danh sach nguoi chien thang %dx%d", size, size);
								DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 10, text, charset);
								sprintf(text, "esc -  quay lai, nhan t de sap xep theo thoi gian va p de sap xep theo diem");
								DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 26, text, charset);


								if (startShow <= 0) startShow = 0;
								if (startShow >= amountBoard - 10) startShow = amountBoard - 10;
								if (amountBoard <= 10) {
									startShow = 0;
									limit = amountBoard;
								}

								if (fileSize == 0 || limit == 0) {
									sprintf(text, "Danh sach nguoi chien thang trong");
									DrawString(screen, CHIEUCAO / 2 - strlen(text) * 4, CHIEURONG / 2 + 16, text, charset);
								}

								int placeShow = 0;
								for (int i = startShow; placeShow < limit; i++) {
									DrawRectangle(screen, CHIEUCAO / 2 - 200, CHIEURONG / 2 - 190 + placeShow * 40, 400, 40, blue, green);
									sprintf(text, "%d. Thoi gian: %.1lfs Diem cua ban: %d",startShow + placeShow + 1, sizeRanking[i].endTime, sizeRanking[i].endScore);
									DrawString(screen, CHIEUCAO / 2 - strlen(text) * 4, CHIEURONG / 2 - 200 + 26 + placeShow * 40, text, charset);
									placeShow++;
								}

								while (SDL_PollEvent(&event)) {
									switch (event.type) {
									case SDL_KEYDOWN:
										if (event.key.keysym.sym == SDLK_ESCAPE) show_ranking = 1;
										if (event.key.keysym.sym == SDLK_t) sortByTime(sizeRanking, amountBoard);
										if (event.key.keysym.sym == SDLK_p) sortByScore(sizeRanking, amountBoard);
										else if (event.key.keysym.sym == SDLK_UP) startShow++;
										else if (event.key.keysym.sym == SDLK_DOWN) startShow--;

										break;
									case SDL_QUIT:
										quit = 1;
										show_ranking = 1;
										break;
									};
								};

								SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
								SDL_RenderCopy(renderer, scrtex, NULL, NULL);
								SDL_RenderPresent(renderer);
							}
							delete[] sizeRanking;
						}
						break;
					case SDL_KEYUP:
						etiSpeed = 1.0;
						break;
					case SDL_QUIT:
						quit = 1;
						break;
				};
			};
			frames++;
		}
		else if (checkLose(tab, size)) {
			double endTime = worldTime;
			while (!quit) {
				SDL_FillRect(screen, NULL, blue);
				DrawRectangle(screen, 4, 4, CHIEUCAO - 8, 36, blue, green);
				sprintf(text, "Ban thua: esc - thoat");
				DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 26, text, charset);

				DrawRectangle(screen, CHIEUCAO / 2 - 200, CHIEURONG / 2 - 36, 400, 36, blue, green);
				sprintf(text, "Thoi gian: %.1lfs  Diem: %u", endTime, score);
				DrawString(screen, CHIEUCAO / 2 - strlen(text) * 4, CHIEURONG / 2 - 20, text, charset);


				while (SDL_PollEvent(&event)) {
					switch (event.type) {
					case SDL_KEYDOWN:
						if (event.key.keysym.sym == SDLK_ESCAPE) quit = 1;
						break;
					case SDL_QUIT:
						quit = 1;
						break;
					};
				};

				SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
				SDL_RenderCopy(renderer, scrtex, NULL, NULL);
				SDL_RenderPresent(renderer);
			}
		}
		else if (checkWin(tab, size)) {
			double endTime = worldTime;
			while (!quit) {
				SDL_FillRect(screen, NULL, blue);
				DrawRectangle(screen, 4, 4, CHIEUCAO - 8, 36, blue, green);
				sprintf(text, "Ban thang: s - luu, esc -  de thoat");
				DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 26, text, charset);

				DrawRectangle(screen, CHIEUCAO / 2 - 200, CHIEURONG / 2 - 36, 400, 36, blue, green);
				sprintf(text, "Thoi gian: %.1lfs  Diem: %u", endTime, score);
				DrawString(screen, CHIEUCAO / 2 - strlen(text) * 4, CHIEURONG / 2 - 20, text, charset);


				while (SDL_PollEvent(&event)) {
					switch (event.type) {
					case SDL_KEYDOWN:
						if (event.key.keysym.sym == SDLK_ESCAPE) quit = 1;
						if (event.key.keysym.sym == SDLK_s) {
							saveToFile(score, endTime, size);



							while (!quit) {

								SDL_FillRect(screen, NULL, blue);
								DrawRectangle(screen, 4, 4, CHIEUCAO - 8, 36, blue, green);
								sprintf(text, "Luu diem!");
								DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 10, text, charset);
								sprintf(text, "esc -  thoat game");
								DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 26, text, charset);

								while (SDL_PollEvent(&event)) {
									switch (event.type) {
									case SDL_KEYDOWN:
										if (event.key.keysym.sym == SDLK_ESCAPE) quit = 1;

										break;
									case SDL_QUIT:
										quit = 1;
										break;
									};
								};

								SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
								SDL_RenderCopy(renderer, scrtex, NULL, NULL);
								SDL_RenderPresent(renderer);
							}
						}
						break;
					case SDL_QUIT:
						quit = 1;
						break;
					};
				};

				SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
				SDL_RenderCopy(renderer, scrtex, NULL, NULL);
				SDL_RenderPresent(renderer);
			}
		}
	};

	for (int i = 0; i < size; ++i)
		delete[] tab[i];
	delete[] tab;

	for (int i = 0; i < size; ++i)
		delete[] backtab[i];
	delete[] backtab;

	delete[] winnerList;

	SDL_FreeSurface(charset);
	SDL_FreeSurface(screen);
	SDL_DestroyTexture(scrtex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
	};
