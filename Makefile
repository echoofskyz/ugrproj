SOURCE_FILES = ./src/*.c ./src/*.h ./src/*/*.c ./src/*/*.h

clean:
	@echo "Cleaning old files."
	@rm -rf ./build/*

build_win: clean
	@echo "Building for Windows."
	@x86_64-w64-mingw32-gcc -o ./build/app.exe ${SOURCE_FILES} -I./include_win -I./include -L./lib_win -lglfw3 -lopengl32 -lgdi32 -mwindows
	
build_win_console: clean
	@echo "Building for Windows with console."
	@x86_64-w64-mingw32-gcc -o ./build/app.exe ${SOURCE_FILES} -I./include_win -I./include -L./lib_win -lglfw3 -lopengl32 -lgdi32
	
build_linux: clean
	@echo "Building for Linux."
	@gcc -o ./build/app ${SOURCE_FILES} -I./include_linux -I./include -lGL -lglfw -lrt -lm -ldl -lX11
	
test_all: test_VPLists

test_VPLists:
	@echo "Testing VPLists"
	@gcc -o ./build/test_list ./tests/test_VPLists.c ./src/collections/List.* ./src/collections/VPLists.*
	@./build/test_list
	
.PHONY: clean build_win build_win_console build_linux test_all test_VPLists
