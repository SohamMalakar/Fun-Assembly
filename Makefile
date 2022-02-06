
CPP=g++

CPPFLAGS=-Ofast -std=c++17

%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c $^

fasml: *.o
	$(CPP) $(CPPFLAGS) $^ -o $@

clean:
	rm *.o fasml

install:
	make && sudo cp -f fasml /usr/local/bin/ && make clean
