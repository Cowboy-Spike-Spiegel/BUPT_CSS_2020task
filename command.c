#include"CPU.h"

/* ͣ��ָ�� -- 0 */
	void stop ( number* num , MTC* cur , int* data )
	{
		
		/* ���� ir */ 
			cur->ir = 256*num->com + 16*num->front + num->behind ;
		
		
		/* ������ */
			cur->ip = cur->ip + 4 ;
		
		
		return ;
	}
	
	
/* ���ݴ���ָ�� -- 1 */
	void transport ( number* num , MTC* cur , int* data )
	{
		
		/* ���� ir */ 
			cur->ir = 256*num->com + 16*num->front + num->behind ;
		
		
		/* ���ִ���ָ�� */ 
			if ( num->behind == 0 )
				cur->ax[num->front] = num->im ;
			
			else if ( num->behind >= 1 && num->behind <= 4 )
				data[ ( cur->ax[num->front] - 16384 ) / 2 ] = cur->ax[num->behind] ;
			
			else if ( num->behind >= 5 && num->behind <= 8 )
				cur->ax[num->front] = data[ ( cur->ax[num->behind] - 16384 ) / 2 ] ;
		
			
		/* ������ */
			cur->ip = cur->ip + 4 ;
		
		
		return ;
	}


/* ��������ָ�� -- 2 3 4 5 */
	void arithmetic ( number* num , MTC* cur , int* data )
	{
		
		/* ���� ir */ 
			cur->ir = 256*num->com + 16*num->front + num->behind ;
		
		
		/* ��������� */
			
			// + ���� 
			if ( num->com == 2 ) {
				
				if ( num->behind == 0 )
					cur->ax[num->front] = cur->ax[num->front] + num->im ;
				
				else
					cur->ax[num->front] = cur->ax[num->front] + data[ ( cur->ax[num->behind] - 16384 ) / 2 ] ;
					
			}
			
			// - ���� 
			if ( num->com == 3 ) {
				
				if ( num->behind == 0 )
					cur->ax[num->front] = cur->ax[num->front] - num->im ;
				
				else
					cur->ax[num->front] = cur->ax[num->front] - data[ ( cur->ax[num->behind] - 16384 ) / 2 ] ;
					
			}
			
			// * ���� 
			if ( num->com == 4 ) {
				
				if ( num->behind == 0 )
					cur->ax[num->front] = cur->ax[num->front] * num->im ;
				
				else
					cur->ax[num->front] = cur->ax[num->front] * data[ ( cur->ax[num->behind] - 16384 ) / 2 ] ;
					
			}
			
			// - ���� 
			if ( num->com == 5 ) {
				
				if ( num->behind == 0 )
					cur->ax[num->front] = cur->ax[num->front] / num->im ;
				
				else
					cur->ax[num->front] = cur->ax[num->front] / data[ ( cur->ax[num->behind] - 16384 ) / 2 ] ;
					
			}
		
		
		/* ������ */
			cur->ip = cur->ip + 4 ;
		
		
		return ;
	}


/* �߼�����ָ�� -- 6 7 8 */
	void logic ( number* num , MTC* cur , int* data )
	{
		
		/* ���� ir */ 
			cur->ir = 256*num->com + 16*num->front + num->behind ;
		
		
		/* ��������� */
		
			// && ���� 
			if ( num->com == 6 ) {
				
				if ( num->behind == 0 )
					cur->ax[num->front] = cur->ax[num->front] && num->im ;
				
				else
					cur->ax[num->front] = cur->ax[num->front] && data[ ( cur->ax[num->behind] - 16384 ) / 2 ] ;
			}
			
			// || ���� 
			if ( num->com == 7 ) {
				
				if ( num->behind == 0 )
					cur->ax[num->front] = cur->ax[num->front] || num->im ;
				
				else
					cur->ax[num->front] = cur->ax[num->front] || data[ ( cur->ax[num->behind] - 16384 ) / 2 ] ;
			}
			
			// ! ���� 
			if ( num->com == 8 ) {
				
				if ( num->behind == 0 )
					cur->ax[num->front] = ! cur->ax[num->front] ;
				
				else
					data[ ( cur->ax[num->behind] - 16384 ) / 2 ] = ! data[ ( cur->ax[num->behind] - 16384 ) / 2 ] ;
			}
		
		
		/* ������ */
			cur->ip = cur->ip + 4 ;
		
		
		return ;
	}


