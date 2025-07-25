#include "graphicaldisplay.h"
#include "board.h"
#include "piece.h"
#include <X11/Xutil.h>
#include <X11/xpm.h>
#include <iostream>
#include <string>

int spriteIndex(char sym) {
    switch (tolower(sym)) {
        case 'k': return 0;
        case 'q': return 1;
        case 'b': return 2;
        case 'n': return 3;
        case 'r': return 4;
        case 'p': return 5;
        default:  return -1;
    }
}


GraphicsDisplay::GraphicsDisplay(Board *b, int pixel) 
    : size(pixel), sq(size / 8), board(b) {
    
    // Initialize all pixmap arrays to 0
    for (int i = 0; i < 6; ++i) {
        whitePix[i] = 0;
        blackPix[i] = 0;
        whiteMask[i] = 0;
        blackMask[i] = 0;
    }
    
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
    do { XNextEvent(dpy, &e);} while (e.type != Expose);
    
    // Attach to board and draw initial state
    loadSprites();
    board->attach(this);
    notify();
}

GraphicsDisplay::~GraphicsDisplay() {
    // Detach from board first to avoid dangling pointer
    if (board) {
        board->detach(this);
    }
    
    for (int i = 0; i < 6; ++i) {
        if (whitePix[i]) XFreePixmap(dpy, whitePix[i]);
        if (blackPix[i]) XFreePixmap(dpy, blackPix[i]);
        if (whiteMask[i]) XFreePixmap(dpy, whiteMask[i]);
        if (blackMask[i]) XFreePixmap(dpy, blackMask[i]);
    }

    if (gc) XFreeGC(dpy, gc);
    if (win) XDestroyWindow(dpy, win);
    if (dpy) XCloseDisplay(dpy);
}


void GraphicsDisplay::loadSprites() {
    std::string whiteFiles[6] = {
        "sprites/wK.xpm", "sprites/wQ.xpm", "sprites/wB.xpm",
        "sprites/wN.xpm", "sprites/wR.xpm", "sprites/wP.xpm"
    };

    std::string blackFiles[6] = {
        "sprites/bK.xpm", "sprites/bQ.xpm", "sprites/bB.xpm",
        "sprites/bN.xpm", "sprites/bR.xpm", "sprites/bP.xpm"
    };

    for (int i = 0; i < 6; ++i) {
        XpmAttributes attr;
        attr.valuemask = XpmReturnPixels | XpmSize;

        if (XpmReadFileToPixmap(dpy, win, whiteFiles[i].c_str(), &whitePix[i], &whiteMask[i], &attr) == XpmSuccess) {
            spriteW = attr.width;
            spriteH = attr.height;
        } else {
            std::cerr << "Failed to load " << whiteFiles[i] << "\n";
            whitePix[i] = 0;
            whiteMask[i] = 0;
        }

        if (XpmReadFileToPixmap(dpy, win, blackFiles[i].c_str(), &blackPix[i], &blackMask[i], &attr) == XpmSuccess) {
            spriteW = attr.width;
            spriteH = attr.height;
        } else {
            std::cerr << "Failed to load " << blackFiles[i] << "\n";
            blackPix[i] = 0;
            blackMask[i] = 0;
        }
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

void GraphicsDisplay::drawSprite(int r, int c, char sym) {
    if (sym == ' ' || sym == '_') return;
    int index = spriteIndex(sym);
    if (index < 0) return;

    // Center the sprite in the square
    int x = c * sq + (sq - 64) / 2;
    int y = r * sq + (sq - 64) / 2;

    Pixmap sprite = isupper(sym) ? whitePix[index] : blackPix[index];

    if (sprite)
        XCopyArea(dpy, sprite, win, gc, 0, 0, 64, 64, x, y);
    else
        drawGlyph(r, c, sym);  // fallback to text
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
                drawSprite(r, c, symbol);
            }
        }
    }
    
    // Flush all drawing commands
    XFlush(dpy);
}

