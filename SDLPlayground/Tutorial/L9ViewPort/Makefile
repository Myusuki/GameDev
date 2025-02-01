FILENAME = main.c
EXENAME = L9
TEST = test.c

install:
	cc $(FILENAME) -o $(EXENAME) -O1 -Wall -Wno-missing-braces -w -lSDL2 -lSDL2_image

debug:
	cc -g $(FILENAME) -o $(EXENAME) -O1 -Wall -Wno-missing-braces -w -lSDL2 -lSDL2_image

test:
	cc $(TEST) -o Test -O1 -Wall -Wno-missing-braces -w -lSDL2 -lSDL2_image

clean:
	rm $(EXENAME) 

remove:
	rm Test
