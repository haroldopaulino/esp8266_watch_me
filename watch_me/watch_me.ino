#include <Wire.h>
//DISPLAY
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#include <Adafruit_MPU6050.h> //(Accel/Gyro)
#include <Adafruit_Sensor.h>

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//RTC
#define DS3231_I2C_ADDRESS 0x68
#include "RTClib.h"
RTC_DS1307 rtc;
char daysOfTheWeek[7][12] = {"Sunday","Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
DateTime currentDateTime;

//(Accel/Gyro)
Adafruit_MPU6050 mpu;

sensors_event_t
  currentAcceleration,
  currentRotation,
  currentTemperature,
  oldRotation;

/*displayModes:
 * 1 = clock
 * 2 = acceleration
 * 3 = gyroscope
 * 4 = temperature
 */
int displayMode = 1;
int currentDelay = 200;
int buzzer = 12;
int button = 14;
int buttonStatus = 0;
int contrastLevel = 1;
int inactivityTimeout = 5000; //5 SECONDS
int currentInactivityValue = 0;
String ampmMode = "";

void setup() {
  Serial.begin(115200);

  initDisplay();
  initRtc(); 
  initAccelGyroTemp();
  initButton();
  beep(50);
}

void loop() {
  getRtcData();
  getAccelGyroTemp();
  checkButtonStatus();
  updateDisplay();
}

void setContrast(String contrastInput){
  int contrastValue = 411;//THE "contrastValue" ranges from 1 to 411
  if (contrastInput == "low") {
    contrastValue = 1;
  }
  
  int prech;
  int brightness; 
  switch (contrastValue){
    case 001 ... 255:
      prech = 0;
      brightness = contrastValue;
      break;
    case 256 ... 411:
      prech = 16;
      brightness = contrastValue - 156;
      break;
    default:
      prech = 16;
      brightness = 255;
      break;
  }
    
  display.ssd1306_command(SSD1306_SETPRECHARGE);      
  display.ssd1306_command(prech);                            
  display.ssd1306_command(SSD1306_SETCONTRAST);         
  display.ssd1306_command(brightness);                           
}

void updateDisplay() {
  display.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);
  Serial.print("currentInactivityValue: ");
  Serial.print(currentInactivityValue);
  Serial.print("     inactivityTimeout: ");
  Serial.println(inactivityTimeout);
  if (currentInactivityValue > inactivityTimeout) {
    setContrast("low");
  } else {
    setContrast("high");
    display.dim(false);
  }
  
  switch(displayMode) {
    case 1 :
      displayClock();
      break;
    case 2 :
      displayAcceleration();
      break;
    case 3 :
      displayGyroscope();
      break;
    case 4 :
      displayTemperature();
      break;
  }
  display.display();
  delay(currentDelay);
}

void displayClock() {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(getRtcTime());

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(105, 6);
  display.println(ampmMode);

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 20);
  display.println(getRtcDate());

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 40);
  display.println(getRtcDayOfWeek());
}

void displayAcceleration() {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Acceleration (m/s^2)");
  
  display.setCursor(0, 20);
  display.println("Accel X:");
  display.setCursor(55, 20);
  display.println(currentAcceleration.acceleration.x);

  display.setCursor(0, 30);
  display.println("Accel Y:");
  display.setCursor(55, 30);
  display.println(currentAcceleration.acceleration.y);

  display.setCursor(0, 40);
  display.println("Accel Z:");
  display.setCursor(55, 40);
  display.println(currentAcceleration.acceleration.z);
  //Serial.println(" m/s^2");
}

void displayGyroscope() {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Gyroscope");

  display.setCursor(0, 20);
  display.println("Gyro X:");
  display.setCursor(50, 20);
  display.println(currentRotation.gyro.x);

  display.setCursor(0, 30);
  display.println("Gyro Y:");
  display.setCursor(50, 30);
  display.println(currentRotation.gyro.y);

  display.setCursor(0, 40);
  display.println("Gyro Z:");
  display.setCursor(50, 40);
  display.println(currentRotation.gyro.z);
}

void displayTemperature() {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Temperature");

  display.setCursor(0, 20);
  display.println("Temp in F:");
  display.setCursor(70, 20);
  display.println(currentTemperature.temperature * 1.8 + 32);
  
  display.setCursor(0, 30);
  display.println("Temp in C:");
  display.setCursor(70, 30);
  display.println(currentTemperature.temperature);
}

void getRtcData() {
  currentDateTime = rtc.now();
  /*
    YEAR = currentDateTime.year()
    MONTH = currentDateTime.month()
    DAY = currentDateTime.day()
    DAY OF THE WEEK = daysOfTheWeek[currentDateTime.dayOfTheWeek()]
    HOUR = currentDateTime.hour()
    MINUTE = currentDateTime.minute()
    SECOND = currentDateTime.second()
  */
}

void initRtc() {
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // <----------------------SET TIME AND DATE: YYYY,MM,DD,HH,MM,SS
    //rtc.adjust(DateTime(2016, 11, 19, 19, 45, 0));
  }
}

void initDisplay() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(500);
  display.clearDisplay();
}

