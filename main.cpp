#include <Arduino.h>
#include <WiFi.h>
#include <RTClib.h>
#include <PubSubClient.h>
#include <ESP32_MailClient.h>

//**************************************
//*********** MQTT CONFIG **************
//**************************************
const char *mqtt_server = "ioticos.org";
const int mqtt_port = 1883;
const char *mqtt_user = "eJ83QiwCgg42aRV";
const char *mqtt_pass = "qWBrDKXf9gKOOvl";
const char *root_topic_subscribe = "5J4eMVxixNzCPKu/input";
const char *root_topic_publish = "5J4eMVxixNzCPKu/output";
int LED_BUILTIN = 2;
RTC_DS3231 RTC;

//**************************************
//*********** WIFICONFIG ***************
//**************************************
const char* ssid = "ARRIS-20A2";
const char* password =  "JPNL9CRJEPXE9VCU";

WiFiServer server(80);

//------------------------CODIGO HTML------------------------------
String pagina = "<!DOCTYPE HTML>"
"<html>"
"<head>  "
"<center>"
"<h1>Servidor Web ESP32</h1>"
"<p><a href='/on'><button style='height:50px;width:100px'>CERRAR</button></a></p>"
"<p><a href='/off'><button style='height:50px;width:100px'>ABRIR</button></a></p>"
"</center>"
"<script>"
"window.onload = function () {"
"var chart = new CanvasJS.Chart('chartContainer', {"
"	theme: 'light2',"
"	animationEnabled: false,	"
"	title:{"
"		text: 'Actividad de Cerradura Electronica 2020'"
"	},"
"	data: ["
"	{"
"		type: 'column',"
"		dataPoints: ["
"			{ label: 'enero',  y: 69  },"
"			{ label: 'febrero',  y: 65  },"
"			{ label: 'marzo',  y: 51  },"
"			{ label: 'abril',  y: 52  },"
"			{ label: 'mayo',  y: 48  },"
"			{ label: 'junio',  y: 49  },"
"			{ label: 'julio',  y: 43  },"
"			{ label: 'agosto',  y: 45  },"
"			{ label: 'septiembre', y: 55  },"
"			{ label: 'octubre', y: 69  },"
"			{ label: 'noviembre',  y: 74  },"
"			{ label: 'diciembre',  y: 76  }"
"		]"
"	}"
"	]"
"});"
"chart.render();"
"var chart = new CanvasJS.Chart('chartContainer2', {"
"theme: 'light2',"
"	animationEnabled: false,"
"	title:{"
"		text: 'Temperatura de ESP32 Noviembre 2020'"
"	},"
"	data: ["
"	{"
"		type: 'area',"
"		dataPoints: ["
"			{ label: '01',  y: 28  },"
"			{ label: '02',  y: 32  },"
"			{ label: '03',  y: 29  },"
"			{ label: '04',  y: 30  },"
"			{ label: '05',  y: 35  },"
"			{ label: '06',  y: 36  },"
"			{ label: '07',  y: 37  },"
"			{ label: '08',  y: 39  },"
"			{ label: '09', y: 40  },"
"			{ label: '10', y: 39  },"
"			{ label: '11',  y: 38  },"
"			{ label: '12',  y: 35  },"
"			{ label: '13',  y: 34  },"
"			{ label: '14',  y: 34  },"
"			{ label: '15',  y: 34  },"
"			{ label: '16',  y: 32  },"
"			{ label: '17',  y: 33  },"
"			{ label: '18',  y: 32  },"
"			{ label: '19',  y: 31  },"
"			{ label: '20',  y: 31  },"
"			{ label: '21',  y: 28  },"
"			{ label: '22',  y: 27  },"
"			{ label: '23',  y: 28  },"
"			{ label: '24',  y: 25  },"
"			{ label: '25',  y: 25  },"
"			{ label: '26',  y: 26  },"
"			{ label: '27',  y: 24  },"
"			{ label: '28',  y: 25  },"
"			{ label: '29',  y: 25  },"
"			{ label: '30',  y: 24  }"
"		]"
"	}"
"	]"
"});  "
"chart.render();"
"var chart = new CanvasJS.Chart('chartContainer3', {"
"animationEnabled: true,"
"	exportEnabled: true,"
"	title:{"
"		text: 'Voltaje Cerradura - Noviembre 2020'"
"	},"
"	axisY: {"
"		title: 'Voltaje (V)',"
"		suffix: ' V'"
"	},"
"	axisX: {"
"		valueFormatString: 'DD MMMM'"
"	},"
"	data: ["
"	{"
"		type: 'rangeArea',"
"		xValueFormatString: 'DD MMMM', "
"		yValueFormatString: '#0.## °C',"
"		toolTipContent: ' <span style=\"color:#4F81BC\">{x}</span><br><b>Min:</b> {y[0]}<br><b>Max:</b> {y[1]}',"
"		dataPoints: ["
"			{ x: new Date(2020,10,01), y:[12, 12] },"
"			{ x: new Date(2020,10,02), y:[12, 12] },"
"			{ x: new Date(2020,10,03), y:[11.4, 12] },"
"			{ x: new Date(2020,10,04), y:[11.5, 12] },"
"			{ x: new Date(2020,10,05), y:[11.8, 12] },"
"			{ x: new Date(2020,10,06), y:[11.6, 12] },"
"			{ x: new Date(2020,10,07), y:[11.9, 12] },"
"			{ x: new Date(2020,10,08), y:[12, 12] },"
"			{ x: new Date(2020,10,09), y:[12, 12] },"
"			{ x: new Date(2020,10,10), y:[12, 12] },"
"			{ x: new Date(2020,10,11), y:[12, 12] },"
"			{ x: new Date(2020,10,12), y:[12, 12] },"
"			{ x: new Date(2020,10,13), y:[12, 12] },"
"			{ x: new Date(2020,10,14), y:[11.5, 12] },"
"			{ x: new Date(2020,10,15), y:[11, 12] },"
"			{ x: new Date(2020,10,16), y:[11.4, 12] },"
"			{ x: new Date(2020,10,17), y:[11.6, 12] },"
"			{ x: new Date(2020,10,18), y:[11.8, 12] },"
"			{ x: new Date(2020,10,19), y:[11, 12] },"
"			{ x: new Date(2020,10,20), y:[11, 12] },"
"			{ x: new Date(2020,10,21), y:[11.1, 12] },"
"			{ x: new Date(2020,10,22), y:[12, 12] },"
"			{ x: new Date(2020,10,23), y:[12, 12] },"
"			{ x: new Date(2020,10,24), y:[12, 12] },"
"			{ x: new Date(2020,10,25), y:[12, 12] },"
"			{ x: new Date(2020,10,26), y:[12, 12] },"
"			{ x: new Date(2020,10,27), y:[12, 12] },"
"			{ x: new Date(2020,10,28), y:[12, 12] },"
"			{ x: new Date(2020,10,29), y:[12, 12] },"
"			{ x: new Date(2020,10,30), y:[12, 12] }"
"		]"
"	}]"
"});"
"chart.render();"
"}"
"</script>"
"</head>"
"<body>"
"<div id='chartContainer' style='height: 370px; width: 100%;'></div>"
"<div id='chartContainer2' style='height: 370px; width: 100%;'></div>"
"<div id='chartContainer3' style='height: 300px; width: 100%;'></div>"
"<script src='https://canvasjs.com/assets/script/canvasjs.min.js'> </script>"
"</body>"
"</html>"
;



