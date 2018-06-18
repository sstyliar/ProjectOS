
output: client.o server.o
	gcc client.c -o client -lncurses 
	gcc server.c -o server -pthread

client.o: client.c
	gcc -c client.c

server.o: server.c
	gcc -c server.c
	
runClient:
	./client 127.0.0.1 12345

runServer:
	gnome-terminal -x ./server 12345

clean:
	rm *.o client server maze*

full_clean:
	rm *.o client server *_stats users maze*