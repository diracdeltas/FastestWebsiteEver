all: server/index.html server/cpkthttp

server/index.html: client/index.html
	sed 's/^\s*//g' client/index.html | tr -d '\n' > server/index.html.stripped
	zopfli --i1000 -c --deflate server/index.html.stripped > server/index.html

server/cpkthttp: server/c/main.c
	gcc -O2 -o server/cpkthttp server/c/main.c

clean:
	rm server/index.html
	rm server/cpkthttp