void initAccelGyroTemp() {
  //The ACCEL-GYRO board has an ADO pin. By default, the I2C address is 0x68, which conflicts with the RTC in our project.
  //If we provide 3.3V to the ADO pin, it will "pull up" the pin, making the default I2C address 0x69
  if (!mpu.begin(0x69)) {
    Serial.println("Failed to find MPU6050 chip (Accel/Gyro/Temp)");
    return;
  }
  Serial.println("MPU6050 chip Found (Accel/Gyro/Temp)!");

  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  //Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
    case MPU6050_RANGE_2_G:
      //Serial.println("+-2G");
      break;
    case MPU6050_RANGE_4_G:
      //Serial.println("+-4G");
      break;
    case MPU6050_RANGE_8_G:
      //Serial.println("+-8G");
      break;
    case MPU6050_RANGE_16_G:
      //Serial.println("+-16G");
      break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  //Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
    case MPU6050_RANGE_250_DEG:
      //Serial.println("+- 250 deg/s");
      break;
    case MPU6050_RANGE_500_DEG:
      //Serial.println("+- 500 deg/s");
      break;
    case MPU6050_RANGE_1000_DEG:
      //Serial.println("+- 1000 deg/s");
      break;
    case MPU6050_RANGE_2000_DEG:
      //Serial.println("+- 2000 deg/s");
      break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  //Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
    case MPU6050_BAND_260_HZ:
      //Serial.println("260 Hz");
      break;
    case MPU6050_BAND_184_HZ:
      //Serial.println("184 Hz");
      break;
    case MPU6050_BAND_94_HZ:
      //Serial.println("94 Hz");
      break;
    case MPU6050_BAND_44_HZ:
      //Serial.println("44 Hz");
      break;
    case MPU6050_BAND_21_HZ:
      //Serial.println("21 Hz");
      break;
    case MPU6050_BAND_10_HZ:
      //Serial.println("10 Hz");
      break;
    case MPU6050_BAND_5_HZ:
      //Serial.println("5 Hz");
      break;
  }
}

void initBuzzer() {
  pinMode(buzzer, OUTPUT);
  buttonStatus = digitalRead(button);
  Serial.print("initButton -> Button status value: ");
  Serial.println(buttonStatus);
}

void initButton() {
  pinMode(button, INPUT);
  buttonStatus = digitalRead(button);
  Serial.print("initButton -> Button status value: ");
  Serial.println(buttonStatus);
}

void beep(unsigned char delayms) { //creating function
  analogWrite(buzzer, 20); //Setting pin to high
  delay(delayms); //Delaying
  analogWrite(buzzer ,0); //Setting pin to LOW
  delay(delayms); //Delaying
  
}

void checkButtonStatus() {
  //Serial.print("Button status value: ");
  //Serial.println(buttonStatus);
  if (buttonStatus != digitalRead(button)) {
    Serial.println("Button pressed");
    buttonStatus = digitalRead(button);
    if (buttonStatus == LOW) {
      Serial.println("Button status: HIGH");
      displayMode++;
      if (displayMode > 4) {
        displayMode = 1;
      }
    } else {
      Serial.println("Button status: LOWs");
    }
  }
}

void getAccelGyroTemp() {
  /* Get new sensor events with the readings */
  oldRotation = currentRotation;
  mpu.getEvent(&currentAcceleration, &currentRotation, &currentTemperature);

  int
    oldX = oldRotation.gyro.x * 10,
    oldY = oldRotation.gyro.y * 10,
    oldZ = oldRotation.gyro.z * 10,
    newX = currentRotation.gyro.x * 10,
    newY = currentRotation.gyro.y * 10,
    newZ = currentRotation.gyro.z * 10;

  Serial.print("oldX: ");
  Serial.print(oldX);
  Serial.print("     newX: ");
  Serial.println(newX);

  Serial.print("oldY: ");
  Serial.print(oldY);
  Serial.print("     newY: ");
  Serial.println(newY);

  Serial.print("oldZ: ");
  Serial.print(oldZ);
  Serial.print("     newZ: ");
  Serial.println(newZ);
  
  if ((oldX == newX &&
       oldY == newY &&
       oldZ == newZ) ||
      (((oldX + 1) == newX) ||
       ((oldY + 1) == newY) ||
       ((oldZ + 1) == newZ)) ||
      (((oldX - 1) == newX) ||
       ((oldY - 1) == newY) ||
       ((oldZ - 1) == newZ))) {
    if (currentInactivityValue < (inactivityTimeout + currentDelay)) {
      currentInactivityValue += currentDelay;
    }
  } else {
    currentInactivityValue = 0;
  }
  /*
  Serial.print("Acceleration X: ");
  Serial.print(currentAcceleration.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(currentAcceleration.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(currentAcceleration.acceleration.z);
  Serial.println(" m/s^2");

  Serial.print("Rotation X: ");
  Serial.print(currentRotation.gyro.x);
  Serial.print(", Y: ");
  Serial.print(currentRotation.gyro.y);
  Serial.print(", Z: ");
  Serial.print(currentRotation.gyro.z);
  Serial.println(" rad/s");

  Serial.print("Temperature: ");
  Serial.print(currentTemperature.temperature);
  Serial.println(" degC");

  Serial.println("");*/
}

String getRtcDate() {
  String output = "";
  char buffer[2];
  sprintf(buffer, "%02d", currentDateTime.month());
  output += buffer;
  output += "/";
  sprintf(buffer, "%02d", currentDateTime.day());
  output += buffer;
  output += "/";
  output += currentDateTime.year();
  return output;
}

String getRtcTime() {
  String output = "";
  char buffer[2];

  int hourValue = currentDateTime.hour();
  char ampm = 'A';
  if (hourValue > 11) {
      ampm = 'P';
   }
  if (hourValue > 12) {
    hourValue = hourValue - 12;
  }
  output += hourValue;
  output += ":";
  sprintf(buffer, "%02d", currentDateTime.minute());
  output += buffer;
  output += ":";
  sprintf(buffer, "%02d", currentDateTime.second());
  output += buffer;

  ampmMode = "";
  ampmMode += ampm;
  ampmMode += "M";
  return output;
}

String getRtcDayOfWeek() {
  return daysOfTheWeek[currentDateTime.dayOfTheWeek()];
}
