/* 
*
* ## upnpOS ##
* Micro Operating System for embedded systems with support for plug and play
* 
* FILE: sensor_button.ino -- Driver for a sensor with a button. This driver supports events
* 
* Version 0.1 - 2013
* 
*/

static int sb_register(char devAddr, char event, int address, void (*evt)(int devId, char eventId, void *data));

static Driver sb_driver;

static int sb_data[1];
static char sb_data_ready;

static void sb_init(){
	sb_driver.id = '3';
	sb_driver.open = sb_open;
	sb_driver.close = sb_close;
	sb_driver.read = sb_read;
	sb_driver.write = sb_write;
        sb_driver.recv = sb_recv_data;
        sb_driver.regist = sb_register;
        sb_driver.unregister = sb_unregister;
        sb_driver.control = sb_control;

        sb_data_ready = 0;

	registerDriver(&sb_driver);
}

static int sb_open(char devAddr){
  //Nothing to do
}

static int sb_close(char devAddr){
  //Nothing to do
}

static void *sb_read(char devAddr, char where){
        Wire.beginTransmission(devAddr);    
        Wire.write(0);                //Send operation (READ)
        Wire.write(where);            //Send address
        Wire.endTransmission();
        int tries = 0;
        
        while(tries <= 50 && sb_data_ready == 0)    // wait for response
	{ 
          delay(50);
          tries++;
        }
        
        if (sb_data_ready == 0)
          return NULL;
          
        sb_data_ready = 0;
        
  return sb_data;
}

static void sb_recv_data(){
  
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

   sb_data[0] = reading;
   sb_data_ready = 1;
}

static int sb_write(char devAddr, char where, char *data){
  //In this case write is not supported by the device
  return -1;
}

static int sb_register(char devAddr, char event, int address, void (*evt)(int devId, char eventId, void *data)){
  
    sb_driver.evt = evt;
  
    Wire.beginTransmission(devAddr);
    Wire.write(3);
    Wire.write(event);
    Wire.write(address);
    return Wire.endTransmission();
}

static int sb_unregister(char devAddr, char event){
    Wire.beginTransmission(devAddr);
    Wire.write(4);
    Wire.write(event);
    return Wire.endTransmission();
}

static int sb_control(char devAddr, char state){
  return 0;
}

