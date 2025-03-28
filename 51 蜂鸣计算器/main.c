#include "reg52.h"              // 引入与51单片机相关硬件寄存器定义和功能宏
// 宏定义
typedef unsigned int u16;                  // 16 位无符号整数
typedef unsigned char u8;                  // 8位无符号整数,char占 1 个字节 

// 编译器从上到下解析代码，固预定义函数声明
void delay_10us(u16 ten_us);
void seg_display(unsigned char which_seg, Number, point); 
void keyWork();	             //矩阵键盘扫描与处理
char pipline_get(int num);	//数码管位处理,将每位数值存入Seg_Buf[]





// 数码管模块
#define SMG_A_DP_PORT  P0 //8位并行I/O端口表示单片机P0.0 至 P0.7共8个引脚	// 段选-显示什么
sbit LSA = P2^2;						                                    // 位选-显示哪个
sbit LSB = P2^3;
sbit LSC = P2^4;
u8 seg_pipline[8] ={0,4,4,1,6,5,5,0};   // 学号存储                                          
char seg_point[8] ={0,0,0,0,0,0,0,0};   // 数码管小数点                                               
unsigned char gsmg_code[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x00, 0x40};              //0,1,2,3,4,5,6,7,8,9,熄灭,负号
void delay_10us(u16 ten_us) {
    while (ten_us--);
}
//数码管由 a, b, c, d, e, f, g 七段 LED 和 一个小数点 dp 组成
void seg_display(unsigned char which_seg, Number, point){
    switch (which_seg) {                            //位选，从左置右
        case 0: LSC = 1; LSB = 1; LSA = 1; break;  //最左第一位数码管
        case 1: LSC = 1; LSB = 1; LSA = 0; break;  //最左第二位数码管
        case 2: LSC = 1; LSB = 0; LSA = 1; break;  //最左第三位数码管
        case 3: LSC = 1; LSB = 0; LSA = 0; break;  //最左第四位数码管
        case 4: LSC = 0; LSB = 1; LSA = 1; break;  //最左第五位数码管
        case 5: LSC = 0; LSB = 1; LSA = 0; break;  //倒数第三个数码管
        case 6: LSC = 0; LSB = 0; LSA = 1; break;  //倒数第二个数码管
        case 7: LSC = 0; LSB = 0; LSA = 0; break;  //倒数第一个数码管
    }
	if(point)									     //段选
		P0 = gsmg_code[Number]|0x80;	// 位或-段码第8位为1-点亮某位数码管小数点
	else
		P0 = gsmg_code[Number];	
	delay_10us(50);					// 延时，确保显示稳定
	P0=0x00;							// 消影，熄灭当前位以避免重影
}










// 蜂鸣器模块
// 无源蜂鸣器-外部信号驱动，一定频率方波信号后，蜂鸣器随信号发出对应频率声音
// Freq 越小，count 越大，方波延时越长频率越低，输出信号频率越小，声音越低沉
sbit BEEP = P2^5; 
void Beep(unsigned int Freq)           //频率与延时成反比
{
		unsigned int i=200;              
		unsigned int count=0;
		count = 6666 / Freq;       // 将频率转换为延时值（每个延时单位 10 微秒）
		while(i--)
		{
			BEEP = !BEEP;		   // 翻转 BEEP 引脚电平，产生方波
			delay_10us(count);	   // 延时，控制方波周期
	}
}






