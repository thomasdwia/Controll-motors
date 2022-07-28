#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 4);

#define ENABLE1 6//4
#define DIRECTION1 4//3
#define STEP1 5//2
#define ENABLE 7
#define DIRECTION 9
#define STEP 8
#define sensorPin A0
#define FORWARD HIGH
#define REVERSE LOW

int i = 0, nilai[10] = {}, count = 0, pre = 2, pre1 = 3;
int kalib = 0, kalibra[2] = {0}, count_kalib = 1, count_select = 2, count_select1 = 3, nomor = 1;
int button1 = 0, button2 = 0, button3 = 0, button4 = 0, button5 = 0;
int SPEED1 = 30, SPEED = 100; // PRESET MOTOR1 = 80
float total = 0, eep, sementara, motora, motorb, vmot = 0;

void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(16, INPUT_PULLUP);

  //MOTOR 1
  pinMode(ENABLE, OUTPUT);
  pinMode(DIRECTION, OUTPUT);
  pinMode(STEP, OUTPUT);
  digitalWrite(ENABLE, LOW);
  //MOTOR 2
  pinMode(ENABLE1, OUTPUT);
  pinMode(DIRECTION1, OUTPUT);
  pinMode(STEP1, OUTPUT);
  digitalWrite(ENABLE1, LOW);

  eep = EEPROM.read(0) / 10.00;
  while (eep > 1) {
    sementara = (eep * 3200) / 7;
    digitalWrite(DIRECTION1, REVERSE);
    for (int j = 0; j < sementara; j++) {
      digitalWrite(STEP1, HIGH);
      delayMicroseconds(100);
      digitalWrite(STEP1, LOW);
      delayMicroseconds(100);
    }
    EEPROM.write(10, 0); eep = 0;
  }

  lcd.begin();
  lcd.setCursor(3, 0);
  lcd.print("VOKASI UGM");
  lcd.setCursor(0, 1);
  lcd.print("================");
  baca();
}

void preset() {
  button4 = digitalRead(12);
  if (button4 == LOW) {
    pre = pre + 2;
    pre1 = pre1 + 2;
    nomor++;
  }
  if (pre > 11) {
    pre = 2;
    pre1 = 3;
    nomor = 1;
  }
  motora = EEPROM.read(pre);
  motorb = EEPROM.read(pre1);
  Serial.println(motora);
  Serial.println(motorb);
  lcd.setCursor(7, 2);
  lcd.print(nomor);
  lcd.setCursor(-4, 3);
  lcd.print(motora);
  lcd.setCursor(2, 3);
  lcd.print(motorb);
}

void masukan() {
  //Tekan tombol 3 kali, tekanan pertama untuk nilai 1, tekanan kedua untuk nilai 2, tekanan ketiga untuk mengunci;
  button2 = digitalRead(3); // button set kalibrasi
  button3 = digitalRead(12); //button select untuk mengisi preset ke berapa dari 1-10
  if (button2 == LOW) {
    kalib++;
    lcd.clear();
  }
  if (button3 == LOW) {
    count_select = count_select + 2;
    count_select1 = count_select + 2;
    nomor++;
    lcd.clear();
  }
  if (count_select > 11 || nomor > 5) {
    count_select = 2;
    count_select = 3;
    nomor = 1;
  }
  if (kalib > 3) {
    kalib = 0;
  }
  if (kalib == 0) {
    Serial.print("Tersimpan pada preset : ");
    Serial.println(nomor);
    lcd.setCursor(-4, 3);
    lcd.print("Preset :");
    lcd.setCursor(4, 3);
    lcd.print(nomor);
  }
  if (kalib == 1) {
    motora = analogRead(A0);
    kalibra[0] = map(motora, 0, 1024, 0, 50);
    lcd.setCursor(-4, 3);
    lcd.print("Posisi1 :");
    lcd.setCursor(5, 3);
    lcd.print(kalibra[0]);
  }
  if (kalib == 2) {
    motorb = analogRead(A0);
    kalibra[1] = map(motorb, 0, 1024, 0, 50);
    lcd.setCursor(-4, 3);
    lcd.print("Posisi2 :");
    lcd.setCursor(5, 3);
    lcd.print(kalibra[1]);
  }
  if (kalib == 3) {
    Serial.print("Tersimpan pada preset : ");
    Serial.println(nomor);
    EEPROM.write(count_select, kalibra[0]);
    EEPROM.write(count_select1, kalibra[1]);
    lcd.setCursor(-1, 3);
    lcd.print("TERSIMPAN!");
  }
}

