#include <WiFi.h>

const char* ssid = "20520597"; // Tên mạng Wi-Fi
const char* password = "20520597"; // Mật khẩu mạng Wi-Fi
const int serverPort = 80; // Cổng máy chủ

WiFiServer wifiServer(serverPort); // Khởi tạo máy chủ Wi-Fi
WiFiClient wifiClient; // Khởi tạo đối tượng client Wi-Fi

void setup() {
  Serial.begin(115200); // Khởi tạo Serial với tốc độ baud 115200
  while (!Serial) { // Chờ Serial khởi động
    delay(10);
  }
  
  WiFi.softAP(ssid, password); // Tạo điểm phát Wi-Fi với tên và mật khẩu đã chỉ định
  IPAddress myIP = WiFi.softAPIP(); // Lấy địa chỉ IP của điểm phát Wi-Fi
  Serial.print("Địa chỉ IP của ESP32 là: ");
  Serial.println(myIP); // In địa chỉ IP của ESP32 trên Serial Monitor

  wifiServer.begin(); // Bắt đầu chạy máy chủ Wi-Fi
  Serial.println("Đang chờ kết nối đến ESP32-Node để lấy dữ liệu ...");
}

void loop() {
  wifiClient = wifiServer.available(); // Chấp nhận kết nối từ đối tượng client Wi-Fi

  if (wifiClient) {
    while (wifiClient.connected()) {
      if (wifiClient.available()) {
        char data[50];
        int dataLength = wifiClient.readBytes(data, 50); // Đọc dữ liệu từ đối tượng client Wi-Fi và lưu vào mảng data
        data[dataLength] = '\0'; // Thêm ký tự kết thúc chuỗi vào cuối mảng data
        Serial.println(data); // In dữ liệu lên Serial Monitor
      }
    }
  }
}