#include <SD.h>
#include <SPI.h>
File myFile;
int pinCS = 10; // Pin 10 on Arduino Uno

void setup() {
    
  Serial.begin(9600);
  pinMode(pinCS, OUTPUT);
  
  // SD Card Initialization
  if (SD.begin())
  {
    Serial.println("SD card is ready to use.");
  } else
  {
    Serial.println("SD card initialization failed");
    return;
  }
  
 pinMode(A0,INPUT);
  
}
void loop() 
{
 
  myFile = SD.open("w.txt", FILE_WRITE);
            if (myFile) {
              Serial.println("Writing to file...");
              myFile.print(analogRead(A0));
              myFile.print(",");
              myFile.println(analogRead(A0));
              myFile.close(); 
              Serial.println("Done.");
            }
           
            else {
              Serial.println("error opening test.txt");
            }
 
  myFile = SD.open("w.txt");
            if (myFile) {
              Serial.println("Read:");
          
              while (myFile.available()) {
                Serial.write(myFile.read());
             }
              myFile.close();
            }
            else {
              Serial.println("error opening test.txt");
            }


}
