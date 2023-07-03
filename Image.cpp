#include "Image.h"

Image::Image()
{
}

Image::Image(int h, int w) {
    height = h;
    width = w;
    pixels = new int[width * height];
}

int Image::getHeight() {
    return height;
}

int Image::getWidth() {
    return width;
}

void Image::setPixelP(int rgb, int x, int y) {
    pixels[x + y * width] = rgb;
}

void Image::setPixel(int r, int g, int b, int x, int y) {
    int rgb = (r << 16) | (g << 8) | b;
    setPixelP(rgb, x, y);
}

int Image::getPixel(int x, int y) {
    return pixels[x + y * width];
}

int* Image::getPixels() {
    return pixels;
}

void Image::setPixels(int* p_pixel) {
    pixels = p_pixel;
}

int Image::getPixelEffect(int a) {
    return pixels[a];
}

void Image::setPixelEffect(int a, int rgb) {
    pixels[a] = rgb;
}