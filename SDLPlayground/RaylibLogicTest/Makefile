FILENAME = main.c
EXENAME = Test

install:
	cc $(FILENAME) -o $(EXENAME) -O1 -Wall -Wno-missing-braces -w -lSDL2

debug:
	cc -g $(FILENAME) -o $(EXENAME) -O1 -Wall -Wno-missing-braces -w -lSDL2

clean:
	rm $(EXENAME) 
