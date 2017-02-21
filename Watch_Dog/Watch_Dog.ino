//buzzer pin to sound alarm
int buzzerPin = 2; 

//ultrasonic sensor pins
int echoPin = 11;    
int triggerPin = 12;   

//pushbutton pins for password
int button1 = 9;
int button2 = 8;
int button3 = 7;

//very short beep
void beep(){
  digitalWrite(buzzerPin, HIGH);
  delay(20);
  digitalWrite(buzzerPin, LOW);
}

void soundAlarm(){
  digitalWrite(buzzerPin, HIGH);
  delay(2000);
  digitalWrite(buzzerPin, LOW);
}

//find distance from ultrasonic sensor based on HC-SR04 Specs
int findDistance(){
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  int inches = duration/74/2;
  return inches;
}
  
void setup() {
  pinMode (buzzerPin, OUTPUT);
  pinMode (triggerPin, OUTPUT);
  pinMode (echoPin, INPUT);
  pinMode (button1, INPUT_PULLUP);
  pinMode (button2, INPUT_PULLUP);
  pinMode (button3, INPUT_PULLUP);
  
  Serial.begin(9600);
}

void loop() {
  //beep to notify me that the distance is going to be set
  //Then wait 6 seconds for me to leave the room
  beep();
  delay(6000);
  beep();
  
  //find initial distance and set a upper and lower distance limit
  //within which the alarm will not ring
  //to accomodate for error in sensor measurement
  int initialDistance = findDistance(); 
  Serial.print("Initial distance = ");
  Serial.println(initialDistance);
  int lowerLimit = initialDistance-5;
  int upperLimit = initialDistance+5;  
  
  //cheaks if correct password sequence is entered
  int passwordStatus = 0;
  while(!passwordStatus){
    delay(500);
    int distance = findDistance();
    Serial.println(distance);
    if (distance>upperLimit || distance<lowerLimit){
      //check again in 2 seconds to account for error
      delay(2000);
      int distance = findDistance();
      if (distance>upperLimit || distance<lowerLimit){
        soundAlarm();
        Serial.println("Alarm triggered!");
        delay(5000);
        if (digitalRead(button3) == LOW){
          Serial.println("First key entered");
          beep();
          delay(2000);
          
          if (digitalRead(button2) == LOW){
            Serial.println("Second key entered");
            beep();
            delay(500);
            
            if (digitalRead(button2) == LOW){
              Serial.println("Third key entered");
              beep();
              delay(500); 
              
              if (digitalRead(button2) == LOW){
                Serial.println("Final key entered");
                beep();
                digitalWrite(12,LOW);
                digitalWrite(13,HIGH);
                passwordStatus = 1;
                delay(5000);
                
              }else soundAlarm();
            }else soundAlarm();
          }else soundAlarm();
        }else soundAlarm();
      } 
    }  
  }
  
  //to pause the alarm and ultrasonic sensor while I am in the room
  //if the correct password is entered
  if (digitalRead(button1) == LOW){
    beep();
    delay(2000);
    //wait for input to restart operation
    while(digitalRead(button1) == HIGH){}
    beep();
    delay(7000);
  }
}
