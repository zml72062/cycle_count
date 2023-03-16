#include <stdlib.h>

/**
 *  Linked list
 */
typedef struct List {
    long node;
    struct List* next;
} List;

/**
 *  Return an array of "list of neighbors" of every node, according to
 *  the given "edge_index" (in shape [2, num_of_edges]).
 * 
 *  @param edge_index the adjacency matrix, in shape [2, num_of_edges]
 *  @param num_nodes the number of nodes
 *  @param num_edges the number of edges
 *  
 *  @return an malloced array of "list of neighbors"
 */
static List** get_neighbor_list(long* edge_index, long num_nodes, long num_edges)
{
    List** neighbor_list = (List**)malloc(sizeof(List*) * num_nodes);
    for (long i = 0; i < num_nodes; i++)
    {
        List* ptr = (neighbor_list[i] = (List*)malloc(sizeof(List)));
        /**
         *  "ptr" is the current linked-list block awaiting to write
         */
        for (long j = 0; j < num_edges; j++)
        {
            if (edge_index[j] == i) /* edge start from i */
            {
                /* allocate the next linked-list block */
                ptr->next = (List*)malloc(sizeof(List));
                
                /* record the end point of the edge */
                ptr->node = edge_index[j + num_edges];

                ptr = ptr->next;
            }
        }
        ptr->node = -1; /* mark the last node of the linked list */
        ptr->next = NULL;
    }
    return neighbor_list;
}

/**
 *  Free the array of "list of neighbors" constructed by "get_neighbor_list"
 *  
 *  @param neighbor_list the malloced array of "list of neighbors"
 *  @param num_nodes the number of nodes
 */
static void free_neighbor_list(List** neighbor_list, long num_nodes)
{
    for (long i = 0; i < num_nodes; i++)
    {
        /* Free linked list at "neighbor_list[i]" */
        List *ptr = neighbor_list[i], *ptr2 = ptr->next;
        while (ptr2 != NULL)
        {
            free(ptr);
            ptr = ptr2;
            ptr2 = ptr->next;
        }
        free(ptr);
    }
    free(neighbor_list);
}

/**
 *  Check if node j is a neighbor of node i.
 * 
 *  @param neighbor_list the malloced array of "list of neighbors"
 *  @param i the first node
 *  @param j the second node
 *  
 *  @return whether node j is a neighbor of node i
 */
static int is_neighbor_of(List** neighbor_list, long i, long j)
{
    List* ptr = neighbor_list[i];
    while (ptr->node != -1)
    {
        if (ptr->node == j)
        {
            return 1;
        }
        ptr = ptr->next;
    }
    return 0;
}

/**
 *  Count paths of given length k that start at node i and end at node j.
 *
 *  @param neighbor_list the malloced array of "list of neighbors"
 *  @param k the path length
 *  @param start the starting node
 *  @param end the ending node
 *  @param num_nodes number of nodes
 *  @param have_passed a vector to record whether a node has been visited,
 *  initially all 0 with length "num_nodes"
 * 
 *  @return number of length-k paths that start from "start" and end at "end"
 */
static long count_paths(List** neighbor_list, long k, long start, long end, long num_nodes,
                        int* have_passed)
{
    have_passed[start] = 1;
    have_passed[end] = 1;

    if (k == 1)
    {
        return is_neighbor_of(neighbor_list, start, end);
    }

    long count = 0;
    for (List* ptr = neighbor_list[start]; ptr->node != -1; ptr = ptr->next)
    {
        long try_node = ptr->node;
        if (have_passed[try_node] == 1)
            continue;
        
        count += count_paths(neighbor_list, k - 1, try_node, end, num_nodes,
                             have_passed);
        have_passed[try_node] = 0;
    }
    return count;
}

/**
 *  Count cycles of given length k that pass a node i.
 *  
 *  @param neighbor_list the malloced array of "list of neighbors"
 *  @param k the cycle length
 *  @param node the starting node
 *  @param num_nodes number of nodes
 *  
 *  @return number of k-cycles that pass node "node" 
 */
static long count_cycles(List** neighbor_list, long k, long node, long num_nodes)
{
    int count = 0;
    for (List* ptr = neighbor_list[node]; ptr->node != -1; ptr = ptr->next)
    {
        int* have_passed = (int*)calloc(num_nodes, sizeof(int));
        count += count_paths(neighbor_list, k - 1, node, ptr->node, num_nodes, 
                             have_passed);
        free(have_passed);
    }
    return count / 2;
}

/**
 *  Node-level count cycles for every node in a graph.
 * 
 *  @param edge_index the adjacency matrix, in shape [2, num_of_edges]
 *  @param num_nodes the number of nodes
 *  @param num_edges the number of edges
 *  @param k the cycle length
 *  @param cycle_list the array in which to store the cycle counts, in
 *  shape [num_nodes, ]
 */
void graph_count_cycles(long* edge_index, long num_nodes, long num_edges, long k,
                        long* cycle_list)
{
    List** neighbor_list = get_neighbor_list(edge_index, num_nodes, num_edges);
    for (int i = 0; i < num_nodes; i++)
    {
        cycle_list[i] = count_cycles(neighbor_list, k, i, num_nodes);
    }
    free_neighbor_list(neighbor_list, num_nodes);
}