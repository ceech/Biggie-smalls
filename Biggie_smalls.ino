float readVcc() 
{
  signed long resultVcc;
  float resultVccFloat;
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(10);                           // Wait for Vref to settle
  ADCSRA |= _BV(ADSC);                 // Convert
  while (bit_is_set(ADCSRA,ADSC));
  resultVcc = ADCL;
  resultVcc |= ADCH<<8;
  resultVcc = 1126400L / resultVcc;    // Back-calculate AVcc in mV
  resultVccFloat = (float) resultVcc / 1000.0; // Convert to Float

  return resultVccFloat;
}

int current = A6;
int chrg = A1;
int cell = A2;
int lipo = A0;
int batterycurrent = A7;

void setup() 
{
  Serial.begin(9600);
}

void loop() 
{
  float napetost = readVcc();

  float tok = ((analogRead(current) * napetost / 1024 ) / 2500) * 1000000; // convert the ADC value to miliamps
  float tokbaterija = ((analogRead(batterycurrent) * napetost / 1024 ) / 2500) * 1000000; // convert the ADC value to miliamps
  float panel = ( analogRead(cell) * napetost / 1024 ) * 2; // measuring input voltage
  float baterija = ( analogRead(lipo) * napetost / 1024 ) * 2; // measuring battery voltage
  int polnjenje = analogRead(chrg);
  
  Serial.print("Vcc = ");
  Serial.print(napetost);
  Serial.println("V");
  delay(400);
  Serial.print("Input current = ");
  Serial.print(tok);
  Serial.println("mA");
  delay(400);
  Serial.print("Charge current = ");
  Serial.print(tokbaterija);
  Serial.println("mA");
  delay(400);
  Serial.print("Solar cell voltage = ");
  Serial.print(panel);
  Serial.println("V");
  delay(400);
  Serial.print("Battery voltage = ");
  Serial.print(baterija);
  Serial.println("V");
  delay(400);
  Serial.print("CHRG = ");
  Serial.println(polnjenje);
  Serial.println("----------------------------");
  delay(2000);
}
/*
Improving accuracy:
To do so, simply measure your Vcc with a voltmeter and with our readVcc() function. Then, replace the constant 1107035L with a new constant:
scale_constant = internal1.1Ref * 1024 * 1000
where
internal1.1Ref = 1,1 * Vcc1 (per voltmeter) / Vcc2 (per readVcc() function)
Example:
For instance, I measure 3,43V from my FTDI, the calculated value of Vref is 1,081V.
So (1,081 x 1000 x 1024) = 1107034,95 or 1107035L rounded up.
Source: http://provideyourown.com/2012/secret-arduino-voltmeter-measure-battery-voltage/
and
https://code.google.com/p/tinkerit/wiki/SecretVoltmeter
*/
