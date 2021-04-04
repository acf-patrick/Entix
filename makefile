.SUFFIXE:

CXX = g++-8
CFLAGS = -std=c++2a -W

SRC = main.cpp component/componentManager.cpp entity/entity.cpp

all : obj
	@echo "... Linkage ..."
	$(CXX) obj/* -o bin

obj:
	@echo "... Compiling ..."
	@mkdir obj
	$(CXX) $(CFLAGS) -c $(SRC)
	@mv *.o obj/

clean:
	@rm -f bin -r obj/