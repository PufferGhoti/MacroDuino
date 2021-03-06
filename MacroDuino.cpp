#include "MacroDuino.h"
#include "variables.h"

MacroDuino::MacroDuino() {

}

char *MacroDuino::control(char *returnData, char *commandString) {
  /******* NOTES  
   * for control functions with an eeprom.write a delay before returning is needed otherwise no data is returned
   * returnData needs to be setup as follows
   * int counter = 0;
   * returnData[counter] = value;
   * counter++
   * the reason being it makes it easier to change
   */

  String command = String(strtok(commandString, "/"));
  int counter;
  boolean check = false;
  char itoa_buffer[6];

  memset(returnData, 0, sizeof returnData);
  memset(itoa_buffer, 0, sizeof itoa_buffer);  
  
  #ifdef DEBUG_SERIAL
	Serial.print("Command Token: ");
	Serial.println(command);
  #endif
  
  /*
  identify
  */
  #ifdef COMMAND_IDENTIFY_ENABLED
  if(command == "identify") {    
    counter = 0;
     
    for(byte j = ADDRESS_START+1; j < ADDRESS_END; j++) {
      returnData[counter] = EEPROM.read(j);
      counter++;
    }    
    returnData[counter] = '\0';
    delay(1);    
    return returnData;    
  }
  #endif
  
  /*
  Setting Arduino Pin modes
  */
  #ifdef COMMAND_PINMODE_ENABLED
  if(command == "pinmode") {
    char *charpin = strtok(NULL, "/");
    char *charmode = strtok(NULL, "/");
    byte pin = atoi(charpin);
    byte mode = atoi(charmode);
    
    this->pinModeSet(pin, mode);
    
    #ifdef DEBUG_SERIAL
		Serial.print("Pin Mode Pin: ");
		Serial.println(pin);
		Serial.print("Pin Mode: ");
		Serial.println(mode);      
    #endif
    
    counter = 0;

    if(charpin[0] != NULL) {
      returnData[counter] = charpin[0];
      counter++;
    }
    if(charpin[1] != NULL) {
      returnData[counter] = charpin[1];
      counter++;
    }    
    returnData[counter] = '/';
    counter++;
    returnData[counter] = *charmode;
    delay(1);    

    return returnData;    
  }
  #endif
  
  /*
  Set pin status
  */
  #ifdef COMMAND_SETPIN_ENABLED
  if(command == "setpin") {
    char *charpin = strtok(NULL, "/");
    char *charpinstatus = strtok(NULL, "/");
    byte pin = atoi(charpin);
    byte pinstatus = atoi(charpinstatus);

    setPinStatus(pin, pinstatus);

    counter = 0;
    check = false;	

    if(charpin[0] != NULL) {
      returnData[counter] = charpin[0];
      counter++;
    }
    if(charpin[1] != NULL) {
      returnData[counter] = charpin[1];
      counter++;
    }    
    returnData[counter] = '/';
    counter++;    
    if(charpinstatus[0] != NULL) {
      returnData[counter] = charpinstatus[0];
      counter++;
    }
    if(charpinstatus[1] != NULL) {
      returnData[counter] = charpinstatus[1];
      counter++;
    } else {
      check = true;
    }
    if(charpinstatus[2] != NULL && check != true) {
      returnData[counter] = charpinstatus[2];
      counter++;
    }
    delay(1);

    return returnData;
  }
  #endif  
  
  /*
  reset all the macros
  */
  #ifdef COMMAND_RESETMACROS_ENABLED
  if(command == "resetmacros") {
    this->resetMacros();

    counter = 0;

    returnData[counter] = 'R';
    counter++;
    returnData[counter] = 'E';
    counter++;
    returnData[counter] = 'S';
    counter++;
    returnData[counter] = 'E';
    counter++;
    returnData[counter] = 'T';
    counter++;
    delay(1);    

    return returnData;
  }
  #endif	
  
  /* 
  analogread
  */
  #ifdef COMMAND_ANALOGPINREAD_ENABLED
  if(command == "analogpinread"){
    char *charwatch_pin = strtok(NULL, "/");
    int watch_pin = atoi(charwatch_pin);

    itoa(analogRead(watch_pin), itoa_buffer, 10);

    int counter = 0;
    boolean check = false;

    returnData[counter] = *charwatch_pin;
    counter++;
    returnData[counter] = '/';
    counter++;
    if(itoa_buffer[0] != NULL) {
      returnData[counter] = itoa_buffer[0]; 
      counter++;
    } else {
      returnData[counter] = 48;
      counter++;
      check = true;
    }
    if(itoa_buffer[1] != NULL && check != true) {
      returnData[counter] = itoa_buffer[1]; 
      counter++;
    } else {
      check = true;
    }
    if(itoa_buffer[2] != NULL && check != true) {
      returnData[counter] = itoa_buffer[2]; 
      counter++;
    } else {
      check = true;
    }
    if(itoa_buffer[3] != NULL && check != true) {
      returnData[counter] = itoa_buffer[3]; 
      counter++;
    } 
    else {
      check = true;
    }
    delay(1);   

    return returnData;
  }
  #endif
  
  #ifdef COMMAND_DIGITALPINREAD_ENABLED
  //TODO - not returning an address and won't return correct pin for values over 9
  if(command == "digitalpinread"){
    char *char_watch_pin = strtok(NULL, "/");
    byte watch_pin = atoi(char_watch_pin);

    itoa(digitalRead(watch_pin), itoa_buffer, 10);

    counter = 0;
    check = false;

    if(char_watch_pin[0] != NULL) {
      returnData[counter] = char_watch_pin[0];
      counter++;
    }
    if(char_watch_pin[1] != NULL) {
      returnData[counter] = char_watch_pin[1];
      counter++;
    } 
    else {
      check = true;
    }
    returnData[counter] = '/';
    counter++;
    returnData[counter] = itoa_buffer[0]; 
    counter++;    
    delay(1);   

    return returnData;
  }
  #endif		

  #if defined(_RTCLIB_H_) && defined(COMMAND_SETDS1307_ENABLED)
	if(command == "setds1307"){	
	  char *charyear = strtok(NULL, "/");
	  char *charmonth = strtok(NULL, "/");      
	  char *charday = strtok(NULL, "/");
	  char *charhour = strtok(NULL, "/");
	  char *charminute = strtok(NULL, "/");
	  char *charsecond = strtok(NULL, "/");

	  byte hour = atoi(charhour);
	  byte minute = atoi(charminute);
	  byte second = atoi(charsecond);
	  byte day = atoi(charday);
	  byte month = atoi(charmonth);
	  int year = atoi(charyear);

	  this->DS1307SetTime(year, month, day, hour, minute, second);

	  int counter = 0;
	  boolean check_hour = false;
	  boolean check_minute = false;
	  boolean check_second = false;
	  boolean check_day = false;
	  boolean check_dow = false;
	  boolean check_month = false;
	  boolean check_year = false;

	  returnData[counter] = 'S'; 
	  counter++;
	  returnData[counter] = 'E'; 
	  counter++;
	  returnData[counter] = 'T'; 
	  counter++;
	  returnData[counter] = 'T'; 
	  counter++;
	  returnData[counter] = 'I'; 
	  counter++;
	  returnData[counter] = 'M'; 
	  counter++;
	  returnData[counter] = 'E'; 
	  counter++;                                    
	  delay(1);   

	  return returnData;
	}	
  #endif

  #if defined(_RTCLIB_H_) && defined(COMMAND_GETDS1307UNITTIME_ENABLED)
	if(command == "getds1307unixtime"){
		char unixtime[11]; 
		DateTime now = rtc.now();

		snprintf(unixtime, sizeof unixtime, "%lu", (unsigned long)now.unixtime());
		strcpy(returnData, unixtime);
		
		return returnData;
	}
  #endif
  
  #ifdef COMMAND_CONFIGUREISE_ENABLED
	if(command == "configureise"){
	  char *charpin = strtok(NULL, "/");
	  char *charvalue1 = strtok(NULL, "/");
	  char *charvalue2 = strtok(NULL, "/");	  	  

	  byte pin = atoi(charpin);
	  byte value1 = atoi(charvalue1);	  
	  byte value2 = atoi(charvalue2);
	  
	  return (char*)this->configureISE(pin, value1, value2);		
	}  
  #endif
  
	#ifdef COMMAND_READISE_ENABLED  
	if(command == "readise"){
	  char *charpin = strtok(NULL, "/");
	  byte pin = atoi(charpin);	  
	  
	  float reading;
	  
	  reading = this->readISE(pin);
	  
	  #ifdef DEBUG_SERIAL
	  Serial.print("readise: ");
	  Serial.println(reading);
	  #endif
		
		return returnData;
	}  	
	#endif

//TODO
#ifdef TLC5940_H
  if(commandToken == 11) {
    int tlc_pin_num = atoi(strtok(NULL, "/"));
    int tlc_action = atoi(strtok(NULL, "/"));
    
		#ifdef DEBUG_SERIAL
		Serial.print("tlc_pin_num ");
		Serial.println(tlc_pin_num);
		Serial.print("tlc_action ");
		Serial.println(tlc_action);
    #endif
   
    controlTLC5940(tlc_pin_num, tlc_action); 
    
    counter = 0;

    returnData[counter] = '{';
    counter++;    
    returnData[counter] = 'S';
    counter++;    
    returnData[counter] = 'E';
    counter++;    
    returnData[counter] = 'T';
    counter++;    
    returnData[counter] = 'T';
    counter++;    
    returnData[counter] = 'L';
    counter++;    
    returnData[counter] = 'C';
    counter++;    
    returnData[counter] = '5';
    counter++;    
    returnData[counter] = '9';
    counter++;    
    returnData[counter] = '4';
    counter++;    
    returnData[counter] = '0';
    counter++;    
    returnData[counter] = '}';
    counter++;       
    delay(1); 

    return returnData;    
  }
#endif

	#if defined(OneWire_h) && defined(COMMAND_DISCOVERONEWIREDEVICES_ENABLED)
  if(command == "discoveronewiredevices"){		
    int devices_found = this->discoverOneWireDevices();

    itoa(devices_found, itoa_buffer, 10);

    int counter = 0;
    returnData[counter] = itoa_buffer[0];
    counter++;
    returnData[counter] = '\0';
    counter++;
    delay(1);
    
    return returnData;
  }
	#endif

	#if defined(OneWire_hh) && defined(COMMAND_GETDS18B20TEMP_ENABLED)
  if(command == "getds18b20temp"){	
    int mem_addresses = (onewire_addresses_memend - onewire_addresses_memstart) / onewire_addresses_bytes;  
    int counter = 0;
    int device_num = 0;
    returnData[counter] = '[';
    counter++;
    for(int i = 0; i < mem_addresses; i++){
      returnData[counter] = '{';
      counter++;
      returnData[counter] = 'D';
      counter++;         
      returnData[counter] = 'E';
      counter++;         
      returnData[counter] = 'V';
      counter++;
      returnData[counter] = 'I';
      counter++;
      returnData[counter] = 'C';
      counter++;
      returnData[counter] = 'E';
      counter++;
      returnData[counter] = ':';
      counter++;
      returnData[counter] = device_num + '0';
      counter++;
      returnData[counter] = ',';
      counter++;
      returnData[counter] = 'V';
      counter++;
      returnData[counter] = 'A';
      counter++;
      returnData[counter] = 'L';
      counter++;
      returnData[counter] = 'U';         
      counter++;
      returnData[counter] = 'E';         
      counter++;
      returnData[counter] = ':';                  
      counter++;
      returnData[counter] = '"';
      counter++;         
      for(int j = 0; j < 8; j++){
        char itoa_buffer[5];
        itoa(EEPROM.read((onewire_addresses_memstart + (i * onewire_addresses_bytes) + j)), itoa_buffer, 10);
        returnData[counter] = itoa_buffer[0];
        if(itoa_buffer[1] != NULL) {
          counter++;
          returnData[counter] = itoa_buffer[1]; 
        }
        counter++;
        returnData[counter] = '-';
        counter++;
      }
      counter--;
      returnData[counter] = '"';
      counter++;         
      returnData[counter] = '}';
      counter++;
      returnData[counter] = ',';
      counter++;
      returnData[counter] = '\r\n';
      counter++;
      device_num++;
    }
    returnData[counter] = ']';
    counter++;
    delay(1);

    return returnData;
  }
#endif 

	#if defined(OneWire_h) && defined(COMMAND_GETDS18B20TEMP_ENABLED)
  if(command == "getds18b20temp"){
    char *chartemp_sensor_num = strtok(NULL, "/");
    int temp_sensor_num = atoi(chartemp_sensor_num);
    int temp_reading = getDS18B20Temp(temp_sensor_num);

    itoa(temp_reading, itoa_buffer, 10);

    int counter = 0;
    boolean check_temp_sensor_num = false;
    boolean check_itoa_buffer = false;

    if(chartemp_sensor_num[0] != NULL) {
      returnData[counter] = chartemp_sensor_num[0];
      counter++;
    } 
    else {
      returnData[counter] = 48;
      counter++;
      check_temp_sensor_num = true;
    }
    if(chartemp_sensor_num[1] != NULL && check_temp_sensor_num != true) {
      returnData[counter] = chartemp_sensor_num[1];
      counter++;
    }
    returnData[counter] = '/';
    counter++;
    if(itoa_buffer[0] != NULL) {
      returnData[counter] = itoa_buffer[0];
      counter++;
    } 
    else {
      returnData[counter] = 48;
      counter++;
      check_itoa_buffer = true;
    }
    if(itoa_buffer[1] != NULL && check_itoa_buffer != true) {
      returnData[counter] = itoa_buffer[1];
      counter++;
    } 
    else {
      check_itoa_buffer = true;
    }
    if(itoa_buffer[2] != NULL && check_itoa_buffer != true) {
      returnData[counter] = itoa_buffer[2];
      counter++;
    } 
    else {
      check_itoa_buffer = true;
    }
    if(itoa_buffer[3] != NULL && check_itoa_buffer != true) {
      returnData[counter] = itoa_buffer[3];
      counter++;
    } 
    else {
      check_itoa_buffer = true;
    }
    if(itoa_buffer[4] != NULL && check_itoa_buffer != true) {
      returnData[counter] = itoa_buffer[4];
      counter++;
    } 
    else {
      check_itoa_buffer = true;
    }
    delay(1);

    return returnData;		
  }
#endif 

#ifdef PHENABLED
  if(commandToken == 28){
    char *char_watch_pin = strtok(NULL, "/");
    int watch_pin = atoi(char_watch_pin);

    //float pHReturned = getPHValue(watch_pin) * 100;
    char pHChar[5];

    //itoa (pHReturned, pHChar, 10);

    return pHChar;
  }
#endif

#ifdef MACROSENABLED
  if(commandToken == 31) {
    char *char_macro_number = strtok(NULL, "/");
    int macro_number = atoi(char_macro_number);

    int max_macros = ((macros_memend - macros_memstart) / macros_bytes) - 1;

    if(macro_number > max_macros) {
      int counter = 0;

      returnData[counter] = '"';
      counter++;      
      returnData[counter] = 'O';
      counter++;
      returnData[counter] = 'U';
      counter++;
      returnData[counter] = 'T';
      counter++;
      returnData[counter] = 'O';
      counter++;
      returnData[counter] = 'F';
      counter++;
      returnData[counter] = 'B';
      counter++;
      returnData[counter] = 'O';
      counter++;
      returnData[counter] = 'U';
      counter++;
      returnData[counter] = 'N';
      counter++;
      returnData[counter] = 'D';
      counter++;
      returnData[counter] = 'S';
      counter++;
      returnData[counter] = '"';
      counter++;      
    } 
    else {
      int counter = 0;
      boolean check_macro_number = false;
      int address_start = macros_memstart + (macros_bytes * macro_number);
      returnData[counter] = '{';
      counter++;
      returnData[counter] = '"';
      counter++;
      returnData[counter] = 'R';
      counter++;
      returnData[counter] = 'U';
      counter++;
      returnData[counter] = 'L';
      counter++;
      returnData[counter] = 'E';
      counter++;
      returnData[counter] = '"';
      counter++;      
      returnData[counter] = ':';
      counter++;      
      returnData[counter] = '"';
      counter++;
      if(char_macro_number[0] != NULL) {
        returnData[counter] = char_macro_number[0];
        counter++;
      } 
      else {
        returnData[counter] = 48;
        counter++;
        check_macro_number = true;
      }
      if(char_macro_number[1] != NULL && check_macro_number != true) {
        returnData[counter] = char_macro_number[1];
        counter++;
      } 
      else {
        check_macro_number = true;
      }
      returnData[counter] = ',';
      counter++;
      for(int i=0; i<macros_bytes; i++) {
        int address = address_start + i;
        int eeprom_value = EEPROM.read(address);

        if(i < 15) {
          char itoa_buffer[5];
          itoa(eeprom_value, itoa_buffer, 10);
          boolean check_itoa_buffer = false;

          if(itoa_buffer[0] != NULL) {
            returnData[counter] = itoa_buffer[0];
            counter++;
          } 
          else {
            returnData[counter] = 48;
            counter++;
            check_itoa_buffer = true;
          }
          if(itoa_buffer[1] != NULL && check_itoa_buffer != true) {
            returnData[counter] = itoa_buffer[1];
            counter++;  
          } 
          else {
            check_itoa_buffer = true;
          }
          if(itoa_buffer[2] != NULL && check_itoa_buffer != true) {
            returnData[counter] = itoa_buffer[2];
            counter++;  
          } 
          else {
            check_itoa_buffer = true;
          }
          returnData[counter] = ',';
          counter++;          
        } 
        else {
          if(eeprom_value != 0) {
            returnData[counter] = eeprom_value;
            counter++;
          }
        }
      }
      returnData[counter] = '"';
      counter++;      
      returnData[counter] = '}';
      counter++;      
    }  
    delay(1);

    return returnData;
  }
#endif

#ifdef MACROSENABLED
  if(commandToken == 32) {
    char *char_macro_number = strtok(NULL, "/");
    int macro_number = atoi(char_macro_number);

    int address = macros_memstart + (macros_bytes * macro_number);

    EEPROM.write(address, 0);

    int counter = 0;

    returnData[counter] = '"';
    counter++;
    returnData[counter] = 'D';
    counter++;
    returnData[counter] = 'E';
    counter++;
    returnData[counter] = 'L';
    counter++;
    returnData[counter] = 'E';
    counter++;
    returnData[counter] = 'T';
    counter++;
    returnData[counter] = 'E';
    counter++;
    returnData[counter] = 'M';
    counter++;
    returnData[counter] = 'A';
    counter++;
    returnData[counter] = 'C';
    counter++;
    returnData[counter] = 'R';
    counter++;
    returnData[counter] = 'O';
    counter++;
    returnData[counter] = '"';
    counter++;    
    delay(1);

    return returnData;
  }  
#endif

#ifdef MACROSENABLED
  if(commandToken == 5) {
    char *macro_name = strtok(NULL, "/");
    int arg1 = atoi(strtok(NULL, "/"));
    int arg2 = atoi(strtok(NULL, "/"));
    int arg3 = atoi(strtok(NULL, "/"));
    int arg4 = atoi(strtok(NULL, "/"));
    int arg5 = atoi(strtok(NULL, "/"));
    int arg6 = atoi(strtok(NULL, "/"));
    int arg7 = atoi(strtok(NULL, "/"));
    int arg8 = atoi(strtok(NULL, "/"));
    int arg9 = atoi(strtok(NULL, "/"));
    int arg10 = atoi(strtok(NULL, "/"));
    int arg11 = atoi(strtok(NULL, "/"));   

    macroSet(macro_name, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11);

    counter = 0;

    returnData[counter] = '"';
    counter++;
    returnData[counter] = 'S';
    counter++;
    returnData[counter] = 'E';
    counter++;
    returnData[counter] = 'T';
    counter++;
    returnData[counter] = 'M';
    counter++;
    returnData[counter] = 'A';
    counter++;
    returnData[counter] = 'C';
    counter++;
    returnData[counter] = 'R';
    counter++;
    returnData[counter] = 'O';
    counter++;
    returnData[counter] = '"';
    counter++;     
    delay(1);

    return returnData;
  }
#endif

  #ifdef PCF8574AENABLED && TwoWire_h
  if(commandToken == 6) {
    byte device = atoi(strtok(NULL, "/"));
    byte pin = atoi(strtok(NULL, "/"));
    byte action = atoi(strtok(NULL, "/"));

    //void controlPCF8574AA(byte device, byte pin_status, byte pin) {

    controlPCF8574A(device, action, pin);

    counter = 0;

    returnData[counter] = '{';
    counter++;  
    returnData[counter] = '"';
    counter++;
    returnData[counter] = 'S';
    counter++;
    returnData[counter] = 'E';
    counter++;
    returnData[counter] = 'T';
    counter++;
    returnData[counter] = 'P';
    counter++;
    returnData[counter] = 'C';
    counter++;
    returnData[counter] = 'F';
    counter++;
    returnData[counter] = '8';
    counter++;
    returnData[counter] = '5';
    counter++;
    returnData[counter] = '7';
    counter++;     
    returnData[counter] = '4';
    counter++;          
    returnData[counter] = '"';
    counter++;          
    returnData[counter] = ':';
    counter++;  
    returnData[counter] = '"';
    counter++;  
    returnData[counter] = 'Y';
    counter++;      
    returnData[counter] = '"';
    counter++;          
    returnData[counter] = '}';
    counter++;      
    delay(1);

    return returnData;
  }   
  #endif

  memset(commandString, 0, sizeof commandString);  	  
   
}

