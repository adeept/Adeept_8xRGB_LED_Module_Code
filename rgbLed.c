#include <wiringPi.h>  
#include <softPwm.h>  
#include <stdio.h> 
#include <stdlib.h>
#include <time.h>

#define LedPinRed     8  
#define LedPinGreen  10  
#define LedPinBlue    9  

const int colors[] = {0xFF0000, 0x00FF00, 0x0000FF, 0xFFFF00, 0x00FFFF, 0xFF00FF, 0xFFFFFF, 0x9400D3};  

int map(int x, int in_min, int in_max, int out_min, int out_max)     
{  
	return (x -in_min) * (out_max - out_min) / (in_max - in_min) + out_min;  
}  

void ledInit(void)  
{ 
	int i = 0;

	for(i = 0; i < 8; i++){
		pinMode(i, OUTPUT);			
	}
	
	for(i = 0; i < 8; i++){
		digitalWrite(i, HIGH);		//all leds off
	}

	softPwmCreate(LedPinRed,  0, 100);  //create a soft pwm, original duty cycle is 0Hz, range is 0~100   
	softPwmCreate(LedPinGreen,0, 100);  
	softPwmCreate(LedPinBlue, 0, 100);  
}  

void ledColorSet(int color)        //set color, for example: 0xde3f47  
{  
	int r_val, g_val, b_val;  

	r_val = (color & 0xFF0000) >> 16;  //get red value  
	g_val = (color & 0x00FF00) >> 8;   //get green value  
	b_val = (color & 0x0000FF) >> 0;   //get blue value  

	r_val = map(r_val, 0, 255, 0, 100);    //change a num(0~255) to 0~100  
	g_val = map(g_val, 0, 255, 0, 100);  
	b_val = map(b_val, 0, 255, 0, 100);  

	softPwmWrite(LedPinRed,   100 - r_val);  //change duty cycle  
	softPwmWrite(LedPinGreen, 100 - g_val);  
	softPwmWrite(LedPinBlue,  100 - b_val);  
}

void ledOff(int num)
{
	digitalWrite(num, HIGH);	//num: 0~7	
}

void ledCtrl(int num, int color) //num: 0~7
{
	digitalWrite(num, LOW);	// led(num) on
	ledColorSet(color);
}

int main(void)  
{  
	int m; 
	int n;  

	if(wiringPiSetup() < 0){ //when initialize wiringPi failed, print message to screen  
		printf("setup wiringPi failed !\n");  
		return -1;   
	}  

	ledInit();  

	int	dly = 100;

	srand(time(0));

	while(1){
		for(m = 0; m < 8; m++){
			n = rand() % 9 - 1;
			ledCtrl(m, colors[n]);
			delay(dly);  
			ledOff(m);	
		}
	}

	return 0;  
}  
