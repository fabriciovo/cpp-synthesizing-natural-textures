#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <math.h>
#include <iostream>
#include <fstream>
#include "Image.h"


Uint32 SDL_GetPixel(SDL_Surface* surface, int x, int y) {
	int bpp = surface->format->BytesPerPixel;
	Uint8* p = static_cast<Uint8*>(surface->pixels) + y * surface->pitch + x * bpp;
	switch (bpp) {
	case 1:
		return *p;
	case 2:
		return *reinterpret_cast<Uint16*>(p);
	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			return p[0] << 16 | p[1] << 8 | p[2];
		else
			return p[0] | p[1] << 8 | p[2] << 16;
	case 4:
		return *reinterpret_cast<Uint32*>(p);
	default:
		return 0;
	}
}

void SDL_SetPixel(SDL_Surface* surface, int x, int y, Uint32 pixel) {
	int bpp = surface->format->BytesPerPixel;
	Uint8* pixelAddress = static_cast<Uint8*>(surface->pixels) + y * surface->pitch + x * bpp;

	switch (bpp) {
	case 1:
		*pixelAddress = static_cast<Uint8>(pixel);
		break;
	case 2:
		*reinterpret_cast<Uint16*>(pixelAddress) = static_cast<Uint16>(pixel);
		break;
	case 3: {
		Uint8 r, g, b;
		SDL_GetRGB(pixel, surface->format, &r, &g, &b);
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
		pixelAddress[0] = r;
		pixelAddress[1] = g;
		pixelAddress[2] = b;
#else
		pixelAddress[2] = r;
		pixelAddress[1] = g;
		pixelAddress[0] = b;
#endif
		break;
	}
	case 4:
		*reinterpret_cast<Uint32*>(pixelAddress) = pixel;
		break;
	default:
		break; // Invalid format
	}
}


int main(int argc, char* argv[])
{
	static const int HEIGHT = 800;
	static const int WIDTH = 600;

	//Input input = Input();

	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha ao inicializar o SDL: %s", SDL_GetError());
		return 1;
	}


	if (TTF_Init() < 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha ao inicializar o SDL_ttf: %s", TTF_GetError());
		SDL_Quit();
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow("Sintese de imagem", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, HEIGHT, WIDTH, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	TTF_Font* font = TTF_OpenFont("arial.ttf", 28);


	if (!window || !renderer || !font) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha ao criar janela, renderer ou carregar a fonte: %s", SDL_GetError());
		TTF_Quit();
		SDL_Quit();
		return 1;
	}

	Image* preResult = new Image(renderer, "preResult64.png", 64, 64);
	Image* sample = new Image(renderer, "sample32.png", 32, 32);

	SDL_Surface* preResultSurface = preResult->getSurface();
	SDL_Surface* sampleSurface = sample->getSurface();

	int s, r;
	int fW = 11;
	int fH = 11;
	int n2 = 11 / 2;
	int px, py;


	SDL_Surface* resultSurface = SDL_CreateRGBSurface(0, preResultSurface->w, preResultSurface->h,
		preResultSurface->format->BitsPerPixel,
		preResultSurface->format->Rmask,
		preResultSurface->format->Gmask,
		preResultSurface->format->Bmask,
		preResultSurface->format->Amask);

	for (int ry = 0; ry < preResultSurface->h; ry++) {
		for (int rx = 0; rx < preResultSurface->w; rx++) {
			double dmin = 999999;
			int px = 0;
			int py = 0;
			for (int sy = 0; sy < sampleSurface->h; sy++) {
				for (int sx = 0; sx < sampleSurface->w; sx++) {
					double d = 0;
					for (int y = -n2; y < n2; y++) {
						for (int x = -n2; x < n2; x++) {
							Uint32 s = SDL_GetPixel(sampleSurface, sx + x, sy + y);
							Uint32 r = SDL_GetPixel(preResultSurface, rx + x, ry + y);
							double diff = abs(static_cast<double>(r) - static_cast<double>(s));
							d += diff;
						}
					}
					if (d < dmin) {
						px = sx;
						py = sy;
						dmin = d;
					}
				}
			}
			Uint32 pixel = SDL_GetPixel(sampleSurface, px, py);
			SDL_SetPixel(resultSurface, rx, ry, pixel);
		}

		system("cls");
		int per = (ry + 1) * 100 / resultSurface->h;
		std::cout << per << " %" << std::endl;
	}


	Image* result = new Image(renderer, resultSurface, 200, 200);
	if (IMG_SavePNG(result->getSurface(), "output.png") != 0) {
		printf("Failed to save image: %s\n", IMG_GetError());
		IMG_Quit();
		SDL_Quit();
		return 1;
	}


	bool quit = false;
	SDL_Event event;
	while (!quit) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				quit = true;
			}
		}
		result->render(200, 200);



		SDL_RenderClear(renderer);

	}


	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();

	return 0;
}