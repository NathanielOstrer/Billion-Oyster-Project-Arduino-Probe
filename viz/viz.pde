import processing.serial.*;

Serial myPort;
String val;

void setup() {
  size(200, 200);
  String portName = Serial.list()[1];
  myPort = new Serial(this, portName, 9600);
  ellipseMode(RADIUS);
  rectMode(CENTER);
}

void draw() {
  String val = myPort.readStringUntil('\n');
  //String val = "ph:3.675 do:6.21 tm:25";
  String[] vals = val.split(" ");
  
  for (int i = 0; i < vals.length; i++) {
    double value = Double.parseDouble(vals[i].substring(3));
    String label = vals[i].substring(0, 2);
    int x = width/vals.length*i + 25;
    int y = height/2;
    int r = (int)value;
    if (r < 10) r *= 5;
    
    
    ellipse(x, y, r, r);
    fill(0);
    text("" + value, x-5, y-50);
    text(label, x-5, y+50);
    fill(255);
  }
}