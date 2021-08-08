#include"CPU.h"

/* �߳� 1 */
	unsigned __stdcall Fun1Proc ( void* pArguments )
	{
		
		//  ��̬����Ĵ������� 
		segment* solid = (segment*) pArguments ;
		
		//  ����Ĵ����ռ�
		MTC* cur = generateStruct ( 1 ) ;
		
		//  �����в����Ĵ����ռ� 
		number* num = ( number* ) malloc ( sizeof ( number ) ) ;
		
		solid->end1 = 1 ; // ��Ϊ���ź� 
		
		/* ִ�� */
		int i , j ; // ��̬�±�
		int Flag ;
		for ( i = 0 , Flag = 1 ; Flag ; ) {
			
			// ���� command          ( ����ʮ������ )
			for ( num->com = 0 ,j = 0 ; j < 8 ; j++ )
				num->com = 2*num->com + solid->txt1[i][j] ;
			
			// ���� front    ( ǰһ���Ĵ���ʮ������ )
			for ( num->front = 0 ; j < 12 ; j++ )
				num->front = 2*num->front + solid->txt1[i][j] ;
				
			// ���� behind   ( ��һ���Ĵ���ʮ������ )
			for ( num->behind = 0 ; j < 16 ; j++ )
				num->behind = 2*num->behind + solid->txt1[i][j] ;
			
			// ���� immediateValue ( ������ʮ������ )
			num->im = countImmediateValue ( solid->txt1 , i , j ) ;
			
			/* ѡ��ָ����� */
			switch ( num->com ) {
			 	
				case 0  :
					stop       ( num , cur , solid->dataSegment ) ;
					break ;
					
				case 1  :
					transport  ( num , cur , solid->dataSegment ) ;
					break ;
					
				case 2  :
					arithmetic ( num , cur , solid->dataSegment ) ;
					break ;
					
				case 3  :
					arithmetic ( num , cur , solid->dataSegment ) ;
					break ;
					
				case 4  :
					arithmetic ( num , cur , solid->dataSegment ) ;
					break ;
					
				case 5  :
					arithmetic ( num , cur , solid->dataSegment ) ;
					break ;
					
				case 6  :
					logic      ( num , cur , solid->dataSegment ) ;
					break ;
					
				case 7  :
					logic      ( num , cur , solid->dataSegment ) ;
					break ;
					
				case 8  :
					logic      ( num , cur , solid->dataSegment ) ;
					break ;
					
				case 9  :
					compare    ( num , cur , solid->dataSegment ) ;
					break ;
					
				case 10 :
					skip       ( num , cur , solid->dataSegment ) ;
					break ;
					
				case 11 :
					putIn_Out  ( num , cur , solid->dataSegment ) ;
					break ;
					
				case 12 :
					putIn_Out  ( num , cur , solid->dataSegment ) ;
					solid->lock = 2 ;
					break ;
					
				case 13 :
					while ( solid->lock == 2 && solid->end2 == 1 ) ;
					lock       ( num , cur , solid              ) ;
					break ;
					
				case 14 :
					release    ( num , cur , solid              ) ;
					break ;
					
				case 15 :
					funcSleep  ( num , cur                      ) ;
					break ;
			}
			
			funcPrint ( num->com , cur , solid ) ;  // ����Ĵ���
			
			i = cur->ip/4 ; // �������±� i 
			
			if ( num->com == 0 )  //  ͣ�� 
				Flag = 0 ;
		}
		
		solid->end1 = 0 ; // ���ź�1��Ϊ�ѽ��� 
		
		//  �ͷ������ڴ�
		free ( cur->ax ) ;
		free ( cur ) ;
		free ( num ) ;
		
		return 0 ;
	}


/* �߳� 2 */
	unsigned __stdcall Fun2Proc ( void* pArguments )
	{
		
		//  ��̬����Ĵ������� 
		segment* solid = (segment*) pArguments ;
		
		//  ����Ĵ����ռ�
		MTC* cur = generateStruct ( 2 ) ;
		
		//  �����в����Ĵ����ռ� 
		number* num = ( number* ) malloc ( sizeof ( number ) ) ; 
		
		solid->end2 = 1 ; // ��Ϊ���ź� 
		
		/* ִ�� */
		int i , j ; // ��̬�±�
		int Flag ;
		for ( i = 0 , Flag = 1 ; Flag ; ) {
			
			// ���� command          ( ����ʮ������ )
			for ( num->com = 0 ,j = 0 ; j < 8 ; j++ )
				num->com = 2*num->com + solid->txt2[i][j] ;
			
			// ���� front    ( ǰһ���Ĵ���ʮ������ )
			for ( num->front = 0 ; j < 12 ; j++ )
				num->front = 2*num->front + solid->txt2[i][j] ;
			
			// ���� behind   ( ��һ���Ĵ���ʮ������ )
			for ( num->behind = 0 ; j < 16 ; j++ )
				num->behind = 2*num->behind + solid->txt2[i][j] ;
			
			// ���� immediateValue ( ������ʮ������ )
			num->im = countImmediateValue ( solid->txt2 , i , j ) ;
			
			/* ѡ��ָ����� */
			switch ( num->com ) {
			 	
				case 0  :
					stop       ( num , cur , solid->dataSegment ) ;
					break ;
					
				case 1  :
					transport  ( num , cur , solid->dataSegment ) ;
					break ;
					
				case 2  :
					arithmetic ( num , cur , solid->dataSegment ) ;
					break ;
					
				case 3  :
					arithmetic ( num , cur , solid->dataSegment ) ;
					break ;
					
				case 4  :
					arithmetic ( num , cur , solid->dataSegment ) ;
					break ;
					
				case 5  :
					arithmetic ( num , cur , solid->dataSegment ) ;
					break ;
					
				case 6  :
					logic      ( num , cur , solid->dataSegment ) ;
					break ;
					
				case 7  :
					logic      ( num , cur , solid->dataSegment ) ;
					break ;
					
				case 8  :
					logic      ( num , cur , solid->dataSegment ) ;
					break ;
					
				case 9  :
					compare    ( num , cur , solid->dataSegment ) ;
					break ;
					
				case 10 :
					skip       ( num , cur , solid->dataSegment ) ;
					break ;
					
				case 11 :
					putIn_Out  ( num , cur , solid->dataSegment ) ;
					break ;
					
				case 12 :
					putIn_Out  ( num , cur , solid->dataSegment ) ;
					solid->lock = 1 ;
					break ;
					
				case 13 :
					while ( solid->lock == 1 && solid->end1 == 1 ) ;
					lock       ( num , cur , solid              ) ;
					break ;
					
				case 14 :
					release    ( num , cur , solid              ) ;
					break ;
					
				case 15 :
					funcSleep  ( num , cur                      ) ;
					break ;
			}
			
			funcPrint ( num->com , cur , solid ) ;  // ����Ĵ���
			
			i = ( cur->ip - 256 ) / 4 ; // �������±� i 
			
			if ( num->com == 0 )  //  ͣ�� 
				Flag = 0 ;
		}
		
		solid->end2 = 0 ; // ���ź�2��Ϊ�ѽ��� 
		
		//  �ͷ������ڴ�
		free ( cur->ax ) ;
		free ( cur ) ;
		free ( num ) ;
		
		return 0 ; 
	}
