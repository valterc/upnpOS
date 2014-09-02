/* 
*
* ## upnpOS ##
* Micro Operating System for embedded systems with support for plug and play
* 
* FILE: sensor_distance.ino -- Driver for a distance sensor.
* 
* Version 0.1 - 2013
* 
*/

static int sd_register(char device_i2c, char event, int address, void (*evt)(int devId, char eventId, void *data));

static Driver sd_driver;

static int sd_data[1];
static char sd_data_ready;

static void sd_init(){
	sd_driver.id = '5';
	sd_driver.open = sd_open;
	sd_driver.close = sd_close;
	sd_driver.read = sd_read;
	sd_driver.write = sd_write;
        sd_driver.recv = sd_recv_data;
        sd_driver.regist = sd_register;
        sd_driver.unregister = sd_unregister;
        sd_driver.control = sd_control;

        sd_data_ready = 0;

	registerDriver(&sd_driver);
}

static int sd_open(char devAddr){
  //Nothing to do
}

static int sd_close(char devAddr){
  //Nothing to do	
}

static void *sd_read(char devAddr, char where){
        Wire.beginTransmission(devAddr);    
        Wire.write(0);                //Send operation (READ)
        Wire.write(where);            //Send address
        Wire.endTransmission();
        int tries = 0;
        
        while(tries <= 50 && sd_data_ready == 0)    // wait for response
	{ 
          delay(50);
          tries++;
        }
        
        if (sd_data_ready == 0)
          return NULL;
          
        sd_data_ready = 0;
        
  return sd_data;
}

static void sd_recv_data(){
  
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

   sd_data[0] = reading;
   sd_data_ready = 1;
}

static int sd_write(char devAddr, char where, char *data){
  Wire.beginTransmission(devAddr);
  Wire.write(1);
  Wire.write(where);
  Wire.write(data);
  return Wire.endTransmission();
}

static int sd_register(char device_i2c, char event, int address, void (*evt)(int devId, char eventId, void *data)){
    sd_driver.evt = evt;
    Wire.beginTransmission(device_i2c);
    Wire.write(3);
    Wire.write(event);
    Wire.write(address);
    return Wire.endTransmission();
}

static int sd_unregister(char device_i2c, char event){
  Wire.beginTransmission(device_i2c);
  Wire.write(4);
  Wire.write(event);
  return Wire.endTransmission();
}

static int sd_control(char device_i2c, char state){
  //Driver could send a control message to change device state, check connection, etc.
  return 0;
}