float c2f(float cel) {
  return (cel * (9.0/5.0)) + (float)3200;
}

int MacroDuino::highValue(int value) {
  return value / 256;
}

int MacroDuino::lowValue(int value) {
  return value % 256; 
}

int MacroDuino::combineValue(unsigned int lb, unsigned int hb) {
  return ((hb * 256) + lb);
}


void MacroDuino::resetMacros(){
  for(int i = MACROS_START; i <= MACROS_END; i++){
    EEPROM.write(i, 0);
  }
}

void MacroDuino::pinModeSet(byte pin, byte mode) {
  if((DIGITAL_PIN_START + pin) <= DIGITAL_PIN_END) {
    EEPROM.write(DIGITAL_PIN_START+pin, mode);
    pinMode(pin, mode);
  }
}

void MacroDuino::setPinStatus(byte pin, byte pinstatus) {
  if(pinstatus <= 1) {
    digitalWrite(pin, pinstatus);
  }
  else if(pinstatus > 1) {
    analogWrite(pin, pinstatus);
  }
}

#ifdef PCF8574AENABLED && TwoWire_h
void controlPCF8574A(byte device, byte pin_status, byte pin) {
  if(pin_status == 0) {
    pcf8574a_states[device] |= (1 << pin);
    Wire.beginTransmission(pcf8574a_addresses[device]);
    Wire.write(pcf8574a_states[device]);
    Wire.endTransmission();
  }
  else if(pin_status == 1) {
    pcf8574a_states[device] &= ~(1 << pin);
    Wire.beginTransmission(pcf8574a_addresses[device]);
    Wire.write(pcf8574a_states[device]);
    Wire.endTransmission();    
  }
}
#endif

