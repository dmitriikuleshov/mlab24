#ifndef LW_TREE_H
#define LW_TREE_H
#include "../headers/symbol.h"


enum ItemType {
    OPERATOR,
    OPERAND,
    VAR,
};


struct Item {
    double value;
    enum OperationType operationType;
    char variable;
    enum ItemType type;
    struct Item* l;
    struct Item* r;
};


typedef struct {
    struct Item* root;
} Tree;


struct Item* create_item(double operand, char variable, enum OperationType operationType, enum ItemType type);
Tree* create_tree();
void destroy_tree(Tree* tree); struct Item* deep_copy(struct Item* item);

#endif //LW_TREE_H