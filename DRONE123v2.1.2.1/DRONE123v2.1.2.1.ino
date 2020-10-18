
#include <SoftwareSerial.h>
#include <Adafruit_GPS.h>


//
//String apn = "safaricom";                               //APN
//String apn_u = "saf";                                   //APN-Username
//String apn_p = "data";                                  //APN-Password
//String url = "http://utm.karimu.co.ke/iot-save.php";    //URL of Server
String post_string = "";
//char post_string[] = "";


float longit = 36.79291;

SoftwareSerial mySerial(13, 12); //ACTUAL (GPS)
SoftwareSerial SWserial(11, 10); // RX, TX (GSM)

Adafruit_GPS GPS(&mySerial);  

void setup()
{
  Serial.begin(115200);
  Serial.println(F("GSM GPS Test"));

  while (!Serial) { // wait till Serial
  }
  
  SWserial.begin(9600);
  GPS.begin(9600);
  
//  Serial.println("Waiting for 15 Seconds....");
  delay(15000);

  SWserial.listen();
//  Serial.println("Configuringn GSM GPRS....");
  while (SWserial.available()) {
    Serial.write(SWserial.read());
  }
  delay(2000);
  gsm_config_gprs();
//  Serial.println("Done Configuringn GSM GPRS.");

//  Serial.println("Configureing the GPS Module...");
  configure_gps_module();
//  Serial.println("Done configureing the GPS Module");
  
}

void loop() {
  update_gps_data();

  gsm_http_post("api_token=GM61w8fLm3N7dXF8MIEcsm9BFe6hv4g484Nydh5ARVINo5H4BRBwCwxHmpEycAW&latitude=-1.284506&longitude=69.691545&altitude=30.25&velocity=0.01&battery=10&end=0");
//  post_string = "";
//  gsm_http_post(post_string);
  delay(500);
}

void configure_gps_module(){

  mySerial.listen();
  
  Serial.println(F("confPS"));

  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);  //set output to only RMC and GGA sentences
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
  GPS.sendCommand(PGCMD_ANTENNA);  // Request updates on antenna status

  delay(1000);
  // Ask for firmware version
  mySerial.println(PMTK_Q_RELEASE);
  
  }

void update_gps_data(){

  mySerial.listen();
  
//    Serial.print("\nTime: ");
//    if (GPS.hour < 10) { Serial.print('0'); }
//    Serial.print(GPS.hour, DEC); Serial.print(':');
//    if (GPS.minute < 10) { Serial.print('0'); }
//    Serial.print(GPS.minute, DEC); Serial.print(':');
//    if (GPS.seconds < 10) { Serial.print('0'); }
//    Serial.print(GPS.seconds, DEC); Serial.print('.');
//    if (GPS.milliseconds < 10) {
//      Serial.print("00");
//    } else if (GPS.milliseconds > 9 && GPS.milliseconds < 100) {
//      Serial.print("0");
//    }
//    
//    Serial.println(GPS.milliseconds);
//    Serial.print("Date: ");
//    Serial.print(GPS.day, DEC); Serial.print('/');
//    Serial.print(GPS.month, DEC); Serial.print("/20");
//    Serial.println(GPS.year, DEC);
//    Serial.print("Fix: "); Serial.print((int)GPS.fix);
//    Serial.print(" quality: "); Serial.println((int)GPS.fixquality);
    Serial.println("gtPS");
    if (GPS.fix) {
      Serial.print("Location (in degrees, works with Google Maps): ");
      Serial.print(GPS.latitudeDegrees, 6);
      Serial.print(", ");
      Serial.println(GPS.longitudeDegrees, 6);

      Serial.print("Speed (knots): "); Serial.println(GPS.speed);
      Serial.print("Angle: "); Serial.println(GPS.angle);
      Serial.print("Altitude: "); Serial.println(GPS.altitude);
      Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
    }
  
  }

void gsm_http_post( String postdata) {
  
  Serial.println(F("http1"));
  gsm_send_serial(F("AT+SAPBR=1,1"));
  gsm_send_serial(F("AT+SAPBR=2,1"));
  gsm_send_serial(F("AT+HTTPINIT"));
  gsm_send_serial(F("AT+HTTPPARA=CID,1"));
  gsm_send_serial(F("AT+HTTPPARA=URL,http://utm.karimu.co.ke/iot-save.php"));
  gsm_send_serial(F("AT+HTTPPARA=CONTENT,application/x-www-form-urlencoded"));
  gsm_send_serial(F("AT+HTTPDATA=192,5000"));
  gsm_send_serial(String(postdata));
  gsm_send_serial(F("AT+HTTPACTION=1"));
  gsm_send_serial(F("AT+HTTPREAD"));
  gsm_send_serial(F("AT+HTTPTERM"));
  gsm_send_serial(F("AT+SAPBR=0,1"));
  Serial.println(F("http0"));
}

/*
  This function is fro setting up the GPRS for GSM ops
*/
void gsm_config_gprs() {

  Serial.println(F("cnfPRS"));
  gsm_send_serial(F("AT+SAPBR=3,1,Contype,GPRS"));
  gsm_send_serial(F("AT+SAPBR=3,1,APN,safaricom"));
  gsm_send_serial(F("AT+SAPBR=3,1,USER,saf"));
  gsm_send_serial(F("AT+SAPBR=3,1,PWD,data"));
  Serial.println(F("PRSok"));
}

/*
 * This Function Simplifies sending of packet data to the GSM module via serial.
 * It has one arg=>command that is the (AT) command to bes sent to the module
 */
void gsm_send_serial(String command) {

  SWserial.listen();
  Serial.println("Send ->: " + command);
  //Serial.println(F("Data for gsm:"));
  SWserial.println(command);
  long wtimer = millis();
  while (wtimer + 3000 > millis()) {
    while (SWserial.available()) {
      Serial.write(SWserial.read());
    }
  }
  Serial.println();
}
