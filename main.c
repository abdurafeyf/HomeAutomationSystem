#include <dht.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
dht DHT;
#define DHT11_PIN 6
int mq2Sensor = A0;
int tempmin = 35;
int humidityMin = 24;
int smokeMin = 300;
//App Code 
String inputs;

#define relay1 7 //Connect relay1 to pin 9
#define relay3 4 //Connect relay3 to pin 7
#define relay4 5 //Connect relay4 to pin 6
boolean flag =true;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(mq2Sensor, INPUT);
//App Code
  Serial.begin(9600); //Set rate for communicating with phone
pinMode(relay1, OUTPUT); //Set relay1 as an output
pinMode(relay3, OUTPUT); //Set relay1 as an output
pinMode(relay4, OUTPUT); //Set relay2 as an output
digitalWrite(relay1, LOW); //Switch relay1 off
digitalWrite(relay3, LOW); //Switch relay1 off
digitalWrite(relay4, LOW); //Swtich relay2 off

}

void loop() {
  int chk = DHT.read11(DHT11_PIN);
  int sensorValue = analogRead(mq2Sensor);
//  Serial.println(DHT.temperature);
//  Serial.println(DHT.humidity);
//  Serial.println(sensorValue);
if(flag!=false){
  if (DHT.temperature >= tempmin) {
    digitalWrite(relay1, LOW);
  }
  else {
    digitalWrite(relay1, HIGH);
  }
  if(DHT.humidity<=humidityMin){
    digitalWrite(relay4, HIGH);
    }else{
    digitalWrite(relay4, LOW);
      }
  if(sensorValue>=smokeMin){
    digitalWrite(relay3, HIGH);
    }else{
    digitalWrite(relay3, LOW);
      } 
}

  lcd.setCursor(0, 0);
  lcd.print("Temperature:");
  lcd.print(int(DHT.temperature)); // display the temperature
  lcd.print("C ");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Humidity:");
  lcd.print(int(DHT.humidity)); // display the temperature
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Smoke:");
  lcd.print(sensorValue); // display the temperature
  
//App Code
while(Serial.available()) //Check if there are available bytes to read
{
delay(10); //Delay to make it stable
char c = Serial.read(); //Conduct a serial read
if (c == '#'){
break; //Stop the loop once # is detected after a word
}
inputs += c; //Means inputs = inputs + c
}
if (inputs.length() >0)
{
    flag = false;
Serial.println(inputs);

if(inputs == "A")
{
digitalWrite(relay1, LOW);
}
else if(inputs == "a")
{
digitalWrite(relay1, HIGH);
}
else if(inputs == "B")
{
//digitalWrite(relay2, LOW);
}
else if(inputs == "b")
{
//digitalWrite(relay2, HIGH);
}
else if(inputs == "C")
{
digitalWrite(relay3, HIGH);
}
else if(inputs == "c")
{
digitalWrite(relay3, LOW);
}
else if(inputs == "D")
{
digitalWrite(relay4, LOW);
}
else if(inputs == "d")
{
digitalWrite(relay4, HIGH);
}
else if(inputs == "E")
{
flag = true;
}
else if(inputs == "e")
{
flag = true;
}
inputs="";
}
