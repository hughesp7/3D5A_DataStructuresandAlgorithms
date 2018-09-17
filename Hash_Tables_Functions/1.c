#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_WORD 40
#define ARRAY_SIZE 104

typedef struct element{
    char string[MAX_WORD];
    int count;

}element;

int hash_fn(char* s);
int next_field(FILE * csv , char * buffer);
void add_hash(element* table, char* field, int hash, int* numterms, int* collisions );
void hash_search(element* table, int numterms, int collisions);
char* read_in(char *s);

int main(int argv, char* argc[])
{
    FILE *csv;
    csv = fopen("names.csv","r");
    if (csv == NULL)
        perror("The file failed to load");
    int return_case, hash, numterms = 0, collisions = 0;
    char field[MAX_WORD];
    element table[ARRAY_SIZE];
    memset(table, 0, sizeof(element)*ARRAY_SIZE);

        while( return_case != 2)
        {
            return_case = next_field(csv, field);
            hash = hash_fn(field);
            add_hash(table, field, hash, &numterms, &collisions);
        }
    printf("Capacity: %d\nNumterms: %d\nCollisions: %d\n", ARRAY_SIZE, numterms, collisions);
    hash_search(table, numterms, collisions);
    
    fclose(csv);
    return 0;
}

int hash_fn(char* s){
    int hash = 0;
    while(*s)
    {
        hash = hash + *s;
        s++;
    }
    
    return hash%ARRAY_SIZE;
}

char* read_in(char *s){ //fgets without newline
    char *a;
    a = fgets(s, ARRAY_SIZE, stdin);
    if(!a) {return a;}
    
    if (s[strlen(s) - 1] == '\n') {s[strlen(s) - 1] = '\0';}

    return s;
}

void hash_search(element* table, int numterms, int collisions){
    char str1[MAX_WORD], end[5] = "QUIT";
    int hash = 0, boolean = 0;
    printf("Enter a name you would like to search for, to finish enter QUIT\n");
    read_in(str1);
    hash = hash_fn(str1);
    while(strcmp(str1, end) != 0)
    {
        while(table[hash].string[0] != '\0')
        {
            if(strcmp(table[hash].string, str1) == 0)
            {
                printf("That name was found %d times\n", table[hash].count);
                boolean = 1;
                break;
            }
            hash++;
            if(hash == ARRAY_SIZE)
            {
                hash = 0;
            }
        }
        if(boolean == 0)
            printf("No such name was found\n");
        printf("Enter a name you would like to search for, to finish enter QUIT\n");
        scanf("%s", str1);
        hash = hash_fn(str1);
        boolean = 0;
    }
}



int next_field( FILE * csv , char * buffer)
{
    int counter = 0;
    int boolean = 0;
    char c = fgetc(csv); 
    while(counter < MAX_WORD)
    {
        if( c == '"')
        {
            c = fgetc(csv);
            if( boolean == 0)
            {
                boolean = 1;
            }
            else
            {
                boolean = 0;
            }
        }
        if(c == ',' && boolean == 0)
        {
            buffer[counter] = ' ';
            counter++;
            buffer[counter] = '\0';
            return 1;
        
        }
        else if( c == '\n')
        {
            buffer[counter] = '\0';
            return 1;
        }
        else if(feof(csv))
        {
            return 2;
        }
        else
        {
            buffer[counter] = c;
            counter++;
            c = fgetc(csv);
        }
    } 
    return 2;   
}

void add_hash(element* table, char* field, int hash, int* numterms, int* collisions ){
    if(table[hash].string[0] == '\0')
    {
        strcpy(table[hash].string, field);
        table[hash].count++;
        (*numterms)++;
        return;
    }
    else if( strcmp(table[hash].string, field) == 0 ) 
    {  
        table[hash].count++;
        (*numterms)++;
        return;
    }
    else
    {
        
        while(table[hash].string[0] != '\0')
        {
            hash++;
            (*collisions)++;
            if(hash == ARRAY_SIZE)
            {
                hash = 0;
            }
            if(strcmp(table[hash].string, field) == 0)
            {
                table[hash].count++;
                (*numterms)++;
                return;
            }
            else if(table[hash].string[0] == '\0')
            {
                strcpy(table[hash].string, field);
                table[hash].count++;
                (*numterms)++;
                return;
            }
        }
    
    }
}