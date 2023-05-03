# LED-Matrix-Rolling-Clock-with-ESP8266
LED-Matrix-Rolling-Clock with ESP8266 (WEMOS D1 mini)

# Pinout:<br>
ESP8266 ----- Max7219 <br>
  VCC ------------ VCC <br>
  GND ----------- GND <br>
  D6  ------------- CLK <br>
  D7  ------------- CS <br>
  D8  ------------- DIN <br>
  
  VCC = 5V <br>
  
  D1 = DHT11 - Sensor <br>

# Installation: 
When the clock is switched on for the first time, an access point is started. (SSID: LED-Matrix-Clock)  <br>
In the captive portal (standard IP: 192.168.4.1) where you can select your own WLAN.  <br>
The API-key and the city-ID of "OpenWeatherMap" can also be entered here. (Optional)  <br>
The API-Key for "NewsAPI" can also be entered here. (Optional) <br>
The display can also be switched on and off at a specific time, for example if it is disturbing at night. (Optional)  <br>
Always enter the switch-on and switch-off times of the display without a leading zero. (e.g. 7:5 for 7h 5min)

# Changing the WLAN or the WLAN password:
When a button installed is (**pin D3 to ground**) then here. <br>
Press the button until the access point is started (10 times). <br>
The button also serves to switch the display on and off at the same time. <br>
Or changing over Webinterface right bottom of the root site. <br>

# Webinterface:
The watch also uses a web interface to make settings. The call is made via the IP address of the clock or "http://ledmatrixclock.local"<br>
<br>
# Software:
New from version 6.0.7/8<br>
Website "fine Tuning"<br>
New from version 6.0.6:<br>
Revised Captive Portal and minor improvements<br>
New from version 6.0.0:<br>
Webinterface completely revised<br>
New from version 5.72 and 5.73:<br>
bug fix <br>
New from version 5.71:<br>
improvement Modul **"news.ino"** and **"myfunctions.ino"** <br>
New from version 5.60:<br>
minor improvements<br>
New from version 5.56:<br>
bug fix <br>
New from version 5.54:<br>
Display of the AccessPoint IP-Address <br>
New from version 5.51:<br>
new Captive Portal (WiFi-Manager) <br>
bug fix <br>
<br>
New from version 5.46:<br>
Now with Arduino OTA.<br>
Sketch updates without a USB cable over the network<br>
<br>
New from version 5.42: <br>
Improved webinterface <br>
bug fix <br>
The webinterface can now be reached with the DNS name "http://ledmatrixclock.local" <br>
