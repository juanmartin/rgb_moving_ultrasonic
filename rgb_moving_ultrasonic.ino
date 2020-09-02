#include <FastLED.h>
#define FASTLED_ALLOW_INTERRUPTS 0
#include <NewPing.h>

#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     13  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 65  // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.


#define NUM_LEDS    60
#define DATA_PIN    3
#define X_PIN       0
#define Y_PIN       1
#define SW_PIN      2
#define VJOY_PIN    8
#define VPING_PIN   7

#define BRIGHTNESS  24

CRGB leds[NUM_LEDS];

int x = 0;
int y = 0;

uint8_t gHue = 0; // rotating "base color" used by many of the patterns


void startupLEDsTest() {
  // startup blink test to confirm LEDs are working.
  FastLED.setBrightness(BRIGHTNESS);
  fill_solid(leds, NUM_LEDS, CRGB(255, 0, 0)); // fill red
  FastLED.show();
  delay(300);
  fill_solid(leds, NUM_LEDS, CRGB(0, 255, 0)); // fill green
  FastLED.show();
  delay(300);
  fill_solid(leds, NUM_LEDS, CRGB(0, 0, 255)); // fill blue
  FastLED.show();
  delay(300);
  FastLED.clear(true);
  FastLED.show();
  //  FastLED.setBrightness(BRIGHTNESS);

} //end_startupLEDsTest


void setup() {
  Serial.begin(9600);

  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical

  pinMode(VJOY_PIN, OUTPUT);
  digitalWrite(VJOY_PIN, HIGH);
  pinMode(VPING_PIN, OUTPUT);
  digitalWrite(VPING_PIN, HIGH);

  startupLEDsTest();

}

void loop() {
  delay(30);
  EVERY_N_MILLISECONDS( 20 ) {
    gHue++;  // slowly cycle the "base color" through the rainbow
    gHue = gHue % 255;
    Serial.println(gHue);
  }

  int distance = 0;
  distance = sonar.ping_cm();
  //  Serial.println( distance );

  if (!sonar.check_timer()) {
    int pos = beatsin16( 20, 0, NUM_LEDS - 1 );
    //    int pos = quadwave8(NUM_LEDS);
    leds[pos] += CHSV( gHue, 255, 192);
  }

  y = analogRead(Y_PIN);
  int ledY = map(y, 0, 1023, 0, 255);       // color

  int X = map(distance, 0, 100, 0, NUM_LEDS);  // led
  //    Serial.print("X: %d\n",X);

  //  fill_rainbow( &(leds[distance]), NUM_LEDS, 0); // does not work along with ping sensor.
  // //////////// CUIDADO CON EL SPREAD, ME PA QUE NO LE DA EL RAM PORQUE TAMPOCO ANDA EL FILL_RAINBOW.
  dotMas(X, 2, gHue, 150);
  fadeToBlackBy( leds, NUM_LEDS, 50);

  FastLED.show();
  //    FastLED.clear();
}


void dotMas(int pos, int spread, int colorHue, int colorDecay) {
  for (int i = 0; i <= spread; i++) {
    leds[pos - i] += CHSV(colorHue, 255, 192 - colorDecay + i * 2);
    leds[pos + i] += CHSV(colorHue, 255, 192 - colorDecay + i * 2);
  }
  leds[pos] += CHSV(colorHue, 255, 255);
}
