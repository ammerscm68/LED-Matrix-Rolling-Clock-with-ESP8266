void DHTHTML() { 
  String HTMLString = F("");
  String RefreshTime = F("30"); // Actualiser le site Web après X secondes
  if (InTempOffset == "999") {InTempValue = "0";} else {InTempValue = InTempMessure();} // Récupérer la valeur de la température
  if (SerialMonitor) {Serial.println("Indoor Temperature: "+InTempValue+" °C");}
  MyWaitLoop(500); // courte attente
  if (InTempOffset == "999") {InHumValue = "0";} else {InHumValue = InHumMessure();} // Retrieve Air Humidity Value
  if (SerialMonitor) {Serial.println("Humidité de l'air Intérieur: "+InHumValue+" %");}
  String TempValueColor, HumValueColor, RefreshButton;
  if (InTempValue != "-" && InHumValue != "-") {
  IntInTempValue = InTempValue.toInt(); 
  IntInHumValue = InHumValue.toInt(); 
  RefreshButton = "href =\"tempvalue\"><button class=\"button\" style=\"margin-left: 100px;height: 48px; width: 180px;color: blue;font-size: 16px;\">Rafraîchir</button>";} else {
  RefreshButton = "href =\"/\"><button class=\"button\" style=\"margin-left: 100px;height: 48px; width: 180px;color: blue;font-size: 16px;\">Rafraîchir</button>";}
  // Température d'évaluation
  if (IntInTempValue < 10) {
  TempValueColor = "<tr><td style=\"height: 10px;\">Température Intérieure actuelle:&nbsp;&nbsp;</td><td id='Bvalue'><span>"+InTempValue+"</span>°C</td></tr>";  
  } else {
  if (IntInTempValue >= 10 && IntInTempValue <= 28) {TempValueColor = "<tr><td style=\"height: 10px;\">Température Intérieure actuelle:&nbsp&nbsp</td><td id='Gvalue'><span>"+InTempValue+"</span>°C</td></tr>";} else {  
  TempValueColor = "<tr><td style=\"height: 10px;\">Température Intérieure actuelle:&nbsp;&nbsp;</td><td id='Rvalue'><span>"+InTempValue+"</span>°C</td></tr>";}
  }
  // Evaluation of Air Humidity
  if (IntInHumValue <= MinComfortHum || IntInHumValue >= MaxComfortHum) {
  digitalWrite(HWLEDPin, HIGH); // LED allumée  
  HumValueColor = "style=\"color: #FC1E17; font-size: 18px; margin-left: 430px;\">at an Air Humidity of:&nbsp;"+InHumValue+"";} else { 
  digitalWrite(HWLEDPin, LOW); // LED off   
  HumValueColor = "style=\"color: lime; font-size: 18px; margin-left: 455px;\">à une Humidité de l'Air de:&nbsp;"+InHumValue+"";}
  
  HTMLString = F("<!DOCTYPE html>");
  HTMLString += F("<html lang='fr-CA'>");
  HTMLString += F("<head>");
  HTMLString += F("<title>Horloge-Matricielle-LED Affichage de la Température</title>");
  HTMLString += F("<meta http-equiv='content-type' content='text/html'; charset='utf-8'>");
  HTMLString += "<meta http-equiv=\"refresh\" content=\""+RefreshTime+"\">";
  HTMLString += F("<style>");
  HTMLString += F("body { background-color: #585858; font-size: 32px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; }");
  HTMLString += F("h1 { color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }");
  HTMLString += F("h2 { font-size: 28px; margin-top: 0px; margin-bottom: 10px; }");
  HTMLString += F("h3 { font-size: 32px; margin-top: 0px; margin-bottom: 10px; }"); 
  HTMLString += F("h4 { font-size: 28px; margin-top: 0px; margin-bottom: 10px; }"); 
  HTMLString += F("#Rvalue { width: 200px; height: 80px; border: 5px solid #F2F2F2; text-align:center; padding: 1px; color: #FC1E17; background-color: #000000; font-size: 60px; }"); 
  HTMLString += F("#Gvalue { width: 200px; height: 80px; border: 5px solid #F2F2F2; text-align:center; padding: 1px; color: lime; background-color: #000000; font-size: 60px; }"); 
  HTMLString += F("#Bvalue { width: 200px; height: 80px; border: 5px solid #F2F2F2; text-align:center; padding: 1px; color: #2ECCFA; background-color: #000000; font-size: 60px; }"); 
  HTMLString += F("</style>"); 
  HTMLString += F("</head>"); 
  HTMLString += F("<body>");
  HTMLString += F("<h1>Horloge-Matricielle-LED</h1>");
  HTMLString += F("<h3>Affichage de la Température</h3>");
  HTMLString += F("<h2><br /></h2>");
  HTMLString += "<h4>Dernière Mesure: &nbsp"+LastInTempMessureDateTime+"</h4>";
  HTMLString += F("<h2><br /></h2>");
  HTMLString += "<table>"+TempValueColor+"</table>";
  HTMLString += "<a "+HumValueColor+"%</a>";
  HTMLString += F("<h2><br /></h2>");
  HTMLString += F("<h2><br /></h2>");
  HTMLString += F("<a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 48px; width: 210px;font-size: 16px;\">Retour à la Page d'Accueil</button></a>");
  HTMLString += "<a "+RefreshButton+"</a>";
  HTMLString += F("<a href =\"/dhtcalibrate\"><button class=\"button\" style=\"margin-left: 100px;height: 48px; width: 180px; color: red; font-size: 16px;\">Calibrer le Capteur</button></a>");
  HTMLString += F("</body>");
  HTMLString += F("</html>");
  SendHTMLString(HTMLString); // Envoyer une chaîne HTML 
  MyWaitLoop(100);}

