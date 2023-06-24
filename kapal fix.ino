#include <Servo.h>             //memasukkan library servo dalam sketch
// #include "FS.h"                //memasukkan library sd card dalam sketch
#include "SD.h"                //memasukkan library sd card dalam sketch
#include <SPI.h>               //memasukkan library untuk komunikasi SPI dalam sketch
#include <Wire.h>              //memasukkan library wire dalam sketch
#include <Adafruit_GFX.h>      //memasukkan library oled dalam sketch
#include <Adafruit_SSD1306.h>  //memasukkan library oled 128x64 dalam sketch
#include "RTClib.h"            //memasukkan library RTC dalam sketch

String keputusan;  //
File file;         //menginisialisasi variabel file
RTC_DS3231 rtc;    //menginisialisasi variabel RTC
Servo servo;       //menginisialisasi servo

#define SCREEN_WIDTH 128                                                   //menetapkan lebar oled
#define SCREEN_HEIGHT 64                                                   //menetapkan tinggi oled
#define OLED_RESET -1                                                      //menetapkan tampilan oled secara berurutan
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);  //menginisialisasi oled

//SD
#define SD_CS 5  //menetapkan pin sd card

//SERVO
#define SERVO_PIN 4  //menetapakan pin servo

//sensor 1
#define TRIG_PIN1 15  //menetapakan pin trigger sensor
#define ECHO_PIN1 2   //menetapakan pin echo sensor

//sensor 2
#define TRIG_PIN2 17  //menetapakan pin trigger sensor
#define ECHO_PIN2 16  //menetapakan pin echo sensor

//sensor 3
#define TRIG_PIN3 13  //menetapakan pin trigger sensor
#define ECHO_PIN3 12  //menetapakan pin echo sensor

//motor driver
#define motor1Pin1 27  //menetapkan pin motor driver1
#define motor1Pin2 26  //menetapkan pin motor driver2

byte sudutkanan = 20;  //membuat variabel untuk nilai sudut servo
byte sudutlurus = 90;  //membuat variabel untuk nilai sudut servo
byte sudutkiri = 170;  //membuat variabel untuk nilai sudut servo
int jarak = 50;        //menginisialisasi jarak minimal sensor kiri kanan
int jarak_lurus = 50;  //menginisialisasi jarak minimal sensor depan
// SET DUTYCYCLE      0   = 0 %   64  = 25 %   127 = 50 %   191 = 75 %     255 = 100 %*/
int kecepatan = 40;       //menginisialisasi kecepatan lurus, motor driver
int kecepatanBelok = 35;  //menginisialisasi kecepatan belok, motor driver

#define SOUND_SPEED 0.034  //menetapkan kecepatan suara

float sensor_1;  //menginisialisasi variabel sensor
float sensor_2;  //menginisialisasi variabel sensor
float sensor_3;  //menginisialisasi variabel sensor
long duration1;  //menginisialisasi variabel durasi
long duration2;  //menginisialisasi variabel durasi
long duration3;  //menginisialisasi variabel durasi

int16_t x, y;   //menginisialisasi letak
uint16_t w, h;  //menginisialisasi letak

