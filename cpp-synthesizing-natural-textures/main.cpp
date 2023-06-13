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

void createPPMFile() {
        int width = 0;
        int height = 0;
        std::string fileName = "";
        
        std::cout << "Creating File..." << std::endl;
            
        std::cout << "File Name: ";
        std::cin >> fileName;
        
        std::cout << "Enter the Width: ";
        std::cin >> width;
        
        std::cout << "Enter the Height: ";
            std::cin >> height;
        
        
        std::ofstream fout(fileName+".ppm");
        if(fout.fail()) {
            std::cout << "Failed to create PPM file." << std::endl;
            return;
        }
        
        
        //Create Header
        fout << "P6\n";
        fout << width << " " << height << "\n";
        fout << "255\n";

        for(int y = 0; y < height; y++){
            for(int x = 0; x < width; x++){
                fout << u_int8_t(rand()%256);
                fout << u_int8_t(rand()%256);
                fout << u_int8_t(rand()%256);
            }
        }
        
        fout.close();
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


    SDL_Window* window = SDL_CreateWindow("Exemplo de Janela", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, HEIGHT, WIDTH, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    TTF_Font* font = TTF_OpenFont("/Users/fabriciovo/Developer/GitHub/cpp-synthesizing-natural-textures/cpp-synthesizing-natural-textures/arial.ttf", 24);

    if (!window || !renderer || !font) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha ao criar janela, renderer ou carregar a fonte: %s", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    Text text(renderer, font, "Sintese de Textura", { 255, 255, 255, 255 }, WIDTH / 2, 20);

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


        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
