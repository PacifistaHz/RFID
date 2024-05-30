#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <LiquidCrystal.h>

#define SS_PIN 10
#define RST_PIN 9
#define SERVO_PIN 8

MFRC522 mfrc522(SS_PIN, RST_PIN);
Servo myServo;
LiquidCrystal lcd(6, 7, 5, 4, 3, 2);

void setup() {
  SPI.begin();
  mfrc522.PCD_Init();
  myServo.attach(SERVO_PIN);
  myServo.write(0); // Servo başlangıç pozisyonu
  lcd.begin(16, 2);
  lcd.print("KART OKUTUNUZ");
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("UID :");
  String content = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

  lcd.setCursor(0, 1);
  lcd.print(content);

  content.toUpperCase();
  delay(2000); // Ekranda UID'nin görünmesi için bekle

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MESAJ : ");
  if (content.substring(1) == "D6 CA A8 89") { // Buraya kartınızın UID'sini yazın
    lcd.setCursor(0, 1);
    lcd.print("KAYITLI");
    myServo.write(90); // Servo açısını değiştir
    delay(3000); // Servonun açık kalma süresi
    myServo.write(0); // Servo başlangıç pozisyonuna dön
  } else {
    lcd.setCursor(0, 1);
    lcd.print("KAYIT HATASI");
  }

  delay(3000); // Mesajın ekranda kalma süresi
  lcd.clear();
  lcd.print("KART OKUTUNUZ");

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}
