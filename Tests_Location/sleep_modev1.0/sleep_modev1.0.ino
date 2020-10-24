
#include <avr/sleep.h>
#define interruptPin 2


void setup() {
  Serial.begin(115200);//Start Serial Comunication
  pinMode(LED_BUILTIN, OUTPUT); //We use the led on pin 13 to indecate when Arduino is A sleep
  pinMode(interruptPin, INPUT_PULLUP); //Set pin d2 to input using the buildin pullup resistor
  digitalWrite(LED_BUILTIN, HIGH); //turning LED on

  //attachInterrupt(0, Going_To_Sleep, HIGH);
}


void loop() {
  Serial.print("The Time is...");
  Serial.println(millis());

  Serial.print("Pin 2 state ...");
  Serial.println(digitalRead(interruptPin));
  
  delay(500);
  if (digitalRead(interruptPin) == HIGH){Going_To_Sleep();}

}

void Going_To_Sleep() {

  detachInterrupt(0);
  Serial.println("Goung to sleep...");
  delay(1000);
  sleep_enable();//Enabling sleep mode
  attachInterrupt(0, wakeUp, LOW);//attaching a interrupt to pin d2
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);//Setting the sleep mode, in our case full sleep
  digitalWrite(LED_BUILTIN, LOW); //turning LED off
  delay(1000); //wait a second to allow the led to be turned off before going to sleep
  sleep_cpu();//activating sleep mode
  Serial.println("just woke up!");//next line of code executed after the interrupt
  digitalWrite(LED_BUILTIN, HIGH); //turning LED on
}

void wakeUp() {
  Serial.println("Interrrupt Fired");//Print message to serial monitor
  sleep_disable();//Disable sleep mode
  detachInterrupt(0); //Removes the interrupt from pin 2;

}
