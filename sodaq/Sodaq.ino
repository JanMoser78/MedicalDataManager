#include "Sodaq_RN2483.h"
#include "Arduino.h"


#define debugSerial SerialUSB
#define loraSerial Serial1
#define ESPSerial Serial

//define tokens
#define NURSE_TOKEN 10
#define PATIENT_TOKEN 20

/* The number of the device: 1,2,3,4 */
#define deviceNo 4

#define beePin ENABLE_PIN_IO

void BLUE() {
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_BLUE, LOW);
}

void RED() {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_BLUE, HIGH);
}

void YELLOW() {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_BLUE, HIGH);
}

void WHITE() {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_BLUE, LOW);
}


void GREEN() {
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_BLUE, HIGH);
}

void CLEAR() {
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_BLUE, HIGH);
}

void blink(int length) {
    switch(deviceNo) {
    case 1:
        BLUE();
        break;
    case 2:
        RED();
        break;
    case 3:
        GREEN();
        break;
    case 4:
        YELLOW();
        break;
    }
    delay(length);
    CLEAR();
}


void setupLED() {
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
}

// OTAA
// Random numbers chosen + device id
uint8_t DevEUI[8] = { 0x9c, 0xd9, 0x0b, 0xb5, 0x2b, 0x6a, 0x1d, deviceNo };

uint8_t AppEUI[8] = { 0xd4, 0x16, 0xcd, 0x0b, 0x7b, 0xcf, 0x2d, 0x5c };

uint8_t AppKey[16] = { 0xa9, 0xbc, 0x8b, 0x6a, 0x81, 0x75, 0xf6, 0x33,
0xe0, 0xd6, 0x64, 0xd9, 0x2b, 0xcb, 0x13, 0x78 };

int counter=0;

unsigned long long millis_offset = 0;

unsigned long long millis_last_alarm = 0;

unsigned char LoRa_buffer[59];

void init_LoRa_buffer(void){
  for (int i = 0 ; i < 59 ; i++ )
    LoRa_buffer[i] = 0;
}

void setupLoRaOTAA(){
  if (LoRaBee.initOTA(loraSerial, DevEUI, AppEUI, AppKey, true))
  {
    debugSerial.println("Communication to LoRaBEE successful.");
  }
  else
  {
    debugSerial.println("OTAA Setup failed!");
  }
}

typedef struct {
  unsigned char compartment_id;
  unsigned short token;
  unsigned char dose;     
  unsigned long long time_from;
  unsigned long long time_to;
  unsigned char critical;
  unsigned char full=false;
} Compartment;

void parse_compartmentdata(void* compartment, unsigned char* LoRa_buffer){
  Compartment* x = (Compartment*)compartment;
  x->compartment_id = LoRa_buffer[0];
  
  ((unsigned char*)&x->token)[0] = LoRa_buffer[2];
  ((unsigned char*)&x->token)[1] = LoRa_buffer[1];
  
  x->dose = LoRa_buffer[3];
  
  x->time_from = array_to_time(LoRa_buffer+4);

  x->time_to = array_to_time(LoRa_buffer+12);

  x->critical = LoRa_buffer[20];
}

unsigned long long array_to_time(unsigned char* arr){
  unsigned long long t = 0;
  for ( int i = 0 ; i < 8 ; i++)
      t += ((unsigned long long)arr[i])<<(8*(7-i));

  return t;
}

void print_time(unsigned long long t){
   debugSerial.println(*(unsigned int*)(((byte*)&t)+4),HEX);
   debugSerial.println(*((unsigned int*)&t),HEX);
   
   /*debugSerial.print("year: ");
   int a = t/30780000000LL;
   debugSerial.print(a);
   t = t%30780000000LL;
   a = t/1036800000LL; 
   debugSerial.print("\tmonth: ");
   debugSerial.print(a);
   t = t%1036800000LL;
   a = t/86400000LL;
   debugSerial.print("\tday: ");
   t = t%86400000LL;
   a = t/3600000LL;
   debugSerial.print("\thour: ");
   debugSerial.print(a);
   t = t%3600000LL;
   a = t/600000LL;
   debugSerial.print("\tminute: ");
   debugSerial.print(a);
   debugSerial.print("\n");*/
}

