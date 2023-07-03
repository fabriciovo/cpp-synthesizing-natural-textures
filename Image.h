#ifndef IMAGE_H
#define IMAGE_H

#include <cmath>
#include <string>
#include <vector>

class Image {
public:
	Image();
	Image(int h, int w);
	int getHeight();
	int getWidth();
	void setPixelP(int rgb, int x, int y);
	void setPixel(int r, int g, int b, int x, int y);
	int getPixel(int x, int y);
	int* getPixels();
	void setPixels(int* p_pixel);
	int getPixelEffect(int a);
	void setPixelEffect(int a, int rgb);

private:
	int* pixels;
    int width, height;
};
#endif /* IMAGE_H */

