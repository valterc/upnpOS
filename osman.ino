/* 
*
* ## upnpOS ##
* Micro Operating System for embedded systems with support for plug and play
* 
* FILE: osman.ino -- OS Core function. Context switching, Driver functions mapping.
* 
* Version 0.1 - 2013
* 
*/


Dev *d_open(int device);
Dev *d_open_addr(int addr);
int d_close(Dev *device);
void *d_read(Dev *device, char where);
int d_write(Dev *device, char where, char* data);
void d_setState(Dev *device, int state);
void os_drv_recv(void);
int d_register(Dev *device, char event, void (*evt)(int devId, void *data));
int d_unregister(Dev *device, char event);
int d_control(Dev *device, char state);

static void setContextForRead(Dev *dev);
static void (*drv_recv)(void);


void init_os(){
	init_devman();
}

void os_drv_recv(){
  if (drv_recv != NULL) {
    drv_recv();
  } else {
    //No driver to receive data
    //Ignore data and empty I2C buffer
    while(Wire.available()){
      Wire.read();
    }
  }
}

static void setContextForRead(Dev *dev){
  
  while(Wire.available()){
    Wire.read();
  }
  
  drv_recv = dev->driver->recv;
}

static void restoreContextAfterRead(){
  
  while(Wire.available()){
    Wire.read();
  }
  
  drv_recv = NULL;
}

Dev *d_open(int device){
	Device *dev = deviceForAddress(device);
	if (dev == NULL)
		return NULL;

	if (dev->state == DEV_SLEEPING)
		return NULL;

	Dev *rdev = (Dev *)malloc(sizeof(Dev));

        if (rdev == NULL)
          return NULL;

	rdev->state = dev->state;
	rdev->driver = dev->driver;
	rdev->i2c_address = dev->i2c_address;
	rdev->address = dev->address;
	rdev->closed = 0;

	int result = rdev->driver->open(dev->i2c_address);

	if (result == -1)
		return NULL;

	return rdev;
}

Dev *d_open_addr(int addr){
	Device *dev = deviceForI2CAddress(addr);
	if (dev == NULL)
		return NULL;

	if (dev->state == DEV_SLEEPING)
		return NULL;

	Dev *rdev = (Dev *)malloc(sizeof(Dev));
        
        if (rdev == NULL)
          return NULL;

	rdev->state = dev->state;
	rdev->driver = dev->driver;
	rdev->i2c_address = dev->i2c_address;
	rdev->address = dev->address;
	rdev->closed = 0;

	int result = rdev->driver->open(dev->i2c_address);

	if (result == -1)
		return NULL;

	return rdev;
}

int d_close(Dev *device){
	device->closed = 1;
	int result = device->driver->close(device->i2c_address);
        free(device);
        return result;
}

void *d_read(Dev *device, char where){
      setContextForRead(device);
      void *data = device->driver->read(device->i2c_address, where);
      restoreContextAfterRead();
      
      //If read is invalid then is considered that the device is not working as expected or is disconnected and it is removed from the device list
      if (data == NULL){
        deleteDevice(device->address);
      }
      
      return data;
}

int d_write(Dev *device, char where, char* data){
    int result = device->driver->write(device->i2c_address, where, data);
    
    //If the data cannot reach the device [I2C error, Device disconnected] then the device is removed from the device list
    if (result > 1){
      deleteDevice(device->address);
    }
    
    return result;
}

int d_register(Dev *device, char event, void (*evt)(int devId, char eventId, void *data)){
  return device->driver->regist(device->i2c_address, event, device->address, evt);
}

int d_unregister(Dev *device, char event){
  return device->driver->unregister(device->i2c_address, event);
}

int d_control(Dev *device, char state){
  return device->driver->control(device->i2c_address, state);
}
