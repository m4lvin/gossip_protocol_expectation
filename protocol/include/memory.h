#ifndef H_MEMORY_H_
#define H_MEMORY_H_

#include "state.h"
#include "queue.h"

#define EXIT_IF_OUT_OF_MEMORY(ptr)			\
	do {					\
		if(!(ptr)) {			\
			fprintf(stderr, "Out of memory\n");	\
			exit(1);		\
		}				\
	} while(0)
	
#define EXIT_IF_ERROR_OPENING_FILE(fp, filename)			\
	do {					\
		if(!(fp)) {			\
			fprintf(stderr, "Error opening file: %s\n", (filename));	\
			exit(1);		\
		}				\
	} while(0)

#define INTERNAL_ERROR(message)			\
	do {					\
		fprintf(stderr, "Internal error: %s\n", (message));	\
		exit(1);	\
	} while(0)


#define MALLOC_SAFE(p, siz) \
do {					\
	(p)= malloc((siz)); \
	EXIT_IF_OUT_OF_MEMORY(p); \
} while(0)

#define MALLOC_ZERO(p, siz)\
do {					\
  MALLOC_SAFE((p), (siz)); \
  memset((p), 0x00, (siz)); \
} while(0)

#define FREE_SAFE(p) \
do {	\
	if ((p)) {	\
		free((p));		\
		(p) = NULL;	\
	}	\
} while(0)

protocol_state_t* new_protocol_state 
(graph g[MAXN*MAXM], int agents, int protocol_name);
  
void destroy_hash(int agents, twin_queues hash[MAXN*MAXN]);

protocol_state_t* new_protocol_state 
(graph g[MAXN*MAXM], int agents, int protocol_name);

child_t *new_child
( graph secrets[MAXN*MAXM], protocol_state_t* childs_state,
  struct queue_node_t* child_can_queue_pos,
  struct queue_node_t* child_fixed_name_queue_pos,
  int calls_to_child);
  
void destroy_protocol_state (protocol_state_t ** s);

int search_in_twin_queues 
(twin_queues twin_q, struct queue_node_t** found, 
 child_t* child, int protocol_name);
 
void destroy_twin_queues(twin_queues* twin_q);

#endif /* H_MEMORY_H_ */
