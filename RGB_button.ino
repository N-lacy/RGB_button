#include "RGB_Button.h"

int state = 0;
int old = 0;
int buttonPoll = 0;

RGB_Button emergency_button(44, 45, 46, 43); // R, G, B, push_button pins


unsigned long previousMillis = 0;
const long interval = 20; // Interval for color update in milliseconds

int startColor[3] = {255, 0, 0};  // Initial color: Red
int endColor[3] = {0, 255, 0};    // Target color: Green
int steps = 100;                  // Number of steps for the transition
int currentStep = 0;              // Current step of the transition

bool em = false;                  // Boolean to control the blinking red feature
bool blinkState = false;          // Boolean to toggle the blink state
unsigned long blinkPreviousMillis = 0;
const long blinkInterval = 500;

unsigned long last_toggle = 0;
const long em_toggle_interval = 10000;


void setup() {
  
  Serial.begin(57600);
  emergency_button.init_RGBbutton();
  
  emergency_button.setColor(startColor);
  emergency_button.runColor();
 
}


void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - last_toggle >= em_toggle_interval) {
    em = true;
    last_toggle = currentMillis;
  }
  

  if (em) {
    // Handle blinking red when em is true
    if (currentMillis - blinkPreviousMillis >= blinkInterval) {
      blinkPreviousMillis = currentMillis;
      blinkState = !blinkState;
      if (blinkState) {
        emergency_button.setColor(255, 0, 0); // Red
      } else {
        emergency_button.setColor(0, 0, 0); // Off
      }
    }
  } else {
    // Handle normal color transitions
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      updateColor();
    }
  }

  
  Serial.println(emergency_button.buttonState());
  delay(100);

  if (emergency_button.buttonState()) {
    emergency_button.setColor((const int[]){255,255,255});
    em = false;
    last_toggle = currentMillis;
  }

  
  emergency_button.runColor();


//  int red, green, blue;
  int colorArray[3];
  
  // Using getColor method with individual RGB values
//  emergency_button.getColor(red, green, blue);
//  Serial.print("Red: ");
//  Serial.print(red);
//  Serial.print(" Green: ");
//  Serial.print(green);
//  Serial.print(" Blue: ");
//  Serial.println(blue);

  // Using getColor method with an RGB array
  emergency_button.getColor(colorArray);
  Serial.print("Red: ");
  Serial.print(colorArray[0]);
  Serial.print(" Green: ");
  Serial.print(colorArray[1]);
  Serial.print(" Blue: ");
  Serial.println(colorArray[2]);
}

void updateColor() {
  int color[3];
  for (int i = 0; i < 3; i++) {
    color[i] = startColor[i] + ((endColor[i] - startColor[i]) * currentStep) / steps;
  }
  emergency_button.setColor(color);
  currentStep++;

  if (currentStep > steps) {
    currentStep = 0;

    // Move to the next color transition
    for (int i = 0; i < 3; i++) {
      startColor[i] = endColor[i];
    }
    getNextColor(endColor);
  }
  
}

void getNextColor(int color[3]) {
  static int colorIndex = 0;
  int colors[][3] = {
    {255, 0, 0},   // Red
    {0, 255, 0},   // Green
    {0, 0, 255},   // Blue
    {255, 255, 0}, // Yellow
    {0, 255, 255}, // Cyan
    {255, 0, 255}, // Magenta
    {255, 255, 255} // White
  };

  for (int i = 0; i < 3; i++) {
    color[i] = colors[colorIndex][i];
  }

  colorIndex = (colorIndex + 1) % (sizeof(colors) / sizeof(colors[0]));
  
}
