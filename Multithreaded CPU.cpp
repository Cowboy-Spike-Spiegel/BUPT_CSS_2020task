#include<windows.h>
#include<process.h>
#include<stdio.h>   
#include<stdlib.h>
#include<time.h> 


/* 0.00 寄存器定义 ---------------------------------------------------- */

	/* 定义寄存器结构体 struct -- segment */
		typedef struct Segment
		{
			HANDLE hMutex ;     //  进程互斥对象 
			int lock ;          //  同步锁 
			int end1 ;          //  线程 1 结束信号 
			int end2 ;          //  线程 2 结束信号 
			int** txt1 ;        //  代码 1
			int line1 ;         //  代码 1 行数 
			int** txt2 ;        //  代码 2 
			int line2 ;         //  代码 2 行数 
			int* codeSegment ;  //  代码寄存器 
			int* dataSegment ;  //  数据寄存器 
		}  segment ;
	
 
	/* 定义寄存器结构体 struct -- MTC */
		typedef struct mtc
		{
			int id ;    // 核心 ID 
			int ip ;    // 程序计数器 
			int flag ;  // 标志寄存器 
			int ir ;    // 命令寄存器 
			int* ax ;   // 寄存器, 0号位舍弃,便于引用
		}  MTC ;
 

	/* 定义行参数数据结构体 */
		typedef struct Number
		{
			int com ;    // 01 ~ 08 位转十进制 
			int front ;  // 09 ~ 12 位转十进制 
			int behind ; // 13 ~ 16 位转十进制 
			int im ;     // 17 ~ 32 位转十进制 
		}  number ; 

	
/* 1.11 空间生成 (malloc) 并 存入数据/初始化 以及释放(free) ----------- */

	/* txt   : 文本暂存空间 生成 并 存入数据 */
	int** generateTxt ( FILE* fPtr , int line ) ;

	/* code  : 代码模拟空间 生成 并 存入数据 */
	int* generateCode ( int** txt1 , int** txt2 , int line1 , int line2 ) ;

	/* data  : 数据模拟空间 生成 并 初始化 */
	int* generateData (  ) ;

	/* struct: 寄存器结构体 生成 并 初始化 */
	MTC* generateStruct ( int id ) ;
	
	/* main : 释放申请空间 */
	void mainFree ( segment* solid ) ;


/* 2.22 计算外包函数 ( 过程复杂 ) ------------------------------------- */

	/* 计算行数 */
	int countLine ( FILE* fPtr ) ;

	/* 计算立即数 */ 
	int countImmediateValue ( int** txt , int c , int l ) ;


/* 3.33 线程函数 ------------------------------------------------------ */ 

	unsigned __stdcall Fun1Proc ( void* pArguments ) ;
	unsigned __stdcall Fun2Proc ( void* pArguments ) ;


/* 4.44 指令集函数 ---------------------------------------------------- */
 
	/* 停机指令 ------ 0 */
	void stop       ( number* num , MTC* cur , int* data ) ;

	/* 数据传送指令 -- 1 */
	void transport  ( number* num , MTC* cur , int* data ) ;

	/* 算术运算指令 -- 2 3 4 5 */
	void arithmetic ( number* num , MTC* cur , int* data ) ;

	/* 逻辑运算指令 -- 6 7 8 */
	void logic      ( number* num , MTC* cur , int* data ) ;

	/* 比较指令 ------ 9 */
	void compare    ( number* num , MTC* cur , int* data ) ;

	/* 跳转指令 ------ 10 */
	void skip       ( number* num , MTC* cur , int* data ) ;

	/* 输入输出指令 -- 11 12 */
	void putIn_Out  ( number* num , MTC* cur , int* data ) ;
	
	/* 上锁 ---------- 13 */
	void lock       ( number* num , MTC* cur , segment* solid ) ;
	
	/* 释放 ---------- 14 */
	void release    ( number* num , MTC* cur , segment* solid ) ;
	 
	/* 函数睡眠 ------ 15 */ 
	void funcSleep  ( number* num , MTC* cur ) ;
	

