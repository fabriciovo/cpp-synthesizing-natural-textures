//
//  Text.cpp
//  cpp-synthesizing-natural-textures
//
//  Created by fabricio varisco oliveira on 12/06/23.
//

#include "Text.h"

Text::Text(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, const SDL_Color& color, int x, int y)
    : renderer(renderer), font(font), text(text), color(color), x(x), y(y), texture(nullptr), width(0), height(0) {
    renderTexture();
}

Text::~Text() {
    if (this->texture) {
        SDL_DestroyTexture(this->texture);
        this->texture = nullptr;
    }
}

void Text::setText(const std::string& newText) {
    this->text = newText;
    this->renderTexture();
}

void Text::setColor(const SDL_Color& newColor) {
    this->color = newColor;
    this->renderTexture();
}

void Text::setPosition(int newX, int newY) {
    this->x = newX;
    this->y = newY;
}

void Text::render() {
    if (this->texture) {
        SDL_Rect dstRect = { this->x, this->y, this->width, this->height };
        SDL_RenderCopy(this->renderer, this->texture, nullptr, &dstRect);
    }
}



void Text::renderTexture() {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }

    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (!surface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha ao criar a superfície de texto: %s", TTF_GetError());
        return;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Falha ao criar a textura de texto: %s", SDL_GetError());
        SDL_FreeSurface(surface);
        return;
    }

    width = surface->w;
    height = surface->h;

    SDL_FreeSurface(surface);
}