Compartment compartment_database[8];
int active_compartment=0;

enum Actions {Dispense = 1, Alarm, Plan_request, Refill, Xdk, Time_request};

typedef struct {
  unsigned char action_id = 0;      // 0
  unsigned char compartment = 0;    // 1
  unsigned short access_token = 0;  // 2,3
  unsigned long long t = 0;         // 8-15
  unsigned short sensor_values[9]={0,0,0,0,0,0,0,0,0}; 
}Paket;

void flipbytes( char* paket){
    char x;
    x=paket[8]; paket[8]=paket[15]; paket[15]=x;
    x=paket[9]; paket[9]=paket[14]; paket[14]=x;
    x=paket[10]; paket[10]=paket[13]; paket[13]=x;
    x=paket[11]; paket[11]=paket[12]; paket[12]=x;
    x=paket[2]; paket[2]=paket[3]; paket[3]=x;
}

typedef struct{
  unsigned short token=0;
  unsigned char permission=0;
}Client;

Client client_database [3];

enum {nurse = 1, patient};

void debugserialprint_compartment(void* x){
  Compartment * compartment = (Compartment*)x;
  debugSerial.println("token:");
  debugSerial.println(compartment->token);

  debugSerial.println("dose:");
  debugSerial.println(compartment->dose);

  debugSerial.println("critical:");
  debugSerial.println(compartment->critical);
  debugSerial.println("time_data:");
 // debugSerial.println(compartment->time_from);
}

void setup() {
  //Power up the LoRaBEE - on loraone/sodaq one
  pinMode(ENABLE_PIN_IO, OUTPUT); // ONE

  digitalWrite(beePin, HIGH); // ONE
  delay(3000);

  //Set baud rate
  debugSerial.begin(57600);
  loraSerial.begin(LoRaBee.getDefaultBaudRate());
  ESPSerial.begin(115200);

  while ((!debugSerial) && (millis() < 4000)){
    // Wait 10 seconds for the Serial Monitor
  }

  // Debug output from LoRaBee
  // LoRaBee.setDiag(debugSerial); // optional

  //Turn the GPS on
  pinMode(GPS_ENABLE, OUTPUT);
  digitalWrite(GPS_ENABLE, HIGH);

  delay(500);

  /* used for blinking */
 // counter=18;

  //connect to the LoRa Network
  setupLoRa();

  delay(1000);
  // TEST DEBUGGER 
  /*Compartment compartment;
  compartment.token = 24567;
  compartment.drug = 203;
  compartment.flag0 = true;
  compartment.flag1 = true;
  compartment.flag2 = true;
  compartment.flag3 = false;
  compartment.flag4 = true;
  compartment.flag5 = true;
  compartment.flag6 = false;
  compartment.flag7 = true;
  compartment.time_data = 345;
  debugserialprint_compartment((void*)&compartment);*/

  //test sscanf
  /*unsigned int a;
  int b = sscanf("$AUTH,45h*", "$AUTH,%u*", &a);
  debugSerial.println(a);
  debugSerial.println(b);*/

  //test
  //debugSerial.println(sizeof(compartment_database)/sizeof(*compartment_database));
  /*debugSerial.println(sizeof(unsigned long));
  debugSerial.println(sizeof(unsigned long long));
  debugSerial.println(sizeof(unsigned int));
  debugSerial.println(sizeof(unsigned short));
  debugSerial.println(sizeof(unsigned char));*/
  /*
  Paket a;
  byte* a_ptr = (byte*)&a;
  debugSerial.println(a_ptr[0], HEX);
  debugSerial.println(a_ptr[1], HEX);
  debugSerial.println(a_ptr[2], HEX);
  debugSerial.println(a_ptr[3], HEX);
  debugSerial.println(a_ptr[4], HEX);
  debugSerial.println(a_ptr[5], HEX);
  debugSerial.println(a_ptr[6], HEX);
  debugSerial.println(a_ptr[7], HEX);
  debugSerial.println(a_ptr[8], HEX);
  debugSerial.println(a_ptr[9], HEX);
  debugSerial.println(a_ptr[10], HEX);
  debugSerial.println(a_ptr[11], HEX);
  debugSerial.println(a_ptr[12], HEX);
  debugSerial.println(a_ptr[13], HEX);
  debugSerial.println(a_ptr[14], HEX);
  debugSerial.println(a_ptr[15], HEX);*/
  
  // sync time
  Paket time_paket;
  time_paket.action_id = Time_request;

  unsigned long send_time = millis();
  sendPacket((uint8_t*)&time_paket,sizeof(time_paket));
  counter = 0;
  
  init_LoRa_buffer();
  /*do{
    while (!LoRaBee.receive(LoRa_buffer, 18, 0)){
        delay(100);
        counter++;
  
        if (counter >=60){
            sendPacket((uint8_t*)"0",1);
            counter = 0;
            debugSerial.println("waiting for time sync...");
            counter2++
        }
    }
    debugSerial.println("waiting for time sync...");
    sendPacket((uint8_t*)"0",1);
  }while (LoRa_buffer[0] != 0xff);*/

  // check for time
  /*for (int i = 0 ; i < 10 ; i++ ){
        debugSerial.println(LoRa_buffer[i],HEX);
      }*/
  
  millis_offset = array_to_time(LoRa_buffer+1) - send_time;

  print_time(millis()+millis_offset);
  debugSerial.println("time synced");
  
  
  setupLED();
  blink(300);
    debugSerial.println("setup done");
}

