#include <ArduinoBLE.h>

BLEService sendService("170F");

BLEUnsignedCharCharacteristic remoteSend("FFE1",  // standard 16-bit characteristic UUID
    BLEWriteWithoutResponse); // remote clients will be able to get notifications if this characteristic changes

long previousMillis = 0;  // last time the battery level was checked, in ms
char output[6];
int byteCount = 0;

void setup() {
  Serial.begin(9600);    // initialize serial communication
  while (!Serial);

  pinMode(LED_BUILTIN, OUTPUT); // initialize the built-in LED pin to indicate when a central is connected
  
  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (1);
  }

  /* Set a local name for the Bluetooth® Low Energy device
     This name will appear in advertising packets
     and can be used by remote devices to identify this Bluetooth® Low Energy device
     The name can be changed but maybe be truncated based on space left in advertisement packet
  */
  
  BLE.setAdvertisedService(sendService);
  sendService.addCharacteristic(remoteSend);
  BLE.addService(sendService);
  
  // start advertising
  BLE.advertise();

  Serial.println("Bluetooth® device active, waiting for connections...");
  Serial.println(remoteSend.valueSize());
}

void loop() {
  // wait for a Bluetooth® Low Energy central
  BLEDevice central = BLE.central();
  int counter = 0;

  // if a central is connected to the peripheral:
  if (central) {
//    Serial.print("Connected to central: ");
//    // print the central's BT address:
//    Serial.println(central.address());
    // turn on the LED to indicate the connection:
    digitalWrite(LED_BUILTIN, HIGH);

    if (remoteSend.value()){
      Serial.println("got data");
      char buf[50];
      
      //byteCount = remoteSend.readValue(outChar);
      Serial.print("length is : ");
      int length = remoteSend.valueLength();
      Serial.println(remoteSend.valueLength());
      remoteSend.readValue(buf,length);
      Serial.println(buf);
     // Serial.println(outChar,DEC);
      byteCount = 0;
      counter++;
    }
   
    // when the central disconnects, turn off the LED:
    digitalWrite(LED_BUILTIN, LOW);
//    Serial.print("Disconnected from central: ");
//    Serial.println(central.address());
  }
}
