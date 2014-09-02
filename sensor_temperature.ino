/* 
*
* ## upnpOS ##
* Micro Operating System for embedded systems with support for plug and play
* 
* FILE: sensor_temperature.ino -- Driver for a temperature sensor.
* 
* Version 0.1 - 2013
* 
*/

static int st_register(char device_i2c, char event, int address, void (*evt)(int devId, char eventId, void *data));

static Driver st_driver;

static int st_data[1];
static char st_data_ready;

static void st_init(){
	st_driver.id = '1';
	st_driver.open = st_open;
	st_driver.close = st_close;
	st_driver.read = st_read;
	st_driver.write = st_write;
        st_driver.recv = st_recv_data;
        st_driver.regist = st_register;
        st_driver.unregister = st_unregister;
        st_driver.control = st_control;

        st_data_ready = 0;

	registerDriver(&st_driver);
}

static int st_open(char devAddr){
  //Nothing to do
}

static int st_close(char devAddr){
  //Nothing to do	
}

static void *st_read(char devAddr, char where){
        Wire.beginTransmission(devAddr);    
        Wire.write(0);                //Send operation (READ)
        Wire.write(where);            //Send address
        Wire.endTransmission();
        int tries = 0;
        
        while(tries <= 50 && st_data_ready == 0)    // wait for response
	{ 
          delay(50);
          tries++;
        }
        
        if (st_data_ready == 0)
          return NULL;
          
        st_data_ready = 0;
        
  return st_data;
}

static void st_recv_data(){
  
  int i = 0;
  int reading = 0;
  
  if(Wire.available() >= 2)   
  { 
    reading = Wire.read();     // receive high byte (overwrites previous reading)
    reading = reading << 8;    // shift high byte to be high 8 bits
    reading |= Wire.read();
  } else {
    return;
  }

   st_data[0] = reading;
   st_data_ready = 1;
}

static int st_write(char devAddr, char where, char *data){
  Wire.beginTransmission(devAddr);
  Wire.write(1);
  Wire.write(where);
  Wire.write(data);
  return Wire.endTransmission();
}

static int st_register(char device_i2c, char event, int address, void (*evt)(int devId, char eventId, void *data)){
  //Events not supported
  return -1;
}

static int st_unregister(char device_i2c, char event){
  //Events not supported
  return -1;
}

static int st_control(char device_i2c, char state){
  return 0;
}



