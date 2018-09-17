#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ARRAY_SIZE 18627
#define NUMBER_SIZE 40

int static numSwaps = 0;
int static numComparisons = 0;

typedef struct game{
    char title[NUMBER_SIZE], platform[NUMBER_SIZE];
    int score, year;
}game;

void printArray(game* array);
char* readIn(char *s);
void quickSort(game* array, int p, int r);
int partition(game* array, int p, int r);
int next_field( FILE *csv, char *buffer);
game* fetch_node( FILE *csv);

int main( int argv, char* argc[])
{
    int p = 0, r = ARRAY_SIZE - 1, counter = 0;
    game array[ARRAY_SIZE];
    game* curr;
    FILE *csv;
    csv = fopen("ign.csv","r");
    if (csv == NULL)
        perror("The file failed to load");
    while( !feof(csv)  )/*!feof(csv)*/
        {
            curr = fetch_node(csv);
            array[counter] = *curr;
            printf("%s,%s,%d,%d\n", curr->title, curr->platform, curr->score, curr->year);
            counter++;
        }
    quickSort(array, p, r);
    printArray(array);
    return 0;
}

void quickSort(game* array, int p, int r)
{
    if(p < r)
    {
        int q = partition( array, p, r);
        quickSort( array, p, q-1);
        quickSort( array, q+1, r);
    }
    else
    {
        return;
    }

}

int partition(game* array, int p, int r)
{
    int pivot = array[r].score;
    int hi = r - 1;
    int lo = p;
        for(;;)
        {
            while( array[lo].score < pivot && lo != r-1 )
            {
                lo++;
                numComparisons++;
            }
            while( array[hi].score > pivot && hi != p )
            {
                hi--;
                numComparisons++;
            }
            if( lo == hi && array[lo].score > pivot)
            {
                game temp = array[r];
                array[r] = array[hi];
                array[hi] = temp;
                numSwaps++;
                return hi; 
            }
            else if( lo >= hi )
            {
                game temp = array[r];
                array[r] = array[hi+1];
                array[hi+1] = temp;
                numSwaps++;
                return hi+1;
            }
            else
            {
                
                game temp = array[lo];
                array[lo] = array[hi];
                array[hi] = temp;
                numSwaps++;
                lo++;
                hi--;
            }
        }

}

void printArray(game* array)
{
    printf("SORTED: ");
    for(int i = 0; i < ARRAY_SIZE; i++)
    {
        printf("%d,%s,%s,%d\n", array[i].score, array[i].title, array[i].platform, array[i].year);
    }
    printf("\nSWAPS: %d\n", numSwaps);
    printf("COMPARISONS: %d\n", numComparisons);
    printf("\n\n");
    return;
}

char* readIn(char *s){ //fgets without newline
    char *a;
    a = fgets(s, ARRAY_SIZE, stdin);
    if(!a) {return a;}
    
    if (s[strlen(s) - 1] == '\n') {s[strlen(s) - 1] = '\0';}

    return s;
}

game* fetch_node( FILE *csv){
char field[NUMBER_SIZE];
    game *newgame = (game*)malloc(sizeof(game));

    next_field( csv, newgame->title);  // name and type are just strings so read
    next_field( csv, newgame->platform); // those directly into the struct

    next_field( csv, field);      // load id into buffer as string
    newgame->score = atoi(field);

    next_field( csv, field);      // load id into buffer as string
    newgame->year = atoi(field);

    return newgame;
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
        if( i<NUMBER_SIZE-1 ) { ++i; } 
    }

    buffer[i] = 0; // null terminate the string
    return end; // flag stating whether or not this is end of the line
}