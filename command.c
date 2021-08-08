#include"CPU.h"

/* 停机指令 -- 0 */
	void stop ( number* num , MTC* cur , int* data )
	{
		
		/* 计算 ir */ 
			cur->ir = 256*num->com + 16*num->front + num->behind ;
		
		
		/* 计数器 */
			cur->ip = cur->ip + 4 ;
		
		
		return ;
	}
	
	
/* 数据传送指令 -- 1 */
	void transport ( number* num , MTC* cur , int* data )
	{
		
		/* 计算 ir */ 
			cur->ir = 256*num->com + 16*num->front + num->behind ;
		
		
		/* 三种传送指令 */ 
			if ( num->behind == 0 )
				cur->ax[num->front] = num->im ;
			
			else if ( num->behind >= 1 && num->behind <= 4 )
				data[ ( cur->ax[num->front] - 16384 ) / 2 ] = cur->ax[num->behind] ;
			
			else if ( num->behind >= 5 && num->behind <= 8 )
				cur->ax[num->front] = data[ ( cur->ax[num->behind] - 16384 ) / 2 ] ;
		
			
		/* 计数器 */
			cur->ip = cur->ip + 4 ;
		
		
		return ;
	}


/* 算术运算指令 -- 2 3 4 5 */
	void arithmetic ( number* num , MTC* cur , int* data )
	{
		
		/* 计算 ir */ 
			cur->ir = 256*num->com + 16*num->front + num->behind ;
		
		
		/* 四种运算符 */
			
			// + 运算 
			if ( num->com == 2 ) {
				
				if ( num->behind == 0 )
					cur->ax[num->front] = cur->ax[num->front] + num->im ;
				
				else
					cur->ax[num->front] = cur->ax[num->front] + data[ ( cur->ax[num->behind] - 16384 ) / 2 ] ;
					
			}
			
			// - 运算 
			if ( num->com == 3 ) {
				
				if ( num->behind == 0 )
					cur->ax[num->front] = cur->ax[num->front] - num->im ;
				
				else
					cur->ax[num->front] = cur->ax[num->front] - data[ ( cur->ax[num->behind] - 16384 ) / 2 ] ;
					
			}
			
			// * 运算 
			if ( num->com == 4 ) {
				
				if ( num->behind == 0 )
					cur->ax[num->front] = cur->ax[num->front] * num->im ;
				
				else
					cur->ax[num->front] = cur->ax[num->front] * data[ ( cur->ax[num->behind] - 16384 ) / 2 ] ;
					
			}
			
			// - 运算 
			if ( num->com == 5 ) {
				
				if ( num->behind == 0 )
					cur->ax[num->front] = cur->ax[num->front] / num->im ;
				
				else
					cur->ax[num->front] = cur->ax[num->front] / data[ ( cur->ax[num->behind] - 16384 ) / 2 ] ;
					
			}
		
		
		/* 计数器 */
			cur->ip = cur->ip + 4 ;
		
		
		return ;
	}


/* 逻辑运算指令 -- 6 7 8 */
	void logic ( number* num , MTC* cur , int* data )
	{
		
		/* 计算 ir */ 
			cur->ir = 256*num->com + 16*num->front + num->behind ;
		
		
		/* 三种运算符 */
		
			// && 运算 
			if ( num->com == 6 ) {
				
				if ( num->behind == 0 )
					cur->ax[num->front] = cur->ax[num->front] && num->im ;
				
				else
					cur->ax[num->front] = cur->ax[num->front] && data[ ( cur->ax[num->behind] - 16384 ) / 2 ] ;
			}
			
			// || 运算 
			if ( num->com == 7 ) {
				
				if ( num->behind == 0 )
					cur->ax[num->front] = cur->ax[num->front] || num->im ;
				
				else
					cur->ax[num->front] = cur->ax[num->front] || data[ ( cur->ax[num->behind] - 16384 ) / 2 ] ;
			}
			
			// ! 运算 
			if ( num->com == 8 ) {
				
				if ( num->behind == 0 )
					cur->ax[num->front] = ! cur->ax[num->front] ;
				
				else
					data[ ( cur->ax[num->behind] - 16384 ) / 2 ] = ! data[ ( cur->ax[num->behind] - 16384 ) / 2 ] ;
			}
		
		
		/* 计数器 */
			cur->ip = cur->ip + 4 ;
		
		
		return ;
	}


