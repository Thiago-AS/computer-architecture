HEADERS = mips.hpp
OBJECTS = mips.o main.o

%.o: %.c $(HEADERS)
	g++ -std=c++11 -c $< -o $@

iss_mips: $(OBJECTS)
	g++ -std=c++11 $(OBJECTS) -o $@

clean:
	-rm -f $(OBJECTS)
	-rm -f iss_mips
