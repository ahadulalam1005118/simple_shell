MKDIR_P=mkdir -p
SOURCE=*.cpp
OUTPUT_DIR=output
OUTPUT=$(OUTPUT_DIR)
SERVER_LIBRARIES=
INCLUDES=
CXX=g++
CXXFLAGS=-Wall -ansi -pedantic -std=c++11
all:
	$(MKDIR_P) $(OUTPUT_DIR)
	$(CXX) $(CXXFLAGS) $(SOURCE) -o $(OUTPUT)/myls
clean:
	rm -rf $(OUTPUT)
