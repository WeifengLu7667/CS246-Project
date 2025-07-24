# Define the C++ compiler to use
CXX = g++

# Define the C++ version and other compiler flags
# -std=c++20 enables C++20 features
# -Wall enables all common warnings
# -g adds debugging information
CXXFLAGS = -std=c++20 -Wall -g

# Define any libraries to link against (for graphics)
LIBS = -lX11

# List all of your .cc source files
SOURCES = main.cc game.cc scoreboard.cc subject.cc \
          board.cc state.cc move.cc posn.cc colour.cc castlinginfo.cc \
          player.cc humanplayer.cc computerplayer.cc \
          piece.cc king.cc queen.cc rook.cc bishop.cc knight.cc pawn.cc \
          textdisplay.cc graphicaldisplay.cc \
          level1.cc level2.cc level3.cc

# Automatically generate the names of the object files (.o) from the source files
OBJS = ${SOURCES:.cc=.o}

# The name of the final executable program
TARGET = chess

# The default rule that runs when you just type "make"
all: ${TARGET}

# Rule to link all the object files into the final executable
${TARGET}: ${OBJS}
    ${CXX} ${CXXFLAGS} ${OBJS} -o ${TARGET} ${LIBS}

# Rule to compile a .cc file into a .o file
%.o: %.cc
    ${CXX} ${CXXFLAGS} -c $< -o $@

# Rule to clean up all the compiled files
clean:
    rm -f ${OBJS} ${TARGET}
	