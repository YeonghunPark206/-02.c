#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TREE_SIZE 1000
#define STACK_SIZE 100

typedef struct Node {
    char val;
    struct Node* left;
    struct Node* right;
} Node;

typedef struct {
    Node* data[STACK_SIZE];
    int top;
} NodeStack;

void push(NodeStack* s, Node* node) {
    if (s->top < STACK_SIZE - 1) {
        s->data[++(s->top)] = node;
    }
}

Node* pop(NodeStack* s) {
    if (s->top >= 0) {
        return s->data[(s->top)--];
    }
    return NULL;
}

int isEmpty(NodeStack* s) {
    return s->top == -1;
}

Node* parse_tree(const char* str) {
    NodeStack stack;
    stack.top = -1;
    Node* curr = NULL;
    const char* p = str;

    while (*p) {
        if (*p == ' ') {
            p++;
        } else if (*p == '(') {
            push(&stack, curr);
            p++;
        } else if (*p == ')') {
            curr = pop(&stack);
            p++;
        } else if (isalpha(*p)) {
            Node* new_node = (Node*)malloc(sizeof(Node));
            new_node->val = *p;
            new_node->left = NULL;
            new_node->right = NULL;

            if (!isEmpty(&stack)) {
                Node* parent = stack.data[stack.top];
                if (parent->left == NULL) {
                    parent->left = new_node;
                } else if (parent->right == NULL) {
                    parent->right = new_node;
                }
            }
            curr = new_node;
            p++;
        } else {
            p++;
        }
    }

    return curr; 
}

void build_array_tree(Node* node, char tree[], int idx) {
    if (!node || idx >= MAX_TREE_SIZE) return;
    tree[idx] = node->val;
    build_array_tree(node->left, tree, 2 * idx);
    build_array_tree(node->right, tree, 2 * idx + 1);
}

void preorder_iterative(char tree[]) {
    int stack[STACK_SIZE];
    int top = -1;
    stack[++top] = 1;

    while (top >= 0) {
        int idx = stack[top--];
        if (idx >= MAX_TREE_SIZE || tree[idx] == 0) continue;
        printf("%c ", tree[idx]);
        stack[++top] = 2 * idx + 1; 
        stack[++top] = 2 * idx;     
    }
}

void inorder_iterative(char tree[]) {
    int stack[STACK_SIZE];
    int top = -1;
    int idx = 1;

    while (top >= 0 || (idx < MAX_TREE_SIZE && tree[idx] != 0)) {
        while (idx < MAX_TREE_SIZE && tree[idx] != 0) {
            stack[++top] = idx;
            idx = 2 * idx;
        }

        if (top < 0) break;

        idx = stack[top--];
        printf("%c ", tree[idx]);
        idx = 2 * idx + 1;
    }
}

void postorder_iterative(char tree[]) {
    int stack1[STACK_SIZE], stack2[STACK_SIZE];
    int top1 = -1, top2 = -1;
    stack1[++top1] = 1;

    while (top1 >= 0) {
        int idx = stack1[top1--];
        if (idx >= MAX_TREE_SIZE || tree[idx] == 0) continue;
        stack2[++top2] = idx;
        stack1[++top1] = 2 * idx;    
        stack1[++top1] = 2 * idx + 1; 
    }

    while (top2 >= 0) {
        printf("%c ", tree[stack2[top2--]]);
    }
}

int main() {
    const char* input = "(A (B (C D) E (G H (I J (K (L) M (N))))))";

    Node* root = parse_tree(input);

    char tree[MAX_TREE_SIZE] = {0};
    build_array_tree(root, tree, 1);

    printf("pre-order: ");
    preorder_iterative(tree);
    printf("\n");

    printf("in-order: ");
    inorder_iterative(tree);
    printf("\n");

    printf("post-order: ");
    postorder_iterative(tree);
    printf("\n");

    return 0;
}
