#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define READING_LINES 233
#define NUMBER_OF_QUESTIONS 11
#define ERROR_CODE_IN_INPUT_SCANNING 'e'
#define MAX_NAME_LENGTH 32

/*
 *Location:     Temple University Computer Science
 *Programmer:   Destin Charles
 *Class:        Introduction to computer programming in C
 *Assignment:   Write a program that combines the file from the previous
 *              lab and make a CSV file containing all the information
 *Date:         12/9/2018
 *Version:      1
 *Description:  This program reads in the files newomr.txt and test_taker_names.txt,
 *              matches the names with id's and displays them along side the
 *              percentags. It then saves all the information in a CSV file
 *              
 */

typedef struct {
        char * delim;
        unsigned int rows;
        unsigned int cols;
        char ** table;
} CSV;

int csv_destroy(CSV * csv);
CSV * csv_create(unsigned int cols, unsigned int rows);
char * csv_get(CSV * csv, unsigned int col, unsigned int row);
int csv_set(CSV * csv, unsigned int col, unsigned int row, char * value);
int csv_save(CSV * csv, char * filename);
void csv_display(CSV * csv);

int main()
{
        int i = 0;
        int j = 0;
        int k = 0;
        int count = 0;
        int temp_grade = 0;
        char temp_char;
        char temp_id[6];
        char temp_name[MAX_NAME_LENGTH];
        char tempScore[NUMBER_OF_QUESTIONS];
        int ids1[READING_LINES];
        int ids2[READING_LINES];
        char id_arr[READING_LINES][6];
        char name_arr[READING_LINES][MAX_NAME_LENGTH];
        char str_ans_arr[READING_LINES][NUMBER_OF_QUESTIONS][2];
        char correct_arr[NUMBER_OF_QUESTIONS] = {'a','d','d','a','b','b','c','a','d','a','d'};
        char grade_arr[READING_LINES][6];
        CSV * csv;

        FILE *IN1 = fopen("newomr.txt", "r");
        FILE *IN2 = fopen("test_taker_names.txt", "r");
        csv = csv_create(14, READING_LINES+1);
        char csv_set_arr[14][12] = { "Identity", "Name", "Q1", "Q2", "Q3", "Q4", "Q5", "Q6", "Q7", "Q8", 
"Q9", "Q10", "Q11", "Grade"};
        for( i = 0; i < 14; i++ )
                csv_set(csv, i,0, csv_set_arr[i]);
        i = 0;
        while(fscanf(IN1, "%d", &ids1[i]) == 1)
        {       
                if(i >= READING_LINES)
                {
                        break;
                }
                for ( k = 0; k < NUMBER_OF_QUESTIONS; k++ )
                {
                        fscanf( IN1, "%c", &tempScore[k]);
                }
                j = 0;
                while(fscanf(IN2, "%d", &ids2[j]) == 1)
                {
                        if(j >= READING_LINES)
                        {
                                break;
                        }
                        for ( k = 0; k < MAX_NAME_LENGTH; k++ )
                        {
                                fscanf( IN2, "%c", &temp_char );
                                if(temp_char == '\n')
                                {
                                        temp_name[k] = '\0';
                                        break;
                                }
                                temp_name[k] = temp_char;
                        }
                        if(ids1[i] == ids2[j])
                        {
                                temp_grade = 0;
                                sprintf(temp_id, "%5.5d", ids1[i]);
                                temp_id[5] = '\0';
                                for(k = 0; k < 6; k++)
                                {
                                        id_arr[i][k] = temp_id[k];
                                }
                                for(k = 0; k < MAX_NAME_LENGTH; k++)
                                {
                                        name_arr[i][k] = temp_name[k];
                                        if(temp_name[k] == '\0')
                                        {
                                                break;
                                        }
                                }
                                for(k = 0; k < NUMBER_OF_QUESTIONS; k++)

                                {

                                        str_ans_arr[i][k][0] = tempScore[k];
                                        str_ans_arr[i][k][1] = '\0';
                                        if(tempScore[k] == correct_arr[k])
                                        {
                                                temp_grade+=100;
                                        }
                                }
                                sprintf(grade_arr[i],"%d%%\0", (temp_grade/NUMBER_OF_QUESTIONS));
                                break;
                        }
                        j++;
                }
                i++;
        }
        for(k = 0; k < READING_LINES; k++)
        {
                csv_set(csv, 0, 1+k, id_arr[k]);
                csv_set(csv, 1, 1+k, name_arr[k]);
                csv_set(csv, 13,1+k, grade_arr[k]);
                for(i = 0; i < NUMBER_OF_QUESTIONS; i++)
                {
                        csv_set(csv,2+i, 1+k, str_ans_arr[k][i]);
                }
        }               
        csv_display(csv);
        csv_save(csv, "full_data.csv");
        csv_destroy(csv);
}

