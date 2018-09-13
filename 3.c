#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

// Max number of edges a node can have (shouldn't need this)
#define MAX_EDGES 120
#define MAX_STOP_NAME 40

struct edge {
    struct vertex* edge;
    struct vertex* from;
    struct edge* next;
    int weight;
};

struct vertex {
    int stop_id, num_edges;
    char stop_name[MAX_STOP_NAME];
    struct edge* head;
    struct vertex* predecessor;
    struct vertex* next;
    double latitude, longitude, distance_to_source;
    
};

struct graph {
    int max_vertices;        // Maximum size of the graph. Do we need a limit?
    int num_vertices;        // current size of the graph
    struct vertex *vertices; // pointer to all nodes in the graph
};

// Functions which manipulate vertices. 

struct vertex* vertex_new();
int            vertex_init ( struct vertex *v );
int            vertex_add_edge ( struct vertex *v, struct edge* new );
void           vertex_print    ( struct vertex *v );
void           vertex_free     ( struct vertex *v );
void           removeList( struct vertex **head, struct vertex* curr);
struct vertex* smallestDist( struct vertex* head);

struct edge*   edge_new        ();
int            edge_init( struct edge* e);
struct edge*   removeEdge(struct edge** head, struct edge* curr );
void           enqueue(struct edge **head, struct edge* curr);
struct edge*   pop(struct edge** head );

void           addList  (struct vertex **head, struct vertex* curr);
//struct vertex* pop(struct vertex **head);
void           printStack( struct vertex* head);
char           findNotVisited( struct vertex* curr );

struct vertex* getNextVertexDFS( struct vertex* curr, struct vertex** head );
void           addEdges( struct vertex* curr, struct vertex** head );
struct edge*   makeEdgeTo( FILE *csv, struct graph* g);
struct edge*   makeEdgeFrom( FILE *csv, struct graph* g);
// Functions which manipulate the graph

struct graph*  graph_new        ( int max_vertices );
int            graph_init       ( struct graph *g, int max_vertices );
struct vertex* graph_get_vertex ( struct graph *g, int id );
int            graph_add_vertex ( struct graph *g, struct vertex* new );
void           graph_print      ( struct graph *g );
void           graph_free       ( struct graph *g );

struct vertex* makeVertex( FILE *csv );
int next_field( FILE *csv, char *buffer);

int depthFirstSearch( struct graph* g, char id );
int breathFirstSearch( struct graph* g, char id );
int dijkstra( struct graph* g, int idfrom, int idto);
void distPrint( struct graph* g, struct vertex* source, int idto);

int main(int argc, char *argv[]){

    struct graph *graph;
    struct vertex* curr_vertex = NULL;
    struct edge* curr_edge = NULL;
    FILE *csv;
    csv = fopen("vertices.csv","r");
    if (csv == NULL)
        perror("The file failed to load");
    // Initialization
    graph = graph_new( 4810 );
    if (!graph) { return EXIT_FAILURE; }
    // Could load this from a file, but we'll hard code it for demo purposes
    while( !feof(csv) )
    {
        curr_vertex = makeVertex( csv );
        graph_add_vertex( graph, curr_vertex );
    }
    
    csv = fopen("edges.csv","r");
    if (csv == NULL)
        perror("The file failed to load");

    while( !feof(csv) )
    {
        curr_edge = makeEdgeTo( csv, graph );
        vertex_add_edge( curr_edge->from, curr_edge);
    }
    fclose(csv);
    csv = fopen("edges.csv","r");
    if (csv == NULL)
        perror("The file failed to load");

    while( !feof(csv) )
    {
        curr_edge = makeEdgeFrom( csv, graph );
        vertex_add_edge( curr_edge->from, curr_edge);
    }
    graph_print(graph);
    dijkstra( graph, 300, 1189 );

    //breathFirstSearch( graph, 'A');
    //depthFirstSearch( graph, 'A' );
    // Do stuff
    

    // terminate
    graph_free(graph);

    return EXIT_SUCCESS;
}

int next_field( FILE *csv, char *buffer) {
    int i=0, end=0, quoted=0;
    
    for(;;) {
        // fetch the next character from file        
        buffer[i] = fgetc(csv);
        // if we encounter quotes then flip our state and immediately fetch next char
        if(buffer[i]=='"') { quoted=!quoted; buffer[i] = fgetc(csv); }
        // end of field on comma if we're not inside quotes
        if(buffer[i]==',' && !quoted) { break; }
        // end record on newline or end of file
        if(feof(csv) || buffer[i]=='\n') { end=1; break; } 
        // truncate fields that would overflow the buffer
        if( i<MAX_STOP_NAME-1 ) { ++i; } 
    }

    buffer[i] = 0; // null terminate the string
    return end; // flag stating whether or not this is end of the line
}

struct vertex* makeVertex( FILE *csv ){
    char field[MAX_STOP_NAME];
    struct vertex *newvertex = vertex_new();
    
    next_field( csv, field);    // load id into buffer as string
    newvertex->stop_id = atoi(field);                       // then parse to integer
    
    next_field( csv, field);
    strcpy( newvertex->stop_name, field );

    next_field( csv, field);      // load id into buffer as string
    newvertex->latitude = atof(field);
    next_field( csv, field);      // load id into buffer as string
    newvertex->longitude = atof(field);  
    
    return newvertex;
}

