#include <TinyGPS++.h>
#include <SoftwareSerial.h>

#include <avr/sleep.h> //tony 
#define interruptPin 2 //tony 

/*
   This sample code demonstrates the normal use of a TinyGPS++ (TinyGPSPlus) object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/
static const int RXPin = 3, TXPin = 4;
static const uint32_t GPSBaud = 9600;

String Longitude;
String Latitude;
String Altitude;
int FIX = 11;

String End;
//GSM
SoftwareSerial mySerial(9, 10);            // RX, TX Pins
String apn = "safaricom";                       //APN
String apn_u = "saf";                     //APN-Username
String apn_p = "data";                     //APN-Password
String url = "http://utm.karimu.co.ke/iot-save.php";  //URL for HTTP-POST-REQUEST

//voltage
const int voltageSensor = A0;
int sensorPin = A1;
float vOUT = 0.0;
float vIN = 0.0;
float R1 = 30000.0;
float R2 = 7500.0;
int value = 0;



// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void setup()
{
  pinMode(FIX, OUTPUT);
  Serial.begin(115200);

  pinMode(interruptPin, INPUT_PULLUP); //tony 
  
  Serial.println(F("FullExample.ino"));
  Serial.println(F("An extensive example of many interesting TinyGPS++ features"));
  Serial.print(F("Testing TinyGPS++ library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println(F("by Mikal Hart"));
  Serial.println();
  Serial.println(F("Sats HDOP  Latitude   Longitude   Fix  Date       Time     Date Alt    Course Speed Card  Distance Course Card  Chars Sentences Checksum"));
  Serial.println(F("           (deg)      (deg)       Age                      Age  (m)    --- from GPS ----  ---- to London  ----  RX    RX        Fail"));
  Serial.println(F("----------------------------------------------------------------------------------------------------------------------------------------"));
}

void loop()
{
  if (digitalRead(interruptPin) == HIGH){End = "1";} else{End = "0";}

  voltage();
  ss.begin(GPSBaud);
  parameters();
  ss.end();
  mySerial.begin(9600);

  mySerial.println("AT");
  runsl();//Print GSM Status an the Serial Output;
  delay(100);
  mySerial.println("AT+SAPBR=3,1,Contype,GPRS");
  runsl();
  delay(100);
  mySerial.println("AT+SAPBR=3,1,APN," + apn);
  runsl();
  delay(100);

  mySerial.println("AT+SAPBR =1,1");
  runsl();
  delay(500);
  mySerial.println("AT+SAPBR=2,1");
  runsl();
  delay(500);
  mySerial.println("AT+HTTPINIT");
  runsl();
  delay(500);
  mySerial.println("AT+HTTPPARA=CID,1");
  runsl();
  delay(100);
  mySerial.println("AT+HTTPPARA=URL," + url);
  runsl();
  delay(100);
  mySerial.println("AT+HTTPPARA=CONTENT,application/x-www-form-urlencoded");
  runsl();
  delay(100);
  mySerial.println("AT+HTTPDATA=192,10000");
  runsl();
  delay(100);
  mySerial.print("api_token=bCpnTfBbe8H7G9b2dWcf8Ed2DqSupZ8K6SSv1HUkFfEywxjW0ihXDKo45VCHWgSqIi4hU&longitude=" + Longitude + "&latitude=" + Latitude + "&altitude=" + Altitude + "&battery=" + vIN + "&end=" + End);
  //mySerial.print("api_token=vI89s84tfxMkjarIKy4mRmZNOwxF8F7pLST5QQxZkSSyZihtCdaWv2y5uIF6&longitude=" + Longitude + "&latitude=" + Latitude + "&altitude=" + Altitude + "&velocity=" + Velocity  + "&battery=" + vIN + "&End=" + End);
  //mySerial.println("api_token=vI89s84tfxMkjarIKy4mRmZNOwxF8F7pLST5QQxZkSSyZihtCdaWv2y5uIF6");
  runsl();
  delay(9000);
  mySerial.println("AT+HTTPACTION=1");
  runsl();
  delay(5000);
  runsl();
  delay(200);
  mySerial.println("AT+HTTPTERM");
  runsl();
  mySerial.end();
  
  if (digitalRead(interruptPin) == HIGH and End == "1"){Going_To_Sleep();}
}


void Going_To_Sleep() {

  detachInterrupt(0);
  Serial.println("Goung to sleep...");
  delay(1000);
  
  sleep_enable();//Enabling sleep mode
  attachInterrupt(0, wakeUp, LOW);//attaching a interrupt to pin d2
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);//Setting the sleep mode, in our case full sleep
//  digitalWrite(LED_BUILTIN, LOW); //turning LED off
  delay(1000); //wait a second to allow the led to be turned off before going to sleep
  sleep_cpu();//activating sleep mode
  Serial.println("just woke up!");//next line of code executed after the interrupt
//  digitalWrite(LED_BUILTIN, HIGH); //turning LED on
}

void wakeUp() {
  Serial.println("Interrrupt Fired");//Print message to serial monitor
  sleep_disable();//Disable sleep mode
  detachInterrupt(0); //Removes the interrupt from pin 2;

}


void parameters() {

  static const double LONDON_LAT = 51.508131, LONDON_LON = -0.128002;

  printInt(gps.satellites.value(), gps.satellites.isValid(), 5);
  printFloat(gps.hdop.hdop(), gps.hdop.isValid(), 6, 1);
  printFloat(gps.location.lat(), gps.location.isValid(), 11, 6);
  printFloat(gps.location.lng(), gps.location.isValid(), 12, 6);
  printInt(gps.location.age(), gps.location.isValid(), 5);
  printDateTime(gps.date, gps.time);
  printFloat(gps.altitude.meters(), gps.altitude.isValid(), 7, 2);
  printFloat(gps.course.deg(), gps.course.isValid(), 7, 2);
  printFloat(gps.speed.kmph(), gps.speed.isValid(), 6, 2);
  printStr(gps.course.isValid() ? TinyGPSPlus::cardinal(gps.course.deg()) : "*** ", 6);

  unsigned long distanceKmToLondon =
    (unsigned long)TinyGPSPlus::distanceBetween(
      gps.location.lat(),
      gps.location.lng(),
      LONDON_LAT,
      LONDON_LON) / 1000;
  printInt(distanceKmToLondon, gps.location.isValid(), 9);

  double courseToLondon =
    TinyGPSPlus::courseTo(
      gps.location.lat(),
      gps.location.lng(),
      LONDON_LAT,
      LONDON_LON);

  printFloat(courseToLondon, gps.location.isValid(), 7, 2);

  const char *cardinalToLondon = TinyGPSPlus::cardinal(courseToLondon);

  printStr(gps.location.isValid() ? cardinalToLondon : "*** ", 6);

  printInt(gps.charsProcessed(), true, 6);
  printInt(gps.sentencesWithFix(), true, 10);
  printInt(gps.failedChecksum(), true, 9);
  Serial.println();

  smartDelay(1000);


  Latitude = String(gps.location.lat() , 6);
  Longitude = String(gps.location.lng() , 6);
  Altitude = String(gps.altitude.meters());

  if (Latitude == "0.000000" or End == "1")
    digitalWrite(FIX, LOW);
  else
    digitalWrite(FIX, HIGH);
    
  if (millis() > 5000 && gps.charsProcessed() < 10)
    Serial.println(F("No GPS data received: check wiring"));

}




// This custom version of delay() ensures that the gps object
// is being "fed".
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}

static void printFloat(float val, bool valid, int len, int prec)
{
  if (!valid)
  {
    while (len-- > 1)
      Serial.print('*');
    Serial.print(' ');
  }
  else
  {
    Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i = flen; i < len; ++i)
      Serial.print(' ');
  }
  smartDelay(0);
}

static void printInt(unsigned long val, bool valid, int len)
{
  char sz[32] = "*****************";
  if (valid)
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i = strlen(sz); i < len; ++i)
    sz[i] = ' ';
  if (len > 0)
    sz[len - 1] = ' ';
  Serial.print(sz);
  smartDelay(0);
}

static void printDateTime(TinyGPSDate &d, TinyGPSTime &t)
{
  if (!d.isValid())
  {
    Serial.print(F("********** "));
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d/%02d/%02d ", d.month(), d.day(), d.year());
    Serial.print(sz);
  }

  if (!t.isValid())
  {
    Serial.print(F("******** "));
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d:%02d:%02d ", t.hour(), t.minute(), t.second());
    Serial.print(sz);
  }

  printInt(d.age(), d.isValid(), 5);
  smartDelay(0);
}

static void printStr(const char *str, int len)
{
  int slen = strlen(str);
  for (int i = 0; i < len; ++i)
    Serial.print(i < slen ? str[i] : ' ');
  smartDelay(0);
}


void runsl() {
  while (mySerial.available()) {
    Serial.write(mySerial.read());
  }

}

void voltage() {

  value = analogRead(voltageSensor);
  vOUT = (value * 5.0) / 1024.0;
  vIN = vOUT / (R2 / (R1 + R2));
  Serial.print("Input = ");
  Serial.println(vIN);
}
