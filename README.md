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
  
  D1 = DHT11 / DHT22 - Sensor (Optional)<br>
  D2 = Passive Buzzer KY-006 (Optional)<br>
  D3 = Hardware Button (Optional but recommended)<br>
  D5 = LED to GND - for Humity Warning (Optional) <br> 

# Installation: 
When the clock is switched on for the first time, an access point is started. (SSID: LED-Matrix-Clock)  <br>
In the captive portal (standard IP: 192.168.4.1) where you can select your own WLAN.  <br>
The API-key and the city-ID of "OpenWeatherMap" can also be entered here. (Optional)  <br>
The API-Key for "NewsAPI" can also be entered here. (Optional) <br>
The display can also be switched on and off at a specific time, for example if it is disturbing at night. (Optional)  <br>

# Changing the WLAN password:
When a button installed is (**pin D3 to ground**) then here. <br>
Press the button until the access point is started (10 times). <br>
The button also serves to switch the display on and off at the same time. <br>
Or changing over Webinterface right bottom of the root site. <br>

# Webinterface:
The watch also uses a web interface to make settings. <br>
The call is made via the IP address of the clock or <br>
"http://ledmatrixuhr.local" (German Version) / "http://ledmatrixclock.local" (English Version)<br>
<br>
# Software:
**New from version 9.2.8**<br>
Bug Fix <br>
**New from version 9.2.7**<br>
minor improvements<br>
**New from version 9.2.6**<br>
Bug Fix <br>
**New from version 9.2.5**<br>
Actions (weather and news) changed from 5 minute interval to 3 minute interval <br>
Bug Fix <br>
**New from version 9.2.3**<br>
Bug Fix <br>
Program compression <br>
**New from version 9.2.2**<br>
Bug Fix <br>
**New from version 9.2.1**<br>
Funtion "void greetings()" renamed in "void Greetings_and_Holiday()"<br>
Function  "void Greetings_and_Holiday()" supplemented by additional holidays<br>
If you specify the german federal state, german non-federal holidays are also displayed<br>
The german federal state is specified in the main file (.ino)<br>
If no german federal state is specified, only german federal holidays are displayed<br>
**New from version 9.1.0**<br>
Function "void loop" highly optimized<br>
Bug Fix <br>
**New from version 9.0.7**<br>
Bug Fix in the "Captive Portal" Modul<br>
**New from version 9.0.6**<br>
Display of indoor and outdoor air humidity in the matrix display <br>
Better distinction between indoor and outdoor temperatures <br>
Better distinction between indoor and outdoor air humidity <br>
Integration of an external LED to warn if the humidity is too high or too low <br>
Optimization of some Void functions <br>
Versions 9.0.0 - 9.0.5 are internal versions <br>
---------------------------------------------<br>
**New from version 8.1.5**<br>
Sound and Owner function improvement <br>
**New from version 8.1.2**<br>
Small correction in the settings Site on Webinterface <br>
**New from version 8.1.1**<br>
Look Clock Owner in the settings on Webinterface <br>
**New from version 8.1.0**<br>
Status passive Buzzer look in the settings on Webinterface <br>
**New from version 8.0.0**<br>
Add Display Check function. Button on the Webinterface <br>
**New from version 7.2.0**<br>
improvement the "greetings()" function <br>
**New from version 7.1.7**<br>
Bug Fix<br>
**New from version 7.1.6**<br>
minor improvements<br>
**New from version 7.1.5**<br>
minor improvements<br>
**New from version 7.1.3/4**<br>
minor improvements<br>
**New from version 7.1.2**<br>
At midnight display of ghosts (Optional set in sketch)<br>
**New from version 7.1.0/1**<br>
Animation when starting and turning the clock off and on via the web interface<br>
**New from version 7.0.9**<br>
Warning if humidity in the room is too low or too high.<br>
**New from version 7.0.8**<br>
minor improvements<br>
Selection of the temperature and humidity sensor type in the sketch (DHT11 or DHT22)<br> 
**New from version 7.0.5/6/7**<br>
Bug Fix<br>
**New from version 7.0.4**<br>
Slightly improved startup sound (only if buzzer installed)<br>
**New from version 7.0.2**<br>
Due to lack of memory, the entire software has been revised<br>
Integration of a passive piezo buzzer. Activation via Captive Portal (Optional)<br>
**New from version 6.1.6/7/8**<br>
bug fix<br>
**New from version 6.1.5**<br>
Corrected logic errors and minor Website improvements<br>
**New from version 6.1.4**<br>
Captive Portal and Webserver improvement (stabilized)<br>
**New from version 6.1.3**<br>
Website "fine Tuning"<br>
**New from version 6.1.0**<br>
Automatic saving of Websitesettings and bug fix<br>
**New from version 6.0.9**<br>
little bug fix<br>
**New from version 6.0.7/8**<br>
Website "fine Tuning"**<br>
**New from version 6.0.6**<br>
Revised Captive Portal and minor improvements<br>
**New from version 6.0.0**<br>
Webinterface completely revised<br>
**New from version 5.72 and 5.73:<br>
bug fix <br>
**New from version 5.71**<br>
improvement Modul **"news.ino"** and **"myfunctions.ino"** <br>
**New from version 5.60**<br>
minor improvements<br>
**New from version 5.56**<br>
bug fix <br>
**New from version 5.54**<br>
Display of the AccessPoint IP-Address <br>
**New from version 5.51**<br>
new Captive Portal (WiFi-Manager) <br>
bug fix <br>
<br>
**New from version 5.46**<br>
**Now with Arduino OTA.<br>
Sketch updates without a USB cable over the network<br>
<br>
**New from version 5.42**<br>
Improved webinterface <br>
bug fix <br>
The webinterface can now be reached with the DNS name "http://ledmatrixuhr.local" <br>