//**************************************
//*********** GLOBALES   ***************
//**************************************
WiFiClient espClient;
PubSubClient client(espClient);
char msg[25];
long count=0;
SMTPData datosSMTP;
String incoming = "";
String mensaje = "";

int contconexion = 0;
String header; // Variable para guardar el HTTP request
String estadoSalida = "off";
const int salida = 2;
int rele2 = 13;
int rele3 = 14;

//************************
//** F U N C I O N E S ***
//************************
void callback(char* topic, byte* payload, unsigned int length);
void reconnect();
void setup_wifi();
void correoAbierta();
void enviarMensaje();

String apikye = "o.TgfyzNc8EwhqXlGa2M6dNOHeikleyZZq";
void enviarMensaje(String title,String mensjj){
	WiFiClientSecure clienton;
	if(!clienton.connect("api.pushbullet.com",443)){
		Serial.print("No se pudo conectar con pushbullet");
	}
	String urrrl = "/v2/pushes";
	String mmmsg = "{\"type\":\"note\",\"title\":\""+title+"\",\"body\":\""+mensjj+"\"}\r\n";
	Serial.print(urrrl);
	clienton.print(String("POST ")+urrrl+" HTTP/1.1\r\n"+
	"Host: "+"api.pushbullet.com"+"\r\n"+
	"Authorization: Bearer "+apikye+"\r\n"+
	"Content-Type: application/json\r\n" +
	"Content-Length: "+String(mmmsg.length()) + "\r\n\r\n");
	clienton.print(mmmsg);
	delay(2000);
	while(clienton.available()==0);
	
	while(clienton.available()){
		String line = clienton.readStringUntil('\n');
		Serial.println(line);
	}
		
}

void correoAbierta(){
	datosSMTP.setLogin("smtp.gmail.com", 465, "esp32devmailsender@gmail.com", "JesusAntonioSilva15");
	datosSMTP.setSender("ESP32S", "esp32devmailsender@gmail.com");
	datosSMTP.setPriority("High");
	datosSMTP.setSubject("ESP32 HOGAR OPENED");
	datosSMTP.setMessage("Hola soy el esp32 y me estoy comunicando contigo, la puerta fue abierta.", false);
	datosSMTP.addRecipient("silva.espinoza.jesus@gmail.com");
	if (!MailClient.sendMail(datosSMTP))
	Serial.println("Error enviando el correo, " + MailClient.smtpErrorReason());
	datosSMTP.empty();
}

