#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_AHTX0.h>
#include <WiFi.h>

const char* ssid = "20520597";
const char* password = "20520597";
const IPAddress serverIP(192, 168, 4, 1);
const int serverPort = 80;
bool isConnected = false; // Khởi tạo biến lưu trạng thái kết nối tới ESP32-Gateway

Adafruit_AHTX0 aht20;
WiFiClient wifiClient;
float temperature;

void setup() {
  Serial.begin(115200);
  aht20.begin();
  Serial.println("Đang kết nối tới mạng Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Đang kết nối tới mạng Wi-Fi...");
    delay(1000);
  }
  Serial.println("Đã kết nối tới mạng Wi-Fi");
}

void loop() {
  sensors_event_t temperature_event;
  aht20.getEvent(NULL, &temperature_event); // Đọc giá trị nhiệt độ từ cảm biến AHT20
  temperature = temperature_event.temperature; // Lưu giá trị nhiệt độ vào biến temperature
  if (isnan(temperature)) { // Kiểm tra nếu không thể đọc giá trị nhiệt độ
    Serial.println("Không thể đọc nhiệt độ từ cảm biến AHT20!"); // In ra thông báo lỗi
    return; // Thoát hàm loop()
  }

  if (!isConnected && wifiClient.connect(serverIP, serverPort)) { // Kiểm tra nếu chưa kết nối tới ESP32-Gateway và kết nối thành công
    Serial.println("Đã kết nối tới ESP32-Gateway"); // Thông báo đã kết nối thành công
    isConnected = true; // Đặt giá trị của biến isConnected thành true để đánh dấu kết nối đã được thiết lập
  }
  
  if (isConnected) { // Kiểm tra nếu đã kết nối tới ESP32-Gateway
    String data = "Nhiệt độ: " + String(temperature); // Tạo chuỗi dữ liệu để gửi lên ESP32-Gateway
    wifiClient.write(data.c_str(), data.length()); // Ghi chuỗi dữ liệu lên kết nối Wi-Fi tới ESP32-Gateway
  } else {
    Serial.println("Chưa kết nối tới ESP32-Gateway!"); // Thông báo lỗi nếu chưa kết nối tới ESP32-Gateway
  }
  
  delay(2000); 
}