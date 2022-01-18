#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct attemptStats {
    int last, best, worst, sum, count;
};

int main() {


    printf("Witaj w WordlePL! Twoim zadaniem jest zgadniecie 5-literowego wyrazu w jak najmniejszej ilosci prob.\n Oznaczenia:\n -litera-  -->  dana litera nie znajduje sie w tym wyrazie\n (litera)  -->  dana litera znajduje sie w tym wyrazie, jednak na innym miejscu\n [litera]  -->  dana litera znajduje sie w tym wyrazie dokladnie na tym samym miejscu\n Powodzenia!\n (Po pierwszej rozgrywce twoja nazwa uzytkownika zostanie zapisana i przed nastepna poznasz swoje statystyki!)\n");

    char username[100];
    printf("Podaj nazwe uzytkownika:\n");

    if (!fgets(username, sizeof username, stdin)) {
        printf("wrong input\n");
        return 1;
    }

    // Oblicz wstępne statystyki dla użytkowanika.
    // Format pliku to linie: <nazwa> <ilosc prób>\n.
    FILE* statsFile = fopen("config/stats.txt", "a+");
    if (statsFile == NULL)
    {
        perror("Nie udalo sie otworzyc pliku config/stats.txt");
        return 1;
    }

    struct attemptStats userStats;
    userStats.best = 0;
    userStats.worst = 0;
    userStats.sum = 0;
    userStats.count = 0;
    int bufferLength = 120;
    char buffer[bufferLength];
    while(fgets(buffer, bufferLength, statsFile)) {
        if (buffer[0] == '\n') {
            continue;
        }

        int notFoundUser = 0;
        for (int i = 0; i<strlen(username)-1; i++) {
            if (buffer[i] != username[i]){
                notFoundUser = 1;
                break;
            }
        }
        if (notFoundUser == 1){
            continue;
        }

        // Znajdz czesc ktora odpowiada za liczbe prob.
        // start: strlen(username)-1+1
        // end: strlen(buffer)-1
        int sliceSize = strlen(buffer)-1-strlen(username);
        char* numberSlice = malloc((sliceSize)*sizeof(char*));

        for(int i= 0; i< sliceSize; i++) {
            numberSlice[i] = buffer[strlen(username)+i];
        }
        userStats.last = atoi(numberSlice);

        if(userStats.best == 0 || userStats.best > userStats.last){
            userStats.best = userStats.last;
        }

        if(userStats.worst == 0 || userStats.worst < userStats.last){
            userStats.worst = userStats.last;
        }

        userStats.sum += userStats.last;
        userStats.count++;

    }

    if (userStats.count > 0){
        printf("Twoje statystyki:\n Najlepszy wynik: %d\n Najgorszy wynik: %d\n Srednia: %d\n",
               userStats.best, userStats.worst, userStats.sum/userStats.count);
    }

    // Wczytaj słownik.
    char dict[1000][5];

    FILE* dictFile = fopen("config/dictionary.txt", "r");
    if (dictFile == NULL)
    {
        perror("Nie udalo sie otworzyc pliku config/dictionary.txt");
        return 1;
    }
    int dictLength = 0;
    int bufferLength2 = 6;
    char buffer2[bufferLength2];
    while(fgets(buffer2, bufferLength2, dictFile)) {
       if (buffer2[0] == '\n') {
           continue;
       }

       for (int i = 0; i<5; i++) {
           dict[dictLength][i] = buffer2[i];
       }
       dictLength++;
    }
    fclose(dictFile);

    int seed;
    time_t tt;
    seed = time(&tt);
    srand(seed);

    int target = rand() % dictLength;

    printf("zgadnij 5-literowe slowo (bez polskich znakow!):\n");
    int attemps = 0;

    for(;;){
        char guess[100];
        if (!fgets(guess, sizeof guess, stdin)) {
           printf("wrong input\n");
           continue;
        }

        if (strlen(guess) != 6) {
            printf("slowo ma miec 5 liter\n");
            continue;
        }
        attemps++;

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
            printf("brawo, wygrana w %d probach!", attemps);

            for(int i = 0; i < strlen(username)-1; i++){
                fprintf(statsFile, "%c", username[i]);
            }
            fprintf(statsFile, " %d\n", attemps);
            fclose(statsFile);
            return 0;
        }

    }

}

