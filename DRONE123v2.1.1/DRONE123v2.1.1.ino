// Working Vertion

#include <SoftwareSerial.h>

String apn = "safaricom";                               //APN
String apn_u = "saf";                                   //APN-Username
String apn_p = "data";                                  //APN-Password
String url = "http://utm.karimu.co.ke/iot-save.php";    //URL of Server
String post_string = "";

SoftwareSerial SWserial(11, 10); // RX, TX (GSM)

void setup()
{
  Serial.begin(115200);
  Serial.println("SIM800 AT COMMANDS Test");
  SWserial.begin(9600);
  Serial.println("Waiting for 15 Seconds....");
  delay(15000);
  while (SWserial.available()) {
    Serial.write(SWserial.read());
  }
  delay(2000);
  gsm_config_gprs();
  Serial.println("Done Configuringn GSM GPRS.");
}

void loop() {
  post_string = "api_token=GM61w8fLm3N7dXF8MIEcsm9BFe6hv4g484Nydh5ARVINo5H4BRBwCwxHmpEycAW&latitude=-1.284506&longitude=36.79291&altitude=30.25&velocity=0.01&battery=10&end=0";
  gsm_http_post(post_string);
  delay(500);
}

void gsm_http_post( String postdata) {
  Serial.println(" --- Start GPRS & HTTP --- ");
  gsm_send_serial("AT+SAPBR=1,1");
  gsm_send_serial("AT+SAPBR=2,1");
  gsm_send_serial("AT+HTTPINIT");
  gsm_send_serial("AT+HTTPPARA=CID,1");
  gsm_send_serial("AT+HTTPPARA=URL," + url);
  gsm_send_serial("AT+HTTPPARA=CONTENT,application/x-www-form-urlencoded");
  gsm_send_serial("AT+HTTPDATA=192,5000");
  gsm_send_serial(postdata);
  gsm_send_serial("AT+HTTPACTION=1");
  gsm_send_serial("AT+HTTPREAD");
  gsm_send_serial("AT+HTTPTERM");
  gsm_send_serial("AT+SAPBR=0,1");
  Serial.println("-------- Done Sending HTTP ------");
}

void gsm_config_gprs() {
  Serial.println(" --- CONFIGURING GPRS --- ");
  gsm_send_serial("AT+SAPBR=3,1,Contype,GPRS");
  gsm_send_serial("AT+SAPBR=3,1,APN," + apn);
  if (apn_u != "") {
    gsm_send_serial("AT+SAPBR=3,1,USER," + apn_u);
  }
  if (apn_p != "") {
    gsm_send_serial("AT+SAPBR=3,1,PWD," + apn_p);
  }
}

void gsm_send_serial(String command) {
  Serial.println("Send ->: " + command);
  SWserial.println(command);
  long wtimer = millis();
  while (wtimer + 3000 > millis()) {
    while (SWserial.available()) {
      Serial.write(SWserial.read());
    }
  }
  Serial.println();
}
