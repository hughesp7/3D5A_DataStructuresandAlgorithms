#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

// Max number of edges a node can have (shouldn't need this)
#define MAX_EDGES    8 

struct edge {
    struct vertex* edge;
    struct edge* next;
    int weight;
};

struct vertex {
    char id;
    struct edge* head;
    struct vertex* predecessor;
    struct vertex* next;
    int num_edges;
    int permanent;
    double distance_to_source;
    
};

struct graph {
    int max_vertices;        // Maximum size of the graph. Do we need a limit?
    int num_vertices;        // current size of the graph
    struct vertex *vertices; // pointer to all nodes in the graph
};

// Functions which manipulate vertices. 

struct vertex* vertex_new      ( char id );
int            vertex_init     ( struct vertex *v, char id );
int            vertex_add_edge ( struct vertex *v, struct vertex* to , int weight );
void           vertex_print    ( struct vertex *v );
void           vertex_free     ( struct vertex *v );
void           removeList( struct vertex **head, struct vertex* curr);
struct vertex* smallestDist( struct vertex* head);
void           printList( struct vertex* head);

struct edge*   edge_new        ( int weight );
int            edge_init( struct edge* e, int weight);
struct edge*   removeEdge(struct edge** head, struct edge* curr );
void           enqueue(struct edge **head, struct edge* curr);
struct edge*   pop(struct edge** head );

void           addList  (struct vertex **head, struct vertex* curr);
//struct vertex* pop(struct vertex **head);
void           printStack( struct vertex* head);
char           findNotVisited( struct vertex* curr );

struct vertex* getNextVertexDFS( struct vertex* curr, struct vertex** head );
void           addEdges( struct vertex* curr, struct vertex** head );


// Functions which manipulate the graph

struct graph*  graph_new        ( int max_vertices );
int            graph_init       ( struct graph *g, int max_vertices );
struct vertex* graph_get_vertex ( struct graph *g, char id );
int            graph_add_vertex ( struct graph *g, char id );
int            graph_add_edge   ( struct graph *g, char id1, char id2, int weight );
void           graph_print      ( struct graph *g );
void           graph_free       ( struct graph *g );

int depthFirstSearch( struct graph* g, char id );
int breathFirstSearch( struct graph* g, char id );
int dijkstra( struct graph* g, char id);
void distPrint( struct graph* g, char id);

int main(int argc, char *argv[]){

    struct graph *graph;
    
    // Initialization
    graph = graph_new( 7 );
    if (!graph) { return EXIT_FAILURE; }

    // Could load this from a file, but we'll hard code it for demo purposes
    for ( char c='A'; c<='G'; c++ ) {
        graph_add_vertex( graph, c );
    }

    graph_add_edge ( graph, 'A', 'B', 1 );
    graph_add_edge ( graph, 'A', 'C', 3 );
    graph_add_edge ( graph, 'A', 'F', 10 );
    graph_add_edge ( graph, 'B', 'C', 1 );
    graph_add_edge ( graph, 'B', 'D', 7 );
    graph_add_edge ( graph, 'B', 'E', 5 );
    graph_add_edge ( graph, 'B', 'G', 2 );
    graph_add_edge ( graph, 'C', 'B', 1 );
    graph_add_edge ( graph, 'C', 'D', 9 );
    graph_add_edge ( graph, 'C', 'E', 3 );
    graph_add_edge ( graph, 'D', 'E', 2 );
    graph_add_edge ( graph, 'D', 'F', 1 );
    graph_add_edge ( graph, 'E', 'D', 2 );
    graph_add_edge ( graph, 'E', 'F', 2 );
    graph_add_edge ( graph, 'G', 'D', 12 );

    graph_print(graph);

    dijkstra( graph, 'A');

    //breathFirstSearch( graph, 'A');
    //depthFirstSearch( graph, 'A' );
    printf("we did it\n");
    // Do stuff
    

    // terminate
    graph_free(graph);

    return EXIT_SUCCESS;
}
/*char findNotVisited( struct vertex* curr){
    char lowest = '\0';
    int counter = 0;
    while( counter != curr->num_edges )
    {
        if( curr->edges[counter]->visited == 0 )
        {
            lowest = curr->edges[counter]->id;
            return lowest;
        }
        counter++;
    }
}*/

