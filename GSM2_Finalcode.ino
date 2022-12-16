#include <SoftwareSerial.h>
SoftwareSerial sim(10, 11);
int _timeout;
String _buffer;
String number = "+918806041000"; //-> change with your number
const int pingPin = 7; // Trigger Pin of Ultrasonic Sensor
const int echoPin = 6; // Echo Pin of Ultrasonic Sensor
String textForSMS;
String textForSMS1;
int smokeS = A0; 
int data = 0; 

const int lm35_pin = A1;


char incomingByte; 
String inputString;
int relay1 = 22;
int relay2 = 24;
int relay3 = 26;
int relay4 = 28;
//

void setup() {
  pinMode(4,OUTPUT);// For Small Red LED
  pinMode(5,OUTPUT);// For Buzzer
  digitalWrite(4,HIGH);
  delay(7000); 
  digitalWrite(4,LOW);
  Serial.begin(9600);
  _buffer.reserve(50);
  Serial.println("System Started...");
  sim.begin(9600);
  delay(1000);
  Serial.println("Type s to send an SMS, r to receive an SMS, and c to make a call");
//For Smoke start
   randomSeed(analogRead(0));
   pinMode(smokeS, INPUT); 
   
  //End Smoke

//Relay On
 pinMode(relay1, OUTPUT);
 digitalWrite(relay1, HIGH);
 pinMode(relay2, OUTPUT);
 digitalWrite(relay2, HIGH);
 pinMode(relay3, OUTPUT);
 digitalWrite(relay3, HIGH);
 pinMode(relay4, OUTPUT);
 digitalWrite(relay4, HIGH);
      //digitalWrite(relay, LOW); // Initial state of the relay
    //  Serial.begin(9600);
     // sim.begin(9600); 

     while(!sim.available()){
        sim.println("AT");
        delay(1000); 
        Serial.println("Connecting...");
        }
      Serial.println("Connected!");  
      sim.println("AT+CMGF=1");  //Set SMS to Text Mode 
      delay(1000);  
      sim.println("AT+CNMI=1,2,0,0,0");  //Procedure to handle newly arrived messages(command name in text: new message indications to TE) 
      delay(1000);
      sim.println("AT+CMGL=\"REC UNREAD\""); // Read Unread Messages
//Relay OFF

  
}

void loop() {
 

    //For Smoke Start
 data = analogRead(smokeS); 
  
  Serial.print("Smoke: "); 
  Serial.println(data); 
 
  
     if ( data >320) // >320
  {
    digitalWrite(5,HIGH);
    delay(1000);
    digitalWrite(5,LOW);
       textForSMS =  "\nGas Or Smoke Detected";  
 SendMessage();
  Serial.println("message sent."); 
delay(5000);
while(1)
{
  
}

  }
//For Smoke End
  
  
  // For ultra sonic
   long duration, inches, cm;
   pinMode(pingPin, OUTPUT);
   digitalWrite(pingPin, LOW);
   delayMicroseconds(2);
   digitalWrite(pingPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(pingPin, LOW);
   pinMode(echoPin, INPUT);
   duration = pulseIn(echoPin, HIGH);
   inches = microsecondsToInches(duration);
   cm = microsecondsToCentimeters(duration);
   Serial.print(inches);
   Serial.print("in, ");
   Serial.print(cm);
   Serial.print("cm");
   Serial.println();
   delay(100);

   if (inches >5)//>5
   {
       digitalWrite(5,HIGH);
    delay(1000);
    digitalWrite(5,LOW);
      textForSMS1 =  "\nIntruder Dectedted";  
 SendMessage1();
  Serial.println("Ultra message sent."); 
delay(5000);
 
   }

  
  //End Ultra

  // For Temp
   int temp_adc_val;
  float temp_val;
  temp_adc_val = analogRead(lm35_pin); 
  temp_val = (temp_adc_val * 4.88); 
  temp_val = (temp_val/10); 
  Serial.print("Temperature = ");
  Serial.print(temp_val);
  Serial.print(" Degree Celsius\n");
  delay(1000);

  if(temp_val>50.00)//>50
  {
     digitalWrite(5,HIGH);
    delay(1000);
    digitalWrite(5,LOW);
     SendMessage2();
  Serial.println("Temp message sent."); 
delay(5000);
  }
  // End Temp






  //Relay ON
 if(sim.available()){
      delay(100);

      // Serial Buffer
      while(sim.available()){
        incomingByte = sim.read();
        inputString += incomingByte; 
        }

        delay(10);      

        Serial.println(inputString);
        inputString.toUpperCase(); // Uppercase the Received Message

        //turn RELAY ON or OFF
        if (inputString.indexOf("RELAY1ON") > -1){
          digitalWrite(relay1, LOW);
          Serial.print("RelayON");
          relayon();
          }
         if (inputString.indexOf("RELAY1OFF") > -1){
          digitalWrite(relay1, HIGH);
          Serial.print("RelayOFF");
          relayoff();
          }          

        delay(50);

         if (inputString.indexOf("RELAY2ON") > -1){
          digitalWrite(relay2, LOW);
          Serial.print("RelayON");
          relayon();
          }
         if (inputString.indexOf("RELAY2OFF") > -1){
          digitalWrite(relay2, HIGH);
          Serial.print("RelayOFF");
          relayoff();
          }          

        delay(50);

         if (inputString.indexOf("RELAY3ON") > -1){
          digitalWrite(relay3, LOW);
          Serial.print("RelayON");
          relayon();
          }
         if (inputString.indexOf("RELAY3OFF") > -1){
          digitalWrite(relay3, HIGH);
          Serial.print("RelayOFF");
          relayoff();
          }          

        delay(50);

         if (inputString.indexOf("RELAY4ON") > -1){
          digitalWrite(relay4, LOW);
          Serial.print("RelayON");
          relayon();
          }
         if (inputString.indexOf("RELAY4OFF") > -1){
          digitalWrite(relay4, HIGH);
          Serial.print("RelayOFF");
          relayoff();
          }          

        delay(50);

        //Delete Messages & Save Memory
        if (inputString.indexOf("OK") == -1){
        sim.println("AT+CMGDA=\"DEL ALL\"");

        delay(1000);}

        inputString = "";
 }
 
 //Rely OFF

    
  if (sim.available() > 0)
    Serial.write(sim.read());
}

//For ultra
long microsecondsToInches(long microseconds) {
   return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}

//End ultra


void SendMessage()
{
  Serial.println ("Sending Message");
  sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);
  Serial.println ("Set SMS Number");
  // sim.write("AT+CPMS=\"ME\"\r");  
  sim.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
  
  delay(1000);
  String SMS = "Gs/ SMoke Detected";
  sim.println(SMS);
 
  delay(100);
   Serial.println ("Sent Message");
  sim.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  _buffer = _readSerial();
}

