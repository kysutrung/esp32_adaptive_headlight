/*
 * Project name: Smart Matrix Led Headlight Using Adaptive Control Technique
 * Part 2 of 2 - Code for ESP32 module
 * Last modified March 2024
 * Project manager - Trungtaulua
 * Using machine learning tools by EdgeImpulse Inc
*/

#include <TM1637Display.h>
#include <HardwareSerial.h>

#define CLK_PIN 23  // Chân kết nối Clock của TM1637 với ESP32
#define DIO_PIN 22  // Chân kết nối Data của TM1637 với ESP32

#define LED_DO 21    // Chân kết nối LED đỏ
#define LED_VANG 19  // Chân kết nối LED vàng
#define LED_XANH 18  // Chân kết nối LED xanh

TM1637Display display(CLK_PIN, DIO_PIN);

const int RELAY_COUNT = 8;  // Số lượng relay
const int relayPins[RELAY_COUNT] = { 13, 12, 14, 27, 26, 25, 33, 32};

void setup() {
  // Khởi động các cổng serial
  Serial.begin(115200);   //Cổng giao tiếp với màn hình
  Serial2.begin(115200);  //Cổng nhận tín hiệu từ ESP32CAM

  // Khởi tạo các chân điều khiển relay là OUTPUT
  for (int i = 0; i < RELAY_COUNT; i++) {
    pinMode(relayPins[i], OUTPUT);
  }

  // Đèn báo 3 màu
  pinMode(LED_DO, OUTPUT);
  pinMode(LED_VANG, OUTPUT);
  pinMode(LED_XANH, OUTPUT);

  // Điều chỉnh độ sáng màn hình 4 số
  display.setBrightness(0x0f);
}

//Khai báo prototype
void xuLyTinHieu(int a);
void xuLyDen01(int b);
void xuLyDen02(int c);
void batTatCaDen();

void loop() {
  if (Serial2.available()) {
    // Đọc dữ liệu từ UART
    int tinHieuNhanDuoc = Serial2.parseInt();
    // Xuất ra màn hình serial kết quả đầu tiên và xử lý bật tắt đèn
    xuLyTinHieu(tinHieuNhanDuoc);
    // Chờ cho ESP32 thứ nhất gửi dữ liệu tiếp theo
    while (Serial2.available()) {
      Serial2.read();  // Đọc và loại bỏ dữ liệu còn lại trong buffer
    }
  }
}

void xuLyTinHieu(int num1) {
  switch (num1) {
    case 3:
      batTatCaDen();
      digitalWrite(LED_DO, LOW);
      digitalWrite(LED_XANH, HIGH);
      digitalWrite(LED_VANG, LOW);
      display.clear();
      break;
    case 4:
      batTatCaDen();
      digitalWrite(LED_DO, LOW);
      digitalWrite(LED_XANH, LOW);
      digitalWrite(LED_VANG, HIGH);
      display.clear();
      break;
    default:
      xuLyDen01(num1);
      digitalWrite(LED_DO, HIGH);
      digitalWrite(LED_XANH, LOW);
      digitalWrite(LED_VANG, LOW);
      display.showNumberDec(num1);
      break;
  }
  // Xuất ra màn hình serial thông tin tin hiệu nhận được
  Serial.print("Tin hieu nhan duoc: ");
  Serial.println(num1);
}

void xuLyDen01(int num) {
  if (num > 100) {
    xuLyDen02(0, 1, 1, 1, 1, 1, 1, 1);
  }
  if (num > 88 && num < 100) {
    xuLyDen02(1, 0, 1, 1, 1, 1, 1, 1);
  }
  if (num >= 72 && num < 88) {
    xuLyDen02(1, 1, 0, 1, 1, 1, 1, 1);
  }
  if (num >= 56 && num < 72) {
    xuLyDen02(1, 1, 1, 0, 1, 1, 1, 1);
  }
  if (num >= 32 && num < 56) {
    xuLyDen02(1, 1, 1, 1, 0, 1, 1, 1);
  }
  if (num >= 16 && num < 32) {
    xuLyDen02(1, 1, 1, 1, 1, 0, 1, 1);
  }
  if (num >= 8 && num < 16) {
    xuLyDen02(1, 1, 1, 1, 1, 1, 0, 1);
  }
  if (num >= 0 && num < 8) {
    xuLyDen02(1, 1, 1, 1, 1, 1, 1, 0);
  }
  if (num < 0) {
    xuLyDen02(1, 1, 1, 1, 1, 1, 1, 1);
  }
}

void xuLyDen02(int a, int b, int c, int d, int f, int g, int h, int i) {
  digitalWrite(relayPins[0], a);
  digitalWrite(relayPins[1], b);s
  digitalWrite(relayPins[2], c);
  digitalWrite(relayPins[3], d);
  digitalWrite(relayPins[4], f);
  digitalWrite(relayPins[5], g);
  digitalWrite(relayPins[6], h);
  digitalWrite(relayPins[7], i);
}

void batTatCaDen() {
  for (int i = 0; i < RELAY_COUNT; i++) {
    digitalWrite(relayPins[i], HIGH); // Bật tất cả các relay
  }
}