// 矩阵键盘模块
sbit P0_0 = P0^0;sbit P0_1 = P0^1;sbit P0_2 = P0^2;sbit P0_3 = P0^3;sbit P0_4 = P0^4;sbit P0_5 = P0^5;sbit P0_6 = P0^6;sbit P0_7 = P0^7;
sbit P1_0 = P1^0;sbit P1_1 = P1^1;sbit P1_2 = P1^2;sbit P1_3 = P1^3;sbit P1_4 = P1^4;sbit P1_5 = P1^5;sbit P1_6 = P1^6;sbit P1_7 = P1^7;
sbit P2_0 = P2^0;sbit P2_1 = P2^1;sbit P2_2 = P2^2;sbit P2_3 = P2^3;sbit P2_4 = P2^4;sbit P2_5 = P2^5;sbit P2_6 = P2^6;sbit P2_7 = P2^7;
sbit P3_0 = P3^0;sbit P3_1 = P3^1;sbit P3_2 = P3^2;sbit P3_3 = P3^3;sbit P3_4 = P3^4;sbit P3_5 = P3^5;sbit P3_6 = P3^6;sbit P3_7 = P3^7;
// P1_3、P1_2、P1_1、P1_0均为高电平，再将P1_3设置为0，依次检查P1_7、P1_6、P1_5、P1_4，分别对应S1、S5、S9、S13；
// P1_3、P1_2、P1_1、P1_0均为高电平，再将P1_2设置为0，依次检查P1_7、P1_6、P1_5、P1_4，分别对应S2、S6、S10、S14；
// P1_3、P1_2、P1_1、P1_0均为高电平，再将P1_1设置为0，依次检查P1_7、P1_6、P1_5、P1_4，分别对应S3、S7、S11、S15;
// P1_3、P1_2、P1_1、P1_0均为高电平，再将P1_0设置为0，依次检查P1_7、P1_6、P1_5、P1_4，分别对应S4、S8、S12、S16。
// P1_3、P1_2、P1_1、P1_0依次设置低电位其余高单位以依次扫描每列，P1_7、P1_6、P1_5、P1_4依次检查每行扫描是否有低电位，8端口成功检查16个按		
// sbit P0_0 = P0^0; P0 是 8051 单片机的 P0 端口寄存器，表键示一个字节（8 位），^ 表示位寻址运算符
void Delay(unsigned int xms)			// 非精确的、软件实现的阻塞式延时					   
{							   
	unsigned char i, j;
	while(xms--)
	{
		i = 2;
		j = 239;
		do
		{
			while (--j);
		} while (--i);
	}
}
unsigned char MatrixKey()				//按键对应
{
    unsigned char KeyNumber = 0; //存储按键编号，初始值为 0    
    P1 = 0xFF;  P1_3 = 0;   //开始扫描第一列
    if (P1_7 == 0)         //P1_7低电平
    {
        Delay(20);           //等待 20 毫秒，消除机械触点弹性作用一连串通断变化			
        while (P1_7 == 0);   //等待按键松开，防止干扰下次扫描
          KeyNumber = 1;     //按下数字键 1
    }
    if (P1_6 == 0){        //P1_6低电平    
        Delay(20); while (P1_6 == 0) KeyNumber = 5;
    }
    if (P1_5 == 0){        //P1_5低电平    
        Delay(20); while (P1_5 == 0) KeyNumber = 9; 
    }
    if (P1_4 == 0){        //P1_4低电平    
        Delay(20); while (P1_4 == 0) KeyNumber = 13;
    }
    P1 = 0xFF;  P1_2 = 0;  //将 P1_2 设为低电平，开始扫描第二列
    if (P1_7 == 0){          
        Delay(20); while (P1_7 == 0); KeyNumber = 2; 
    }
    if (P1_6 == 0){
        Delay(20); while (P1_6 == 0); KeyNumber = 6; 
    }
    if (P1_5 == 0){    
        Delay(20); while (P1_5 == 0); KeyNumber = 10; 
    }
    if (P1_4 == 0){
        Delay(20); while (P1_4 == 0); KeyNumber = 14; 
    }
    P1 = 0xFF; P1_1 = 0; 
    if (P1_7 == 0){
        Delay(20); while (P1_7 == 0); KeyNumber = 3; 
    }
    if (P1_6 == 0){
        Delay(20); while (P1_6 == 0); KeyNumber = 7; 
    }
    if (P1_5 == 0) 
    {
        Delay(20); while (P1_5 == 0); KeyNumber = 11; 
    }
    if (P1_4 == 0){
        Delay(20); while (P1_4 == 0); KeyNumber = 15; 
    }
    P1 = 0xFF;  P1_0 = 0; 
    if (P1_7 == 0){
        Delay(20); while (P1_7 == 0); KeyNumber = 4; 
    }
    if (P1_6 == 0){
        Delay(20); while (P1_6 == 0); KeyNumber = 8; 
    }
    if (P1_5 == 0){
        Delay(20); while (P1_5 == 0); KeyNumber = 12; 
    }
    if (P1_4 == 0){
        Delay(20); while (P1_4 == 0); KeyNumber = 16; 
    }
    return KeyNumber;     //返回检测到的按键编号
}







// 点阵模块
#define LEDDZ_COL_PORT	P0	      //点阵列控制端口
sbit SRCLK = P3^6;	              //移位寄存器时钟输入
sbit sRCLK = P3^5;		          //存储寄存器时钟输入
sbit SER   = P3^4; 		          //串行数据输入
u8 Dot_Buf[7][8] = {
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // 不显示	
{0x00, 0x18, 0x18, 0x7e, 0x7e, 0x18, 0x18, 0x00},   // +			
{0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00},   // -			
{0x00, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42, 0x00},   // x			
{0x00, 0x18, 0x18, 0x5a, 0x5a, 0x18, 0x18, 0x00},   // /			
{0x00, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x00},   // =			
{0x7e, 0xc3, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81}};  // C			
u8 Dot_Pos[8]= {0x7f, 0xbf, 0xdf, 0xef, 0xf7, 0xfb, 0xfd, 0xfe};  //列刷新
void hc595_write_data(u8 dat)							               
{
	u8 i=0;												
	for(i=0;i<8;i++){         //循环8次将一字节写入寄存器	
		SER=dat>>7;           //移位操作提取最高位即7
		dat<<=1;              //低位移至高位
		SRCLK=0;
		delay_10us(1);
		SRCLK=1;
		delay_10us(1);        //移位寄存器时钟上升沿数据存入寄存器
	}
	sRCLK=1;                  //存储寄存器时钟上升沿显示寄存器数据
	delay_10us(1);            //延时，确保时钟信号稳定
	sRCLK=0;                  //设置存储时钟为低电平
}
void dotMatrix(char which){	
u8 i=0, k=0;
for(k=0;k<30;k++)		 // 多次刷新点阵显示，增强视觉亮度及显示效果
		 for(i=0;i<8;i++)
		 {	                                            
			LEDDZ_COL_PORT=Dot_Pos[i];             //第i列低电位
			hc595_write_data(Dot_Buf[which][i]);   //显示which符号第i列数据
			delay_10us(100);                       //延时等待显示稳定
			hc595_write_data(0x00);                //消影	
		 }	
}





