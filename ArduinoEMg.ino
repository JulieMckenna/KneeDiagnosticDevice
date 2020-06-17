const int numReadings = 10;

float readings[4][numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
float total1 = 0;                  // the running total
float total2 = 0; 
float total3 = 0; 
float total4 = 0;  
float average1 = 0;             // the average
float average2 = 0;
float average3 = 0; 
float average4 = 0; 
float restingavg1 = 0;
float restingavg2 = 0;
float restingavg3 = 0;
float restingavg4 = 0;

//multiply the analog input by * 5.0/1023.0 - to convert the read in values to a scale from 0-5

void setup() {
  // put your setup code here, to run once:

  //EMG sensor 1
  pinMode(A0, INPUT);

  //EMG sensor 2
  pinMode(A1, INPUT);

  //EMG sensor 3
  pinMode(A2, INPUT);

  //EMG sensor 4
  //pinMode(A3, INPUT);

  //LED light
  pinMode(0, OUTPUT);
   
  //start the connection
  Serial.begin(9600);

  //the user rests for a little - allows to adjust
  for (int i = 0; i < 100; i++){
    Serial.print(0);
    restingavg1 = restingavg1 + (analogRead(A0)*(5.0/1023.0));
    restingavg2 = restingavg2 + (analogRead(A1)*(5.0/1023.0));
    restingavg3 = restingavg3 + (analogRead(A2)*(5.0/1023.0);
    restingavg4 = restingavg4 + (analogRead(A3)*(5.0/1023.0);    
  }

  //to get the average for the resting period
  restingavg1 = restingavg1/(100);
  //Serial.print(restingavg1);
  restingavg2 = restingavg2/(100);
  //Serial.println(restingavg2);
  restingavg3 = restingavg3/(100);
  //Serial.println(restingavg3);
  restingavg4 = restingavg4/(100);
  //Serial.println(restingavg4);

 // initialize all the readings to 0:
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[0][thisReading] = 0;
    readings[1][thisReading] = 0;
    readings[2][thisReading] = 0;
    readings[3][thisReading] = 0;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  
  // subtract the last reading:
  total1 = total1 - readings[0][readIndex];
  total2 = total2 - readings[1][readIndex];
  total3 = total3 - readings[2][readIndex];
  total4 = total4 - readings[3][readIndex];
  // read from the sensor:
  readings[0][readIndex] = (analogRead(A0)*(5.0/1023.0);
  readings[1][readIndex] = (analogRead(A1)*(5.0/1023.0);
  readings[2][readIndex] = (analogRead(A2)*(5.0/1023.0);
  readings[3][readIndex] = (analogRead(A3)*(5.0/1023.0);
  
  // add the reading to the total:
  total1 = total1 + readings[0][readIndex];
  total2 = total2 + readings[1][readIndex];
  total3 = total3 + readings[2][readIndex];
  total4 = total4 + readings[3][readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  average1 = total1/numReadings;
  average2 = total2/numReadings;
  average3 = total3/numReadings;
  average4 = total4/numReadings;
  // send it to the computer as ASCII digits
  
  //Serial.println(analogRead(A0)/20);
  //Serial.println(analogRead(A0)/20 - restingavg);
  
  //top bound of the resting
  Serial.print(" ");
  //Serial.print(40);
  Serial.print(" ");
  //low bound of the resting
  //Serial.print(-40);
  Serial.print(" ");
  //prints the EMG output
  //Serial.println(average1 - restingavg1);
  Serial.println(average2 - restingavg2);
  Serial.println(average3 - restingavg3);
  Serial.println(average4 - restingavg4);
  delay(1);

  //if the EMG crosses either the +16 or -16 line should signal a light that the muscle is engaged for quad

  //EMG 2 - between 0 and 40 resting (hamstring)

  //if any of the emg signals are out of resting range - turn on led

}

//graph
