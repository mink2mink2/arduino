#include <ESP8266WiFi.h> 
#include <Servo.h>

Servo ObjServo; 
static const int ServoGPIO = D5; 

const char* ssid = "SSID_NAME"; 
const char* password = "SSID_PW"; 

WiFiServer server(80);
String header;
String valueString = String(0);
int positon1 = 0;
int positon2 = 0;

void setup() 
{
Serial.begin(115200); 
ObjServo.attach(ServoGPIO); 
Serial.print("Making connection to "); 
Serial.println(ssid);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.println("WiFi connected.");
Serial.println("IP address: ");
Serial.println(WiFi.localIP());
server.begin(); 
}

void loop(){
WiFiClient client = server.available(); 
if (client)
{ 

String header = client.readStringUntil('\r');
client.println("HTTP/1.1 200 OK");
client.println("Content-type:text/html");
client.println("Connection: close");
client.println();

client.println("<!DOCTYPE html><html>");
client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
client.println("<title>Servo Angle Controller</title>");
client.println("<link rel=\"icon\" href=\"data:,\">");
client.println("<style>body { text-align: center; font-family: \"Trebuchet MS\", Arial; margin-left:auto; margin-right:auto;}");
client.println(".headertext{ font-weight:bold; font-family:Arial ; text-align: center; color: brown ;}");
client.println(".slider { width: 500px; }</style>");
client.println("<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js\"></script>");

client.println("</head><body><h1><u><div class = \"headertext\"> Web Server based Servo Angle Controller</u></h1>");
client.println("<p><h3>Angle</h3><span id=\"servoPos\"></span></p>"); 
client.println("<input type=\"range\" min=\"0\" max=\"180\" class=\"slider\" id=\"servoSlider\" onchange=\"servo(this.value)\" value=\""+valueString+"\"/>");

client.println("<script>var slider = document.getElementById(\"servoSlider\");");
client.println("var servoP = document.getElementById(\"servoPos\"); servoP.innerHTML = slider.value;");
client.println("slider.oninput = function() { slider.value = this.value; servoP.innerHTML = this.value; }");
client.println("$.ajaxSetup({timeout:1000}); function servo(pos) { ");
client.println("$.get(\"/?value=\" + pos + \"&\"); {Connection: close};}</script>");
client.println("</body></html>"); 

if(header.indexOf("GET /?value=")>=0) 
{
positon1 = header.indexOf('=');
positon2 = header.indexOf('&');
valueString = header.substring(positon1+1, positon2);

ObjServo.write(valueString.toInt());
Serial.println(valueString); 
} 

header = "";
client.stop();
Serial.println("");
}
}
