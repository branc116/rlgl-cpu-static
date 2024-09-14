
FLAGS= -ggdb -fsanitize=address
RL_FLAGS= -ggdb -fsanitize=address -O3 -I. -DPLATFORM_DESKTOP

build/case0.png: build/tests
	./build/tests
	imv output/case*.png

build/tests: tests.c build/simp.o build/raylib.a common.h
	gcc $(FLAGS) -o build/tests tests.c build/simp.o build/raylib.a -lm

build/simp.o: simp.c simp_math.h common.h
	gcc $(FLAGS) -c -o build/simp.o simp.c

build/raylib.a: build/rcore.o build/rmodels.o build/rshapes.o build/rtext.o build/rtextures.o build/utils.o
	ar rcs $@ $^

build/rcore.o: external/raylib/rcore.c
	gcc $(RL_FLAGS) -c -o build/rcore.o external/raylib/rcore.c

build/rmodels.o: external/raylib/rmodels.c
	gcc $(RL_FLAGS) -c -o build/rmodels.o external/raylib/rmodels.c

build/rshapes.o: external/raylib/rshapes.c
	gcc $(RL_FLAGS) -c -o build/rshapes.o external/raylib/rshapes.c

build/rtext.o: external/raylib/rtext.c
	gcc $(RL_FLAGS) -c -o build/rtext.o external/raylib/rtext.c

build/rtextures.o: external/raylib/rtextures.c
	gcc $(RL_FLAGS) -c -o build/rtextures.o external/raylib/rtextures.c

build/utils.o: external/raylib/utils.c
	gcc $(RL_FLAGS) -c -o build/utils.o external/raylib/utils.c
