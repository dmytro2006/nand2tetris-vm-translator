FLAGS = -Iheaders -std=c++23

translator.exe : compiler.o file.o preprocessor.o tokenizer.o translator.o
	g++ src/compiler.cpp src/file.cpp src/preprocessor.cpp src/tokenizer.cpp src/translator.cpp $(FLAGS) -o translator.exe

compiler.o : src/compiler.cpp
	g++ -c src/compiler.cpp $(FLAGS) -o src/compiler.o

file.o : src/file.cpp
	g++ -c src/file.cpp $(FLAGS) -o src/file.o

preprocessor.o : src/preprocessor.cpp
	g++ -c src/preprocessor.cpp $(FLAGS) -o src/preprocessor.o

tokenizer.o : src/tokenizer.cpp
	g++ -c src/tokenizer.cpp $(FLAGS) -o src/tokenizer.o

translator.o : src/translator.cpp
	g++ -c src/translator.cpp $(FLAGS) -o src/translator.o

clean:
	ifeq ($(OS), Windows_NT)
		echo "Clean is not supported in Windows, use rm src/*.o"
	else 
		rm src/*.o