#include <SoftwareSerial.h>
#include <Adafruit_GPS.h>

SoftwareSerial GSM_Serial(11, 10); //SIM800L Tx & Rx

SoftwareSerial mySerial(13, 12);
Adafruit_GPS GPS(&mySerial);

// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO  true


//gsm variable
String apn = "safaricom";                       //APN
String apn_u = "saf";                           //APN-Username
String apn_p = "data";                          //APN-Password
String url = "http://utm.karimu.co.ke/iot-save.php";  //URL for HTTP-POST-REQUEST
String data1;   //String for the first Paramter (e.g. Sensor1)
String data2;   //String for the second Paramter (e.g. Sensor2)
//end

//VARIABLE
String Latitude;
String Longitude;
String Altitude;
String Velocity;
String End;
String Elevation;
String vIN;

void setup()
{
  Serial.begin(115200);
  /*------------------------GSM--------------------------------------------------------------
    //  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
    //  Serial.begin(115200);
    //
    //  //Begin serial communication with Arduino and SIM800L
    //  GSM_Serial.begin(9600);
    //
    //  Serial.println("Initializing...");
    //  delay(1000);
    //
    //  GSM_Serial.println("AT"); //Once the handshake test is successful, i t will back to OK
    //  updateSerial();
    //
    //  GSM_Serial.println("ATD+ +254722534687;"); //  change ZZ with country code and xxxxxxxxxxx with phone number to dial
    //  updateSerial();
    //  delay(20000); // wait for 20 seconds...
    //  GSM_Serial.println("ATH"); //hang up
    //  updateSerial();
    ------------------------GSM END--------------------------------------------------------------*/

  /*------------------------GPS--------------------------------------------------------------*/

  delay(5000);
  Serial.println("Adafruit GPS library basic test!");

  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);

  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // uncomment this line to turn on only the "minimum recommended" data
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time

  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz

  // Request updates on antenna status, comment out to keep quiet
  GPS.sendCommand(PGCMD_ANTENNA);

  delay(1000);
  // Ask for firmware version
  mySerial.println(PMTK_Q_RELEASE);

  /*------------------------GPS END--------------------------------------------------------------*/

}


uint32_t timer = millis(); //gps timmer


