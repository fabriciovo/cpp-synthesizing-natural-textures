#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

class Image {
public:
    Image(SDL_Renderer* renderer, const std::string& imagePath, int width, int height);

    ~Image();
    void render(int x, int y);

private:
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    int width, height;
};
