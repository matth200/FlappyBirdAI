.PHONY: all clean clean-all

GXX=g++

SRC_FOLDER=src
BIN_FOLDER=bin
APPNAME=app

FLAGS=-lSDL -lSDL_ttf

all: $(BIN_FOLDER)/$(APPNAME)

$(BIN_FOLDER)/$(APPNAME): $(BIN_FOLDER)/main.o $(BIN_FOLDER)/m_learning.o $(BIN_FOLDER)/bird.o $(BIN_FOLDER)/draw.o $(BIN_FOLDER)/world.o  $(BIN_FOLDER)/wall.o
	$(GXX) -o $@ $^ $(FLAGS)

$(BIN_FOLDER)/main.o: main.cpp
	$(GXX) -o $@ -c $< $(FLAGS)

$(BIN_FOLDER)/m_learning.o: $(SRC_FOLDER)/m_learning.cpp $(SRC_FOLDER)/m_learning.h
	$(GXX) -o $@ -c $< $(FLAGS)

$(BIN_FOLDER)/bird.o: $(SRC_FOLDER)/bird.cpp $(SRC_FOLDER)/bird.h
	$(GXX) -o $@ -c $< $(FLAGS)

$(BIN_FOLDER)/draw.o: $(SRC_FOLDER)/draw.cpp $(SRC_FOLDER)/draw.h
	$(GXX) -o $@ -c $< $(FLAGS)

$(BIN_FOLDER)/world.o: $(SRC_FOLDER)/world.cpp $(SRC_FOLDER)/world.h
	$(GXX) -o $@ -c $< $(FLAGS)

$(BIN_FOLDER)/wall.o: $(SRC_FOLDER)/wall.cpp $(SRC_FOLDER)/wall.h
	$(GXX) -o $@ -c $< $(FLAGS)

clean:
	rm -f $(BIN_FOLDER)/*.o

clean-all: clean
	rm -f $(BIN_FOLDER)/$(APPNAME)