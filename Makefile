# ---------- compiler & flags -----------------------------------------
CXX      = g++
CXXFLAGS = -std=c++20 -Wall -g -I/opt/X11/include       # add -I paths here if needed
LIBS     = -L/opt/X11/lib -lX11 -lXpm                      # -lXpm if you load XPM sprites (uncomment)

# ---------- source files ---------------------------------------------
SOURCES = \
    main.cc game.cc scoreboard.cc subject.cc \
    board.cc \
    player.cc humanplayer.cc computerplayer.cc \
    piece.cc king.cc queen.cc rook.cc bishop.cc knight.cc pawn.cc \
    textdisplay.cc graphicaldisplay.cc \
    level1.cc level2.cc level3.cc level4.cc

OBJS   = ${SOURCES:.cc=.o}
TARGET = chess

# ---------- default target -------------------------------------------
all: ${TARGET}

# ---------- link step -------------------------------------------------
${TARGET}: ${OBJS}
	${CXX} ${CXXFLAGS} ${OBJS} -o ${TARGET} ${LIBS}

# ---------- compile step ---------------------------------------------
%.o: %.cc
	${CXX} ${CXXFLAGS} -c $< -o $@

# ---------- clean -----------------------------------------------------
clean:
	rm -f ${OBJS} ${TARGET}
