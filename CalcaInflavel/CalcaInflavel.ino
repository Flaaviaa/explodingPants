#include <Wire.h>
#include <ITG3200.h>

ITG3200 gyro = ITG3200();
float  gx,gy,gz;
float  gx_rate, gy_rate, gz_rate;

#define Register_ID 0
#define Register_2D 0x2D
#define Register_X0 0x32
#define Register_X1 0x33
#define Register_Y0 0x34
#define Register_Y1 0x35
#define Register_Z0 0x36
#define Register_Z1 0x37
int rele_solenoide_esq = 3;
int rele_solenoide_dir = 4;

int ADXAddress = 0x53;
int reading = 0;
int val=0;
int X0,X1,X_out;
int Y0,Y1,Y_out;
int Z1,Z0,Z_out;
double Xa,Ya,Za;
float acceleration_magnitude;

float goffsetX, goffsetY, goffsetZ;
float gyro_magnitude;
void setup() {
  Wire.begin();                
  Serial.begin(9600);
  pinMode (rele_solenoide_esq, OUTPUT);
  pinMode (rele_solenoide_dir, OUTPUT);
  for (int i = 0; i <= 200; i++) {
    gyro.readGyro(&gx,&gy,&gz);
    if (i == 0) {
      goffsetX = gx;
      goffsetY = gy;
      goffsetZ = gz;
    }
    if (i > 1) {
      goffsetX = (gx + goffsetX) / 2;
      goffsetY = (gy + goffsetY) / 2;
      goffsetZ = (gz + goffsetZ) / 2;
    }
  }
  
  
  delay(1000);
  gyro.init(ITG3200_ADDR_AD0_LOW); 
  Serial.print("zero Calibrating...");
  gyro.zeroCalibrate(2500, 2);
  
  Wire.beginTransmission(ADXAddress);
  Wire.write(Register_2D);
  Wire.write(8);
  Wire.endTransmission();
}

void loop() {
  digitalWrite(rele_solenoide_esq, LOW);
  digitalWrite(rele_solenoide_dir, LOW);
  //--------------X
  Wire.beginTransmission(ADXAddress); // transmit to device
  Wire.write(Register_X0);
  Wire.write(Register_X1);
  Wire.endTransmission();
  Wire.requestFrom(ADXAddress,2); 
  
  if(Wire.available()<=2) {
    X0 = Wire.read();
    X1 = Wire.read(); 
    X1=X1<<8;
    X_out=X0+X1;   
  }
  
  //------------------Y
  Wire.beginTransmission(ADXAddress); // transmit to device
  Wire.write(Register_Y0);
  Wire.write(Register_Y1);
  Wire.endTransmission();
  Wire.requestFrom(ADXAddress,2); 
  
  if(Wire.available()<=2) {
    Y0 = Wire.read();
    Y1 = Wire.read(); 
    Y1=Y1<<8;
    Y_out=Y0+Y1;
  }
  
  //------------------Z
  Wire.beginTransmission(ADXAddress); // transmit to device 
  Wire.write(Register_Z0);
  Wire.write(Register_Z1);
  Wire.endTransmission();
  Wire.requestFrom(ADXAddress,2); 
  
  if(Wire.available()<=2) {
    Z0 = Wire.read();
    Z1 = Wire.read(); 
    Z1=Z1<<8;
    Z_out=Z0+Z1;
  }
  
  Xa = X_out/256.0;
  Ya = Y_out/256.0;
  Za = Z_out/256.0;
  
  acceleration_magnitude = sqrt(pow(Xa,2) + pow(Ya, 2) + pow(Za, 2));
  
  Serial.print("aceleration magnitude= ");
  Serial.print(acceleration_magnitude);
  Serial.print("       ");
  
  gyro.readGyro(&gx,&gy,&gz);
  gx_rate = (gx-goffsetX) / 16.4;
  gy_rate = (gy-goffsetY) / 16.4;
  gz_rate = (gz-goffsetZ) / 16.4;
  
  gyro_magnitude = sqrt(pow(gx_rate, 2) + pow(gy_rate, 2) + pow(gz_rate, 2));
  
  Serial.print("gyro magnitude= ");
  Serial.print(gyro_magnitude);
  Serial.print("       ");
  
  if (acceleration_magnitude > 2 && gyro_magnitude > 0.4){
     digitalWrite(rele_solenoide_esq, HIGH);
     digitalWrite(rele_solenoide_dir, HIGH); 
  }
  
  delay(100);
}
