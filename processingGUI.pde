import controlP5.*;
import processing.serial.*; //import the Serial library
Serial myPort;  //the Serial port object
PrintWriter output;
ControlP5 cp5;
String val;
// since we're doing serial handshaking, 
// we need to check if we've heard from the microcontroller
boolean firstContact = false;

void setup() {
  size(900,700); // window size (width, height)  
  //  initialize your serial port and set the baud rate to 9600
  myPort = new Serial(this, "COM6", 9600);
  int d = day();
  int m = month();
  int y = year();
  int s = second();
  int h = hour();
  int mi = minute();
  String timestamp = (str(d) + "_" + str(m) + "_" + str(y) + "_h" + str(h) + "_m" + str(mi) + "_s" + str(s));
  output = createWriter(timestamp + ".txt");
  myPort.bufferUntil('\n'); 
  
  //buttons
  cp5 = new ControlP5(this);
 
 // change the default font to Verdana
 PFont p = createFont("Verdana",20); 
 ControlFont font = new ControlFont(p);
 
 // change the original colors
 cp5.setColorForeground(0xffaa0000);
 cp5.setColorBackground(0xff660000);
 cp5.setFont(font);
 cp5.setColorActive(0xffff0000);

  cp5.addButton("VMO strength")    //text on button
    .setPosition(70,10)   //position of button
    .setSize(170,40)       //size of button
  ;  
 
  Numberbox n0 = cp5.addNumberbox("VMO output")
   .setValue(0)
   .setPosition(270,10)
   .setSize(100,40);
   // change Multiplier of the Numberbox ( default is 1 )
   //n0.setMultiplier(30); 
       
  cp5.addToggle("VMO working")
   .setPosition(670,10)
   .setSize(40,40); 
   
  cp5.addButton("VL strength")    //text on button
    .setPosition(70,110)   //position of button
    .setSize(170,40)       //size of button
  ;  
 
  Numberbox n1 = cp5.addNumberbox("VL output")
   .setValue(0)
   .setPosition(270,110)
   .setSize(100,40);
   // change Multiplier of the Numberbox ( default is 1 )
   //n0.setMultiplier(30); 
       
  cp5.addToggle("VL working")
   .setPosition(670,110)
   .setSize(40,40); 
  
  cp5.addButton("ham strength")    //text on button
    .setPosition(70,210)   //position of button
    .setSize(170,40)       //size of button
  ;  
 
  Numberbox n2 = cp5.addNumberbox("strength ham output")
   .setValue(0.00)
   .setPosition(270,210)
   .setSize(100,40);
   // change Multiplier of the Numberbox ( default is 1 )
   //n0.setMultiplier(30); 
       
  cp5.addToggle("ham working")
   .setPosition(670,210)
   .setSize(40,40);   
  
  cp5.addButton("rom")
   .setPosition(70,310)
   .setSize(170,40) 
  ;  
 
  Numberbox n3 = cp5.addNumberbox("flex angle")
   .setValue(0.00)
   .setPosition(270,310)
   .setSize(100,40);
   // change Multiplier of the Numberbox ( default is 1 )
   //n1.setMultiplier(30);   
 
  cp5.addButton("acl laxity")
   .setPosition(70,410)
   .setSize(170,40) 
  ;  
 
  Numberbox n4 = cp5.addNumberbox("acl displacement")
   .setValue(0.00)
   .setPosition(270,410)
   .setSize(100,40);
   // change Multiplier of the Numberbox ( default is 1 )
   //n2.setMultiplier(30);   
     
  cp5.addToggle("acl guarding")
   .setPosition(670,410)
   .setSize(40,40); 
 
  cp5.addButton("mcl laxity")
   .setPosition(70,510)
   .setSize(170,40) 
  ;  
 
  Numberbox n5 = cp5.addNumberbox("mcl displacement")
   .setValue(0.00)
   .setPosition(270,510)
   .setSize(100,40);
   // change Multiplier of the Numberbox ( default is 1 )
   //n3.setMultiplier(30);   
   
  //cp5.addNumberbox("mcl angle")
  //  .setValue(0.00)
  //  .setPosition(470,510)
  //  .setSize(100,40);
     
  cp5.addToggle("mcl guarding")
   .setPosition(670,510)
   .setSize(40,40); 
  
  cp5.addButton("lcl laxity")
   .setPosition(70,610)
   .setSize(170,40) 
  ;  
 
  Numberbox n6 = cp5.addNumberbox("lcl displacement")
   .setValue(0)
   .setPosition(270,610)
   .setSize(100,40);
   // change Multiplier of the Numberbox ( default is 1 )
   //n4.setMultiplier(30);   
     
  cp5.addToggle("lcl guarding")
   .setPosition(670,610)
   .setSize(40,40); 
  

}