void loop()
{
  /*------------------------GSM--------------------------------------------------------------*/

  GSM_Serial.begin(9600);
  End = "0";
  GSM_Serial.println("AT");
  updateSerial();//Print GSM Status an the Serial Output;
  delay(100);
  GSM_Serial.println("AT+SAPBR=3,1,Contype,GPRS");
  updateSerial();
  delay(100);
  GSM_Serial.println("AT+SAPBR=3,1,APN," + apn);
  updateSerial();
  delay(100);
  GSM_Serial.println("AT+SAPBR =1,1");
  updateSerial();
  delay(500);
  GSM_Serial.println("AT+SAPBR=2,1");
  updateSerial();
  delay(500);
  GSM_Serial.println("AT+HTTPINIT");
  updateSerial();
  delay(500);
  GSM_Serial.println("AT+HTTPPARA=CID,1");
  updateSerial();
  delay(100);
  GSM_Serial.println("AT+HTTPPARA=URL," + url);
  updateSerial();
  delay(100);
  GSM_Serial.println("AT+HTTPPARA=CONTENT,application/x-www-form-urlencoded");
  updateSerial();
  delay(100);
  GSM_Serial.println("AT+HTTPDATA=192,10000");
  updateSerial();
  delay(100);
  //GSM_Serial.print("api_token=nCEdlyIEl95E2DkignU2XCLSSkzaPqphfWmeydcA6qiJvRxFcAwEEAD1izAG2fQ&longitude=" + Longitude + "&latitude=" + Latitude + "&altitude=" + Altitude + "&battery=" + vIN + "&end=" + End);
  //mySerial.print("api_token=vI89s84tfxMkjarIKy4mRmZNOwxF8F7pLST5QQxZkSSyZihtCdaWv2y5uIF6&longitude=" + Longitude + "&latitude=" + Latitude + "&altitude=" + Altitude + "&velocity=" + Velocity  + "&battery=" + vIN + "&End=" + End);
  GSM_Serial.println("api_token=GM61w8fLm3N7dXF8MIEcsm9BFe6hv4g484Nydh5ARVINo5H4BRBwCwxHmpEycAW");
  updateSerial();
  delay(9000);
  GSM_Serial.println("AT+HTTPACTION=1");
  updateSerial();
  delay(5000);
  GSM_Serial.end();

  /*------------------------GSM END--------------------------------------------------------------*/

  /*------------------------GPS--------------------------------------------------------------
    char c = GPS.read();
    // if you want to debug, this is a good time to do it!
    if ((c) && (GPSECHO))
    Serial.write(c);

    // if a sentence is received, we can check the checksum, parse it...
    if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
    //Serial.println(GPS.lastNMEA());   // this also sets the newNMEAreceived() flag to false

    if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
      return;  // we can fail to parse a sentence in which case we should just wait for another
    }

    // approximately every 2 seconds or so, print out the current stats
    if (millis() - timer > 2000) {
    timer = millis(); // reset the timer

    Serial.print("\nTime: ");
    if (GPS.hour < 10) {
      Serial.print('0');
    }
    Serial.print(GPS.hour, DEC); Serial.print(':');
    if (GPS.minute < 10) {
      Serial.print('0');
    }
    Serial.print(GPS.minute, DEC); Serial.print(':');
    if (GPS.seconds < 10) {
      Serial.print('0');
    }
    Serial.print(GPS.seconds, DEC); Serial.print('.');
    if (GPS.milliseconds < 10) {
      Serial.print("00");
    } else if (GPS.milliseconds > 9 && GPS.milliseconds < 100) {
      Serial.print("0");
    }
    Serial.println(GPS.milliseconds);
    Serial.print("Date: ");
    Serial.print(GPS.day, DEC); Serial.print('/');
    Serial.print(GPS.month, DEC); Serial.print("/20");
    Serial.println(GPS.year, DEC);
    Serial.print("Fix: "); Serial.print((int)GPS.fix);
    Serial.print(" quality: "); Serial.println((int)GPS.fixquality);
    if (GPS.fix) {
      Serial.print("Location: ");
      Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
      Serial.print(", ");
      Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);

      Serial.print("Speed (knots): "); Serial.println(GPS.speed);
      Serial.print("Angle: "); Serial.println(GPS.angle);
      Serial.print("Altitude: "); Serial.println(GPS.altitude);
      Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
    }
    }
    ------------------------GPS END--------------------------------------------------------------*/
}

void parameters() {

  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
  if ((c) && (GPSECHO))
    Serial.write(c);

  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
    //Serial.println(GPS.lastNMEA());   // this also sets the newNMEAreceived() flag to false

    if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
      return;  // we can fail to parse a sentence in which case we should just wait for another
  }

  // approximately every 2 seconds or so, print out the current stats
  if (millis() - timer > 2000) {
    timer = millis(); // reset the timer

    Serial.print("\nTime: ");
    if (GPS.hour < 10) {
      Serial.print('0');
    }
    Serial.print(GPS.hour, DEC); Serial.print(':');
    if (GPS.minute < 10) {
      Serial.print('0');
    }
    Serial.print(GPS.minute, DEC); Serial.print(':');
    if (GPS.seconds < 10) {
      Serial.print('0');
    }
    Serial.print(GPS.seconds, DEC); Serial.print('.');
    if (GPS.milliseconds < 10) {
      Serial.print("00");
    } else if (GPS.milliseconds > 9 && GPS.milliseconds < 100) {
      Serial.print("0");
    }
    Serial.println(GPS.milliseconds);
    Serial.print("Date: ");
    Serial.print(GPS.day, DEC); Serial.print('/');
    Serial.print(GPS.month, DEC); Serial.print("/20");
    Serial.println(GPS.year, DEC);
    Serial.print("Fix: "); Serial.print((int)GPS.fix);
    Serial.print(" quality: "); Serial.println((int)GPS.fixquality);
    if (GPS.fix) {
      Serial.print("Location: ");
      Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
      Serial.print(", ");
      Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);

      Serial.print("Speed (knots): "); Serial.println(GPS.speed);
      Serial.print("Angle: "); Serial.println(GPS.angle);
      Serial.print("Altitude: "); Serial.println(GPS.altitude);
      Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
    }
  }



}



void updateSerial()
{
  delay(500);
  while (Serial.available())
  {
    GSM_Serial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while (GSM_Serial.available())
  {
    Serial.write(GSM_Serial.read());//Forward what Software Serial received to Serial Port
  }
}
