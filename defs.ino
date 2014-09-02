/* 
*
* ## upnpOS ##
* Micro Operating System for embedded systems with support for plug and play
* 
* FILE: defs.ino -- Definition of data structures used by the OS
* 
* Version 0.1 - 2013
* 
*/


/* Driver for a device */
typedef struct driver{
  int id;                                                                                                     /** Driver ID */
  int (*open)(char device_i2c);                                                                               /** Driver Open function */
  int (*close)(char device_i2c);                                                                              /** Driver Close function */
  void *(*read)(char device_i2c, char where);                                                                 /** Driver Read function */
  int (*write)(char device_i2c, char where, char *data);                                                      /** Driver Write function */
  int (*regist)(char device_i2c, char event, int address, void (*evt)(int devId, char eventId, void *data));  /** Driver Register event function */
  int (*unregister)(char device_i2c, char event);                                                             /** Driver Unregister event function */
  int (*control)(char device_i2c, char state);                                                                /** Driver Control function */
  void (*evt)(int devId, char eventId, void *data);                                                           /** Driver Event receive function */
  void (*recv)(void);                                                                                         /** Function to invoke when reading data is received*/
}Driver;

/* Connected device */
typedef struct dev{
  char isNull;        /** Indicates if the device is valid */
  char i2c_address;   /** I2C Device address */
  char driver_id;     /** Driver Id */
  Driver *driver;     /** Driver for the device */
  int state;          /** Device state */
  int address;        /** Device internal address */
        
  dev& operator=(const int){   /** For initialization with NULL */
    i2c_address = 0;
    driver_id = 0;
    state = 0;
    address = 0;
    driver = NULL;
    isNull = 1;
  }
        
}Device;


int DEV_CONNECTED = 0;
int DEV_SLEEPING = 1;


/* Opened device */
typedef struct rdev{
  char i2c_address;   /** Device i2c address */
  char data[16];      /** Device data cache. Can be used by the user/driver */
  Driver *driver;     /** Driver for controling the device */
  int state;          /** Device current state */
  int address;        /** Device internal address */
  char closed;        /** Indicates that this connection to the device is now closed */
        
}Dev;