// =====================================================================================================================================

String InTempMessure() {
int IntITempValue = -1;  
  if (SerialMonitor) {Serial.println(F(""));
  Serial.println(F("La Température Intérieure est Mesurée ..."));}
  float TempWert = dht.getTemperature();
  String ITempValue = String(TempWert,0);
  ITempValue.replace("-0", "0");
  ITempValue.replace("+0", "0");
  ITempValue.replace("+", "");
  ITempValue.trim(); // Supprimer les espaces
  if (isnan(TempWert) || ITempValue == "" || ITempValue == "nan" || (!IsNumeric(ITempValue))) {ITempValue = F("-");} 
  if (ITempValue == "-") {
  if (SerialMonitor) {Serial.println(F("Aucune Valeur valide reçue du Capteur !"));}
  LastInTempMessureDateTime = F("n/a / n/a");}
  else {
  IntITempValue = ITempValue.toInt();
  IntITempValue = IntITempValue + InTempOffsetInt; 
  ITempValue = String(IntITempValue);   
  if (SerialMonitor) {Serial.println("Température Intérieure: "+ITempValue+" °C");} // only Debugmodus
  LastInTempMessureDateTime = FormatedDateTimeFR(true);}
  return ITempValue;}

// ====================================================================================================================================

String InHumMessure() {
int IntIHumValue = -1;  
  if (SerialMonitor) {Serial.println(F(""));
  Serial.println(F("L'Humidité de l'Air Intérieur est Mesurée ..."));}
  float LfWert = dht.getHumidity();
  String IHumValue = String(LfWert,0); 
  IHumValue.replace("-0", "0");
  IHumValue.replace("+0", "0");
  IHumValue.replace("+", "");
  IHumValue.trim(); // Supprimer les espaces
  if (isnan(LfWert) || IHumValue == "" || IHumValue == "nan" || (!IsNumeric(IHumValue))) {IHumValue = F("-");} 
  if (IHumValue == "-") {
  if (SerialMonitor) {Serial.println(F("Aucune Valeur valide reçue du Capteur !"));}
  LastInTempMessureDateTime = F("n/a / n/a");}
  else {
  IntIHumValue = IHumValue.toInt();
  IntIHumValue = IntIHumValue + InHumOffsetInt; 
  IHumValue = String(IntIHumValue); 
  if (SerialMonitor) {Serial.println("Humidité de l'Air Intérieur: "+IHumValue+" %");} // only Debugmodus
  LastInTempMessureDateTime = FormatedDateTimeFR(true);} 
  return IHumValue;}

// ====================================================================================================================================  

