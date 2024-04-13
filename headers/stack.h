#ifndef C_LABS_STACK_H
#define C_LABS_STACK_H
#include "../headers/symbol.h" 


struct StackItem {
    Symbol value;
    struct StackItem* pr;
};


typedef struct  {
	int size;
	struct StackItem* back;
} Stack;


int pop_stack(Stack* s);
struct StackItem* top(Stack* s);
int insert_stack(Stack* s, Symbol value);
Stack* create_stack();
void destroy_stack(Stack* stack);

#endif //C_LABS_STACK_H