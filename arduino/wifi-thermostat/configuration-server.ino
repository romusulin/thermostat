#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char* ap_ssid = "Termostat";
const char* ap_password = "";
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
	<style>
	.hour {
		background-color: lightcyan;
		border-radius: 3px;
	}
	input[type=checkbox] {
		transform: scale(2);
		margin-left:4px;
	}
  input[type=submit] {
		height: 3em;
    width: 12em;
	}
	span {
		margin: 3px;
		padding: 3px;
		display: inline-block;
	}
	</style>
	<title>Termostat</title>
  
	<meta name="viewport" content="width=device-width, 
initial-scale=1">
</head><body>
<form action="/" method="post">
    <span>SSID: <input name="ssid" type="input" value="%SSID%"></span><br>
    <span>Password: <input name="password" type="input" value="%PASSWORD%"></span><br>
    <span>API URL: <input name="apiurl" type="input" value="%APIURL%"></span>
    <br><br>
	<input type="submit" value="Spremi">
</form>
</body></html>)rawliteral";

IPAddress local_IP(192, 168, 4, 27);
IPAddress gateway(192, 168, 4, 9);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);
IPAddress secondaryDNS(8, 8, 4, 4);
AsyncWebServer server(80);

const char* KEY_SSID = "ssid";
const char* KEY_PASSWORD = "password";
const char* KEY_API_URL = "apiurl";

String processor(const String& var) {
  if (var == "SSID") {
	return settings.WIFI_SSID;
  } else if (var == "PASSWORD") {
	return settings.WIFI_PASSWORD;
  } else if (var == "APIURL") {
    return settings.API_URL;
  }

  return String();
}

void runConfigurationAccessPoint() {
  WiFi.mode(WIFI_MODE_AP);
	Serial.print("Setting soft-AP configuration...");
	Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "done!" : "failed!");
	Serial.print("Setting soft-AP...");
	Serial.println(WiFi.softAP(ap_ssid) ? "done!" : "failed!");
	Serial.print("Hotpost IP address: ");
	Serial.println(WiFi.softAPIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    Serial.println("GET /");
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/", HTTP_POST, [](AsyncWebServerRequest* request) {
    Serial.println("POST /");
    int paramsCount = request->params();
    for (int i = 0; i < paramsCount; i++) {
      AsyncWebParameter* p = request->getParam(i);
      String key = p->name().c_str();
      String value = p->value().c_str();
      value.trim();
      Serial.println("Changing: " + String(key) + " -> " + String(value));

      if (key == KEY_SSID) {
        settings.WIFI_SSID = value;
      } else if (key == KEY_PASSWORD) {
        settings.WIFI_PASSWORD = value;
      } else if (key == KEY_API_URL) {
        settings.API_URL = value;
      }
    }

    saveSettingsToEeprom();
    shouldReboot = true;
    request->redirect("/");
  });
  
  server.begin();
}
