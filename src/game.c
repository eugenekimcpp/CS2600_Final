/*

    Program: game.c
    Programmer: Eguene Kim
    Description: This program will perform a simple tic tac toe game

*/
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

// functoin prototypes
int checkWinner();
int isFull();
void makeMove();
void displayBoard();
void resetBoard();


// global variables
char board[3][3];
const char O = 'O';
const char X = 'X';
char winner; // will be used to store user character

int main()
{

    // variables
    // use do while loop to mark the borad

    int continue; // check if esp send quit(0) otherwise conitnue
    int player; 

    // seed RNG
    srand(time(0)); 

    // game lobby
    printf("Welcome to Tic-Tac-Toe game. \n
            Waiting for user1 to start the game...");
    // *******ESP CODE HERE that modity the continue
    if(continue == 0)
    {
        printf("Thanks for playing!\n");
        return 0;
    }
    else
    {
        // Enter here when user1 wants to play
        do{
        




        // prompt user1 if want to continue
        // ******ESP CODE HERE
        if(continue == 0)
        {
            printf("Thanks for playing!\n");
            return 0;
        }
        else
        {
            // if ESP choose to play more
            resetBoard();
        }

      } while(continue == 1);  

    }
    
    






    return 0;
}


void board()
{


}