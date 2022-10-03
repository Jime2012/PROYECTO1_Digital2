

/************************** Configuration ***********************************/

#include <SPI.h>
#include "config.h"

#define PIC_SCK (18)
#define PIC_MISO (19)
#define PIC_MOSI (23)
#define PIC_CS (5)


int data1;
int data2;
int data3;
int data4;
int temp;
int cont = 0;
int cont1 = 0;
unsigned long previousMillis = 0;    
unsigned long previousMillis1 = 0;  
const long interval = 100;
const long interval1 = 3000;
int32_t lux;
int16_t lumens;
int val;
int val2;
int val3;
int sec_decenas = 0;
int sec_unidades = 0;
int min_decenas = 0;
int min_unidades = 0;
int hour_unidades = 0;
int hour_decenas = 0;
int horas = 0;
int minutos = 0;
int segundos = 0;



/************************ Example Starts Here *******************************/

// this int will hold the current count for our sketch
int count = 0;

// set up the 'counter' feed
AdafruitIO_Feed *Temperatura = io.feed("Temperatura");
AdafruitIO_Feed *LUX = io.feed("LUX");
AdafruitIO_Feed *Horas = io.feed("Horas");
AdafruitIO_Feed *Minutos = io.feed("Minutos");
AdafruitIO_Feed *Segundos = io.feed("Segundos");
void setup() {

  // start the serial connection
  SPIClass SPI1(VSPI);
  Serial.begin(9600);
  pinMode(PIC_CS, OUTPUT);
  digitalWrite(PIC_CS,HIGH); 
  // inicializar el SPI  
  SPI.begin(); 
  SPI.setClockDivider(SPI_CLOCK_DIV4);
  SPI.beginTransaction(SPISettings(1000000,MSBFIRST,SPI_MODE0));
   
  
  // wait for serial monitor to open
  while(! Serial);

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();
  unsigned long currentMillis = millis();


  digitalWrite(PIC_CS, LOW);
    data1 = SPI.transfer(1);
    delay(50);
    data2 = SPI.transfer(2);
    delay(50);
    data3 = SPI.transfer(3);
    delay(50);
    data4 = SPI.transfer(4);
    delay(50);
    temp = SPI.transfer(5);
    delay(50);
    val = SPI.transfer(6);
    delay(50);
    val2 = SPI.transfer(7);
    delay(50);
    val3 = SPI.transfer(8);
    delay(50);
  digitalWrite(PIC_CS, HIGH);
    \


  Serial.println("data1:");
  Serial.println(data1);
  Serial.println("data2:");
  Serial.println(data2);
  Serial.println("data3:");
  Serial.println(data3);
  Serial.println("data4:");
  Serial.println(data4);
  Serial.println("temp:");
  Serial.println(temp);
  Serial.println("val:");
  Serial.println(val);
  Serial.println("val2:");
  Serial.println(val2);
  Serial.println("val3:");
  Serial.println(val3);
  

  lux = data1;
  lux = (lux<<8)|data2;
  lux = (lux<<8)|data3;
  lumens = (lux<<8)|data4;
  
  sec_unidades = (val&0x0F);  //Mascara para unidades o 4 LSB --> Revisar datasheet DS3231
  sec_decenas = (val >> 4);  //Shifteo para decenas o 4 MSB
  min_unidades = (val2&0x0F);  //Mascara para unidades o 4 LSB --> Revisar datasheet DS3231
  min_decenas = (val2 >> 4);  //Shifteo para decenas o 4 MSB
  hour_unidades = (val3&0x0F);  //Mascara para unidades o 4 LSB --> Revisar datasheet DS3231
  hour_decenas = (val3 >> 4);  //Shifteo para decenas o 4 MSB

  horas = (hour_decenas*10)+(hour_unidades);
  minutos = (min_decenas*10)+(min_unidades);
  segundos = (sec_decenas*10)+(sec_unidades);
  
  unsigned long currentMillis1 = millis();

  if (currentMillis1 - previousMillis1 >= interval1) {
    previousMillis1 = currentMillis1;
    if (cont1 == 0){
    Temperatura->save(temp);
    }
    else if (cont1 == 1){
    Horas->save(horas);
    }
    else if (cont1 == 2){
    Minutos->save(minutos);
    }
    else if (cont1 == 3){
    LUX->save(lumens);
    }
    else if (cont1 == 4){
    Segundos->save(segundos);
    }
    
    if(cont1 < 5){
      cont1++;
    }
    else{
      cont1 = 0;
    }
    
    }
 
}