void setup() {
  Serial.begin(9600);  // initialize serial port

  //RTC
  if (!rtc.begin()) {                       //memulai komunikasi RTC
    Serial.println("RTC Tidak Ditemukan");  //kalimat yang akan ditampilkan ketika RTC tidak ditemukan
  }

  if (!SD.begin(5)) {                     //kondisi ketika RTC tidak ditemukan
    Serial.println("Card Mount Failed");  //hasil yang ditampilkan dari kondisi sebelumnya
    return;                               //kembal ke awal untuk memeriksa
  }

  //OLED
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  //begin sd card
  display.clearDisplay();                     //HAPUS OLET BAWAAN

  //SERVO
  servo.attach(4);  // attaches the servo on pin 9 to the servo object

  //ROLEPLAY DISPLAY
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.getTextBounds("START", 0, 0, &x, &y, &w, &h);
  display.setCursor((SCREEN_WIDTH - w) / 2, 0);
  display.println("START");

  display.getTextBounds("MOTOR", 0, 0, &x, &y, &w, &h);
  display.setCursor((SCREEN_WIDTH - w) / 2, 25);
  display.println("MOTOR");

  display.display();
  delay(2500);
  display.clearDisplay();

  //GERAKKAN SERVO
  Serial.println("30");
  servo.write(30);
  delay(1000);
  Serial.println("90");
  servo.write(90);
  delay(1000);
  Serial.println("170");
  servo.write(170);
  delay(1000);
  Serial.println("90");
  servo.write(90);
  delay(1000);

  //ROLEPLAY DISPLAY II
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.getTextBounds("MOTOR", 0, 0, &x, &y, &w, &h);
  display.setCursor((SCREEN_WIDTH - w) / 2, 0);
  display.println("MOTOR");

  display.getTextBounds("READY", 0, 0, &x, &y, &w, &h);
  display.setCursor((SCREEN_WIDTH - w) / 2, 25);
  display.println("READY");

  display.display();
  delay(1000);
  display.clearDisplay();


  //SENSOR ULTASONIK
  pinMode(TRIG_PIN1, OUTPUT);  // set ESP32 pin to output mode
  pinMode(ECHO_PIN1, INPUT);   // set ESP32 pin to input mode

  pinMode(TRIG_PIN2, OUTPUT);  // set ESP32 pin to output mode
  pinMode(ECHO_PIN2, INPUT);

  pinMode(TRIG_PIN3, OUTPUT);  // set ESP32 pin to output mode
  pinMode(ECHO_PIN3, INPUT);

  //Motor Control
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);

  digitalWrite(motor1Pin1, HIGH);  //Motor dc
}

