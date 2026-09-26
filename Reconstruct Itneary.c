#include <stdlib.h>
#include <string.h>

// A simple dictionary entry to represent the adjacency list for an airport
typedef struct {
    char src[4];
    char** destinations;
    int size;
    int capacity;
} AirportNode;

// Comparator to sort destinations lexicographically descending
int compareDestinations(const void* a, const void* b) {
    return strcmp(*(const char**)b, *(const char**)a);
}

// Comparator to sort the adjacency list entries by source airport ascending
int compareAirports(const void* a, const void* b) {
    return strcmp(((AirportNode*)a)->src, ((AirportNode*)b)->src);
}

// Binary search helper to find the index of a source airport in our graph layout
int findAirportIndex(AirportNode* graph, int graphSize, const char* src) {
    int low = 0, high = graphSize - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        int cmp = strcmp(graph[mid].src, src);
        if (cmp == 0) return mid;
        if (cmp < 0) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}

// Hierholzer's DFS traversal algorithm
void dfs(const char* airport, AirportNode* graph, int graphSize, char** itinerary, int* itinerarySize) {
    int idx = findAirportIndex(graph, graphSize, airport);
    
    if (idx != -1) {
        // While there are remaining outgoing flights from this airport
        while (graph[idx].size > 0) {
            // Pop the lexicographically smallest destination (stored at the end)
            char* next_dest = graph[idx].destinations[--(graph[idx].size)];
            dfs(next_dest, graph, graphSize, itinerary, itinerarySize);
        }
    }
    
    // Path block is exhausted; push current airport to the raw path log (in reverse order)
    itinerary[(*itinerarySize)++] = strdup(airport);
}

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
char** findItinerary(char*** tickets, int ticketsSize, int* ticketsColSize, int* returnSize) {
    // 1. Collect unique source airports to build our graph structures
    AirportNode* graph = (AirportNode*)malloc(ticketsSize * sizeof(AirportNode));
    int graphSize = 0;

    for (int i = 0; i < ticketsSize; i++) {
        char* src = tickets[i][0];
        char* dst = tickets[i][1];

        // Linear scan to insert/find the unique source node template
        int idx = -1;
        for (int j = 0; j < graphSize; j++) {
            if (strcmp(graph[j].src, src) == 0) {
                idx = j;
                break;
            }
        }

        if (idx == -1) {
            idx = graphSize++;
            strcpy(graph[idx].src, src);
            graph[idx].capacity = 4;
            graph[idx].size = 0;
            graph[idx].destinations = (char**)malloc(graph[idx].capacity * sizeof(char*));
        }

        if (graph[idx].size >= graph[idx].capacity) {
            graph[idx].capacity *= 2;
            graph[idx].destinations = (char**)realloc(graph[idx].destinations, graph[idx].capacity * sizeof(char*));
        }
        graph[idx].destinations[graph[idx].size++] = dst;
    }

    // Sort graph keys to enable binary search lookups later
    qsort(graph, graphSize, sizeof(AirportNode), compareAirports);

    // Sort destinations descending so that popping from the back gets the smallest string
    for (int i = 0; i < graphSize; i++) {
        qsort(graph[i].destinations, graph[i].size, sizeof(char*), compareDestinations);
    }

    // 2. Allocate the final itinerary path space (Total nodes = ticketsSize + 1)
    char** itinerary = (char**)malloc((ticketsSize + 1) * sizeof(char*));
    int itinerarySize = 0;

    // 3. Run Hierholzer's algorithm starting from "JFK" as mandated by the problem rule
    dfs("JFK", graph, graphSize, itinerary, &itinerarySize);

    // 4. Reverse the accumulated itinerary to restore the correct chronological order
    for (int i = 0; i < itinerarySize / 2; i++) {
        char* temp = itinerary[i];
        itinerary[i] = itinerary[itinerarySize - 1 - i];
        itinerary[itinerarySize - 1 - i] = temp;
    }

    // Clean up graph structures allocation
    for (int i = 0; i < graphSize; i++) {
        free(graph[i].destinations);
    }
    free(graph);

    *returnSize = itinerarySize;
    return itinerary;
}