/* 比较指令 -- 9 */
	void compare ( number* num , MTC* cur , int* data )
	{
		
		/* 计算 ir */ 
			cur->ir = 256*num->com + 16*num->front + num->behind ;
		
		
		/* 寄存器 1 2 3 4 与 立即数作比较 */
			if ( num->behind == 0 ) {
				
				if ( cur->ax[num->front] > num->im )
					cur->flag = 1 ;
				
				else if ( cur->ax[num->front] == num->im )
					cur->flag = 0 ;
				
				else if ( cur->ax[num->front] < num->im )
					cur->flag = -1 ;
					
			}
		
		/* 寄存器 1 2 3 4 和 寄存器 5 6 7 8 作比较 */
			else {
				
				if ( cur->ax[num->front] > data[ ( cur->ax[num->behind] - 16384 ) / 2 ] )
					cur->flag = 1 ;
				
				else if ( cur->ax[num->front] == data[ ( cur->ax[num->behind] - 16384 ) / 2 ] )
					cur->flag = 0 ;
				
				else if ( cur->ax[num->front] < data[ ( cur->ax[num->behind] - 16384 ) / 2 ] )
					cur->flag = -1 ;
					
			}
		
		
		/* 计数器 */
			cur->ip = cur->ip + 4 ;
		
		
		return ;
	}


/* 跳转指令 -- 10 */
	void skip ( number* num , MTC* cur , int* data )
	{
		
		/* 计算 ir */ 
			cur->ir = 256*num->com + 16*num->front + num->behind ;
		
		
		/* 三种跳转指令 */
			if ( num->behind == 0 )
				cur->ip = cur->ip + num->im ;
			
			else if ( num->behind == 1 && cur->flag == 0 )
				cur->ip = cur->ip + num->im ;
			
			else if ( num->behind == 2 && cur->flag == 1 )
				cur->ip = cur->ip + num->im ;
			
			else if ( num->behind == 3 && cur->flag == -1 )
				cur->ip = cur->ip + num->im ;
		
		/* 不跳转 */
			else
				cur->ip = cur->ip + 4 ;
		
		
		return ;
	}


/* 输入输出指令 -- 11 12 */
	void putIn_Out ( number* num , MTC* cur , int* data )
	{
		
		/* 计算 ir */ 
			cur->ir = 256*num->com + 16*num->front + num->behind ;
		
		
		/* 输入 */
			if ( num->com == 11 ) {
				
				printf ( "in:\n" ) ;
				scanf ( "%d" , & cur->ax[num->front] ) ;
			}
		
		/* 输出 */
			else if ( num->com == 12 ) {
				
				printf ( "id = %d out: %d\n\n" , cur->id , cur->ax[num->front] ) ;
			}
		
		/* 计数器 */
			cur->ip = cur->ip + 4 ;
		
		
		return ;
	}


/* 上锁 -- 13 */
	void lock ( number* num , MTC* cur , segment* solid )
	{
		
		/* 计算 ir */ 
			cur->ir = 256*num->com + 16*num->front + num->behind ;
		
		/* 锁住句柄 */
			WaitForSingleObject ( solid->hMutex , INFINITE ) ;
		
		/* 计数器 */
			cur->ip = cur->ip + 4 ;
		
		
		return ;
	}


/* 释放 -- 14 */
	void release ( number* num , MTC* cur , segment* solid )
	{
		
		/* 计算 ir */ 
			cur->ir = 256*num->com + 16*num->front + num->behind ;
		
		/* 释放句柄 */
			ReleaseMutex ( solid->hMutex ) ;
		
		/* 计数器 */
			cur->ip = cur->ip + 4 ;
		
		
		return ;
	}


/* 睡眠 -- 15 */
	void funcSleep ( number* num , MTC* cur )
	{
		
		/* 计算 ir */ 
			cur->ir = 256*num->com + 16*num->front + num->behind ;
		
		/* 睡眠立即数秒 */
			Sleep ( num->im ); 
		
		/* 计数器 */
			cur->ip = cur->ip + 4 ;
		
		
		return ;
	}
