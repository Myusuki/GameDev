FILENAME = main.c
EXENAME = Test
LIBRARIES = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -ltmx -lxml2 -lz
LIBRARY_PATHS = -L./tmx_1.10.0/lib/

install:
	cc $(FILENAME) -o $(EXENAME) -O1 -Wall -std=c99 -Wno-missing-braces -g $(LIBRARIES) $(LIBRARY_PATHS)