//TODO - intelligently handle numbers > 255 and store them.
#ifdef MACROSENABLED
void macroSet(char *macro_name,  int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11){
  int arg12;
  int arg13;
  int arg14;
  int memstart;
  for(int i=0; i<((MACROS_END - MACROS_START)/MACRO_BYTE_LENGTH); i++) {
    if(EEPROM.read((MACROS_START + (i * MACRO_BYTE_LENGTH))) == 0) {
      memstart = MACROS_START + (i * MACRO_BYTE_LENGTH);
      break;
    }
  }

  if(arg1 > 255) {
    arg14 = highValue(arg1);    
    arg1 = lowValue(arg1);
  }
  else if(arg2 > 255) {
    arg14 = highValue(arg2);    
    arg2 = lowValue(arg2);    
  }
  else if(arg3 > 255) {
    arg14 = highValue(arg3);    
    arg3 = lowValue(arg3);    
  }
  else if(arg4 > 255) {
    arg14 = highValue(arg4);    
    arg4 = lowValue(arg4);    
  }
  else if(arg5 > 255) {
    arg14 = highValue(arg5);    
    arg5 = lowValue(arg5);
  }
  else if(arg6 > 255) {
    arg14 = highValue(arg6);    
    arg6 = lowValue(arg6);    
  }
  else if(arg7 > 255) {
    arg14 = highValue(arg7);    
    arg7 = lowValue(arg7);    
  }
  else if(arg8 > 255) {
    arg14 = highValue(arg8);    
    arg8 = lowValue(arg8);
  }    

  EEPROM.write(memstart, 1);
  EEPROM.write(memstart + 1, arg1);  
  EEPROM.write(memstart + 2, arg2);
  EEPROM.write(memstart + 3, arg3);
  EEPROM.write(memstart + 4, arg4);
  EEPROM.write(memstart + 5, arg5);
  EEPROM.write(memstart + 6, arg6);
  EEPROM.write(memstart + 7, arg7);
  EEPROM.write(memstart + 8, arg8);
  EEPROM.write(memstart + 9, arg9);
  EEPROM.write(memstart + 10, arg10);
  EEPROM.write(memstart + 11, arg11);
  EEPROM.write(memstart + 12, arg12);
  EEPROM.write(memstart + 13, arg13);
  EEPROM.write(memstart + 14, arg14);  

  int counter = 0;
  boolean check = false;
  for(int i=15; i<= 19; i++) {
    if(macro_name[counter] != NULL && check != true) {
      EEPROM.write(memstart + i, macro_name[counter]);
      counter++;
    } 
    else if(macro_name[counter] == NULL && check == false) {
      EEPROM.write(memstart + i, 0);      
      check = true;
    }
  } 
}
#endif

