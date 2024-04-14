#include "../headers/stack.h"
#include <malloc.h>


int pop_stack(Stack* s) {
    if (s->size == 0) return -1;
	StackItem* tmp = s->back;
    s->back = s->back->pr;
	free(tmp);
    s->size--;
    return 0;
}


StackItem* top(Stack* s) {
    return s->back;
}


int put_in_stack(Stack* s, Symbol value) {
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

void destroy_stack_item(StackItem* stack_item){
	free(stack_item->pr);
	free(&stack_item->value);
	free(stack_item);
}

void destroy_stack(Stack* stack) {
    while (stack->size){
        // struct StackItem* t = top(stack);
        pop_stack(stack);
        // free(t);
    }
	free(stack);
}
