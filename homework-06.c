#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define VERTEX_CNT 100
#define SPARSE_EDGE_CNT 100
#define DENSE_EDGE_CNT 4000

typedef struct {
    int n;
    int **adj;
    long long cmp_insert_delete;
    long long cmp_check;
    long long cmp_print_adj;
} MatrixGraph;

typedef struct ListNode {
    int vertex;
    struct ListNode *next;
} ListNode;

typedef struct {
    int n;
    ListNode **adjList;
    long long cmp_insert_delete;
    long long cmp_check;
    long long cmp_print_adj;
} ListGraph;

typedef struct {
    int u,v;
} Edge;

int cmp_edge(const void *a, const void *b)
{
    Edge *ea = (Edge*)a, *eb = (Edge*)b;
    if(ea->u != eb->u) return ea->u - eb->u;
    return ea->v - eb->v;
}

void generate_edges(Edge *edges, int edge_count, int max_vertex) {
    int max_possible = max_vertex*(max_vertex-1)/2;
    Edge *all_edges = (Edge*)malloc(sizeof(Edge)*max_possible);
    int idx=0;
    for(int i=0; i<max_vertex; i++) {
        for(int j=i+1; j<max_vertex; j++){
            all_edges[idx].u = i;
            all_edges[idx].v = j;
            idx++;
        }
    }
    for(int i=max_possible-1; i>0; i--){
        int r = rand()%(i+1);
        Edge temp = all_edges[i];
        all_edges[i] = all_edges[r];
        all_edges[r] = temp;
    }
    for(int i=0; i<edge_count; i++){
        edges[i] = all_edges[i];
    }
    free(all_edges);
}

MatrixGraph *mgraph_create(int n) {
    MatrixGraph *g = (MatrixGraph*)malloc(sizeof(MatrixGraph));
    g->n = n;
    g->adj = (int**)malloc(sizeof(int*)*n);
    for(int i=0; i<n; i++){
        g->adj[i] = (int*)calloc(n, sizeof(int));
    }
    g->cmp_insert_delete = 0;
    g->cmp_check = 0;
    g->cmp_print_adj = 0;
    return g;
}

void mgraph_free(MatrixGraph *g){
    for(int i=0; i<g->n; i++){
        free(g->adj[i]);
    }
    free(g->adj);
    free(g);
}

void mgraph_insert_edge(MatrixGraph *g, int u, int v){
    g->cmp_insert_delete++;
    g->adj[u][v] = 1;
    g->adj[v][u] = 1;
}

void mgraph_delete_edge(MatrixGraph *g, int u, int v){
    g->cmp_insert_delete++;
    g->adj[u][v] = 0;
    g->adj[v][u] = 0;
}

int mgraph_edge_exists(MatrixGraph *g, int u, int v){
    g->cmp_check++;
    return g->adj[u][v] == 1;
}

void mgraph_print_adjacent(MatrixGraph *g, int u){
    for(int i=0; i<g->n; i++){
        g->cmp_print_adj++;
        if(g->adj[u][i]) {
        }
    }
}

ListGraph *lgraph_create(int n){
    ListGraph *g = (ListGraph*)malloc(sizeof(ListGraph));
    g->n = n;
    g->adjList = (ListNode**)malloc(sizeof(ListNode*)*n);
    for(int i=0; i<n; i++){
        g->adjList[i] = NULL;
    }
    g->cmp_insert_delete = 0;
    g->cmp_check = 0;
    g->cmp_print_adj = 0;
    return g;
}

void free_list(ListNode *head){
    ListNode *cur = head;
    while(cur){
        ListNode *tmp = cur->next;
        free(cur);
        cur = tmp;
    }
}

void lgraph_free(ListGraph *g){
    for(int i=0; i<g->n; i++){
        free_list(g->adjList[i]);
    }
    free(g->adjList);
    free(g);
}

void lgraph_insert_edge(ListGraph *g, int u, int v){
    ListNode *cur = g->adjList[u];
    while(cur){
        g->cmp_insert_delete++;
        if(cur->vertex == v) return;
        cur = cur->next;
    }
    ListNode *newNode = (ListNode*)malloc(sizeof(ListNode));
    newNode->vertex = v;
    newNode->next = g->adjList[u];
    g->adjList[u] = newNode;

    cur = g->adjList[v];
    while(cur){
        g->cmp_insert_delete++;
        if(cur->vertex == u) return;
        cur = cur->next;
    }
    newNode = (ListNode*)malloc(sizeof(ListNode));
    newNode->vertex = u;
    newNode->next = g->adjList[v];
    g->adjList[v] = newNode;
}

void lgraph_delete_edge(ListGraph *g, int u, int v){
    ListNode **cur = &g->adjList[u];
    while(*cur){
        g->cmp_insert_delete++;
        if((*cur)->vertex == v){
            ListNode *delNode = *cur;
            *cur = delNode->next;
            free(delNode);
            break;
        }
        cur = &((*cur)->next);
    }

    cur = &g->adjList[v];
    while(*cur){
        g->cmp_insert_delete++;
        if((*cur)->vertex == u){
            ListNode *delNode = *cur;
            *cur = delNode->next;
            free(delNode);
            break;
        }
        cur = &((*cur)->next);
    }
}

int lgraph_edge_exists(ListGraph *g, int u, int v){
    ListNode *cur = g->adjList[u];
    while(cur){
        g->cmp_check++;
        if(cur->vertex == v) return 1;
        cur = cur->next;
    }
    return 0;
}

void lgraph_print_adjacent(ListGraph *g, int u){
    ListNode *cur = g->adjList[u];
    while(cur){
        g->cmp_print_adj++;
        cur = cur->next;
    }
}

