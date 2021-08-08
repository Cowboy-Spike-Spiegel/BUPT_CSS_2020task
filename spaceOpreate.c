#include"CPU.h"

/* txt : 文本暂存空间 生成 并 存入数据 */
	int** generateTxt ( FILE* fPtr , int line )
	{
		
		/* 申请 txt空间 : 二维和一维 */
			int** txt = ( int** ) malloc ( sizeof ( int* ) * ( line ) ) ;
			
			int i , j ;
			char ch ;
			for ( i = 0 ; i < line ; i++ )
				txt[i] = ( int* ) malloc ( sizeof ( int ) * 32 ) ;
		
		/* 存入 txt 数据 */
			for ( i = 0 ; i < line ; i++ ) {
				
				for ( j = 0 ; j < 32 ; j++ ) {
					ch = fgetc ( fPtr ) ;
					txt[i][j] = ch - 48 ;
					
				}
		
				/* 判断结束条件 */
				if ( i == line-1 )
					break ;
				else
					while ( ch != 10 )
						ch = fgetc ( fPtr ) ; 
			}
		
		
		return txt ;
	}


/* code : 代码模拟空间 生成 并 存入数据 */
	int* generateCode ( int** txt1 , int** txt2 , int line1 , int line2 )
	{
		
		/* 申请代码空间 */
			int* code = ( int* ) malloc ( sizeof ( int ) * 128 ) ;
			
			int i , j ;
			for ( i = 0 ; i < 128 ; i++ ) // 初始化
				code[i] = 0 ;
		
		/* 存入 code 数据 */
			for ( i = 0 ; i < line1 ; i++ )
				for ( j = 0 ; j < 32 ; j++ )
					code[i] = 2*code[i] + txt1[i][j] ;
			
			for ( i = 64 ; i < line2 + 64 ; i++ )
				for ( j = 0 ; j < 32 ; j++ )
					code[i] = 2*code[i] + txt2[i-64][j] ;
		
			
		return code ;
	}


/* data : 数据模拟空间 生成 并 初始化 */
	int* generateData (  )
	{
		
		/* 申请数据空间 */
			int* data = ( int* ) malloc ( sizeof ( int ) * 256 ) ;
		
		/* 初始化 */
			int i ;
			for ( i = 0 ; i < 256 ; i++ )
				data[i] = 0 ;
			
		
		return data ;
	}


/* struct : 寄存器结构体 生成 并 初始化 */
	MTC* generateStruct ( int id )
	{
		
		/* 申请结构体空间 */
		MTC* cur = ( MTC* ) malloc ( sizeof ( MTC ) ) ;
		
		/* 初始化 */
			cur->id = id ;    //  id 初始化 
			
			if ( id == 1 )    //  ip 初始化 
				cur->ip = 0 ;
			else
				cur->ip = 256 ;
				
			cur->flag = 0 ;  // flag 初始化 
			cur->ir = 0 ;     //  ir 初始化 
			
			cur->ax = ( int* ) malloc ( sizeof ( int ) * 9 ) ;  // ax 初始化 
			int  i ;
			for ( i = 0 ; i < 9 ; i++ )
				cur->ax[i] = 0 ;
		
		
		return cur ;
	}


/* main : 释放申请空间 */
	void mainFree ( segment* solid )
	{
		
		int i ;
		for ( i = 0 ; i < solid->line1 ; i++ )  //  释放 txt1 
			free ( solid->txt1[i] ) ;
		free ( solid->txt1 ) ;
		for ( i = 0 ; i < solid->line2 ; i++ )  //  释放 txt2 
			free ( solid->txt2[i] ) ;
		free ( solid->txt2 ) ;
		
		free ( solid->codeSegment ) ;  //  释放代码寄存器 
		free ( solid->dataSegment ) ;  //  释放数据寄存器 
		
		free ( solid ) ;  //  释放固态储存寄存器 
		
		
		return ;
	}
