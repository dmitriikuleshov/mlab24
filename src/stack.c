#include "../headers/stack.h"
#include <malloc.h>


int pop_stack(Stack* s) {
    if (s->size == 0) return -1;
    s->back = s->back->pr;
    s->size--;
    return 0;
}


struct StackItem* top(Stack* s) {
    return s->back;
}


int insert_stack(Stack* s, Symbol value) {
    struct StackItem* item = (struct StackItem*)malloc(sizeof(struct StackItem));
	if (!item) return -1;
    item->value = value;
    item->pr = s->back;
    s->back = item;
    s->size++;
    return 0;
}


Stack* create_stack() {
    Stack* stack = (Stack*) malloc(sizeof(Stack));     
	if (!stack) return NULL;
	stack->size = 0;
    stack->back = NULL;
    return stack;
}


void destroy_stack(Stack* stack) {
    while (stack->size){
        struct StackItem* t = top(stack);
        pop_stack(stack);
        free(t);
    }
}