/*
 *Functoin:     csv_destroy
 *Description:  gets rid of CSV file
 *Author:       Destin Charles
 *Date:         12/9/2018
 *Inputs:       CSV * csv
 *Outputs:      CSV * csv
 *Globals:      none
 *Returns:      0
 */


int csv_destroy(CSV * csv) {
        if (csv == NULL) { return 0; }
        if (csv->table != NULL) { free(csv->table); }
        if (csv->delim != NULL) { free(csv->delim); }
        free(csv);
        return 0;
}

/*
 *Functoin:     csv_create
 *Description:  Makes space for the CSV file
 *Author:       Destin Charles
 *Date:         12/9/2018
 *Inputs:       unsigned int cols,   unsigned int rows
 *Outputs:      none
 *Globals:      none
 *Returns:      csv, NULL
 */


CSV * csv_create(unsigned int cols, unsigned int rows) 
{
        CSV * csv;

        csv = malloc(sizeof(CSV));
        csv->rows = rows;
        csv->cols = cols;
        csv->delim = strdup(",");

        csv->table = malloc(sizeof(char *) * cols * rows);
        if (csv->table == NULL) { goto error; }

        memset(csv->table, 0, sizeof(char *) * cols * rows);

        return csv;

        error:
                csv_destroy(csv);
                return NULL;
}

/*
 *Functoin:     csv_get
 *Description:  gets values for csv file for each row and column
 *Author:       Destin Charles
 *Date:         12/9/2018
 *Inputs:       unsigned int col, unsigned int row, CSV * csv
 *Outputs:      CSV * csv
 *Globals:      none
 *Returns:      csv (as a table)
 */

char * csv_get(CSV * csv, unsigned int col, unsigned int row) 
{
        unsigned int idx;
        idx = col + (row * csv->cols);
        return csv->table[idx];
}

/*
 *Functoin:     csv_set
 *Description:  givess values for csv file for each row and column
 *Author:       Destin Charles
 *Date:         12/9/2018
 *Inputs:       unsigned int col, unsigned int row, CSV * csv, char * value
 *Outputs:      CSV * csv
 *Globals:      none
 *Returns:      0
 */


int csv_set(CSV * csv, unsigned int col, unsigned int row, char * value) 
{
        unsigned int idx;
        idx = col + (row * csv->cols);
        csv->table[idx] = value;
        return 0;
}

/*
 *Functoin:     csv_save
 *Description:  opens csv file then stores and saves contents in it
 *Author:       Destin Charles
 *Date:         12/9/2018
 *Inputs:       CSV * csv, char * filename
 *Outputs:      CSV * csv
 *Globals:      none
 *Returns:      0
 */


int csv_save(CSV * csv, char * filename) 
{
        FILE * fp;
        int row, col;
        char * content;

        fp = fopen(filename, "w");
        for (row=0; row<csv->rows; row++)
        {
                for (col=0; col<csv->cols; col++)
                {
                        content = csv_get(csv, col, row);
                        fprintf(fp, "%s%s", content,
                        ((col == csv->cols-1) ? "" : csv->delim) );
                }
                fprintf(fp, "\n");
        }

        fclose(fp);
        return 0;
}

/*
 *Functoin:     csv_display
 *Description:  displays copntents of CSV file
 *Author:       Destin Charles
 *Date:         12/9/2018
 *Inputs:       CSV * csv
 *Outputs:      CSV * csv
 *Globals:      none
 *Returns:      none
 */

void csv_display(CSV * csv) {
        int row, col;
        char * content;

        if ((csv->rows == 0) || (csv->cols==0))
        {
                printf("[Empty table]\n");
                return ;
        }
        printf("\n[Table cols=%d rows=%d]\n", csv->cols, csv->rows);
        for (row=0; row<csv->rows; row++)
        {
                printf("[|");
                for (col=0; col<csv->cols; col++)
                {
                        content = csv_get(csv, col, row);
                        printf("%s\t|", content);
                }
                printf("]\n");
        }
        printf("\n");
}