#ifdef DIGITALPINSENABLED
void runDigitalMacro(unsigned int mem_address){
  if(digitalRead(EEPROM.read(mem_address + 2)) == EEPROM.read(mem_address + 3)) {
    if(EEPROM.read(mem_address + 5) <= 1) {
      if(EEPROM.read(mem_address + 4) <= 13) {  
        digitalWrite(EEPROM.read(mem_address + 4), EEPROM.read(mem_address + 5));
      } 
      else if(EEPROM.read(mem_address + 4) == 254) { //pcf8574
        #ifdef PCF8574AENABLED && TwoWire_h
          controlPCF8574A(EEPROM.read(mem_address + 6), EEPROM.read(mem_address + 5), EEPROM.read(mem_address + 7));
        #endif        
      }
    }
    else{
      analogWrite(EEPROM.read(mem_address + 4), EEPROM.read(mem_address + 5));		
    }
  }
}
#endif

#ifdef ANALOGENABLED
void runAnalogMacro(unsigned int mem_address){
  if(EEPROM.read(mem_address + 4) == 1) {
    if(analogRead(EEPROM.read(mem_address + 2)) < combineValue(EEPROM.read(mem_address + 3), EEPROM.read(mem_address + 14))) {
      if(EEPROM.read(mem_address + 6) <= 1) {
        if(EEPROM.read(mem_address + 5) <= 13) {
          digitalWrite(EEPROM.read(mem_address + 5), EEPROM.read(mem_address + 6));
        }
        else if(EEPROM.read(mem_address + 5) == 254) { //pcf8574
          #ifdef PCF8574AENABLED && TwoWire_h
            controlPCF8574A(EEPROM.read(mem_address + 7), EEPROM.read(mem_address + 6), EEPROM.read(mem_address + 8));
          #endif          
        }
      }
      else if(EEPROM.read(mem_address + 6) > 1) {
        analogWrite(EEPROM.read(mem_address + 5), EEPROM.read(mem_address + 6));
      }
    }
  }
  else if(EEPROM.read(mem_address + 4) == 2) {
    if(analogRead(EEPROM.read(mem_address + 2)) > combineValue(EEPROM.read(mem_address + 3), EEPROM.read(mem_address + 14))) {
      if(EEPROM.read(mem_address + 6) <= 1) {
        if(EEPROM.read(mem_address + 5) <= 13) {
          digitalWrite(EEPROM.read(mem_address + 5), EEPROM.read(mem_address + 6));
        }
        else if(EEPROM.read(mem_address + 5) == 254) { //pcf8574
          #ifdef PCF8574AENABLED && TwoWire_h
            controlPCF8574A(EEPROM.read(mem_address + 7), EEPROM.read(mem_address + 6), EEPROM.read(mem_address + 8));
          #endif          
        }
      }
      else if(EEPROM.read(mem_address + 6) > 1) {
        analogWrite(EEPROM.read(mem_address + 5), EEPROM.read(mem_address + 6));
      }
    }	
  }
  else if(EEPROM.read(mem_address + 4) == 3) {
    if(analogRead(EEPROM.read(mem_address + 2)) == combineValue(EEPROM.read(mem_address + 3), EEPROM.read(mem_address + 14))) {
      if(EEPROM.read(mem_address + 6) <= 1) {
        if(EEPROM.read(mem_address + 5) <= 13) {
          digitalWrite(EEPROM.read(mem_address + 5), EEPROM.read(mem_address + 6));
        }
        else if(EEPROM.read(mem_address + 5) == 254) { //pcf8574
          #ifdef PCF8574AENABLED && TwoWire_h
            controlPCF8574A(EEPROM.read(mem_address + 7), EEPROM.read(mem_address + 6), EEPROM.read(mem_address + 8));
          #endif          
        }
      }
      else if(EEPROM.read(mem_address + 6) > 1) {
        analogWrite(EEPROM.read(mem_address + 5), EEPROM.read(mem_address + 6));
      }
    }
  }
}
#endif

