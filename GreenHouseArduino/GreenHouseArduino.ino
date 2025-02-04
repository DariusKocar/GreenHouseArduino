#include <DHT11.h>

DHT11 dht11(D4);
const int A1A = D3;
const int A1B = D2;
const int fullMotorTime = 40000; // ms
const int openTemp = 30; // C
const int closeTemp = 20; // C
int openPercentage = 0; // %
int onePercentMS = 300; // %
int currentMoveCounter = 0; 
int maxMoveCount = 300; 

void open(int motorOnTime){
        // Motor A
        Serial.println("Motor OPEN start");
        digitalWrite(A1A, HIGH);
        digitalWrite(A1B, LOW);

        delay(motorOnTime);

        Serial.println("Motor OPEN stop");
        digitalWrite(A1A, LOW);
        digitalWrite(A1B, LOW);
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
}

void fullClose(){
  Serial.print("FULL close");
  close(fullMotorTime);
  openPercentage = 0;
  currentMoveCounter = 0;
}

void fullOpen(){
  Serial.print("FULL open");
  open(fullMotorTime);
  openPercentage = 100;
  currentMoveCounter = 0;
}

void setOpenPercentage(int newPercentage){
  currentMoveCounter++;
  if (currentMoveCounter > maxMoveCount){
    // Motor always closes faster than opens. This drifts to close. So Close fullly once in a while to reset
      fullClose();
  }

  if (newPercentage == 100) {
      fullOpen();
  }
  else if (newPercentage == 0) {
      fullClose();
  } 
  else {
    int diff = newPercentage - openPercentage;
    if (newPercentage > openPercentage){
      open(diff * onePercentMS);
    }
    else{
      close(diff * onePercentMS);
    }
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

        int newPercentage = (int)100*(((double)openTemp - closeTemp)/100.0 * max(0, temperature - closeTemp));
        setOpenPercentage(newPercentage);
    } else {
        // Print error message based on the error code.
        Serial.println(DHT11::getErrorString(result));
    }
    // Read temp every second
    delay(1000);
}

