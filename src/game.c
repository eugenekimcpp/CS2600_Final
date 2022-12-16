/*

    Program: game.c
    Programmer: Eguene Kim
    Description: This program will perform a simple tic tac toe game

*/
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <mosquitto.h> 


// functoin prototypes
void makeMove();
void displayBoard();
void resetBoard();
int checkWinner();
int isFull();
void player1Move();
void player2Move();


// global variables
char board[3][3];
char winner; 
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
            //reset the setting
            int status = 0; 
            int who = 0; 
            int counter = 2; 
            resetBoard();
        
            do{
                // one game play
                printf("current status is: \n");
                displayBoard();
                who = counter % 2;
                if(who == 0)
                {
                    //player 1 move
                }
                else
                {
                    //player 2 move
                }
                ++counter; 
                status = checkWinner(); 

            }while (status == 0);

        // display the result
         if((status == 1) && (winner == X))
            {
                displayBoard();
                printf("Player1 win!!!\n");
            }
            else if((status == 1) && (winner == O))
            {
                displayBoard();
                printf("Player2 win!!!\n");
            }
            else
            {
                displayBoard();
                printf("It's a tie\n");
            }


        // prompt user1 if want to continue
        printf("Play again? 1)yes 2)quit ");
        scanf("%d", &continue);
        // ******ESP CODE HERE
        if(continue == 0)
        {
            printf("Thanks for playing!\n");
            return 0;
        }
      } while(continue == 1);  

    }
    
    return 0;
}


void displayBoard()
{
    printf("+-----------+\n");
    printf("| %c | %c | %c |\n", board[0][0],board[0][1],board[0][2]);
    printf("+-----------+\n");
    printf("| %c | %c | %c |\n", board[1][0],board[1][1],board[1][2]);
    printf("+-----------+\n");
    printf("| %c | %c | %c |\n", board[2][0],board[2][1],board[2][2]);
    printf("+-----------+\n");
}

void resetBoard()
{
    for(int i = 0; i < 3; ++i)
    {
        for(int j = 0; j < 3; ++j)
        {
            board[i][j] = ' ';
        }
    }
}

int isFull()
{
    
    for(int i = 0; i < 3; ++i)
    {
        for(int j = 0; j<3;++j)
        {
            if(board[i][j] == ' ')
            {
                // if encounter any open spot, then it not full
                return 0; 
            }
        }
    }

    return 1; 
}

int checkWinner()
{
    // initially 0 = progressing
    int result = 0; 
    int check; 

    if((board[0][0] == board[0][1]) && (board[0][1] == board[0][2]) && board[0][0] != ' ') 
    {
        // Column check start
        winner = board[0][0]; // store winning character 
        return 1;
    }
    else if((board[1][0] == board[1][1]) && (board[1][1] == board[1][2]) && board[1][0] != ' ') 
    {
        winner = board[1][0];
        return 1;    
    }
    else if((board[2][0] == board[2][1]) && (board[2][1] == board[2][2]) && board[2][0] != ' ')
    {
        winner = board[2][0];
        return 1; 
    } 
    else if((board[0][0] == board[1][0]) && (board[1][0] == board[2][0]) && board[0][0] != ' ')
    {
        // ROW check start
        winner = board[0][0];
        return 1; 
    }
    else if((board[0][1] == board[1][1]) && (board[1][1] == board[2][1]) && board[0][1] != ' ')
    {
        winner = board[0][1];
        return 1; 
    }
    else if((board[0][2] == board[1][2]) && (board[1][2] == board[2][2]) && board[0][2] != ' ')
    {
        winner = board[0][2];
        return 1; 
    }
    else if((board[0][0] == board[1][1]) && (board[1][1] == board[2][2]) && board[0][0] != ' ')
    {
        // diagnal check start
        winner = board[0][0];
        return 1; 
    }
    else if((board[0][2] == board[1][1]) && (board[1][1] == board[2][0]) && board[0][2] != ' ')
    {
        winner = board[0][2];
        return 1; 
    }
    else
    {
        check = isFull(); 

        //enter here if no match found
        if(check == 1)
        {
            //enter here when board is full, but no matching winner found
            return -1; // tie
        }
        else
        {
            return 0; //still progressing
        }

    }

    return result;
    


}