void setupLoRa(){
    setupLoRaOTAA();
}

void sendPacket(uint8_t* packet, uint8_t l){
  switch (LoRaBee.send(1, packet, l))
    {
    case NoError:
      debugSerial.println("Successful transmission.");
      break;
    case NoResponse:
      debugSerial.println("There was no response from the device.");
      setupLoRa();
      break;
    case Timeout:
      debugSerial.println("Connection timed-out. Check your serial connection to the device! Sleeping for 20sec.");
      delay(20000);
      break;
    case PayloadSizeError:
      debugSerial.println("The size of the payload is greater than allowed. Transmission failed!");
      break;
    case InternalError:
      debugSerial.println("Oh No! This shouldn't happen. Something is really wrong! Try restarting the device!\r\nThe network connection will reset.");
      setupLoRa();
      break;
    case Busy:
      debugSerial.println("The device is busy. Sleeping for 10 extra seconds.");
      delay(10000);
      break;
    case NetworkFatalError:
      debugSerial.println("There is a non-recoverable error with the network connection. You should re-connect.\r\nThe network connection will reset.");
      setupLoRa();
      break;
    case NotConnected:
      debugSerial.println("The device is not connected to the network. Please connect to the network before attempting to send data.\r\nThe network connection will reset.");
      setupLoRa();
      break;
    case NoAcknowledgment:
      debugSerial.println("There was no acknowledgment sent back!");
      // When you this message you are probaly out of range of the network.
      break;
    default:
      break;
    }
}

