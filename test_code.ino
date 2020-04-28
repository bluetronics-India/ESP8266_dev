/* Sample code for testing ESP8266 Development Board v1.0
 *  Developed by Rajarajan Elango
 *  Bluetronics
 *  Https://bluetronics.co.in
 */


//#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial
#define DHTTYPE DHT11
#define DHTPIN  14

#include "CayenneDefines.h"
#include "BlynkSimpleEsp8266.h"
#include "CayenneWiFiClient.h"
#include <DHT.h>

// Cayenne authentication token. This should be obtained from the Cayenne Dashboard.
char token[] = "";
// Your network name and password.
char ssid[] = "";
char password[] = "";
//Variables for DHT11 values
float h, t;
bool Humidity = false;
bool Temperature = false;

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);


DHT dht(DHTPIN, DHTTYPE);
;

void setup()
{
  Serial.begin(9600);
  Serial.print("Setup");
  Cayenne.begin(token, ssid, password);

  Humidity = false;
  Temperature = false;
 Wire.begin(0, 2); // SDA, SDL
 display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}

void loop()
{
  //Run Cayenne Functions
  Cayenne.run();
 
    if (Humidity && Temperature){
    Serial.println("Got all values - sleeping");
    delay(100);
    ESP.deepSleep(60000000, WAKE_RF_DEFAULT);
    delay(100);
  }
}

CAYENNE_OUT(V0){
  Serial.println("Entered Humidity");

  //Check if read failed and try until success
  do {
    //Read humidity (percent)
    h = dht.readHumidity();

    delay(1000);
  } while  (isnan(h));

  Serial.print("Humidity: ");
  Serial.println(h);

  //Set Humidity to true so we know when to sleep
  Humidity = true;

  //Write to Cayenne Dashboard
  Cayenne.virtualWrite(V0, h);
}

CAYENNE_OUT(V1){
  Serial.println("Entered Temperature");
  
  //Check if read failed and try until success
  do {
    //Read temperature as Fahrenheit
    t = dht.readTemperature(true);
   
    delay(1000);
  } while  (isnan(t));

  Serial.print("Temperature: ");
  Serial.println(t);

  //Set Temperature to true so we know when to sleep
  Temperature = true;

  //Write to Cayenne Dashboard
  Cayenne.virtualWrite(V1, t);

   display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);  
  display.println(h);
  display.setCursor(45, 0);
  display.println("%");
  display.setCursor(0, 10);  
  display.println(t);    
  display.setCursor(45, 10);
  display.println("Temp");
  display.display();
}

