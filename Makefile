all: server/index.html server/cpkthttp

server/index.html: client/index.html
	zopfli client/index.html --i1000 -c --deflate > server/index.html

server/cpkthttp: server/c/main.c
	gcc -O2 -o server/cpkthttp server/c/main.c

clean:
	rm server/index.html
	rm server/cpkthttp
