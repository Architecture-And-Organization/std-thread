CXXFLAGS+=-Wall -Werror -std=c++11 -O -g
CXXFLAGS+=-pthread

PROGS=thread1 thread1a thread2 thread3 thread4 thread5 reduce
all: $(PROGS)

clean:
	rm -f $(PROGS)

world: clean all
