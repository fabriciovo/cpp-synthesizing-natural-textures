//
//  main.cpp
//  cpp-synthesizing-natural-textures
//
//  Created by fabricio varisco oliveira on 10/06/23.
//

#include <iostream>
#include <fstream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "Text.h"
#include "Image.h"


Image* synthesizeTextures(SDL_Surface* sampleImage, SDL_Surface* preSynthesisImage, SDL_Renderer* renderer, int blockSize = 8, int iterations = 10)
{
    // Converter as superfícies para o formato RGBA
    SDL_Surface* sampleRGBA = SDL_ConvertSurfaceFormat(sampleImage, SDL_PIXELFORMAT_RGBA8888, 0);
    SDL_Surface* preSynthesisRGBA = SDL_ConvertSurfaceFormat(preSynthesisImage, SDL_PIXELFORMAT_RGBA8888, 0);

    // Obter as dimensões das imagens
    int sampleWidth = sampleRGBA->w;
    int sampleHeight = sampleRGBA->h;
    int preSynthesisWidth = preSynthesisRGBA->w;
    int preSynthesisHeight = preSynthesisRGBA->h;

    // Criar uma superfície para a imagem de resultado
    SDL_Surface* resultSurface = SDL_CreateRGBSurfaceWithFormat(0, sampleWidth, sampleHeight, 32, SDL_PIXELFORMAT_RGBA8888);

    // Dividir as imagens em blocos
    int numBlocksX = sampleWidth / blockSize;
    int numBlocksY = sampleHeight / blockSize;

    for (int i = 0; i < numBlocksX; i++) {
        for (int j = 0; j < numBlocksY; j++) {
            int sampleX = i * blockSize;
            int sampleY = j * blockSize;
            int preSynthesisX = i * blockSize % preSynthesisWidth;
            int preSynthesisY = j * blockSize % preSynthesisHeight;

            SDL_Rect sampleRect = { sampleX, sampleY, blockSize, blockSize };
            SDL_Surface* sampleBlock = SDL_CreateRGBSurfaceWithFormat(0, blockSize, blockSize, 32, SDL_PIXELFORMAT_RGBA8888);
            SDL_BlitSurface(sampleRGBA, &sampleRect, sampleBlock, NULL);

            SDL_Rect preSynthesisRect = { preSynthesisX, preSynthesisY, blockSize, blockSize };
            SDL_Surface* preSynthesisBlock = SDL_CreateRGBSurfaceWithFormat(0, blockSize, blockSize, 32, SDL_PIXELFORMAT_RGBA8888);
            SDL_BlitSurface(preSynthesisRGBA, &preSynthesisRect, preSynthesisBlock, NULL);

            Uint32* samplePixels = static_cast<Uint32*>(sampleBlock->pixels);
            Uint32* preSynthesisPixels = static_cast<Uint32*>(preSynthesisBlock->pixels);
            Uint32* resultPixels = static_cast<Uint32*>(resultSurface->pixels);

            for (int k = 0; k < blockSize * blockSize; k++) {
                Uint8 sampleR, sampleG, sampleB, sampleA;
                SDL_GetRGBA(samplePixels[k], sampleBlock->format, &sampleR, &sampleG, &sampleB, &sampleA);

                Uint8 preSynthesisR, preSynthesisG, preSynthesisB, preSynthesisA;
                SDL_GetRGBA(preSynthesisPixels[k], preSynthesisBlock->format, &preSynthesisR, &preSynthesisG, &preSynthesisB, &preSynthesisA);

                Uint8 resultR = preSynthesisR + (sampleR - preSynthesisR) * iterations / (iterations + 1);
                Uint8 resultG = preSynthesisG + (sampleG - preSynthesisG) * iterations / (iterations + 1);
                Uint8 resultB = preSynthesisB + (sampleB - preSynthesisB) * iterations / (iterations + 1);
                Uint8 resultA = preSynthesisA + (sampleA - preSynthesisA) * iterations / (iterations + 1);

                resultPixels[sampleY * resultSurface->w + sampleX + k % blockSize + k / blockSize * resultSurface->w] = SDL_MapRGBA(resultSurface->format, resultR, resultG, resultB, resultA);
            }

            SDL_FreeSurface(sampleBlock);
            SDL_FreeSurface(preSynthesisBlock);
        }
    }

    SDL_FreeSurface(sampleRGBA);
    SDL_FreeSurface(preSynthesisRGBA);

    Image* resultImage = new Image(renderer, resultSurface, sampleWidth, sampleHeight);

    SDL_FreeSurface(resultSurface);

    return resultImage;
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
    TTF_Font* font = TTF_OpenFont("/Users/fabriciovo/Developer/GitHub/cpp-synthesizing-natural-textures/cpp-synthesizing-natural-textures/arial.ttf", 24);

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
    Image * imageT = new Image(renderer, "/Users/fabriciovo/Developer/GitHub/cpp-synthesizing-natural-textures/cpp-synthesizing-natural-textures/t.png", 200, 150);
    Image * imageI = new Image(renderer, "/Users/fabriciovo/Developer/GitHub/cpp-synthesizing-natural-textures/cpp-synthesizing-natural-textures/i.png", 200, 150);
    
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