String CheckInHumidity() {
// Humidité intérieure - Message si trop faible ou trop élevée
String RLF = F("");
if (InHumValue != "!") {
if (InTempOffset == "999") {InHumValue = "0";} else {InHumValue = InHumMessure();} // Récupérer la valeur d'Humidité  
MyWaitLoop(100); // attends brièvement
  if (InHumValue != "-") {
  if (InTempOffset != "999") {    
  IntInHumValue = InHumValue.toInt();  
  if (IntInHumValue <= MinComfortHum) {RLF = F("MIN");     
  // Humidité trop faible 
  StatusWarningLED("HIGH");  // Allumer la LED    
  InHumMessage = F("Attention : Humidité trop faible !");}
  if (IntInHumValue >= MaxComfortHum) {RLF = F("MAX");
  // Humidité trop élevée     
  StatusWarningLED("HIGH");  // Allumer la LED 
  if (WeatherFunctions and Display){
  if (humidity >= MaxComfortHum || humidity == -1) {InHumMessage = F("Attention : Humidité trop élevée !");} else 
  {InHumMessage = F("Attention : Humidité trop élevée ! - Veuillez aérer");}
  } else {InHumMessage = F("Attention : Humidité trop élevée !");}}
  if (RLF != "MIN" && RLF != "MAX") {RLF = F("NORMAL"); StatusWarningLED("LOW"); // Éteindre la LED
  InHumMessage = F("");}
  } else {RLF = F("NORMAL"); InHumMessage = F("Capteur DHT désactivé !"); StatusWarningLED("LOW");} // Éteindre la LED
  } else {StatusWarningLED("LOW"); // Éteindre la LED
  InHumValue = F("!");
  RLF = F("FAIL");
  InHumMessage = F("");}} else {StatusWarningLED("LOW"); RLF = F("FAIL");} // Éteindre la LED et échouer
  return RLF;  
}

// ====================================================================================================================================

