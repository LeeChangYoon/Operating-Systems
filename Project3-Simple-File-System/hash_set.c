#include "hash_set.h"

int hash_func (char* str, int len) {
    unsigned long hash = 0;
    int c;
    for (int i = len - 1; i >= 0; i--)
    {
        c = str[i] - 'a' + 1;
        hash = (((hash << 5) - hash) + c) % TABLE_SIZE;
        continue;
    }
    return hash % TABLE_SIZE;
}

void insert_node(BST_set* set, Pair* pair) {
	SetNode* new_node = malloc(sizeof(*new_node));
	new_node->pair = pair;
	new_node->right = NULL;
    new_node->left = NULL;

	if (set->root == NULL) {
		set->root = new_node;
	} else {
		SetNode* current_node = set->root;
		
		while (1) {
			if (new_node->pair->hash < current_node->pair->hash) {
			    if (current_node->left != NULL)
                    current_node = current_node->left;
                else {
                    current_node->left = new_node;
                    break;
                }
            } else if (new_node->pair->hash > current_node->pair->hash) {
                if (current_node->right != NULL)
                    current_node = current_node->right;
                else {
                    current_node->right = new_node;
                    break;
                }
            } else {
                while(1) {
                    if (!strcmp(new_node->pair->name, current_node->pair->name)) {
                        return;
                    } else {
                        fprintf(stderr, "HASH COLLISION");
                        exit(-1);
                    }
                }
            }
		}
	}
	set->size++;
}

int find_inode(BST_set* set, char* name) {
    int hash = hash_func(name, strlen(name));
	if (set->root == NULL) {
		return -1;
	} else {
		SetNode* current_node = set->root;
		
		while (1) {
			if (hash < current_node->pair->hash) {
			    if (current_node->left != NULL)
                    current_node = current_node->left;
                else {
                    return -1;
                }
            } else if (hash > current_node->pair->hash) {
                if (current_node->right != NULL)
                    current_node = current_node->right;
                else {
                    return -1;
                }
            } else {
                while(1) {
                    if (!strcmp(name, current_node->pair->name)) {
                        return current_node->pair->inode;
                    } else {
                        fprintf(stderr, "HASH COLLISION");
                        exit(-1);
                    }
                }
            }
		}
	}
}