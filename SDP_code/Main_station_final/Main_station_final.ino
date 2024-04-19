//radio
#include <SPI.h>
#include <RF24.h>
#include <LiquidCrystal_I2C.h> // Library for LCD
//wifi
#include "WiFiS3.h"
#include "arduino_secrets.h" 
#include <ArduinoJson.h>


//radio setup
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

// Wifi setup
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;
char server[] = "team23.azurewebsites.net"; // your server address
WiFiClient client;

//init lcd
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows

//globals
const int numOfPlants = 5;
const int LED_PIN= 2; //led to arduino pin 2
const int buzzer = 4; //buzzer to arduino pin 4
//button and pump stuff
const int buttonPin = 5;  // the number of the pushbutton pin
int buttonState = 0;  // variable for reading the pushbutton status
const float FlowRate= 120.0*2/60.0/1000.0; // ML flow rate of all connect pumps added together
const int PumpPin = 3;    // the number of the Pump control pin
unsigned long lastAttemptTime = 60000; // Variable to store the time of the last attempt
int poll_counter = 0;


int tempMV[numOfPlants] = {0,0,0,0,0};
int Water_amount[numOfPlants]= {0,0,0,0,0};


//LCD functions
void LCD_Connecting_to_wifi(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Connecting to:");
  lcd.setCursor(0,1);
  lcd.print(SECRET_SSID);
}
void LCD_nothing_to_do(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Nothing to do");
}
void LCD_polling_plants(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Polling plants");
}
void LCD_plants_to_water(){
  /* prints something like
  Please water
  Plants:1,2,3,4,5
  */
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Please water");
  lcd.setCursor(0,1);
  String plants="Plants:";
  for (int i = 0; i < numOfPlants; i++) {
        if (Water_amount[i] != 0) {
            plants+= i+1; //append i to string
            plants+= ","; //append i to string
        }
      }
  Serial.println(plants);
  lcd.print(plants);
}
void LCD_press_button(){
  /* prints something like
  Press button to
  dispense water
  */
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Press button to");
  lcd.setCursor(0,1);
  lcd.print("dispense water");
}
void LCD_display_time(float time){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("time remainng");
  lcd.setCursor(0,1);
  String tempstring = String(time/1000);
  tempstring+=" sec";
  lcd.print(tempstring);
}
void LCD_water_per_plant(int plantNum, int water){
  lcd.clear();
  lcd.setCursor(0,0);
  String tempstring1 = "Plant ";
  tempstring1 += (plantNum+1);
  lcd.print(tempstring1);
  lcd.setCursor(0,1);
  String tempstring2 = String(water);
  tempstring2 += " ml";
  lcd.print(tempstring2);
}
void LCD_button_water_complete(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Press button");
  lcd.setCursor(0,1);
  lcd.print("when done");
}
void LCD_reset_loop(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("reseting");
}


void printWifiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
bool hasZero(int array[], int size) {
    for (int i = 0; i < size; i++) {
        if (array[i] == 0) {
            return true; // Found a zero, return true
        }
    }
    return false; // No zero found
}
void setArrayToZero(int array[], int size) {
    for (int i = 0; i < size; i++) {
        array[i] = 0;
    }
}

void poll_plants(){
  // step 1: get new moisture values
  LCD_polling_plants();
  if(hasZero(tempMV,numOfPlants)){
    if (radio.available()) {
      char text[32] = "";
      radio.read(&text, sizeof(text));
      Serial.println(text);

      // Parse the received message
      int num1, num2;
      sscanf(text, "%d,%d", &num1, &num2);

      // Create an array to hold the numbers
      int numbers[2];
      numbers[0] = num1;
      numbers[1] = num2;

      // Print the numbers in the array
      Serial.print("Received numbers: ");
      for (int i = 0; i < 2; i++) {
        Serial.print(numbers[i]);
        if (i < 1) {
          Serial.print(", ");
        }
      }
      Serial.println();

      tempMV[numbers[0]-1]=numbers[1]; // update temp array of MV
      Serial.print("  updated tempMV");
    }else{
      Serial.println("no message in queue");

      Serial.print(", state of tempMV is:");
      for (int i = 0; i < numOfPlants; i++) {
        Serial.print(tempMV[i]);
        Serial.print(", "); // Add a space between elements for better readability
      }
    }
    delay(2000);
}
}
void Phase3(){
  /* steps:
  1. work out amount of water to dispense (done in Water_amount)
  2. wait for button confirmation
  3. dispense
  */
  //step 1
  float water_to_dispense = 0;
  for (int i = 0; i < numOfPlants; i++) {
      water_to_dispense += Water_amount[i];}
  // step 2
  buttonState = digitalRead(buttonPin);
  // waiting for button-------------------------------------------------
  while(buttonState == LOW){
    Serial.println("Waiting for button");
    LCD_press_button();
    for (int i = 0; i < 4; i++) {
      //tone(buzzer, 1000);//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Buzzer
      digitalWrite(LED_PIN, HIGH);
      buttonState = digitalRead(buttonPin);
      delay(500);}
    LCD_plants_to_water();
    for (int i = 0; i < 4; i++) {
      //noTone(buzzer);//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Buzzer
      digitalWrite(LED_PIN, LOW);
      buttonState = digitalRead(buttonPin);
      delay(500);}
  }//---------------------------------------------------------
  noTone(buzzer);//Buzzer off
  Serial.println("Button pressed");
  digitalWrite(LED_PIN, LOW);
  // step 3
  float time_to_run_pump = water_to_dispense/FlowRate;
  digitalWrite(PumpPin, HIGH);
  Serial.println("dispense water for:");
  Serial.print(time_to_run_pump);
  Serial.print(" ms");
  time_to_run_pump=5000; // for testing
  for(int i = 0; i < time_to_run_pump/1000; i++) {
    delay(1000);
    Serial.println("time remaning:");
    Serial.print(time_to_run_pump-1000*i);
    LCD_display_time(time_to_run_pump-1000*i);
  }
  //delay(5000); // stand in, replace with time_to_run_pump in final
  digitalWrite(PumpPin, LOW);
  Serial.println("water dispensed");

}
void Phase4(){
  /*flash plants to water on lcd and amount.
  wait for user confirmation of watering completion*/
  buttonState = digitalRead(buttonPin);
  int i=0;
  while(buttonState == LOW){
    if(i>=numOfPlants){i=-1;}
    else{
    buttonState = digitalRead(buttonPin);
    LCD_water_per_plant(i,Water_amount[i]);
    delay(500);
    buttonState = digitalRead(buttonPin);
    delay(500);
    buttonState = digitalRead(buttonPin);
    LCD_button_water_complete();
    delay(500);
    buttonState = digitalRead(buttonPin);
    delay(500);
    }
    i+=1;}
}



