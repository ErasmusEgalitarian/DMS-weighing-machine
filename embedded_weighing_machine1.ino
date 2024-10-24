int Rled= 12;
int Yled=11;
int Gled=10;
int contador = 0;
int botao = 7;
int estadobotao = 0;
int estadosensorP=0;
int sensorP=A5;
int flag=0;
int buzzer=9;


void setup () {
  	Serial.begin(9600);
	pinMode (Rled, OUTPUT );
    pinMode (Yled, OUTPUT );
    pinMode (Gled, OUTPUT );
    pinMode(botao, INPUT_PULLUP);
    digitalWrite(Rled,LOW);
    digitalWrite(Yled,LOW);
    digitalWrite(Gled,LOW);
    
}

void loop () {
  noTone(buzzer);
  estadosensorP=analogRead(sensorP);
  Serial.println(estadosensorP);
  delay(10);
  
if (estadosensorP<780){        
  //acende led verde
    digitalWrite(Gled,HIGH);
    digitalWrite(Yled,LOW);
    digitalWrite(Rled,LOW);     
}                           
  else if (estadosensorP<833 && estadosensorP>780){
    //acende led laranja  
    digitalWrite(Gled,LOW);
    digitalWrite(Yled,HIGH);
    digitalWrite(Rled,LOW);     
    }
  else {
    //acende led vermelho e buzzer
    digitalWrite(Gled,LOW);
    digitalWrite(Yled,LOW);
    digitalWrite(Rled,HIGH);
    tone(buzzer, 440, 200);
    delay (100);
}                              
}

/*
void loop () {
  estadobotao=digitalRead(botao);

if (estadobotao==LOW && flag==1){ 
               
      flag=0;                  
      digitalWrite(Rled,HIGH);     
}                           
    else if (estadobotao==LOW && flag==0){                        
      flag=1;                 
      digitalWrite(Rled,LOW);     
    }
    delay(200);  
}                              
*/