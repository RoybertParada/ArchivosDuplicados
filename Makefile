md5	: mddriver.c md5c.o global.h
	gcc -O2 -o md5 mddriver.c md5c.o

md5lib.a: md5c.o
	ar -r libmd5.a md5c.o

md5c.o	: md5c.c md5.h global.h
	gcc -O2 -c md5c.c

