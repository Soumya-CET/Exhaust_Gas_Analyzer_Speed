
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

SoftwareSerial s(D2,D3);
const char* ssid     = "Redmi";
const char* password = "arpanet1234";
const char* host = "bybtiot.000webhostapp.com";

String myString; // complete message from arduino, which consistors of snesors data
char rdata; // received charactors
int firstVal, secondVal, thirdVal; // sensors 

void setup()
{
  // Debug console
  Serial.begin(9600);
  s.begin(9600);
  delay(100);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Netmask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: ");
  Serial.println(WiFi.gatewayIP());
  

}

void loop()
{
  if (s.available() > 0 ) 
  { Serial.print(".");
    rdata = s.read(); 
    myString = myString+ rdata; 
    if( rdata == '\n')
    {
    String l = getValue(myString, ',', 0);
    String m = getValue(myString, ',', 1);
    String n = getValue(myString, ',', 2); 
    firstVal = l.toInt();
    secondVal = m.toInt();
    thirdVal = n.toInt();
    Serial.println(firstVal);
    Serial.println(secondVal);
     myString = "";
    // end new code
    
/*
    if (isnan(firstVal) || isnan(secondVal)) {
    Serial.println("Failed to read from serial!");
    return;
  }
*/
  //Serial.print("connecting to ");
  //Serial.println(host);

  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  String url = "/api/insert.php?temp=" + String(firstVal) + "&hum="+ String(secondVal);
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(500);
  /*
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  */
  Serial.println();
  Serial.println("closing connection");
  
  }
    
  }

}

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
