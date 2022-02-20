/**
 * Authors: Fotiou Dimitrios(9650), Andreas Eleutheriadis(9649)
 * @brief Here are the implementations of Queue utilities
 * @source https://www.geeksforgeeks.org/priority-queue-using-linked-list/
 **/
#include <stdlib.h>
#include <Queue.h>

struct queue_node* new_node(int d, float p)
{
    struct queue_node* temp = (struct queue_node*)malloc(sizeof(struct queue_node));
    temp->idx = d;
    temp->priority = p;
    temp->next = NULL;
    return temp;
}

void pop(struct queue_node** head)
{
    struct queue_node* temp = *head;
    struct queue_node* prev = NULL;
    while(temp->next!=NULL)
    {
        prev = temp;
        temp = temp->next;
    }
    prev->next = NULL;
    free(temp);
}

struct queue_node* top(struct queue_node** head)
{
    struct queue_node* temp = *head;
    while(temp->next != NULL)
    {
        temp = temp->next;
    }
    return temp;
}

int isEmpty(struct queue_node** head)
{
    return (*head) == NULL;
}

int get_heap_size(struct queue_node** head)
{
    if(isEmpty(head))
        return 0;
    struct queue_node* temp = *head;
    int k = 0;
    while(temp != NULL)
    {
        k++;
        temp = temp->next;
    }
    return k;
}

void push(struct queue_node** head, int d, float p)
{
    struct queue_node* start = (*head);
 
    // Create new Node
    struct queue_node* temp = new_node(d, p);
    if(isEmpty(head))
    {
        *head = temp;
        return;
    }
 
    // Special Case: The head of list has lesser
    // priority than new node. So insert new
    // node before head node and change head node.
    if ((*head)->priority > p) 
    {
        // Insert New Node before head
        temp->next = *head;
        (*head) = temp;
    }
    else 
    {
 
        // Traverse the list and find a
        // position to insert new node
        while (start->next != NULL && start->next->priority < p) 
        {
            start = start->next;
        }
 
        // Either at the ends of the list
        // or at required position
        temp->next = start->next;
        start->next = temp;
    }
}

void queue_to_arr(struct queue_node* head, int* array, int k)
{
    struct queue_node* temp = head;
    for(int i = 0; i < k; i++)
    {
        if(temp != NULL)
        {
            array[i] = temp->idx;
            temp = temp->next;
        }
    }
}

void free_queue(struct queue_node** head)
{
    struct queue_node* temp = (*head);
    while(temp != NULL)
    {
        struct queue_node* next = temp->next;
        free(temp);
        temp = next;
    }
}

