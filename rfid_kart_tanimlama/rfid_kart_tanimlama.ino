#include <SPI.h>   // SPI haberleşme kütüphanesi çağırıyoruz
#include <RFID.h>  // Rfid kütüphanemizi çağırıyoruz
RFID rfid(10,9); //SS veya SDA pini ile RST pinini arduino da bağladığımız numaraları giriyoruz 
void setup() {
  Serial.begin(9600);  //Serial port u başlatıyoruz
  SPI.begin();         // SPI haberleşmeyi başlatıyoruz
  rfid.init();         //  Rfid okuma işlemi için hazır
}
 
void loop() {
   
  if(rfid.isCard())      // eğer kart algılandı ise...
  {
    if(rfid.readCardSerial())   // Kart seri numarasını oku
    {
// kart seri numarasını okuyup hex kodlarına dönüştürüyoruz...
      Serial.print("Kart ID: ");
      Serial.print(rfid.serNum[0],HEX);   
      Serial.print(rfid.serNum[1],HEX);  
      Serial.print(rfid.serNum[2],HEX);
      Serial.print(rfid.serNum[3],HEX);
      Serial.println(rfid.serNum[4],HEX);
    }
    rfid.halt();     // kart yeniden okuma yapabilir...
  }
}
