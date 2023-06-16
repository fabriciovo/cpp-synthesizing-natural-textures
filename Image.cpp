#include "Image.h"

Image::Image(SDL_Renderer* renderer, const std::string& imagePath, int width, int height)
        : renderer(renderer), texture(nullptr), width(width), height(height) {
        // Carregar a imagem
        SDL_Surface* imageSurface = IMG_Load(imagePath.c_str());
        if (!imageSurface) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha ao carregar a imagem: %s", IMG_GetError());
            return;
        }

        // Criar uma textura a partir da superfície da imagem
        texture = SDL_CreateTextureFromSurface(renderer, imageSurface);
        this->surface = imageSurface;
        if (!texture) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha ao criar a textura da imagem: %s", SDL_GetError());
            SDL_FreeSurface(imageSurface);
            return;
        }

        SDL_FreeSurface(imageSurface);
    }

Image::Image(SDL_Renderer* renderer, SDL_Surface* surface, int width, int height)
        : renderer(renderer), width(width), height(height)
    {
        // Crie a textura a partir da superfície usando o renderer fornecido
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        // Verifique se a criação da textura foi bem-sucedida

        // Libere a memória alocada para a superfície
        SDL_FreeSurface(surface);
    }

Image::~Image() {
        if (texture) {
            SDL_DestroyTexture(texture);
            texture = nullptr;
        }
    }

SDL_Surface* Image::getSurface() const {
    return surface;
}

    void Image::render(int x, int y) {
        SDL_Rect destRect = { x, y, width, height };
        SDL_RenderCopy(renderer, texture, nullptr, &destRect);
    }

int Image::getWidth() {
    return this->width;
}

int Image::getHeight() {
    return this->height;
}