void draw() {
  background(0);
}

void serialEvent( Serial myPort) {
//put the incoming data into a String - 
//the '\n' is our end delimiter indicating the end of a complete packet
val = myPort.readStringUntil('\n');
//make sure our data isn't empty before continuing
if (val != null) {
  //trim whitespace and formatting characters (like carriage return)
  val = trim(val);
  println(val);
  if(val.equals("0") == false){
    output.println(val);
  }

  //look for our 'A' string to start the handshake
  //if it's there, clear the buffer, and send a request for data
  if (firstContact == false) {
    if (val.equals("A")) {
      myPort.clear();
      firstContact = true;
      myPort.write("A");
      println("contact");
    }
  }
  else { //if we've already established contact, keep getting and parsing data
    
    String[] line = split(val, ",");
 
    if(line[0].equals("vmo") == true){ 
      cp5.getController("VMO output").setValue(float(line[1]));
      if((float(line[1]) > 12.0) | (float(line[1]) < 10.4)){
        cp5.getController("VMO working").setValue(1);
      }
      else{
        cp5.getController("VMO working").setValue(0); 
      } 
    }  
    if(line[0].equals("vl") == true){ 
      cp5.getController("VL output").setValue(int(line[1]));
      if((float(line[1]) > 11.5) | (float(line[1]) < 9.5)){
        cp5.getController("VL working").setValue(1);
      }
      else{
        cp5.getController("VL working").setValue(0); 
      } 
    }    
    if(line[0].equals("h") == true){ 
      cp5.getController("ham output").setValue(int(line[1]));
      if(int(line[1]) >= 0.8){
        cp5.getController("ham working").setValue(1);
      }
      else{
        cp5.getController("ham working").setValue(0); 
      } 
    }
    if(line[0].equals("r") == true){
      cp5.getController("flex angle").setValue(int(line[1]));
    }
    if(line[0].equals("ad") == true){        //ACL Displacement, will be calculated to only see ACL Max
      cp5.getController("acl displacement").setValue(float(line[1]));
    } 
    if(line[0].equals("ag") == true){
      if((float(line[1]) > 12.0) | (float(line[1]) < 10.5)){
        cp5.getController("acl guarding").setValue(1);
      }
      if(line[1].equals("100.0") == true){
        cp5.getController("acl guarding").setValue(0);
      }
    }
    if(line[0].equals("ma") == true){         //MCL angle to calculate displacement
      cp5.getController("flex angle").setValue(float(line[1]));
    }
    if(line[0].equals("md") == true){        //MCL Displacement, will be calculated to only see ACL Max
      cp5.getController("mcl displacement").setValue(float(line[1]));
    } 
    if(line[0].equals("la") == true){         //LCL angle to calculate displacement
      cp5.getController("flex angle").setValue(float(line[1]));
    }
    if(line[0].equals("ld") == true){        //LCL Displacement, will be calculated to only see ACL Max
      cp5.getController("lcl displacement").setValue(float(line[1]));
    } 
    
    if (mousePressed == true) 
    {                           //if we clicked in the window
      myPort.write('1');        //send a 1
      println("1");
    }
    
    
    // when you've parsed the data you have, ask for more:
    myPort.write("A");
    }
  }
  output.flush();
}

void controlEvent(ControlEvent theEvent) {
 
 if(theEvent.isController()) { 
 
   //print("control event from : "+theEvent.getController().getName());
   //println(", value : "+theEvent.getController().getValue());
   
   // clicking on the strength button prompts arduino to run strength test 
   if(theEvent.getController().getName()=="VMO strength") {
     myPort.write('v');
   }  
   
   if(theEvent.getController().getName()=="VL strength") {
     myPort.write('q');
   } 
   
   if(theEvent.getController().getName()=="ham strength") {
     myPort.write('h');
   }  
  
   // clicking on the rom button prompts arduino to run the rom test
   if(theEvent.getController().getName()=="rom") {
     myPort.write('r');
   }  
   
   // clicking on the acl laxity button prompts arduino to run the laxity test
   if(theEvent.getController().getName()=="acl laxity") {
     myPort.write('a');
   }
   
   // clicking on the mcl laxity button prompts arduino to run the laxity test
   if(theEvent.getController().getName()=="mcl laxity") {
     myPort.write('m');
   }
    
   // clicking on the lcl laxity button prompts arduino to run the laxity test
   if(theEvent.getController().getName()=="lcl laxity") {
     myPort.write('l');
   }
 }
}
