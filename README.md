# **Watch Me**

## A ESP8266 based watch that has the following:

#### 1) _A 0.98" LED screen communicating via I2C;_
#### 2) _A real time clock (RTC DS3231) communicating via I2C;_
#### 3) _A 3.7V LiPo battery;_
#### 4) _A 3.7V LiPo battery charger;_
#### 5) _An LC709203F LiPoly / LiIon Fuel Gauge and Battery Monitor;_
#### 6) _A step-up 3V to 5V 1A;_
#### 7) _A GY-521 MPU-6050 3 Axis Gyroscope and Accelerometer;_
#### 8) _A 3V buzzer;_
#### 9) _3 tack buttons;_

## <ins>**The functionality includes the following:**</ins>

#### * The first screen displays the main clock at rest, with the format hh:mm AM/PM
![PXL_20250519_205457624](https://github.com/user-attachments/assets/547338b1-fd94-47e9-895d-2206eb1b5d64)

#### 1) When the watch is first at rest, and not moving at all, the LED brightness level is very high;

#### 2) After a second, the LED brightness is set to low;

#### 3) When the watch is moved again, the LED brightness is set to high again;

#### 4) If the movement is enough, the clock screen changes to;
![PXL_20250519_205509609](https://github.com/user-attachments/assets/ef00a01f-e5ee-470d-8d53-a4184bebf713)

![PXL_20250519_205509165 MP](https://github.com/user-attachments/assets/e05f4207-6014-46f0-b6e2-5c1a2d730aec)

#### 5) If you press the button for the D5 pin, then you go to the next screen;
![PXL_20250519_223457380](https://github.com/user-attachments/assets/a2fc9f93-6531-4b29-b156-0d8570587295)

#### 6) The next screen reads the Gyroscope data and displays it;
![PXL_20250519_205207516](https://github.com/user-attachments/assets/45949adf-0ae0-4907-905c-121cdd459b27)

#### 7) The next screen reads the Accelerometer data and displays it;
![PXL_20250519_205219659 MP](https://github.com/user-attachments/assets/f8f9058f-9e96-4483-9f15-b9145c98ba36)

#### 8) The next screen is the beginning of a game of "Heads Up". I have not yet implemented the logic, however the plan is for the user to hold the device to their forehead and quickly flip forward or backwards, which will increase or decrease points, at the same time as the screen displays the next word the the user needs to guess;
![PXL_20250519_205229828 MP](https://github.com/user-attachments/assets/d0eb8033-2162-4a3f-91cf-828a2a40183f)

#### 9) The next screen reads the battery level and displays it, however I don't have a battery hooked up in the picture, but I did test it and it works great;
![PXL_20250519_205242337](https://github.com/user-attachments/assets/1903e8b8-567b-45db-a474-4b8f514f6c41)

#### 10) The next screen allows you to increase the hour by pressing the button for the D7 pin, which writes to the real time clock RTC (DS3231) and the time is maintained by a coin cell;
![PXL_20250519_205333220](https://github.com/user-attachments/assets/ad816934-e790-4ec4-a7a3-843cf2bffd00)

#### 11) The next screen allows you to increase the minutes by pressing the button for the D7 pin, which writes to the real time clock RTC (DS3231) and the time is maintained by a coin cell;
![PXL_20250519_205339630](https://github.com/user-attachments/assets/f684ecac-cf03-46cc-b72b-4ad7dc97df29)