// 四则运算模块
double number = 0, lastNum = 0;	          // 当前数字 上一数字
char operator = 0;                          // 存储运算符号
unsigned char Key=0;						 // 存储键值
bit startFlag = 0, signFlag = 0;	         // 开始标志-符号标志
void keyWork()
{
	char i = 0;								 
	if((Key == 16)&&(startFlag == 0))         // 蜂鸣器计算器开启
	{
		for(i=0; i<8; i++){
			seg_pipline[i] = 10;              // 数码管清零
			seg_point[i] = 0;                 // 小数位清零
		}
		startFlag = 1;                        // 标志蜂鸣器计算器开启
	}	
	if(startFlag)
		switch(Key){
			case 1:                           // 移位显示操作数存储-处理-低沉 
					number = number * 10 + 1; pipline_get(number); Beep(250);
			break;				
			case 2:
					number = number * 10 + 2; pipline_get(number); Beep(300);
			break;				
			case 3:
					number = number * 10 + 3; pipline_get(number); Beep(330);
			break;				
			case 4:
					number = number * 10 + 4; pipline_get(number); Beep(360);
			break;
			case 5:
					number = number * 10 + 5; pipline_get(number); Beep(390);
			break;				
			case 6:
					number = number * 10 + 6; pipline_get(number); Beep(420);
			break;				
			case 7:
					number = number * 10 + 7; pipline_get(number); Beep(450);
			break;				
			case 8:
					number = number * 10 + 8; pipline_get(number); Beep(520);
			break;				
			case 9:
					number = number * 10 + 9; pipline_get(number); Beep(555);
			break;				
			case 10:
					number = number * 10 + 0; pipline_get(number); Beep(666);
			break;				
			case 11:
					if(!signFlag){ //运算符号输入，保存前一操作数，开始新操作数
						lastNum = number; number = 0; signFlag = 1;
					} operator = '+'; Beep(700);
			break;				
			case 12:
					if(!signFlag){
						lastNum = number; number = 0; signFlag = 1;
					} operator = '-'; Beep(780);
			break;				
			case 13:
					if(!signFlag){
						lastNum = number; number = 0; signFlag = 1;
					}
					operator = '*'; Beep(880);
			break;
			case 14:
					if(!signFlag){
						lastNum = number; number = 0; signFlag = 1;
					}
					operator = '/'; Beep(1000);
			break;				
			case 15:
					if(signFlag){     
						signFlag = 0;
						switch(operator){
							case '+':
								number = lastNum + number; pipline_get(number);
							break;
							case '-':
								number = lastNum - number; pipline_get(number);
							break;
							case '*':
								number = lastNum * number; pipline_get(number);
							break;
							case '/':		//仅/用小数位
								number = lastNum / number * 1.0; // 双精度型
								pipline_get(number*10); // 小数位转整数以显示
								// 整数位为0，使其显示的出来
								if((int)number%10 == 0)	seg_pipline[6] = 0;		
								seg_point[6] = 1;			//保留1位小数
							break;
						}
						lastNum = number; // 保存左边操作数
						number = 0;		 // 保存结果						
					}
					else {
						number = lastNum;	// 保持不变						                 
						pipline_get(number);
					}Beep(1100);
			break;							
			case 16:
				for(i=0; i<8; i++){         // 清零
					seg_pipline[i]=10; seg_point[i]=0; number=0; signFlag=0;
				}Beep(1200);
			break;
		}
}
char pipline_get(int num)           // 使num转从右边置左,每次存最低位
{
	char i = 0, j = 0;
	if(num >= 0){
		seg_pipline[7] = (int)num%10;       // 整数最低位  
		for(i = 6; i>=0; i--){
			num /= 10;                      // 去除最低位
			if(num == 0){
				for(j=i; j>=0; j--) seg_pipline[j] = 10;	//其余熄灭
				return(i);
			}
			seg_pipline[i] = (int)num % 10; // 整数最低位	
		}
	}
	else{
		i = pipline_get(number*(-1));			//负数处理 
		seg_pipline[i] = 11;					//最左边位为负号
	}
}












// 主函数模块
void main()
{
	char i=0, which=0;
	//dotMatrix(0);							//
	while(1)                                //轮询通信
	{							
		Key = MatrixKey();					//矩阵键盘模块-响应键入	
		keyWork();							//四则运算模块-四则运算
		for(i = 0; i<8; i++)					//数码管模块-数码管显示
			seg_display(i, seg_pipline[i],seg_point[i]);
		if(Key>=11 && Key<=16)				//点阵模块-运算符号显示
		{
			which = Key - 10;
			dotMatrix(which);                  
		}
	}
}	