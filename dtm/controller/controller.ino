#include <SPI.h>
#include <RF24.h>

RF24 radio(9, 10);  // nRF24L01+ module (CE, CSN)
const byte address[6] = "00001";  // Unique address

void setup() {
  // Start serial communication for debugging
  Serial.begin(9600);

  // Initialize the nRF24L01+ radio
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_HIGH);

  Serial.println("Controller ready.");
}

void loop() {
  // Send commands based on user input
  if (Serial.available()) {
    char command = Serial.read();

    if (command == 'w') {
      radio.write("FWD", sizeof("FWD"));  // Send Forward command
      Serial.println("Sending: Forward");
    } else if (command == 's') {
      radio.write("BK", sizeof("BK"));  // Send Backward command
      Serial.println("Sending: Backward");
    } else if (command == 'a') {
      radio.write("LFT", sizeof("LFT"));  // Send Left command
      Serial.println("Sending: Left");
    } else if (command == 'd') {
      radio.write("RGT", sizeof("RGT"));  // Send Right command
      Serial.println("Sending: Right");
    } else if (command == 'u') {
      radio.write("UP", sizeof("UP"));  // Send Up command
      Serial.println("Sending: Up");
    } else if (command == 'j') {
      radio.write("DN", sizeof("DN"));  // Send Down command
      Serial.println("Sending: Down");
    }
  }
}
