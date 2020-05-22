// arduino sketch designed for a ATMega328P (5v 16 Hz) on a Pro Mini, connected to two daisy-chained Luminati led boards
#include "FastLED.h"

//Number of LEDs
#define NUM_LEDS 16

//Define our clock and data lines
#define DATA_PIN 11
#define CLOCK_PIN 13

//Create the LED array
CRGB leds[NUM_LEDS];

#define BRIGHTNESS 10

bool bright_direction = 0;
int start_bright = 0;
int curr_bright;

const int MY_PALETTE_SIZE = 5832;
// 256/15 +1 = 18, so 18 ^ 3 = 5832


void setup() { 

      //Tell FastLED what we're using. Note "BGR" where you might normally find "RGB".
      //This is just to rearrange the order to make all the colors work right.
      FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR>(leds, NUM_LEDS);

      //Set global brightness
      FastLED.setBrightness(start_bright);
      curr_bright = start_bright;
      Serial.begin(115200);
      randomSeed(analogRead(0));
}

CRGB get_random_color()
{
  // produces totally random colors. Maybe TOO random
  // maybe round to the nearest 5 or 10 or 15 value to reduce color set?
  CRGB color;
  color.red = random(256);
  color.green = random(256);
  color.blue = random(256);
  Serial.print(color.red);Serial.print("\t");
  Serial.print(color.green);Serial.print("\t");
  Serial.println(color.blue);
  return color;
}

int get_new_brightness(int old)
{
  // Serial.println(old);
  if (old >= BRIGHTNESS) {
    bright_direction = 1;
    return BRIGHTNESS - 1;
  }
  if (old == 0) {
    bright_direction = 0;
    return 1;
  }
  if (!bright_direction)
    return old + 1;
  if (bright_direction)
    return old - 1;
  
}

void smooth_color_pulse_once(CRGB color, uint16_t wait) { 

  uint8_t x;
  curr_bright = 0;

  do
  {
    curr_bright = get_new_brightness(curr_bright);
    for (x = 0; x < NUM_LEDS; x++)
    {
      leds[x] = color;
      FastLED.setBrightness(curr_bright);
      FastLED.show();
    }
    delay(wait);
    if (curr_bright == 0)
      break;
  } while (1);

}

void smooth_blue_pulse() { 

  uint8_t x;
  uint16_t wait = 200;
  uint8_t count = 0;

  do
  {
    curr_bright = get_new_brightness(curr_bright);
    for (x = 0; x < NUM_LEDS; x++)
    {
      leds[x] = CRGB::Blue;
      FastLED.setBrightness(curr_bright);
      FastLED.show();
    }
    delay(wait);
    count += 1;
  } while (count < 10);

}

void run_all_colors_mod_15()
{
  FastLED.setBrightness(10);
  for (int x = 0; x <= 255; x+=15)
  {
    for (int y = 0; y <= 255; y+=15)
    {
      for (int z = 0; z <= 255; z+=15)
      {
        for (int l = 0; l < NUM_LEDS; l++)
        {
          leds[l] = CRGB(z, y, x);
          FastLED.show();
          delay(5);
        }
      }
    }
  }
}

// a lot faster cycling than mod 15
void run_all_colors_mod_51()
{
  FastLED.setBrightness(10);
  for (int x = 0; x <= 255; x+=51)
  {
    for (int y = 0; y <= 255; y+=51)
    {
      for (int z = 0; z <= 255; z+=51)
      {
        for (int l = 0; l < NUM_LEDS; l++)
        {
          leds[l] = CRGB(z, y, x);
          FastLED.show();
          delay(5);
        }
      }
    }
  }
}


void loop() {

  for (int x = 0; x < 10; x++)
      smooth_color_pulse_once(CRGB::Red, 200);
  for (int x = 0; x < 10; x++)
      smooth_color_pulse_once(CRGB::Blue, 200);
  // run_all_colors_mod_51();
}
