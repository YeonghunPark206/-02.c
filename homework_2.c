#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 1000

typedef struct TreeNode {
    char data;
    struct TreeNode **children;
    int child_count;
} TreeNode;

int node_count = 0;
TreeNode* node_array[MAX_NODES + 1];

char input[1000];
int pos = 0, len = 0;

TreeNode* parse_tree();
int get_height(TreeNode* root);
int get_leaf_count(TreeNode* root);
void traverse_to_array(TreeNode* root);
void free_tree(TreeNode* root);

int main() {
    scanf("%[^\n]", input);
    len = strlen(input);
    
    TreeNode* root = parse_tree();
    
    node_count = 0;
    traverse_to_array(root);
    
    int height = get_height(root);
    
    int leaf_count = get_leaf_count(root);
    
    printf("%d,%d,%d", height, node_count, leaf_count);
    
    free_tree(root);
    
    return 0;
}

TreeNode* parse_tree() {
    
    if (pos >= len) return NULL;
    
    if (input[pos] != '(') return NULL;
    pos++; 
    
    if (pos >= len) return NULL;
    char node_data = input[pos++];
    
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->data = node_data;
    node->children = NULL;
    node->child_count = 0;
    
    while (pos < len && input[pos] == '(') {
        TreeNode* child = parse_tree();
        if (child != NULL) {
            node->children = (TreeNode**)realloc(node->children, sizeof(TreeNode*) * (node->child_count + 1));
            node->children[node->child_count++] = child;
        }
    }
    
    if (pos < len && input[pos] == ')') {
        pos++; 
    }
    
    return node;
}

int get_height(TreeNode* root) {
    if (root == NULL) return -1;
    if (root->child_count == 0) return 0; 
    
    int max_child_height = -1;
    for (int i = 0; i < root->child_count; i++) {
        int h = get_height(root->children[i]);
        if (h > max_child_height) max_child_height = h;
    }
    return max_child_height + 1;
}

int get_leaf_count(TreeNode* root) {
    if (root == NULL) return 0;
    if (root->child_count == 0) return 1;
    int leaf_sum = 0;
    for (int i = 0; i < root->child_count; i++) {
        leaf_sum += get_leaf_count(root->children[i]);
    }
    return leaf_sum;
}

void traverse_to_array(TreeNode* root) {
    if (root == NULL) return;
    node_array[++node_count] = root;
    for (int i = 0; i < root->child_count; i++) {
        traverse_to_array(root->children[i]);
    }
}

void free_tree(TreeNode* root){
    if (root == NULL) return;
    for (int i = 0; i < root->child_count; i++){
        free_tree(root->children[i]);
    }
    free(root->children);
    free(root);
}
