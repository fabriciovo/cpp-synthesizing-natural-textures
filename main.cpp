//
//  main.cpp
//  cpp-synthesizing-natural-textures
//
//  Created by fabricio varisco oliveira on 10/06/23.
//

#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "Text.h"
#include "Image.h"



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
    Image image(renderer, "/Users/fabriciovo/Developer/GitHub/cpp-synthesizing-natural-textures/cpp-synthesizing-natural-textures/t.png", 200, 150);
    
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
        
        image.render(90, HEIGHT / 2);
        image.render(WIDTH / 2, HEIGHT / 2);
        image.render(510, HEIGHT / 2);
        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
