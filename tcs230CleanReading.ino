/* 
 * Project: Color reading with TCS230 module
 * Autor: Pedro Igo Sousa Lucas
 * Code written over Danilo Nogueira`s code (http://autocorerobotica.blog.br/utilizando-sensor-de-cor-tcs230-com-arduino/). Access 10/02/2020
*/

// ==================================================================================================*/

// --- Pin connection ---
  #define    S2    9
  #define    S3    10
  #define    OUT   8

// --- Reading tolerance ---
  #define    tolerance 5

// --- How many colors set. Must be at least 1 ---
  #define    colorNumber 3

// --- Base delay for color reading ---
  #define Delay 50

// ==================================================================================================

// --- Variables ---
  // Store each color read by sensor
  int red;   
  int green;
  int blue;
  // Matrix with RGB values set to each color (Color x RGB)
  int color [colorNumber][3] = { 0 };
  
// ==================================================================================================

void setup() 
{
  // S2 and S3 tell sensor which color to send over
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  // Sensor returns color value
  pinMode(OUT, INPUT);
  
  Serial.begin(9600);
  
  // Starting calibration
  calibrate();
}
 
// ==================================================================================================

void loop() 
{
  Serial.println("To leave, press any key and enter");
  Serial.println("Starting in 3 seconds...");
  delay(3000);
  loopRead();
  calibrate();
}

// ==================================================================================================

// --- Color calibration ---
void calibrate()
{
  for(int i = 0; i < colorNumber; i++)
  {
    Serial.print("Calibrating color ");
    Serial.print(i);
    Serial.println(". Press any key and enter when ready");
    getChar();
    getRGB();
    color[i][0] = red;
    color[i][1] = green;
    color[i][2] = blue;
    Serial.println("Calibrated.");
  }
}

// --- Serial input ---
char getChar()
{
  while (Serial.available() == 0)
  {}
  // Returns uppercase version of any alpha char
  return(toupper(Serial.read()));
}

// --- Color reading ---
void getRGB()
{
  // Reading filter for RED
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  
  // Read pulse in LOW
  red = pulseIn(OUT, LOW);
  delay(10);
 
  // Reading filter for GREEN
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  
  green = pulseIn(OUT, LOW);
  delay(10);
 
  // Reading filter for blue
  digitalWrite(S2, LOW);
  digitalWrite(S3,HIGH);
  
  blue = pulseIn(OUT, LOW);
  delay(10);
} 

// --- Tolerance interval check ---
bool inInterval(int value,int center)
{
  if ((value <= center + tolerance) && (value >= center - tolerance))
  {
    return true;
  }
  return false;
}

// --- Color identification ---
int readColor()
{
  getRGB();
  for(int i = 0; i < colorNumber; i++)
  {
    if (inInterval(red, color[i][0]) &&
        inInterval(green, color[i][1]) && 
        inInterval(blue, color[i][2]))
    {
      return i;
    }
  }
  return -1; // No match value
}

// --- Color reading (loop) ---
void loopRead()
{
  bool loop = true;
  while(loop)
  {
    Serial.println(readColor());
    if (Serial.available() > 0)
    {
      Serial.read();
      loop = false;
    }
    delay(Delay);
  }
}
