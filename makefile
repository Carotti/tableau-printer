CFLAGS += -std=c++11 -Wall -I include/

bin/tableau-printer: src/main.cpp | include/tableau.hpp include/fraction.hpp bin/
	g++ $^ -o $@ $(CFLAGS)

bin/:
	mkdir -p $@

clean:
	rm -rf bin/
