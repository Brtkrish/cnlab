#include <stdio.h>

#define MAX 10
#define INF 999

void printPath(int parent[], int j) {
    if (parent[j] == -1)
        return;
    printPath(parent, parent[j]);
    printf(" -> %d", j);
}

int main() {
    int n, cost[MAX][MAX], dist[MAX], visited[MAX], parent[MAX];
    int i, j, min, next, src;

    printf("Enter number of routers: ");
    scanf("%d", &n);

    printf("Enter the cost matrix:\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            scanf("%d", &cost[i][j]);
            if (cost[i][j] == 0 && i != j)
                cost[i][j] = INF;
        }
    }

    printf("Enter source router: ");
    scanf("%d", &src);

    for (i = 0; i < n; i++) {
        dist[i] = cost[src][i];
        visited[i] = 0;
        if (dist[i] < INF && i != src)
            parent[i] = src;
        else
            parent[i] = -1;
    }

    dist[src] = 0;
    visited[src] = 1;

    for (i = 0; i < n - 1; i++) {
        min = INF;
        for (j = 0; j < n; j++) {
            if (!visited[j] && dist[j] < min) {
                min = dist[j];
                next = j;
            }
        }

        visited[next] = 1;

        for (j = 0; j < n; j++) {
            if (!visited[j] && (min + cost[next][j] < dist[j])) {
                dist[j] = min + cost[next][j];
                parent[j] = next;
            }
        }
    }

    printf("\nShortest paths from router %d:\n", src);
    for (i = 0; i < n; i++) {
        if (i == src) continue;
        printf("To %d: Distance = %d, Path = %d", i, dist[i], src);
        printPath(parent, i);
        printf("\n");
    }

    return 0;
}
