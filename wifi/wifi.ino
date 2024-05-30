#include <SPI.h>   // SPI haberleşme kütüphanesi çağırıyoruz
#include <RFID.h>  // RFID kütüphanesini çağırıyoruz
#include <ESP8266WiFi.h>  // ESP8266 WiFi kütüphanesini çağırıyoruz

RFID rfid(10, 9); // SS (SDA) ve RST pinlerini Arduino üzerinde bağladığımız pin numaraları ile tanımlıyoruz

// WiFi bilgileri
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// Yetkili kart seri numarası
byte authorizedCard[5] = {0x12, 0x34, 0x56, 0x78, 0x90};

void setup() {
  Serial.begin(9600);  // Seri haberleşmeyi başlatıyoruz
  SPI.begin();         // SPI haberleşmeyi başlatıyoruz
  rfid.init();         // RFID modülünü başlatıyoruz

  // LED pinlerini çıkış olarak ayarla
  pinMode(2, OUTPUT);  // Yeşil LED
  pinMode(3, OUTPUT);  // Kırmızı LED

  // WiFi'ye bağlan
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("WiFi'ye bağlanılıyor...");
  }
  Serial.println("WiFi bağlantısı başarılı.");
}

void loop() {
  if (rfid.isCard()) {  // Eğer kart algılandı ise...
    if (rfid.readCardSerial()) {  // Kartın seri numarasını oku
      // Kart seri numarasını okuyup hex kodlarına dönüştürüyoruz
      Serial.print("Kart ID: ");
      for (int i = 0; i < 5; i++) {
        Serial.print(rfid.serNum[i], HEX);  // Her bir byte'ı hex formatında yazdır
        if (i < 4) {
          Serial.print("-");  // Byte'lar arasında tire koy
        }
      }
      Serial.println();  // Yeni satır

      // Kartın yetkili olup olmadığını kontrol et
      boolean isAuthorized = true;
      for (int i = 0; i < 5; i++) {
        if (rfid.serNum[i] != authorizedCard[i]) {
          isAuthorized = false;
          break;
        }
      }

      if (isAuthorized) {
        digitalWrite(2, HIGH);  // Yeşil LED'i yak
        sendToServer(true);  // Yetkili kart bilgilerini sunucuya gönder
        delay(2000);
        digitalWrite(2, LOW);  // Yeşil LED'i söndür
      } else {
        digitalWrite(3, HIGH);  // Kırmızı LED'i yak
        sendToServer(false);  // Yetkisiz kart bilgilerini sunucuya gönder
        delay(2000);
        digitalWrite(3, LOW);  // Kırmızı LED'i söndür
      }
    }
    rfid.halt();  // Kart yeniden okuma yapabilir hale getir
  }
}

void sendToServer(bool isAuthorized) {
  WiFiClient client;
  const char* host = "your_server_address";
  const int port = 80;
  
  if (!client.connect(host, port)) {
    Serial.println("Sunucuya bağlanılamadı.");
    return;
  }

  String url = "/update?status=";
  url += (isAuthorized ? "authorized" : "unauthorized");

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  delay(500);
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  Serial.println();
  client.stop();
}
