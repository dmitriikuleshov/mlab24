#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "../headers/stack.h"
#include "../headers/tree.h"


enum Assoc {
	ASSOC_LEFT,
	ASSOC_RIGHT,
};


int operation_priority(enum OperationType operation) {
	switch (operation) {
		case MINUS:
		case PLUS:
			return 1;
		case MULT:
		case DIV:
			return 2;
		case POW:
			return 3;
		case UNARY_MINUS:
			return 4;
	}
	return -1;
}


char operation_to_char(enum OperationType operation) {
	switch (operation) {
		case UNARY_MINUS:
			return '-';
		case MINUS:
			return '-';
		case PLUS:
			return '+';
		case MULT:
			return '*';
		case DIV:
			return '/';
		case POW:
			return '^';
	}
	return -1;
}


int is_space(char c) {
	return c == '\t' || c == ' ';
}


int get_char(char *c) {
	*c = getchar();
	while (is_space(*c)) {
		*c = getchar();
	}
	return 0;
}


int get_operation(const char *c, enum SymbolType *prevType, enum OperationType *operation) {
	if (*c == '*') {
		*operation = MULT;
	}
	else if (*c == '/') {
		*operation = DIV;
	}
	else if (*c == '+') {
		*operation = PLUS;
	}
	else if (*c == '^') {
		*operation = POW;
	}
	else if (*c == '-' && (*prevType == NONE || *prevType == LEFT_BR || *prevType == OPERATION)) {
		*operation = UNARY_MINUS;
	}
	else if (*c == '-') {
		*operation = MINUS;
	}
	else {
		return -1;
	}
	*prevType = OPERATION;
	return 0;
}


int is_operation(char c) {
	return c == '+' || c == '*' || c == '/' || c == '^' || c == '-';
}


enum Assoc get_assoc(enum OperationType operation) {
	switch (operation) {
		case MINUS:
		case PLUS:
		case MULT:
		case DIV:
			return ASSOC_LEFT;
		case POW:
		case UNARY_MINUS:
			return ASSOC_RIGHT;
	}
	return -1;
}


int get_symbol(Symbol *s, enum SymbolType *prevType) {
	char c;
	get_char(&c);
	enum OperationType operation;
	double v;
	if (c == '\n') {
		return -1;
	}
	if (is_operation(c)) {
		s->type = OPERATION;
		if (get_operation(&c, prevType, &operation) == -1) {
			return -1;
		}
		s->operation = operation;
	} else if (c == '(') {
		s->type = LEFT_BR;
	}
	else if (c == ')') {
		s->type = RIGHT_BR;
	}
	else if ((c >= '0' && c <= '9') || c == '.') {
		ungetc(c, stdin);
		scanf("%lf", &v);
		s->type = NUMBER;
		s->val = v;
	}
	else if ((c >= 'a' && c <= 'z') || ((c >= 'A' && c <= 'Z'))) {
		ungetc(c, stdin);
		scanf("%[a-zA-z]", &s->variable);
		s->type = VARIABLE;
	} else {
		s->type = NONE;
	}
	*prevType = s->type;
	return 0;
}


int handle_right_br(Stack *st, Stack *rev) {
	for (;;) {
		if (st->size == 0) {
			printf("Error: No closing bracket\n");
			return -1;
		}
		struct StackItem *t = top(st);
		if (t->value.type == LEFT_BR) {
			pop_stack(st);
			return 0;
		}
		put_in_stack(rev, t->value);
		pop_stack(st);
	}
}


void handle_operation(Stack *s, Stack *rev, Symbol *t) {
	for (;;) {
		if (s->size == 0)
			return;
		struct StackItem *cur_t = top(s);
		if (cur_t->value.type != OPERATION)
			return;
		if (
			(get_assoc(cur_t->value.operation) == ASSOC_LEFT && operation_priority(t->operation) <= operation_priority(cur_t->value.operation)) ||
			(get_assoc(cur_t->value.operation) == ASSOC_RIGHT && operation_priority(t->operation) < operation_priority(cur_t->value.operation))) {
			put_in_stack(rev, cur_t->value);
			pop_stack(s);
		} else {
			return;
		}
	}
}


int insert_to_item(struct Item *cur_item, struct Item *item_to_insert) {
	if (!cur_item)
		return 1;
	if (cur_item->type == OPERAND || cur_item->type == VAR)
		return -1;
	int res = insert_to_item(cur_item->r, item_to_insert);
	if (res == 1) {
		cur_item->r = item_to_insert;
		return 0;
	}
	else if (res == 0)
		return 0;
	if (cur_item->operationType == UNARY_MINUS) {
		return -1;
	}
	res = insert_to_item(cur_item->l, item_to_insert);
	if (res == 1) {
		cur_item->l = item_to_insert;
		return 0;
	} else if (res == 0)
		return 0;
	return -1;
}


int insert(Tree *tree, struct Item *item) {
	if (!tree->root) {
		tree->root = item;
		return 0;
	}
	return insert_to_item(tree->root, item);
}


void printItem(struct Item *item, int depth) {
	if (item->r)
		printItem(item->r, depth + 1);
	for (int i = 0; i < depth; ++i) {
		printf("\t");
	}
	if (item->type == OPERATOR)
		printf("%c", operation_to_char(item->operationType));
	else if (item->type == VAR)
		printf("%c", item->variable);
	else
		printf("%.2lf", item->value);
	printf("\n");
	if (item->l)
		printItem(item->l, depth + 1);
}


void printTree(Tree *tree) {
	if (!tree->root)
		return;
	printItem(tree->root, 0);
}