#ifdef _RTCLIB_H_
void MacroDuino::DS1307SetTime(int year, byte month, byte day, byte hour, byte minute, byte second) {
  rtc.adjust(DateTime(year, month, day, hour, minute, second));
}
#endif


//TODO
// add in support for on state and off state
#ifdef DS1307_h
void runDS1307Macro(unsigned int mem_address){
  long time_start;
  long time_stop;
  long rtc_time;		

  rtc_time = (RTC.get(DS1307_HR,true) * 60 * 60) + (RTC.get(DS1307_MIN,true) * 60);
  time_start = (EEPROM.read(mem_address + 2) * 60 * 60) + (EEPROM.read(mem_address + 3) * 60);
  time_stop = (EEPROM.read(mem_address + 4) * 60 * 60) + (EEPROM.read(mem_address + 5) * 60);

  if(RTC.get(DS1307_DOW, false) == EEPROM.read(mem_address + 6) || EEPROM.read(mem_address + 6) == 0) {	
    if(rtc_time >= time_start && rtc_time <= time_stop) {
      if(EEPROM.read(mem_address + 8) <= 1) {
        if(EEPROM.read(mem_address + 7) <= 13) {
          digitalWrite(EEPROM.read(mem_address + 7), EEPROM.read(mem_address + 8));
        } else if(EEPROM.read(mem_address + 7) == 254) { //pcf8574
          #ifdef PCF8574AENABLED
            controlPCF8574A(EEPROM.read(mem_address + 10), EEPROM.read(mem_address + 8), EEPROM.read(mem_address + 11));
          #endif
        }
      } else {
        if(rtc_time < (time_start + EEPROM.read(mem_address + 9))) { //fade in
          if(EEPROM.read(mem_address + 7) <= 13) {
            int pwm_value = (rtc_time - time_start) * (EEPROM.read(mem_address + 8) / EEPROM.read(mem_address + 9));
            #ifdef DEBUG_SERIAL
						Serial.print("PWM Fade In: ");
						Serial.println(pwm_value);
            #endif
            analogWrite(EEPROM.read(mem_address + 7), pwm_value);
          }
        } else if(rtc_time > (time_stop - EEPROM.read(mem_address + 9))) { //fade out
          int pwm_value = (time_stop - rtc_time) * (EEPROM.read(mem_address + 8) / EEPROM.read(mem_address + 9));
          #ifdef DEBUG_SERIAL
					Serial.print("PWM Fade Out: ");        
					Serial.println(pwm_value);
          #endif        
          analogWrite(EEPROM.read(mem_address + 7), pwm_value);
        } else {
          #ifdef DEBUG_SERIAL
					Serial.print("PWM On at: ");
					Serial.println(EEPROM.read(mem_address + 8));
          #endif
          analogWrite(EEPROM.read(mem_address + 7), EEPROM.read(mem_address + 8));
        }

      }
    } else if(rtc_time > time_stop) {
      digitalWrite(EEPROM.read(mem_address + 7), 0);
      #ifdef PCF8574AENABLED
        controlPCF8574A(EEPROM.read(mem_address + 10), 0, EEPROM.read(mem_address + 11));    
      #endif
    }
  }
}
#endif

