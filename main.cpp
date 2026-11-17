#include <stdio.h>

#define outputHeight 100
#define outputWidth 100

typedef struct {
    unsigned char r, g, b;
} Color;

Color img[outputHeight * outputWidth];

// unsigned char img[outputHeight * outputWidth * 3];

void changePixel(int x, int y, Color c) {
    img[y * outputWidth + x] = c;
}

void drawBresenhamLine(int x0, int y0, int x1, int y1) { // Took me some youtube tutorials to figure out
    int dx = x1 - x0;
    int dy = y1 - y0;

    if (dx != 0) {
        int y = y0;
        int p = 2*dy - dx;
        for (int i = 0; i <= dx; i++) {
            changePixel(x0 + i, y, {255, 255, 255});
            if (p >= 0) {
                y += 1;
                p = p - 2*dx;
            }
            p = p + 2*dy;
        }
    }
} 


void setup(void) {
    for (int i = 0; i < outputWidth * outputWidth; i++) img[i] = (Color){0, 0, 0};
    drawBresenhamLine(1, 1, 90, 12);
}

int main(void) {
    setup();

    FILE *f = fopen("out.ppm", "wb");
    fprintf(f, "P6\n%d %d\n255\n", outputWidth, outputHeight);
    fwrite(img, 1, outputWidth * outputHeight * 3, f);
    fclose(f);
    return 0;

}