/* �Ƚ�ָ�� -- 9 */
	void compare ( number* num , MTC* cur , int* data )
	{
		
		/* ���� ir */ 
			cur->ir = 256*num->com + 16*num->front + num->behind ;
		
		
		/* �Ĵ��� 1 2 3 4 �� ���������Ƚ� */
			if ( num->behind == 0 ) {
				
				if ( cur->ax[num->front] > num->im )
					cur->flag = 1 ;
				
				else if ( cur->ax[num->front] == num->im )
					cur->flag = 0 ;
				
				else if ( cur->ax[num->front] < num->im )
					cur->flag = -1 ;
					
			}
		
		/* �Ĵ��� 1 2 3 4 �� �Ĵ��� 5 6 7 8 ���Ƚ� */
			else {
				
				if ( cur->ax[num->front] > data[ ( cur->ax[num->behind] - 16384 ) / 2 ] )
					cur->flag = 1 ;
				
				else if ( cur->ax[num->front] == data[ ( cur->ax[num->behind] - 16384 ) / 2 ] )
					cur->flag = 0 ;
				
				else if ( cur->ax[num->front] < data[ ( cur->ax[num->behind] - 16384 ) / 2 ] )
					cur->flag = -1 ;
					
			}
		
		
		/* ������ */
			cur->ip = cur->ip + 4 ;
		
		
		return ;
	}


/* ��תָ�� -- 10 */
	void skip ( number* num , MTC* cur , int* data )
	{
		
		/* ���� ir */ 
			cur->ir = 256*num->com + 16*num->front + num->behind ;
		
		
		/* ������תָ�� */
			if ( num->behind == 0 )
				cur->ip = cur->ip + num->im ;
			
			else if ( num->behind == 1 && cur->flag == 0 )
				cur->ip = cur->ip + num->im ;
			
			else if ( num->behind == 2 && cur->flag == 1 )
				cur->ip = cur->ip + num->im ;
			
			else if ( num->behind == 3 && cur->flag == -1 )
				cur->ip = cur->ip + num->im ;
		
		/* ����ת */
			else
				cur->ip = cur->ip + 4 ;
		
		
		return ;
	}


/* �������ָ�� -- 11 12 */
	void putIn_Out ( number* num , MTC* cur , int* data )
	{
		
		/* ���� ir */ 
			cur->ir = 256*num->com + 16*num->front + num->behind ;
		
		
		/* ���� */
			if ( num->com == 11 ) {
				
				printf ( "in:\n" ) ;
				scanf ( "%d" , & cur->ax[num->front] ) ;
			}
		
		/* ��� */
			else if ( num->com == 12 ) {
				
				printf ( "id = %d out: %d\n\n" , cur->id , cur->ax[num->front] ) ;
			}
		
		/* ������ */
			cur->ip = cur->ip + 4 ;
		
		
		return ;
	}


/* ���� -- 13 */
	void lock ( number* num , MTC* cur , segment* solid )
	{
		
		/* ���� ir */ 
			cur->ir = 256*num->com + 16*num->front + num->behind ;
		
		/* ��ס��� */
			WaitForSingleObject ( solid->hMutex , INFINITE ) ;
		
		/* ������ */
			cur->ip = cur->ip + 4 ;
		
		
		return ;
	}


/* �ͷ� -- 14 */
	void release ( number* num , MTC* cur , segment* solid )
	{
		
		/* ���� ir */ 
			cur->ir = 256*num->com + 16*num->front + num->behind ;
		
		/* �ͷž�� */
			ReleaseMutex ( solid->hMutex ) ;
		
		/* ������ */
			cur->ip = cur->ip + 4 ;
		
		
		return ;
	}


/* ˯�� -- 15 */
	void funcSleep ( number* num , MTC* cur )
	{
		
		/* ���� ir */ 
			cur->ir = 256*num->com + 16*num->front + num->behind ;
		
		/* ˯���������� */
			Sleep ( num->im ); 
		
		/* ������ */
			cur->ip = cur->ip + 4 ;
		
		
		return ;
	}
