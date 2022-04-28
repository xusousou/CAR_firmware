#include "motor_control.h"
#include "timer.h"

#include "util.h"


extern float rx[4];
int i;
float rx_set[4];
float rx_rate = 1.0;

void rx_value()
{
	for(i=0;i<4;i++)
	{
		rx_set[i]=rx[i]*rx_rate ;
		if( fabsf ( rx_set[i]) <=  STICKS_DEADBAND ) {
			rx_set[i]=0.0f;
		}else{
				if ( rx_set[ i ] >= 0 ) {
				rx_set[ i ] = mapf( rx_set[ i ], STICKS_DEADBAND, 1, 0, 1 );
			} else {
				rx_set[ i ] = mapf( rx_set[ i ], -STICKS_DEADBAND, -1, 0, -1 );
			}
		}
		
	}
}

void forward() //前进
{	
	rx_value();
	
	M1A_pwm(rx_set[3]*100);
	M1B_pwm(0);
	
	M2A_pwm(rx_set[3]*100);
	M2B_pwm(0);
	
	M3A_pwm(rx_set[3]*100);
	M3B_pwm(0);
	
	M4A_pwm(rx_set[3]*100);
	M4B_pwm(0);	
}

void backward() //后退
{	
	rx_value();
	
	M1A_pwm(0);
	M1B_pwm(rx_set[3]*100);
	
	M2A_pwm(0);
	M2B_pwm(rx_set[3]*100);
	
	M3A_pwm(0);
	M3B_pwm(rx_set[3]*100);
	
	M4A_pwm(0);
	M4B_pwm(rx_set[3]*100);	
}

void pan_right()// 右平移
{
	rx_value();
	
	M1A_pwm(0);
	M1B_pwm(rx_set[3]*100);
	
	M2A_pwm(rx_set[3]*100);
	M2B_pwm(0);
	
	M3A_pwm(0);
	M3B_pwm(rx_set[3]*100);
	
	M4A_pwm(rx_set[3]*100);
	M4B_pwm(0);	
}
void pan_left() //左平移
{
	rx_value();
	
	M1A_pwm(rx_set[3]*100);
	M1B_pwm(0);
	
	M2A_pwm(0);
	M2B_pwm(rx_set[3]*100);
	
	M3A_pwm(rx_set[3]*100);
	M3B_pwm(0);
	
	M4A_pwm(0);
	M4B_pwm(rx_set[3]*100);	
}

void turn_up_right()
{
	rx_value();
	
	M1A_pwm(0);
	M1B_pwm(100-rx_set[1]*100);
	
	M2A_pwm(0);
	M2B_pwm(100-rx_set[1]*100);
	
	M3A_pwm(rx_set[3]*100);
	M3B_pwm(0);
	
	M4A_pwm(rx_set[3]*100);
	M4B_pwm(0);	
}

void turn_up_left()
{
	rx_value();
	
	M1A_pwm(rx_set[3]*100);
	M1B_pwm(0);
	
	M2A_pwm(rx_set[3]*100);
	M2B_pwm(0);
	
	M3A_pwm(0);
	M3B_pwm(100-rx_set[1]*100);
	
	M4A_pwm(0);
	M4B_pwm(100-rx_set[1]*100);	
}


void turn_down_right()
{
	rx_value();
		
	M1A_pwm(100+rx_set[1]*100);
	M1B_pwm(0);
	
	M2A_pwm(100+rx_set[1]*100);
	M2B_pwm(0);
	
	M3A_pwm(0);
	M3B_pwm(rx_set[2]*100);
	
	M4A_pwm(0);
	M4B_pwm(rx_set[3]*100);	
	
}

void turn_down_left()
{
	rx_value();
	
	M1A_pwm(0);
	M1B_pwm(rx_set[1]*100);
	
	M2A_pwm(0);
	M2B_pwm(rx_set[1]*100);
	
	M3A_pwm(100-rx_set[1]*100);
	M3B_pwm(0);
	
	M4A_pwm(100-rx_set[1]*100);
	M4B_pwm(0);		

}

