#include <stdio.h>
#include <stdlib.h>

int next_field ( FILE * csv , char * buffer , int max_len );

int main( int argc, char * argv [] ){

    FILE *csv;
    csv = fopen("pokemon.csv","r");
    int max_len = 250, return_case = 0;
    char field[max_len];
    if (csv == NULL)
        perror("The file failed to load");

        while( return_case != 2)
        {
            return_case = next_field(csv, field, max_len);
            if( return_case == 0 )
            {
                printf("%s\n", field);
            }
            if( return_case == 1 )
            {
                printf("%s\n", field);
            }
        }
    fclose(csv);
    system("pause");
    return 0;
}

int next_field ( FILE * csv , char * buffer , int max_len )
{
    int counter = 0;
    int boolean = 0;
    char c = fgetc(csv); 
    while(counter < max_len)
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
            return 0;
        
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
}
