int Rled= 12;
int Yled=11;
int Gled=10;
int estadosensorP=0;
int sensorP=A5;
int flag=0;
int buzzer=9;

void setup () {
  	Serial.begin(9600);
	pinMode (Rled, OUTPUT );
    pinMode (Yled, OUTPUT );
    pinMode (Gled, OUTPUT );
    digitalWrite(Rled,LOW);
    digitalWrite(Yled,LOW);
    digitalWrite(Gled,LOW);
}

void loop () {
  noTone(buzzer);
  sensorstate=analogRead(sensorP);
  Serial.println(sensorstate);
  delay(10);
  
  if (sensorstate<780){        
    //pop up green led
      digitalWrite(Gled,HIGH);
      digitalWrite(Yled,LOW);
      digitalWrite(Rled,LOW);     
  }                           
    else if (sensorstate<833 && sensorstate>780){
      //pop up orange led
      digitalWrite(Gled,LOW);
      digitalWrite(Yled,HIGH);
      digitalWrite(Rled,LOW);     
      }
    else {
      //pop up red led and buzzer
      digitalWrite(Gled,LOW);
      digitalWrite(Yled,LOW);
      digitalWrite(Rled,HIGH);
      tone(buzzer, 100, 50);
      delay (100);
  }
}