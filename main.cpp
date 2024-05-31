#include "tgaimage.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor blue = TGAColor(0, 0, 255, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
const TGAColor yellow = TGAColor(255, 255, 0, 255);
void line(int, int, int, int, TGAImage &, TGAColor);

int main(int argc, char **argv) {
  TGAImage image(100, 100, TGAImage::RGB);
  line(0, 0, 100, 100, image,
       TGAColor(
           white)); // these two lines are "backward" and don't work, I guess...
  line(100, 100, 50, 0, image, TGAColor(red));
  line(100, 100, 0, 50, image, TGAColor(blue));
  line(0, 0, 50, 100, image, TGAColor(yellow));
  line(0, 0, 100, 50, image, TGAColor(green));
  image.write_tga_file("output.tga");
  return 0;
}

// line goes in as point-point form, and gets processed parametrically wrt x
void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
  // ensure larger value comes later
  if (x0 > x1) {
    std::swap(x0, x1);
    std::swap(y0, y1);
  }

  for (int x = x0; x < x1; ++x) {
    double t = (x - x0) / (double)(x1 - x0);
    int y = y1 * (1. - t) - y0 * t;
    image.set(y, x, color);
  }
}
