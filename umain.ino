/* 
*
* ## upnpOS ##
* Micro Operating System for embedded systems with support for plug and play
* 
* FILE: umain.ino -- User code entry point.
* 
* Version 0.1 - 2013
* 
*/


char state[2];
int counter;
byte registered = 0;
byte un = 0;
Dev *button;
Dev *buzzer;
Dev *temp;
Dev *led;
Dev *distance;


//Initialize drivers, variables, etc.
void uinit(){
  st_init();
  sb_init();
  sd_init();
  b_init();
  l_init();
  state[1] = 0;
}

//Drivers are now loaded
void usetup() {  

}
 
//Main loop
void uloop() {
  /*
  //Button event
  if (button != NULL && registered == 0){
    int result = d_register(button, 0, btnPress);
    
    Serial.print("Register result: ");
    Serial.println(result);
    
    registered = 1;
  } else {
    if (button == NULL)
      button = d_open_addr(1);
  }
  
  /*
  //Unregister event
  if (button != NULL && un == 1){
    d_unregister(button, 0);
    un = -1;
  }
  */
  
  /*
  //Button read
  if (button != NULL){
    void *data = d_read(button, 0);
    
    if (data == NULL){
      d_close(button);
      button = NULL;
    } else {
      int buttonState = ((int *)data)[0];
      Serial.print("Button state: ");
      Serial.println(buttonState);
      if (buttonState == 0){
        state[0] = (state[0] + 1) % 5;
      }
    }
  } else {
    button = d_open_addr(1);
  }
  */
  
  /*
  //Temperature read
  if (temp != NULL){
    void *data = d_read(temp, 0);
    
    if (data == NULL){
      d_close(temp);
      temp = NULL;
    } else {
      int temperature = ((int *)data)[0];
      Serial.print("Temperature: ");
      Serial.println(temperature);
    }
  } else {
    temp = d_open_addr(3);
  }
  */
  
  
  //LED write
  if (led != NULL){
    int result = d_write(led, 0, state);
    Serial.print("Write result: ");
    Serial.println(result);
    if (result != 0){
      d_close(led);
      led = NULL;
    }
  } else {
    led = d_open_addr(4);
  }
  
  
  /*
  //Buzzer write
  if (buzzer != NULL){
    int result = d_write(buzzer, 0, state);
    Serial.print("Write result: ");
    Serial.println(result);
    if (result != 0){
      d_close(buzzer);
      buzzer = NULL;
    }
  } else {
    buzzer = d_open_addr(2);
  }
  */
  
  /*
  //Distance event
  if (distance != NULL && registered == 0){
    int result = d_register(distance, 0, distanceWarning);
    
    Serial.print("Register result: ");
    Serial.println(result);
    
    registered = 1;
  } else {
    if (distance == NULL)
      distance = d_open_addr(5);
  }
  */
  /*
  //Unregister event
  if (distance != NULL && un == 1){
    d_unregister(distance, 0);
    un = -1;
  }
  */
  
  //Auto state switching
  counter++;
  if (counter > 5){
    counter = 0;
    state[0] = (state[0] + 1) % 5;
  }
  
  delay(500);

}


//Button Event handler
void btnPress(int devId, char eventId, void *data){
  
  Serial.println("Received Event");
  Serial.print("DevId: ");
  Serial.println(devId);
  Serial.print("event: ");
  Serial.println(eventId);
  Serial.print("Data: ");
  Serial.println(((int *)data)[0]);
  
  int buttonState = ((int *)data)[0];
  state[0] = (state[0] + 1) % 5;
  
  un = 1;
}

//Distance Event handler
void distanceWarning(int devId, char eventId, void *data){
  
  Serial.println("Received Event");
  
  Serial.print("DevId: ");
  Serial.println(devId);

  Serial.print("event: ");
  Serial.println(eventId);
  
  Serial.print("Data: ");
  
  
  char *data_ = (char *)data;
  
  int value = data_[0];     // receive high byte (overwrites previous reading)
    value = value << 8;    // shift high byte to be high 8 bits
    value |= data_[1];
  
  Serial.println(value);
  
  un = 1;
  
}
