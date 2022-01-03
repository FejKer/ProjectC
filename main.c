#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "checkMove.c"
#include "gameEnd.c"
#include "undoMove.c"
#include "print.c"

int sec = 0;
int min = 0;

void *timer(void *threadid){
    sleep(1);
    sec++;
    if(sec % 60 == 0){
        sec = 0;
        min++;
    }
}


int main() {
    system("clear");
    char input[255];
    int wrongCommand = 0;
    int wrong = 0;
    int round = 1;
    int column = 0;
    char name1[255];
    char name2[255];
//    int sec = 0, min = 0;                     //timer
    FILE *fptr;
    fptr = fopen("data.bin", "ab+");            //plik z danymi o graczach i wynikach
    char array[6][7];                           //tablica charow, zeby byl czytelnieszy output
    for (int i = 0; i < 6; i++){
        for(int j = 0; j < 7; j++){             //zerowanie tablicy
            array[i][j] = 5;
        }
    }
    printMenu();
    empty_stdin();
    printf("Podaj imie gracza 1.\n");
    scanf("%[^\n]s", name1);
    empty_stdin();
    printf("Podaj imie gracza 2.\n");
    scanf("%[^\n]s", name2);

    int undo = 0;
    int failedUndo = 0;
    while(1){
        printField(array, name1, name2, round, wrong, wrongCommand, failedUndo);
        wrong = 0;
        wrongCommand = 0;
        printf("\n");
        printf("\n");
        printf("Podaj numer kolumny do ktorej chcesz wrzucic zeton.\n");
        printf("Lub wpisz 0 aby wejsc do menu.\n");
        scanf("%d", &column);
        if(column == 0){
            int ret = printMenuInGame(array, column, round, undo);
            if(ret == 1){
                if(!undo){
                    round--;
                } else {
                    failedUndo = 1;
                }
                undo = 1;       //cofniecie ruchu
                continue;
            } else if(ret == 2){
                main();           //powrot do menu programu
            }
        }
        if(column > 7 || column < 1){
            wrongCommand = 1;
            empty_stdin();
            continue;
        }
        if(checkColumn(array, column)){
            if(round % 2 == 1){
                insertCoin(array, column, 'c');         //wrzucenie zetonu
            } else {
                insertCoin(array, column, 'z');
            }
            undo = 0;
            failedUndo = 0;
        } else {
            wrong = 1;
            round--;
        }

        if(checkDraw(array)){
            printf("Gra zakonczona remisem.\n");
        }
        if(checkWin(array, round)){
            printField(array, name1, name2, round, wrong, wrongCommand, failedUndo);
            printf("\n");
            printf("Gra zakonczona.\n");
            printf("Wygrywa ");                     //sprawdzenie zakonczenia gry
            if(round % 2 == 1){
                printf("%s!\n", name1);
            } else {
                printf("%s!\n", name2);
            }
            sleep(5);
            break;
        }
        round++;
    }




    pthread_t thread_id; 
    pthread_create(&thread_id, NULL, timer, NULL);          //rozpoczecie watku z timerem do zrobienia



    return 0;
}