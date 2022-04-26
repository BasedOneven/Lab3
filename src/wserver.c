#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "request.h"
#include "queue.h"
#include "io_helper.h"

queue_t jobs;
pthread_cond_t fill = PTHREAD_COND_INITIALIZER;
pthread_mutex_t m   = PTHREAD_MUTEX_INITIALIZER;
char default_root[] = ".";

void *worker(void *arg) {
  while (1) {	
    pthread_mutex_lock(&m);
    while (Queue_Empty(&jobs))// if empty, no job to do
	{
		pthread_cond_wait(&fill, &m); // wait for jobs
	}

    int fd;
    Queue_Dequeue(&jobs, &fd);
    pthread_mutex_unlock(&m);

    request_handle(fd);
    close_or_die(fd);
  }
  return NULL;
}

// n workers
int thread_pool_init(int n) {
  assert(n > 0);
  Queue_Init(&jobs);
  pthread_t t;

  for (int i = 0; i < n; i++)
    pthread_create(&t, NULL, worker, NULL);

  return 1;
}

//
// ./wserver [-d <basedir>] [-p <portnum>] 
// 
int main(int argc, char *argv[]) {
    int c;
    char *root_dir = default_root;
    int port = 10000;
	int worker_threads = 5;

    while ((c = getopt(argc, argv, "d:p:t:b:s:")) != -1)
	switch (c) {
	case 'd':
	    root_dir = optarg;
		printf("d: %s\n", optarg);
	    break;
	case 'p':
	    port = atoi(optarg);
		printf("p: %s\n", optarg);
	    break;
	case 't':
		worker_threads = atoi(optarg);
		printf("t: %s\n", optarg);
		break;
	case 'b':
		printf("b: %s\n", optarg);
		break;
	case 's':
		printf("s: %s\n", optarg);
		break;
	default:
	    fprintf(stderr, "usage: wserver [-d basedir] [-p port] [-t threads] [-b buffers] [-s schedalg]\n");
	    exit(1);
	}

    // run out of this directory
    chdir_or_die(root_dir);

	// create worker thread
	thread_pool_init(worker_threads);

    // now, get to work
    int listen_fd = open_listen_fd_or_die(port);
    while (1) {
		struct sockaddr_in client_addr;
		int client_len = sizeof(client_addr);
		int conn_fd = accept_or_die(listen_fd, (sockaddr_t *) &client_addr, (socklen_t *) &client_len);
		Queue_Enqueue(&jobs, conn_fd);
		pthread_cond_signal(&fill);
    }
    return 0;
}
