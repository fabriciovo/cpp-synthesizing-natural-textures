#include <math.h>
#include <iostream>
#include <fstream>

#include "Image.h"

int min(int a, int b) {
    return b < a ? b : a;
}

int max(int a, int b) {
    return b > a ? b : a;
}

Image * readImage(std::string imgName) {
    std::ifstream arqE;
    arqE.open(imgName + ".ppm");
    char buffer[50];
    int r, g, b;
    int width = 255, height = 255;
    int transp;

    arqE.getline(buffer, 50);
    arqE >> width;
    arqE >> height;

    arqE >> transp;
    Image* img = new Image(height, width);
    for (int i = height - 1; i >= 0; i--)
    {
        for (int j = 0; j < width; j++)
        {
            arqE >> r;
            arqE >> g;
            arqE >> b;
            img->setPixel(r, g, b, j, i);
        }
    }
    arqE.close();
    return img;
}

void save(Image* image, std::string name)
{
    int temp, r, g, b;
    std::ofstream arqS;
    arqS.open(name + ".ppm");
    arqS << "P3" << std::endl;
    arqS << image->getWidth();
    arqS << " ";
    arqS << image->getHeight();
    arqS << std::endl;
    arqS << 255 << std::endl;
    for (int i = image->getHeight() - 1; i >= 0; i--)
    {
        for (int j = 0; j < image->getWidth(); j++)
        {
            temp = image->getPixel(j, i);
            r = temp >> 16 & 255;
            g = temp >> 8 & 255;
            b = temp & 255;
            arqS << r << " ";
            arqS << g << " ";
            arqS << b << "\n";
        }
    }
    arqS.close();
}

double dist(int c0, int c1) {
    int r1 = (c0 >> 16) & 0xff;
    int g1 = (c0 >> 8) & 0xff;
    int b1 = c0 & 0xff;

    int r2 = (c1 >> 16) & 0xff;
    int g2 = (c1 >> 8) & 0xff;
    int b2 = c1 & 0xff;

    int r = r1 - r2;
    int g = g1 - g2;
    int b = b1 - b2;

    double ret = sqrt(r * r + g * g + b * b);
    return ret;
}

int neighbor(Image* s, int sx, int sy, int x, int y, int fW, int fH) {
    int w = s->getWidth();
    int h = s->getHeight();
    int imageX = (sx + x + w) % w;
    int imageY = (sy + y + h) % h;
    int cor = s->getPixel(imageX, imageY);
    return cor;
}

int main()
{
    Image* sample = readImage("sample32");
    Image* result = readImage("preResult64");


    int s = 0, r = 0;
    int fW = 11;
    int fH = 11;
    int n2 = 11 / 2;
    int px = 0, py = 0;
    for (int ry = 0; ry < result->getHeight(); ry++) {
        for (int rx = 0; rx < result->getWidth(); rx++) {
            double dmin = 999999;
            for (int sy = 0; sy < sample->getHeight(); sy++) {
                for (int sx = 0; sx < sample->getWidth(); sx++) {
                    double d = 0;
                    for (int y = -n2; y < n2; y++) {
                        for (int x = -n2; x < n2; x++) {
                            s = neighbor(sample, sx, sy, x, y, fW, fH);
                            r = neighbor(result, rx, ry, x, y, fW, fH);
                            d += dist(r, s);
                        }
                    }
                    if (d < dmin) {
                        px = sx;
                        py = sy;
                        dmin = d;
                    }
                }
            }
            result->setPixelP(sample->getPixel(px, py), rx, ry);
        }
        int per = ry * 100 / result->getHeight();
        std::cout << per << " %" << std::endl;
    }
    save(result, "output");
    return 0;
}