void loop() {
  // UNTUK WAKTU
  DateTime waktu = rtc.now();

  analogWrite(27, kecepatan);
  analogWrite(26, 0);

  //AMBIL DATA SENSOR 1
  //ROLEPLAY SENSOR
  digitalWrite(TRIG_PIN1, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN1, HIGH);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN1, LOW);

  duration1 = pulseIn(ECHO_PIN1, HIGH);
  sensor_1 = duration1 * SOUND_SPEED / 2;

  //AMBIL DATA SENSOR 2
  digitalWrite(TRIG_PIN2, LOW);
  delayMicroseconds(2);
  // Sets the TRIG_PIN on HIGH state for 10 micro seconds
  digitalWrite(TRIG_PIN2, HIGH);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN2, LOW);

  // Reads the ECHO_PIN, returns the sound wave travel time in microseconds
  duration2 = pulseIn(ECHO_PIN2, HIGH);
  sensor_2 = duration2 * SOUND_SPEED / 2;

  //AMBIL DATA SENSOR 3
  digitalWrite(TRIG_PIN3, LOW);
  delayMicroseconds(2);
  // Sets the TRIG_PIN on HIGH state for 10 micro seconds
  digitalWrite(TRIG_PIN3, HIGH);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN3, LOW);

  // Reads the ECHO_PIN, returns the sound wave travel time in microseconds
  duration3 = pulseIn(ECHO_PIN3, HIGH);
  sensor_3 = duration3 * SOUND_SPEED / 2;

  Serial.print(sensor_1);
  Serial.print(", ");
  Serial.print(sensor_2);
  Serial.print(", ");
  Serial.println(sensor_3);
  display.display();

  //TAMPILAN DATA
  display.setCursor(0, 0);  //oled display
  display.setTextSize(1);
  display.setTextColor(WHITE, BLACK);
  display.print(waktu.day(), DEC);
  display.print('/');
  display.print(waktu.month(), DEC);
  display.print('/');
  display.print(waktu.year(), DEC);
  display.print(" ");

  display.print(waktu.hour(), DEC);
  display.print(':');
  display.print(waktu.minute(), DEC);
  display.print(':');
  display.print(waktu.second(), DEC);
  display.println();
  display.display();



  display.setCursor(0, 15);  //oled display
  display.print("S 1 = ");
  display.print(sensor_1);
  display.print(" cm");
  display.display();

  display.setCursor(0, 25);  //oled display
  display.print(" S 2 = ");
  display.print(sensor_2);
  display.print(" cm");
  display.display();

  display.setCursor(0, 35);  //oled display
  display.print(" S 3 = ");
  display.print(sensor_3);
  display.println(" cm");
  display.display();

  //sensor 2
  //LURUS
  if (sensor_1 == 0 || sensor_2 == 0 || sensor_3 == 0) {  //BELOK KIRI
    keputusan = "0 DETECTED";
    display.getTextBounds("0 DETECTED", 0, 0, &x, &y, &w, &h);
    display.setTextSize(1.5);
    display.setCursor((SCREEN_WIDTH - w) / 2, 45);
    display.println("0 DETECTED");
    display.display();
    display.clearDisplay();
  }

  else if (sensor_2 > jarak && sensor_1 > jarak && sensor_3 > jarak_lurus) {
    keputusan = "LURUS";
    servo.write(90);  // rotate servo motor to 90 degree
    analogWrite(27, kecepatan);
    display.getTextBounds("LURUS", 0, 0, &x, &y, &w, &h);
    display.setTextSize(1.5);
    display.setCursor((SCREEN_WIDTH - w) / 2, 45);
    display.println("LURUS");

    // display.println("  lURUS ");
    display.display();
    display.clearDisplay();
    // delay(500);
  }

  //BELOK KANAN
  else if (sensor_1 < jarak && sensor_2 > jarak && sensor_3 > jarak_lurus) {
    keputusan = "KANAN";
    servo.write(sudutkanan);
    analogWrite(27, kecepatanBelok);
    display.getTextBounds("BELOK KE KANAN", 0, 0, &x, &y, &w, &h);
    display.setTextSize(1.5);
    display.setCursor((SCREEN_WIDTH - w) / 2, 45);
    display.println("BELOK KE KANAN");
    display.display();
    display.clearDisplay();
  }
  //BELOK KIRI
  else if (sensor_1 > jarak && sensor_2 < jarak && sensor_3 > jarak_lurus) {
    keputusan = "KIRI";
    servo.write(sudutkiri);
    analogWrite(27, kecepatanBelok);
    display.getTextBounds("BELOK KE KIRI", 0, 0, &x, &y, &w, &h);
    display.setTextSize(1.5);
    display.setCursor((SCREEN_WIDTH - w) / 2, 45);
    display.println("BELOK KE KIRI");

    display.display();
    display.clearDisplay();
  }
  //DATA LURUS-KANAN
  else if (sensor_3 < jarak_lurus && sensor_2 > jarak && sensor_1 < jarak) {  //BELOK KANAN
    keputusan = "KANAN";
    servo.write(sudutkanan);
    analogWrite(27, kecepatanBelok);
    display.getTextBounds("BELOK KE KANAN", 0, 0, &x, &y, &w, &h);
    display.setTextSize(1.5);
    display.setCursor((SCREEN_WIDTH - w) / 2, 45);
    display.println("BELOK KE KANAN");
    display.display();
    display.clearDisplay();
  }

  //DATA LURUS-KIRI
  else if (sensor_3 < jarak_lurus && sensor_2 < jarak && sensor_1 > jarak) {  //BELOK KIRI
    keputusan = "KIRI";
    servo.write(sudutkiri);
    analogWrite(27, kecepatanBelok);
    display.getTextBounds("BELOK KE KIRI", 0, 0, &x, &y, &w, &h);
    display.setTextSize(1.5);
    display.setCursor((SCREEN_WIDTH - w) / 2, 45);
    display.println("BELOK KE KIRI");
    display.display();
    display.clearDisplay();
  }
  file = SD.open("/KEDUA.txt", FILE_APPEND);
  if (file) {
    Serial.println("Writing to test.txt...");
    file.print(waktu.hour(), DEC);
    file.print(':');
    file.print(waktu.minute(), DEC);
    file.print(':');
    file.print(waktu.second(), DEC);
    file.print("  Jarak 1 = ");   
    file.print(sensor_1);
    file.print("  Jarak 2 = ");
    file.print(sensor_2);
    file.print("  Jarak 3 = ");
    file.print(sensor_3);
    file.print("  ");
    file.println(keputusan);
    file.close();
  } else {
    Serial.println("error opening test.txt");
  }
  delay(100);
}