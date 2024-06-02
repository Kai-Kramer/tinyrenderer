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
  for (int i = 3000000; i != 0; --i) {
    if (i <= 100)
      line(0, 0, 100 - i, 0 + i, image, TGAColor(0, 2.25f * i, 0, 255));
    else if (i <= 200)
      line(200 - i, i - 100, 100, 100, image,
           TGAColor(255 - 2.25f * (i - 100), 0, 0, 255));
    else
      line(100 * ((i % 4) == 0), 100 * ((i % 4) == 3), 100 * ((i % 4) == 2),
           100 * ((i % 4) == 1), image, TGAColor(white));
  }
  image.write_tga_file("output.tga");
  return 0;
}

// line goes in as point-point form, and gets processed parametrically wrt x
void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
  bool steep = false;
  // "if slope > 1"
  if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
    std::swap(x0, y0);
    std::swap(x1, y1);
    steep = true;
  }
  // ensure larger value comes later--but this flips the image! (reflection
  // along y=x)
  if (x0 > x1) {
    std::swap(x0, x1);
    std::swap(y0, y1);
  }

  for (int x = x0; x <= x1; x++) {
    double t = (x - x0) / (double)(x1 - x0);
    int y = y0 * (1. - t) + y1 * t;
    if (steep) // un-reflect it to make it right-side-up
      image.set(y, x, color);
    else
      image.set(x, y, color);
  }
}