/*struct vertex* getNextVertexDFS( struct vertex* curr, struct vertex** head)
{
    char lowest = findNotVisited( curr );
    if( lowest == '\0') { return pop( head ); }
    
    struct vertex* temp = NULL;
    struct vertex* best = NULL;
    int boolean = 0;
    for(int i = 0; i < curr->num_edges; i++ )
    {
        temp = curr->edges[i];
        if( temp->visited == 0 && lowest >= temp->id )
        {
            boolean = 1;
            lowest = temp->id;
            best = temp;
        }
    }
    if( boolean == 0 ) { return pop( head ); }
    
    addList( head, best );
    return best;
}*/

/*int depthFirstSearch( struct graph* g, char id )
{
    char array[g->max_vertices];
    int counter = 0;
    struct vertex* curr = graph_get_vertex( g, id );
    struct vertex* head = NULL;
    addList( &head, curr);

    while( curr != NULL)
    {
        printStack( head );
        if( curr->visited == 0)
        {
            array[counter] = curr->id;
            counter++;
            curr->visited = 1;
        }
        printf( "Working on -> %c\n\n", curr->id);
        curr = getNextVertexDFS( curr, &head);
        
    }
    
    for(int i = 0; i < counter; i++)
    {
        printf("%c,", array[i] );
    }
    printf("\n");
    for( int i = 0; i < g->num_vertices; i++) { (&g->vertices[i])->visited = 0;}
    return 1;
}*/





/*void addList(struct vertex **head, struct vertex* curr)
{
    curr->next = *head;
    *head = curr;
}

int breathFirstSearch( struct graph* g, char id )
{
    struct vertex* curr = graph_get_vertex( g, id );
    struct vertex* head = NULL;
    enqueue( &head, curr);

    while( curr != NULL )
    {
       printf("%c\n", curr->id );
       addEdges( curr, &head);
       curr = pop( &head );
    }
    printf("\n");
    for( int i = 0; i < g->num_vertices; i++) { (&g->vertices[i])->visited = 0;}
    return 1;
}

void addEdges( struct vertex* curr, struct vertex** head )
{

    for( int i = 0; i < curr->num_edges; i++)
    {
        if( curr->edges[i]->visited == 0)
        {
            enqueue( head, curr->edges[i]);
            curr->edges[i]->visited = 1;
        }
    }
    return;
}

int enqueue(struct vertex **head, struct vertex* curr)
{
    struct vertex* prev = *head;
    struct vertex* current = *head;
    if( *head  == NULL)
    {
        curr->next = NULL;
        *head = curr;
        return 1;
    }
    
    while( current != NULL )
    {
        //printf("%c", current->id);
        if( current->next == NULL)
        {
            current->next = curr;
            return 1;
        }
        current = current->next;
    }
    return 0;
}

struct vertex* pop(struct vertex** head ){
    return *head = (*head)->next;
}

void printStack( struct vertex* head)
{
    struct vertex* curr = head;
    while( curr != NULL)
    {
        printf("%c,", curr->id);
        curr = curr->next;
    }
    printf("\n");
}*/

int dijkstra( struct graph* g, char id)
{
    struct vertex* curr = graph_get_vertex( g, id);
    curr->distance_to_source = 0;
    struct vertex* head = NULL;
    for(int i = 0; i < g->num_vertices; i++)
    {
       addList( &head, &g->vertices[i] ); 
    }
    while( head != NULL )
    {
        curr = smallestDist( head );
        //printf("working on %c\n", curr->id );
        removeList( &head, curr);
        for( int i = 0; i < curr->num_edges; i++)
        {
            struct edge* temp = pop( &curr->head );
            int alt = curr->distance_to_source + temp->weight;
            if( alt < temp->edge->distance_to_source)
            {
                temp->edge->distance_to_source = alt;
                temp->edge->predecessor = curr;
                //printf("Setting %c distance to source to %d and predecessor to %c\n",temp->edge->id, alt, curr->id);
            }
        }
        
    }
    /*for(int i = 1; i < g->num_vertices; i++)
    {
       printf("%c has a predecessor of %c\n",g->vertices[i].id, g->vertices[i].predecessor->id); 
    }*/
    distPrint( g, 'A');
    return 1;
}

