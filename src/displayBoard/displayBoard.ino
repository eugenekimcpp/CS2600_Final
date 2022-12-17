#include <Keypad.h>
#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "NETGEAR93";
const char* password = "excitedtable945";

char *mqttServer = "test.mosquitto.org";
int mqttPort = 1883;

WiFiClient wificlient;
PubSubClient client(wificlient);

const char* device_name = "ESP32";

const char* mqtt_pub_topic = "eugene/xmove";


const char* mqtt_sub_topic = "eugene/omove";
const char* mqtt_sub_winner = "eugene/winnerStatus";


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

int changeBoard[] = {                       
  B00100100, 
  B00100100, 
  B11111111, 
  B00100100, 
  B00100100, 
  B11111111, 
  B00100100, 
  B00100100
};

const int xMove[9][8] = {
//r 1    2.   3.    4.   5.   6.   7.   8
  {0x02,0x01,0x00,0x00,0x00,0x00,0x00,0x00}, //1
  {0x10,0x08,0x00,0x00,0x00,0x00,0x00,0x00}, //2
  {0x80,0x40,0x00,0x00,0x00,0x00,0x00,0x00}, //3 base
  {0x00,0x00,0x00,0x02,0x01,0x00,0x00,0x00}, //4
  {0x00,0x00,0x00,0x10,0x08,0x00,0x00,0x00}, //5
  {0x00,0x00,0x00,0x80,0x40,0x00,0x00,0x00}, //6
  {0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x01}, //7
  {0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x08}, //8
  {0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x40}  //9

};

const int oMove[9][8]{
  {0x03,0x03,0x00,0x00,0x00,0x00,0x00,0x00},
  {0x18,0x18,0x00,0x00,0x00,0x00,0x00,0x00},
  {0xC0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00},
  {0x00,0x00,0x00,0x03,0x03,0x00,0x00,0x00},
  {0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00},
  {0x00,0x00,0x00,0xC0,0xC0,0x00,0x00,0x00},
  {0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x03},
  {0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18},
  {0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xC0}

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

  
  setup_wifi();
 
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  
  
}

void loop() {
  
 
  

  if (!client.connected()){
      reconnect();
   }

  // reset previous data
  winner = 0; 
  winnerChar = ' ';
  resetBoard();
  int counter=0, who=0;  
  // get the key if want to start, * to start otherwise try again
  
  
  char start, move;

  char input; 
  // NEW game session 
  //game();
  
}


void game()
{
  
  int counter=0, who=0;  
  // get the key if want to start, * to start otherwise try again
  
  
  char start, move;
  
  do{
    Serial.print("Please press '*' to start: ");
    start = keypad.getKey();
  } while (start != keys[3][0]);
  
  
  Serial.println("New game will start soon...");
  delay(2000);


  // loop until winner == 0
  do{
    displayBoard();
    who = counter % 2; 
    if(who == 0)
    {
      
      // player1 move
      Serial.print("Player 1 move: ");
      move = keypad.getKey(); 
      switch (move){
        case '1':
          makeMoveBoard(move, who); 
          displayBoard();
          break;
        case '2':
          break;
        case '3':
          break;
        case '4':
          break;
        case '5':
          break;
        case '6':
          break;
        case '7':
          break;
        case '8':
          break;
        case '9':
          break;
        case '0':
          Serial.println("QUITTING THE GAME NOW.............");
          delay(2000);
          return;
        default : 
          break; 
      }

    }
    else
    {
      // player2 move
    }
    counter++;
    // ******* receive check winner

  } while(winner == 0);
  
  // Out loop when game is finished
  // wait 2 sec to get back to loop()
  delay(2000);
  
  // winner display will handled in c


}




void callback(char* topic, byte* payload, unsigned int length) {
   Serial.print("Message received from: "); 
   Serial.println(topic);
   char *message; 
   for (int i = 0; i < length; i++){
     Serial.print((char)payload[i]);
     message += (char)payload[i];
   }
   Serial.println();
   Serial.println();
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() 
{
  while(!client.connected()){
    Serial.print("Attemping MQTT connection...");
    if(client.connect(device_name)){
      Serial.println("connected");
      client.subscribe("eugene/testsub");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println( "try again in 5 seconds");
      delay(5000);
    }

  }
}

// Below is for the boards
void displayBoard()
{
  int cols = B00000001;
  
  for (int i = 0; i < 8; i++){
  matrixRowsVal(changeBoard[i]);
  matrixColsVal(~cols);
  cols <<= 1;
  }
}

void makeMoveBoard(char location, int who)
{
  int cols = B00000001;
  int moveTo = atoi(&location);
  moveTo -= 1; // make it zero based for the index
  if(who == 0)
  {
    //player 1 move
    for (int i = 0; i < 8; i++){
    matrixRowsVal(changeBoard[i] | xMove[moveTo][i]);
    matrixColsVal(~cols);
    cols <<= 1;
    }
  }
  else
  {
    //player 2 move
    for (int i = 0; i < 8; i++){
    matrixRowsVal(changeBoard[i] | oMove[moveTo][i]);
    matrixColsVal(~cols);
    cols <<= 1;
    }
  }
}

void resetBoard(){
  int cols = B00000001;

  for(int i = 0; i < 8; i++){
    matrixRowsVal(changeBoard[i] & board[i]); // & will trun ones down
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