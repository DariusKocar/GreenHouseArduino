#include <DHT11.h>

DHT11 dht11(D4);
const int A1A = D3;
const int A1B = D2;
const int motorDelay = 30000; // ms
const int openTemp = 30; // C
const int closeTemp = 20; // C
bool isOpen = false;

void setup() {
  Serial.begin(115200);
  Serial.println("Start");
  pinMode(A1A, OUTPUT);
  pinMode(A1B, OUTPUT);

  digitalWrite(A1A, LOW);
  digitalWrite(A1B, LOW);

  close();
}

void close(){
        // Motor A
        Serial.println("Motor CLOSE start");
        digitalWrite(A1A, HIGH);
        digitalWrite(A1B, LOW);

        delay(motorDelay);

        Serial.println("Motor CLOSE stop");
        digitalWrite(A1A, LOW);
        digitalWrite(A1B, LOW);
        isOpen = false;
}

void open(){
        // Motor A
        Serial.println("Motor OPEN start");
        digitalWrite(A1A, LOW);
        digitalWrite(A1B, HIGH);

        delay(motorDelay);

        Serial.println("Motor OPEN stop");
        digitalWrite(A1A, LOW);
        digitalWrite(A1B, LOW);
        isOpen = true;
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

        if (isOpen){
          if (temperature < closeTemp) {
            close();
          }
        }
        else {
          if (temperature > openTemp) {
            open();
          }
        }
    } else {
        // Print error message based on the error code.
        Serial.println(DHT11::getErrorString(result));
    }
    // Read temp every second
    delay(1000);
}

