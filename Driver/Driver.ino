//引脚部分
uint8_t din = 14;//DA
uint8_t clk = 2;//CK
uint8_t cs = 15;//CS
uint8_t Reset = 13;//RS
uint8_t en = 12;//EN

//VFD写入
void write_6302(unsigned char w_data){
  unsigned char i;
  for (i = 0; i < 8; i++)
  {
    digitalWrite(clk, LOW);
    if ( (w_data & 0x01) == 0x01)
    {
      digitalWrite(din, HIGH);
    }
    else
    {
      digitalWrite(din, LOW);
    }
    w_data >>= 1;
    digitalWrite(clk, HIGH);
  }
}

//VFD显示
void S1201_show(void)
{
  digitalWrite(cs, LOW);//开始传输
  write_6302(0xe8);//地址寄存器起始位置
  digitalWrite(cs, HIGH); //停止传输
}

//初始化VFD
void VFD_init()
{
  //设置有多少个数字字符
  digitalWrite(cs, LOW);
  write_6302(0xe0);
  delayMicroseconds(5);
  write_6302(0x07);//8个
  digitalWrite(cs, HIGH);
  delayMicroseconds(5);

  //设置亮度
  digitalWrite(cs, LOW);
  write_6302(0xe4);
  delayMicroseconds(5);
  write_6302(0xff);//0~255（16进制：0x00~0xff） 
  digitalWrite(cs, HIGH);
  delayMicroseconds(5);
}

/*（不推荐使用）
 (x,chr)
 x:显示该字符的位置0-7
 chr:该字符在datasheet里的位置?
 */
void S1201_WriteOneChar(unsigned char x, unsigned char chr)
{
  digitalWrite(cs, LOW);  //开始传输
  write_6302(0x20 + x); //地址寄存器起始位置
  write_6302(chr + 0x30);
  digitalWrite(cs, HIGH); //停止传输
  S1201_show();
}

/*
 改进版
 (x,chr)
 x:显示该字符的位置0-7
 chr:该字符的ASCII
 */
void S1201_WriteOneChar1(unsigned char x, unsigned char chr)
{
  digitalWrite(cs, LOW);  //开始传输
  write_6302(0x20 + x); //地址寄存器起始位置
  write_6302(chr);
  digitalWrite(cs, HIGH); //停止传输
  S1201_show();
}

/*
 在指定位置打印字符串
 (x,str)
 x:显示该字符串的位置0-7
 str:要显示的字符串
 */
void S1201_WriteStr(int x, String str){
  for(int i = 0;i<str.length();i++){
    S1201_WriteOneChar1(i+x,str.charAt(i));   
  }
}

//VFD清屏
void S1201_clear(){
  for(int i = 0;i<8;i++){
    S1201_WriteOneChar1(i,16);
  }
}

void setup() {
  pinMode(en, OUTPUT);
  pinMode(clk, OUTPUT);
  pinMode(din, OUTPUT);
  pinMode(cs, OUTPUT);
  pinMode(Reset, OUTPUT);
  digitalWrite(Reset, LOW);
  digitalWrite(en, HIGH);//VFD使能开。（就是开关VFD屏，HIGH是亮，LOW是灭）
  delayMicroseconds(5);
  digitalWrite(Reset, HIGH);
  VFD_init();
}


void loop() {
  /*使用例*/
  S1201_WriteStr(0,"ABCDEFGH");//显示字符串
  
  delay(1000);
  
  for(int i = 0;i<8;i++){
    S1201_WriteOneChar1(i,i+'0');//显示单个字符
  }
  
  delay(1000);
  
  S1201_clear();//清屏
}
