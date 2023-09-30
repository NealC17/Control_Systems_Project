#include <Servo.h>
#include <Wire.h>

Servo xy;
Servo z;
int angleXY;
int angleZ;
//float x, y, z;
String temp = "";
const int addr = 0x68;
float roll, pitch, yaw;
float ax, ay, az;
float gx, gy, gz;
float currentTime = 0;
float previousTime = 0;
void setup() {
  Serial.begin(9600);
  //delay(5000);
  xy.attach(5);
  z.attach(6);

  Wire.begin();
  Wire.beginTransmission(addr);
  Wire.write(0x00);
  Wire.endTransmission(true);
  
  
}

void loop() {
  currentTime = millis();
  Serial.print("angleXY : ");
  Serial.print(angleXY);
  Serial.print("\tangleZ : ");
  Serial.print(angleZ);
  Serial.print("\troll : ");
  Serial.print(roll);
  Serial.print("\tpitch : ");
  Serial.print(pitch);
  Serial.print("\tyaw : ");
  Serial.print(yaw);
  Serial.print("\t d yaw: ");
  Serial.print(gz * (currentTime - previousTime)/1000 - 0.021);
  Serial.print("\n");

  if(Serial.available() > 0){
    temp = Serial.readString();
    angleXY = temp.substring(0, temp.indexOf(' ')).toInt();
    angleZ = temp.substring(temp.indexOf(' ')+1).toInt();
    
  }
  Wire.beginTransmission(addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(addr, 6, true);
  
  ax = (Wire.read() << 8 | Wire.read())/131.0;
  ay = (Wire.read() << 8 | Wire.read())/131.0;
  az = (Wire.read() << 8 | Wire.read())/131.0;

  Wire.beginTransmission(addr);
  Wire.write(0x43);
  Wire.endTransmission(false);
  Wire.requestFrom(addr, 6, true);
  
  gx = (Wire.read() << 8 | Wire.read())/131.0;
  gy = (Wire.read() << 8 | Wire.read())/131.0;
  gz = (Wire.read() << 8 | Wire.read())/131.0;

  

  roll = atan(ay/ sqrt(ax * ax + az * az)) * 180 / PI;
  pitch = atan(-ax/ sqrt(ay * ay + az * az)) * 180 / PI;
  yaw += gz * (currentTime - previousTime)/1000 - 0.021;

  previousTime = currentTime;


  

  
  xy.write(map(round(-yaw+angleXY),-90, 90, 0, 180));
  z.write(map(round(-pitch*0.95 - 0.05 * gx + angleZ),-90, 90, 0, 180)); 
}