void distPrint( struct graph* g, char id)
{
    char sorted[g->max_vertices];
    int counter = 0;
    struct vertex* source = graph_get_vertex( g, id);

    for( int i = 0; i < g->num_vertices; i++)
    {
        struct vertex* curr = &g->vertices[i];
        struct vertex* temp = curr;
        while( curr != source)
        {
            sorted[counter] = curr->id;
            curr = curr->predecessor;
            counter++;
        }
        sorted[counter] = 'A';
        printf("Shortest path from %c to %c is\n", id, temp->id);
        for(int x = counter; x >= 0; x--)
        {
            printf("%c, ", sorted[x]);
        }
        printf("\n");
        counter = 0;
    }
}

void printList( struct vertex* head)
{
    struct vertex* curr = head;
    while( curr != NULL)
    {
        printf("%c,", curr->id);
        curr = curr->next;
    }
    printf("\n");
}

struct edge* pop(struct edge** head ){
    struct edge* temp = *head;
    *head = (*head)->next;
    return temp;
}

void addList(struct vertex **head, struct vertex* curr)
{
    curr->next = *head;
    *head = curr;
}

struct vertex* smallestDist( struct vertex* head)
{
    double distance = INFINITY;
    struct vertex* curr = head;
    struct vertex* best = NULL;
    while( curr != NULL)
    {
        if( curr->distance_to_source < distance)
        {
            distance = curr->distance_to_source;
            best = curr;
        }
        curr = curr->next;
    }
    return best;
}

void removeList( struct vertex **head, struct vertex* curr)
{
    struct vertex* current = *head;
    struct vertex* prev = NULL;
    if( *head == NULL)  { return; }

    if( curr == *head)
    {
        *head = curr->next;
        curr = NULL;
        return;
    }

    while( current != curr)
    {
        prev = current;
        current = current->next;
    }
    prev->next = curr->next;
    curr = NULL;

}

struct vertex*
vertex_new ( char id ) {
    struct vertex *v;

    // I did not test malloc here because I know that vertex_init will do it
    // for me. If there is an issue with the allocation then vertex_init will
    // return 0
    v = malloc ( sizeof(struct vertex) );
    if(!vertex_init(v, id)) {
        // On failure, just to be safe, free whatever got allocated
        vertex_free(v);
        v = NULL;
    }
    return v;
}

struct edge* edge_new ( int weight )
{
    struct edge* e = malloc(sizeof(struct edge));
    if(!edge_init( e, weight) )
    {
        e = NULL;
    }
    return e;
}

struct graph *
graph_new ( int max_vertices ) {
    struct graph *g;

    // I did not test malloc here because I know that graph_init will do it
    // for me. If there is an issue with the allocation then graph_init will
    // return 0
    g = malloc(sizeof(struct graph));
    if (!graph_init ( g, max_vertices )) {
        // On failure, just to be safe, free whatever got allocated
        graph_free(g);
        g = NULL;
    }

    // g will point to a new graph on success or NULL on failure
    return g;
}

int vertex_init ( struct vertex *v, char id ) {
    // Check that we have a vertex before we do anything
    if ( v == NULL ) { return 0; }

    // Initialize everything
    v->id = id;
    v->num_edges = 0;
    v->next = NULL;
    v->distance_to_source = INFINITY;
    v->head = NULL;
    v->predecessor = NULL;
    v->permanent = 0;

    // return successful initialization
    return 1;
}

int edge_init( struct edge* e, int weight)
{
    if( e == NULL ) { return 0; }

    e->weight = weight;
    e->edge = NULL;
}

