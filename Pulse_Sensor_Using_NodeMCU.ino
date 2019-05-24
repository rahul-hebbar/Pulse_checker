#define USE_ARDUINO_INTERRUPTS false
#include <PulseSensorPlayground.h>
#include <SPI.h>
#include <ThingerESP8266.h>
#define USERNAME "*username"
#define DEVICE_ID "*devicename"
#define DEVICE_CREDENTIAL "*devicecredentials"
const int PULSE_INPUT = A0;
const int THRESHOLD = 550;
byte samplesUntilReport;
const byte SAMPLES_PER_SERIAL_SAMPLE = 500;
PulseSensorPlayground pulseSensor;	# built in library used to read analog values from pulse sensor
ThingerESP8266 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);	# used to connect to cloud

void setup() {
  Serial.begin(115200);
  pulseSensor.analogInput(PULSE_INPUT);	#read analog input
  pulseSensor.setSerial(Serial);	#get input from pulse sensor serially
  pulseSensor.setThreshold(THRESHOLD);	#define threshold
  samplesUntilReport = SAMPLES_PER_SERIAL_SAMPLE;
  thing.add_wifi("x", "y");	#connect to Wifi with x - network name and y - network password
}

void loop() {
  if (pulseSensor.sawNewSample()) {
    if (--samplesUntilReport == (byte) 0) {
      samplesUntilReport = SAMPLES_PER_SERIAL_SAMPLE;
      if (pulseSensor.sawStartOfBeat()) {
        int myBPM = pulseSensor.getBeatsPerMinute(); #store serial analog values
        Serial.println(myBPM);
        if (myBPM < 30 or myBPM > 150) {  
           thing.handle();	
           thing.call_endpoint("*endpoint name");	# call endpoint to send emails
        }
        delay(1000);	# minimum resting delay for code
      }
  }
  }
}