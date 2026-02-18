#include <DHT.h>

#define TRIG_PIN 9
#define ECHO_PIN 8
#define TRIGGER_PIN 7
#define DHT_PIN 2
#define DHT_TYPE DHT11

DHT dht(DHT_PIN,DHT_TYPE);


struct Kalman{
  float x; //estimated distance (m)
  float P; //estimation error covariance
  float Q; //process noise
  float R; //measurement noise
};

Kalman kf;

//Global variables
float threshold = 0.5;
bool triggered = false;
float bias = 0.0; //auto-calibration bias
float knownDistance = 2.7;// known distance during calibration

float speedOfSound(float tempC){
  return 331.3 + 0.606 * tempC;
}

//measure distance
float measureDistance(float tempC){
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, HIGH);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000); // 30Ms timeout
  if(duration==0) return -1;
  float v = speedOfSound(tempC);
  float distance = (duration * 1e-6 * v)/2.0;

  return distance;
}

//auto-calibration
float calibrate(float tempC){
  const int N = 50;
  float sum = 0.0;
  int valid = 0;

  for(int i=0; i<N; i++){
    float d = measureDistance(tempC);
    if(d>0.2 && d<4.0){
      sum += d;
      valid++; 
    }
    delay(50);
  }
  if(valid==0) return 0;
  return (sum/valid)/ - knownDistance; //offset
}

//initialize kalman filter
void kalmanInit(float initialValue){
  kf.x= initialValue;
  kf.P = 1.0;
  kf.Q = 0.01;
  kf.R = 0.1;
}

//kalman update
float kalmanUpdate(float z){
  // kalman update
  kf.P += kf.Q;

  //kalman Gain
  float K = kf.P/(kf.P + kf.R);

  //correction
  kf.x += K*(z-kf.x);
  kf.P *= (1-K);

  return kf.x;
}

void setup() {
  Serial.begin(9600);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  dht.begin();
  delay(2000); // wait for dht to stabilize

  Serial.println("Calibrating...");

  float temp = dht.readTemperature();
  if(isnan(temp)) temp = 25.0;

  bias = calibrate(temp);

  float firstMeasurement = measureDistance(temp) - bias;
  kalmanInit(firstMeasurement);

  Serial.print("calibration bias: ");
  Serial.print(bias, 4);
  Serial.print(" m");
}

void loop() {
  //Serial.println("loop");
  float temp = dht.readTemperature();
  if(isnan(temp)) temp = 25.0;

  float raw = measureDistance(temp);
  if(raw < 0.02 || raw > 4.0) return;

  float corrected = raw - bias;
  float filtered = kalmanUpdate(corrected);

  //send trigger to esp32 if threshold is passed
  if(!triggered && filtered <= threshold){
    digitalWrite(TRIGGER_PIN, HIGH);
    delay(2);
    digitalWrite(TRIGGER_PIN, LOW);
    triggered = true;
    Serial.println("Interrupt sent");
  }
  

  //Serial.println("Temp: ");
  //Serial.println(temp);
  //Serial.println(" C | Raw: ");
  //Serial.println(raw, 3);
  //Serial.println(" m | Filtered: ");
  //Serial.println(filtered, 3);
  //Serial.println(" m");

  delay(100);

}
