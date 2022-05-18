all: path-step1 path-step2 path-step3 path-step4

path-step1: main1.cpp
	g++ -g -o path-step1 main1.cpp

path-step2: main2.cpp
	g++ -g -o path-step2 main2.cpp

path-step3: main3.cpp
	g++ -g -o path-step3 main3.cpp

path-step4: main4.cpp
	g++ -g -o path-step4 main4.cpp

clean:
	rm path-step1 path-step2 path-step3 path-step4
