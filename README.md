# LED-Matrix-Rolling-Clock-with-ESP8266
LED-Matrix-Rolling-Clock with ESP8266 (WEMOS D1 mini)

# Pinout:<br>
ESP8266 ----- Max7219 <br>
  VCC ------------ VCC <br>
  GND ----------- GND <br>
  D6  ------------ CLK <br>
  D7  ---------- CS <br>
  D8  ---------- DIN <br>

# Installation: 
When the clock is switched on for the first time, an access point is started. (SSID: LED-Matrix-Clock)  <br>
In the captive portal (standard IP: 192.168.4.1) where you can select your own WLAN.  <br>
The API-key and the city-ID of "OpenWeatherMap" can also be entered here. (Optional)  <br>
The API-Key for "NewsAPI" can also be entered here. (Optional) <br>
The display can also be switched on and off at a specific time, for example if it is disturbing at night. (Optional)  <br>
Always enter the switch-on and switch-off times of the display without a leading zero. (e.g. 7:5 for 7h 5min)

# Changing the WLAN or the WLAN password:
When a button installed is (pin D3 to ground) then here. <br>
Press the button until the access point is started (10 times). <br>
The button also serves to switch the display on and off at the same time. <br>
Or changing over Webinterface right bottom of the root site. <br>

# Webinterface:
The watch also uses a web interface to make settings. The call is made via the IP address of the clock. <br>
<br>
# Software:
New from version 5.42: <br>
Improved webinterface <br>
bug fix <br>
The webinterface can now be reached with the DNS name "http://ledmatrixclock.local" <br>
