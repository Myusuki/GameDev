FILENAME = main.c
EXENAME = L16
TEST = test.c
LIBFILES = lib/*.o


install:
	cc $(FILENAME) $(LIBFILES) -o $(EXENAME) -O1 -Wall -Wno-missing-braces -w -lSDL2 -lSDL2_image -lSDL2_ttf

debug:
	cc -g $(FILENAME) $(LIBFILES) -o $(EXENAME) -O1 -Wall -Wno-missing-braces -w -lSDL2 -lSDL2_image -lSDL2_ttf

test:
	cc $(TEST) $(LIBFILES) -o Test -O1 -Wall -Wno-missing-braces -w -lSDL2 -lSDL2_image -lSDL2_ttf

library:
	cc -c -g lib/init.c lib/texture.c
clean:
	rm $(EXENAME) 

remove:
	rm Test