//For Ultra
void SendMessage1()
{
  Serial.println ("Sending Message");
  sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);
  Serial.println ("Set SMS Number");
  // sim.write("AT+CPMS=\"ME\"\r");  
  sim.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
  
  delay(1000);
  String SMS = "Intruder Detected";
  sim.println(SMS);
 
  delay(100);
   Serial.println ("Sent Message1");
  sim.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  _buffer = _readSerial();
}
//End Ultra

//For Temp
void SendMessage2()
{
  Serial.println ("Sending Message");
  sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);
  Serial.println ("Set SMS Number");
  // sim.write("AT+CPMS=\"ME\"\r");  
  sim.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
  
  delay(1000);
  String SMS = "Temp is high";
  sim.println(SMS);
 
  delay(100);
   Serial.println ("Sent Message1");
  sim.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  _buffer = _readSerial();
}
//End Temp



//For relay-1
void relayon()
{
  Serial.println ("Sending Message");
  sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);
  Serial.println ("Set SMS Number");
  // sim.write("AT+CPMS=\"ME\"\r");  
  sim.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
  
  delay(1000);
  String SMS = "Bulb ON";
  sim.println(SMS);
 
  delay(100);
   Serial.println ("Sent Message1");
  sim.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  _buffer = _readSerial();
}
//End relay 1

//For relay-1 off
void relayoff()
{
  Serial.println ("Sending Message");
  sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);
  Serial.println ("Set SMS Number");
  // sim.write("AT+CPMS=\"ME\"\r");  
  sim.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
  
  delay(1000);
  String SMS = "Bulb OFF";
  sim.println(SMS);
 
  delay(100);
   Serial.println ("Sent Message1");
  sim.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  _buffer = _readSerial();
}
//End relay 1 off



void RecieveMessage()
{
  Serial.println ("SIM800L Read an SMS");
  delay (1000);
  sim.println("AT+CNMI=2,2,0,0,0"); // AT Command to receive a live SMS
  delay(1000);
  Serial.write ("Unread Message done");
}

String _readSerial() {
  _timeout = 0;
  while  (!sim.available() && _timeout < 12000  )
  {
    delay(13);
    _timeout++;
  }
  if (sim.available()) {
    return sim.readString();
  }
}

void callNumber() {
  sim.print (F("ATD"));
  sim.print (number);
  sim.print (F(";\r\n"));
  _buffer = _readSerial();
  Serial.println(_buffer);
}