#if defined(OneWire_h) && defined(COMMAND_DISCOVERONEWIREDEVICES_ENABLED)
int MacroDuino::discoverOneWireDevices() {
  OneWire ds(ONEWIRE_PIN);

  byte i;
  byte present = 0;
  byte data[12];
  byte addr[8];
  unsigned int d = 0;
  unsigned int e;

  while(ds.search(addr)) {
    e = ONEWIRE_ADDRESSES_START + (d * ONEWIRE_ADDRESSES_BYTES);
    
    for( i = 0; i < 8; i++) {
      EEPROM.write((e + i), addr[i]);
      #ifdef DEBUG_SERIAL
      Serial.print(addr[i], HEX);
      #endif
    }
    #ifdef DEBUG_SERIAL
    Serial.println();
    #endif

    if ( OneWire::crc8( addr, 7) != addr[7]) {
      return 0;
    }
    d++;
  }
  EEPROM.write(NUM_ONEWIRE_DEVICES, d);

  ds.reset_search();
  return d;
}
#endif

#if defined(OneWire_h) && defined(COMMAND_GETDS18B20TEMP_ENABLED)
int MacroDuino::getDS18B20Temp(int device_num) {
  OneWire ds(ONEWIRE_PIN);

  byte i;
  byte present = 0;
  byte data[12];
  byte addr[8];
  int HighByte, LowByte, TReading, SignBit, Tc_100, Whole, Fract;

  for(i = 0; i < 8; i++){
    addr[i] = EEPROM.read((ONEWIRE_ADDRESSES_START + (ONEWIRE_ADDRESSES_BYTES * device_num) + i)); 
  }

	#ifdef DEBUG_SERIAL
	if ( OneWire::crc8( addr, 7) != addr[7]) {
		Serial.print("CRC is not valid!\n");
	}
	#endif

  if ( addr[0] != 0x28) {
		#ifdef DEBUG_SERIAL
    Serial.print("Device is not a DS18S20 family device.\n");
    #endif
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44,1);

#if ONEWIRE_PARASITIC_POWER_MODE == 1	
  delay(1000);
#endif

  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);

  for ( i = 0; i < 9; i++) {
    data[i] = ds.read();
  }

  LowByte = data[0];
  HighByte = data[1];
  TReading = (HighByte << 8) + LowByte;
  SignBit = TReading & 0x8000;  // test most sig bit
  if (SignBit) {
    TReading = (TReading ^ 0xffff) + 1; // 2's comp
  }
  Tc_100 = (6 * TReading) + TReading / 4;    // multiply by (100 * 0.0625) or 6.25

#ifndef CELSIUS
  Tc_100 = c2f(Tc_100);
#endif 

  return Tc_100;
}
#endif

#ifdef ORPENABLED
int getORPValue(unsigned int pin) {
  int analogreadings;
  int millivolts;
  unsigned int i;

  analogreadings = 0;
  for(i = 1; i < 50; i++){
    analogreadings += analogRead(pin);
    // give analog pin time to recover
    delay(10);
  }
  analogreadings /= i;

  //millivolts = ((analogreadings * ARDUINO_VOLTAGE) / 1024) * 1000;
  return millivolts; 
}
#endif