size_t mgraph_memory_usage(MatrixGraph *g){
    return sizeof(MatrixGraph) + sizeof(int*)*g->n + sizeof(int)*g->n*g->n;
}

size_t lgraph_memory_usage(ListGraph *g){
    size_t size = sizeof(ListGraph);
    size += sizeof(ListNode*)*g->n;
    for(int i=0; i<g->n; i++){
        ListNode *cur = g->adjList[i];
        while(cur){
            size += sizeof(ListNode);
            cur = cur->next;
        }
    }
    return size;
}

#define TEST_OPS 1000

void test_mgraph(MatrixGraph *g, Edge *edges, int edge_count){
    for(int i=0; i<edge_count; i++){
        mgraph_insert_edge(g, edges[i].u, edges[i].v);
    }
    for(int i=0; i<edge_count/10; i++){
        mgraph_delete_edge(g, edges[i].u, edges[i].v);
        mgraph_insert_edge(g, edges[i].u, edges[i].v);
    }
    for(int i=0; i<TEST_OPS; i++){
        int u = rand()%g->n;
        int v = rand()%g->n;
        if(u == v) v = (v+1)%g->n;
        mgraph_edge_exists(g,u,v);
    }
    for(int i=0; i<TEST_OPS; i++){
        int u = rand()%g->n;
        mgraph_print_adjacent(g,u);
    }
}

void test_lgraph(ListGraph *g, Edge *edges, int edge_count){
    for(int i=0; i<edge_count; i++){
        lgraph_insert_edge(g, edges[i].u, edges[i].v);
    }
    for(int i=0; i<edge_count/10; i++){
        lgraph_delete_edge(g, edges[i].u, edges[i].v);
        lgraph_insert_edge(g, edges[i].u, edges[i].v);
    }
    for(int i=0; i<TEST_OPS; i++){
        int u = rand()%g->n;
        int v = rand()%g->n;
        if(u == v) v = (v+1)%g->n;
        lgraph_edge_exists(g, u, v);
    }
    for(int i=0; i<TEST_OPS; i++){
        int u = rand()%g->n;
        lgraph_print_adjacent(g, u);
    }
}

int main(){
    srand((unsigned)time(NULL));
    Edge edges_sparse[SPARSE_EDGE_CNT];
    Edge edges_dense[DENSE_EDGE_CNT];

    generate_edges(edges_sparse, SPARSE_EDGE_CNT, VERTEX_CNT);
    generate_edges(edges_dense, DENSE_EDGE_CNT, VERTEX_CNT);

    MatrixGraph *mg_sparse = mgraph_create(VERTEX_CNT);
    test_mgraph(mg_sparse, edges_sparse, SPARSE_EDGE_CNT);
    size_t mem_mg_sparse = mgraph_memory_usage(mg_sparse);

    printf("케이스 1: 희소그래프-인접행렬\n");
    printf("메모리 %zu Bytes\n", mem_mg_sparse);
    printf("간선 삽입/삭제 비교 %lld번\n", mg_sparse->cmp_insert_delete);
    printf("두 정점의 연결 확인 비교 %lld번\n", mg_sparse->cmp_check);
    printf("한 노드의 인접 노드 출력 비교 %lld번\n\n", mg_sparse->cmp_print_adj);
    mgraph_free(mg_sparse);

    ListGraph *lg_sparse = lgraph_create(VERTEX_CNT);
    test_lgraph(lg_sparse, edges_sparse, SPARSE_EDGE_CNT);
    size_t mem_lg_sparse = lgraph_memory_usage(lg_sparse);

    printf("케이스 2: 희소그래프-인접리스트\n");
    printf("메모리 %zu Bytes\n", mem_lg_sparse);
    printf("간선 삽입/삭제 비교 %lld번\n", lg_sparse->cmp_insert_delete);
    printf("두 정점의 연결 확인 비교 %lld번\n", lg_sparse->cmp_check);
    printf("한 노드의 인접 노드 출력 비교 %lld번\n\n", lg_sparse->cmp_print_adj);
    lgraph_free(lg_sparse);

    MatrixGraph *mg_dense = mgraph_create(VERTEX_CNT);
    test_mgraph(mg_dense, edges_dense, DENSE_EDGE_CNT);
    size_t mem_mg_dense = mgraph_memory_usage(mg_dense);

    printf("케이스 3: 밀집그래프-인접행렬\n");
    printf("메모리 %zu Bytes\n", mem_mg_dense);
    printf("간선 삽입/삭제 비교 %lld번\n", mg_dense->cmp_insert_delete);
    printf("두 정점의 연결 확인 비교 %lld번\n", mg_dense->cmp_check);
    printf("한 노드의 인접 노드 출력 비교 %lld번\n\n", mg_dense->cmp_print_adj);
    mgraph_free(mg_dense);

    ListGraph *lg_dense = lgraph_create(VERTEX_CNT);
    test_lgraph(lg_dense, edges_dense, DENSE_EDGE_CNT);
    size_t mem_lg_dense = lgraph_memory_usage(lg_dense);

    printf("케이스 4: 밀집그래프-인접리스트\n");
    printf("메모리 %zu Bytes\n", mem_lg_dense);
    printf("간선 삽입/삭제 비교 %lld번\n", lg_dense->cmp_insert_delete);
    printf("두 정점의 연결 확인 비교 %lld번\n", lg_dense->cmp_check);
    printf("한 노드의 인접 노드 출력 비교 %lld번\n", lg_dense->cmp_print_adj);
    lgraph_free(lg_dense);

    return 0;
}
