LINKER      = g++
LINKERFLAGS = -s
CXX         = g++
CXXFLAGS    = -O3 -Wall -std=c++14
BIN         = table-gen-for-main-parser
vpath %.o build
OBJ         = table-gen-for-main-parser.o char_conv.o
LINKOBJ     = build/table-gen-for-main-parser.o build/char_conv.o

.PHONY: all all-before all-after clean clean-custom

all: all-before $(BIN) all-after

clean: clean-custom 
	rm -f ./build/*.o
	rm -f ./build/$(BIN)

.cpp.o:
	$(CXX) -c $< -o $@ $(CXXFLAGS) 
	mv $@ ./build

$(BIN):$(OBJ)
	$(LINKER) -o $(BIN) $(LINKOBJ) $(LINKERFLAGS)
	mv $(BIN) ./build