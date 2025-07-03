FILENAME = main.c
EXENAME = app
INCLUDE_FILES = ./include/player.c ./include/mapRender.c
INCLUDE_DIR = -I./include/
LIBRARIES = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -ltmx -lxml2 -lz
LIBRARY_PATHS = -L../tmx_1.10.0/lib/

install:
	cc $(FILENAME) $(INCLUDE_FILES) -o $(EXENAME) -O1 -Wall -Wno-missing-braces $(INCLUDE_DIR) $(LIBRARIES) $(LIBRARY_PATHS)
debug:
	cc $(FILENAME) $(INCLUDE_FILES) -o $(EXENAME) -g -Wall -Wno-missing-braces $(INCLUDE_DIR) $(LIBRARIES) $(LIBRARY_PATHS)
