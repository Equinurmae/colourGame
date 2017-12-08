// You do not need to change this file.
// The display module provides graphics for the sketch program.

// A display structure needs to be created by calling newDisplay, and then
// needs to be passed to each sketching function.
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

struct display;
typedef struct display display;

// Create a display object representing a plain white window of a given size.
display *newDisplay(char *title, int width, int height);

// Change the drawing colour.
void colour(display *d, int rgba);

// Pause for the given number of milliseconds.
void pause(display *d, int ms);

// Clear the display to white.
void clear(display *d);

// Wait for a key press.
char key(display *d);

void displayScore(display *d, char *string, SDL_Color color);

void displayText(display *d, char *string, SDL_Color color);

// Hold the display for a few seconds, then shut down.
void end(display *d);
