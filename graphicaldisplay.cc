#include "graphicaldisplay.h"
#include "board.h"
#include "piece.h"
#include <X11/Xutil.h>
#include <iostream>
#include <string>

GraphicsDisplay::GraphicsDisplay(Board *b, int pixel) 
    : size(pixel), sq(size / 8), board(b) {
    
    // Open connection to X server
    dpy = XOpenDisplay(nullptr);
    if (!dpy) {
        std::cerr << "Cannot open X display" << std::endl;
        return;
    }
    
    int screen = DefaultScreen(dpy);
    
    // Create window
    win = XCreateSimpleWindow(
        dpy, 
        RootWindow(dpy, screen),
        100, 100,           // x, y position
        size, size,         // width, height
        1,                  // border width
        BlackPixel(dpy, screen),  // border color
        WhitePixel(dpy, screen)   // background color
    );
    
    // Create graphics context
    gc = XCreateGC(dpy, win, 0, nullptr);
    
    // Set window properties
    XStoreName(dpy, win, "Chess Board");
    
    // Select input events
    XSelectInput(dpy, win, ExposureMask | KeyPressMask);
    
    // Map window (make it visible)
    XMapWindow(dpy, win);
    
    // Wait for window to be mapped
    XEvent e;
    do {
        XNextEvent(dpy, &e);
    } while (e.type != Expose);
    
    // Attach to board and draw initial state
    board->attach(this);
    notify();
}

GraphicsDisplay::~GraphicsDisplay() {
    if (dpy) {
        if (gc) XFreeGC(dpy, gc);
        if (win) XDestroyWindow(dpy, win);
        XCloseDisplay(dpy);
    }
}

void GraphicsDisplay::drawSquare(int r, int c, bool dark) {
    if (!dpy) return;
    
    int x = c * sq;
    int y = r * sq;
    
    // Set color based on square type
    int screen = DefaultScreen(dpy);
    if (dark) {
        XSetForeground(dpy, gc, BlackPixel(dpy, screen));
    } else {
        XSetForeground(dpy, gc, WhitePixel(dpy, screen));
    }
    
    // Fill rectangle
    XFillRectangle(dpy, win, gc, x, y, sq, sq);
    
    // Draw border
    XSetForeground(dpy, gc, BlackPixel(dpy, screen));
    XDrawRectangle(dpy, win, gc, x, y, sq, sq);
}

void GraphicsDisplay::drawGlyph(int r, int c, char sym) {
    if (!dpy || sym == ' ' || sym == '_') return;
    
    int x = c * sq + sq / 2;  // Center horizontally
    int y = r * sq + sq / 2;  // Center vertically
    
    int screen = DefaultScreen(dpy);
    
    // Set text color (opposite of background for visibility)
    bool dark_square = ((r + c) % 2 == 1);
    if (dark_square) {
        XSetForeground(dpy, gc, WhitePixel(dpy, screen));
    } else {
        XSetForeground(dpy, gc, BlackPixel(dpy, screen));
    }
    
    // Load a font
    Font font = XLoadFont(dpy, "fixed");
    XSetFont(dpy, gc, font);
    
    // Convert char to string
    std::string text(1, sym);
    
    // Get text dimensions for centering
    int text_width = XTextWidth(XQueryFont(dpy, font), text.c_str(), 1);
    
    // Adjust position for centering
    x -= text_width / 2;
    y += 5; // Small adjustment for vertical centering
    
    // Draw the character
    XDrawString(dpy, win, gc, x, y, text.c_str(), 1);
    
    XUnloadFont(dpy, font);
}

void GraphicsDisplay::notify() {
    if (!dpy) return;
    
    // Clear window
    XClearWindow(dpy, win);
    
    // Draw all squares
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            // Determine if square is dark
            bool dark = ((r + c) % 2 == 1);
            
            // Draw the square
            drawSquare(r, c, dark);
            
            // Get piece at this position
            const Piece* piece = board->getPieceAt(r, c);
            if (piece) {
                char symbol = piece->getSymbol();
                drawGlyph(r, c, symbol);
            }
        }
    }
    
    // Flush all drawing commands
    XFlush(dpy);
}

