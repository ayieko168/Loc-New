

float input_voltage = 0.0;
float temp=0.0;


void setup()
{
   Serial.begin(9600);     //  opens serial port, sets data rate to 9600 bps
   Serial.print("DIGITAL VOLTMETER");
}
void loop()
{

//Conversion formula for voltage
   
   int analog_value = analogRead(A0);
   input_voltage = (analog_value * 5.0) / 1024.0; 

   
   if (input_voltage < 0.1) 
   {
     input_voltage=0.0;
   } 

    Serial.print("Voltage= ");
    Serial.println(input_voltage);
    delay(300);
}
