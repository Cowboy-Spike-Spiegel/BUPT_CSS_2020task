#include"CPU.h"

/* 线程 1 */
	unsigned __stdcall Fun1Proc ( void* pArguments )
	{
		
		//  固态储存寄存器赋名 
		segment* solid = (segment*) pArguments ;
		
		//  申请寄存器空间
		MTC* cur = generateStruct ( 1 ) ;
		
		//  申请行参数寄存器空间 
		number* num = ( number* ) malloc ( sizeof ( number ) ) ;
		
		solid->end1 = 1 ; // 改为有信号 
		
		/* 执行 */
		int i , j ; // 动态下标
		int Flag ;
		for ( i = 0 , Flag = 1 ; Flag ; ) {
			
			// 计算 command          ( 命令十进制数 )
			for ( num->com = 0 ,j = 0 ; j < 8 ; j++ )
				num->com = 2*num->com + solid->txt1[i][j] ;
			
			// 计算 front    ( 前一个寄存器十进制数 )
			for ( num->front = 0 ; j < 12 ; j++ )
				num->front = 2*num->front + solid->txt1[i][j] ;
				
			// 计算 behind   ( 后一个寄存器十进制数 )
			for ( num->behind = 0 ; j < 16 ; j++ )
				num->behind = 2*num->behind + solid->txt1[i][j] ;
			
			// 计算 immediateValue ( 立即数十进制数 )
			num->im = countImmediateValue ( solid->txt1 , i , j ) ;
			
			/* 选择指令并操作 */
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
			
			funcPrint ( num->com , cur , solid ) ;  // 输出寄存器
			
			i = cur->ip/4 ; // 修正行下标 i 
			
			if ( num->com == 0 )  //  停机 
				Flag = 0 ;
		}
		
		solid->end1 = 0 ; // 将信号1改为已结束 
		
		//  释放申请内存
		free ( cur->ax ) ;
		free ( cur ) ;
		free ( num ) ;
		
		return 0 ;
	}


/* 线程 2 */
	unsigned __stdcall Fun2Proc ( void* pArguments )
	{
		
		//  固态储存寄存器赋名 
		segment* solid = (segment*) pArguments ;
		
		//  申请寄存器空间
		MTC* cur = generateStruct ( 2 ) ;
		
		//  申请行参数寄存器空间 
		number* num = ( number* ) malloc ( sizeof ( number ) ) ; 
		
		solid->end2 = 1 ; // 改为有信号 
		
		/* 执行 */
		int i , j ; // 动态下标
		int Flag ;
		for ( i = 0 , Flag = 1 ; Flag ; ) {
			
			// 计算 command          ( 命令十进制数 )
			for ( num->com = 0 ,j = 0 ; j < 8 ; j++ )
				num->com = 2*num->com + solid->txt2[i][j] ;
			
			// 计算 front    ( 前一个寄存器十进制数 )
			for ( num->front = 0 ; j < 12 ; j++ )
				num->front = 2*num->front + solid->txt2[i][j] ;
			
			// 计算 behind   ( 后一个寄存器十进制数 )
			for ( num->behind = 0 ; j < 16 ; j++ )
				num->behind = 2*num->behind + solid->txt2[i][j] ;
			
			// 计算 immediateValue ( 立即数十进制数 )
			num->im = countImmediateValue ( solid->txt2 , i , j ) ;
			
			/* 选择指令并操作 */
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
			
			funcPrint ( num->com , cur , solid ) ;  // 输出寄存器
			
			i = ( cur->ip - 256 ) / 4 ; // 修正行下标 i 
			
			if ( num->com == 0 )  //  停机 
				Flag = 0 ;
		}
		
		solid->end2 = 0 ; // 将信号2改为已结束 
		
		//  释放申请内存
		free ( cur->ax ) ;
		free ( cur ) ;
		free ( num ) ;
		
		return 0 ; 
	}
