bool shouldBeRunning() {
  int currentHour = rtc.getHour(true);

  return hours[currentHour] ? true : false;
}

void setRtcTime(unsigned long unixTs) {
  rtc.setTime(unixTs);
}