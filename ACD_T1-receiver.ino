#include <RCSwitch.h>

#define led_up    8  //выход 8 на пускатель движения вверх
#define led_down  9
#define led_left  10
#define led_right 11
#define led_ready 12 //выход включения подсветки готовности к работе

#define iRECbit1 16  //биты включения этой кран-балки
#define iRECbit2 32

//#define REC1       1
//#define REC2       2
//#define REC_BOTH   15 //достаточно проверить нужные биты

#define CMD_UP_bit 1 //0001
#define CMD_DOWN_bit 2  //0010
#define CMD_LEFT_bit 4  //0100
#define CMD_RIGHT_bit 8   //1000
//#define CMD_READY 5  //Команда зажечь лампу - сообщить о готовности к работе

const long alarm = 100;
int guard = 0;

long cycles = 0;


RCSwitch myR = RCSwitch();

void setup() {
  // put your setup code here, to run once:
  pinMode(led_up , OUTPUT); //Настройка пинов на выход
  pinMode(led_down , OUTPUT);
  pinMode(led_left , OUTPUT);
  pinMode(led_right , OUTPUT);
  pinMode(led_ready , OUTPUT);
  //pinMode(13,OUTPUT);

  myR.enableReceive(0);       //включаем приёмник на 0 входе RX0
  Serial.begin(9600);
}

void loop() {
    
  if( myR.available() ){
     int v = myR.getReceivedValue();
     
     /*//digitalWrite(13, HIGH);   // зажигаем светодиод
     //delay(1000);              // ждем секунду
     //digitalWrite(13, LOW);    // выключаем светодиод
     digitalWrite (led_ready, HIGH);
     delay (1000);
     digitalWrite (led_ready, LOW);
     */
     Serial.println ("\n" + String(v,BIN));

     if (bool(v&iRECbit1) & bool(v&iRECbit2)) {/// 
       
       /*for (int i = 0; i < 5; i++)
       {
       digitalWrite (led_ready, HIGH); //первым делом включаем подсветку
       delay (100);
       digitalWrite (led_ready, LOW);
       delay (100);  
       }*/
       
       digitalWrite (led_ready, HIGH);
       guard = 0;

       if (bool(v&CMD_UP_bit) ) {
          digitalWrite( led_up, HIGH );
          digitalWrite( led_down, LOW );
          guard = 0;
       }
       else {
         if (bool(v&CMD_DOWN_bit)) {
           digitalWrite( led_up, LOW );
           digitalWrite( led_down, HIGH );
           guard = 0;
         } 
       }

       if (bool(v&CMD_LEFT_bit)) {
          digitalWrite( led_left, HIGH );
          digitalWrite( led_right, LOW );
          guard = 0;
       }
       else {
         if (bool(v&CMD_RIGHT_bit)) {
          digitalWrite( led_left, LOW );
          digitalWrite( led_right, HIGH );
          guard = 0;
         }
       }
       
          guard++;
      } else {
          digitalWrite( led_up, LOW );
          digitalWrite( led_down, LOW );
          digitalWrite( led_left, LOW );
          digitalWrite( led_right, LOW );
          digitalWrite( led_ready, LOW );

      }

      
      myR.resetAvailable();
  } else {
          
      guard++;
      Serial.print (guard);
      if (guard>alarm) {
      digitalWrite( led_up, LOW );
      digitalWrite( led_down, LOW );
      digitalWrite( led_left, LOW );
      digitalWrite( led_right, LOW );
      digitalWrite( led_ready, LOW );
      Serial.println ("\n" + String(guard) + " cycle " + String(cycles));
      cycles++;
      //delay (1000);
      /*for (int i = 0; i < 3; i++)
      {
      digitalWrite(13, HIGH);   // зажигаем светодиод
      delay(100);              // ждем секунду
      digitalWrite(13, LOW);    // выключаем светодиод
      delay(100);   
      }

      delay (1000);           // ждем секунду*/
      guard = 0;
      }
    }      
  
}
