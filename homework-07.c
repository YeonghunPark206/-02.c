#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 10
#define M 20

static int adj[N][N];

static void add_edge_unique(int u, int v) {
    if (u == v) return;
    if (adj[u][v]) return;
    adj[u][v] = adj[v][u] = 1;
}

static void generate_random_graph(unsigned int seed) {
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            adj[i][j] = 0;
    srand(seed);
    int edges = 0;
    while (edges < M) {
        int u = rand() % N;
        int v = rand() % N;
        if (u == v) continue;
        if (!adj[u][v]) {
            adj[u][v] = adj[v][u] = 1;
            edges++;
        }
    }
}

static void bfs_shortest_paths(int s, int dist_out[N]) {
    for (int i = 0; i < N; ++i) dist_out[i] = -1;
    int q[N];
    int head = 0, tail = 0;
    dist_out[s] = 0;
    q[tail++] = s;
    while (head < tail) {
        int u = q[head++];
        for (int v = 0; v < N; ++v) {
            if (adj[u][v] && dist_out[v] == -1) {
                dist_out[v] = dist_out[u] + 1;
                q[tail++] = v;
            }
        }
    }
}

int main(int argc, char* argv[]) {
    unsigned int seed = (unsigned int)time(NULL);
    if (argc >= 2) seed = (unsigned int)strtoul(argv[1], NULL, 10);
    generate_random_graph(seed);
    printf("Seed=%u\n", seed);
    printf("Adjacency Matrix (%d vertices, %d edges):\n", N, M);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            printf("%d%c", adj[i][j], (j+1==N)?'\n':' ');
        }
    }
    printf("\nAll-Pairs Shortest Path Lengths (BFS on unweighted, undirected graph):\n");
    for (int s = 0; s < N; ++s) {
        int dist[N];
        bfs_shortest_paths(s, dist);
        for (int t = s + 1; t < N; ++t) {
            printf("(%d,%d): %d\n", s, t, dist[t]);
        }
    }
    return 0;
}
