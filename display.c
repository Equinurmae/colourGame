/* A small graphics module for line drawing, based on SDL.  The functions
draw onto the window surface, and then call SDL_UpdateWindowSurface to display
what has been drawn onto the screen.  A software renderer is used, since
SDL's direct surface drawing functions don't include line drawing. */

#include "display.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct display {
    int width, height;
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;
};

// If SDL fails, print the SDL error message, and stop the program.
static void fail() {
    fprintf(stderr, "Error: %s\n", SDL_GetError());
    SDL_Quit();
    exit(1);
}

// Check return values from SDL functions.
void *notNull(void *p) { if (p == NULL) fail(); return p; }
int notNeg(int n) { if (n < 0) fail(); return n; }

// Create a new display object.
display *newDisplay(char *title, int width, int height) {
    display *d = malloc(sizeof(display));
    d->width = width;
    d->height = height;
    notNeg(SDL_Init(SDL_INIT_VIDEO));
    TTF_Init();
    int x = SDL_WINDOWPOS_UNDEFINED, y = SDL_WINDOWPOS_UNDEFINED;
    d->window = notNull(SDL_CreateWindow(title, x, y, width, height, 0));
    SDL_Surface *surface = notNull(SDL_GetWindowSurface(d->window));
    d->renderer = notNull(SDL_CreateSoftwareRenderer(surface));
    SDL_SetRenderDrawColor(d->renderer, 255, 255, 255, 255);
    SDL_RenderClear(d->renderer);
    SDL_UpdateWindowSurface(d->window);
    SDL_SetRenderDrawColor(d->renderer, 0, 0, 0, 255);
    return d;
}


void colour(display *d, int rgba) {
    int r = (rgba >> 24) & 0xFF;
    int g = (rgba >> 16) & 0xFF;
    int b = (rgba >> 8) & 0xFF;
    int a = rgba & 0xFF;
    notNeg(SDL_SetRenderDrawColor(d->renderer, r, g, b, a));
}

void clear(display *d) {
    SDL_SetRenderDrawColor(d->renderer, 255, 255, 255, 255);
    SDL_RenderClear(d->renderer);
    SDL_UpdateWindowSurface(d->window);
    SDL_SetRenderDrawColor(d->renderer, 0, 0, 0, 255);
}

void pause(display *d, int ms) {
    if (ms > 0) SDL_Delay(ms);
}

char key(display *d) {
    SDL_Event event_structure;
    SDL_Event *event = &event_structure;
    while (true) {
        int r = SDL_WaitEvent(event);
        if (r == 0) fail("Bad event", "");
        if (event->type == SDL_QUIT) {
            SDL_Quit();
            exit(0);
        }
        else if (event->type == SDL_KEYUP) {
            int sym = event->key.keysym.sym;
            return (char)sym;
        }
    }
}

void displayScore(display *d, char *string, SDL_Color color) {
    d->font = TTF_OpenFont("arial.ttf", 50);
    SDL_Surface * surface = TTF_RenderText_Blended_Wrapped(d->font, string, color, 1280);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(d->renderer, surface);
    int texW = 0;
    int texH = 0;
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
    SDL_Rect dstrect = { 0, 0, texW, texH };
    SDL_RenderCopy(d->renderer, texture, NULL, &dstrect);
    SDL_RenderPresent(d->renderer);
    SDL_UpdateWindowSurface(d->window);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void displayText(display *d, char *string, SDL_Color color) {
    int winW = 0, winH = 0;
    SDL_GetWindowSize(d->window, &winW, &winH);
    d->font = TTF_OpenFont("arial.ttf", 100);
    SDL_Surface * surface = TTF_RenderText_Blended_Wrapped(d->font, string, color, winW);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(d->renderer, surface);
    int texW = 0;
    int texH = 0;
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
    SDL_Rect dstrect = { (winW/2)-(texW/2), (winH/2)-(texH/2), texW, texH };
    SDL_RenderCopy(d->renderer, texture, NULL, &dstrect);
    SDL_RenderPresent(d->renderer);
    SDL_UpdateWindowSurface(d->window);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void end(display *d) {
    SDL_Delay(2000);
    TTF_CloseFont(d->font);
    TTF_Quit();
    SDL_Quit();
}
