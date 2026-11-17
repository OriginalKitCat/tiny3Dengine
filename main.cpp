#include <stdio.h>

#define outputHeight 100
#define outputWidth 100

unsigned char img[outputHeight * outputWidth * 3];


void setup(void) {
    for (int i = 0; i < outputWidth * outputWidth * 3; i++) img[i] = 0;
}

int main(void) {
    setup();

    FILE *f = fopen("out.ppm", "wb");
    fprintf(f, "P6\n%d %d\n255\n", outputWidth, outputHeight);
    fwrite(img, 1, outputWidth * outputHeight * 3, f);
    fclose(f);
    return 0;
}