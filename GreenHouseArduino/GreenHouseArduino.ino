#include <DHT11.h>

DHT11 dht11(D4);
const int A1A = D3;
const int A1B = D2;
const int fullMotorTime = 20000; // ms
const int openTemp = 30; // C
const int closeTemp = 20; // C
bool isOpen = false;
int openPercentage = 0; // %
int onePercentMS = 500; // %

void open(int motorOnTime){
        // Motor A
        Serial.println("Motor OPEN start");
        digitalWrite(A1A, HIGH);
        digitalWrite(A1B, LOW);

        delay(motorOnTime);

        Serial.println("Motor OPEN stop");
        digitalWrite(A1A, LOW);
        digitalWrite(A1B, LOW);
        isOpen = false;
}

void close(int motorOnTime){
        // Motor A
        Serial.println("Motor CLOSE start");
        digitalWrite(A1A, LOW);
        digitalWrite(A1B, HIGH);

        delay(motorOnTime);

        Serial.println("Motor CLOSE stop");
        digitalWrite(A1A, LOW);
        digitalWrite(A1B, LOW);
        isOpen = true;
}

void fullClose(){
  close(fullMotorTime);
  openPercentage = 0;
}

void setOpenPercentage(int newPercentage){
  if (newPercentage > openPercentage){
    int diff = newPercentage - openPercentage;
    open(diff * onePercentMS);
    Serial.print("NEW percentage ");
    Serial.print(newPercentage);
    Serial.println("%");
  }
  if (newPercentage < openPercentage){
    int diff = openPercentage - newPercentage;
    close(diff * onePercentMS);
    Serial.print("NEW percentage ");
    Serial.print(newPercentage);
    Serial.println("%");
  }

Serial.print("NEW percentage ");
    Serial.print(newPercentage);
    Serial.println("%");
  openPercentage = newPercentage;
}



void setup() {
  Serial.begin(115200);
  Serial.println("Start");
  pinMode(A1A, OUTPUT);
  pinMode(A1B, OUTPUT);

  digitalWrite(A1A, LOW);
  digitalWrite(A1B, LOW);

  fullClose();
}


void loop() {
    int temperature = 0;
    int humidity = 0;

    int result = dht11.readTemperatureHumidity(temperature, humidity);
    if (result == 0) {
        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.print(" °C\tHumidity: ");
        Serial.print(humidity);
        Serial.println(" %");

        Serial.print("Test: ");
        Serial.println(((double)openTemp - closeTemp)/100.0);
        Serial.print("Test2: ");
        Serial.println(max(0, temperature - closeTemp));
        int newPercentage = (int)100*(((double)openTemp - closeTemp)/100.0 * max(0, temperature - closeTemp));
        setOpenPercentage(newPercentage);
        // if (isOpen){
        //   if (temperature < closeTemp) {
        //     close();
        //   }
        // }
        // else {
        //   if (temperature > openTemp) {
        //     open();
        //   }
        // }
    } else {
        // Print error message based on the error code.
        Serial.println(DHT11::getErrorString(result));
    }
    // Read temp every second
    delay(1000);
}

