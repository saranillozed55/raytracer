CXX = g++
CXXFLAGS = -std=c++20 -O2 -Iinclude

raytracer: main.cpp $(wildcard include/*.h)
	$(CXX) $(CXXFLAGS) main.cpp -o raytracer

run: raytracer
	./raytracer > image.ppm

clean:
	rm -f raytracer
	rm -f image.ppm