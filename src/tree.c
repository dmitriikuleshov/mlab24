#include "../headers/tree.h"
#include <malloc.h>
#include <memory.h>

struct Item* create_item(double value, char variable, enum OperationType operationType, enum ItemType type) {
    struct Item* item = (struct Item*)malloc(sizeof(struct Item));     
	if (!item) return NULL;
    item->type = type;
    item->operationType = operationType;
    item->variable = variable;
    item->value = value;
    item->l = NULL;
    item->r = NULL;
    return item;
}


Tree* create_tree() {
    Tree* tree = (Tree*) malloc(sizeof(Tree));     
	if (!tree) return NULL;
    tree->root = NULL;
    return tree;
}


void destroy_item(struct Item* item){
    if (!item->l && !item->r) return;
    if (item->l) {
        destroy_item(item->l);
        struct Item* itemPtr = item->l;
        item->l = NULL;
        free(itemPtr);
    }
    if (item->r) {
        destroy_item(item->r);
        struct Item* itemPtr = item->r;
        item->r = NULL;
        free(itemPtr);
    }
}


struct Item* deep_copy(struct Item* item) {     
	struct Item* copy = malloc(sizeof (*item->r));     
	memcpy(copy, item, sizeof (*item));
    if (item->r){
        struct Item* copy_r = deep_copy(item->r);
		copy->r = copy_r;
    }
    if (item->l){
        struct Item* copy_l = deep_copy(item->l);
		copy->l = copy_l;
    }
    return copy;
}


void destroy_tree(Tree* tree){
    if (!tree->root) return;
    destroy_item(tree->root);
    tree->root = NULL;
}