void setup() {
  //alert pins
  pinMode(buzzer, OUTPUT); // Set buzzer - pin as an output
  pinMode(LED_PIN, OUTPUT);//
  digitalWrite(LED_PIN, LOW);
  // pump stuff
  pinMode(buttonPin, INPUT);
  pinMode(PumpPin, OUTPUT);
  //initialize lcd screen
  lcd.init();
  // turn on the backlight
  lcd.backlight();
  // stuff for reciver
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_HIGH);
  radio.startListening();

  //wifi
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true);
  }
  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }
  LCD_Connecting_to_wifi();
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(5000);
  }
  LCD_nothing_to_do();
  //printWifiStatus();
  Serial.println("end of setup");
}



void loop() {
  // put your main code here, to run repeatedly:
  // Check if at least a minute has passed since the last attempt
  if (millis() - lastAttemptTime >= 60000) { // 60000 milliseconds = 1 minute
  
      //lastAttemptTime = millis();
      if(poll_counter<=5){ // phase 1
        poll_plants();
        poll_counter+=1;
      }else{
        lastAttemptTime=0;
        for (int i = 0; i < numOfPlants; ++i) {
          if (tempMV[i] != 0) {
            sendPostRequest(i, tempMV[i]);
          }
        }
        Serial.println("Sent MVs to server");
        sendGetWaterRequest();//phase 3 determine watering amounts
        Serial.println("Amount to water:");
        for (int i = 0; i < numOfPlants; i++) {
          Serial.print(Water_amount[i]);
          Serial.print(", "); // Add a space between elements for better readability
        }
        LCD_plants_to_water();
        Phase3();
        Phase4();
        LCD_reset_loop();
        setArrayToZero(tempMV,numOfPlants); // !!!!!! may need to move this
        poll_counter=0;
        delay(5000);

      }
     

  }
}


void sendPostRequest(int index, int mv) {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;

    // Your Flask server endpoint
    String endpoint = "/update_data";

    // Sample JSON data
    String json_data = "{\"number1\": " + String(mv) + ", \"json_index\": " + String(index) + "}";

    Serial.println("\nStarting connection to server...");
    if (client.connect(server, 80)) {
      Serial.println("connected to server");
      client.println("POST " + endpoint + " HTTP/1.1");
      client.println("Host: team23.azurewebsites.net");
      client.println("Content-Type: application/json");
      client.print("Content-Length: ");
      client.println(json_data.length());
      client.println("Connection: close");
      client.println();
      client.println(json_data);
    }
  } else {
    Serial.println("Error in WiFi connection");
  }
}





void sendGetWaterRequest() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;

    // Your Flask server endpoint
    String endpoint = "/Amount_to_water";

    Serial.println("\nStarting GET request to server...");
    if (client.connect(server, 80)) {
      Serial.println("Connected to server");
      client.println("GET " + endpoint + " HTTP/1.1");
      client.println("Host: team23.azurewebsites.net");
      client.println("Connection: close");
      client.println();

      // Wait for response from the server
      while (client.connected()) {
        if (client.available()) {
          String line = client.readStringUntil('\n');
          if (line == "\r") {
            // Empty line indicates start of response body
            break;
          }
        }
      }

      // Parse the JSON response
      StaticJsonDocument<200> doc; // Adjust the size as needed
      DeserializationError error = deserializeJson(doc, client);
      if (error) {
        Serial.println("Error parsing JSON response");
      } else {
        // Extract values from JSON array
        int index = 0;
        for (JsonVariant value : doc.as<JsonArray>()) {
          Water_amount[index++] = value.as<int>();
        }
      }
    } else {
      Serial.println("Failed to connect to server");
    }
    client.stop(); // Close the connection
  } else {
    Serial.println("Error in WiFi connection");
  }
}
