#ifndef LW_STACK_H
#define LW_STACK_H
#include "../headers/symbol.h" 


typedef struct StackItem {
    Symbol value;
    struct StackItem* pr;
} StackItem;


typedef struct  {
	int size;
	struct StackItem* back;
} Stack;


void destroy_stack_item(StackItem* stack_item);
int pop_stack(Stack* s);
struct StackItem* top(Stack* s);
int put_in_stack(Stack* s, Symbol value);
Stack* create_stack();
void destroy_stack(Stack* stack);

#endif //LW_STACK_H