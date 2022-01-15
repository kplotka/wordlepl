#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


int main() {

    char username[100];
    printf("podaj nazwe uzytkownika:\n");

    if (!fgets(username, sizeof username, stdin)) {
        printf("wrong input\n");
        return 1;
    }

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
    int dictLength = 0;
    while(fgets(buffer, bufferLength, filePointer)) {
       if (buffer[0] == '\n') {
           continue;
       }

       for (int i = 0; i<5; i++) {
           dict[dictLength][i] = buffer[i];
       }
       dictLength++;
    }
    fclose(filePointer);

    int seed;
    time_t tt;
    seed = time(&tt);
    srand(seed);

    int target = rand() % dictLength;

    printf("zgadnij 5-literowe slowo (bez polskich znakow!):\n");

    for(;;){
        char guess[100];
        if (!fgets(guess, sizeof guess, stdin)) {
           printf("wrong input\n");
           continue;
        }

        if (strlen(guess) != 6) {
            printf("wrong input, put 5 letter word\n");
            continue;
        }

        // TODO(kplotka): Zwaliduj slowo w slowniku, czy wgl istnieje.

        // strcmp(guess, dict[target]) nie działa dla nas, bo dict[X] jest char[5], jednak strcmp czasem czyta wiecej elementow
        // gdyz jest to dwuwymarowa lista charow.

        int correctLetters = 0;

        for (int i = 0; i<5; i++) {
            if (guess[i] == dict[target][i]) {
                printf("[%c]", guess[i]);
                correctLetters++;
                continue;
            }
            int found = 0;
            for (int k = 0; k<5; k++){
                if (guess[i] == dict[target][k]){
                    found = 1;
                    break;
                }
            }
            if (found == 1){
                printf("(%c)", guess[i]);
                continue;
            }
                printf("-%c-", guess[i]);
        }
        printf("\n");

        if (correctLetters == 5){
            printf("brawo, wygrana!");
            return 0;
        }

    }


    return 0;
}