void loop() {

  /*Paket to_send;
  
  to_send.action_id = Dispense;
  to_send.t = 1000;
  to_send.access_token = 123;
  to_send.compartment = 4;

  flipbytes((char*)&to_send);*/

  //check for alarm every half an hour
  unsigned long long t = millis()+millis_offset;
  if (1 || millis_last_alarm <= t-1800000LL){
    millis_last_alarm = t;
    if (1 || compartment_database[active_compartment].time_to < t && compartment_database[active_compartment].full == true){
      Paket send_alarm;
      send_alarm.action_id = Alarm;
      send_alarm.access_token = compartment_database[active_compartment].token;
      send_alarm.compartment = active_compartment;
      flipbytes((char*)&send_alarm);
      sendPacket((uint8_t*)&send_alarm,sizeof(send_alarm));
      debugSerial.println("alarm pakage sent");
    }
  }
  
  char ESP_buffer[12];
  //chargeing stage
  int i = 6;
  
  if (ESPSerial.available()) {
      bool end_of_buf = false;
      bool begin_found = false;
      while(ESPSerial.available() && !begin_found)
        begin_found = (ESPSerial.read() == '$');
      if(begin_found)
      {
        ESPSerial.readBytesUntil('*', ESP_buffer, 10);
        unsigned int token;
        if (1 == sscanf(ESP_buffer, "AUTH,%u", &token)){
            if ( token == PATIENT_TOKEN ){
                debugSerial.println("Patient");
                if ( active_compartment < 8 && t > compartment_database[active_compartment].time_from){
                  ESPSerial.print(active_compartment);
                  debugSerial.println("command open compartment sent");
                  //send dispense
                  Paket send_dispense;
                  send_dispense.action_id = Dispense;
                  send_dispense.access_token = token;
                  send_dispense.compartment = active_compartment;
                  send_dispense.t = t;

                  flipbytes((char*)&send_dispense);
                  sendPacket((uint8_t*)&send_dispense,sizeof(send_dispense));
                  
                  active_compartment++;
                  if ( active_compartment == 8 ){
                    Paket send_refil;
                    send_refil.action_id = Refill;
                    sendPacket((uint8_t*)&send_refil,sizeof(send_refil));
                    debugSerial.println("refill request sent");
                 
                  }
                  else{
                    debugSerial.println("no refill needed");
                  }
                  
                }
                else{
                  debugSerial.println("all compartments empty or not in time frame");
                }
                
            }
            else if ( token == NURSE_TOKEN ){
                debugSerial.println("Nurse");
                //for ( int i = 0 ; i < 8 ; i++ ){
                        Paket comp_request;
                        comp_request.action_id = Plan_request;
                        comp_request.compartment = i;
                      
                        unsigned long send_time = millis();
                        sendPacket((uint8_t*)&comp_request,sizeof(comp_request));
                        debugSerial.println("compartment data request sent");
                        counter = 0;
                        
                        init_LoRa_buffer();
                        do{
                          while (!LoRaBee.receive(LoRa_buffer, 18, 0)){
                              delay(100);
                              counter++;
                        
                              if (counter >=60){
                                  sendPacket((uint8_t*)"0",1);
                                  counter = 0;
                                  debugSerial.println("waiting for plan data...");
                              }
                          }
                          debugSerial.println("waiting for plan data...");
                          sendPacket((uint8_t*)"0",1);
                        }while (LoRa_buffer[0] != 0xff );
                        debugSerial.println("compartment data received");
                        
                        parse_compartmentdata(&compartment_database[i] , LoRa_buffer);
                        compartment_database[i].full = true;

                        ESPSerial.print(active_compartment);
                        
                        debugSerial.println("command open compartment sent");
                        
                //}
            }
        }
      }
  }

  /*if (int x = LoRaBee.receive(LoRa_buffer, 48, 0)) {
      debugSerial.println("package received");
      for (int i = 0 ; i < 10 ; i++ ){
        debugSerial.println(LoRa_buffer[i],HEX);
      }
      debugSerial.println(x);
      //test receive
      CLEAR();
      RED();
      delay(100);
      CLEAR();
  }*/
 
  /*Compartment x;
  parse_compartmentdata((void*) &x, LoRa_buffer);
  debugSerial.println("data");
  debugSerial.println(x.compartment_id);
  debugSerial.println(x.token);
  debugSerial.println(x.critical);*/
  

/*
   Paket time_paket;
    time_paket.action_id = Time_request;
    if(counter >= 10) {
      debugSerial.println(sizeof(time_paket));
      sendPacket((uint8_t*)&to_send,sizeof(time_paket));
      //blink(200);
      counter = 0;
  } else {
      blink(30);
      counter++;
  }*/
  
  //Blink long after sending packet
  /*if(counter >= 10) {
      debugSerial.println(sizeof(to_send));
      sendPacket((uint8_t*)&to_send,sizeof(to_send));
      //blink(200);
      counter = 0;
  } else {
      blink(30);
      counter++;
  }*/

  delay(3000);

  //delay(1000);
}
