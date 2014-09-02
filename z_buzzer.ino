/* 
*
* ## upnpOS ##
* Micro Operating System for embedded systems with support for plug and play
* 
* FILE: z_buffer.ino -- Driver for a buzzer.
* 
* Version 0.1 - 2013
* 
*/

static int b_register(char device_i2c, char event, int address, void (*evt)(int devId, char eventId, void *data));

static Driver b_driver;

static int b_data[1];

void b_init(){
	b_driver.id = '2';
	b_driver.open = b_open;
	b_driver.close = b_close;
	b_driver.read = b_read;
	b_driver.write = b_write;
        b_driver.regist = b_register;
        b_driver.unregister = b_unregister;
        b_driver.control = b_control;

	registerDriver(&b_driver);
        b_data[0] = 0;
}

int b_open(char devAddr){
	
}

int b_close(char devAddr){
	
}

void *b_read(char devAddr, char where){
	return &b_data;
}

int b_write(char devAddr, char where, char *data){
  Wire.beginTransmission(devAddr);
  Wire.write(1);
  Wire.write(where);
  Wire.write(data);
  return Wire.endTransmission();
}

static int b_register(char device_i2c, char event, int address, void (*evt)(int devId, char eventId, void *data)){
  //Events not supported
  return -1;
}

static int b_unregister(char device_i2c, char event){
  //Events not supported
  return -1;
}

static int b_control(char device_i2c, char state){
  return 0;
}