void setup() {
  pinMode(LED_BUILTIN,OUTPUT);
  pinMode(rele2,OUTPUT);
  pinMode(rele3,OUTPUT);
  digitalWrite(rele2, LOW);
  digitalWrite(rele3, LOW);
  
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  server.begin(); // iniciamos el servidor
  
  

}

void loop() {
  WiFiClient cliente = server.available();
  if (cliente) {                             // Si se conecta un nuevo cliente
    Serial.println("New Client.");          // 
    String currentLine = "";                //
    while (cliente.connected()) {            // loop mientras el cliente está conectado
      if (cliente.available()) {             // si hay bytes para leer desde el cliente
        char c = cliente.read();             // lee un byte
        Serial.write(c);                    // imprime ese byte en el monitor serial
        header += c;
        if (c == '\n') {                    // si el byte es un caracter de salto de linea
          // si la nueva linea está en blanco significa que es el fin del 
          // HTTP request del cliente, entonces respondemos:
          if (currentLine.length() == 0) {
            cliente.println("HTTP/1.1 200 OK");
            cliente.println("Content-type:text/html");
            cliente.println("Connection: close");
            cliente.println();
            
            // enciende y apaga el GPIO por medio DEL SERVIDOR WEB ESP32
            if (header.indexOf("GET /on") >= 0) { //ON
              Serial.println("GPIO on");
              estadoSalida = "on";
              digitalWrite(rele2, HIGH);//mandar señales
			  digitalWrite(rele3, HIGH);
            } else if (header.indexOf("GET /off") >= 0) { //OFF
              Serial.println("GPIO off");
              estadoSalida = "off";
              digitalWrite(rele2, LOW);//mandar señal apagado
			  digitalWrite(rele3, LOW);
            }
            
            // Muestra la página web
            cliente.println(pagina);
            
            // la respuesta HTTP temina con una linea en blanco
            cliente.println();
            break;
          } else { // si tenemos una nueva linea limpiamos currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // si C es distinto al caracter de retorno de carro
          currentLine += c;      // lo agrega al final de currentLine
        }
      }
    }
    // Limpiamos la variable header
    header = "";
    // Cerramos la conexión
    cliente.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }




  if (!client.connected()) {
		reconnect();
	}

  if (client.connected()){
    
	//String str = "La cuenta es -> " + String(count);
    //str.toCharArray(msg,25);
    //client.publish(root_topic_publish,msg);
    //count++;

	


	if (incoming.equalsIgnoreCase("0")) //MQTT RECIBIENDO MENSAJE
	{
		digitalWrite(rele2, HIGH); //SI EL MENSAJE ES 0 Cerrada LA CHAPA
	 	digitalWrite(rele3, HIGH);
		
		mensaje = "Puerta Cerrada";
		mensaje.toCharArray(msg,25);
		client.publish(root_topic_publish,msg);
	}
	else if (incoming.equalsIgnoreCase("1"))
	{
		digitalWrite(rele2, LOW); //SI EL MENSAJE ES 1 Abierta LA CHAPA
		digitalWrite(rele3, LOW);
		correoAbierta();
		enviarMensaje("NotificacionPuerta","Puerta abierta");
		mensaje = "Puerta Abierta";
		mensaje.toCharArray(msg,25);
		client.publish(root_topic_publish,msg);
	}
	incoming = "";
    delay(1000);
  }
  client.loop();
}




//*****************************
//***    CONEXION WIFI      ***
//*****************************
void setup_wifi(){
	delay(10);
	// Nos conectamos a nuestra red Wifi
	Serial.println();
	Serial.print("Conectando a ssid: ");
	Serial.println(ssid);

	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

	Serial.println("");
	Serial.println("Conectado a red WiFi!");
	Serial.println("Dirección IP: ");
	Serial.println(WiFi.localIP());
}



//*****************************
//***    CONEXION MQTT      ***
//*****************************

void reconnect() {

	while (!client.connected()) {
		Serial.print("Intentando conexión Mqtt...");
		// Creamos un cliente ID
		String clientId = "IOTICOS_H_W_";
		clientId += String(random(0xffff), HEX);
		// Intentamos conectar
		if (client.connect(clientId.c_str(),mqtt_user,mqtt_pass)) {
			Serial.println("Conectado!");
			// Nos suscribimos
			if(client.subscribe(root_topic_subscribe)){
        Serial.println("Suscripcion ok");
      }else{
        Serial.println("fallo Suscripciión");
      }
		} else {
			Serial.print("falló :( con error -> ");
			Serial.print(client.state());
			Serial.println(" Intentamos de nuevo en 5 segundos");
			delay(5000);
		}
	}
}


//*****************************
//***       CALLBACK        ***
//*****************************

void callback(char* topic, byte* payload, unsigned int length){
	incoming = "";
	Serial.print("Mensaje recibido desde -> ");
	Serial.print(topic);
	Serial.println("");
	for (int i = 0; i < length; i++) {
		incoming += (char)payload[i];
	}
	incoming.trim();
	Serial.println("Mensaje -> " + incoming);
		
}