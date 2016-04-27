all: HT MT MOVELOG

MT: mt.o Node.o
	g++ mt.o Node.o -lm -o MT

HT: ht.o Node.o
	g++ ht.o Node.o -lm -o HT

MOVELOG: node_loc_log.o Node.o
	g++ node_loc_log.o Node.o -lm -o MOVELOG

INTERMEET: intermeet.o Node.o
	g++ intermeet.o Node.o -lm -o INTERMEET

node_loc_log.o: node_loc_log.cpp
	g++ -Wall node_loc_log.cpp -c

intermeet.o: intermeet.cpp
	g++ -Wall intermeet.cpp -c
mt.o: mt.cpp
	g++ -Wall mt.cpp -c
ht.o: ht.cpp
	g++ -Wall ht.cpp -c

Node.o: Node.cpp Node.h
	g++ -Wall Node.cpp -c

clean:
	rm -f *.o core HT MT MOVELOG

