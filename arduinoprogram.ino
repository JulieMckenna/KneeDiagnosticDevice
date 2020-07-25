char val; // Data received from the serial port
boolean ledState = LOW; //to toggle our LED
const int numReadings = 10;

float readings[4][numReadings];      // the readings from the analog input
float FlexReadings[numReadings];
float LDS[1000];                 //reading from LDS - use to find min and max
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

//for flex sensor
//Constants:
const int ledPin = 3;     //pin 3 has PWM funtion
const int flexPin = A5;   //pin A0 to read analog input
float totalFlex = 0;

//Variables:
int value;                //save analog value
int averageFlex = 0;
float muscle;

//for LDS
float LaxityACL = 0;
float LaxityMCL = 0;
float LaxityLCL = 0;

void setup() 
{
  pinMode(ledPin, OUTPUT); // Set pin as OUTPUT
   //EMG sensor 1
  pinMode(A0, INPUT);
  //EMG sensor 2
  pinMode(A1, INPUT);
  //EMG sensor 3
  pinMode(A2, INPUT);
  //EMG sensor 4
  pinMode(A3, INPUT);
  //Flex sensor
  pinMode(flexPin, INPUT);
  //LDS
  pinMode(A4, INPUT);
  //initialize serial communications at a 9600 baud rate
  Serial.begin(9600);

     //the user rests for a little - allows to adjust
  for (int i = 0; i < 100; i++){
    Serial.println(0);
    restingavg1 = restingavg1 + (analogRead(A0)*(5.0/1023.0));
    restingavg2 = restingavg2 + (analogRead(A1)*(5.0/1023.0));
    restingavg3 = restingavg3 + (analogRead(A2)*(50.0/1023.0));
    restingavg4 = restingavg4 + (analogRead(A3)*(50.0/1023.0));    
  }

  //to get the average for the resting period
  restingavg1 = restingavg1/(100);
  restingavg2 = restingavg2/(100);
  restingavg3 = restingavg3/(100);
  restingavg4 = restingavg4/(100);
  
  establishContact();  // send a byte to establish contact until receiver responds 
}

void loop()
{
  if (Serial.available() > 0) { // If data is available to read,
    val = Serial.read(); // read it and store it in val

    if(val == '1') //if we get a 1
    {
       ledState = !ledState; //flip the ledState
       digitalWrite(ledPin, ledState); 
    }  
    else if(val == 'v'){ //VMO is EMG1
      for(int i = 0; i<1000; i++){    //10 seconds to flex leg
        muscle = EMG1()*10;   //scaled for visibility
        Serial.println("vmo," + (String)muscle);
      }
    }
    else if(val == 'q'){ //VL is EMG2
      for(int i = 0; i<5000; i++){    //5 seconds to flex leg
        value = EMG2()*10;   //scaled for visibility
        Serial.println("vl," + (String)value);
      }
    }
    else if(val == 'h'){ //hamstring is EMG3
      for(int i = 0; i<5000; i++){    //5 seconds to flex leg
        value = EMG3()*10;   //scaled for visibility
        Serial.println("h," + (String)value);
      }
    }
    else if(val == 'r'){
      for(int i = 0; i<100; i++){
        value = flex();
        delay(100);
        Serial.println("r," + (String)value);
      }
    }
    else if(val == 'a'){ //acl laxity
      double min = 0.0, max = 0.0;
      for(int i = 0; i<30; i++){
        double val = EMG1()*10;
        if(i>8){
          Serial.println("ag," + (String)val);
        }
        LaxityACL = laxity();
        if((max == 0.0) and (min == 0.0)){
          max = LaxityACL;
          min = LaxityACL;
        }
        else if(max < LaxityACL){
          max = LaxityACL;
        }
        else if(min > LaxityACL){
          min = LaxityACL;
        }
        delay(100);
      }
      LaxityACL = max-min;
      Serial.println("ad," + (String)LaxityACL);
      delay(5000);
      Serial.println("ag,100.0");
    }
    else if(val == 'm'){ //mcl laxity
      double min = 0.0, max = 0.0;
      float angle = flex();
      Serial.println("ma," + (String)angle);
      for(int i = 0; i<30; i++){
        LaxityMCL = laxity();
        if((max == 0.0) and (min == 0.0)){
          max = LaxityMCL;
          min = LaxityMCL;
        }
        else if(max < LaxityMCL){
          max = LaxityMCL;
        }
        else if(min > LaxityMCL){
          min = LaxityMCL;
        }
        delay(100);
      }
      LaxityMCL = (max-min)/cos(angle);
      Serial.println("md," + (String)LaxityMCL);
    }
    else if(val == 'l'){ //lcl laxity
      double min = 0.0, max = 0.0;
      float angle = flex();
      Serial.println("la," + (String)angle);
      for(int i = 0; i<30; i++){
        LaxityLCL = laxity();
        if((max == 0.0) and (min == 0.0)){
          max = LaxityLCL;
          min = LaxityLCL;
        }
        else if(max < LaxityLCL){
          max = LaxityLCL;
        }
        else if(min > LaxityLCL){
          min = LaxityLCL;
        }
        delay(100);
      }
      LaxityLCL = (max-min)/sin(angle);
      Serial.println("ld," + (String)LaxityLCL);
    }
  }
  else {
    Serial.println("0"); //send back a hello world
    delay(100);
    }
}

void establishContact() {
  while (Serial.available() <= 0) {
    Serial.println("A");   // send a capital A
    delay(300);
  }
}

double flex(){
   //flex sensor:
  value = analogRead(flexPin);         //Read and save analog value from potentiometer
  //Serial.println(value);               //Print value
  //range out of sleeve: (bent)180 = 855-860, 90 = 810-815, 0(straight) = 615-620
  // (180)860- 0(615) = 245. per degree = 245/180 = 1.4  = 1 degree
  //in the sleeve 0 degrees is 658
 
  //find average of the values
  readIndex = 0;
  
  totalFlex = totalFlex - FlexReadings[readIndex];
  FlexReadings[readIndex] = double((value - 658)/1.44);
  totalFlex = totalFlex + FlexReadings[readIndex];

  readIndex = readIndex + 1;

    // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }
  
  averageFlex = totalFlex/numReadings;
  return averageFlex;
}

//returns double that the processing reads to determine if out of resting range
double EMG1(){
  // subtract the last reading:
  total1 = total1 - readings[0][readIndex];
 
  // read from the sensor:
  readings[0][readIndex] = (analogRead(A0)*(5.0/1023.0));
  
  // add the reading to the total:
  total1 = total1 + readings[0][readIndex];
  
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  average1 = total1/numReadings;

  delay(1);
  return average1;
}

double EMG2(){
  // subtract the last reading:
  total2 = total2 - readings[1][readIndex];
  
  // read from the sensor:
  readings[1][readIndex] = (analogRead(A1)*(5.0/1023.0));
  
  // add the reading to the total:
  total2 = total2 + readings[1][readIndex];
  
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  average2 = total2/numReadings;

  delay(1);
  return average2;
}

double EMG3(){
  // subtract the last reading:
  total3 = total3 - readings[2][readIndex];
  
  // read from the sensor:
  readings[2][readIndex] = (analogRead(A2)*(5.0/1023.0));
  
  // add the reading to the total:
  total3 = total3 + readings[2][readIndex];
  
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  average3 = total3/numReadings;

  delay(1);
  return average3;
}

//test for Lachmans - needs no inputs
double laxity(){
  double voltageread = float(analogRead(A4)*(5.0/1023.0)/.0254);

  delay(10);
  Serial.println(voltageread);
  return voltageread;  
}