void up_right()
{
	rx_value();
	
	M1A_pwm(rx_set[3]*100);
	M1B_pwm(0);
	
	M2A_pwm(0);
	M2B_pwm(0);
	
	M3A_pwm(rx_set[3]*100);
	M3B_pwm(0);
	
	M4A_pwm(0);
	M4B_pwm(0);	
}

void up_left()
{
	rx_value();
	
	M1A_pwm(0);
	M1B_pwm(0);
	
	M2A_pwm(rx_set[3]*100);
	M2B_pwm(0);
	
	M3A_pwm(0);
	M3B_pwm(0);
	
	M4A_pwm(rx_set[3]*100);
	M4B_pwm(0);	
}
void down_right()
{
	rx_value();
	
	M1A_pwm(0);
	M1B_pwm(0);
	
	M2A_pwm(0);
	M2B_pwm(rx_set[3]*100);
	
	M3A_pwm(0);
	M3B_pwm(0);
	
	M4A_pwm(0);
	M4B_pwm(rx_set[3]*100);	
}
void down_left()
{
	rx_value();
	
	M1A_pwm(0);
	M1B_pwm(rx_set[3]*100);
	
	M2A_pwm(0);
	M2B_pwm(0);
	
	M3A_pwm(0);
	M3B_pwm(rx_set[3]*100);
	
	M4A_pwm(0);
	M4B_pwm(0);	
}



void slid() //滑行
{
	M1A_pwm(0);
	M1B_pwm(0);
	
	M2A_pwm(0);
	M2B_pwm(0);
	
	M3A_pwm(0);
	M3B_pwm(0);
	
	M4A_pwm(0);
	M4B_pwm(0);	
}

void brush() //刹车
{
	M1A_pwm(50);
	M1B_pwm(50);
	
	M2A_pwm(50);
	M2B_pwm(50);
	
	M3A_pwm(50);
	M3B_pwm(50);
	
	M4A_pwm(50);
	M4B_pwm(50);	
}

void Motor_Control()
{
	rx_value();
	if(rx_set[1] >= STICKS_DEADBAND && rx_set[0] == 0.0f && rx_set[2] == 0.0f){
			forward();
		}else if(rx_set[1] >= 0.0f && rx_set[0] == 0.0f && rx_set[2] <= -STICKS_DEADBAND){
			turn_up_left();
		}else if(rx_set[1] >= 0.0f && rx_set[0] == 0.0f && rx_set[2] >= STICKS_DEADBAND){
			turn_up_right();
		}else if(rx_set[1] <= -STICKS_DEADBAND && rx_set[0] == 0.0f && rx_set[2] == 0.0f){
			backward();
		}else if(rx_set[1] <= 0.0f && rx_set[0] == 0.0f && rx_set[2] <= -STICKS_DEADBAND){
			turn_down_left();
		}else if(rx_set[1] <= 0.0f && rx_set[0] == 0.0f && rx_set[2] >= STICKS_DEADBAND){
			turn_down_right();
		}else if(rx_set[1] == 0.0f && rx_set[0] >= STICKS_DEADBAND && rx_set[2] == 0.0f){
			pan_left();
		}else if(rx_set[1] == 0.0f && rx_set[0] <= -STICKS_DEADBAND && rx_set[2] == 0.0f){
			pan_right();
		}else if(rx_set[1] >= STICKS_DEADBAND && rx_set[0] >= STICKS_DEADBAND && rx_set[2] == 0.0f){
			up_right();
		}else if(rx_set[1] >= STICKS_DEADBAND && rx_set[0] <= -STICKS_DEADBAND && rx_set[2] == 0.0f){
			up_left();
		}else if(rx_set[1] <= -STICKS_DEADBAND && rx_set[0] >= STICKS_DEADBAND && rx_set[2] == 0.0f){
			down_right();
		}else if(rx_set[1] <= -STICKS_DEADBAND && rx_set[0] <= -STICKS_DEADBAND && rx_set[2] == 0.0f){
			down_left();
		}
		else{
					slid();
				}
}

