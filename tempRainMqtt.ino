#include <OneWire.h>
#include <ESP8266WiFi.h> 
#include <PubSubClient.h> 
#include <DallasTemperature.h>

<<<<<<< HEAD
//cambio
=======
//estamos en la rama ota 
//hago otra modificacion a la rama ota
>>>>>>> ota

#define oneWirePin 2  // PIN DE SENSOR DS18B20..TEMPERATURA 
#define pinLluvia A0  // PIN DE SENSOR ANALOGICO DE LLUVIA
#define vcc 12        // PIN SUMINISTRA ENERGIA SENSOR LLUVIA

OneWire oneWireBus(oneWirePin);
DallasTemperature sensor(&oneWireBus);

// CONFIGURA LAS VARIABLES NOMBRE DE LA RED Y PASS
const char* ssid = "MOVISTAR_74F8";
const char* wifi_password = "Rx44744YNPAAPYExAMAA";

// TIEMPO QUE ESTARA EN SLEEP EN SEGUNDOS
const int sleepSeconds = 1800;

// CONFIGURA LOS PARAMETROS DE MQT
const char* mqtt_server = "192.168.1.20";
const char* mqtt_topic = "temperatura";
const char* mqtt_username = "ruben";
const char* mqtt_password = "rub";
const char* clientID = "esp8266 con sensor de temperatura";


// INICIA EL CLIENTE WIFI Y EL SERVIDOR MQTT
WiFiClient wifiClient;
PubSubClient client(mqtt_server, 1883, wifiClient);

void setup() {

	// HACE QUE SE PUEDA RESETEAR CUANDO FINALIZA EL TEMPORIZADOR DE SLEEP
	pinMode(0, WAKEUP_PULLUP);
	Serial.begin(115200);
	pinMode(vcc, OUTPUT);
	sensor.begin();
	WiFi.begin(ssid, wifi_password);

	// MIENTRAS NO ESTÉ CONECTADO A WIFI, ESPERA HASTA CONECTARSE
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
	}
	// CONECTA MQTT E INFORMA DEL ESTADO DE LA CONEXION
	if (client.connect(clientID, mqtt_username, mqtt_password)) {
		Serial.println("Connected to MQTT Broker!");
	}
	else {
		Serial.println("Connection to MQTT Broker failed...");
	}
}
void loop() {
	digitalWrite(vcc, HIGH);
	delay(100);


	sensor.requestTemperatures();
	float temp = sensor.getTempCByIndex(0);
	char tempF[6];
	char * temperatura = dtostrf(temp, 5, 1, tempF);
	client.publish(mqtt_topic, temperatura);

	delay(1000);

	float lluvia = analogRead(pinLluvia);
	char llueveF[8];
	char * gotas = dtostrf(lluvia, 8, 0, llueveF);
	client.publish(mqtt_topic, gotas);


	digitalWrite(vcc, LOW);
	delay(100);
	ESP.deepSleep(sleepSeconds * 1000000); // primer numero segundos
}
