#ifndef bus_h
#define bus_h
#include <sys/ipc.h>
#include <malloc.h>
#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/shm.h>  
#include <string.h>
#include <errno.h>

#define PORT_CAPACITY 3
#define MAX_NUM_BUSES 10
#define	TABLE_KEY 3030

//int msg_count =0;
struct busmessage
{
	char data[50];
};

struct Dir
{
	struct busmessage msg[PORT_CAPACITY]; 
	pid_t attached_process_id;
	int count;
	int location;
};

struct bus
{
	key_t key;
	int id;
	struct Dir *d[2];
};

struct table
{
	struct bus *b;
	struct table *next;
	struct table *head;
	struct table *tail;
	int bus_count;
};

int busget(key_t ,int );

int busattach(int , int );

int buswrite (int , struct busmessage );

int busread(int , struct busmessage *);

int buspush(int );

int busclose(int );

int readbus(int );

int buscount(int , int );

int detach(int );

int busdestroy(key_t );
#endif
