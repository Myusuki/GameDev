install:
	cc main.c -o Snake -O1 -Wall -std=c99 -Wno-missing-braces -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

debug:
	cc -g main.c -o Snake -O1 -Wall -std=c99 -Wno-missing-braces -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

clean:
	rm Snake

test:
	cc test.c -o Test -O1 -Wall -std=c99 -Wno-missing-braces -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

remove:
	rm Test
