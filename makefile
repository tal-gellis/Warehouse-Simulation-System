all: clean compile link run

link: compile
	g++ -o bin/warehouse bin/main.o bin/Order.o bin/customer.o bin/volunteer.o bin/WareHouse.o bin/Action.o

compile:
	g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -c -o bin/main.o src/main.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -c -o bin/Order.o src/Order.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -c -o bin/customer.o src/customer.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -c -o bin/volunteer.o src/Volunteer.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -c -o bin/WareHouse.o src/WareHouse.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -c -o bin/Action.o src/Action.cpp

clean:
	rm -f bin/*

run:
	./bin/warehouse