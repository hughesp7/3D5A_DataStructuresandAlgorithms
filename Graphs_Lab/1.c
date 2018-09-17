#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

// Max number of edges a node can have (shouldn't need this)
#define MAX_EDGES    8 

struct vertex {
    char id;
    
    // An array is very rigid. How do we make this better?
    // How do we handle the case where edges have weights?
    // Maybe it's time for another struct?
    struct vertex* edges[MAX_EDGES];
    int num_edges;
    int visited;
    struct vertex* next;
};

struct graph {
    int max_vertices;        // Maximum size of the graph. Do we need a limit?
    int num_vertices;        // current size of the graph
    struct vertex *vertices; // pointer to all nodes in the graph
};

// Functions which manipulate vertices. 

struct vertex* vertex_new      ( char id );
int            vertex_init     ( struct vertex *v, char id );
int            vertex_add_edge ( struct graph* g, struct vertex *v, char id );
void           vertex_print    ( struct vertex *v );
void           vertex_free     ( struct vertex *v );

void           push  (struct vertex **head, struct vertex* curr);
struct vertex* pop(struct vertex **head);
int            enqueue(struct vertex **head, struct vertex* curr);
void           printStack( struct vertex* head);
char           findNotVisited( struct vertex* curr );

struct vertex* getNextVertexDFS( struct vertex* curr, struct vertex** head );
void           addEdges( struct vertex* curr, struct vertex** head );


// Functions which manipulate the graph

struct graph*  graph_new        ( int max_vertices );
int            graph_init       ( struct graph *g, int max_vertices );
struct vertex* graph_get_vertex ( struct graph *g, char id );
int            graph_add_vertex ( struct graph *g, char id );
int            graph_add_edge   ( struct graph *g, char id1, char id2 );
void           graph_print      ( struct graph *g );
void           graph_free       ( struct graph *g );

int depthFirstSearch( struct graph* g, char id );
int breathFirstSearch( struct graph* g, char id );

int main(int argc, char *argv[]) {

    struct graph *graph;
    
    // Initialization
    graph = graph_new( 7 );
    if (!graph) { return EXIT_FAILURE; }

    // Could load this from a file, but we'll hard code it for demo purposes
    for ( char c='A'; c<='G'; c++ ) {
        graph_add_vertex( graph, c );
    }

    graph_add_edge ( graph, 'A', 'B' );
    graph_add_edge ( graph, 'A', 'D' );
    graph_add_edge ( graph, 'A', 'E' );
    graph_add_edge ( graph, 'B', 'C' );
    graph_add_edge ( graph, 'B', 'D' );
    graph_add_edge ( graph, 'C', 'B' );
    graph_add_edge ( graph, 'C', 'D' );
    graph_add_edge ( graph, 'D', 'F' );
    graph_add_edge ( graph, 'F', 'C' );
    graph_add_edge ( graph, 'E', 'D' );

    graph_print(graph);

    breathFirstSearch( graph, 'A');
    depthFirstSearch( graph, 'A' );

    // Do stuff
    

    // terminate
    graph_free(graph);

    return EXIT_SUCCESS;
}
char findNotVisited( struct vertex* curr){
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
}

struct vertex* getNextVertexDFS( struct vertex* curr, struct vertex** head)
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
    
    push( head, best );
    return best;
}

int depthFirstSearch( struct graph* g, char id )
{
    printf("Depth first search from: %c\n", id);
    char array[g->max_vertices];
    int counter = 0;
    struct vertex* curr = graph_get_vertex( g, id );
    struct vertex* head = NULL;
    push( &head, curr);

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
}





void push(struct vertex **head, struct vertex* curr)
{
    curr->next = *head;
    *head = curr;
}

int breathFirstSearch( struct graph* g, char id )
{
    printf("Breath first search from: %c\n", id);
    struct vertex* curr = graph_get_vertex( g, id );
    struct vertex* head = NULL;
    enqueue( &head, curr);

    while( curr != NULL )
    {
       printf( "Working on -> %c\n\n", curr->id);
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

int
vertex_init ( struct vertex *v, char id ) {
    // Check that we have a vertex before we do anything
    if ( v == NULL ) { return 0; }

    // Initialize everything
    v->id = id;
    v->num_edges = 0;
    v->visited = 0;
    v->next = NULL;

    // return successful initialization
    return 1;
}

void
vertex_print ( struct vertex *v ) {
    // Check that we have a vertex before we do anything
    if (!v) { return; }

    // Print vertex label
    printf("%c", v->id);
   
    // Print vertex edges
    for ( int i=0; i<v->num_edges; i++ ) {
        printf(" -> %c", (v->edges[i])->id);
    }

    printf("\n");
}

void
vertex_free ( struct vertex *v ) {
    if (v) { free(v); }
}

int vertex_add_edge ( struct graph* g, struct vertex *v, char id ) {
    if ( !v ) { return 0; }

    // Check to see that we haven't reached max capacity of edges
    if ( v->num_edges >= MAX_EDGES ) { return 0; }

    // If we have room, store the edge and update this vertex's edge count
    
    v->edges[v->num_edges] = graph_get_vertex( g, id);
    v->num_edges++;

    return 1;
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

struct vertex * graph_get_vertex ( struct graph *g, char id ) {
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
graph_add_vertex ( struct graph *g, char id ) {
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

int graph_add_edge ( struct graph *g, char id1, char id2 ) {
    // First make sure that we've been given a graph to operate on
    if (!g) { return 0; }

    // Ensure both vertices are actually in the graph
    struct vertex *v1 = graph_get_vertex(g, id1);
    struct vertex *v2 = graph_get_vertex(g, id2);
    if (!v1 || !v2) { return 0; }

    // return the result of the vertex_add_edge function (1 if edge added, 0
    // if there was a problem)
    return vertex_add_edge( g, v1, v2->id );
}

void graph_print ( struct graph *g ) {
    if (!g) { return; }

    // Iterate over all vertices in the graph and print them
    for ( int i=0; i<g->num_vertices; i++ ) {
        vertex_print( &g->vertices[i] );
    }
    printf("\n");
}

void
graph_free ( struct graph *g ) {
    // Do some checks and free whetever needs to be freed
    if (g) {
        if (g->vertices) { free(g->vertices); }
        free(g);
    }
}