#ifdef OneWire_hh && EEPROM_h 
void runDS18B20Macro(int mem_address) {
  int temp;

  temp = getDS18B20Temp(EEPROM.read(mem_address + 2));

  if(EEPROM.read(mem_address + 3) == 1) {
    if(temp < ((EEPROM.read(mem_address + 4) * 100) + EEPROM.read(mem_address + 5))) {
      if(EEPROM.read(mem_address + 7) <= 1) {
        if(EEPROM.read(mem_address + 6) <= 13) {
          digitalWrite(EEPROM.read(mem_address + 6), EEPROM.read(mem_address + 7));
        }
        else if(EEPROM.read(mem_address + 6) == 254) { //pcf8574
          #ifdef PCF8574AENABLED && TwoWire_h
            controlPCF8574A(EEPROM.read(mem_address + 8), EEPROM.read(mem_address + 7), EEPROM.read(mem_address + 9));
          #endif
        }
      } 
      else {
        analogWrite(EEPROM.read(mem_address + 6), EEPROM.read(mem_address + 7));			
      }
    }
  } 
  else if(EEPROM.read(mem_address + 3) == 2) {
    if(temp > ((EEPROM.read(mem_address + 4) * 100) + EEPROM.read(mem_address + 5))) {
      if(EEPROM.read(mem_address + 7) <= 1) {
        if(EEPROM.read(mem_address + 6) <= 13) {
          digitalWrite(EEPROM.read(mem_address + 6), EEPROM.read(mem_address + 7));
        }
        else if(EEPROM.read(mem_address + 6) == 254) { //pcf8574
          #ifdef PCF8574AENABLED && TwoWire_h
            controlPCF8574A(EEPROM.read(mem_address + 8), EEPROM.read(mem_address + 7), EEPROM.read(mem_address + 9));
          #endif
        }
      } 
      else {
        analogWrite(EEPROM.read(mem_address + 6), EEPROM.read(mem_address + 7));			
      }
    }	
  } 
  else if(EEPROM.read(mem_address + 3) == 3) {
    if(temp == ((EEPROM.read(mem_address + 4) * 100) + EEPROM.read(mem_address + 5))) {
      if(EEPROM.read(mem_address + 7) <= 1) {
        if(EEPROM.read(mem_address + 6) <= 13) {
          digitalWrite(EEPROM.read(mem_address + 6), EEPROM.read(mem_address + 7));
        }
        else if(EEPROM.read(mem_address + 6) == 254) { //pcf8574
          #ifdef PCF8574AENABLED && TwoWire_h
            controlPCF8574A(EEPROM.read(mem_address + 8), EEPROM.read(mem_address + 7), EEPROM.read(mem_address + 9));
          #endif
        }
      } 
      else {
        analogWrite(EEPROM.read(mem_address + 6), EEPROM.read(mem_address + 7));			
      }
    }	
  }
}
#endif  

#ifdef TLC5940_H
void controlTLC5940(int tlc_pin_num, int tlc_action) {
  Tlc.set(tlc_pin_num, tlc_action);
  Tlc.update();
  delay(100);
}
#endif

void runMacros() {
  int macro_mem_address_start;
  for(unsigned int i = 0; i < ((MACROS_END - MACROS_START) / MACRO_BYTE_LENGTH); i++) {
    macro_mem_address_start = MACROS_START + (i * MACRO_BYTE_LENGTH);
    if(EEPROM.read(macro_mem_address_start) == 1) {    
      if(EEPROM.read(macro_mem_address_start + 1) == 1) {
        #ifdef DIGITALPINSENABLED
          runDigitalMacro(macro_mem_address_start);
        #endif
      }
      else if(EEPROM.read(macro_mem_address_start + 1) == 2) {
        #ifdef ANALOGENABLED
          runAnalogMacro(macro_mem_address_start);
        #endif  
      }
      else if(EEPROM.read(macro_mem_address_start + 1) == 3) {
#ifdef DS1307_h
        runDS1307Macro(macro_mem_address_start);
#endif
      }
      else if(EEPROM.read(macro_mem_address_start + 1) == 4) {
        #ifdef OneWire_hh && DS18B20ENABLED
          runDS18B20Macro(macro_mem_address_start);
        #endif
      }
    }
  }
}

/**************************************************************************/
/*!
    @brief  Tries to read the IP address and other connection details
*/
/**************************************************************************/
bool MacroDuino::displayConnectionDetails(Adafruit_CC3000& cc3000) {
  uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv;
  
  if(!cc3000.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv))
  {
  	#ifdef DEBUG_SERIAL
    Serial.println(F("Unable to retrieve the IP Address!\r\n"));
    #endif
    return false;
  }
  else
  {
  	#ifdef DEBUG_SERIAL
    Serial.print(F("\nIP Addr: ")); cc3000.printIPdotsRev(ipAddress);
    Serial.print(F("\nNetmask: ")); cc3000.printIPdotsRev(netmask);
    Serial.print(F("\nGateway: ")); cc3000.printIPdotsRev(gateway);
    Serial.print(F("\nDHCPsrv: ")); cc3000.printIPdotsRev(dhcpserv);
    Serial.print(F("\nDNSserv: ")); cc3000.printIPdotsRev(dnsserv);
    Serial.println();
    #endif
    return true;
  }
}

/**************************************************************************/
/*!
 @brief  Tries to read the 6-byte MAC address of the CC3000 module
 */
/**************************************************************************/
/*void displayMACAddress(void)
{
  uint8_t macAddress[6];

  if(!cc3000.getMacAddress(macAddress))
  {
    Serial.println(F("Unable to retrieve MAC Address!\r\n"));
  }
  else
  {
    Serial.print(F("MAC Address : "));
    cc3000.printHex((byte*)&macAddress, 6);
  }
}*/

#ifdef EEPROM_h
void MacroDuino::setDeviceAddress() {
  if(EEPROM.read(ADDRESS_START) != 253) {
    EEPROM.write(ADDRESS_START, 253); 
    for(byte j = ADDRESS_START+1; j < ADDRESS_END; j++) {
      EEPROM.write(j, random(65, 90));  
    }
  } 
  else {
    for(byte j = ADDRESS_START+1; j < ADDRESS_END; j++) {
      this->_deviceID[j-1] = EEPROM.read(j);
	  this->_deviceID[j] = '\0';
    }
	#ifdef DEBUG_SERIAL
	Serial.print("Device ID: ");
	Serial.println(this->_deviceID);
	#endif   
  }
}

char* MacroDuino::deviceID() {
	return this->_deviceID;
}
#endif

/**************************************************************************/
/*!
 @brief  Tries to read the CC3000's internal firmware patch ID
 */
/**************************************************************************/
uint16_t MacroDuino::cc3000CheckFirmwareVersion(Adafruit_CC3000& cc3000) {
  uint8_t major, minor;
  uint16_t version;

  #ifndef CC3000_TINY_DRIVER  
  if(!cc3000.getFirmwareVersion(&major, &minor)) {
    version = 0;
  } 
  else {
    version = major; 
    version <<= 8; 
    version |= minor;
  }
  #endif
  return version;
}

