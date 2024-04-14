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


void destroy_item(struct Item* item) {
    if (!item) return;

    // Рекурсивно освобождаем память для левого и правого поддеревьев
    destroy_item(item->l);
    destroy_item(item->r);

    // Освобождаем память для самого узла
    free(item);
}


void destroy_tree(Tree* tree){
	if (!tree) {
		return;
	}

    destroy_item(tree->root);
    free(tree);
}


struct Item* deep_copy(struct Item* item) {     
	struct Item* copy = malloc(sizeof (*item->r));     
	memcpy(copy, item, sizeof (*item));
    copy->l=NULL;
    copy->r=NULL;
    if (item->r){
        struct Item* copy_r = deep_copy(item->r);
		copy->r = copy_r;
    }
    if (item->l) {
        struct Item* copy_l = deep_copy(item->l);
		copy->l = copy_l;
    }
    return copy;
}


