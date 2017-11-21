CFLAGS += -std=c++11 -w -Wall -I include/

bin/tableau-printer: src/main.cpp | include/tableau.hpp include/fraction.hpp
	g++ $^ -o $@ $(CFLAGS)
