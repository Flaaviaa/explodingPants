#include <Wire.h>

#define rele_solenoide A0

const int MPU = 0x68;

float AccX, AccY, AccZ;

float GyroX, GyroY, GyroZ;
float accAngleX, accAngleY, gyroAngleX, gyroAngleY, gyroAngleZ;
float roll, pitch, yaw;
float AccErrorX, AccErrorY, GyroErrorX, GyroErrorY, GyroErrorZ;
float elapsedTime, currentTime, previousTime;

int c = 0;

void setup() {
  Serial.begin(19200);
  Wire.begin();                      
  Wire.beginTransmission(MPU);       
  Wire.write(0x6B);                  
  Wire.write(0x00);                  
  Wire.endTransmission(true);
}

void loop() {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true);
  
  AccX = (Wire.read() << 8 | Wire.read()) / 4096.0;
  AccY = (Wire.read() << 8 | Wire.read()) / 4096.0;
  AccZ = (Wire.read() << 8 | Wire.read()) / 4096.0;
  
  Wire.beginTransmission(MPU);
  Wire.write(0x43);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true);
  
  GyroX = (Wire.read() << 8 | Wire.read()) / 131.0;
  GyroY = (Wire.read() << 8 | Wire.read()) / 131.0;
  GyroZ = (Wire.read() << 8 | Wire.read()) / 131.0;
  
  GyroX = GyroX + 0.56;
  GyroY = GyroY - 2;
  GyroZ = GyroZ + 0.79;
  
  if (AccX >= 0.2 && AccX <= 0.4 && GyroX >= 26 && GyroX <= 30){
     digitalWrite(rele_solenoide, HIGH);  
  } else if (AccY >= 0.2 && AccY <= 0.4 && GyroY >= 26 && GyroY <= 30){
     digitalWrite(rele_solenoide, HIGH);  
  } else if (AccZ >= 0.2 && AccY <= 0.4 && GyroY >= 26 && GyroY <= 30){
     digitalWrite(rele_solenoide, HIGH);  
  }

}
