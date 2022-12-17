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
void on_connect(struct mosquitto *mosq, void *obj, int rc);
void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg);


// global variables
char board[9];
char winner; 
const char O = 'O';
const char X = 'X';
char winner; // will be used to store user character

int main()
{

    //mqtt setup 
    int rc, id=12;
    mosquitto_lib_init();

    struct mosquitto *mosq;

    mosq = mosquitto_new("pub",true,&id);
    mosquitto_connect_callback_set(mosq, on_connect);
    mosquitto_message_callback_set(mosq, on_message);


    
    rc = mosquitto_connect(mosq, "test.mosquitto.org", 1883, 10);
	if(rc) {
		printf("Could not connect to Broker with return code %d\n", rc);
		return -1;
	}
    printf("We are now connected to the broker!\n");

    // variables
    // use do while loop to mark the borad

    int continue; // check if esp send quit(0) otherwise conitnue
    int player; 

    // seed RNG
    srand(time(0)); 

    // game lobby
    printf("Welcome to Tic-Tac-Toe game. \nWaiting for user1 to start the game...");
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
                    int oFilled = 0;
                    int o; 
                    do{
                        printf("player2's turn (0-8): ");
                        scanf("%d",&o);
                        if(board[o] == ' ')
                        {
                            board[o] = O;
                            char *pub = &(char)o;  
                            mosquitto_publish(mosq,"eugene/omove",1,pub,0,false);
                        }
                        else
                        {
                            continue; 
                        }
                    } while (oFilled == 0);

                }
                ++counter; 
                status = checkWinner(); 
                char *pub_winner = &(char)status; 
                mosquitto_publish(mosq, "eugene/winnerStatus", pub_winner,0,false);

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
    mosquitto_disconnect(mosq);
	mosquitto_destroy(mosq);

	mosquitto_lib_cleanup();
    return 0;
}


void displayBoard()
{
    printf("+-----------+\n");
    printf("| %c | %c | %c |\n", board[0],board[1],board[2]);
    printf("+-----------+\n");
    printf("| %c | %c | %c |\n", board[4],board[5],board[6]);
    printf("+-----------+\n");
    printf("| %c | %c | %c |\n", board[7],board[8],board[9]);
    printf("+-----------+\n");
}

void resetBoard()
{
    for(int i = 0; i < 3; ++i)
    {
        board[i] = ' ';
    }
}

int isFull()
{
    
    for(int i = 0; i < 3; ++i)
    {
        if(board[i][j] == ' ')
            {
                // if encounter any open spot, then it not full
                return 0; 
            }
    }

    return 1; 
}

int checkWinner()
{
    // initially 0 = progressing
    int result = 0; 
    int check; 

    if((board[0] == board[1]) && (board[1] == board[2]) && board[0] != ' ') 
    {
        // Column check start
        winner = board[0]; // store winning character 
        return 1;
    }
    else if((board[3] == board[4]) && (board[4]== board[5]) && board[3] != ' ') 
    {
        winner = board[3];
        return 1;    
    }
    else if((board[6] == board[7]) && (board[7] == board[8]) && board[6] != ' ')
    {
        winner = board[6];
        return 1; 
    } 
    else if((board[0] == board[3]) && (board[3] == board[6]) && board[0] != ' ')
    {
        // ROW check start
        winner = board[0];
        return 1; 
    }
    else if((board[1] == board[4]) && (board[4] == board[7]) && board[1] != ' ')
    {
        winner = board[1];
        return 1; 
    }
    else if((board[2] == board[5]) && (board[5] == board[8]) && board[2] != ' ')
    {
        winner = board[2];
        return 1; 
    }
    else if((board[0] == board[4]) && (board[4] == board[8]) && board[0] != ' ')
    {
        // diagnal check start
        winner = board[0];
        return 1; 
    }
    else if((board[2] == board[4]) && (board[4] == board[6]) && board[2] != ' ')
    {
        winner = board[2];
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

void on_connect(struct mosquitto *mosq, void *obj, int rc) {
	printf("ID: %d\n", * (int *) obj);
	if(rc) {
		printf("Error with result code: %d\n", rc);
		exit(-1);
	}
	mosquitto_subscribe(mosq, NULL, "eugene/xmove", 0);
}

void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg) {
	printf("New message with topic %s: %s\n", msg->topic, (char *) msg->payload);
}