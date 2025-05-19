int i;//для использования в циклах.
int t = 10000;//константа ожидания.
bool mood; //наличие движения.
int ozone = 0;//условное количество озона.
void setup() {
  // put your setup code here, to run once:
pinMode(12, INPUT);
pinMode(5, OUTPUT);
}
void SensorCheck(){
  if(digitalRead (12) == 1){
    mood = true;
    }
    else if(digitalRead (12) == 0){
      mood = false;
      }
  
  
  }

void DispenseGas(){
  
  digitalWrite(5, HIGH);
  for (i = 0; i<=t;i++){
    delay(1);
    ozone++;
    SensorCheck();
    }
  digitalWrite(5, LOW);
  for (i = 0; i<=10800000;i++){
    delay(1);
    SensorCheck();
    }
   ozone = 0;
  }
void loop() {
  SensorCheck();
  // put your main code here, to run repeatedly:
if(mood == true){
    SensorCheck();
    }else if(mood == false){
      if (ozone == 0){
     for (i = 0;i>=5000;i++){
      delay(1);
      SensorCheck();
      }
          DispenseGas();
          
      }else if (ozone > 0){
        for (i = 0;i>=ozone*1080;i++){
          delay(1);
          SensorCheck();
          }
          ozone = 0;
        }
    }
}
