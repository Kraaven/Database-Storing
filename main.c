#include <stdio.h>
#include <string.h>

int EvaluateFileName(char* f_name) {
    // Check if the string ends with ".dat" or ".datB"
    if (strcmp(f_name + strlen(f_name) - 5, ".datB") == 0){
        return 0;
    }
    return 1;
}

int main()
{
    char FileName[100];

    printf("Enter a Data File\n=> ");
    fgets(FileName, sizeof(FileName), stdin);
    printf("Took %d Characters\n", strcspn(FileName, "\n"));
    FileName[strcspn(FileName, "\n")] = '\0'; 
    FILE *db_file = fopen(FileName, "r");

    while (db_file == NULL || EvaluateFileName(FileName) == 1)
    {
        printf("File Does not Exist\n");
        printf("Enter a Data File\n=> ");
        fgets(FileName, sizeof(FileName), stdin);
        printf("Took %d Characters\n", strcspn(FileName, "\n"));
        FileName[strcspn(FileName, "\n")] = '\0'; 
        db_file = fopen(FileName, "r");
    }

    printf("File %s Loaded", FileName);
}



