# 1 "/www/wwwroot/HTX/MayChiecRuou/MayChiecRuou.ino"
# 2 "/www/wwwroot/HTX/MayChiecRuou/MayChiecRuou.ino" 2
# 3 "/www/wwwroot/HTX/MayChiecRuou/MayChiecRuou.ino" 2
# 4 "/www/wwwroot/HTX/MayChiecRuou/MayChiecRuou.ino" 2


# 7 "/www/wwwroot/HTX/MayChiecRuou/MayChiecRuou.ino" 2
# 8 "/www/wwwroot/HTX/MayChiecRuou/MayChiecRuou.ino" 2
Preferences preferences;
WebServer server(80);

int button1 = 32;
int button2 = 27;

int percentHigh1 = 80;
int percentHigh2 = 80;

int motor1 = 17;
int motor2 = 4;

const int freq = 5000; // Tần số PWM (5 kHz)
const int resolution = 8; // Độ phân giải 8 bit (0-255)
const int maxDuty = 255;
bool button1_state = 0x0;
bool button2_state = 0x0;
bool motor1_state = 0x0;
bool motor2_state = 0x0;
int timeRun1 = 8500; // 13s
int timeRun2 = 8500; // 13s
int minMotorSpeed1 = 80;
int minMotorSpeed2 = 80;
unsigned long delay1 = 0;
unsigned long delay2 = 0;
IPAddress local_ip(192, 168, 2, 1);
IPAddress gateway(192, 168, 2, 1);
IPAddress subnet(255, 255, 255, 0);
void handle_NotFound()
{
    server.send(404, "text/plain", "Not found");
}
void setup()
{
    // Tắt Watchdog Timer hệ thống
    esp_task_wdt_deinit();
    Serial0.begin(115200);
    ledcAttach(motor1, freq, resolution);
    ledcAttach(motor2, freq, resolution);

    ledcWrite(motor1, 0);
    ledcWrite(motor2, 0);

    readFromEEPROM();

    Serial0.println("Started !!");
    Serial0.println("Motor 1: " + String(motor1));
    Serial0.println("Motor 2: " + String(motor2));
    Serial0.println("Button 1: " + String(button1));
    Serial0.println("Button 2: " + String(button2));
    Serial0.println("timeRun1: " + String(timeRun1));
    Serial0.println("timeRun2: " + String(timeRun2));
    Serial0.println("minMotorSpeed1: " + String(minMotorSpeed1));
    Serial0.println("minMotorSpeed2: " + String(minMotorSpeed2));
    Serial0.println("percentHigh1: " + String(percentHigh1));
    Serial0.println("percentHigh2: " + String(percentHigh2));

    motor1_state = 0x0;
    motor2_state = 0x0;

    pinMode(button1, 0x05);
    pinMode(button2, 0x05);

    button1_state = digitalRead(button1);
    button2_state = digitalRead(button2);

    // Serial.println("Started !!");
    //  Connect to Wi-Fi network with SSID and password
    Serial0.print("Bắt đầu tạo AP..");
    // Remove the password parameter, if you want the AP (Access Point) to be open
    WiFi.mode(WIFI_MODE_AP);
    if (!WiFi.softAP("HTXXANH"))
    {
        Serial0.print("Không thể tạo HTXXANH");
        while (1)
            ;
    }
    Serial0.println("Đang khởi động AP");
    delay(100);
    IPAddress IP = WiFi.softAPIP();
    Serial0.print("AP IP address: ");
    Serial0.println(IP);
    server.on("/", handleRoot);
    server.onNotFound(handle_NotFound);
    server.begin();
    Serial0.println("Đã sẵn sàng");
}
void saveToEEPROM()
{
    preferences.begin("storage", false);
    preferences.putInt("timeRun1", timeRun1);
    preferences.putInt("timeRun2", timeRun2);
    preferences.putInt("minMotorSpeed1", minMotorSpeed1);
    preferences.putInt("minMotorSpeed2", minMotorSpeed2);
    preferences.putInt("percentHigh1", percentHigh1);
    preferences.putInt("percentHigh2", percentHigh2);
    preferences.end();
    Serial0.println("Data saved!");
}
void readFromEEPROM()
{
    preferences.begin("storage", true);
    timeRun1 = preferences.getInt("timeRun1", 8500);
    timeRun2 = preferences.getInt("timeRun2", 8500);
    minMotorSpeed1 = preferences.getInt("minMotorSpeed1", 80);
    minMotorSpeed2 = preferences.getInt("minMotorSpeed2", 80);
    percentHigh1 = preferences.getInt("percentHigh1", 80);
    percentHigh2 = preferences.getInt("percentHigh2", 80);

    preferences.end();
    Serial0.println("Đã tải cấu hình!");
}
void handleRoot()
{
    // Kiểm tra xem URL có tham số "time" hay không
    String alert = "";
    if (server.hasArg("percentHigh1") && server.hasArg("percentHigh2") && server.hasArg("timeRun1") && server.hasArg("timeRun2") && server.hasArg("minMotorSpeed1") && server.hasArg("minMotorSpeed2"))
    {
        timeRun1 = server.arg("timeRun1").toInt();
        timeRun2 = server.arg("timeRun2").toInt();
        minMotorSpeed1 = server.arg("minMotorSpeed1").toInt();
        minMotorSpeed2 = server.arg("minMotorSpeed2").toInt();
        percentHigh1 = server.arg("percentHigh1").toInt();
        percentHigh2 = server.arg("percentHigh2").toInt();
        alert = "Đã lưu cấu hình thành công !";
        saveToEEPROM();
    }
    // Tạo nội dung phản hồi HTML
    String html = ("<!doctype html>");
    html += ("<html lang=\"vi\">");
    html += ("  <head>");
    html += ("    <!-- Required meta tags -->");
    html += ("    <meta charset=\"utf-8\">");
    html += ("    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
    html += ("    <!-- Bootstrap CSS -->");
    html += ("    <link href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css\" rel=\"stylesheet\">");
    html += ("    <title>Điều chỉnh máy chiết rượu</title>");
    html += ("  </head>");
    html += ("  <body>");
    html += ("    <div class=\"container mt-5\">");
    html += ("      <h1 class=\"mb-4 text-center\">Điều chỉnh máy chiết rượu</h1>");
    if (alert != "")
    {
        html += ("<div class='alert alert-success' role='alert'>" + alert + "</div>");
    }
    html += ("      <form method=\"get\">");
    html += ("        <div class=\"mb-3\">");
    html += ("          <label for=\"minMotorSpeed1\" class=\"form-label\">Tốc độ nhỏ nhất Motor 1</label>");
    html += ("          <input type=\"number\" class=\"form-control\" id=\"minMotorSpeed1\" name=\"minMotorSpeed1\" value=\"" + (String)minMotorSpeed1 + "\" placeholder=\"Nhập tốc độ Motor 1\" min=\"0\" max=\"255\">");
    html += ("        </div>");
    html += ("        <div class=\"mb-3\">");
    html += ("          <label for=\"timeRun1\" class=\"form-label\">Thời gian chạy Motor 1 (ms)</label>");
    html += ("          <input type=\"number\" class=\"form-control\" id=\"timeRun1\" name=\"timeRun1\" value=\"" + (String)timeRun1 + "\"  placeholder=\"Nhập thời gian chạy Motor 1\">");
    html += ("        </div>");
    html += ("        <div class=\"mb-3\">");
    html += ("          <label for=\"percentHigh1\" class=\"form-label\">Phần trăm cao Motor 1</label>");
    html += ("          <input type=\"number\" class=\"form-control\" id=\"percentHigh1\" name=\"percentHigh1\" value=\"" + (String)percentHigh1 + "\" placeholder=\"Nhập phần trăm cao Motor 1\" min=\"0\" max=\"100\">");
    html += ("        </div>");
    html += ("        <div class=\"mb-3\">");
    html += ("          <label for=\"minMotorSpeed2\" class=\"form-label\">Tốc độ nhỏ nhất Motor 2</label>");
    html += ("          <input type=\"number\" class=\"form-control\" id=\"minMotorSpeed2\" name=\"minMotorSpeed2\" value=\"" + (String)minMotorSpeed2 + "\" placeholder=\"Nhập tốc độ Motor 2\" min=\"0\" max=\"255\">");
    html += ("        </div>");
    html += ("        <div class=\"mb-3\">");
    html += ("          <label for=\"timeRun2\" class=\"form-label\">Thời gian chạy Motor 2 (ms)</label>");
    html += ("          <input type=\"number\" class=\"form-control\" id=\"timeRun2\" name=\"timeRun2\" value=\"" + (String)timeRun2 + "\" placeholder=\"Nhập thời gian chạy Motor 2\">");
    html += ("        </div>");
    html += ("        <div class=\"mb-3\">");
    html += ("          <label for=\"percentHigh2\" class=\"form-label\">Phần trăm cao Motor 2</label>");
    html += ("          <input type=\"number\" class=\"form-control\" id=\"percentHigh2\" name=\"percentHigh2\" value=\"" + (String)percentHigh2 + "\" placeholder=\"Nhập phần trăm cao Motor 2\" min=\"0\" max=\"100\">");
    html += ("        </div>");
    html += ("        <button type=\"submit\" class=\"btn btn-primary\">Lưu</button>");
    html += ("      </form>");
    html += ("    </div>");
    html += ("    <!-- Bootstrap Bundle with Popper -->");
    html += ("    <script src=\"https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/js/bootstrap.bundle.min.js\"></script>");
    html += ("  </body>");
    html += ("</html>");
    // Gửi phản hồi với mã 200 (OK) và kiểu nội dung là text/html
    server.send(200, "text/html", html);
}
void loop()
{
    server.handleClient();
    if (digitalRead(button1) != button1_state)
    {
        delay(10);
        if (digitalRead(button1) != button1_state)
        {
            button1_state = digitalRead(button1);
            if (button1_state == 0x1)
            {
                Serial0.println("Button 1 pressed");
                if (motor1_state)
                {
                    // Nếu đang chạy sẽ dừng
                    ledcWrite(motor1, 0);
                    motor1_state = 0x0;
                    Serial0.println("Turn motor 1 off");
                    delay1 = 0;
                }
                else
                {
                    ledcWrite(motor1, maxDuty);
                    motor1_state = 0x1;
                    delay1 = millis();
                    Serial0.println("Turn motor 1 on");
                }
            }
        }
    }
    if (digitalRead(button2) != button2_state)
    {
        delay(10);
        if (digitalRead(button2) != button2_state)
        {
            button2_state = digitalRead(button2);
            if (button2_state == 0x1)
            {
                Serial0.println("Button 2 pressed");
                if (motor2_state)
                {
                    // Nếu đang chạy sẽ dừng
                    ledcWrite(motor2, 0);
                    motor2_state = 0x0;
                    Serial0.println("Turn motor 2 off");
                    delay2 = 0;
                }
                else
                {
                    ledcWrite(motor2, maxDuty);
                    motor2_state = 0x1;
                    delay2 = millis();
                    Serial0.println("Turn motor 2 on");
                }
            }
        }
    }
    if (motor1_state)
    {
        if ((millis() - delay1) > timeRun1)
        {
            ledcWrite(motor1, 0);
            motor1_state = 0x0;
            Serial0.println("Turn motor 1 off");
            delay1 = 0;
        }
        else
        {
            unsigned long elapsed1 = millis() - delay1;
            int dutyCycle1 = maxDuty;
            // Tính duty cycle theo hàm mũ giảm dần
            Serial0.println(round((elapsed1 * 100) / delay1));
            if (round((elapsed1 * 100) / delay1) > percentHigh1)
            {
                dutyCycle1 = minMotorSpeed1;
            }
            // Debug: in ra giá trị thời gian và duty cycle
            // Cập nhật giá trị PWM
            ledcWrite(motor1, dutyCycle1);
            Serial0.println("dutyCycle motor 1: " + (String)dutyCycle1);
        }
        Serial0.println("Delay1: " + String(millis() - delay1));
    }
    if (motor2_state)
    {
        if ((millis() - delay2) > timeRun2)
        {
            ledcWrite(motor2, 0);
            motor2_state = 0x0;
            Serial0.println("Turn motor 2 off");
            delay2 = 0;
        }
        else
        {
            unsigned long elapsed2 = millis() - delay2;
            // Tính duty cycle theo hàm mũ giảm dần
            int dutyCycle2 = maxDuty;
            Serial0.println(round((elapsed2 * 100) / delay2));
            if (round((elapsed2 * 100) / delay2) > percentHigh2)
            {
                dutyCycle2 = minMotorSpeed2;
            }
            ledcWrite(motor2, dutyCycle2);
            Serial0.println("dutyCycle motor 2: " + (String)dutyCycle2);
        }
        Serial0.println("Delay2: " + String(millis() - delay2));
    }
    delay(100);
}
