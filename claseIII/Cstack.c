#include "Cstack.h";
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct CStackNode* newNode_CStack(int data)
{
    struct CStackNode* stackNode = (struct CStackNode*) malloc(sizeof(struct StackNode));
    stackNode->stacknode = newNode(data);
    return stackNode;
}
 
int isEmpty_CStack(struct CStackNode* root)
{
    return !root;
}
 
void push_CStack(struct CStackNode** root, int data)
{
    pthread_mutex_lock((*root)->mutex_CStack);
    push(&(*root)->stacknode,data);
    pthread_mutex_unlock((*root)->mutex_CStack);

}
 
int pop_CStack(struct CStackNode** root)
{
    pthread_mutex_lock((*root)->mutex_CStack);
    if (isEmpty(*root))
        return INT_MIN;
    struct CStackNode* temp = *root;
    *root = (*root)->stacknode->next;
    pthread_mutex_unlock((*root)->mutex_CStack);
    int popped = temp->stacknode->data;
    free(temp);
 
    return popped;
}
 
int top_CStack(struct CStackNode* root)
{
    pthread_mutex_lock(root->mutex_CStack);
    if (isEmpty(root)){
        pthread_mutex_unlock(root->mutex_CStack);
        return INT_MIN;
    }
    pthread_mutex_unlock(root->mutex_CStack);
    return root->stacknode->data;
}

void stackFree_CStack(struct CStackNode* root)
{
  pthread_mutex_t* mutex = root->mutex_CStack;  
  pthread_mutex_lock(root->mutex_CStack);  
  // we free the stack just popping all the elements
  while(!isEmpty(root))
	  pop(&root->stacknode);
  pthread_mutex_unlock(mutex); //al popear no va a existir mas el root del mutex  
}
