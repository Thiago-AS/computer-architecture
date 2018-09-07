HEADERS = mem.h
OBJECTS = mem.o main.o

%.o: %.c $(HEADERS)
	gcc -c $< -o $@

mem_mips: $(OBJECTS)
	gcc $(OBJECTS) -o $@

clean:
	-rm -f $(OBJECTS)
	-rm -f mem_mips
