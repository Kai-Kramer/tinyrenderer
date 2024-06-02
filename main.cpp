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
                    //  line(100, 100, 50, 0, image, TGAColor(red));
                    //  line(100, 100, 0, 50, image, TGAColor(blue));
                    //  line(0, 0, 50, 100, image, TGAColor(yellow));
                    //  line(0, 0, 100, 50, image, TGAColor(green));
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
// all computations done on cpu.
// -O3 time ~1.36 seconds for 3 million lines
//
// optimization 1: eliminate division (and therefore FP rounding error) by
// approximating the line. 'error' is found as dy/dx (slope) and is tallied on a
// per-pixel basis.
//-O3 time ~0.91 seconds for 3 million lines
//
// optimization 2: using the approximation method above is looking for error +/-
// 0.5 pixels. This is actually just a ratio of dy/dx, so I can just fully
// eliminate fp values by comparing dy and dx directly.
// -O3 time ~0.65 seconds for 3 million lines
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
  int dx = x1 - x0;
  int dy = y1 - y0;
  int derror = std::abs(dy * 2);
  int error = 0;
  int y = y0;
  int increment = (y1 > y0 ? 1 : -1);
  // for-loop is duplicated to eliminate branching
  // old version (pre mathematic optimization)
  /*
  if (steep) {
    for (int x = x0; x <= x1; x++) {
      double t = (x - x0) / (double)(x1 - x0);
      int y = y0 * (1. - t) + y1 * t;
      image.set(y, x, color);
    }
  } else {
    for (int x = x0; x <= x1; x++) {
      double t = (x - x0) / (double)(x1 - x0);
      int y = y0 * (1. - t) + y1 * t;
      image.set(x, y, color);
    }
  }*/
  if (steep) {
    for (int x = x0; x <= x1; ++x) {
      image.set(y, x, color);
      error += derror;
      if (error > dx) {
        y += increment;
        error -= dx * 2;
      }
    }
  } else {
    for (int x = x0; x <= x1; ++x) {
      image.set(x, y, color);
      error += derror;
      if (error > dx) {
        y += increment;
        error -= dx * 2;
      }
    }
  }
}
