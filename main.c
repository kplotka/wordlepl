#include <stdio.h>
#include <stdlib.h>


int main() {

    char username[100];
    printf("podaj nazwe uzytkownika:\n");
    scanf("%s", &username);

    char dict[1000][5];

    FILE* filePointer;
    int bufferLength = 6;
    char buffer[bufferLength];

    filePointer = fopen("config/dictionary.txt", "r");
    if (filePointer == NULL)
    {
        perror("Nie udalo sie otworzyc pliku config/dictionary.txt");
        return 1;
    }

    while(fgets(buffer, bufferLength, filePointer)) {
        printf("%s\n", buffer);
    }

    fclose(filePointer);

    return 0;
}

