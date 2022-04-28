/*
The MIT License (MIT)

Copyright (c) 2016 silverx

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/


// STM32 acro firmware
// files of this project should be assumed MIT licence unless otherwise noted


#include "project.h"

#include "timer.h"
#include "led.h"
#include "util.h"
#include "sixaxis.h"
#include "drv_adc.h"
#include "drv_time.h"
#include "drv_softi2c.h"
#include "config.h"
#include "drv_pwm.h"
#include "drv_adc.h"
#include "drv_gpio.h"
#include "drv_serial.h"
#include "rx.h"
#include "drv_spi.h"
#include "control.h"
#include "pid.h"
#include "defines.h"
#include "drv_i2c.h"
#include "drv_softi2c.h"
#include "drv_serial.h"
#include "buzzer.h"
#include "drv_fmc2.h"
#include "gestures.h"
#include "binary.h"
#include "drv_osd.h"
#include "motor_control.h"
#include <stdio.h>
#include <math.h>
#include <inttypes.h>

#ifdef USE_SERIAL_4WAY_BLHELI_INTERFACE
#include "drv_softserial.h"
#include "serial_4way.h"
#endif									   
						   
						   
#if defined (__GNUC__)&& !( defined (SOFT_LPF_NONE) || defined (SOFT_LPF_1ST_HZ) || defined (SOFT_LPF_2ST_HZ) )
#warning the soft lpf may not work correctly with gcc due to longer loop time
#endif

#define DEBUG

#ifdef DEBUG
#include "debug.h"
debug_type debug;
#endif



unsigned char OSD_DATA[20] = {0x01,0x02,0x03,0x04};

// hal
void clk_init(void);
void imu_init(void);
extern void flash_load( void);
extern void flash_hard_coded_pid_identifier(void);


// looptime in seconds
float looptime;
// filtered battery in volts
float vbattfilt = 0.0;
float vbatt_comp = 4.2;
// voltage reference for vcc compensation
float vreffilt = 1.0;
// average of all motors
float thrfilt = 0;

unsigned int lastlooptime;
// signal for lowbattery
int lowbatt = 1;	

//int minindex = 0;

// holds the main four channels, roll, pitch , yaw , throttle
float rx[4];

// holds auxilliary channels
// the last 2 are always on and off respectively
char aux[AUXNUMBER] = { 0 ,0 ,0 , 0 , 0 , 0};
char lastaux[AUXNUMBER];
// if an aux channel has just changed
char auxchange[AUXNUMBER];

// bind / normal rx mode
extern int rxmode;
// failsafe on / off
extern int failsafe;
extern float hardcoded_pid_identifier;
extern int onground;
int in_air;
int armed_state;
int arming_release;
int binding_while_armed = 1;

//Experimental Flash Memory Feature
int flash_feature_1 = 0;
int flash_feature_2 = 0;
int flash_feature_3 = 0;

// for led flash on gestures
int ledcommand = 0;
int ledblink = 0;
unsigned long ledcommandtime = 0;

void failloop( int val);
#ifdef USE_SERIAL_4WAY_BLHELI_INTERFACE
volatile int switch_to_4way = 0;
static void setup_4way_external_interrupt(void);
#endif									   
int random_seed = 0;


int main(void)
{
	
	delay(1000);

	

#ifdef ENABLE_OVERCLOCK
clk_init();
#endif
	


  gpio_init();	
  ledon(255);									//Turn on LED during boot so that if a delay is used as part of using programming pins for other functions, the FC does not appear inactive while programming times out
	spi_init();
	
  time_init();
	osd_spi_init();

#if defined(RX_DSMX_2048) || defined(RX_DSM2_1024)    
		rx_spektrum_bind(); 
#endif
		
	delay(100000);
		
	i2c_init();	
		
	TIM1_Config();
	TIM1_PWM_Config(249);
	TIM3_Config();
	TIM3_PWM_Config(249);

    delay(1000);
 
	sixaxis_init();

	if ( sixaxis_check() ) 
	{
		
	}
	else 
	{
        //gyro not found   
		failloop(4);
	}
	
	adc_init();
//set always on channel to on
aux[CH_ON] = 1;	
	
#ifdef AUX1_START_ON
aux[CH_AUX1] = 1;
#endif	
	
    rx_init();
	
int count = 0;
	
while ( count < 64 )
{
	vbattfilt += adc_read(0);
	delay(1000);
	count++;
}

 vbattfilt = vbattfilt/64;	
// startvref = startvref/64;

    gyro_cal();


#ifdef SERIAL_ENABLE
serial_init();
#endif



imu_init();

extern int liberror;
if ( liberror ) 
{
    failloop(7);
}

 lastlooptime = gettime();


//
//
// 		MAIN LOOP
//
//

#ifdef USE_SERIAL_4WAY_BLHELI_INTERFACE
	setup_4way_external_interrupt();
#endif  

	while(1)
	{ 
		// gettime() needs to be called at least once per second 
		unsigned long time = gettime(); 
		looptime = ((uint32_t)( time - lastlooptime));
		if ( looptime <= 0 ) looptime = 1;
		looptime = looptime * 1e-6f;
		if ( looptime > 0.02f ) // max loop 20ms
		{
			failloop( 6);	
			//endless loop			
		}
	
		#ifdef DEBUG				
		debug.totaltime += looptime;
		lpf ( &debug.timefilt , looptime, 0.998 );
		#endif
		lastlooptime = time;
		
		if ( liberror > 20) 
		{
			failloop(8);
			// endless loop
		}

        // read gyro and accelerometer data	
		sixaxis_read();
		
        // all flight calculations and motors
//		control();
			Motor_Control();
        // attitude calculations for level mode 		
 		extern void imu_calc(void);		
		imu_calc(); 
           
#ifdef DEBUG
	debug.vbatt_comp = vbatt_comp ;
#endif		

if ( LED_NUMBER > 0)
{
// led flash logic	
    if ( lowbatt )
        ledflash ( 500000 , 8);
    else
    {
        if ( rxmode == RXMODE_BIND)
        {// bind mode
            ledflash ( 100000, 12);
        }else
        {// non bind
            if ( failsafe) 
                {
                    ledflash ( 500000, 15);			
                }
            else 
            {  
                int leds_on = !aux[LEDS_ON];
                if (ledcommand)
                {
                    if (!ledcommandtime)
                      ledcommandtime = gettime();
                    if (gettime() - ledcommandtime > 500000)
                    {
                        ledcommand = 0;
                        ledcommandtime = 0;
                    }
                    ledflash(100000, 8);
                }
                else if (ledblink)
                {
                    unsigned long time = gettime();
                    if (!ledcommandtime)
                    {
                        ledcommandtime = time;
                        if ( leds_on) ledoff(255);
                        else ledon(255); 
                    }
                    if ( time - ledcommandtime > 500000)
                    {
                        ledblink--;
                        ledcommandtime = 0;
                    }
                     if ( time - ledcommandtime > 300000)
                    {
                        if ( leds_on) ledon(255);
                        else  ledoff(255);
                    }
                }
                else if ( leds_on )
                {
                    if ( LED_BRIGHTNESS != 15)	
                    led_pwm(LED_BRIGHTNESS);
                    else ledon(255);
                }
                else ledoff(255);
            }
        } 		       
    }
}

#ifdef BUZZER_ENABLE	
	buzzer();
#endif

// receiver function
checkrx();

/*osd data transmit*/
while ( (gettime() - time) < LOOPTIME 	);
static uint8 i;
    switch(i)
    {
        case 0  :OSD_Data_Send(1,(int)(vbattfilt*100));//transmit vbattfil 
        break; /* 可选的 */
        case 1  :OSD_Data_Send(2,aux[0]);  //transmit ARMING state	
        break; /* 可选的 */
        case 2  :OSD_Data_Send(3,1);//transmit vbattfilt
        break; /* 可选的 */
        default : break;
    }
i++;
if(i>2)
{
	i=0;
}


	}// end loop
	

}

// 2 - low battery at powerup - if enabled by config
// 3 - radio chip not detected
// 4 - Gyro not found
// 5 - clock , intterrupts , systick
// 6 - loop time issue
// 7 - i2c error 
// 8 - i2c error main loop


void failloop( int val)
{
	for ( int i = 0 ; i <= 3 ; i++)
	{
//		pwm_set( i ,0 );
	}	
	while(1)
	{
		for ( int i = 0 ; i < val; i++)
		{
		 ledon( 255);		
		 delay(200000);
		 ledoff( 255);	
		 delay(200000);			
		}
		delay(800000);
	}	
	
}


void HardFault_Handler(void)
{
	failloop(5);
}
void MemManage_Handler(void) 
{
	failloop(5);
}
void BusFault_Handler(void) 
{
	failloop(5);
}
void UsageFault_Handler(void) 
{
	failloop(5);
}


