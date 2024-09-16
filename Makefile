CONFIG:=DEBUG



ifeq ($(CONFIG), DEBUG)
	FLAGS= -ggdb -fsanitize=address
	SW_RL_FLAGS= -DRLGL_SW -ggdb -fsanitize=address -O3 -I. -DPLATFORM_DESKTOP
	GL_RL_FLAGS= -ggdb -fsanitize=address -O3 -I. -DPLATFORM_DESKTOP
	BUILD_DIR=build
else ifeq ($(CONFIG), RELEASE)
	FLAGS= -ggdb -O3
	SW_RL_FLAGS= -DRLGL_SW -O3 -I. -DPLATFORM_DESKTOP
	GL_RL_FLAGS= -O3 -I. -DPLATFORM_DESKTOP
	BUILD_DIR=build_release
else
	echo "CONFIG is not DEBUG|RELEASE"
	exit 1
endif

$(BUILD_DIR)/case0.png: $(BUILD_DIR)/tests_sw
	./$(BUILD_DIR)/tests_sw
	imv output/case*.png

$(BUILD_DIR)/tests_gl: tests.c $(BUILD_DIR)/raylib.gl.a common.h
	gcc $(FLAGS) -o $(BUILD_DIR)/tests_gl tests.c $(BUILD_DIR)/raylib.gl.a -lglfw -lm

$(BUILD_DIR)/tests_sw: tests.c $(BUILD_DIR)/simp.o $(BUILD_DIR)/raylib.sw.a common.h
	gcc $(FLAGS) -o $(BUILD_DIR)/tests_sw tests.c $(BUILD_DIR)/simp.o $(BUILD_DIR)/raylib.sw.a -lm

$(BUILD_DIR)/simp.o: simp.c simp_math.h common.h
	gcc $(FLAGS) -c -o $(BUILD_DIR)/simp.o simp.c


RL_SRC= external/raylib/rcore.c external/raylib/rmodels.c external/raylib/rshapes.c external/raylib/rtext.c external/raylib/rtextures.c external/raylib/utils.c external/raylib/raudio.c
RL_SW_OBJS= $(patsubst external/raylib/%.c, $(BUILD_DIR)/%.sw.o, $(RL_SRC))
RL_GL_OBJS= $(patsubst external/raylib/%.c, $(BUILD_DIR)/%.gl.o, $(RL_SRC))

$(BUILD_DIR)/raylib.gl.a: $(RL_GL_OBJS)
	ar rcs $@ $^

$(BUILD_DIR)/raylib.sw.a: $(RL_SW_OBJS)
	ar rcs $@ $^

$(BUILD_DIR)/%.sw.o: external/raylib/%.c
	gcc $(SW_RL_FLAGS) -c -o $@ $<

$(BUILD_DIR)/%.gl.o: external/raylib/%.c
	gcc $(GL_RL_FLAGS) -c -o $@ $<