void DHTSensorCalibrate() {
String HTMLString;
bool MDTime;
if (!await) {
// Lors de l'enregistrement, le bouton a été enfoncé 
if (server.method() == HTTP_POST) {
    // Décalage du capteur de température DHT
    InTempOffset = server.arg("DHTTo");
    InTempOffset.replace("+", "");
    InTempOffset.trim();
    if (InTempOffset != "") {
    if (!IsNumeric(InTempOffset)) {InTempOffset = F("!");} else {
    InTempOffsetInt = InTempOffset.toInt();}
    } else {InTempOffset = F("0");}
    // Décalage du capteur d'humidité DHT
    InHumOffset = server.arg("DHTHo");
    InHumOffset.replace("+", "");
    InHumOffset.trim(); 
    if (InHumOffset != "") {
    if (!IsNumeric(InHumOffset)) {InHumOffset = F("!");} else {
    InHumOffsetInt = InHumOffset.toInt();} 
    } else {InHumOffset = F("0");}

    // Si erreur de saisie
    if (InTempOffset == "!" || InHumOffset == "!") {
    HTMLString = F("<!doctype html><html lang='fr-CA'>");
    HTMLString += F("<head><meta charset='utf-8'>");
    HTMLString += F("<meta name='viewport' content='width=device-width, initial-scale=1'>");
    HTMLString += F("<title>Horloge-Matricielle-LED</title>");
    HTMLString += F("<style>");
    HTMLString += F("*,::after,::before{box-sizing:border-box;}body{margin:0;font-family:");
    HTMLString += F("'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;");
    HTMLString += F("font-weight:400;line-height:1.5;color:#212529;background-color:#585858;}.form-control{display:");
    HTMLString += F("block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}button{border:1px solid");
    HTMLString += F("transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;");
    HTMLString += F("line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:420px;padding:15px;margin:auto;}h1,p{text-align: center}");
    HTMLString += F("h1 { font-size: 42px; color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }");
    HTMLString += F("h5 { font-size: 18px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 0px;}"); 
    HTMLString += F("h6 { font-size: 14px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 10px;}"); 
    HTMLString += F("</style>");
    HTMLString += F("</head>");
    HTMLString += F("<body><main class='form-signin'>");
    HTMLString += F("<h1Horloge-Matricielle-LED</h1>");
    if (DHT11Sensor) {
    HTMLString += F("<h5>(Étalonnage du Capteur DHT11)</h5>");} else{
    HTMLString += F("<h5>(Étalonnage du Capteur DHT22)</h5>");}
    HTMLString += F("<br/>");
    HTMLString += F("<h6>Entrée(s) invalide(s) - Veuillez corriger !</h6>");
    HTMLString += F("<br/>");
    HTMLString += F("<a href =\"/dhtcalibrate\"><button>Dos</button></a>");
    HTMLString += F("</main>");
    HTMLString += F("</body>");
    HTMLString += F("</html>");
    SendHTMLString(HTMLString); // Envoyer une chaîne HTML
    MyWaitLoop(100);
    } else {
    if (littleFSInit){
        // *** Enregistrer les entrées dans le système de fichiers ***
    InTempOffset.trim(); // Décalage du capteur DHT (température)
    InHumOffset.trim(); // Décalage du capteur DHT (humidité)
    if (InTempOffset != ""){
    SetupSave("dht-intempoffset.txt", InTempOffset);} else {
    if (LittleFS.exists("/dht-intempoffset.txt")){LittleFS.remove("/dht-intempoffset.txt");}}
    if (InHumOffset != ""){
    SetupSave("dht-inhumoffset.txt", InHumOffset);} else {
    if (LittleFS.exists("/dht-inhumoffset.txt")){LittleFS.remove("/dht-inhumoffset.txt");}}
    MyWaitLoop(500); // 0,5 secondes. attendez
    if (SerialMonitor) {Serial.println(F("Données de décalage du Capteur DHT enregistrées..."));}
    // Afficher le Site Web 
    HTMLString = F("<!doctype html><html lang='fr-CA'>");
    HTMLString += F("<head><meta charset='utf-8'>");
    HTMLString += F("<meta name='viewport' content='width=device-width, initial-scale=1'>");
    HTMLString += F("<title>Horloge-Matricielle-LED</title>");
    HTMLString += F("<style>");
    HTMLString += F("*,::after,::before{box-sizing:border-box;}body{margin:0;font-family:");
    HTMLString += F("'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;");
    HTMLString += F("font-weight:400;line-height:1.5;color:#212529;background-color:#585858;}.form-control{display:");
    HTMLString += F("block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}button{border:1px solid");
    HTMLString += F("transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;");
    HTMLString += F("line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:520px;padding:15px;margin:auto;}h1,p{text-align: center}");
    HTMLString += F("h1 { font-size: 42px; color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }");
    HTMLString += F("h5 { font-size: 18px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 0px;}"); 
    HTMLString += F("h6 { font-size: 14px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 10px;}"); 
    HTMLString += F("</style>");
    HTMLString += F("</head>");
    HTMLString += F("<body><main class='form-signin'>");
    HTMLString += F("<h1>Horloge-Matricielle-LED</h1>");
    if (DHT11Sensor) {
    HTMLString += F("<h5>(Étalonnage du Capteur DHT11)</h5>");} else{
    HTMLString += F("<h5>(Étalonnage du Capteur DHT22)</h5>");}
    HTMLString += F("<br/>");
    HTMLString += F("<h6>Les Paramètres ont été Enregistrés</h6>");
    HTMLString += F("<br/>");
    HTMLString += F("<a href =\"/tempvalue\"><button>D'ACCORD</button></a>");
    HTMLString += F("</main>");
    HTMLString += F("</body>");
    HTMLString += F("</html>");   
    CheckInHumidity(); // Mesure de l'humidité intérieure 
    MyWaitLoop(100);
    SendHTMLString(HTMLString);} // Envoyer une chaîne HTML
    else { 
    // DHT Sensor Offset Settings were not saved
     if (SerialMonitor) {Serial.println(F("Les Paramètres de décalage du Capteur DHT n'ont pas été Enregistrés !"));}
    HTMLString = F("<!doctype html><html lang='fr-CA'>");
    HTMLString += F("<head><meta charset='utf-8'>");
    HTMLString += F("<meta name='viewport' content='width=device-width, initial-scale=1'>");
    HTMLString += F("<title>Horloge-Matricielle-LED</title>");
    HTMLString += F("<style>");
    HTMLString += F("*,::after,::before{box-sizing:border-box;}body{margin:0;font-family:");
    HTMLString += F("'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;");
    HTMLString += F("font-weight:400;line-height:1.5;color:#212529;background-color:#585858;}.form-control{display:");
    HTMLString += F("block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}button{border:1px solid");
    HTMLString += F("transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;");
    HTMLString += F("line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:420px;padding:15px;margin:auto;}h1,p{text-align: center}"); 
    HTMLString += F("h1 { font-size: 42px; color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }");   
    HTMLString += F("h5 { font-size: 18px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 0px;}"); 
    HTMLString += F("h6 { font-size: 14px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 10px;}"); 
    HTMLString += F("</style>");
    HTMLString += F("</head>");
    HTMLString += F("<body><main class='form-signin'>");
    HTMLString += F("<h1>Horloge-Matricielle-LED</h1>");
    if (DHT11Sensor) {
    HTMLString += F("<h5>(Étalonnage du Capteur DHT11)</h5>");} else{
    HTMLString += F("<h5>(Étalonnage du Capteur DHT22)</h5>");}
    HTMLString += F("<br/>");
    HTMLString += F("<h6>Erreur lors de l'enregistrement des Paramètres</h6>");
    HTMLString += F("<br/>");
    HTMLString += F("<a href =\"/tempvalue\"><button>Dos</button></a>");
    HTMLString += F("</main>");
    HTMLString += F("</body>");
    HTMLString += F("</html>"); 
    SendHTMLString(HTMLString); // Envoyer une chaîne HTML
    MyWaitLoop(100);
    digitalWrite(HWLEDPin, LOW); // LED éteinte
    }} // Terminer l'erreur d'entrée
  } else {  
    // Settings
    HTMLString =  F("<!doctype html><html lang='fr-CA'>");
    HTMLString += F("<head><meta charset='utf-8'>");
    HTMLString += F("<meta name='viewport' content='width=device-width, initial-scale=1'>");
    HTMLString += F("<title>Horloge-Matricielle-LED</title>");
    HTMLString += F("<style>");
    HTMLString += F("*,::after,::before{box-sizing:border-box;}body{margin:0;font-family:");
    HTMLString += F("'Segoe UI',Roboto,'Helvetica Neue',Arial,'Noto Sans','Liberation Sans';font-size:1rem;");
    HTMLString += F("font-weight:400;line-height:1.5;color:#212529;background-color:#585858;}.form-control{display:");
    HTMLString += F("block;width:100%;height:calc(1.5em + .75rem + 2px);border:1px solid #ced4da;}button{border:1px solid");
    HTMLString += F("transparent;color:#fff;background-color:#007bff;border-color:#007bff;padding:.5rem 1rem;font-size:1.25rem;");
    HTMLString += F("line-height:1.5;border-radius:.3rem;width:100%}.form-signin{width:100%;max-width:520px;padding:15px;margin:auto;}h1,p{text-align: center}");
    HTMLString += F("label {color: white;}");
    HTMLString += F("h1 { font-size: 42px; color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }");
    HTMLString += F("h5 { font-size: 18px; color: #f5f5f5; text-align:center; margin-top: 0px; margin-bottom: 0px;}"); 
    HTMLString += F("h6 { font-size: 12px; color: #ffcc00; text-align:center; margin-top: 15px; margin-bottom: 15px;}");
    HTMLString += F("</style>");
    HTMLString += F("</head>");
    HTMLString += F("<body><main class='form-signin'>");
    HTMLString += F("<form action='/dhtcalibrate' method='post'>");
    HTMLString += F("<h1>Horloge-Matricielle-LED</h1>");
    if (DHT11Sensor) {
    HTMLString += F("<h5>(Étalonnage du Capteur DHT11)</h5>");} else{
    HTMLString += F("<h5>(Étalonnage du Capteur DHT22)</h5>");}
    // ******************************************* Parameter ****************************************************************************************************************************************************************************
    if (DHT11Sensor) { 
    HTMLString += "<div class='form-floating'><br/><label>Décalage de Température du Capteur DHT11 (Exemple : -1 ou +1)</label><input type='text' maxlength='3' class='form-control' name='DHTTo' id='DHTTo' value='"+InTempOffset+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>Décalage d'Humidité du Capteur DHT11 (Exemple : -1 ou +1)</label><input type='text' maxlength='3' class='form-control' name='DHTHo' id='DHTHo' value='"+InHumOffset+"'></div>";} else {
    HTMLString += "<div class='form-floating'><br/><label>Décalage de Température du Capteur DHT22 (Exemple : -1 ou +1)</label><input type='text' maxlength='3' class='form-control' name='DHTTo' id='DHTTo' value='"+InTempOffset+"'></div>";
    HTMLString += "<div class='form-floating'><br/><label>Décalage d'Humidité du Capteur DHT22 (Exemple : -1 ou +1)</label><input type='text' maxlength='3' class='form-control' name='DHTHo' id='DHTHo' value='"+InHumOffset+"'></div>";}
    // ******************************************* Parameter ****************************************************************************************************************************************************************************
    HTMLString += F("<h6>*) - 999 à Température désactive le Capteur DHT (pas d'affichage dans la Matrice LED)</h6>");
    HTMLString += F("<button type='submit'>Sauvegarder</button>");
    HTMLString += F("<br/><br/>");
    HTMLString += F("</form>");
    HTMLString += F("<a href =\"/tempvalue\"><button>Annuler</button></a>");
    HTMLString += F("<br/><br/>");
    HTMLString += F("</main>");
    HTMLString += F("</body>");
    HTMLString += F("</html>");  
    SendHTMLString(HTMLString);
    MyWaitLoop(100);}} else {PleaseWait();}} // Envoyer une chaîne HTML or wait  

// ====================================================================================================================================