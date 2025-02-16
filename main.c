#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILENAME_LENGTH 64
#define MAX_INPUT_LENGTH 100
#define MAX_COLUMN_NAME_LENGTH 64

typedef struct
{
    int COL_Type;
    char COL_Name[MAX_COLUMN_NAME_LENGTH];
} Column;

typedef struct
{
    FILE *LoadedTable;
    Column *Configs;
    int Column_Count;
    char *TableName;
    int *DataStart;
} TABLE;

int EvaluateFileName(const char *f_name)
{
    size_t len = strlen(f_name);
    return (len >= 3 && strcmp(f_name + len - 3, ".tb") == 0) ? 0 : 1;
}

int getValidatedOption()
{
    char input[MAX_INPUT_LENGTH];
    char *errorptr;
    int option;

    do
    {
        printf("Actions:\n1. Load Pre-existing File\n2. Write New File\n=> ");
        scanf("%s", input);
        option = strtoll(input, &errorptr, 10);
        while (getchar() != '\n')
            ;
    } while (*errorptr != '\0' || (option != 1 && option != 2));

    return option;
}

void getFileName(char *FileName)
{
    do
    {
        printf("Enter a Data File\n=> ");
        fgets(FileName, MAX_FILENAME_LENGTH, stdin);
        FileName[strcspn(FileName, "\n")] = '\0';
    } while (EvaluateFileName(FileName) == 1);
}

TABLE *loadFile()
{
    char FileName[MAX_FILENAME_LENGTH];
    getFileName(FileName);
    FILE *db_file = fopen(FileName, "rb+");

    while (db_file == NULL)
    {
        printf("Invalid file. Please try again.\n");
        getFileName(FileName);
        db_file = fopen(FileName, "rb+");
    }

    printf("File %s Loaded\n", FileName);

    int TableNameSize;
    fread(&TableNameSize, sizeof(int), 1, db_file);
    char TableName[TableNameSize];
    fread(TableName, sizeof(char), TableNameSize, db_file);
    TableName[TableNameSize] = '\0';
    printf("Table Name: %s\nName char Count: %d\n", TableName, TableNameSize);

    int COLS_Count;
    fread(&COLS_Count, sizeof(int), 1, db_file);

    Column *COL_Config = (Column *)malloc(COLS_Count * sizeof(Column));
    if (!COL_Config)
    {
        printf("Memory allocation failed for column configurations!\n");
        fclose(db_file);
        return NULL;
    }

    fread(COL_Config, sizeof(Column), COLS_Count, db_file);

    // printf("\nColumn Configuration\n");
    // for (int i = 0; i < COLS_Count; i++)
    // {
    //     printf("-----\nName: %s\nType: %d\n", COL_Config[i].COL_Name, COL_Config[i].COL_Type);
    // }

    TABLE *run = (TABLE *)malloc(sizeof(TABLE));
    if (!run)
    {
        printf("Memory allocation failed for TABLE structure!\n");
        free(COL_Config);
        fclose(db_file);
        return NULL;
    }

    run->LoadedTable = db_file;
    run->Configs = COL_Config;
    run->Column_Count = COLS_Count;

    run->TableName = (char *)malloc(TableNameSize);
    strcpy(run->TableName, TableName);

    run->DataStart = (int *)ftell(db_file);

    return run;
}

void createFile()
{
    char TableName[MAX_FILENAME_LENGTH];
    printf("Write name of the Table\n=> ");
    fgets(TableName, sizeof(TableName), stdin);
    TableName[strcspn(TableName, "\n")] = '\0';

    char TrueName[MAX_FILENAME_LENGTH];
    snprintf(TrueName, sizeof(TrueName), "%s.tb", TableName);

    printf("\nGiven Name: %s\nTrue Name: %s\n", TableName, TrueName);

    FILE *db_file = fopen(TrueName, "wb+");
    int namesize = strlen(TableName);
    fwrite(&namesize, sizeof(int), 1, db_file);
    fwrite(TableName, sizeof(char), namesize, db_file);

    printf("File Created\nSetup Table columns\nWarning: Table can be configured only once. Be careful\n");

    int COLS;
    do
    {
        printf("Enter Number of Columns\n=> ");
        scanf("%d", &COLS);
        while (getchar() != '\n')
            ;
    } while (COLS <= 0);

    Column configs[COLS];
    printf("Entering Column Configuration\nWarning: Table can be configured only once. Be careful\nFormat: [Column Name] [Column Type]\nKeys: \n[Int -> 1]\n[String_64 -> 2]\n");

    for (int i = 0; i < COLS; i++)
    {
        printf("Configure Column[%d]\n=> ", i);
        scanf("%63s %d", configs[i].COL_Name, &configs[i].COL_Type);
    }

    fwrite(&COLS, sizeof(int), 1, db_file);
    fwrite(configs, sizeof(Column), COLS, db_file);
    fclose(db_file);
}

char **splitString(const char *input, int *numTokens)
{
    char **result = (char **)malloc(16 * sizeof(char *));
    char *tempStr = strdup(input);
    char *token;
    int count = 0;

    token = strtok(tempStr, " ");
    while (token != NULL && count < 16)
    {
        result[count] = strdup(token);
        count++;
        token = strtok(NULL, " ");
    }

    *numTokens = count;
    free(tempStr);
    return result;
}

void freeSplitString(char **arr, int numTokens)
{
    for (int i = 0; i < numTokens; i++)
    {
        free(arr[i]);
    }
    free(arr);
}

int main()
{
    printf("Welcome to Table Editor\n");
    int option = getValidatedOption();
    system("clear");
    if (option == 1)
    {
        TABLE *Table = loadFile();
        if (!Table)
        {
            printf("Table could not be loaded. Exiting");
            return (int)0;
        }

        char Query[128];
        while (1)
        {
            system("clear");
            printf("Table Access : %s\n=> ", Table->TableName);
            fgets(Query, 128, stdin);
            Query[strcspn(Query, "\n")] = '\0';
            int TokenCount;

            char **Instructions = splitString(Query, &TokenCount);

            if (strcmp(Instructions[0], "INFO") == 0)
            {
                for (int i = 0; i < Table->Column_Count; i++)
                {
                    printf("-----\nName: %s\nType: %d\n", Table->Configs[i].COL_Name, Table->Configs[i].COL_Type);
                }

                printf("Querry Complete. Press Enter to Continue");
                getchar();
            }

            free(Instructions);
        }
    }
    else
    {
        createFile();
    }
    return 0;
}
