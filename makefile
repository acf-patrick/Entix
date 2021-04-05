.SUFFIXE:

CXX = g++-8
CFLAGS = -std=c++2a -W

ECS = core/ecs/component/componentManager.cpp core/ecs/entity/entity.cpp
CLIENT = main.cpp

HEADER = core/
LIB = libs/

SRC = $(ECS) $(CLIENT)

all : obj
	@echo "... Linkage ..."
	$(CXX) obj/* -L $(LIB) -ltmx -lyaml-cpp -o bin

obj: $(SRC)
	@echo "... Compiling ..."
	@mkdir -p obj
	$(CXX) $(CFLAGS) -I $(HEADER) -c $?
	@mv *.o obj/

clean:
	@rm -f bin -r obj/