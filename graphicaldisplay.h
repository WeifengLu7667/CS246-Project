#ifndef GRAPHICALDISPLAY_H
#define GRAPHICALDISPLAY_H

#include <X11/Xlib.h>
#include "observer.h"

class Board;

class GraphicsDisplay : public Observer {
    Display *dpy{nullptr};
    Window win{0};
    GC gc{0};

    const int size; // pixel width  (e.g. 560)
    const int sq; // size / 8
    Board *board; // watched model

    void drawSquare(int r,int c,bool dark);
    void drawGlyph (int r,int c,char sym); // letters/overlays
    // (OPTIONAL) sprite support ------------------
    // Pixmap whitePix[6], blackPix[6]; int spriteW,spriteH;
    // void loadSprites(); void drawSprite(int r,int c,char sym);

public:
    explicit GraphicsDisplay(Board *b,int pixel=560);
    ~GraphicsDisplay() override;

    void notify() override; // redraw whole board
};

#endif