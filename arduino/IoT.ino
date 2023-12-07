
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define  trig  D2
#define  echo  D1
#define led1 14    // D5
#define button1 5 // D1
boolean bt1_state = HIGH;
boolean check = false;
long duration;
int distance;
unsigned long times=millis();
WidgetLED led_connect(V0);

// You should get Auth Token in the Blynk App.

char auth[] = "cT34Hr2keiIpUpo_BtCrQLAmMoVDpMAz";
char serial;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "LC COFFEE";
char pass[] = "68686868";

BlynkTimer timer;
void setup()
{
  // Debug console
  pinMode(trig, OUTPUT);  // Sets the trigPin as an Output
  pinMode(echo, INPUT);   // Sets the echoPin as an Inpu
  Serial.begin(9600);
  delay(100);
    pinMode(led1, OUTPUT);
  pinMode(button1, INPUT_PULLUP);
  Blynk.begin(auth, ssid, pass);
  // Setup a function to be called every second
}

void loop()
{

  Blynk.run();
  timer.run();
  //checkPython();
  assistance(); 
  if(check == false){
    sendSensor();
  }else{
    assistance();
  }

}
void sendSensor()
{
  digitalWrite(trig, LOW);   // Makes trigPin low
  delayMicroseconds(2);       // 2 micro second delay

  digitalWrite(trig, HIGH);  // tigPin high
  delayMicroseconds(10);      // trigPin high for 10 micro seconds
  digitalWrite(trig, LOW);   // trigPin low
  duration = pulseIn(echo, HIGH);   //Read echo pin, time in microseconds
  distance = duration * 0.034 / 2;   //Calculating actual/real distance
  if(Serial.available() > 0)
  {
    serial = Serial.read();
    if( distance < 100 && serial == 't'){
      digitalWrite(led1, LOW);   // Makes trigPin low
      Blynk.virtualWrite(V1,digitalRead(led1));
      Serial.print("success");
      Serial.println(serial);

 }else{
     digitalWrite(led1, HIGH);   // Makes trigPin low
      Blynk.virtualWrite(V1, HIGH);
 }
  }
  Serial.print("Distance = ");        //Output distance on arduino serial monitor
  Serial.println(distance);
  Blynk.virtualWrite(V2, distance);
}
void assistance(){
    if(millis()-times>1000){
    if (led_connect.getValue()){
      led_connect.off();
    }else {
      led_connect.on();
    }
    times=millis();
  }
  if(digitalRead(button1)==LOW){
    if(bt1_state==HIGH){
      digitalWrite(led1,!digitalRead(led1));
      Blynk.virtualWrite(V1,digitalRead(led1));
      bt1_state=LOW;
      delay(200);
    }
  }else{
    bt1_state=HIGH;
  }
  
}
BLYNK_CONNECTED() {
  // Request the latest state from the server
  Blynk.syncVirtual(V1);
}
BLYNK_WRITE(V1){
  int p = param.asInt();
  digitalWrite(led1, p); 
    Serial.println(p);
  if(p==0){
    check= true;
  }else{
    check = false;
  }
}
 void checkPython(){
  if(Serial.available() > 0)
  {
      serial = Serial.read();
      if(serial == 't'){
      digitalWrite(led1, LOW);   // Makes trigPin low
      Blynk.virtualWrite(V1,digitalRead(led1));
      Serial.print("success");
      Serial.println(serial);      
      }else{
        digitalWrite(led1, HIGH);   // Makes trigPin low
      Blynk.virtualWrite(V1,digitalRead(led1));
      Serial.print("fail");
      Serial.println(serial); 
      }
   } 
 }


  

