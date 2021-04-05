.SUFFIXE:

CXX = g++-8
CFLAGS = -std=c++2a -W

CORE = core/ecs/component/componentManager.cpp\
 core/ecs/entity/entity.cpp\
 core/application/application.cpp\
 core/entry/entry.cpp
 
CLIENT = test.cpp

HEADER = core/
LIB = libs/

SRC = $(CORE) $(CLIENT)

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