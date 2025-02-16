#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int EvaluateFileName(char *f_name)
{
    if (strcmp(f_name + strlen(f_name) - 3, ".tb") == 0)
    {
        return 0;
    }
    return 1;
}

int main()
{

    printf("Welcome to Table Editor\n");

    char input[100];
    char *errorptr;

    printf("Actions:\n1. Load Pre-existing File\n2. Write New File\n=> ");

    scanf("%s", input);
    int option = strtoll(input, &errorptr, 10);

    while (*errorptr != '\0' || (option != 1 && option != 2))
    {
        system("clear");
        printf("Invalid selection. Please choose either 1 or 2.\n");
        printf("Actions:\n1. Load Pre-existing File\n2. Write New File\n=> ");
        errorptr = NULL;
        while (getchar() != '\n')
            ;

        scanf("%s", input);
        option = strtoll(input, &errorptr, 10);
    }
    while (getchar() != '\n')
        ;

    system("clear");
    FILE *db_file;
    if (option == 1)
    {
        char FileName[64];

        printf("Enter a Data File\n=> ");
        fgets(FileName, sizeof(FileName), stdin);

        // printf("Took %d Characters\n", strcspn(FileName, "\n"));
        FileName[strcspn(FileName, "\n")] = '\0';
        db_file = fopen(FileName, "r");

        while (db_file == NULL || EvaluateFileName(FileName) == 1)
        {
            system("clear");
            printf("Enter a Data File\n=> ");
            fgets(FileName, sizeof(FileName), stdin);

            // printf("Took %d Characters\n", strcspn(FileName, "\n"));
            FileName[strcspn(FileName, "\n")] = '\0';
            db_file = fopen(FileName, "rb+");
        }

        printf("File %s Loaded\n", FileName);

        int TableNameSize = 0;
        fread(&TableNameSize, sizeof(int), 1, db_file);
        char TableName[TableNameSize];
        fread(&TableName, sizeof(TableName), 1, db_file);

        printf("Table Name : %s\nName char Count : %d", TableName, TableNameSize);
        fclose(db_file);
    }
    else
    {
        printf("Write name of the Table\n=> ");
        char TableName[64];
        fgets(TableName, sizeof(TableName), stdin);
        int namesize = strcspn(TableName, "\n");
        TableName[strcspn(TableName, "\n")] = '\0';


        char TrueName[namesize + 4];
        for (int i = 0; i < namesize; i++)
        {
            TrueName[i] = TableName[i];
        }
        TrueName[namesize] = '.';
        TrueName[namesize + 1] = 't';
        TrueName[namesize + 2] = 'b';
        TrueName[namesize + 3] = '\0';
        

        printf("\n Given Name : %s\nTrueName %s", TableName, TrueName);

        db_file = fopen(TrueName, "wb+");

        fwrite(&namesize, sizeof(int), 1, db_file);
        fwrite(&TableName, sizeof(char), namesize, db_file);
        fclose(db_file);
    }
}
