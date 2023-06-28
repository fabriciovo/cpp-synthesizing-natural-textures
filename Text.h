//
//  Text.h
//  cpp-synthesizing-natural-textures
//
//  Created by fabricio varisco oliveira on 12/06/23.
//

#ifndef TEXT_H
#define TEXT_H
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class Text {
public:
    Text(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, const SDL_Color& color, int x, int y);

    
    ~Text();
    
    void setText(const std::string& newText);
    
    void setColor(const SDL_Color& newColor);
    
    void setPosition(int newX, int newY);
    
    void render();
    
private:
    SDL_Renderer* renderer;
    TTF_Font* font;
    std::string text;
    SDL_Color color;
    int x, y;
    SDL_Texture* texture;
    int width, height;
    
    void renderTexture();
    
};


#endif /* TEXT_H */
