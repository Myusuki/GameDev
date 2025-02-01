FILENAME = main.c
EXENAME = InputHandling
LIBRARIES = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

install:
	cc $(FILENAME) -o $(EXENAME) -O1 -Wall -std=c99 -Wno-missing-braces $(LIBRARIES)

clean:
	rm $(EXENAME)

test:
	cc test.c -o Test -O1 -Wall -std=c99 -Wno-missing-braces $(LIBRARIES)

cleanTest:
	rm Test