struct edge* makeEdgeTo( FILE *csv, struct graph* g){
    char field[MAX_STOP_NAME];
    struct edge *newedge = edge_new();

    next_field( csv, field);      // load id into buffer as string
    newedge->from = graph_get_vertex( g, atoi(field) );                       // then parse to int
    next_field( csv, field);      // load id into buffer as string
    newedge->edge = graph_get_vertex( g, atoi(field) );
    next_field( csv, field);      // load id into buffer as string
    newedge->weight = atoi(field);  
    
    return newedge;
}

struct edge* makeEdgeFrom( FILE *csv, struct graph* g){
    char field[MAX_STOP_NAME];
    struct edge *newedge = edge_new();

    next_field( csv, field);      // load id into buffer as string
    newedge->edge = graph_get_vertex( g, atoi(field) );                       // then parse to int
    next_field( csv, field);      // load id into buffer as string
    newedge->from = graph_get_vertex( g, atoi(field) );
    next_field( csv, field);      // load id into buffer as string
    newedge->weight = atoi(field);  
    return newedge;
}

int dijkstra( struct graph* g, int id, int idto)
{
    struct vertex* source = graph_get_vertex( g, id);
    struct vertex* curr = source;
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
    distPrint( g, source, idto);
    return 1;
}

void distPrint( struct graph* g, struct vertex* source, int idto)
{
    struct vertex *sorted[g->max_vertices];
    int counter = 0;

            struct vertex* curr = graph_get_vertex( g, idto );
            struct vertex* temp = curr;
            while( curr != source)
            {
                sorted[counter] = curr;
                curr = curr->predecessor;
                counter++;
            }
            sorted[counter] = source;
            printf("Shortest path from %s to %s is\n", source->stop_name, temp->stop_name);
            for(int x = counter; x >= 0; x--)
            {
                printf("%d  %s  %f  %f\n", sorted[x]->stop_id, sorted[x]->stop_name, sorted[x]->latitude, sorted[x]->longitude);
            }
            printf("\n");
            counter = 0;
    
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

struct vertex* vertex_new () {
    struct vertex *v;

    // I did not test malloc here because I know that vertex_init will do it
    // for me. If there is an issue with the allocation then vertex_init will
    // return 0
    v = malloc ( sizeof(struct vertex) );
    if(!vertex_init( v ) ) {
        // On failure, just to be safe, free whatever got allocated
        vertex_free(v);
        v = NULL;
    }
    return v;
}

int vertex_init ( struct vertex *v ) {
    // Check that we have a vertex before we do anything
    if ( v == NULL ) { return 0; }

    // Initialize everything
    v->stop_id = 0;
    v->num_edges = 0;
    v->next = NULL;
    v->distance_to_source = INFINITY;
    v->longitude = 0;
    v->latitude = 0;
    v->head = NULL;
    v->predecessor = NULL;

    // return successful initialization
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

int edge_init( struct edge* e)
{
    if( e == NULL ) { return 0; }

    e->weight = 0;
    e->edge = NULL;
    e->next = NULL;
    e->from = NULL;
}

struct edge* edge_new ()
{
    struct edge* e = malloc(sizeof(struct edge));
    if(!edge_init( e) )
    {
        e = NULL;
    }
    return e;
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

void vertex_print ( struct vertex *v ) {
    // Check that we have a vertex before we do anything
    if (!v) { return; }

    // Print vertex label
    printf("%s", v->stop_name);
   
    // Print vertex edges
    struct edge* curr = v->head;

    if ( curr == NULL) { printf("\n"); return; }

    printf(" -> %s =  %d ", curr->edge->stop_name, curr->weight);
    while( curr->next != NULL) {
        curr = curr->next;
        printf(" -> %s =  %d ", curr->edge->stop_name, curr->weight);
        
    }

    printf("\n");
}

void
vertex_free ( struct vertex *v ) {
    if (v) { free(v); }
}

int vertex_add_edge ( struct vertex *v, struct edge* new )
{
    if ( !v ) { return 0; }

    // Check to see that we haven't reached max capacity of edges
    if ( v->num_edges >= MAX_EDGES ) { return 0; }

    // If we have room, store the edge and update this vertex's edge count
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
    
    /*curr->next = *head;
    *head = curr;
    return;*/
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

struct vertex* graph_get_vertex ( struct graph *g, int id )
{
    // Good idea to make sure that g actually points to something
    if (!g) { return NULL; }
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // This linear search to find a vertex is horrible!
    // What datastructures can we use to make it faster?
    struct vertex *v = NULL;
    for ( int i=0; i<g->num_vertices; i++ ) {
        if ( g->vertices[i].stop_id == id ) {
            v = &g->vertices[i];
            break;
        }
    }

    // v will point to the vertex if we found it. Else it will be NULL
    return v;
}

int graph_add_vertex( struct graph *g, struct vertex* new )
{
    // First make sure that we've been given a graph to operate on
    if (!g) { return 0; }
    // Make sure the graph is not full
    if (g->num_vertices >= g->max_vertices) { return 0; }

    // Check to see if the vertex is already in the graph    
    struct vertex *v = graph_get_vertex(g, new->stop_id);
    // If it is, report failure
    if (v) { return 0; }

    // Now try to add the vertex to the graph.    
    g->vertices[g->num_vertices] =  *new;

    // If init was successful then increase the vertices count
    g->num_vertices++;

    return 1;
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