/* 5.55 窗口输出总集函数 ---------------------------------------------- */
 
	/* func : 输出寄存器状态 */
	void funcPrint ( int com , MTC* cur , segment* solid ) ;
	
	/* main : 输出 dataSegment 和 codeSegment */
	void mainPrint ( segment* solid ) ;  
	

/* -------------------------------------------------------------------- */

/* 主函数 */

int main ( int argc , char *argv[ ] )
{
	
	/* 处理 solid : 包括 codeSegment , dataSegment */
	segment* solid = ( segment* ) malloc ( sizeof ( segment ) ) ;
	srand ( (unsigned) time ( NULL ) ) ;
	solid->lock = rand() % 2 + 1 ; // lock 随机初始化  
	
	// 创建互斥句柄	
	solid->hMutex = CreateMutex ( NULL , FALSE , NULL ) ;
	
	// 处理 dict1.dic	
	FILE *fPtr1 =  fopen ( "dict1.dic" , "r" ) ;
	solid->line1 = countLine ( fPtr1 ) ;
	solid->txt1 = generateTxt ( fPtr1 , solid->line1 ) ;
	fclose ( fPtr1 ) ;
	
	// 处理 dict2.dic 
	FILE *fPtr2 =  fopen ( "dict2.dic" , "r" ) ;
	solid->line2 = countLine ( fPtr2 ) ;
	solid->txt2 = generateTxt ( fPtr2 , solid->line2 ) ;
	fclose ( fPtr2 ) ;
	
	// codeSegment初始化 
	solid->codeSegment
	= generateCode ( solid->txt1 , solid->txt2 , solid->line1 , solid->line2 ) ;
	
	// dataSegment初始化 
	solid->dataSegment = generateData (  ) ;
	solid->dataSegment[0] = 100 ;  //  初始化票数为 100 
		
	/* 创建线程 1 , 2 */
	HANDLE hThread1
	= (HANDLE) _beginthreadex ( NULL , 0 , Fun1Proc , solid , 0 , NULL ) ;
	HANDLE hThread2
	= (HANDLE) _beginthreadex ( NULL , 0 , Fun2Proc , solid , 0 , NULL ) ;
	
	/* 关闭线程 1 , 2  , 并释放内存 */
	WaitForSingleObject ( hThread1 , INFINITE ) ;
	CloseHandle ( hThread1 ) ;
	WaitForSingleObject ( hThread2 , INFINITE ) ;	
	CloseHandle ( hThread2 ) ;
	
	/* 输出 code 和 data 和释放固态储存寄存器 */
	mainPrint ( solid ) ;
	mainFree ( solid ) ;
	
	
	return 0 ;
}


/* -------------------------------------------------------------------- */


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


/* func : 输出寄存器状态 */
	void funcPrint ( int com , MTC* cur , segment* solid )
	{
		
		/* 打印寄存器状态 */ 
		printf ( "id = %d\nip = %d\nflag = %d\nir = %d\n"
			"ax1 = %d ax2 = %d ax3 = %d ax4 = %d\n"
			"ax5 = %d ax6 = %d ax7 = %d ax8 = %d\n\n"
			, cur->id , cur->ip , cur->flag , cur->ir 
			, cur->ax[1] , cur->ax[2] , cur->ax[3] , cur->ax[4] 
			, cur->ax[5] , cur->ax[6] , cur->ax[7] , cur->ax[8] ) ;
			
		return ;
	}


/* main : 输出 dataSegment 和 codeSegment */
	void mainPrint ( segment* solid ) 
	{	
		
		int i ;
		/* 打印 code */ 
			printf ( "codeSegment :\n" ) ;
			 
			for ( i = 0 ; i < 128 ; i++ ) {
				
				printf ( "%d" , solid->codeSegment[i] ) ;
				
				if ( ( i+1 ) % 8 == 0 )
					printf ( "\n" ) ;
				else
					printf ( " " ) ;
			}
		
		/* 打印 data */
			printf ( "\ndataSegment :\n" ) ;
			
			for ( i = 0 ; i < 256 ; i++ ) {\
				
				printf ( "%d" , solid->dataSegment[i] ) ;
				
				if ( ( i+1 ) % 16 == 0 )
					printf ( "\n" ) ;
				else
					printf ( " " ) ;		
			}
		
		
		return ;
	}
