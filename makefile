.SUFFIXE:

CXX 	= 	g++
CFLAGS 	= 	-std=c++2a -W

CORE 	= 	core/ecs/component/manager.cpp\
			core/ecs/entity/entity.cpp\
			core/ecs/entity/container.cpp\
			core/ecs/components.cpp\
			core/application/application.cpp\
			core/entry/entry.cpp\
			core/scene/scene.cpp\
			core/renderer/renderer.cpp\
			core/event/event.cpp\
			core/event/listner.cpp\
			core/util/blur.cpp
 
CLIENT 	= 	test.cpp

HEADER 	=  	core/
LIBS 	= 	libs/

SDL 	= 	-lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
# YAML 	= 	-lyaml-cpp
# TMX 	= 	-lxml2 -lz -ltmx
# BOX2D =	-lbox2d

SRC 	= 	$(CORE) $(CLIENT)

ifeq ($(OS), Windows_NT)
	# fill
else
	L = $(SDL) -L$(LIBS)linux/ $(TMX) $(YAML)
endif

all : obj
	@echo "... Linkage ..."
	$(CXX) obj/* $(L) -o bin

obj: $(SRC)
	@echo "... Compiling ..."
	@mkdir -p obj
	$(CXX) $(CFLAGS) -I $(HEADER) -c $?
	@mv *.o obj/

clean:
	@rm -f bin *.o -r obj/
