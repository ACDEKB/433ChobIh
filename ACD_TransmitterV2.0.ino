#include <RCSwitch.h>

#define btn_up    8  //Кнопка вверх на восьмом выводе
#define btn_down  9     
#define btn_left  10
#define btn_right 11
#define btn_mode  12
#define led_LD    6 //Левый светодиод
#define led_RD    7 //Правый светодиод ?которая балка?

#define Transmitter 4 //Передатчик сидит на 4 выводе (нога 7 от TX|RX)

#define REC1     3     //    0011
#define REC2     12    //Работает балка по ?правому? светодиоду  1100
#define REC_BOTH 15   //Работают сразу обе балки    1111

#define CMD_UP     1  //Команда вверх 0001
#define CMD_DOWN   2  //   0010
#define CMD_LEFT   4  //   0100
#define CMD_RIGHT  8  //   1000
//#define CMD_READY  5  //Команда зажечь лампу - сообщить о готовности к работе

#define bytes 8

int rec_mode = 3;
int rec_code = 15;
int commandos = 0;

int val_btn_up = 0;
int val_btn_down = 0;
int val_btn_left = 0;
int val_btn_right = 0;
int val_btn_mode = 0;
int old_val_btn_mode = 0;

RCSwitch myT = RCSwitch();

int codeToInt (int rec, int cmd){   //Из двух кодов Mode и Command
  
  return rec*16 + cmd;
  //return rec*4 + cmd - 1;
}

void setup() {
  Serial.begin (9600);
  // put your setup code here, to run once:
  pinMode(btn_up , INPUT);  //Устанавливаем нужные ноги на вход для кнопок
  pinMode(btn_down , INPUT);
  pinMode(btn_left , INPUT);
  pinMode(btn_right , INPUT);
  pinMode(btn_mode , INPUT);

  pinMode(led_LD , OUTPUT);   //Устанавливаем ныжные ноги на выход для светодиодов
  pinMode(led_RD , OUTPUT);
  
  myT.enableTransmit(Transmitter);
  //Serial.begin(9600);

  digitalWrite( led_LD, HIGH );
  digitalWrite( led_RD, HIGH );

  //myT.send( codeToInt( rec_mode, 0 ), bytes); //Первым делом зажигаем лампы на готовых кран-балках
}

void loop() {
  // put your main code here, to run repeatedly:
  //test diods
  /*
  digitalWrite( led_LD, HIGH );
          digitalWrite( led_RD, HIGH );
         
  delay(1000);
  
  digitalWrite( led_LD, LOW );
          digitalWrite( led_RD, LOW );
         
  delay(1000);
  */
  //myT.send( codeToInt( rec_mode, CMD_READY ), bytes); //Первым делом зажигаем лампы на готовых кран-балках
  
  val_btn_up = digitalRead(btn_up);;
  val_btn_down = digitalRead(btn_down);
  val_btn_left = digitalRead(btn_left);
  val_btn_right = digitalRead(btn_right);
  val_btn_mode = digitalRead(btn_mode);
 
  if (val_btn_mode == LOW) old_val_btn_mode = 0;

  if ((val_btn_mode == HIGH)&&(old_val_btn_mode == 0)) {
    rec_mode++;
    if (rec_mode > 3) rec_mode = 1;
    old_val_btn_mode = 1;
    if (rec_mode == 1) {
      digitalWrite(led_LD, HIGH);
      digitalWrite(led_RD, LOW );
      rec_code = REC1;
    } else if (rec_mode == 2) {
      digitalWrite(led_LD, LOW);
      digitalWrite(led_RD, HIGH);
      rec_code = REC2;
    } else if (rec_mode == 3) {
      digitalWrite( led_LD, HIGH );
      digitalWrite( led_RD, HIGH );
      rec_code = REC_BOTH;
    }
  } 


  if (val_btn_up != val_btn_down) { //Если обе кнопки нажаты или отпущены, то...
    if (val_btn_up == HIGH) {
      commandos |= CMD_UP;
      commandos &= ~CMD_DOWN;
    }
    else {
      commandos |= CMD_DOWN;
      commandos &= ~CMD_UP;
    }
  }
  else {                            //Обнуляем команды
    commandos &= ~CMD_DOWN;
    commandos &= ~CMD_UP;
    
  }
  
  if (val_btn_left != val_btn_right) { //Если обе кнопки нажаты или отпущены, то ....
    if (val_btn_left == HIGH ) {
      commandos |= CMD_LEFT;
      commandos &= ~CMD_RIGHT;
    }
    else{
      commandos |= CMD_RIGHT;
      commandos &= ~CMD_LEFT;
    }
  }
  else {                            //Обнуляем команды
    commandos &= ~CMD_LEFT;
    commandos &= ~CMD_RIGHT;
  
  }
  myT.send( codeToInt( rec_code, commandos ), bytes);
   Serial.println (String(rec_code,BIN) + "  " + String(commandos, BIN) + " / sec " + String (millis()/1000));
 }
