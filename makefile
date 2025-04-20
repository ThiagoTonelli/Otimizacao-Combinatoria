TARGET = programa
CXX = g++
CXXFLAGS = -O3 -march=native -flto -ffast-math -funroll-loops -fomit-frame-pointer -fprefetch-loop-arrays -lm -w

all: compila executa clean

compila:
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.cpp

executa:
	./$(TARGET)

clean:
	rm -f $(TARGET)


