build:
	g++ -I./Eigen/ -std=c++14 -O3 *.cpp -o raytracer
clean:
	rm *.o && rm raytracer
 
