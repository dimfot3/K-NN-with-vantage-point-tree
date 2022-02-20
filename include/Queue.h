/**
 * Authors: Fotiou Dimitrios(9650), Andreas Eleutheriadis(9649)
 * @brief Here are the declaration of queue utilities. 
 * @source https://www.geeksforgeeks.org/priority-queue-using-linked-list/
 **/
#ifndef _QUEUE_H
#define _QUEUE_H


/**
 * @brief this is a priority queue implementation
 * 
 * @param idx  the data of the queue slot, idx of a point
 * @param priority the priority distance, lower distance inticate higher priority
 * @param next pointer to the next queue slot
 **/
struct queue_node
{
    int idx;
    float priority;
    struct queue_node* next;
};

/**
 * @brief creates a new node for the queue
 * @param d data for the node
 * @param p priority of the node
 * @return the pointer to the new node
 **/
struct queue_node* new_node(int d, float p);


/**
 * @brief removes the node with lower priority(higher distance) and return then new head
 * @param head pointer to pointer of the starting node of the queue
 * @param return the new head
 **/
void pop(struct queue_node** head);

/**
 * @brief checks if the queue is empty
 * 
 * @param head pointer to pointer of the starting node of the queue
 * @return 0 if queue is empty, 1 otherwise
 **/
int isEmpty(struct queue_node** head);

/**
 * @brief returns the size of the queue
 * 
 * @param head pointer to pointer of the starting node of the queue
 **/
int get_heap_size(struct queue_node** head);


/**
 * @brief push a node to the queue
 * 
 * @param head pointer to pointer of the starting node of the queue
 * @param d data to the node
 * @param p priority of the node
 * @return the new head
 **/
void push(struct queue_node** head, int d, float p);


/**
 * @brief queue to array
 * 
 * @param head pointer of the start of the queue
 * @param array where to save the data of queue
 * @param k number of elements to save
 **/
void queue_to_arr(struct queue_node* head, int* array, int k);

/**
 * @brief free the queue
 * 
 * @param head pointer to pointer of the starting node of the queue
 * @return void
 **/
void free_queue(struct queue_node** head);

/**
 * @brief return the top element with the smaller priority
 * 
 * @param head pointer to pointer of the starting node of the queue
 * @return the top element 
 **/
struct queue_node* top(struct queue_node** head);

#endif