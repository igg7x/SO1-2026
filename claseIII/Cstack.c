#include "Cstack.h";
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct CStackNode* newNode_CStack(int data)
{
    struct CStackNode* stackNode = (struct CStackNode*) malloc(sizeof(struct StackNode));
    stackNode->stack = newNode(data);
    return stackNode;
}
 
int isEmpty(struct StackNode* root)
{
    return !root;
}
 
void push_CStack(struct StackNode** root, int data)
{

    pthread_mutex_lock()
    push()
    pthread_mutex_unlock()

}
 
int pop(struct StackNode** root)
{
    if (isEmpty(*root))
        return INT_MIN;
    struct StackNode* temp = *root;
    *root = (*root)->next;
    int popped = temp->data;
    free(temp);
 
    return popped;
}
 
int top(struct StackNode* root)
{
    if (isEmpty(root))
        return INT_MIN;
    return root->data;
}

void stackFree(struct StackNode* root)
{
  // we free the stack just popping all the elements
  while(!isEmpty(root))
  {
	  pop(&root);
  }
}