int transformItem(struct Item *item) {
	if (!item)
		return 0;
	if (item->type == OPERATOR && item->operationType == MULT) {
		if (item->l->type == OPERATOR && item->l->operationType == PLUS) {
			item->operationType = PLUS;
			struct Item *multItem1 = create_item(0,'c', MULT, OPERATOR);
			if (!multItem1)
				return -1;
			struct Item *multItem2 = create_item(0,'c', MULT, OPERATOR);
			if (!multItem2)
				return -1;
			multItem1->r = item->l->r;
			multItem1->l = item->r;
			multItem2->r = item->l->l;
			multItem2->l = deep_copy(item->r);
			free(item->l); // fixed
			item->r = multItem1;
			item->l = multItem2;
			transformItem(item->l);
			transformItem(item->r);
			return 0;
		}
		if (item->r->type == OPERATOR && item->r->operationType == PLUS) {
			item->operationType = PLUS;
			struct Item *multItem1 = create_item(0, 'c', MULT, OPERATOR);
			if (!multItem1)
				return -1;
			struct Item *multItem2 = create_item(0, 'c', MULT, OPERATOR);
			if (!multItem2)
				return -1;
			multItem1->r = item->r->r;
			multItem1->l = item->l;
			multItem2->r = item->r->l;
			multItem2->l = deep_copy(item->l);
			free(item->r); // fixed
			item->r = multItem1;
			item->l = multItem2;
			transformItem(item->r);
			transformItem(item->l);
			return 0;
		}
	}
	transformItem(item->r);
	transformItem(item->l);
	return 0;
}


int transform(Tree *tree) {
	if (!tree->root)
		return 0;
	return transformItem(tree->root);
}


void print_expr_item(struct Item *item) {
	if (item->type == OPERAND) {
		printf(" %.4lf ", item->value);
		return;
	}
	if (item->type == VAR) {
		printf(" %c ", item->variable);
		return;
	}
	if (item->operationType == UNARY_MINUS) {
		printf("(-(");
		print_expr_item(item->r);
		printf("))");
		return;
	}
	if (item->l->type == OPERATOR && operation_priority(item->operationType) >= operation_priority(item->l->operationType)) {
		printf("(");
		print_expr_item(item->l);
		printf(")");
	} else {
		print_expr_item(item->l);
	}
	printf(" %c ", operation_to_char(item->operationType));
	if (item->r->type == OPERATOR && operation_priority(item->operationType) >= operation_priority(item->r->operationType)) {
		printf("(");
		print_expr_item(item->r);
		printf(")");
	}
	else {
		print_expr_item(item->r);
	}
}


void print_expr(Tree *tree) {
	if (!tree->root)
		return;
	print_expr_item(tree->root);
	printf("\n");
}


int input(Stack *stack, Stack *rev, Tree *tree) {
	Symbol s;
	enum SymbolType st = NONE;
	destroy_tree(tree);
	printf("Enter expression: ");
	while (get_symbol(&s, &st) != -1)
	{
		switch (s.type) {
			case NONE:
				printf("Char not recognized\n");
				break;
			case NUMBER:
			case VARIABLE:
				put_in_stack(rev, s);
				break;
			case OPERATION:
				handle_operation(stack, rev, &s);
				put_in_stack(stack, s);
				break;
			case LEFT_BR:
				put_in_stack(stack, s);
				break;
			case RIGHT_BR:
				if (handle_right_br(stack, rev) == -1) {
					return -1;
				}
				break;
		}
	}
	while (stack->size != 0) {
		s = top(stack)->value;
		if (s.type == LEFT_BR) {
			printf("Error: no pair for bracket\n");
			return -1;
		}
		put_in_stack(rev, s);
		pop_stack(stack);
	}
	while (rev->size != 0)
	{
		struct StackItem *item = top(rev);
		struct Item *treeItem = create_item(
			item->value.type == NUMBER ? item->value.val : 0, item->value.type == VARIABLE ? item->value.variable : 'c', item->value.type == OPERATION ? item->value.operation : DIV, item->value.type == NUMBER ? OPERAND : item->value.type == VARIABLE ? VAR : OPERATOR);
		if (!treeItem)
			return -1;
		insert(tree, treeItem);
		pop_stack(rev);
	}
	return 0;
}


void print_commands() {
	printf("Commands: \n");
	printf("\t1 - Input an expression\n");
	printf("\t2 - Show tree\n");
	printf("\t3 - Transform tree\n");
	printf("\t4 - Show expression\n");
	printf("\t5 - exit\n");
}


int handle_input(int option, Stack *stack, Stack *rev, Tree *tree) {
	switch (option) {
		case 1:
			if (input(stack, rev, tree) == -1)
				return -1;
			break;
		case 2:
			printTree(tree);
			break;
		case 3:
			if (transform(tree) == -1)
				return -1;
			break;
		case 4:
			print_expr(tree);
			break;
		case 5:
			destroy_tree(tree);
			destroy_stack(stack);
			destroy_stack(rev);
			exit(0);
			break;
		default:
			print_commands();
			break;
	}
	return 0;
}


int input_option(int *option) {
	int is_eof = scanf("%d", option);
	char t;
	scanf("%c", &t);
	return is_eof;
}


int main() {
	Tree *tree = create_tree();
	if (!tree)
		return -1;
	Stack *stack = create_stack();
	Stack *rev = create_stack();
	if (!stack || !rev)
		return -1;
	int option;
	print_commands();
	printf("Enter command: ");
	while ((input_option(&option)) != EOF) {
		handle_input(option, stack, rev, tree);
		printf("Enter command: ");
	}
}