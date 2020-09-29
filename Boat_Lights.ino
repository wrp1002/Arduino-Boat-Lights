#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
ESP8266WebServer webServer(80);

const int LED_PIN     =  D8;
const int LED_COUNT   =  60;
const int INIT_BRIGHT =  50;
const int MAX_COLOR_MODE = 9;
const char* NETWORK_NAME = "Lights";
const int KIT_WIDTH = 3;

enum COLOR_MODES {COLOR, COLOR_WHITE, COLOR_BLUE, COLOR_RED, RAINBOW, FADE, LUCKY_CHARMS, KIT, SPACE};


struct RotaryDial {
  int prev;
  int pin1;
  int pin2;
  int buttonPin;
  int count;
};

struct StripData {
  int colorMode;
  int stepSize;
  int sleepTimerStart;
  int sleepTimer;
  bool lightsOn;
  int prevBrightness;
  long targetColor;
  int fadeTimer;
  int fadeTimerStart;
  int dir;
} stripData;

struct RotaryDial rots[2];
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif

  strip.begin();
  strip.setBrightness(INIT_BRIGHT);
  strip.show();
  
  srand(analogRead(0));
  Serial.begin(9600);
  delay(500);

  rots[0].pin1 = D1;
  rots[0].pin2 = D3;
  rots[0].buttonPin = D4;
  rots[0].prev = 0;
  rots[0].count = 0;

  rots[1].pin1 = D5;
  rots[1].pin2 = D6;
  rots[1].buttonPin = D7;
  rots[1].prev = 0;
  rots[1].count = INIT_BRIGHT;

  stripData.colorMode = COLOR_WHITE;
  stripData.stepSize = 2;
  stripData.sleepTimer = 0;
  stripData.sleepTimerStart = 250;
  stripData.lightsOn = true;
  stripData.prevBrightness = INIT_BRIGHT;
  stripData.targetColor = 0;
  stripData.fadeTimer = 0;
  stripData.fadeTimerStart = 60000;

  for (int i = 0; i < 2; i++) {
    pinMode(rots[i].pin1, INPUT_PULLUP);
    pinMode(rots[i].pin2, INPUT_PULLUP);
    pinMode(rots[i].buttonPin, INPUT_PULLUP);
  }

  attachInterrupt(digitalPinToInterrupt(rots[0].pin1), UpdatePre1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(rots[0].buttonPin), UpdatePre1, FALLING);

  attachInterrupt(digitalPinToInterrupt(rots[1].pin1), UpdatePre2, CHANGE);
  attachInterrupt(digitalPinToInterrupt(rots[1].buttonPin), UpdatePre2, FALLING);

  UpdateDials(0);
  UpdateDials(1);

  SetColorMode(SPACE);

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(NETWORK_NAME);

  // if DNSServer is started with "*" for domain name, it will reply with
  // provided IP to all DNS request
  dnsServer.start(DNS_PORT, "*", apIP);

  // replay to all requests with same HTML

  webServer.on("/", HTTP_GET, HandleRoot);
  webServer.on("/color", HTTP_GET, HandleColor);
  webServer.onNotFound(HandleRoot);
  webServer.begin();

  Serial.println("Initialized");
}

void loop() {
  dnsServer.processNextRequest();
  webServer.handleClient();

  if (stripData.sleepTimer > 0)
    stripData.sleepTimer--;

  UpdateColors();

  delay(1);
}
