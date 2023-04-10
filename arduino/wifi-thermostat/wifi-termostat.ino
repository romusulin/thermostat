#include <WiFi.h>
#include <ESP32Time.h>
#include <Preferences.h>
#include <Arduino_JSON.h>
#include <HTTPClient.h>

Preferences preferences;
HTTPClient http;
ESP32Time rtc(0);

const int DELAY_RELAY = 1000;
const int DELAY_FETCH_DATA = 1000 * 10;
const int DELAY_TIME_LOG = 1000 * 60;
const int WIFI_RETRY_COUNT = 5;
const char* EEPROM_WIFI_SSID = "ws";
const char* EEPROM_WIFI_PASSWORD = "wp";
const char* EEPROM_WIFI_API_URL = "au";

enum RelayStatus {CLOSED, OPEN};

struct Settings_s {
  String WIFI_SSID;
  String WIFI_PASSWORD;
  String API_URL;
};

struct Delay_s {
  unsigned long startTime;
  int timerDelay;
};

Settings_s settings;
Delay_s relayDelay;
Delay_s dataFetchDelay;
Delay_s timeLogDelay;
RelayStatus relayStatus;
bool isConfigurationInProgress;
bool shouldReboot;
bool hours[24] = {};

bool isWiFiConnected() {
  return WiFi.status() == WL_CONNECTED;
}

bool connectToWifi() {
  WiFi.mode(WIFI_MODE_STA);
  WiFi.begin(settings.WIFI_SSID.c_str(), settings.WIFI_PASSWORD.c_str());

  for (int i = 0; i < WIFI_RETRY_COUNT && !isWiFiConnected(); i++) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.print("WiFi network IP: ");
  Serial.println(WiFi.localIP());

  return isWiFiConnected();
}

bool isTimedOut(Delay_s* d) {
  unsigned long currentMs = millis();
  bool isReady = false;
  if (currentMs - d->startTime >= d->timerDelay) {
    d->startTime = currentMs;
    isReady = true;
  }

  return isReady;
}

void fetchData() {
  http.begin(settings.API_URL);
  http.addHeader("Content-Type", "Content-Type: application/json"); 
  int httpResponseCode = http.GET();

  if (httpResponseCode <= 0) {
    Serial.println("Http fetch error:" + String(httpResponseCode));
  }
  
  String payload = http.getString();
  http.end();
  Serial.print("Http status: " + String(httpResponseCode));
  Serial.println("; Payload: " + String(payload));
  JSONVar result = JSON.parse(payload);
  
  unsigned long epoch = result["unixTimestamp"];
  String resultHours = result["hours"];

  setHours(resultHours);
  setRtcTime(epoch);
}

void setHours(String str) {
  for (int i = 0; i < str.length(); i++) {
    char c = str[i];
    hours[i] = c == '1' ? true : false;
  }
}

void loopWiFi() {
  if (isTimedOut(&dataFetchDelay)) {
    if (!isWiFiConnected()) {
      connectToWifi();
    } else {
      fetchData();
    }
  }
}

void loopRelay() {
  if (isTimedOut(&relayDelay)) {
    if (shouldBeRunning() && relayStatus == CLOSED) {
      Serial.print("Opening relay...");
      digitalWrite(15, HIGH);
      digitalWrite(14, HIGH);
      relayStatus = OPEN;
      Serial.println("done!");
    } else if (!shouldBeRunning() && relayStatus == OPEN) {
      Serial.print("Closing relay...");
      relayStatus = CLOSED;
      digitalWrite(15, LOW);
      digitalWrite(14, LOW);
      Serial.println("done!");
    }
  }
}

void loopDataLog() {
  if (isTimedOut(&timeLogDelay)) {
    Serial.println("# " + String(rtc.getTime("%T")));
  }
}

void setup() {
  Serial.begin(115200);
  rtc.setTime(0, 0, 12, 1, 1, 2022);
  preferences.begin("app", false);

  pinMode(15, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(BUTTON_BUILTIN, INPUT_PULLUP);

  relayDelay = {0, DELAY_RELAY};
  dataFetchDelay = {0, DELAY_FETCH_DATA};
  timeLogDelay = {0, DELAY_TIME_LOG};

  relayStatus = CLOSED;

  readSettingsFromEeprom();
}

void loop() {
  if (isConfigurationInProgress) {
    return;
  }

  if (shouldReboot) {
    Serial.println("Restart in progress...");
    delay(15000);
    ESP.restart();
  }

  int isButtonPressed = digitalRead(BUTTON_BUILTIN) == LOW;
  if (isButtonPressed) {
    isConfigurationInProgress = true;
    runConfigurationAccessPoint();
    delay(5000);
  }

  loopWiFi();
  loopRelay();
  loopDataLog();
}