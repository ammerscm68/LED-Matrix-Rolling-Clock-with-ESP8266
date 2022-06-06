# LED-Matrix-Rolling-Clock-with-ESP8266
LED-Matrix-Rolling-Clock with ESP8266 (WEMOS D1 mini)

# Installation: 
When the clock is switched on for the first time, an access point is started. (SSID: LED-Matrix-Clock)  <br>
In the captive portal (standard IP: 192.168.4.1) where you can select your own WLAN.  <br>
The API-key and the city-ID of OpenWeatherMap can also be entered here. (Optional)  <br>
The display can also be switched on and off at a specific time, for example if it is disturbing at night. (Optional)  <br>
Always enter the switch-on and switch-off times of the display without a leading zero. (e.g. 7:5:0 for 7h 5min 0sec)

# Changing the WLAN or the WLAN password:
The installation of a button (pin D3 to ground) is necessary here.
Press the button until the access point is started.
The button also serves to switch the display on and off at the same time.

# Webinterface:
The watch also uses a web interface to make settings. The call is made via the IP address of the clock.
