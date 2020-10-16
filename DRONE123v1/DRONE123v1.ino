#include <SoftwareSerial.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#include <VoltageReference.h>
#include <TinyGPS.h>
#include <Adafruit_GPS.h>
#include <Adafruit_GPS.h>

//VARIABLE
String Latitude;
String Longitude;
String Altitude;
String Velocity;
String End;
String Elevation;
String vIN;
//END
//---Bicolor Status LED
int RED_LED = 8;
int GREEN_LED = 9;

//SoftwareSerial Debug(6, 5); // Debugging (RX TX)
//SoftwareSerial GPS_Serial(1, 0);   // Connected to GPS
//SoftwareSerial GSM_Serial (10,11);// connected to GSM
Adafruit_GPS GPS_Serial(13, 12);   // Connected to GPS (RX TX)
SoftwareSerial mySerial(11, 10);

Adafruit_BMP280 bmp;
VoltageReference vRef = VoltageReference();
TinyGPS gps;
//gsm variable
String apn = "safaricom";                       //APN
String apn_u = "saf";                     //APN-Username
String apn_p = "data";                     //APN-Password
String url = "http://utm.karimu.co.ke/iot-save.php";  //URL for HTTP-POST-REQUEST
String data1;   //String for the first Paramter (e.g. Sensor1)
String data2;   //String for the second Paramter (e.g. Sensor2)
//end
void setup() {

  Serial.begin(115200);
  Serial.println("Serial Initialized!");
  
  pinMode(RED_LED, OUTPUT); // Status LEDs
  pinMode(GREEN_LED, OUTPUT); // Status LEDs

  vRef.begin();              //Vcc read library
  //-----------------------------BMP 280------------------------------------
  if (!bmp.begin()) {
    //Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

  //-----------------------------GPS------------------------------------
  Serial.println("Initializing..."); 
  delay(1000);

  mySerial.println("AT"); //Once the handshake test is successful, i t will back to OK
  updateSerial();
  
  mySerial.println("ATD+ +254722534687;"); //  change ZZ with country code and xxxxxxxxxxx with phone number to dial
  updateSerial();
  delay(20000); // wait for 20 seconds...
  mySerial.println("ATH"); //hang up
  updateSerial();

}


void loop()
{

  GPS_Serial.begin(9600);

  if (GPS_Serial.fix) {
      Serial.print("Location: ");
      Serial.print(GPS_Serial.latitude, 4); Serial.print(GPS_Serial.lat);
      Serial.print(", ");
      Serial.print(GPS_Serial.longitude, 4); Serial.println(GPS_Serial.lon);
      Serial.print("Location (in degrees, works with Google Maps): ");
      Serial.print(GPS_Serial.latitudeDegrees, 4);
      Serial.print(", ");
      Serial.println(GPS_Serial.longitudeDegrees, 4);
      Serial.print("Speed (knots): "); Serial.println(GPS_Serial.speed);
      Serial.print("Angle: "); Serial.println(GPS_Serial.angle);
      Serial.print("Altitude: "); Serial.println(GPS_Serial.altitude);
      Serial.print("Satellites: "); Serial.println((int)GPS_Serial.satellites);
    }

  
//  parameters();
//  GPS_Serial.end();
//  mySerial.begin(9600);
//  End = "0";
//  mySerial.println("AT");
//  runsl();//Print GSM Status an the Serial Output;
//  delay(100);
//  mySerial.println("AT+SAPBR=3,1,Contype,GPRS");
//  runsl();
//  delay(100);
//  mySerial.println("AT+SAPBR=3,1,APN," + apn);
//  runsl();
//  delay(100);
//
//  mySerial.println("AT+SAPBR =1,1");
//  runsl();
//  delay(500);
//  mySerial.println("AT+SAPBR=2,1");
//  runsl();
//  delay(500);
//  mySerial.println("AT+HTTPINIT");
//  runsl();
//  delay(500);
//  mySerial.println("AT+HTTPPARA=CID,1");
//  runsl();
//  delay(100);
//  mySerial.println("AT+HTTPPARA=URL," + url);
//  runsl();
//  delay(100);
//  mySerial.println("AT+HTTPPARA=CONTENT,application/x-www-form-urlencoded");
//  runsl();
//  delay(100);
//  mySerial.println("AT+HTTPDATA=192,10000");
//  runsl();
//  delay(100);
//  //mySerial.print("api_token=nCEdlyIEl95E2DkignU2XCLSSkzaPqphfWmeydcA6qiJvRxFcAwEEAD1izAG2fQ&longitude=" + Longitude + "&latitude=" + Latitude + "&altitude=" + Altitude + "&battery=" + vIN +"&end=" + End);
//  //mySerial.print("api_token=vI89s84tfxMkjarIKy4mRmZNOwxF8F7pLST5QQxZkSSyZihtCdaWv2y5uIF6&longitude=" + Longitude + "&latitude=" + Latitude + "&altitude=" + Altitude + "&velocity=" + Velocity  + "&battery=" + vIN + "&End=" + End);
//  mySerial.println("api_token=GM61w8fLm3N7dXF8MIEcsm9BFe6hv4g484Nydh5ARVINo5H4BRBwCwxHmpEycAW&longitude=" + Longitude + "&latitude=" + Latitude + "&altitude=" + Altitude + "&battery=" + vIN + "&end=" + End);
//  runsl();
//  delay(9000);
//  mySerial.println("AT+HTTPACTION=1");
//  runsl();
//  delay(5000);
//  mySerial.end();


}



//void parameters() {
//
//
//  bool newData = false;
//  unsigned long chars;
//  unsigned short sentences, failed;
//
//  for (unsigned long start = millis(); millis() - start < 1000;)
//  {
//    while (GPS_Serial.available())
//    {
//      char c = GPS_Serial.read();
//      Serial.println(c); // uncomment this line if you want to see the GPS data flowing
//      if (gps.encode(c)) // Did a new valid sentence come in?
//        newData = true;
//    }
//  }
//
//  if (newData)
//  {
//    float flat, flon, GPS_altitude, Travel_speed;
//    unsigned long age;
//    GPS_altitude = gps.f_altitude();
//    Travel_speed = gps.f_speed_kmph();
//
//    gps.f_get_position(&flat, &flon, &age);
//
//    Serial.println();
//    Serial.println("Values:- ");
//    Serial.print("LAT=");
//    Serial.print(flat , 6);
//    Serial.print(" LON=");
//    Serial.print(flon , 6);
//    Serial.print(" Speed=");
//    Serial.print(Travel_speed , 6);
//    Serial.print(" GPS Altitude=");
//    Serial.print(GPS_altitude , 6);
//    Serial.print(" SAT=");
//    Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
//    Serial.print(" PREC=");
//    Serial.println(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
//
//    delay(500);
//    Latitude = String(flat);
//    Longitude = String(flon);
//    //Altitude = String((GPS_altitude , 6));
//
//
//
//  }
//  int Satellites = 0;
//  Satellites = int(gps.satellites());
//
//  if (Satellites > 4 && Satellites <= 22 )
//  {
//    digitalWrite(GREEN_LED, HIGH);
//    delay(100);
//    digitalWrite(GREEN_LED, LOW);
//    delay(100);
//    digitalWrite(GREEN_LED, HIGH);
//    delay(100);
//    digitalWrite(GREEN_LED, LOW);
//  }
//  else
//  {
//    digitalWrite(RED_LED, HIGH);
//    delay(100);
//    digitalWrite(RED_LED, LOW);
//    delay(100);
//    digitalWrite(RED_LED, HIGH);
//    delay(100);
//    digitalWrite(RED_LED, LOW);
//  }
//
//  float vcc = vRef.readVcc() / 1000.00;
//  Serial.print("Battery Voltage: ");
//  Serial.println(String(vcc) + " volts");
//
//
//
//  //    mySerial.print(bmp.readTemperature());
//  //    mySerial.println(" *C");
//  //
//  Serial.print(F("Pressure = "));
//  Serial.print(bmp.readPressure());
//  Serial.println(" Pa");
//  Serial.print(F("Approx altitude = "));
//  Serial.print(bmp.readAltitude(1013.25)); /* Adjusted to local forecast! */
//  Serial.println(" m");
//
//
//  Altitude = String((bmp.readAltitude(1013.25)));
//  vIN = String(vcc);
//  delay(1000);
//
//
//
//
//}

void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}

void runsl() {
  while (mySerial.available()) {
    Serial.print(mySerial.read());
  }

}
