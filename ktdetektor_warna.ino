int redPin = 11;
int greenPin = 9;
int bluePin = 10;
int buttonPin = 2;
int buttonState = 0; 
int phrPin = 0; //photoresistor pin

//other variables for calibrating and measuring
float calwr, calwg, calwb, calbr, calbg, calbb, r, g, b = 0;


//uncomment this line if using a Common Anode LED
#define COMMON_ANODE

void setup(){
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(phrPin, INPUT);
  Serial.begin(9600);
}
     
void loop(){
  buttonState = digitalRead(buttonPin);
  if(buttonState == 0){ 
    Serial.println("button pressed: calibrating white");   
    calibration();
  }
  else{
    measure();
  }
}

float readColor(int times){
  float avg, total, current = 0;
  for(int n = 0; n <= times; n++){
    current = analogRead(phrPin);
    total += current;
    delay(20);
  } 
  avg = total/times;
  return avg;
}

void setColor(int red, int green, int blue){
  #ifdef COMMON_ANODE
  red = 255 - red;
  green = 255 - green;
  blue = 255 - blue;
  #endif
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

/*
This function is needed because the raw measure is influenced by 
environmental light.
*/
void calibration(){
  //first calibrate with white color
  setColor(255, 0, 0);
  delay(100);
  calwr = readColor(7);
  setColor(0, 255, 0);
  delay(100);
  calwg = readColor(7);
  setColor(0, 0, 255);
  delay(100);
  calwb = readColor(7);
  setColor(0, 0, 0);
  //then wait until the button is pressed again 
  //so we can calibrate with black color
  Serial.println("waiting to calibrate black");
  for(int i = 0; i <= 10; i+=0){
    buttonState = digitalRead(buttonPin);
    if(buttonState == 0){   
      //calibrate with black color 
      setColor(255, 0, 0);
      delay(100);
      calbr = readColor(7);
      setColor(0, 255, 0);
      delay(100);
      calbg = readColor(7);
      setColor(0, 0, 255);
      delay(100);
      calbb = readColor(7);
      setColor(0, 0, 0);
      i = 20;
    } 
    else{
      //nothing
    }  
  }
}
  
void measure(){
  float deltacal = 0;
  setColor(255, 0, 0);
  delay(100);
  deltacal = calwr-calbr;
  r = (readColor(7) - calbr)/(deltacal)*255;
  setColor(0, 255, 0);
  delay(100);
  deltacal= calwg-calbg;
  g = (readColor(7) - calbg)/(deltacal)*255;
  setColor(0, 0, 255);
  delay(100);
  deltacal = calwb-calbb;
  b = (readColor(7) - calbb)/(deltacal)*255;
  Serial.print(int(r));
  Serial.print(",");
  Serial.print(int(g));
  Serial.print(",");
  Serial.println(int(b));
}

