./generator_ga:
	g++ -O2 -std=gnu++11 *.cpp -o generator_ga

debug:
	g++ -g *.cpp -o generator_ga

clean:
	rm -f *.o generator_ga log.txt
