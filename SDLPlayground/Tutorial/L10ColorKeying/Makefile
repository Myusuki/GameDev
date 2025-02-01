FILENAME = main.cpp
EXENAME = L10
TEST = test.c

install:
	g++ $(FILENAME) -o $(EXENAME) -O1 -Wall -Wno-missing-braces -w -lSDL2 -lSDL2_image

debug:
	g++ -g $(FILENAME) -o $(EXENAME) -O1 -Wall -Wno-missing-braces -w -lSDL2 -lSDL2_image

test:
	cc $(TEST) -g -o Test -O1 -Wall -Wno-missing-braces -w -lSDL2 -lSDL2_image

clean:
	rm $(EXENAME) 

remove:
	rm Test
