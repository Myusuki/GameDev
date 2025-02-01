install:
	cc main.c -o Asteroid -O1 -Wall -std=c99 -Wno-missing-braces -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

debug:
	cc -g main.c -o Asteroid -O1 -Wall -std=c99 -Wno-missing-braces -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

clean:
	rm Asteroid

test:
	cc test.c -o Test -O1 -Wall -std=c99 -Wno-missing-braces -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

remove:
	rm Test
