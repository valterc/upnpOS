/* 
*
* ## upnpOS ##
* Micro Operating System for embedded systems with support for plug and play
* 
* FILE: devman.ino -- OS Device manager
* 
* Version 0.1 - 2013
* 
*/

#include <Wire.h>
#include "logging.h"

int registerDriver(Driver *driver);
Device *deviceForAddress(int dev_address);
Device *deviceForI2CAddress(char dev_address);
void outputDeviceInfo(Device *d);
static int replaceDeviceDriver(Device *d, char dev_driver_id, char dev_address);
static int loadDevice(char dev_driver_id, char dev_address);
static Driver * loadDriverForDevice(char dev_driver_id);
static void handleNewConnection();

#define MAX_DEVS 64
static Device devices[MAX_DEVS];
static int deviceCount = 0;

#define MAX_DRIVERS 32
static Driver *drivers[MAX_DRIVERS];
static int driverCount = 0;


void init_devman(){
  
  for (int i = 0; i < MAX_DEVS; i++ ){
    devices[i] = NULL;
  }
  
  Wire.begin(0);
  Wire.onReceive(receiveEvent);
  
  #if ENA_LOG
    Serial.println("I2C begin");
  #endif
  
}

static void receiveEvent(int howMany)
{
  #if ENA_LOG
    Serial.println("Data received");
  #endif
  
  char received = -1;

  if(Wire.available())
  { 
    received = Wire.read();
  }
  
  switch (received){
    case '0':
      handleNewConnection();
      break;
    case '1':
      //process data
      os_drv_recv();
      break;
    case '2':
      //ignore data
      break;
    case '5':
      //event data
      invokeDriverEvent();
      break;
    default:
    #if ENA_LOG
        Serial.print(received);
        Serial.println("Invalid Operation");
    #endif
      break;
  }
    

}

static void handleNewConnection(){

  char received = 0;

  if(Wire.available())
  { 
    received = Wire.read();
  }
  
  char dev_address = received;
  
  if(Wire.available())
  { 
    received = Wire.read();
  }

  char dev_driver_id = received;
 
  Device *d = deviceForI2CAddress(dev_address);
  
  if (d != NULL){
    int result = replaceDeviceDriver(d, dev_driver_id, dev_address); 
    
    #if ENA_LOG
    if (result == 1){
        Serial.println("Dev replaced");
        Serial.print("Address: ");  
        Serial.println(dev_address, DEC);
        Serial.print("Driver: ");  
        Serial.println(dev_driver_id, DEC);
    } else {
        Serial.println("No drv for dev");
        Serial.print("Address: ");  
        Serial.println(dev_address, DEC);
        Serial.print("Driver: ");  
        Serial.println(dev_driver_id, DEC);
    }
    #endif
    
  }
   
  int result = loadDevice(dev_driver_id, dev_address);
  
  //outputDeviceInfo(&devices[deviceCount-1]);
  
  #if ENA_LOG
  if (result == 1){
      Serial.println("New dev con");
      Serial.print("Address: ");  
      Serial.println(dev_address, DEC);
      Serial.print("Driver: ");  
      Serial.println(dev_driver_id, DEC);
  } else {
      Serial.println("No drv for dev");
      Serial.print("Address: ");  
      Serial.println(dev_address, DEC);
      Serial.print("Driver: ");  
      Serial.println(dev_driver_id, DEC);  
  }
  #endif
  
}

static int replaceDeviceDriver(Device *d, char dev_driver_id, char dev_address){
  Driver *drv = (Driver *)loadDriverForDevice(dev_driver_id);

  if (drv == NULL)
    return -1;

  d->i2c_address = dev_address;
  d->driver = drv;
  d->state = DEV_CONNECTED;
  d->driver_id = dev_driver_id;
  d->isNull = 0;

  return 1;
}

static Driver * loadDriverForDevice(char dev_driver_id){
 
  for (int i = 0; i < driverCount; ++i)
  {
    if (drivers[i]->id == dev_driver_id)
      return drivers[i];
  }

  return NULL;
}

static int loadDevice(char dev_driver_id, char dev_address){

  int result = 0;

  Driver *d = (Driver *)loadDriverForDevice(dev_driver_id);

  if (d == NULL)
    return -1;

  devices[deviceCount].i2c_address = dev_address;
  devices[deviceCount].driver = d;
  devices[deviceCount].address = deviceCount;
  devices[deviceCount].state = DEV_CONNECTED;
  devices[deviceCount].driver_id = dev_driver_id;
  devices[deviceCount].isNull = 0;

  deviceCount++;

  return 1;
}

int registerDriver(Driver *driver){

  if (driver->open == NULL) return -1;
  if (driver->close == NULL) return -1;
  if (driver->read == NULL) return -1;
  if (driver->write == NULL) return -1;
  if (driver->control == NULL) return -1;
  if (driver->regist == NULL) return -1;
  if (driver->unregister == NULL) return -1;
  if (driver->recv == NULL) return -1;

  drivers[driverCount] = driver;
  driverCount++;
  
  #if ENA_LOG
    Serial.print("Driver '");
    Serial.print(driver->id);
    Serial.println("' Reg");
  #endif
  
}

Device *deviceForAddress(int dev_address){
  for (int i = 0; i < deviceCount; i++)
  {
      if (devices[i].isNull != 1)
        if (devices[i].address == dev_address)
          return &devices[i];
  }
  return NULL;
}

Device *deviceForI2CAddress(char dev_address){
  for (int i = 0; i < deviceCount; i++)
  {
    if (devices[i].isNull != 1)
      if (devices[i].i2c_address == dev_address)
        return &devices[i];
  }
  return NULL;
}

void outputDeviceInfo(Device *d){
  
  if (d == NULL)
    return;
  
  if (d->isNull)
    return;
  
  //Serial.println("DI:");
  Serial.print("I2C: "); 
  Serial.println(d->i2c_address);
  Serial.print("Drv: "); 
  Serial.println(d->driver_id);
  Serial.print("Add: "); 
  Serial.println(d->address);
}

void deleteDevice(int dev_address){
  Device *d = deviceForAddress(dev_address);
  
  if (d == NULL)
    return;
  
  d->isNull = 1;  
  
  #if ENA_LOG
    Serial.print("Device '");
    Serial.print(dev_address);
    Serial.println("' deleted!");
  #endif
  
}

void invokeDriverEvent(){
  char eventId = 0;

  if(Wire.available())
  { 
    eventId = Wire.read();
  }
  
  int devId = -1;
  
  if(Wire.available())
  { 
    devId = Wire.read();
  }
  
  Device *d = deviceForAddress(devId);
  
  if (d == NULL)
    return;
  
  char data[16];
  int i = 0;
  while(Wire.available() && i < 16){  
    data[i++] = Wire.read();
  }
  
  if (d->driver->evt != NULL){
    d->driver->evt(devId, eventId, data);
  }
  
  clearI2CBuffer();
}

void clearI2CBuffer(){
  while(Wire.available()){  
     Wire.read();
  }
}


