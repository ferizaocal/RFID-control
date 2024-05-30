#include <SPI.h>
#include <RFID.h>
RFID rfid(10,9); //SS veya SDA pini, RST pini
void setup() {
  SPI.begin(); //SPI haberleşme başlatıldı
  rfid.init(); //kart okumaya hazır
  pinMode(3, OUTPUT);  // yeşil led bağlantısı
  pinMode(2, OUTPUT);  // kırmızı led bağlantısı
}
// yetki verdiğiniz kart numarasını aşağıdaki alana girin...
byte kartim[5] = {0x12,0x92,0x9A,0x1E,0x04}; 
void loop() {
  boolean gecerlikart = true; //başlangıçta geçerlikart bilgisi "true" olarak kabul ediliyor...
  if(rfid.isCard()) //yeni kart okunduysa
  {
    if(rfid.readCardSerial())
    {
      Serial.print("Kart ID: ");
      Serial.print(rfid.serNum[0],HEX);
      Serial.print(rfid.serNum[1],HEX);
      Serial.print(rfid.serNum[2],HEX);
      Serial.print(rfid.serNum[3],HEX);
      Serial.println(rfid.serNum[4],HEX);
    }
    for(int i=0; i<5; i++)
    {
      if(rfid.serNum[i] != kartim[i]) // sisteme kayıtlı kart ile okunan kart karşılaştırılıyor
      {
        gecerlikart = false; // kart farklı ise geçerlikart değişkeni "false" yapılıyor
      }
    }
    if(gecerlikart==true)//eğer geçerli kart ise
    {
      digitalWrite(2, HIGH); // yeşil ledi yak
      delay(2000); //2 sn bekle
      digitalWrite(2, LOW); // yeşil ledi söndür
    }
    else //eğer geçerli kart değil ise
    {
      digitalWrite(3, HIGH); // kırmızı ledi yak
      delay(2000); //2 sn bekle
      digitalWrite(3, LOW); //kırmızı ledi söndür
    }
    rfid.halt();
  }
}
