/* 
*
* ## upnpOS ##
* Micro Operating System for embedded systems with support for plug and play
* 
* FILE: z_led.ino -- Driver for a light emitter.
* 
* Version 0.1 - 2013
* 
*/

static int l_register(char device_i2c, char event, int address, void (*evt)(int devId, char eventId, void *data));

static Driver l_driver;

static int l_data[1];

void l_init(){
	l_driver.id = '4';
	l_driver.open = l_open;
	l_driver.close = l_close;
	l_driver.read = l_read;
	l_driver.write = l_write;
        l_driver.regist = l_register;
        l_driver.unregister = l_unregister;
        l_driver.control = l_control;

	registerDriver(&l_driver);
        l_data[0] = 0;
}

int l_open(char devAddr){
	
}

int l_close(char devAddr){
	
}

void *l_read(char devAddr, char where){
	return &l_data;
}

int l_write(char devAddr, char where, char *data){
  Wire.beginTransmission(devAddr);
  Wire.write(1);
  Wire.write(where);
  Wire.write(data);
  return Wire.endTransmission();
}

static int l_register(char device_i2c, char event, int address, void (*evt)(int devId, char eventId, void *data)){
  //Events not supported
  return -1;
}

static int l_unregister(char device_i2c, char event){
  //Events not supported
  return -1;
}

static int l_control(char device_i2c, char state){
  return 0;
}

