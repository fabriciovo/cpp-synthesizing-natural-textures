//
//  main.cpp
//  cpp-synthesizing-natural-textures
//
//  Created by fabricio varisco oliveira on 10/06/23.
//

#include <iostream>
#include <fstream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <vector>
#include <math.h>
#include "Text.h"
#include "Image.h"

struct Position {
    int x, y;
};

struct ValidPositions {
    struct Position candidates[3];
};

ValidPositions escolhe_vizinho() {


    // return ;

};


float isSimilarColor(SDL_Color color1, SDL_Color color2) {
    int redDiff = std::powf(color1.r - color2.r, 2);
    int greenDiff = std::powf(color1.g - color2.g, 2);
    int blueDiff = std::powf(color1.b - color2.b, 2);
    return redDiff + greenDiff + blueDiff;
}

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
    Uint8* p = static_cast<Uint8*>(surface->pixels) + y * surface->pitch + x * bpp;
    switch (bpp) {
    case 1:
        *p = pixel;
        break;
    case 2:
        *reinterpret_cast<Uint16*>(p) = pixel;
        break;
    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        }
        else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;
    case 4:
        *reinterpret_cast<Uint32*>(p) = pixel;
        break;
    }
}



Image* synthesizeTextures(SDL_Surface* sampleImage, SDL_Surface* preSynthesisImage, SDL_Renderer* renderer, int blockSize = 8, int iterations = 10)
{


    SDL_Surface* sampleImageSurface = SDL_CreateRGBSurface(0, sampleImage->w, sampleImage->h,
        sampleImage->format->BitsPerPixel,
        sampleImage->format->Rmask,
        sampleImage->format->Gmask,
        sampleImage->format->Bmask,
        sampleImage->format->Amask);


    SDL_Surface* preSynthesisImageSurface = SDL_CreateRGBSurface(0, preSynthesisImage->w, preSynthesisImage->h,
        preSynthesisImage->format->BitsPerPixel,
        preSynthesisImage->format->Rmask,
        preSynthesisImage->format->Gmask,
        preSynthesisImage->format->Bmask,
        preSynthesisImage->format->Amask);




    //std::vector<ValidPositions> candidates;
    //for (int i = 0; i < sampleImage->h; i++) {}
    //for (int j = 0; j < sampleImage->w; j++)
    //    candidates.push_back(escolhe_vizinho());


    for (int y = 0; y < preSynthesisImage->h; y++) {
        for (int x = 0; x < preSynthesisImage->w; x++) {
            //TODO pegar vizinhanca do resultado aqui
            for (int y = 0; y < sampleImage->h; y++) {
                for (int x = 0; x < sampleImage->w; x++) {
                    //matrix n vizinhos do pixel da amostra


                    //comparar matrizes de vizinhanca aqui (aqui so pra descobrir a diferenca total) retorna o valor total
                    
                    

                    //e o valor total vai ser comparado 
                    
                }
            }
            //x e o y do for da amostra q  teve o menor valor da amostra é o pixel q tem q copiar
        }
    }



    Image* newImage = new Image(renderer, sampleImage, 200, 150);
    return newImage;

}

int main(int argc, char* argv[]) {
    static const int HEIGHT = 800;
    static const int WIDTH = 600;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha ao inicializar o SDL: %s", SDL_GetError());
        return 1;
    }

    if (TTF_Init() < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha ao inicializar o SDL_ttf: %s", TTF_GetError());
        SDL_Quit();
        return 1;
    }


    SDL_Window* window = SDL_CreateWindow("Sintese de Textura", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, HEIGHT, WIDTH, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    TTF_Font* font = TTF_OpenFont("arial.ttf", 24);

    if (!window || !renderer || !font) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha ao criar janela, renderer ou carregar a fonte: %s", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    //Title
    Text text(renderer, font, "Sintese de Textura", { 255, 255, 255, 255 }, WIDTH / 2, 20);

    //Image Titles
    Text textImagem(renderer, font, "Imagem", { 255, 255, 255, 255 }, 90, HEIGHT / 2 - 30);
    Text textTextura(renderer, font, "Textura", { 255, 255, 255, 255 }, WIDTH / 2, HEIGHT / 2 - 30);
    Text textResult(renderer, font, "Resultado", { 255, 255, 255, 255 }, 510, HEIGHT / 2 - 30);

    //Images
    Image* imageT = new Image(renderer, "t.png", 200, 150);
    Image* imageI = new Image(renderer, "i.png", 200, 150);

    // Obter as superfícies das imagens
    SDL_Surface* sampleSurface = imageI->getSurface();
    SDL_Surface* preSynthesisSurface = imageT->getSurface();

    // Sintetizar as texturas
    Image* resultImage = synthesizeTextures(sampleSurface, preSynthesisSurface, renderer);

    bool quit = false;
    SDL_Event event;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_RenderClear(renderer);

        text.render();
        textImagem.render();
        textTextura.render();
        textResult.render();

        imageI->render(90, HEIGHT / 2);
        imageT->render(WIDTH / 2, HEIGHT / 2);
        resultImage->render(510, HEIGHT / 2);
        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}