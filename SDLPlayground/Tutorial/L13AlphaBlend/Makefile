FILENAME = main.c
EXENAME = L13
TEST = test.c
LIBFILES = lib/*.o


install:
	cc $(FILENAME) $(LIBFILES) -o $(EXENAME) -O1 -Wall -Wno-missing-braces -w -lSDL2 -lSDL2_image

debug:
	cc -g $(FILENAME) -o $(EXENAME) -O1 -Wall -Wno-missing-braces -w -lSDL2 -lSDL2_image

test:
	cc $(TEST) $(LIBFILES) -o Test -O1 -Wall -Wno-missing-braces -w -lSDL2 -lSDL2_image

library:
	cc -c -g lib/init.c lib/texture.c
clean:
	rm $(EXENAME) 

remove:
	rm Test
