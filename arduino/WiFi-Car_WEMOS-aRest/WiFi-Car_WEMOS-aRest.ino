// Import required libraries
#include "ESP8266WiFi.h"
#include <aREST.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//Modified library: https://github.com/mcauser/Adafruit_SSD1306/tree/esp8266-64x48

//Define the pins
// Motor A
int PWM1 = D7; // PWM Pin Motor 1 (ln1)
int PoM1 = D8;   // Polarity Pin Motor 1 (ln2)
// Motor B
int PWM2 = D5; // PWM Pin Motor 2 (ln4)
int PoM2 = D6;   // Polarity Pin Motor 2 (ln3)
//int enA = D3; //Motor1 Speed Pin (although it will not be used, for simplicity)
//int enB = D4; //Motor2 Speed Pin (although it will not be used, for simplicity)


//int Motor1Speed = 123; // Speed 0-255. Output is 3.3v (analog). LLC needed. (Motor starts rotating after 4,5V)
//int Motor2Speed = 123; // Speed 0-255. Output is 3.3v (analog., LLC needed. (Motor starts rotating after 4,5V)

// Create aREST instance
aREST rest = aREST();

// WiFi parameters - Special for 2nd Thess Code Festival workshop
const char* ssid = "ThessCodeFestival_WiFi-CAR";
const char* password = "*********";

// The port to listen for incoming TCP connections 
#define LISTEN_PORT           80

// Create an instance of the server
WiFiServer server(LISTEN_PORT);

// Functions from aRest.js
int stop(String message);
int forward(String message);
int right(String message);
int left(String message);
int backward(String message);
int leftSpin(String message);
int rightSpin(String message);


////////////////////////// [start] Defining the OLED display
#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

#if (SSD1306_LCDHEIGHT != 48)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
////////////////////////// [end] of defing OLED display


void setup(void)
{  
  // Start Serial
  Serial.begin(9600);
  // Mapping Functions from JS to WEMOS         
  rest.function("stop", handbrake);
  rest.function("forward", goForward);
  rest.function("left", turnLeft);
  rest.function("right", turnRight);
  rest.function("backward", goBackward);
  rest.function("leftSpin", spinLeft);
  rest.function("rightSpin", spinRight);
      
  // Give a name and and ID to this device
  rest.set_id("1");
  rest.set_name("robot");
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
  
  // Print the IP address
  Serial.println(WiFi.localIP());

  ////////////////////////// [start] OLED display
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 64x48)
  // init done
  display.display();
  delay(2000);
 
  // Clear the buffer.
  display.clearDisplay();

  // Display the initial Text showing the IP
  showText("init");
  
  ////////////////////////// [end] OLED Display

  //set the Motor pins as outputs
  pinMode(PWM1, OUTPUT); 
  pinMode(PoM1, OUTPUT); 
  pinMode(PWM2, OUTPUT);   
  pinMode(PoM2, OUTPUT);
//  pinMode(enA, OUTPUT);
//  pinMode(enB, OUTPUT);
  
}

void loop() {
  
  // Handle REST calls
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  while(!client.available()){
    delay(1);
  }
  rest.handle(client);
 
}

// Give Direction to the First Motor
void motor1(String direction){
  Serial.println("Motor 1:");
  Serial.println(direction);
  
  if (direction == "forward"){
      digitalWrite(PoM1, HIGH) ;
      digitalWrite(PWM1, LOW);
      
  }else if (direction == "backward"){             
      digitalWrite(PoM1, LOW) ;
      digitalWrite(PWM1, HIGH);
      
  }else if (direction == "stop"){           
      digitalWrite(PoM1, LOW) ;
      digitalWrite(PWM1, LOW);
      
  }else{
      // stops the motor in case of junk data
      digitalWrite(PoM1, LOW) ;
      digitalWrite(PWM1, LOW);
  }  
}

// Give Direction to the Second Motor
void motor2(String direction){
  Serial.println("Motor 2:");
  Serial.println(direction);
  
  if (direction == "forward"){
      digitalWrite(PoM2, HIGH) ;
      digitalWrite(PWM2, LOW);
      
  }else if (direction == "backward"){             
      digitalWrite(PoM2, LOW) ;
      digitalWrite(PWM2, HIGH);
      
  }else if (direction == "stop"){           
      digitalWrite(PoM2, LOW) ;
      digitalWrite(PWM2, LOW);
      
  }else{
      // stops the motor in case of junk data
      digitalWrite(PoM2, LOW) ;
      digitalWrite(PWM2, LOW);
  }  
}


// Mapping the User's Commands to Motor Directions
int turnLeft(String command) {
  showText("<<<");
  // Motor 1
  motor1("forward");
  // Motor 2
  motor2("stop");
}

int turnRight(String command) {
  showText("  >>>");
  // Motor 1
  motor1("stop");
  // Motor 2
  motor2("forward");
}

int goForward(String command) {
  showText("GO!");
  // Motor 1
  motor1("forward");
  // Motor 2
  motor2("forward");
}

int goBackward(String command) {
  showText("BACK");
  // Motor 1
  motor1("backward");
  // Motor 2
  motor2("backward");
}

int handbrake(String command) {
  // Motor 1
  motor1("stop");
  // Motor 2
  motor2("stop");
  showText("init");
}

int spinLeft(String command) {
  showText("<CraZy!");
  // Motor 1
  motor1("backward");
  // Motor 2
  motor2("forward");
}

int spinRight(String command) {
  showText("CraZy!>");
  // Motor 1
  motor1("forward");
  // Motor 2
  motor2("backward");
}

// A function to handle text displaying in OLED
void showText(String text){
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,0);

    // The initial Screen
    if (text == "init"){      
      display.println("2nd Thess Code Festival");
      display.println("~-~-~-~-~-");
      display.println(WiFi.localIP());
      
    } else{
      display.setTextSize(2);
      display.println(text);
    }


    // General display and clearing the buffer
    display.display();
    delay(199); // some delay to give time to oled to draw
    display.clearDisplay();
}
