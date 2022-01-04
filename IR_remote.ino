#include <IRremote.h>
#include <avr/sleep.h>
#include <avr/power.h>

#define VOL_UP_BUTTON_PIN 5
#define VOL_DOWN_BUTTON_PIN 10
#define SOURCE_BUTTON_PIN 12
//#define IR_RECEIVE_PIN 2
#define IR_SEND_PIN 11
#define INTERRUPT_PIN 3
//#define DEBUG

//unsigned long vol_plus_code = 3772833823; //HEX E0E0E01F
//unsigned long vol_minus_code = 3772829743; //HEX E0E0D02F
//unsigned long source_code = 3772809343; //HEX E0E0807F
 
const unsigned int vol_up[68]={4600,4350,650,1550,700,1500,700,1550,700,400,700,400,700,450,650,450,700,400,700,1500,700,1550,650,1550,700,400,700,400,700,450,650,450,700,400,700,1500,700,1550,650,1550,700,400,700,450,700,400,700,400,700,400,700,450,650,450,650,450,650,1550,700,1500,700,1550,700,1500,700,1550,650};
const unsigned int vol_down[68]={4600,4350,700,1550,650,1550,700,1500,700,450,650,450,700,400,700,400,700,400,700,1550,700,1500,700,1550,700,400,700,400,700,400,700,450,650,450,650,1550,700,1500,700,450,650,1550,700,400,700,400,700,450,700,400,700,400,700,400,700,1550,700,400,700,1500,700,1500,700,1550,700,1500,700};
const unsigned int src[68]={4600,4350,700,1550,650,1550,700,1500,700,450,650,450,700,400,700,400,700,400,700,1550,700,1500,700,1550,700,400,700,400,700,400,700,400,700,400,700,1550,700,400,700,450,650,450,650,450,700,400,700,400,700,400,700,450,650,1550,700,1500,700,1550,650,1550,700,1500,700,1550,700,1500,700};

int srcBtnPrestate = 1;
unsigned long previousMillis = 0;

void setup() {
  Serial.begin(115200);
  pinMode(VOL_UP_BUTTON_PIN, INPUT_PULLUP);
  pinMode(VOL_DOWN_BUTTON_PIN, INPUT_PULLUP);
  pinMode(SOURCE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(INTERRUPT_PIN, INPUT_PULLUP);
//  pinMode(13, OUTPUT);
//  digitalWrite(13, LOW);

  IrSender.begin(IR_SEND_PIN, ENABLE_LED_FEEDBACK);
  //IrReceiver.begin(IR_RECEIVE_PIN);
}
 
void loop() {
  unsigned long currentMillis = millis();
  
  int volUpBtn = digitalRead(VOL_UP_BUTTON_PIN);
  if (volUpBtn == 0) {
    #ifdef DEBUG
    Serial.println("VOL UP PUSHED");
    #endif
    IrSender.sendRaw(vol_up,68,38);
    delay(50);
    previousMillis = currentMillis;
  } 

  int volDownBtn = digitalRead(VOL_DOWN_BUTTON_PIN);
  if (volDownBtn == 0) {
    #ifdef DEBUG
    Serial.println("VOL DOWN PUSHED");
    #endif
    IrSender.sendRaw(vol_down,68,38);
    delay(50);
    previousMillis = currentMillis;
  } 

  int srcBtn = digitalRead(SOURCE_BUTTON_PIN);
  if (srcBtn == 1 && srcBtnPrestate == 0) {
    #ifdef DEBUG
    Serial.println("SOURCE PUSHED");
    #endif
    IrSender.sendRaw(src,68,38);
    srcBtnPrestate = 1;
    delay(50);
    previousMillis = currentMillis;
  } 
  else if (srcBtn == 0) {
    srcBtnPrestate = 0;
  }

  if (currentMillis-previousMillis > 5000) {
    #ifdef DEBUG
    Serial.println("Going to sleep...");
    #endif
    delay(100);
    going_to_sleep();
    #ifdef DEBUG
    Serial.println("Waking up...");
    #endif
  }

//  if (IrReceiver.decode()) {
//    if (IrReceiver.decodedIRData.decodedRawData != 0) {
//      Serial.print(F("Decoded raw data: "));
//      Serial.print(IrReceiver.decodedIRData.decodedRawData, HEX);
//      Serial.print(F(", decoded command: "));
//      Serial.println(IrReceiver.decodedIRData.command, HEX);
//      IrSender.sendRaw(src,68,38);
//    }
//    IrReceiver.resume(); 
//  }
}

//Sleep Mode function
void going_to_sleep() {
  sleep_enable(); //enable sleep mode
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), wake_up, LOW);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); //full sleep mode
  sleep_cpu(); //activate sleep mode
}

//Wake Up function
void wake_up() {
  sleep_disable(); //disable sleep mode
  detachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN)); //remove the interrupt
}
