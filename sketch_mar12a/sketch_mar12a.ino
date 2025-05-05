// #include <WiFi.h>
// #include <HTTPClient.h>
// #include <ArduinoJson.h>
// #include "DHT.h"

// #define DHTPIN 4
// #define DHTTYPE DHT11
// #define MQ135_PIN 34  // MQ-135 sensor connected to ADC pin GPIO34
// DHT dht(DHTPIN, DHTTYPE);

// const char* ssid = "NEDUET-Wifi Network";
// const char* password = "neduet33iacc44&";
// const char* serverName = "http://192.168.169.157:5000/api/sensors"; // Change to your actual server IP

// void setup() {
//     Serial.begin(115200);
//     WiFi.begin(ssid, password);

//     Serial.print("Connecting to WiFi...");
//     while (WiFi.status() != WL_CONNECTED) {
//         delay(5000);
//         Serial.print(".");
//     }
//     Serial.println("\nConnected to WiFi!");
//     Serial.print("ESP32 IP Address: ");
//     Serial.println(WiFi.localIP());

//     dht.begin();
//     pinMode(MQ135_PIN, INPUT);
// }

// void loop() {
//     if (WiFi.status() == WL_CONNECTED) {
//         float temperature = dht.readTemperature();
//         float humidity = dht.readHumidity();
//         int air_quality = analogRead(MQ135_PIN); // Read from MQ-135 sensor

//         Serial.println("Reading sensor values...");
//         Serial.print("Temperature: "); Serial.println(temperature);
//         Serial.print("Humidity: "); Serial.println(humidity);
//         Serial.print("Air Quality: "); Serial.println(air_quality);

//         WiFiClient client;
//         HTTPClient http;

//         http.begin(client, serverName);
//         http.addHeader("Content-Type", "application/json");

//         StaticJsonDocument<200> doc;
//         doc["temperature"] = temperature;
//         doc["humidity"] = humidity;
//         doc["air_quality"] = air_quality;
//         String requestBody;
//         serializeJson(doc, requestBody);

//         Serial.print("Sending JSON: ");
//         Serial.println(requestBody);

//         int httpResponseCode = http.POST(requestBody);

//         Serial.print("HTTP Response code: ");
//         Serial.println(httpResponseCode);

//         if (httpResponseCode > 0) {
//             String response = http.getString();
//             Serial.println("Server Response: " + response);
//         } else {
//             Serial.println("Error sending request. Check server or WiFi.");
//         }

//         http.end();
//     } else {
//         Serial.println("WiFi Disconnected");
//     }

//     delay(5000);
// }



//-------------------------------------------------------------------------------------------------------------------



#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "DHT.h"
#include <WiFiManager.h> // Add this


#define DHTPIN 4
#define DHTTYPE DHT11
#define MQ135_PIN 34  // MQ-135 sensor connected to ADC pin GPIO34
DHT dht(DHTPIN, DHTTYPE);

// Remove hardcoded SSID and password
const char* serverName = "https://bright-aliza-asnaif-bfedfd0f.koyeb.app/api/sensors"; 

void setup() {
    Serial.begin(115200);

    // WiFiManager setup
    WiFiManager wm;
    bool res;
    res = wm.autoConnect("ESP32-Setup", "esp32password"); // SSID and password for config portal

    if (!res) {
        Serial.println("❌ Failed to connect or timed out.");
        // Optional: reset or retry
        ESP.restart();
    }

    Serial.println("✅ Connected to WiFi using WiFiManager!");
    Serial.print("ESP32 IP Address: ");
    Serial.println(WiFi.localIP());

    dht.begin();
    pinMode(MQ135_PIN, INPUT);
}

