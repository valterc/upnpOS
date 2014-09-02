upnpOS
======


####Micro Operating System for embedded systems with support for plug and play devices
-----------


The upnpOS is a tiny operating system developed for Atmel 8bit CPU based microcontrollers such as arduino.

System Architecture
-----

The system is split in 4 components:

![upnpOS Arch](https://raw.githubusercontent.com/valterc/upnpOS/master/readme/arch.PNG "OS Architecture")

- **upnpos** – OS Entry point
- **devman** – Device manager
- **osman** – Driver function mapping, context management
- **drivers** – Device drivers
 

___

###Usage

Create drivers for your devices and use the functions on the umain file to interact with the OS and the devices. 

 


##### Driver creation

The driver structure has the following functions:

```C
typedef struct driver{
   /** Driver ID */
  int id;    
   /** Driver Open function */
  int (*open)(char device_i2c);
   /** Driver Close function */
  int (*close)(char device_i2c);  
   /** Driver Read function */
  void *(*read)(char device_i2c, char where);  
   /** Driver Write function */
  int (*write)(char device_i2c, char where, char *data);    
   /** Register event function */
  int (*regist)(char device_i2c, char event, int address, void (*evt)(int devId, char eventId, void *data)); 
   /** Unregister event function */
  int (*unregister)(char device_i2c, char event);                                                   
   /** Control function */
  int (*control)(char device_i2c, char state);                                                       
   /** Event receive function */
  void (*evt)(int devId, char eventId, void *data); 
   /** Function to invoke when reading data is received*/
  void (*recv)(void);                                                                                   
}Driver;
   ```
Driver initialization and registration within the OS:

```C
static Driver drv_driver;

static void drv_init(){
	drv_driver.id = 'ID HERE';
	drv_driver.open = drv_open;
	drv_driver.close = drv_close;
	drv_driver.read = drv_read;
	drv_driver.write = drv_write;
    drv_driver.recv = drv_recv_data;
    drv_driver.regist = drv_register;
    drv_driver.unregister = drv_unregister;
    drv_driver.control = drv_control;

	registerDriver(&drv_driver);
}
```

##### Example usage:

```C

//umain.ino file

char state;
Dev *led;

//Initialize drivers, variables, etc.
void uinit() {
    led_drv_init(); //initialize the device driver
    state = 0;
}

//Drivers are now loaded
void usetup() {  

}

//Main loop
void uloop() {

    if (led != NULL){
        /*Send the state to the device, in this case lets send the light intensity to a led*/
        int result = d_write(led, 0, state);
        
        /*Check if the data was sent correctly*/
        if (result != 0){
          d_close(led);
          led = NULL;
        }
    } else {
        /*Open the device, NULL on failure*/
        led = d_open_addr(4);
    }
    
    state = (state + 1) % 5;
  
    delay(2500);
}

```

### License

MIT License, see the [LICENSE file](https://github.com/valterc/upnpOS/blob/master/LICENSE).