
int result = 0;

#include <SoftwareSerial.h>
#include<Wire.h>
 
// 블루투스
#define BT_RXD 2
#define BT_TXD 3

SoftwareSerial test(BT_TXD, BT_RXD);


//등받이
int SensorPin0 = A0; //analog pin 0
int SensorPin1 = A1; //analog pin 1
int SensorPin2 = A2; //analog pin 2
int SensorPin3 = A3; //analog pin 3

int HI_limit = 1000;
int LOW_limit = 0;

// 방석(3.3v)
int En0 = 13;  //  Low enabled
int En1 = 12;  //  Low enabled
int S0  = 11;
int S1  = 10;
int S2  = 9;
int S3  = 8;
int SIG_pin = A8; //ADC

//그라운드 추가
int q = 0;
  int vib = 100; 
  int z=0;
  int y= 0;

void setup() {
  Serial.begin(9600);
  test.begin(9600);
  pinMode(En0, OUTPUT);
  pinMode(En1, OUTPUT);
 
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
   result = readVcc();

}

int readMux(int channel){
  int controlPin[] = {S0,S1,S2,S3,En0,En1};
 
  int muxChannel[32][6]={
        {1,0,1,0,0,1}, //channel 10
    {1,1,0,0,0,1}, //channel 12
    {1,1,1,0,0,1}, //channel 14
    {0,0,0,0,1,0}, //channel 16
    {0,0,1,0,1,0}, //channel 18
    {0,1,0,0,1,0}, //channel 20
    {0,1,0,1,0,1}, //channel 5
 {0,1,1,0,0,1}, //channel 6
    {0,1,1,1,0,1}, //channel 7
    {1,0,0,0,0,1}, //channel 8
    {1,0,0,1,0,1}, //channel 9
    
    {1,0,1,1,0,1}, //channel 11
    {1,1,0,1,0,1}, //channel 13
    {1,1,1,1,0,1}, //channel 15
    {0,0,0,1,1,0}, //channel 17
    {0,0,1,1,1,0}, //channel 19
    {0,1,0,1,1,0}, //channel 21
    {0,1,1,0,1,0}, //channel 22
    {0,1,1,1,1,0}, //channel 23
    {1,0,0,0,1,0}, //channel 24
    {1,0,0,1,1,0}, //channel 25
    
    {0,0,0,0,0,1}, //channel 0
    
    {0,0,0,1,0,1}, //channel 1
    {0,0,1,0,0,1}, //channel 2
    {0,0,1,1,0,1}, //channel 3
    {0,1,0,0,0,1}, //channel 4
   
    
    {1,0,1,0,1,0}, //channel 26
    {1,0,1,1,1,0}, //channel 27
    {1,1,0,0,1,0}, //channel 28
    {1,1,0,1,1,0}, //channel 29
    {1,1,1,0,1,0}, //channel 30
  };
 
  //loop through the 6 sig
  for(int i = 0; i < 6; i ++){
    digitalWrite(controlPin[i], muxChannel[channel][i]);
  }
 
  //read the value at the SIG pin
  int val = analogRead(SIG_pin);
 
  //return the value
  return val;
}
 
long readVcc() {
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    ADMUX = _BV(MUX5) | _BV(MUX0);
  #elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = _BV(MUX3) | _BV(MUX2);
  #else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif  

  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring

  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH  
  uint8_t high = ADCH; // unlocks both

  long result = (high<<8) | low;

  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return result; // Vcc in millivolts
}

void loop() {
 
 int result_2 = map(result,0,6000,0,100);
  
 int value = 0;
  int v = 0;
  
  int SensorReading0 = analogRead(SensorPin0); 
  int SensorReading1 = analogRead(SensorPin1);
  int SensorReading2 = analogRead(SensorPin2); 
  int SensorReading3 = analogRead(SensorPin3); 

  int A1 = map(SensorReading0, LOW_limit, 1024, 0, 100);
  int A2 = map(SensorReading1, LOW_limit, 1024, 0, 100);
  int A3 = map(SensorReading2, LOW_limit, 1024, 0, 100);
  int A4 = map(SensorReading3, LOW_limit, 1024, 0, 100);

  for(int i = 0; i < 31; i ++){
   value += readMux(i);
  int x = map(readMux(i),0,230,0,100); 
   test.print(x);
 if (i == 30){
  test.print(',');
 }
 else {
  test.print('^');
 }
    delay(1);
  }
  
 test.print(A1); test.print('^');
  test.print(A2); test.print('^');
  test.print(A3); test.print('^');
  test.print(A4); test.print(',');
 
  test.println(result_2);
  
  delay(1000); }
