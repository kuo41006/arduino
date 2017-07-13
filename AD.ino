volatile int ggyy = 1;  // 使用這當 Flag 給  ISR 使用 !
int ledPin =13;
const int myTOP = 15624;  // 大約 0.5 秒 when Prescaler == 1024
const int AD1 = A0; 
const int AD2 = A2; 
const int AD3 = A4; 
const int AD4 = A6; 
int AD1_Value = 0; 
int AD2_Value = 0; 
int AD3_Value = 0; 
int AD4_Value = 0; 
byte adc1_h , adc1_l ,adc2_h , adc2_l ,adc3_h , adc3_l ,adc4_h , adc4_l  ; 
byte adc_start = 0x03;
byte adc_stop1 = 0x02;
byte adc_stop2 = 0x01;
//資料格式  03 xx xx xx xx xx xx xx 02 01
int count = 0; 

int adspeed = 0;

ISR(TIMER1_COMPA_vect)
{
  
  Serial1.println("");
  Serial1.println("");
  Serial1.println("");
  Serial1.print("ad_speed : ");
  Serial1.println(adspeed);
  Serial1.println("");
  Serial1.println("");
  Serial1.println("");
  
  adspeed = 0 ;
  digitalWrite(ledPin, ggyy);  // ggyy 是 0 或 1
  ggyy = 1 - ggyy; //  給下次進入  ISR 用
  delay(1000);
}



void setup() {
  // put your setup code here, to run once:
  Serial1.begin(115200);
  Serial.begin(9600);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // turn Off the LED
  cli();  // 禁止中斷
  TCCR1A = 0;
  TCCR1B = 0; 
  TCCR1B |= (1<<WGM12);  // CTC mode; Clear Timer on Compare
  TCCR1B |= (1<<CS10) | (1<<CS12);  // Prescaler == 1024
  OCR1A = myTOP;  // TOP count for CTC, 與 prescaler 有關
  TCNT1=0;  // counter 歸零 
  TIMSK1 |= (1 << OCIE1A);  // enable CTC for TIMER1_COMPA_vect
  sei();  // 允許中斷
}

void loop() {
  
  
  AD1_Value = analogRead(AD1);
  AD2_Value = analogRead(AD2);
  AD3_Value = analogRead(AD3);
  AD4_Value = analogRead(AD4);
  
  
  adc1_h =  (AD1_Value & 0x300)>>8 ;
  adc1_l =  AD1_Value & 0xFF ; 

  adc2_h =  (AD2_Value & 0x300)>>8 ;
  adc2_l =  AD2_Value & 0xFF ; 

  adc3_h =  (AD3_Value & 0x300)>>8 ;
  adc3_l =  AD3_Value & 0xFF ; 

  adc4_h =  (AD4_Value & 0x300)>>8 ;
  adc4_l =  AD4_Value & 0xFF ; 



  Serial1.write(adc_start);
  Serial1.write(adc1_h);
  Serial1.write(adc1_l);

  Serial1.write(adc2_h);
  Serial1.write(adc2_l);

  Serial1.write(adc3_h);
  Serial1.write(adc3_l);

  Serial1.write(adc4_h);
  Serial1.write(adc4_l);
  Serial1.write(adc_stop1);
  Serial1.write(adc_stop2);

  
 /* 
  Serial1.println(AD2_Value,HEX);

  Serial1.println(AD3_Value,HEX);

  Serial1.println(AD4_Value,HEX);
*/
  delay(100);
  adspeed++;
  
}
