CPPFLAGS = -Iheaders -std=c++23
objects = compiler.o file.o preprocessor.o tokenizer.o translator.o

translator.exe: $(objects)
	g++ $(objects) $(CPPFLAGS) -o translator.exe

$(objects): %.o: src/%.cpp
	g++ -c $< $(CPPFLAGS) -o $@

clean:
	ifeq ($(OS), Windows_NT)
		echo "Clean is not supported in Windows, use rm src/*.o"
	else 
		rm *.o