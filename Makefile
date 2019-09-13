all: client server

client: client.o
	g++ -g -Wall -Wextra -Werror client.cpp -lstdc++ -o client
	
server: server.o
	g++ -g -Wall -Wextra -Werror server.cpp -lstdc++ -o server
	
clean:
	\rm *.o client server
