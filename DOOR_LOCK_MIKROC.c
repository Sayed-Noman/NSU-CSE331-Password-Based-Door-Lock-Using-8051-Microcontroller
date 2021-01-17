#include<reg51.h>
#include<string.h>

//Micro Define
#define lcdport P2

//Pin configuaration for lcd-screen
sbit rs=P3^4;
sbit rw=P3^5;
sbit en=P3^6;

//Pin configuaration for servo-motor
sbit m1=P3^0;
sbit m2=P3^1;

//Pin configuaration for Keypad: row
sbit r1=P1^0;
sbit r2=P1^1;
sbit r3=P1^2;
sbit r4=P1^3;

//Pin configuaration for Keypad: col
sbit c1=P1^4;
sbit c2=P1^5;
sbit c3=P1^6;

//Pin configuaration for Buzzer
sbit buzzer=P1^7;

//Globar Array Declaration for Storig Password
char uid[]="54321";
char id[5];

//Function declaration
void lcdint(); // Function to initialize Lcd-Screen
void lcddis(char *); //Function to Display a string
void delay(int); //Delay Function
void lcdcmd(char); // Function to command Lcd
void lcddata(char); // Function to display a single character in screen
char lcdkey(); // Lcd Keyboard function
char scan_key(); //Function to take single key input
void door_open(); //Function to open door
void door_close(); //Function to close Door
void sounder(); // Function to Buzz sounder

void main(){
	
	int n;
	char key;
	
	P2=0x00; // Using Port 2 as output port
	P1=0xff; // Using Port 1  as input port
	
	lcdint(); //Intitializing LCD-Screen
	lcddis("Door is Locked");
	//lcdcmd(0xc0);
	//lcddis("Enter Passcode:");
	delay(100);
	lcdcmd(0x01);  //Clearing Display Screen
	lcdcmd(0x02);	//4bit mode for LCd-Screen
	lcddis("Enter Password:");
	lcdcmd(0xc0);	// Cursor beggining of second line
	n=0;
	while(n<5){
		key=scan_key();
		id[n]=key;
		//lcddata(key);
		delay(100);
		n++;
	}
	lcdcmd(0x01); //ClearingLCD-Screen
	lcdcmd(0x02);	// 4bit mode for LCD-Screen
	
	if(strcmp(uid,id)==0){
		lcddis("Password Matched");
		delay(200);
		door_open(); //openining door
		delay(600);
		door_close(); //closing door
		lcdcmd(0x01); //Clearing LCD-Screen
		lcdcmd(0x02);	//4bit mode for LCD-Screen
	}
	else{
		lcddis("Wrong Password");
		sounder();	//Buzzing Sounde
		delay(200);
		lcdcmd(0x01); //Clearing LCD-Screen
		lcdcmd(0x02);	//4bit mode for LCD-Screen
		lcddis("Please Try Again");
		delay(200);
		lcdcmd(0x01); //Clearing LCD-Screen
		lcdcmd(0x02);	//4bit mode for LCD-Screen
	}
}

//Lcd-Screen Initialization Function
void lcdint(){
	lcdcmd(0x38);	//  lcd initialize matrix 8bit
	delay(2);
	lcdcmd(0x01);	// Clearing Lcd-Screen
	delay(2);
	lcdcmd(0x80);	// lcd screen row-0 col-0
	delay(2);
	lcdcmd(0x0e);	//Display cursor blinking
	delay(2);
}
//Delay Function
void delay(int x)
{
	int i,j;
	for(i=0;i<x;i++)
	for(j=0;j<1275;j++);
}
//Lcd-Command Function
void lcdcmd(char A){
	lcdport=A;	// character is conntected to Lcd port-1
	rs=0;	//reg select pin initially for command registor	
	rw=0;	//reg writ pin initially write operation
	en=1; // enable pin is initially high
	delay(1);
	en=0;	//after delay enable pin low
}
//Lcd-Display String Function
void lcddis(char *p){
	while(*p!='\0'){
		lcddata(*p);
		delay(10);
		p++;
	}
}

//Lcd-Data Function to Send Char to Lcd Data port
void lcddata(char value)
{
	lcdport=value; //Data pins are coonected to Lcd-PORT-2
	rs=1;	// reg select pin initially to select Data resistor
	rw=0;	//  reg write pin initially write operation
	en=1;	// enable pin is initially high
	delay(1);
	en=0;	//after delay enable is lowr delay enable pin low
}

// Buzzer Function
void sounder(){
	int i;
	for(i=0;i<5;i++){
		buzzer=1; //buzzer on
		delay(100);
		buzzer=0; //buzzer of
	}
}
//Function to Open Door
void door_open(){
	lcdcmd(0x01); //Clearing LCD-Screen
	lcdcmd(0x02); //4bit mode for LCD-Screen
	lcddis("Opening Door...");
	m1=1; //Motor on
	m2=0; //motor off
}
//Function to Close Door
void door_close(){
	lcdcmd(0x01); 
	lcdcmd(0x02);
	lcddis("Closing Door...");
	m1=0;
	m2=0;
	delay(20);
	//m1=1;
	//m2=0;
	m1=0;
	m2=1;
	delay(500);
	m1=0;
	m2=0;
}
//Dunction to scan single Key and return it to main Function
char scan_key()
{
	char b='a';
	while(b=='a'){
		b=lcdkey();
	}
	return b;
}
//Function to take input from Lcd Keyboard 4*3
char lcdkey(){
	c1=c2=c3=1; // all columns are initially low
	r1=r2=r3=r4=0; // all rows are initially low (active-high)
	
	//when first row is active and others are incactive
	r1=0;
	r2=r3=r4=1;
	//if row=0 column=0 then 1 is returned or if we enter 1 in keypad 2
	if(c1==0){
		lcddata('*');
		delay(2);
		return '1';
	}
	//if row=0 column=1 then 2 is returned or if we enter  in keypad 2
	if(c2==0){
		lcddata('*');
		delay(2);
		return '2';
	}
	//if row=0 column=2 then 3 is returned or if we enter  in keypad 3
	if(c3==0){
		lcddata('*');
		delay(2);
		return '3';
	}
	
	//when second row is active and others are incactive
	r2=0;
	r1=r3=r4=1;
	//if row=1 column=0 then 4 is returned or if we enter  in keypad 4
	if(c1==0){
		lcddata('*');
		delay(2);
		return '4';
	}
	//if row=1 column=1 then 5 is returned or if we enter  in keypad 5
	if(c2==0){
		lcddata('*');
		delay(2);
		return '5';
	}
	//if row=1 column=2 then 6 is returned or if we enter  in keypad 6
	if(c3==0){
		lcddata('*');
		delay(2);
		return '6';
	}
	
	//when third row is active and others are incactive
	r3=0;
	r1=r2=r4=1;
	//if row=2 column=0 then 7 is returned or if we enter  in keypad 7
	if(c1==0){
		lcddata('*');
		delay(2);
		return '7';
	}
	//if row=2 column=1 then 8 is returned or if we enter  in keypad 8
	if(c2==0){
		lcddata('*');
		delay(2);
		return '8';
	}
	//if row=2 column=2 then 9 is returned or if we enter  in keypad 9
	if(c3==0){
		lcddata('*');
		delay(2);
		return '9';
	}
	
	//when fourth row is active and others are incactive
	r4=0;
	r1=r2=r3=1;
	//if row=3 column=0 then * is returned or if we enter  in keypad *
	if(c1==0){
		lcddata('*');
		delay(2);
		return '*';
	}
	//if row=3 column=1 then 0 is returned or if we enter  in keypad 0
	if(c2==0){
		lcddata('*');
		delay(2);
		return '0';
	}
	//if row=3 column=2 then # is returned or if we enter  in keypad #
	if(c3==0){
		lcddata('*');
		delay(2);
		return '#';
	}
	return 'a';
}
