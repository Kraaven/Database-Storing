#include <stdio.h>
#include <string.h>

int main(){
    char FileName[100];

    do
    {
        printf("Give a String \n=>");
        fgets(FileName, sizeof(FileName), stdin);
        printf("Took %d Characters\n", strcspn(FileName, "\n")-1);
        FileName[strcspn(FileName, "\n")] = '\0';

    } while (strcmp(FileName, "") == 0 || strcmp(FileName, " ") == 0);

    for (int i = 0; i < 500; i++)
    {
        printf("%d : %c\n", i, FileName[i]);
    }
    
    
}