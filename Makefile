
FLAGS= -ggdb -fsanitize=address
SW_RL_FLAGS= -DRLGL_SW -ggdb -fsanitize=address -O3 -I. -DPLATFORM_DESKTOP
GL_RL_FLAGS= -ggdb -fsanitize=address -O3 -I. -DPLATFORM_DESKTOP


build/case0.png: build/tests_sw
	./build/tests_sw
	imv output/case*.png

build/tests_gl: tests.c build/raylib.gl.a common.h
	gcc $(FLAGS) -o build/tests_gl tests.c build/raylib.gl.a -lglfw -lm

build/tests_sw: tests.c build/simp.o build/raylib.sw.a common.h
	gcc $(FLAGS) -o build/tests_sw tests.c build/simp.o build/raylib.a -lm

build/simp.o: simp.c simp_math.h common.h
	gcc $(FLAGS) -c -o build/simp.o simp.c


RL_SRC= external/raylib/rcore.c external/raylib/rmodels.c external/raylib/rshapes.c external/raylib/rtext.c external/raylib/rtextures.c external/raylib/utils.c external/raylib/raudio.c
RL_SW_OBJS= $(patsubst external/raylib/%.c, build/%.sw.o, $(RL_SRC))
RL_GL_OBJS= $(patsubst external/raylib/%.c, build/%.gl.o, $(RL_SRC))

build/raylib.gl.a: $(RL_GL_OBJS)
	ar rcs $@ $^

build/raylib.sw.a: $(RL_SW_OBJS)
	ar rcs $@ $^

build/%.sw.o: external/raylib/%.c
	gcc $(SW_RL_FLAGS) -c -o $@ $<

build/%.gl.o: external/raylib/%.c
	gcc $(GL_RL_FLAGS) -c -o $@ $<

