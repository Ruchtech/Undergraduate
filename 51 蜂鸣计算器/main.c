#include "reg52.h"              // ������51��Ƭ�����Ӳ���Ĵ�������͹��ܺ�
// �궨��
typedef unsigned int u16;                  // 16 λ�޷�������
typedef unsigned char u8;                  // 8λ�޷�������,charռ 1 ���ֽ� 

// ���������ϵ��½������룬��Ԥ���庯������
void delay_10us(u16 ten_us);
void seg_display(unsigned char which_seg, Number, point); 
void keyWork();	             //�������ɨ���봦��
char pipline_get(int num);	//�����λ����,��ÿλ��ֵ����Seg_Buf[]





// �����ģ��
#define SMG_A_DP_PORT  P0 //8λ����I/O�˿ڱ�ʾ��Ƭ��P0.0 �� P0.7��8������	// ��ѡ-��ʾʲô
sbit LSA = P2^2;						                                    // λѡ-��ʾ�ĸ�
sbit LSB = P2^3;
sbit LSC = P2^4;
u8 seg_pipline[8] ={0,4,4,1,6,5,5,0};   // ѧ�Ŵ洢                                          
char seg_point[8] ={0,0,0,0,0,0,0,0};   // �����С����                                               
unsigned char gsmg_code[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x00, 0x40};              //0,1,2,3,4,5,6,7,8,9,Ϩ��,����
void delay_10us(u16 ten_us) {
    while (ten_us--);
}
//������� a, b, c, d, e, f, g �߶� LED �� һ��С���� dp ���
void seg_display(unsigned char which_seg, Number, point){
    switch (which_seg) {                            //λѡ����������
        case 0: LSC = 1; LSB = 1; LSA = 1; break;  //�����һλ�����
        case 1: LSC = 1; LSB = 1; LSA = 0; break;  //����ڶ�λ�����
        case 2: LSC = 1; LSB = 0; LSA = 1; break;  //�������λ�����
        case 3: LSC = 1; LSB = 0; LSA = 0; break;  //�������λ�����
        case 4: LSC = 0; LSB = 1; LSA = 1; break;  //�������λ�����
        case 5: LSC = 0; LSB = 1; LSA = 0; break;  //���������������
        case 6: LSC = 0; LSB = 0; LSA = 1; break;  //�����ڶ��������
        case 7: LSC = 0; LSB = 0; LSA = 0; break;  //������һ�������
    }
	if(point)									     //��ѡ
		P0 = gsmg_code[Number]|0x80;	// λ��-�����8λΪ1-����ĳλ�����С����
	else
		P0 = gsmg_code[Number];	
	delay_10us(50);					// ��ʱ��ȷ����ʾ�ȶ�
	P0=0x00;							// ��Ӱ��Ϩ��ǰλ�Ա�����Ӱ
}










// ������ģ��
// ��Դ������-�ⲿ�ź�������һ��Ƶ�ʷ����źź󣬷��������źŷ�����ӦƵ������
// Freq ԽС��count Խ�󣬷�����ʱԽ��Ƶ��Խ�ͣ�����ź�Ƶ��ԽС������Խ�ͳ�
sbit BEEP = P2^5; 
void Beep(unsigned int Freq)           //Ƶ������ʱ�ɷ���
{
		unsigned int i=200;              
		unsigned int count=0;
		count = 6666 / Freq;       // ��Ƶ��ת��Ϊ��ʱֵ��ÿ����ʱ��λ 10 ΢�룩
		while(i--)
		{
			BEEP = !BEEP;		   // ��ת BEEP ���ŵ�ƽ����������
			delay_10us(count);	   // ��ʱ�����Ʒ�������
	}
}






// �������ģ��
sbit P0_0 = P0^0;sbit P0_1 = P0^1;sbit P0_2 = P0^2;sbit P0_3 = P0^3;sbit P0_4 = P0^4;sbit P0_5 = P0^5;sbit P0_6 = P0^6;sbit P0_7 = P0^7;
sbit P1_0 = P1^0;sbit P1_1 = P1^1;sbit P1_2 = P1^2;sbit P1_3 = P1^3;sbit P1_4 = P1^4;sbit P1_5 = P1^5;sbit P1_6 = P1^6;sbit P1_7 = P1^7;
sbit P2_0 = P2^0;sbit P2_1 = P2^1;sbit P2_2 = P2^2;sbit P2_3 = P2^3;sbit P2_4 = P2^4;sbit P2_5 = P2^5;sbit P2_6 = P2^6;sbit P2_7 = P2^7;
sbit P3_0 = P3^0;sbit P3_1 = P3^1;sbit P3_2 = P3^2;sbit P3_3 = P3^3;sbit P3_4 = P3^4;sbit P3_5 = P3^5;sbit P3_6 = P3^6;sbit P3_7 = P3^7;
// P1_3��P1_2��P1_1��P1_0��Ϊ�ߵ�ƽ���ٽ�P1_3����Ϊ0�����μ��P1_7��P1_6��P1_5��P1_4���ֱ��ӦS1��S5��S9��S13��
// P1_3��P1_2��P1_1��P1_0��Ϊ�ߵ�ƽ���ٽ�P1_2����Ϊ0�����μ��P1_7��P1_6��P1_5��P1_4���ֱ��ӦS2��S6��S10��S14��
// P1_3��P1_2��P1_1��P1_0��Ϊ�ߵ�ƽ���ٽ�P1_1����Ϊ0�����μ��P1_7��P1_6��P1_5��P1_4���ֱ��ӦS3��S7��S11��S15;
// P1_3��P1_2��P1_1��P1_0��Ϊ�ߵ�ƽ���ٽ�P1_0����Ϊ0�����μ��P1_7��P1_6��P1_5��P1_4���ֱ��ӦS4��S8��S12��S16��
// P1_3��P1_2��P1_1��P1_0�������õ͵�λ����ߵ�λ������ɨ��ÿ�У�P1_7��P1_6��P1_5��P1_4���μ��ÿ��ɨ���Ƿ��е͵�λ��8�˿ڳɹ����16����		
// sbit P0_0 = P0^0; P0 �� 8051 ��Ƭ���� P0 �˿ڼĴ��������ʾһ���ֽڣ�8 λ����^ ��ʾλѰַ�����
void Delay(unsigned int xms)			// �Ǿ�ȷ�ġ����ʵ�ֵ�����ʽ��ʱ					   
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
unsigned char MatrixKey()				//������Ӧ
{
    unsigned char KeyNumber = 0; //�洢������ţ���ʼֵΪ 0    
    P1 = 0xFF;  P1_3 = 0;   //��ʼɨ���һ��
    if (P1_7 == 0)         //P1_7�͵�ƽ
    {
        Delay(20);           //�ȴ� 20 ���룬������е���㵯������һ����ͨ�ϱ仯			
        while (P1_7 == 0);   //�ȴ������ɿ�����ֹ�����´�ɨ��
          KeyNumber = 1;     //�������ּ� 1
    }
    if (P1_6 == 0){        //P1_6�͵�ƽ    
        Delay(20); while (P1_6 == 0) KeyNumber = 5;
    }
    if (P1_5 == 0){        //P1_5�͵�ƽ    
        Delay(20); while (P1_5 == 0) KeyNumber = 9; 
    }
    if (P1_4 == 0){        //P1_4�͵�ƽ    
        Delay(20); while (P1_4 == 0) KeyNumber = 13;
    }
    P1 = 0xFF;  P1_2 = 0;  //�� P1_2 ��Ϊ�͵�ƽ����ʼɨ��ڶ���
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
    return KeyNumber;     //���ؼ�⵽�İ������
}







