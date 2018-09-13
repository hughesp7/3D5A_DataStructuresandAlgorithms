#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_WORD 40
#define ARRAY_SIZE 120344

typedef struct Node{
    char forename[MAX_WORD], surname[MAX_WORD], dep_ID[MAX_WORD], person_type[MAX_WORD], gender[MAX_WORD], nationality[MAX_WORD], religion[MAX_WORD], occupation[MAX_WORD];
    int person_id, age;
    struct Node* next;

}Node;

typedef struct element{
    char surname[MAX_WORD];
    Node* head;
    int count;

}element;

int hash_fn(char* s);
int next_field( FILE *csv, char *buffer);
void add_hash(element* table, Node* field, int hash, int* numterms, int* collisions );
void hash_search(element* table, int numterms, int collisions);
void print_list(Node *list);
void add_node(Node **head, Node* curr);
Node* fetch_node(FILE *csv);
char* read_in(char *s);

int main(int argv, char* argc[])
{
    FILE *csv;
    csv = fopen("people.csv","r");
    if (csv == NULL)
        perror("The file failed to load");
    int return_case, hash, numterms = 0, collisions = 0, counter = 0;
    Node* curr;
    char field[MAX_WORD];
    element table[ARRAY_SIZE];
    memset(table, 0, sizeof(element)*ARRAY_SIZE);

        while( !feof(csv)  )/*!feof(csv)*/
        {
            curr = fetch_node(csv);
            hash = hash_fn(curr->surname);
            add_hash(table, curr, hash, &numterms, &collisions);
            printf("%d,%s,%s,%s,%d,%s,%s,%s,%s,%s\n", curr->person_id, curr->dep_ID, curr->surname, curr->forename, curr->age, curr->person_type, curr->gender, curr->nationality, curr->religion, curr->occupation);
            counter++;
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
    return (31*hash)%ARRAY_SIZE;
}

void hash_search(element* table, int numterms, int collisions){
    char str1[MAX_WORD], end[5] = "quit";
    int hash = 0, boolean = 0;
    printf("Enter a name you would like to search for, to finish enter quit\n");
    read_in(str1);
    hash = hash_fn(str1);
    while(strcmp(str1, end) != 0)
    {
        while(table[hash].surname[0] != '\0')
        {
            if(strcmp(table[hash].surname, str1) == 0)
            {
                printf("That surname was found %d times\n", table[hash].count);
                printf("The following people were found with that surname\n");
                print_list(table[hash].head);
                boolean = 1;
                break;
            }
            hash++;
            if(hash >= ARRAY_SIZE)
            {
                hash = hash -ARRAY_SIZE;
            }
        }
        if(boolean == 0)
            printf("No such name was found\n");
        printf("Enter a name you would like to search for, to finish enter quit\n");
        scanf("%s", str1);
        hash = hash_fn(str1);
        boolean = 0;
    }
}

void print_list(Node *list){
    Node* curr = list;
        while(curr != NULL)
        {
            printf("%d,%s,%s,%s,%d,%s,%s,%s,%s,%s\n", curr->person_id, curr->dep_ID, curr->surname, curr->forename, curr->age, curr->person_type, curr->gender, curr->nationality, curr->religion, curr->occupation);
            curr = curr->next;
        }
 }

 char* read_in(char *s){ //fgets without newline
    char *a;
    a = fgets(s, ARRAY_SIZE, stdin);
    if(!a) {return a;}
    
    if (s[strlen(s) - 1] == '\n') {s[strlen(s) - 1] = '\0';}

    return s;
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
		if( i<MAX_WORD-1 ) { ++i; } 
	}

	buffer[i] = 0; // null terminate the string
	return end; // flag stating whether or not this is end of the line
}



void add_hash(element* table, Node* field, int hash, int* numterms, int* collisions ){
    int store;
    if(table[hash].surname[0] == '\0')
    {
        strcpy(table[hash].surname, field->surname);
        table[hash].count++;
        add_node(&table[hash].head, field);
        (*numterms)++;
        return;
    }
    else if( strcmp(table[hash].surname, field->surname) == 0 ) 
    {  
        table[hash].count++;
        add_node(&table[hash].head, field);
        (*numterms)++;
        return;
    }
    else
    {
        store = hash;
        hash++;
        while( hash != store)
        {
            
            if(hash >= ARRAY_SIZE)
            {
                hash = hash - ARRAY_SIZE;
            }
            if(strcmp(table[hash].surname, field->surname) == 0)
            {
                table[hash].count++;
                add_node(&table[hash].head, field);
                (*numterms)++;
                return;
            }
            else if(table[hash].surname[0] == '\0')
            {
                strcpy(table[hash].surname, field->surname);
                table[hash].count++;
                add_node(&table[hash].head, field);
                (*numterms)++;
                return;
            }
            hash++;
            (*collisions)++;
        }
    
    }
}

void add_node(Node **head, Node* curr){
    curr->next = *head;
    *head = curr;
}

Node* fetch_node( FILE *csv) {
    char field[MAX_WORD];
    Node *newnode = (Node*)malloc(sizeof(Node));

    next_field( csv, field);      // load id into buffer as string
    newnode->person_id = atoi(field);                       // then parse to integer

    next_field( csv, newnode->dep_ID);  // name and type are just strings so read
    next_field( csv, newnode->surname); // those directly into the struct
    next_field( csv, newnode->forename);

    next_field( csv, field);      // load id into buffer as string
    newnode->age = atoi(field);

    next_field( csv, newnode->person_type);
    next_field( csv, newnode->gender);
    next_field( csv, newnode->nationality);
    next_field( csv, newnode->religion);
    next_field( csv, newnode->occupation);
    

    // do string compare to get legendary as bool
    
    return newnode;
}