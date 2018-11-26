#include <SPI.h>
#include <WiFi.h>
#include <math.h>

#define LED 4
#define Touch 2
#define Buzzer 3
#define TEMP A0
#define IsLight A1
#define Move 13

int gas = 0;
int B=4275;
int R0 = 100000;
String touchState = "close";


//String resp = "HTTP/1.1 200 OK \nContent-Type: application/json;charset=utf-8 \n Connection: close\n";

char ssid[] = "SSID_wifi";      //  your network SSID (name) 
char pass[] = "password_wifi";   // your network password
int keyIndex = 0;                 // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
WiFiServer server(8080);




void setup() {
  Serial.begin(9600);      // initialize serial communication
  pinMode(LED, OUTPUT);      // set the LED pin mode
  pinMode(Buzzer, OUTPUT);
  pinMode(Touch, INPUT);
  pinMode(IsLight, INPUT);
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present"); 
    while(true);        // don't continue
  } 

  String fv = WiFi.firmwareVersion();
  if( fv != "1.1.0" )
    Serial.println("Please upgrade the firmware");

  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);

    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(10000);
  } 
  server.begin();                           // start the web server on port 80
  printWifiStatus();                        // you're connected now, so print out the status
}


void loop() {
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {
      
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character
          if (currentLine.length() == 0) {  
//            client.println("HTTP/1.1 200 OK");
//            client.println("Content-type:text/html");
            client.println();
            break;         
          } 
          else {      // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }     
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /light/on")) {
          digitalWrite(LED, HIGH);               // GET /H turns the LED on
        }
        else if (currentLine.endsWith("GET /light/off")) {
          digitalWrite(LED, LOW);                // GET /L turns the LED off
        }

        else if (currentLine.endsWith("GET /buzzer/on")) {
          digitalWrite(Buzzer, HIGH);               // GET /H turns the LED on
        }
        else if (currentLine.endsWith("GET /buzzer/off")) {
          digitalWrite(Buzzer, LOW);                // GET /L turns the LED off
        }

        else if (currentLine.endsWith("GET /temperature")) {
           int val = analogRead(TEMP);                               // get analog value
           float resistance=1023.0/val-1.0;                      // get resistance
           resistance = resistance *R0;
           float temperature=1.0/(log(resistance/R0)/B+1/298.15)-273.15;     // calc temperature
           client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: application/json;charset=utf-8");
          client.println("Connection: close");
          client.println();
           client.println(temperature);
           client.println();  
        }

        else if (currentLine.endsWith("GET /touch")){
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: application/json;charset=utf-8");
          client.println("Connection: close");
          client.println();
            client.println(touchState);
          client.println();
        }
        else if (currentLine.endsWith("GET /touch/open")){
            touchState = "1";
            Serial.println("1");
        }
        else if (currentLine.endsWith("GET /touch/close")){
            touchState = "0";
            Serial.println("0");
        }
        else if (currentLine.endsWith("GET /door")){
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: application/json;charset=utf-8");
          client.println("Connection: close");
          client.println();
          client.println(touchState);
          client.println();
        }
        else if (currentLine.endsWith("GET /move")){
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: application/json;charset=utf-8");
          client.println("Connection: close");
          client.println();
            if (digitalRead(Move)) {
              client.println("1");
            }else{
               client.println("0");
                }
              client.println();
              
        }
        
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
  else if (digitalRead(Touch) == HIGH){
      if (touchState == "close"){
          touchState = "1";
          Serial.println("1");
        }
        else {
            touchState = "0";
            Serial.println("0");
          }
          delay(1000);
    }
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}