void loop() {
    if (WiFi.status() == WL_CONNECTED) {
        float temperature = dht.readTemperature();
        float humidity = dht.readHumidity();
        int air_quality = analogRead(MQ135_PIN); // Read from MQ-135 sensor

        Serial.println("Reading sensor values...");
        Serial.print("Temperature: "); Serial.println(temperature);
        Serial.print("Humidity: "); Serial.println(humidity);
        Serial.print("Air Quality: "); Serial.println(air_quality);

        WiFiClient client;
        HTTPClient http;

        http.begin(client, serverName);
        http.addHeader("Content-Type", "application/json");

        StaticJsonDocument<200> doc;
        doc["temperature"] = temperature;
        doc["humidity"] = humidity;
        doc["air_quality"] = air_quality;
        String requestBody;
        serializeJson(doc, requestBody);

        Serial.print("Sending JSON: ");
        Serial.println(requestBody);

        int httpResponseCode = http.POST(requestBody);

        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);

        if (httpResponseCode > 0) {
            String response = http.getString();
            Serial.println("Server Response: " + response);
        } else {
            Serial.println("Error sending request. Check server or WiFi.");
        }

        http.end();
    } else {
        Serial.println("WiFi Disconnected");
    }

    delay(5000);
}




//------------------------------------------------------------------------------------------------------------------



// #include <WiFi.h>
// #include <HTTPClient.h>
// #include <ArduinoJson.h>
// #include "DHT.h"
// #include <WiFiManager.h> // Add this
// #include <ArduinoOTA.h>



// #define DHTPIN 4
// #define DHTTYPE DHT11
// #define MQ135_PIN 34  // MQ-135 sensor connected to ADC pin GPIO34
// DHT dht(DHTPIN, DHTTYPE);

// // Remove hardcoded SSID and password
// const char* serverName = "http://192.168.100.229:5000/api/sensors"; 

// void setup() {
//     Serial.begin(115200);

//     // WiFiManager setup
//     WiFiManager wm;
//     bool res;
//     res = wm.autoConnect("ESP32-Setup", "esp32password");

//     if (!res) {
//         Serial.println("❌ Failed to connect or timed out.");
//         ESP.restart();
//     }

//     Serial.println("✅ Connected to WiFi using WiFiManager!");
//     Serial.print("ESP32 IP Address: ");
//     Serial.println(WiFi.localIP());

//     // Initialize OTA
//     ArduinoOTA.setHostname("ESP32-OTA");
//     ArduinoOTA.setPassword("yourpassword");
//     ArduinoOTA.begin();

//     dht.begin();
//     pinMode(MQ135_PIN, INPUT);
// }


// unsigned long lastSendTime = 0;
// const unsigned long sendInterval = 5000; // 5 seconds

// void loop() {
//     ArduinoOTA.handle(); // Should be called as frequently as possible

//     if (millis() - lastSendTime >= sendInterval) {
//         lastSendTime = millis();

//         if (WiFi.status() == WL_CONNECTED) {
//             float temperature = dht.readTemperature();
//             float humidity = dht.readHumidity();
//             int air_quality = analogRead(MQ135_PIN);

//             Serial.println("Reading sensor values...");
//             Serial.print("Temperature: "); Serial.println(temperature);
//             Serial.print("Humidity: "); Serial.println(humidity);
//             Serial.print("Air Quality: "); Serial.println(air_quality);

//             WiFiClient client;
//             HTTPClient http;
//             http.begin(client, serverName);
//             http.addHeader("Content-Type", "application/json");

//             StaticJsonDocument<200> doc;
//             doc["temperature"] = temperature;
//             doc["humidity"] = humidity;
//             doc["air_quality"] = air_quality;

//             String requestBody;
//             serializeJson(doc, requestBody);
//             Serial.print("Sending JSON: ");
//             Serial.println(requestBody);

//             int httpResponseCode = http.POST(requestBody);
//             Serial.print("HTTP Response code: ");
//             Serial.println(httpResponseCode);

//             if (httpResponseCode > 0) {
//                 String response = http.getString();
//                 Serial.println("Server Response: " + response);
//             } else {
//                 Serial.println("Error sending request. Check server or WiFi.");
//             }

//             http.end();
//         } else {
//             Serial.println("WiFi Disconnected");
//         }
//     }
// }


