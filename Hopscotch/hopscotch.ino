#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
//#3-10
//13-20
// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        12 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 50 // Popular NeoPixel ring size
#define MIDPOINT 75
#define BTNNUM 7
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 5 // Time (in milliseconds) to pause between pixels
// Variables will change:
int lastState = HIGH; // the previous state from the input pin
int currentState;     // the current reading from the input pin
int progressTracker = 1;
int btns[] = {2,22,17,25,26,21,32};
int btnLights[][20] = {{3,4,19,20}, {5},{18}, {6,7,16,17}, {8,15},{25}, {9,10,13,14}};
int btnLightsTotal[] = {3,4,5,9,10,13,14,15,16,17,18,19,20};

bool btnState[] = {false, false, false, false};
void setup() {

  Serial.begin(115200);
  // initialize the pushbutton pin as an pull-up input
  for(int i =0; i< sizeof(btns)/sizeof(int); i++){
      pinMode(btns[i], INPUT_PULLUP);
  }
    pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
    for(int i =0; i< NUMPIXELS; ++i){
            setColor(i,0,30,0);

  }
}

void loop() {
  for(int i = 0; i< BTNNUM; ++i){    
    currentState = digitalRead(btns[i]);
    if(currentState == LOW){
      changeLights(btnLights[i], 0,sizeof(btnLights[i])/sizeof(int),0,0,30);

      if(i == progressTracker || i==progressTracker+1 || i==progressTracker-1){
        progressTracker++;
      }else{
        Serial.println("OUT OF ORDER");
      }
        if(progressTracker >= 7 && i==6){
          celebrate(btnLightsTotal,16);
          progressTracker=0;
        }
      Serial.printf("PROGRESS TRACKER %d\n", progressTracker);
    }else if(currentState == HIGH){
      changeLights(btnLights[i], 0, sizeof(btnLights[i])/sizeof(int),30,0,0); 

    }

  } 

  pixels.show();  

}

void setColor(int pos, int r,int g, int b) {
  if (pos < 100){
     pixels.setPixelColor(pos, pixels.Color(g,r,b));
  }
  else if (pos >= 100 && pos < 388) {
    if (pos >=103 && pos % 4 == 0){
     pixels.setPixelColor(pos-3, pixels.Color(r,g,b));
     pixels.setPixelColor(pos-2, pixels.Color(r,g,b));
     pixels.setPixelColor(pos-1, pixels.Color(r,g,b));
     pixels.setPixelColor(pos, pixels.Color(r,g,b));
    }
  }
}
void changeLights(int lights[], int start,int end, int r,int g, int b){
  for(int i =start; i<end; ++i){
    setColor(lights[i], r,g,b);
  }
}

void celebrate(int lights[], int size) {

    Serial.println("yay");

    changeLights(lights, 0,size/4,0,0,30);
      pixels.show();  

    delay(500);
    changeLights(lights, size/4, size/2, 30,0,0);
      pixels.show();  

    delay(500);
    changeLights(lights, size/2, size*3/4, 0,30,0);
      pixels.show();  

    delay(500);
    changeLights(lights, size*3/4, size, 10,30,30);
   pixels.show();  

  delay(2000);
    /*
    for(int j =0; j<size/4; ++j){
      setColor(lights[j],255,0,0);
    }
    for(int j = size/4; j <size/2; ++j){
      setColor(lights[j],0,255,0);

    }
    for(int j=size/2; j<size*(3/4); ++j){
      setColor(lights[j],0,0,255);

    }
    for(int j=size*(3/4); j<size; ++j){
      setColor(lights[j],255,0,0);

    }
    */
}