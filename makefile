CC = g++
#CFLAGS = -Wall -g -std=c++11 -DUSE_GLEW
#INCLUDES = -I./ -I./GL -I./Glm
#LIBS = -lglew32 -lfreeglut -lopengl32  
#-lGLEW -lGLU -lglut -lGL 

#  Msys/MinGW
ifeq "$(OS)" "Windows_NT"
CFLAGS=-Wall -g -std=c++11 -DUSE_GLEW
INCLUDES = -I./ -I./GL -I./Glm
LIBS=-lfreeglut -lglew32 -lglu32 -lopengl32 -lm
CLEAN=rm -f *.exe *.o *.a
else
#  OSX
ifeq "$(shell uname)" "Darwin"
RES=$(shell uname -r|sed -E 's/(.).*/\1/'|tr 12 21)
INCLUDES = -I./ -I./GL -I./Glm
CFLG=-O3 -Wall -Wno-deprecated-declarations -DRES=$(RES)
LIBS=-framework GLUT -framework OpenGL
#  Linux/Unix/Solaris
else
CFLAGS=-Wall -g -std=c++11 -DUSE_GLEW
INCLUDES = -I./ -I./GL -I./Glm
LIBS= -lGLEW -lGLU -lglut -lGL 
endif

#  OSX/Linux/Unix/Solaris
CLEAN=rm -f $(EXE) *.o *.a
endif
# Source files
SRCS = main.cpp \
       light.cpp \
       model.cpp \
       scene.cpp \
       shader.cpp \
       utils.cpp \
       vertexbuffer.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Target executable
TARGET = final

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Dependencies
main.o: main.cpp
light.o: light.cpp light.h
model.o: model.cpp model.h
scene.o: scene.cpp scene.h
shader.o: shader.cpp shader.h
utils.o: utils.cpp utils.h
vertexbuffer.o: vertexbuffer.cpp vertexbuffer.h

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean