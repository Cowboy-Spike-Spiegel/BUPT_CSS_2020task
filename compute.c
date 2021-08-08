#include"CPU.h"

/* 计算行数 */ 
	int countLine ( FILE* fPtr )
	{
		
		int line = 0 ; // 行数初始化 0
		
		char ch ;
		int i , sum ;  // 动态下标 和 countLine数组之和
			
		/* 循环 line 计数 */
			int Flag = 1 ;
			while ( Flag ) {
			
				for ( sum = 0 , i = 0 ; i < 8 ; i++ ) {
					
					ch = fgetc ( fPtr ) ;  // 存入字符型
					sum = sum + ch - 48 ; // 计算 8位数之和
					
				}
				
				line++ ; // 行数计数
				
				if ( sum == 0 )  // 前 8 位都为 0 , 和为 0
					Flag = 0 ;
				
				/* 吃掉 \n 包括 在内的所有字符 */
				ch = fgetc ( fPtr ) ;
				while ( ch != 10 && ch != 0 )
					ch = fgetc ( fPtr ) ;
				
			}
		
			rewind ( fPtr ) ;  // 重置文件指针 
		
		
		return line ;
	}


/* 计算立即数 */ 
	int countImmediateValue ( int** txt , int c , int l )
	{
		
		/* 参数定义 */
			int im = 0 ;   // 立即数初始化
			int Flag = 1 ; //立即数正负标记
			int i ;        // 动态下标
		
		/* 立即数为正 */
			if ( txt[c][l] == 0 ) 
				for ( i = 1 ; i < 16 ; i++ )
					im = 2*im + txt[c][l+i] ;
			
			else {
				
				Flag = 0 ; // 标记为负数
				
				/* copy原数组而不改变原数组 */
					int* copy = ( int* ) malloc ( sizeof ( int ) * 15 ) ;
					for ( i = 1 ; i < 16 ; i++ )
						copy[i-1] = txt[c][l+i] ;
				
				/* 取反 */
					for ( i = 0 ; i < 15 ; i++ ) {
						if ( copy[i] == 0 )
							copy[i] = 1 ;
						
						else if ( copy[i] == 1 )
							copy[i] = 0 ;	
					}
				
				/* +1 */
					copy[14]++ ;
					for ( i = 14 ; i > 0 ; i-- )  // 进位 
						
						if ( copy[i] > 1 ) {
							copy[i-1] = copy[i-1] + 1 ;
							copy[i] = copy[i] - 2 ;
						}
						
				/* 计算立绝对值 */
					for ( i = 0 ; i < 15 ; i++ )
						im = 2*im + copy[i] ;
			
				free ( copy ) ;
				
			}
			
		/* 若标记为负则取负 */ 
			if ( Flag == 0 )
				im = -im ;
		
		
		return im ; 
	}
