#include <Arduino.h>

#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1   // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C // If not work please scan the bus
#define OLED_SDA 14         // D5
#define OLED_SCL 12         // D6

Adafruit_SSD1306 display(SCREEN_WIDTH,SCREEN_HEIGHT,&Wire);


// Inicializar la comunicacion serie
#include <SoftwareSerial.h>
SoftwareSerial miSerial(14, 12);// Pines RX=D5 (14), TX=D6 (12)


void setup () {
  Serial.begin(115200);  // We initialize serial connection so that we could print values from sensor.
 
  // Conexion UART con el otro ESP
  miSerial.begin(9600);


  Wire.begin(OLED_SDA, OLED_SCL);
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE); 
  display.setCursor(0,0);
}

void loop () {
  
  // Every 1 second, do a measurement using the sensor and print the distance in centimeters.
  int valor = analogRead(A0);
  int humedad = map(valor, 1023, 0, 0, 100);

  display.clearDisplay();
  display.setCursor(0,0);
  display.print(humedad);
  display.print("% de humedad");
  display.display();
  
  miSerial.println(humedad);


}
