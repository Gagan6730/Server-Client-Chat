all:
	gcc -o server server_16037.c
	gcc -o client client_16037.c

clean:
	rm -f client server