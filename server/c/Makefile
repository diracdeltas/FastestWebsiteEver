all: server server-deflate

server-deflate:main.c
	${CC} -o $@ $< -DDEFLATE -lz

server:main.c
	${CC} -o $@ $< 

.PHONY:
clean:
	rm server server-deflate
