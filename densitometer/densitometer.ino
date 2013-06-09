#include <SoftwareSerial.h>
#include <SerialLCD.h>
SerialLCD slcd(11,12);


//const int start = 3;
const int button1000 = 5;
const int button4000 = 7;
const int button10000 = 9;
const int ledPin =  13;    
int displayLEDCount = 0;

int measureState = HIGH;       // HIGH - измерение LOW - нет
unsigned long maxCount = 1000;
unsigned long count = 0;

int state = HIGH;
int previousState = HIGH;

unsigned long startTime = 0;
unsigned long totalTime = 0;
float floatTime = 0;

void setup() {
  // initialize the LED pin as an output:
  attachInterrupt(0, muonCount, RISING);
  pinMode(3, INPUT);
  pinMode(2, INPUT);
  attachInterrupt(1, start, RISING);
  pinMode(ledPin, OUTPUT);      
  // initialize the pushbutton pin as an input:
  pinMode(button4000, INPUT);
  pinMode(button10000, INPUT);     
  pinMode(button1000, INPUT);     
  //pinMode(start, INPUT);  
    
  slcd.begin();

  //slcd.print("hello, world!");
}

void muonCount() {

    if (measureState == LOW) {
      count += 1;
      if (count == maxCount) {
       measureState = HIGH; 
      }
    }


}

void displayLED() 
{
  displayLEDCount += 1;
  if (displayLEDCount % 20 == 0) {
    slcd.clear();
  }
 slcd.setCursor(0,0);
  slcd.print("CN:");
 slcd.print(count,DEC);

 slcd.setCursor(0, 1); 
 slcd.print("TM:");
 floatTime = (float)totalTime / 10.0;
 slcd.print(floatTime,1);
 slcd.setCursor(8, 0); 
 slcd.print("MX:");
 slcd.print(maxCount,DEC);

}

void start(){
    measureState = !measureState;

    if (measureState == LOW) {
       startTime = millis(); 
       count = 0;
       digitalWrite(ledPin, HIGH);
    } else {
     digitalWrite(ledPin, LOW); 
    }
  
  
}

void loop(){
  // read the state of the pushbutton value:
  //state = digitalRead(start);

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
 
  if (measureState == LOW) 
  {
     totalTime = (millis() - startTime)/100;
  }
  
 
  if ((digitalRead(button1000) == LOW) && (measureState == HIGH)) {     
    maxCount = 100;
  }
  if ((digitalRead(button10000) == LOW) && (measureState == HIGH)) {     
    maxCount = 10000;
  }
  if ((digitalRead(button4000) == LOW) && (measureState == HIGH)) {      
    maxCount = 4000;
  }
  //delay(20); 
  displayLED();
  
}
