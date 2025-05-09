# LED-Matrix-Rolling-Clock-with-ESP8266
LED-Matrix-Rolling-Clock with ESP8266 (WEMOS D1 mini or Wemos D1 mini Pro)

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
  D3 = Hardware Button to GND (Optional but recommended)<br>
  D5 = LED to GND - for Humity and Radioactivity Warning (Optional) <br> 
<br>
<br>
# Settings in the Arduino IDE (Menu Item “Tools”) <br>
# For ESP8266 (Wemos/LOLIN D1 Mini / Wemos/LOLIN D1 Mini Pro)

![grafik](https://github.com/user-attachments/assets/cf63eb2f-9dc9-4c2e-aa97-f560f226a1eb)

**Attention: All settings on the ESP8266 will be lost !!!**
<br>
<br>
# Installation: 
When the Clock is switched on for the first time, an Access Point is started. <br>
<br> SSID: <br>
**LED-Matrix-Clock** (English)<br>
**LED-Matrix-Uhr** (German) <br>
**LED-Matrice-Horlog** (French) <br>
<br>
In the Captive Portal (standard IP: 192.168.4.1) where you can select your own WLAN.  <br>
The API-key and the City-ID of "OpenWeatherMap" can also be entered here. (Optional)  <br>
The API-Key for "NewsData" can also be entered here. (Optional) <br>
The GMCMap.com History-ID and the GMT-Offset can also entered here. (Optional) <br>
The up to 3 Owners of the Clock can also be entered here. (Optional) <br>

<br>

# Changing the WLAN password:
When a button installed is (**pin D3 to ground**) then here. <br>
Press the button until the access point is started (10 times). <br>
The button also serves to switch the display on and off at the same time. <br>
Or changing over Webinterface right bottom of the root site. <br>

<br>

# Webinterface:
The watch also uses a web interface to make settings. <br>
The call is made via the IP address of the clock or <br>
"http://ledmatrixuhr.local" (German Version) <br>
"http://ledmatrixclock.local" (English Version)<br>
"http://ledmatricehorloge.local" (French Version) <br>

<br>

# GMCMap.com:
Example Setting in Captive Portal: <br>
GMCMap.com - History-ID: **12345678** <br>
GMCMap.com - GMT-Offset: **2** <br>
(Web URL: www.gmcmap.com/historyData.asp?Param_ID=12345678&systemTimeZone=2)  <br>
<br>
Explanation of the "GMCMapMidnightAccessDelay" Function:<br>
In the "gmc.ino" File, after downloading the Current Data, the <br>
Current Date is compared with the last Date on the GMCMap <br>
Website History of the Sensor. If the current Date does not <br>
Match the last Date in the GMCMap History, the Clock Displays <br>
an Error Message.In the worst case, this can happen after <br> 
Midnight, if there is no new GMCMap Data. With the <br>
"GMCMapMidnightAccessDelay" Function, you can now Wait a <br>
certain Number of Minutes until the next Values ​​are downloaded <br>
from GMCMap Website in Order to avoid an accidental Error Message.<br>
<br>
Example: The GMC-Sensor sends its Values ​​to the GMCMap.com <br>
Website every 10 Minutes. In this Case, a Value of 30 for the <br>
"GMCMapMidnightAccessDelay" Function would be useful. <br>
This means that after 30 Minutes GMCMap Data will be downloaded again. <br>
The Default Value is 30 Minutes. <br>

<br>

# Video:
YouTube Video: (German)<br>
https://www.youtube.com/watch?v=6Jl69GYtSMw <br>
YouTube Video: (English)<br>
https://www.youtube.com/watch?v=ZmM3IA_AyQY <br>

<br>

# Software:
**New from Version 10.2.2**<br>
BugFix <br>
Add a Canadian Version <br>
**New from Version 10.2.1**<br>
Module "GMC.ino" further optimized. This resulted in 2-3 kB of RAM being regained.<br>
**New from Version 10.2.0**<br>
Versions 10.1.3 to 10.1.9 are internal Versions. <br>
The NTP Server Pool can now be Changed in the "Captiv Portal".<br>
Due to lack of Memory (RAM), from this Version onwards only either the "GMC.ino" Module or the "News.ino" module can be used. <br>
Source Code further optimized.<br>
BugFix <br>
**New from Version 10.1.2**<br>
Version 10.1.1 is a internal beta<br>
Added Valentine's Day to the Holidays even if it is not a Holiday, Many Hearts are Displayed <br>
The Time Zone can now be changed in the Captive Portal. <br>
The NTP Server issue should now be Resolved <br>
**New from Version 10.1.0**<br>
From this Version on you can set in the Captive Portal whether Midnight Ghosts should appear or not. <br>
(Previously this could only be changed directly in the Sketch) <br>
**New from Version 10.0.9**<br>
Correction of a small Cosmetic defect that bothered me. Sorry <br>
**New from Version 10.0.8**<br>
Display of the Clock Status from the last Start in the Web Interface. (Example: WLAN failure) <br>
Stabilization of the Web Interface <br>
Major Bugfix with Notepad++ and the Compare Function <br>
Version 10.0.5-10.0.7 are internal Versions <br>
**New from Version 10.0.4**<br>
Inserting the Function "GMCMapMidnightAccessDelay" - Explanation see above <br>
BugFix <br>
**New from Version 10.0.3**<br>
Fix logic Error in "GMSMap.com" Function <br>
**New from Version 10.0.2** <br>
With corresponding OpenWeatherMap Data, the Clock now also shows the probability of Snowfall (Chance of Snow) <br>
**New from Version 10.0.1**<br>
New Major Version (Version 10.0.0 is a internal beta)<br>
Add a new Function "gmcmap.com" - Download the current Radioactivity Values ​​at the selected Location.<br>
[The Necessary Parameters can be set in the "Captive Portal" (see above)] <br>
Display of the current WiFi Signal strength for better Location selection of the Clock <br>
(On the Matrix Display [Alert] and on the Web Interface) <br>
Automatic deactivation of Online Functions in case of very poor WiFi Signal strength <br>
The maximum of 3 Owners of the Clock can now be changed at any Time in the "Captive Portal".<br>
Optimization of the source Code <br>
BugFix <br>
**New from Version 9.9.4**<br>
BugFix. <br>
**New from Version 9.9.3**<br>
BugFix. <br>
**New from Version 9.9.2**<br>
"NewsAPI.org" has stopped Service in Europe. In Search<br>
after a new News Service I came across “NewsData.io”.<br>
I have adapted the “News.ino” Module accordingly.<br>
since “NewsData.io” only Outputs the Data via HTTPS, I had to<br>
from <WiFiClient.h> to <WiFiClientSecure.h>.<br>
unfortunately, this costs significantly more Memory (RAM).<br>
The Settings of the Arduino IDE need to be Adjusted.<br>
After this Setting, all Settings on the ESP8266 go lost.<br>
The Corresponding Settings in the Arduino IDE are Shown (see above).<br>
Other Minor Improvements have been added.<br>
BugFix. <br>
Version 9.8.6 - Version 9.9.1 are internal Versions.<br>
**New from Version 9.8.5**<br>
Corrected Logic Error in "GoMatrixAction()" function.<br>
**New from Version 9.8.4**<br>
Version 9.8.2 to 9.8.3 Internal Versions<br>
Improvement of some Functions<br>
Improving stability<br>
Elimination of logic Errors<br>
**New from Version 9.8.1**<br>
If the DHT Sensor is defective and cannot be Replaced immediately, it is now possible to deactivate <br>
the Sensor until it is Replaced so that the Values ​​are no longer displayed on the LED Matrix. (Sensor Calibration)<br>
Integration of the "Pushover Messaging Service" in the Captiv Portal (Access Point).<br>
The Clock sends a Message, Example to the Smartphone or Tablet, if the Airhumidity in the Room is too high or too low. (Optional)<br>
**New from Version 9.7.1**<br>
Version 9.5.6 to 9.7.0 Internal Versions <br>
The current Weather and current News can now be viewed on the Website. <br>
The Calibration (Offset) of the DHT Sensor can now be done live. (No longer via the Access Point) <br>
Further optimization of the source code <br>
Huge BugFix <br>
**New from Version 9.5.5**<br>
More Source code optimization<br>
BugFix <br>
**New from Version 9.5.4**<br>
Source code optimization for more free memory<br>
**New from version 9.5.3**<br>
Internal version<br>
**New from Version 9.5.2**<br>
If OpenWeatherMap is activated, the outside temperature and outside air humidity are <br>
now also displayed on the Website. <br>
**New from Version 9.5.1**<br>
Date and Calendar Week View improved <br>
**New from Version 9.5.0**<br>
The current date is now output with the current calendar week <br>
BugFix <br>
**New from Version 9.4.1**<br>
Improvement of WIFI and NTP routine <br>
**New from Version 9.4.0**<br>
For the German and Austrian versions, the federal state can now be changed in the Captiv portal.<br>
minor improvements<br>
BugFix <br>
**New from Version 9.3.3**<br>
BugFix <br>
**New from Version 9.3.2**<br>
Switching the Temperature output of the US Version from Celsius to Fahrenheit <br>
Add a Version for Ireland and Austria <br>
BugFix <br>
**New from Version 9.3.1**<br>
Revision of the function "void Greetings_and_Holiday()"<br>
Bug Fix <br>
Add English (US) Version <br>
**New from Version 9.3.0**<br>
Bug Fix <br>
**New from Version 9.2.9**<br>
Split in German (de) and English (UK) Version<br>
**New from Version 9.2.8**<br>
Bug Fix <br>
**New from Version 9.2.7**<br>
minor improvements<br>
**New from Version 9.2.6**<br>
Bug Fix <br>
**New from Version 9.2.5**<br>
Actions (weather and news) changed from 5 minute interval to 3 minute interval <br>
Bug Fix <br>
**New from Version 9.2.3**<br>
Bug Fix <br>
Program compression <br>
**New from Version 9.2.2**<br>
Bug Fix <br>
**New from Version 9.2.1**<br>
Funtion "void greetings()" renamed in "void Greetings_and_Holiday()"<br>
Function  "void Greetings_and_Holiday()" supplemented by additional holidays<br>
If you specify the german federal state, german non-federal holidays are also displayed<br>
The german federal state is specified in the main file (.ino)<br>
If no german federal state is specified, only german federal holidays are displayed<br>
**New from Version 9.1.0**<br>
Function "void loop" highly optimized<br>
Bug Fix <br>
**New from Version 9.0.7**<br>
Bug Fix in the "Captive Portal" Modul<br>
**New from Version 9.0.6**<br>
Display of indoor and outdoor air humidity in the matrix display <br>
Better distinction between indoor and outdoor temperatures <br>
Better distinction between indoor and outdoor air humidity <br>
Integration of an external LED to warn if the humidity is too high or too low <br>
Optimization of some Void functions <br>
Versions 9.0.0 - 9.0.5 are internal versions <br>
---------------------------------------------<br>
**New from Version 8.1.5**<br>
Sound and Owner function improvement <br>
**New from Version 8.1.2**<br>
Small correction in the settings Site on Webinterface <br>
**New from Version 8.1.1**<br>
Look Clock Owner in the settings on Webinterface <br>
**New from Version 8.1.0**<br>
Status passive Buzzer look in the settings on Webinterface <br>
**New from Version 8.0.0**<br>
Add Display Check function. Button on the Webinterface <br>
**New from Version 7.2.0**<br>
improvement the "greetings()" function <br>
**New from Version 7.1.7**<br>
Bug Fix<br>
**New from Version 7.1.6**<br>
minor improvements<br>
**New from Version 7.1.5**<br>
minor improvements<br>
**New from Version 7.1.3/4**<br>
minor improvements<br>
**New from Version 7.1.2**<br>
At midnight display of ghosts (Optional set in sketch)<br>
**New from Version 7.1.0/1**<br>
Animation when starting and turning the clock off and on via the web interface<br>
**New from Version 7.0.9**<br>
Warning if humidity in the room is too low or too high.<br>
**New from Version 7.0.8**<br>
minor improvements<br>
Selection of the temperature and humidity sensor type in the sketch (DHT11 or DHT22)<br> 
**New from Version 7.0.5/6/7**<br>
Bug Fix<br>
**New from Version 7.0.4**<br>
Slightly improved startup sound (only if buzzer installed)<br>
**New from Version 7.0.2**<br>
Due to lack of memory, the entire software has been revised<br>
Integration of a passive piezo buzzer. Activation via Captive Portal (Optional)<br>
**New from Version 6.1.6/7/8**<br>
bug fix<br>
**New from Version 6.1.5**<br>
Corrected logic errors and minor Website improvements<br>
**New from Version 6.1.4**<br>
Captive Portal and Webserver improvement (stabilized)<br>
**New from Version 6.1.3**<br>
Website "fine Tuning"<br>
**New from Version 6.1.0**<br>
Automatic saving of Websitesettings and bug fix<br>
**New from Version 6.0.9**<br>
little bug fix<br>
**New from Version 6.0.7/8**<br>
Website "fine Tuning"**<br>
**New from Version 6.0.6**<br>
Revised Captive Portal and minor improvements<br>
**New from Version 6.0.0**<br>
Webinterface completely revised <br>
**New from Version 5.72 and 5.73:<br>
bug fix <br>
**New from Version 5.71**<br>
improvement Modul **"news.ino"** and **"myfunctions.ino"** <br>
**New from Version 5.60**<br>
minor improvements<br>
**New from Version 5.56**<br>
bug fix <br>
**New from Version 5.54**<br>
Display of the AccessPoint IP-Address <br>
**New from Version 5.51**<br>
new Captive Portal (WiFi-Manager) <br>
bug fix <br>
<br>
**New from Version 5.46**<br>
**Now with Arduino OTA.<br>
Sketch updates without a USB cable over the network<br>
<br>
**New from Version 5.42**<br>
Improved webinterface <br>
bug fix <br>
The webinterface can now be reached with the DNS name "http://ledmatrixuhr.local" <br>
<br>
