FILENAME = main.c
EXENAME = Test
INCLUDE_FILES = ./include/player.c ./include/mapRender.c ./include/quadtree.c
INCLUDE_DIR = -I./include/
LIBRARIES = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -ltmx -lxml2 -lz
LIBRARY_PATHS = -L../tmx_1.10.0/lib/

install:
	cc $(FILENAME) $(INCLUDE_FILES) -o $(EXENAME) -g -O1 -Wall -std=c99 -Wno-missing-braces $(INCLUDE_DIR) $(LIBRARIES) $(LIBRARY_PATHS)
