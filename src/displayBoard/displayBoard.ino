#include <Keypad.h>


// GPIO set up
int latchPin = 2;          // Pin connected to ST_CP of 74HC595（Pin12）
int clockPin = 4;          // Pin connected to SH_CP of 74HC595（Pin11）
int dataPin = 15;          // Pin connected to DS of 74HC595（Pin14）

// const variables
const int board[] = {                       
  B00100100, 
  B00100100, 
  B11111111, 
  B00100100, 
  B00100100, 
  B11111111, 
  B00100100, 
  B00100100
};

const char O = 'O';
const char X = 'X';
char winnerChar; // later assign with winner char
int winner = 0; // winner flag. 1 if there is winner



// define the symbols on the buttons of the keypad
char keys[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[4] = {14, 27, 26, 25}; // connect to the row pinouts of the keypad
byte colPins[4] = {13, 21, 22, 23};   // connect to the column pinouts of the keypad

// initialize an instance of class NewKeypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, 4, 4);







void setup() {
  

  Serial.begin(115200); // Initialize the serial port and set the baud rate to 115200
  Serial.println("ESP32 is ready!"); 
  
  pinMode(latchPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  displayBoard();
}

void loop() {
  char input;
  do{
    Serial.println("Press '*' to start Tic Tac Toe!");
    input = keypad.getKey();
  } while(input != keys[3][0]);
  
}




// Below is for the boards
void displayBoard()
{
  int cols = B00000001;
  
  for (int i = 0; i < 8; i++){
  matrixRowsVal(board[i]);
  matrixColsVal(~cols);
  cols <<= 1;
  }
 

}

void matrixRowsVal(int value)
{
  // make latchPin output low level
  digitalWrite(latchPin, LOW);
  // Send serial data to 74HC595
  shiftOut(dataPin, clockPin, LSBFIRST, value);
  // make latchPin output high level, then 74HC595 will update the data to parallel output
  digitalWrite(latchPin, LOW);
}

void matrixColsVal(int value) {
  // make latchPin output low level
  digitalWrite(latchPin, LOW);
  // Send serial data to 74HC595
  shiftOut(dataPin, clockPin, MSBFIRST, value);
  // make latchPin output high level, then 74HC595 will update the data to parallel output
  digitalWrite(latchPin, HIGH);
}