/*
 * iPhone Battery HDQ Status Example
 * 
 * Copyright (c) 2016 Joe Honold http://mozzwald.com
 *  
 * Requires the Arduino HDQ library from
 * https://github.com/mozzwald/HDQLib
 * 
 */

#include <HDQ.h>

HDQ HDQ(HDQ_DEFAULT_PIN); // Arduino digital pin 7

void setup() {
  Serial.begin(57600);

}

void loop() {
  uint8_t BYT1, BYT2;
  int dev, firm, hard, major, minor;
  
  /* Print all registers for testing purposes
  for (uint8_t jj = 0; jj < 0x3F; jj++) { 
      BYT1 = HDQ.read(jj);
      BYT2 = HDQ.read(jj+1);
      int total = word(BYT2, BYT1);
  
      Serial.print("Register 0x");
      Serial.print(jj, HEX);
      Serial.print(" / 0x");
      Serial.print(jj+1, HEX);
      Serial.print(": ");
      Serial.println(total);
  
      jj++;
    }
  */

  /* Get device type */
  BYT1 = HDQ.write(0x0, 0x01);
  BYT2 = HDQ.write(0x1, 0x00);
  BYT1 = HDQ.read(0x00);
  BYT2 = HDQ.read(0x01);
  dev = (BYT2*100)+BYT1;

/* Attempt to detect which iPhone battery is connected
   based on device, firmware and hardware versions 
   https://ripitapart.wordpress.com/2014/09/15/so-phone-me-maybe-a-list-of-iphone-batteries-with-gas-gauge-functionality/

   Devices (dev var):
      167 = ? Fake ?
      565 = bq27541 (3G, 3GS, 4G, 4GS)
      569 = bq27545 (5G, 5C, 5S)
      
   Firmware (firm var):
      123 = v1.17 (3GS)
      137 = v1.25 (4G)
      153 = v1.35 (4GS)
      316 = v3.10 (5G, 5C, 5S)
      502 = v5.02 (6G, 6GP)
      601 = v6.01 (6S, 6SP)

   Hardware (hard var):
      ??? = ???
      
*/
     
  if(BYT1 != 255 && BYT2 != 255){ // Check if HDQ capable battery is connected
    Serial.println("");
    //Serial.print(BYT2, HEX); Serial.println(BYT1, HEX);
    
    /* Get Firmware Version */
    BYT1 = HDQ.write(0x0, 0x02);
    BYT2 = HDQ.write(0x1, 0x00);
    BYT1 = HDQ.read(0x00);
    BYT2 = HDQ.read(0x01);
    firm = (BYT2*100)+BYT1;
  
    /* Get Hardware Version */
    BYT1 = HDQ.write(0x0, 0x03);
    BYT2 = HDQ.write(0x1, 0x00);
    BYT1 = HDQ.read(0x00);
    BYT2 = HDQ.read(0x01);
    hard = (BYT2*100)+BYT1;

    if(dev == 565){ // bq27541 (3G, 3GS, 4G, 4GS)
      if(firm == 123){
        Serial.println("iPhone 3GS Battery Detected");
      }else if(firm == 137){
        Serial.println("iPhone 4G Battery Detected");
      }else if(firm == 153){
        Serial.println("iPhone 4GS Battery Detected");
      }else{
        Serial.println("Unknown HDQ Battery Detected");
      }
    }else if(dev == 569){ // bq27545 (5G, 5C, 5S)
      if(hard == 167){
        Serial.println("iPhone 5S / 5C Battery Detected");
      }else{
        Serial.println("Unknown HDQ Battery Detected");
      }
    }else if(dev == 167){ // Fake?
      Serial.println("Unknown (Fake iPhone 5S/5C) HDQ Battery Detected");
    }else{
      Serial.println("Unknown HDQ Battery Detected");
    }

    /* Display Device */
    Serial.print("Device: ");
    switch(dev) {
      case 565:
        Serial.println("bq27541");
        break;
      case 569:
        Serial.println("bq27545");
        break;
      default:
        Serial.print("Unknown (");
        Serial.print(dev);
        Serial.println(")");
        break;
    }

    /* Display Firmware Version */
    major = firm/100;
    minor = firm-(major*100);
    Serial.print("Firmware: v");
    Serial.print(major, HEX);
    Serial.print(".");
    Serial.println(minor, HEX);

    /* Display Hardware Version */
    Serial.print("Hardware: 0x");
    Serial.println(hard, HEX);

    /* Current charge in mAH */
    BYT1 = HDQ.write(0x0, 0x01);
    BYT2 = HDQ.write(0x1, 0x00);
    BYT1 = HDQ.read(0x10);
    BYT2 = HDQ.read(0x11);
    Serial.print("Remaining Capacity: ");
    Serial.print(word(BYT2, BYT1), DEC);
    Serial.println(" mAH");
    
    /* Full charge capacity mAH */
    BYT1 = HDQ.write(0x0, 0x01);
    BYT2 = HDQ.write(0x1, 0x00);
    BYT1 = HDQ.read(0x12);
    BYT2 = HDQ.read(0x13);
    Serial.print("Full Charge Capacity: ");
    Serial.print(word(BYT2, BYT1), DEC);
    Serial.println(" mAH");
  
    /* Design capacity mAH */
    BYT1 = HDQ.write(0x0, 0x01);
    BYT2 = HDQ.write(0x1, 0x00);
    BYT1 = HDQ.read(0x3c);
    BYT2 = HDQ.read(0x3d);
    Serial.print("Design Capacity: ");
    Serial.print(word(BYT2, BYT1), DEC);
    Serial.println(" mAH");
    
    /* Time to Empty Minutes */
    BYT1 = HDQ.write(0x0, 0x01);
    BYT2 = HDQ.write(0x1, 0x00);
    BYT1 = HDQ.read(0x16);
    BYT2 = HDQ.read(0x17);
    int tto = word(BYT2, BYT1);
    Serial.print("Time to empty: ");
    if(tto == -1){
      Serial.println("N/A, not discharging");
    }else{
      Serial.print(tto, DEC);
      Serial.println(" minutes");
    }
    
    /* Time to Full Minutes */
    BYT1 = HDQ.write(0x0, 0x01);
    BYT2 = HDQ.write(0x1, 0x00);
    BYT1 = HDQ.read(0x18);
    BYT2 = HDQ.read(0x19);
    int ttf = word(BYT2, BYT1);
    Serial.print("Time to full: ");
    if(ttf == -1){
      Serial.println("N/A, not charging");
    }else{
      Serial.print(ttf, DEC);
      Serial.println(" minutes");
    }
  
    /* State of Charge % */
    BYT1 = HDQ.write(0x0, 0x01);
    BYT2 = HDQ.write(0x1, 0x00);
    BYT1 = HDQ.read(0x2c);
    BYT2 = HDQ.read(0x2d);
    Serial.print("State of Charge: ");
    Serial.print(word(BYT2, BYT1), DEC);
    Serial.println(char(37));

    /* Voltage mV */
    BYT1 = HDQ.write(0x0, 0x01);
    BYT2 = HDQ.write(0x1, 0x00);
    BYT1 = HDQ.read(8);
    BYT2 = HDQ.read(9);
    int mvolts = word(BYT2, BYT1);
    float volts = (mvolts*0.001);
    Serial.print("Battery Voltage: ");
    Serial.print(volts);
    Serial.print("V (");
    Serial.print(mvolts);
    Serial.println("mV)");
    
    /* Temperature (in Kelvin to C/F) */
    BYT1 = HDQ.write(0x0, 0x01);
    BYT2 = HDQ.write(0x1, 0x00);
    BYT1 = HDQ.read(6);
    BYT2 = HDQ.read(7);
    int tempK = word(BYT2, BYT1);
    float tempC = (tempK-2731)/10;
    float tempF = (1.8*((tempK*0.1)-273))+32;
    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.print(char(176));
    Serial.print("C / ");
    Serial.print(tempF);
    Serial.print(char(176));
    Serial.print("F / ");
    Serial.print(tempK);
    Serial.println(" raw");
    
    /* Charge Cycle Count */
    BYT1 = HDQ.write(0x0, 0x01);
    BYT2 = HDQ.write(0x1, 0x00);
    BYT1 = HDQ.read(0x2a);
    BYT2 = HDQ.read(0x2b);
    Serial.print("Charge Cycle Count: ");
    Serial.print(word(BYT2, BYT1), DEC);
    Serial.println(" times");
  }else{
    // HDQ Battery isn't connected
    Serial.println("Battery not detected");
  }
  
  delay(5000);  
  //Serial.println("");
}

