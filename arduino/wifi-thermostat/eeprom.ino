void saveSettingsToEeprom() {
  preferences.putString(EEPROM_WIFI_SSID, settings.WIFI_SSID);
  preferences.putString(EEPROM_WIFI_PASSWORD, settings.WIFI_PASSWORD);
  preferences.putString(EEPROM_WIFI_API_URL, settings.API_URL);
  Serial.println("Saving to EEPROM: " + settings.WIFI_SSID + ", " + settings.WIFI_PASSWORD + ", " + settings.API_URL);
}

void readSettingsFromEeprom() {
  String ssid = preferences.getString(EEPROM_WIFI_SSID, "");
  settings.WIFI_SSID = ssid;

  String wifiPassword = preferences.getString(EEPROM_WIFI_PASSWORD, "");
  settings.WIFI_PASSWORD = wifiPassword;

  String apiUrl = preferences.getString(EEPROM_WIFI_API_URL, "");
  settings.API_URL = apiUrl;
  
  Serial.println("Reading from EEPROM: " + settings.WIFI_SSID + ", " + settings.WIFI_PASSWORD + ", " + settings.API_URL);   
}
