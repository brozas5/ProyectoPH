#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <UrlEncode.h>
#include <PubSubClient.h>

const char* ssid1 = "minote10Lite";
const char* password1 = "1234567890";
const char *ssid = "MiNote10Lite";
const char *password = "1234567890";
const char* mqtt_server = "68.183.119.177"; // Este es el address en mqtt dash
const char* keyDevice = "";
WiFiClient espClient;
PubSubClient client(espClient);
float calibration_value = 21.34;
int phval = 0;
unsigned long int avgval;
int buffer_arr[10], temp;

void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("OK");
  }
void reconnect() {
  if (!client.connected()) {

    if (client.connect(keyDevice,"","")){//Servidor broker es el usuario y prueba12 la contraseña en mqtt dash
      Serial.println("connected");
      client.subscribe("ph/#"); // Topico al momento de crear el boton
    } else {
      delay(5000);
    }
  }
}
// +international_country_code + phone number
// Portugal +351, example: +351912345678
String phoneNumber = "+56950993478";
String apiKey = "6748743";

void sendMessage(String message){

  // Data to send with HTTP POST
  String url = "http://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" + urlEncode(message);
  WiFiClient client; 
  HTTPClient http;
  http.begin(client, url);

  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  
  // Send HTTP POST request
  int httpResponseCode = http.POST(url);
  if (httpResponseCode == 200){
    Serial.print("Message sent successfully");
  }
  else{
    Serial.println("Error sending the message");
    Serial.print("HTTP response code: ");
    Serial.println(httpResponseCode);
  }
    // Free resources
       http.end();
 }
#define pin_analogo 0  // usamos la primera entrada análoga
int analoga;
float m = 5.95833;
float n = 1.005;
float ph;
float voltaje;

void setup() {
Serial.begin(115200);
Serial.begin(9600);
  Serial.begin(9600);
  Serial.println("iniciando");
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  //client.setCallback(callback); 
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  // put your setup code here, to run once:
Serial.begin(9600);
}
Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  
}

void loop() {
if (!client.connected())
  {
    reconnect();
  }
  client.loop();
  for (int i = 0; i < 10; i++)
  {
    buffer_arr[i] = analogRead(A0);
    delay(30);
  }
for (int i = 0; i < 9; i++)
  {
    for (int j = i + 1; j < 10; j++)
    {
      if (buffer_arr[i] > buffer_arr[j])
      {
        temp = buffer_arr[i];
        buffer_arr[i] = buffer_arr[j];
        buffer_arr[j] = temp;
      }
    }
    // put your main code here, to run repeatedly:
analoga = 1023 - analogRead(pin_analogo);
voltaje = ((float)analoga / 1024.0) * 5.0;
ph = (voltaje * m) - n;

Serial.print("PH: ");
Serial.println(ph);

//Serial.print("V: ");
//Serial.println(voltaje);
client.publish("ph/01",String(ph).c_str()); 

delay(1000);
String message;
if(ph <= 7)  {
  // Send Message to WhatsAPP
 message.concat(ph);
 message.concat("¡Alerta! ");
 sendMessage(message);
}
else{
message.concat(" PH normal ");
message.concat(ph);
}
}}
