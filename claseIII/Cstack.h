#ifndef CSTACK_UNBOUND_H
#define CSTACK_UNBOUND_H
#include "stack_unbound.h"
#include <pthread.h>

struct CStackNode {  
    struct StackNode* stack ; 
    pthread_mutex_t mutex_CStack ; 
};

struct StackNode* newNode_CStack(int data);

int isEmpty_CStack(struct CStackNode* root);
 
void push_CStack(struct CStackNode** root, int data);
 
int pop_CStack(struct CStackNode** root);
 
int top_CStack(struct CStackNode* root);

void stackFree_CStack(struct CStackNode* root);

#endif /* CONCURRENT_STACK_H */