/**************************************************************************/
/*!
 @brief  Tries to read the IP address and other connection details
 */
/**************************************************************************/
bool displayConnectionDetails(Adafruit_CC3000& cc3000)
{
  uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv;

  if(!cc3000.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv))
  {
  	#ifdef DEBUG_SERIAL
    Serial.println(F("Unable to retrieve the IP Address!\r\n"));
    #endif
    return false;
  }
  else
  {
  	#ifdef DEBUG_SERIAL
    Serial.print(F("\nIP Addr: ")); 
    cc3000.printIPdotsRev(ipAddress);
    Serial.print(F("\nNetmask: ")); 
    cc3000.printIPdotsRev(netmask);
    Serial.print(F("\nGateway: ")); 
    cc3000.printIPdotsRev(gateway);
    Serial.print(F("\nDHCPsrv: ")); 
    cc3000.printIPdotsRev(dhcpserv);
    Serial.print(F("\nDNSserv: ")); 
    cc3000.printIPdotsRev(dnsserv);
    Serial.println();
    #endif
    return true;
  }
}

#ifdef COMMAND_CONFIGUREISE_ENABLED
bool MacroDuino::configureISE(byte pin, int value1, int value2) {
	if((ISE_CONF_START + (pin * ISE_CONF_BYTES)) > ISE_CONF_END) {
		//out of address space
		return false;
	}

	byte matchCount;
	unsigned long reading = 0;
	int oldValue = 1;
	int newValue = 0;	
	int calibration_solution_first = (pin * ISE_CONF_BYTES) + ISE_CONF_START;
	int calibration_solution_second = (pin * ISE_CONF_BYTES) + ISE_CONF_START + 1;		
	int calibration_value_first = (pin * ISE_CONF_BYTES) + ISE_CONF_START + 2;
	int calibration_value_second	= (pin * ISE_CONF_BYTES) + ISE_CONF_START + 4;
	
  EEPROM.write(calibration_solution_first, value1);

	matchCount = 0;
  reading = 0;
  while(matchCount < ISE_CONFIGURE_MATCHCOUNT) {
    oldValue = newValue;
    reading = 0;
    #ifdef DEBUG_SERIAL
    Serial.println("Not stable, carrying on");
    #endif
    for(byte i=0; i<255; i++) {
      reading = reading + analogRead(pin);
			#ifdef DEBUG_SERIAL
			Serial.print("Reading: ");
			Serial.println(reading);      
			#endif
      delay(10);
    }
    newValue = reading/255;
    
    if(newValue == oldValue) {
      matchCount++;
    }
    #ifdef DEBUG_SERIAL
    Serial.print("Old value: ");
    Serial.println(oldValue);
    Serial.print("New value: ");
    Serial.println(newValue);    
    Serial.print("Match Count: ");
    Serial.println(matchCount);   
    #endif
    delay(500); 
  }
  matchCount = 0;

  EEPROM.write(calibration_value_first, this->lowValue(newValue));
  EEPROM.write(calibration_value_first+1, this->highValue(newValue)); 

  oldValue = 1;
  newValue = 0; 

	if(mqttclient.connected()) {
		memset(mqttString, 0, sizeof mqttString); 
		strcat(mqttString, MQTT_Username);
		strcat(mqttString, "/"); 
		strcat(mqttString, "configureise");
		mqttclient.publish(mqttString, "Place probe in second solution.");
	}
	#ifdef DEBUG_SERIAL
	Serial.println("Now configuring second solution");
	#endif
	
	delay(30000);
	
  //second calibration solution
	EEPROM.write(calibration_solution_second, value2);
	
	matchCount = 0;
  reading = 0;
  while(matchCount < ISE_CONFIGURE_MATCHCOUNT) {
    oldValue = newValue;
    reading = 0;    
    #ifdef DEBUG_SERIAL
    Serial.println("Not stable, carrying on");
    #endif
    reading = 0;
    for(byte i=0; i<255; i++) {
      reading = reading + analogRead(pin);
      delay(10);
    }
    newValue = reading/255;
    
    if(newValue == oldValue) {
      matchCount++;
    }
		
		#ifdef DEBUG_SERIAL
    Serial.print("Old value: ");
    Serial.println(oldValue);
    Serial.print("New value: ");
    Serial.println(newValue);		
    Serial.print("Match Count: ");
    Serial.println(matchCount);    
    #endif
    delay(500);
  }
  matchCount = 0;
  EEPROM.write(calibration_value_second, lowValue(newValue));
  EEPROM.write(calibration_value_second+1, highValue(newValue));
  
  return true;  
}
#endif

#ifdef COMMAND_READISE_ENABLED
float MacroDuino::readISE(byte pin) {
  int y1 = EEPROM.read((pin * ISE_CONF_BYTES) + ISE_CONF_START);
  int y2 = EEPROM.read((pin * ISE_CONF_BYTES) + ISE_CONF_START + 1);
  int x1 = this->combineValue(EEPROM.read((pin * ISE_CONF_BYTES) + ISE_CONF_START + 2), EEPROM.read((pin * ISE_CONF_BYTES) + ISE_CONF_START + 3));  
  int x2 = this->combineValue(EEPROM.read((pin * ISE_CONF_BYTES) + ISE_CONF_START + 4), EEPROM.read((pin * ISE_CONF_BYTES) + ISE_CONF_START + 5));
  //work out m for y = mx + b
  float m = ((float)y1 - (float)y2) / ((float)x1 - (float)x2);
  //work out b for y = mx + 
  float b = (float)y1 - ((float)m * (float)x1);
  
  #ifdef DEBUG_SERIAL
  Serial.print("X1: ");
  Serial.println(x1);
  Serial.print("X2: ");
  Serial.println(x2);  
  Serial.print("Y1: ");
  Serial.println(y1);  
  Serial.print("Y2: ");
  Serial.println(y2);  
  Serial.print("M: ");
  Serial.println(m);
  Serial.print("B: ");
  Serial.println(b);
  #endif

  long reading = 0;
  for(byte i=0; i<255; i++) {
    reading = reading + analogRead(pin);
    delay(10);
  }
  reading = reading/255;
  #ifdef DEBUG_SERIAL
  Serial.print("Analog: ");
  Serial.println(reading);
  #endif
  
  float y = (m*(float)reading) + b;
  return y;
}
#endif