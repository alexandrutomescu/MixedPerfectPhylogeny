CC = g++
FILES = main.cpp utils.cpp OptionParser.cpp heuristic.cpp poly_alg.cpp draw_tree.cpp flow.cpp
OUT_EXE = ../mixedphylogeny
PATH_TO_LEMON = ../lemon_binaries_linux/include/

CPPFLAGS = -std=c++11 -O3 -I $(PATH_TO_LEMON)

all:
	cd src; $(CC) $(FILES) $(CPPFLAGS) -o $(OUT_EXE) 

clean:
	rm -rf $(OUT_EXE)