CC = g++

GL_FLAGS = -lglut -lGLEW -lGL -lGLU

UNNECCESARY_DEBUG = -Wall -Wextra -pedantic

all: build

build: spindle.cc
	$(CC) spindle.cc -o spindle $(GL_FLAGS)
