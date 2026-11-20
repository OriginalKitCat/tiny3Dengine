#include <stdio.h>
#include <SDL2/SDL.h>

#define outputHeight 100
#define outputWidth 100
#define SCALE 6

typedef struct {
    unsigned char r, g, b;
} Color;

Color img[outputHeight * outputWidth];

Color black = {0, 0, 0};

typedef struct
{
    float x, y;
} Point2D;

typedef struct
{
    float x, y, z;
} Point3D;


// unsigned char img[outputHeight * outputWidth * 3];

void clearScreen(void) {
    for (int i = 0; i < outputWidth * outputHeight; i++) img[i] = (Color){0, 0, 0};
}

void changePixel(int x, int y, Color c) {
    img[y * outputWidth + x] = c;
}

void changePixelPoint2D(Point2D p, Color c) {
    changePixel(p.x, p.y, c);
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

Point2D OnScreen(Point2D p) {
    return {
        (p.x + 1) * outputWidth / 2,
        (1 - p.y) * outputHeight / 2
    };
}

Point2D Project3D(Point3D p) {
    return {
        p.x / p.z,
        p.y / p.z
    };
}

void setup(void) {
    clearScreen();
    // drawBresenhamLine(1, 1, 90, 12);
    changePixelPoint2D(OnScreen(Project3D({0, 0.5, 3})), {255, 255, 255});
}

void update(float t) {
    clearScreen();
    float dt = 0;
    Point3D p = {0, 0.5, 1 + t};
    changePixelPoint2D(OnScreen(Project3D(p)), (Color){100, 255, 50});
}



int main(int argc, char **argv) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *win = SDL_CreateWindow("renderer",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        outputWidth * SCALE, outputHeight * SCALE, 0);
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_Texture *tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGB24,
        SDL_TEXTUREACCESS_STREAMING, outputWidth, outputHeight);

    int running = 1;
    Uint64 start = SDL_GetPerformanceCounter();
    Uint64 freq = SDL_GetPerformanceFrequency();

    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = 0;
        }

        float t = (float)(SDL_GetPerformanceCounter() - start) / freq;
        update(t);

        SDL_UpdateTexture(tex, NULL, img, outputWidth * sizeof(Color));
        SDL_RenderClear(ren);
        SDL_RenderCopy(ren, tex, NULL, NULL);  // stretches 100x100 to the window
        SDL_RenderPresent(ren);                // vsync paces this to the monitor rate
    }

    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}