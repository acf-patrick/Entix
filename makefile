.SUFFIXE:

CXX = g++-8
CFLAGS = -std=c++2a -W

ECS = ecs/component/componentManager.cpp ecs/entity/entity.cpp
CLIENT = main.cpp

SRC = $(ECS) $(CLIENT)

all : obj
	@echo "... Linkage ..."
	$(CXX) obj/* -o bin

obj: $(SRC)
	@echo "... Compiling ..."
	@mkdir -p obj
	$(CXX) $(CFLAGS) -c $?
	@mv *.o obj/

clean:
	@rm -f bin -r obj/