// ����ģ��
#define LEDDZ_COL_PORT	P0	      //�����п��ƶ˿�
sbit SRCLK = P3^6;	              //��λ�Ĵ���ʱ������
sbit sRCLK = P3^5;		          //�洢�Ĵ���ʱ������
sbit SER   = P3^4; 		          //������������
u8 Dot_Buf[7][8] = {
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // ����ʾ	
{0x00, 0x18, 0x18, 0x7e, 0x7e, 0x18, 0x18, 0x00},   // +			
{0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00},   // -			
{0x00, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42, 0x00},   // x			
{0x00, 0x18, 0x18, 0x5a, 0x5a, 0x18, 0x18, 0x00},   // /			
{0x00, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x00},   // =			
{0x7e, 0xc3, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81}};  // C			
u8 Dot_Pos[8]= {0x7f, 0xbf, 0xdf, 0xef, 0xf7, 0xfb, 0xfd, 0xfe};  //��ˢ��
void hc595_write_data(u8 dat)							               
{
	u8 i=0;												
	for(i=0;i<8;i++){         //ѭ��8�ν�һ�ֽ�д��Ĵ���	
		SER=dat>>7;           //��λ������ȡ���λ��7
		dat<<=1;              //��λ������λ
		SRCLK=0;
		delay_10us(1);
		SRCLK=1;
		delay_10us(1);        //��λ�Ĵ���ʱ�����������ݴ���Ĵ���
	}
	sRCLK=1;                  //�洢�Ĵ���ʱ����������ʾ�Ĵ�������
	delay_10us(1);            //��ʱ��ȷ��ʱ���ź��ȶ�
	sRCLK=0;                  //���ô洢ʱ��Ϊ�͵�ƽ
}
void dotMatrix(char which){	
u8 i=0, k=0;
for(k=0;k<30;k++)		 // ���ˢ�µ�����ʾ����ǿ�Ӿ����ȼ���ʾЧ��
		 for(i=0;i<8;i++)
		 {	                                            
			LEDDZ_COL_PORT=Dot_Pos[i];             //��i�е͵�λ
			hc595_write_data(Dot_Buf[which][i]);   //��ʾwhich���ŵ�i������
			delay_10us(100);                       //��ʱ�ȴ���ʾ�ȶ�
			hc595_write_data(0x00);                //��Ӱ	
		 }	
}





// ��������ģ��
double number = 0, lastNum = 0;	          // ��ǰ���� ��һ����
char operator = 0;                          // �洢�������
unsigned char Key=0;						 // �洢��ֵ
bit startFlag = 0, signFlag = 0;	         // ��ʼ��־-���ű�־
void keyWork()
{
	char i = 0;								 
	if((Key == 16)&&(startFlag == 0))         // ����������������
	{
		for(i=0; i<8; i++){
			seg_pipline[i] = 10;              // ���������
			seg_point[i] = 0;                 // С��λ����
		}
		startFlag = 1;                        // ��־����������������
	}	
	if(startFlag)
		switch(Key){
			case 1:                           // ��λ��ʾ�������洢-����-�ͳ� 
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
					if(!signFlag){ //����������룬����ǰһ����������ʼ�²�����
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
							case '/':		//��/��С��λ
								number = lastNum / number * 1.0; // ˫������
								pipline_get(number*10); // С��λת��������ʾ
								// ����λΪ0��ʹ����ʾ�ĳ���
								if((int)number%10 == 0)	seg_pipline[6] = 0;		
								seg_point[6] = 1;			//����1λС��
							break;
						}
						lastNum = number; // ������߲�����
						number = 0;		 // ������						
					}
					else {
						number = lastNum;	// ���ֲ���						                 
						pipline_get(number);
					}Beep(1100);
			break;							
			case 16:
				for(i=0; i<8; i++){         // ����
					seg_pipline[i]=10; seg_point[i]=0; number=0; signFlag=0;
				}Beep(1200);
			break;
		}
}
char pipline_get(int num)           // ʹnumת���ұ�����,ÿ�δ����λ
{
	char i = 0, j = 0;
	if(num >= 0){
		seg_pipline[7] = (int)num%10;       // �������λ  
		for(i = 6; i>=0; i--){
			num /= 10;                      // ȥ�����λ
			if(num == 0){
				for(j=i; j>=0; j--) seg_pipline[j] = 10;	//����Ϩ��
				return(i);
			}
			seg_pipline[i] = (int)num % 10; // �������λ	
		}
	}
	else{
		i = pipline_get(number*(-1));			//�������� 
		seg_pipline[i] = 11;					//�����λΪ����
	}
}












// ������ģ��
void main()
{
	char i=0, which=0;
	//dotMatrix(0);							//
	while(1)                                //��ѯͨ��
	{							
		Key = MatrixKey();					//�������ģ��-��Ӧ����	
		keyWork();							//��������ģ��-��������
		for(i = 0; i<8; i++)					//�����ģ��-�������ʾ
			seg_display(i, seg_pipline[i],seg_point[i]);
		if(Key>=11 && Key<=16)				//����ģ��-���������ʾ
		{
			which = Key - 10;
			dotMatrix(which);                  
		}
	}
}	