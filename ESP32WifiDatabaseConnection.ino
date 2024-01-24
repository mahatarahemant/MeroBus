#include <Wire.h>
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>
SFE_UBLOX_GNSS myGNSS;
#include <SoftwareSerial.h>
#include <HTTPClient.h>
#include<WiFi.h>

#define led 22

//url of the database of web hosted
String URL = "http://192.168.254.16/gnss/test_data.php";

const char* ssid = "YOUR WIFI ID";
const char* password = "YOUR WIFI PASSWORD";

double lat = 0.00000000;
double lng = 0.00000000;

long lastTime = 0;

void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT);
  connectWiFi();
  

  while (!Serial)
    ;  // Wait for user to open terminal
  Serial.println("SparkFun u-blox Example");

  Wire.begin();

  if (myGNSS.begin() == false) {
    Serial.println(F("u-blox GNSS not detected at default I2C address. Please check wiring. Freezing."));
    while (1)
      ;
  }
}

void printSpeed(long speed) {
  float speedMtr = speed / 1000.0;
  Serial.print(speedMtr);
  Serial.print("m/s");
}

void loop() {

  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }
  if (millis() - lastTime > 1000) {
    lastTime = millis();  //Update the timer

    long latitude = myGNSS.getLatitude();
    long longitude = myGNSS.getLongitude();
    long speed = myGNSS.getGroundSpeed();

    Serial.print(F("Latitude: "));
    double lat = latitude / 10000000.0;
    Serial.print(lat);

    Serial.print(F(", Longitude: "));
    double lng = longitude / 10000000.0;
    Serial.print(lng);

    Serial.print(F(" speed: "));
    float speedMtr = speed / 1000.0;
    Serial.print(speedMtr);
    Serial.print("/n");
    Serial.println();

   String postData = "lat=" + String(lat, 8) + "&lng=" + String(lng, 8);

    HTTPClient http;
    http.begin(URL);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

 int httpCode = http.POST(postData);
  String payload = http.getString();

  Serial.print("URL : ");
  Serial.println(URL);
  Serial.print("Data: ");
  Serial.println(postData);
  Serial.print("httpCode: ");
  Serial.println(httpCode);
  Serial.print("payload : ");
  Serial.println(payload);
  Serial.println("--------------------------------------------------");
  digitalWrite(2, HIGH);
  delay(2000);
  digitalWrite(2, LOW);
  delay(2000);
  }
}

void connectWiFi() {
  WiFi.mode(WIFI_OFF);
  delay(1000);
  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500),
      Serial.print(".");
  }

  Serial.print("connected to : ");
  Serial.println(ssid);
  Serial.print("IP adress : ");
  Serial.println(WiFi.localIP());
}