int
graph_init ( struct graph *g, int max_vertices ) {
    // Good idea to make sure that g actually points to something
    if (!g) { return 0; };

    // Set everything up
    g->max_vertices = max_vertices;
    g->num_vertices = 0;
    // Try to malloc out some space for the vertices
    g->vertices = malloc(sizeof(struct vertex) * g->max_vertices);
    // if malloc fails then this function should report failure
    if(g->vertices == NULL) { return 0; }

    // return success
    return 1;
}

void
vertex_print ( struct vertex *v ) {
    // Check that we have a vertex before we do anything
    if (!v) { return; }

    // Print vertex label
    printf("%c", v->id);
   
    // Print vertex edges
    struct edge* curr = v->head;

    if ( curr == NULL) { printf("\n"); return; }

    printf(" -> %c =  %d ", curr->edge->id, curr->weight);
    while( curr->next != NULL) {
        curr = curr->next;
        printf(" -> %c =  %d ", curr->edge->id, curr->weight);
        
    }

    printf("\n");
}

void
vertex_free ( struct vertex *v ) {
    if (v) { free(v); }
}

int vertex_add_edge ( struct vertex *v, struct vertex* to, int weight ) 
{
    if ( !v ) { return 0; }

    // Check to see that we haven't reached max capacity of edges
    if ( v->num_edges >= MAX_EDGES ) { return 0; }

    // If we have room, store the edge and update this vertex's edge count
    struct edge* new = edge_new( weight );
    new->edge = to;
    enqueue( &v->head, new);
    v->num_edges++;

    return 1;
}

void enqueue(struct edge **head, struct edge* curr)
{
    struct edge* prev = *head;
    struct edge* current = *head;
    if( *head  == NULL)
    {
        curr->next = NULL;
        *head = curr;
        return;
    }
    
    while( current != NULL )
    {
        //printf("%c", current->id);
        if( current->next == NULL)
        {
            current->next = curr;
            return;
        }
        current = current->next;
    }
    return;
}
    

struct edge*   removeEdge(struct edge** head, struct edge* curr ){

}


struct vertex * graph_get_vertex ( struct graph *g, char id ) 
{
    // Good idea to make sure that g actually points to something
    if (!g) { return NULL; }
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // This linear search to find a vertex is horrible!
    // What datastructures can we use to make it faster?
    struct vertex *v = NULL;
    for ( int i=0; i<g->num_vertices; i++ ) {
        if ( g->vertices[i].id == id ) {
            v = &g->vertices[i];
            break;
        }
    }

    // v will point to the vertex if we found it. Else it will be NULL
    return v;
}

int
graph_add_vertex ( struct graph *g, char id ) 
{
    // First make sure that we've been given a graph to operate on
    if (!g) { return 0; }
    // Make sure the graph is not full
    if (g->num_vertices >= g->max_vertices) { return 0; }

    // Check to see if the vertex is already in the graph    
    struct vertex *v = graph_get_vertex(g, id);
    // If it is, report failure
    if (v) { return 0; }

    // Now try to add the vertex to the graph.    
    if (!vertex_init( &g->vertices[g->num_vertices], id )) { 
        return 0;
    }

    // If init was successful then increase the vertices count
    g->num_vertices++;

    // return success
    return 1;
}

int graph_add_edge ( struct graph *g, char id1, char id2, int weight )
{
    // First make sure that we've been given a graph to operate on
    if (!g) { return 0; }

    // Ensure both vertices are actually in the graph
    struct vertex *v1 = graph_get_vertex(g, id1);
    struct vertex *v2 = graph_get_vertex(g, id2);
    if (!v1 || !v2) { return 0; }

    // return the result of the vertex_add_edge function (1 if edge added, 0
    // if there was a problem)
    return vertex_add_edge( v1, v2, weight );
}

void graph_print ( struct graph *g )
{
    if (!g) { return; }

    // Iterate over all vertices in the graph and print them
    for ( int i=0; i<g->num_vertices; i++ ){
        vertex_print( &g->vertices[i] );
    }
    printf("\n");
}

void
graph_free ( struct graph *g ){
    // Do some checks and free whetever needs to be freed
    if (g) {
        if (g->vertices) { free(g->vertices); }
        free(g);
    }
}