void loop() {
  button1 = digitalRead(2);
  if (button1 == LOW) {
    lcd.clear();
    count++;
    count_kalib++;
  }
  if (count > 2) {
    count = 0;
  }
  if (count == 0) {
    lcd.setCursor(3, 0);
    lcd.print("VOKASI UGM");
    lcd.setCursor(0, 1);
    lcd.print("================");
    lcd.setCursor(-4, 2);
    lcd.print("MODE:RUNNING");
    Serial.println("MODE: RUNNING");
    Serial.println(nomor);
    SPEED = analogRead(A1);
    SPEED = map(SPEED, 0, 1023, 20, 150);
    button4 = digitalRead(11);
    lcd.setCursor(-4,3);
    lcd.print("0");
    if (SPEED < 100){
       lcd.setCursor(-3, 3);
       lcd.print(SPEED); 
    }
    else{
       lcd.setCursor(-4, 3);
       lcd.print(SPEED); 
    }
    if (button4 == LOW) {
      vmot = 1;
    }
    Serial.write("0");
    String coba;
    coba = Serial.readString();
    if (vmot == 1 || coba == "1") {
      Serial.write("1");
      // MOTOR 2;
      for (int i = 0; i < 1; i++) {
        lcd.setCursor(0, 3);
        lcd.print("JALAN");
        int a = motora * 460, b = motorb * 460;
        float hasil;
        hasil = 9000;//2 * 1000000 / 200;

        //PROGRAM JALAN MOTOR 1 DAN MOTOR 2
        for ( int count = 0; count < hasil; count++) {
          digitalWrite(DIRECTION, FORWARD);
          digitalWrite(STEP, HIGH);
          delayMicroseconds(SPEED);
          digitalWrite(STEP, LOW);
          delayMicroseconds(SPEED);
        }
        for (int j = 0; j < a; j++) {
          digitalWrite(DIRECTION1, FORWARD);
          digitalWrite(STEP1, HIGH);
          delayMicroseconds(SPEED1);
          digitalWrite(STEP1, LOW);
          delayMicroseconds(SPEED1);
          i++;
          total = i * 70 / 3200;
          EEPROM.write(0, total);
        }
        for ( int count = 0; count < hasil; count++) {
          digitalWrite(DIRECTION, FORWARD);
          digitalWrite(STEP, HIGH);
          delayMicroseconds(SPEED);
          digitalWrite(STEP, LOW);
          delayMicroseconds(SPEED);
        }
        for (int j = 0; j < b; j++) {
          digitalWrite(DIRECTION1, FORWARD);
          digitalWrite(STEP1, HIGH);
          delayMicroseconds(SPEED1);
          digitalWrite(STEP1, LOW);
          delayMicroseconds(SPEED1);
          i++;
          total = i * 70 / 3200;
          EEPROM.write(0, total);
        }
        for ( int count = 0; count < hasil; count++) {
          digitalWrite(DIRECTION, FORWARD);
          digitalWrite(STEP, HIGH);
          delayMicroseconds(SPEED);
          digitalWrite(STEP, LOW);
          delayMicroseconds(SPEED);
        }
        for (int j = 0; j < a + b; j++) {
          digitalWrite(DIRECTION1, REVERSE);
          digitalWrite(STEP1, HIGH);
          delayMicroseconds(SPEED1);
          digitalWrite(STEP1, LOW);
          delayMicroseconds(SPEED1);
          i--;
          total = i * 70 / 3200;
          EEPROM.write(0, total);
        }
        vmot = 0;
      }
    }
  }
  else if (count == 1) {
    lcd.setCursor(3, 0);
    lcd.print("VOKASI UGM");
    lcd.setCursor(0, 1);
    lcd.print("================");
    lcd.setCursor(-4, 2);
    lcd.print("MODE:PRESET");
    Serial.println("MODE: PRESET");
    Serial.write("0");
    preset();
  }
  else {
    lcd.setCursor(3, 0);
    lcd.print("VOKASI UGM");
    lcd.setCursor(0, 1);
    lcd.print("================");
    Serial.println("MODE: KALIBRASI");
    lcd.setCursor(-4, 2);
    lcd.print("MODE:KALIBRASI");
    Serial.write("0");
    masukan();
    Serial.println(kalibra[0]);
    Serial.println(kalibra[1]);
  }
  delay(200);
}
