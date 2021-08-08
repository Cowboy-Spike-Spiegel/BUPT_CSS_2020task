#include<stdio.h>
#include<stdlib.h>


/**   结 构 体 定 义   ********************************************************************************************************/


/* 定义寄存器结构体 struct -- MTC */
typedef struct mtc
{
	/* 程序计算器 */
		int ip ;

	/* 标志寄存器 */
		int flag ;

	/* 进制寄存器 */
		int ir ;

	/* 数据寄存器 1 , 2 , 3 , 4 和 地址寄存器 5 , 6 , 7 , 8 */
		int* ax ;  // 0号位舍弃,便于引用

}  MTC ;                                                  // TIP : MTC为本人名字拼音首字母 



/**  子 函 数 声 明   **********************************************************************************************************/


/* 1.11 空间生成 (malloc) 并 存入数据/初始化 以及释放(free) */

	/* txt   : 文本暂存空间 生成 并 存入数据 ------------ Line : 160 ~ 191 */
		int** generateTxtSpace ( FILE* fPtr , int line ) ;

	/* code  : 代码模拟空间 生成 并 存入数据 ------------ Line : 194 ~ 212 */
		int* generateCodeSpace ( int** txt , int line ) ;

	/* data  : 数据模拟空间 生成 并 初始化 -------------- Line : 215 ~ 229 */
		int* generateDataSpace (  ) ;

	/* struct: 寄存器结构体 生成 并 初始化 -------------- Line : 232 ~ 251 */
		MTC* generateStruct (  ) ;
	
	/* 释放申请空间 ------------------------------------- Line : 254 ~ 269 */
		void Free ( int** txt , int line , MTC* cur , int* code , int* data ) ;


/* 2.22 计算外包函数 ( 过程复杂 ) */

	/* 计算行数 ----------------------------------------- Line : 272 ~ 308 */
		int countLine ( FILE* fPtr ) ;

	/* 计算立即数 --------------------------------------- Line : 311 ~ 370 */ 
		int countImmediateValue ( int** txt , int c , int l ) ;


/* 3.33 指令集函数 */
 
	/* 停机指令 ------ 0 -------------------------------- Line : 373 ~ 386 */
		void stop       ( int com , int front , int behind , int im , MTC* cur , int* data ) ;

	/* 数据传送指令 -- 1 -------------------------------- Line : 389 ~ 413 */
		void transport  ( int com , int front , int behind , int im , MTC* cur , int* data ) ;

	/* 算术运算指令 -- 2 3 4 5 -------------------------- Line : 416 ~ 476 */
		void arithmetic ( int com , int front , int behind , int im , MTC* cur , int* data ) ;

	/* 逻辑运算指令 -- 6 7 8 ---------------------------- Line : 479 ~ 525 */
		void logic      ( int com , int front , int behind , int im , MTC* cur , int* data ) ;

	/* 比较指令 ------ 9 -------------------------------- Line : 538 ~ 570 */
		void compare    ( int com , int front , int behind , int im , MTC* cur , int* data ) ;

	/* 跳转指令 ------ 10 ------------------------------- Line : 573 ~ 600 */
		void skip       ( int com , int front , int behind , int im , MTC* cur , int* data ) ;

	/* 输入输出指令 -- 11 12 ---------------------------- Line : 603 ~ 629 */
		void putIn_Out  ( int com , int front , int behind , int im , MTC* cur , int* data ) ;


/* 4.44 窗口输出总集函数 */
 
	/* 输出寄存器状态 ----------------------------------- Line : 632 ~ 681 */
		void printState ( int com , MTC* cur , int* code , int* data ) ;
	


/**   主 函 数   ***************************************************************************************************************/


int main( int argc , char* argv[] )
{
	/* 打开文件 */ 
		FILE *fPtr =  fopen ( "dict.dic" , "r" ) ;
	
	/* 计算行数 ( 停机指令包括在内的文本行数 ) */
		int line = countLine ( fPtr ) ;
	
	/* 空间生成和初始化 -- 运用 malloc函数 */
		int** txt = generateTxtSpace ( fPtr , line ) ;  // 文本暂存空间 生成 并 存入数据
		int* code = generateCodeSpace ( txt , line ) ;  // 代码模拟空间 生成 并 存入数据
		int* data = generateDataSpace (  ) ;            // 数据模拟空间 生成 并 初始化
		MTC* cur = generateStruct (  ) ;                // 寄存器结构体 生成 并 初始化
	
	/* 行参数定义 */	
		int com      = 0 ;  // 01 ~ 08 位转十进制 
		int front          = 0 ;  // 09 ~ 12 位转十进制 
		int behind         = 0 ;  // 13 ~ 16 位转十进制 
		int im = 0 ;  // 17 ~ 32 位转十进制 
	
	/* 执行 */
		int i , j ; // 动态下标
		for ( i = 0 ; i < line ; ) {
			
			for ( com = 0 ,j = 0 ; j < 8 ; j++ )        // 计算 command          ( 命令十进制数 )
				com = 2*com + txt[i][j] ;
			
			for ( front = 0 ; j < 12 ; j++ )            // 计算 front    ( 前一个寄存器十进制数 )
				front = 2*front + txt[i][j] ;
			
			for ( behind = 0 ; j < 16 ; j++ )           // 计算 behind   ( 后一个寄存器十进制数 )
				behind = 2*behind + txt[i][j] ;
			
			im = countImmediateValue ( txt , i , j ) ;  // 计算 immediateValue ( 立即数十进制数 )
			
			/* 选择指令并操作 */
			switch ( com ) {
			 	
				case 0  : stop       ( com , front , behind , im , cur , data ) ; break ; // 停机     -- 0 
				case 1  : transport  ( com , front , behind , im , cur , data ) ; break ; // 数据传送 -- 1
				case 2  : arithmetic ( com , front , behind , im , cur , data ) ; break ; // 算术 '+' -- 2
				case 3  : arithmetic ( com , front , behind , im , cur , data ) ; break ; // 算术 '-' -- 3
				case 4  : arithmetic ( com , front , behind , im , cur , data ) ; break ; // 算术 '*' -- 4
				case 5  : arithmetic ( com , front , behind , im , cur , data ) ; break ; // 算术 '/' -- 5
				case 6  : logic      ( com , front , behind , im , cur , data ) ; break ; // 逻辑 '&&'-- 6 
				case 7  : logic      ( com , front , behind , im , cur , data ) ; break ; // 逻辑 '||'-- 7
				case 8  : logic      ( com , front , behind , im , cur , data ) ; break ; // 逻辑 '!' -- 8
				case 9  : compare    ( com , front , behind , im , cur , data ) ; break ; // 比较     -- 9
				case 10 : skip       ( com , front , behind , im , cur , data ) ; break ; // 跳转     --10
				case 11 : putIn_Out  ( com , front , behind , im , cur , data ) ; break ; // 输入 in  --11
				case 12 : putIn_Out  ( com , front , behind , im , cur , data ) ; break ; // 输出 out --12
			}
			
			i = cur->ip/4 ; // 修正行下标 i 
			
			printState ( com , cur , code , data ) ; // 输出寄存器状态 
		}
		
		Free ( txt , line , cur , code , data ) ; // 释放申请空间 
	
	
	return 0 ;
}



/**   子 函 数   ****************************************************************************************************************/


/* txt : 文本暂存空间 生成 并 存入数据 */
	int** generateTxtSpace ( FILE* fPtr , int line )
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
	int* generateCodeSpace ( int** txt , int line )
	{
		
		/* 申请代码空间 */
			int* code = ( int* ) malloc ( sizeof ( int ) * 128 ) ;
			
			int i , j ;
			for ( i = 0 ; i < 128 ; i++ ) // 初始化
				code[i] = 0 ;
		
		/* 存入 code 数据 */
			for ( i = 0 ; i < line ; i++ )
				for ( j = 0 ; j < 32 ; j++ )
					code[ i ] = 2*code[i] + txt[i][j] ;
		
			
		return code ;
	}


/* data : 数据模拟空间 生成 并 初始化 */
	int* generateDataSpace (  )
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
	MTC* generateStruct (  )
	{
		
		/* 申请结构体空间 */
		MTC* cur = ( MTC* ) malloc ( sizeof ( MTC ) ) ;
		
		/* 初始化 */
			cur->ip = 0 ;
			cur->flag = 0 ;
			cur->ir = 0 ;
			
			cur->ax = ( int* ) malloc ( sizeof ( int ) * 9 ) ;
			int  i ;
			for ( i = 0 ; i < 9 ; i++ )
				cur->ax[i] = 0 ;
		
		
		return cur ;
	}


/* 释放申请空间 */
	void Free ( int** txt , int line , MTC* cur , int* code , int* data )
	{
		
		int i ;
		for ( i = 0 ; i < line ; i++ )
			free ( txt[i] ) ;
		free ( txt ) ;
		
		free ( cur ) ;
		free ( code ) ;
		free ( data ) ;
		
		
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
	void stop ( int com , int front , int behind , int im , MTC* cur , int* data )
	{
		
		/* 计算 ir */ 
			cur->ir = 256*com + 16*front + behind ;
		
		
		/* 计数器 */
			cur->ip = cur->ip + 4 ;
		
		
		return ;
	}
	
	
/* 数据传送指令 -- 1 */
	void transport ( int com , int front , int behind , int im , MTC* cur , int* data )
	{
		
		/* 计算 ir */ 
			cur->ir = 256*com + 16*front + behind ;
		
		
		/* 三种传送指令 */ 
			if ( behind == 0 )
				cur->ax[front] = im ;
			
			else if ( behind >= 1 && behind <= 4 )
				data[ ( cur->ax[front] - 16384 ) / 2 ] = cur->ax[behind] ;
			
			else if ( behind >= 5 && behind <= 8 )
				cur->ax[front] = data[ ( cur->ax[behind] - 16384 ) / 2 ] ;
		
			
		/* 计数器 */
			cur->ip = cur->ip + 4 ;
		
		
		return ;
	}


/* 算术运算指令 -- 2 3 4 5 */
	void arithmetic ( int com , int front , int behind , int im , MTC* cur , int* data )
	{
		
		/* 计算 ir */ 
			cur->ir = 256*com + 16*front + behind ;
		
		
		/* 四种运算符 */
			
			// + 运算 
			if ( com == 2 ) {
				
				if ( behind == 0 )
					cur->ax[front] = cur->ax[front] + im ;
				
				else
					cur->ax[front] = cur->ax[front] + data[ ( cur->ax[behind] - 16384 ) / 2 ] ;
					
			}
			
			// - 运算 
			if ( com == 3 ) {
				
				if ( behind == 0 )
					cur->ax[front] = cur->ax[front] - im ;
				
				else
					cur->ax[front] = cur->ax[front] - data[ ( cur->ax[behind] - 16384 ) / 2 ] ;
					
			}
			
			// * 运算 
			if ( com == 4 ) {
				
				if ( behind == 0 )
					cur->ax[front] = cur->ax[front] * im ;
				
				else
					cur->ax[front] = cur->ax[front] * data[ ( cur->ax[behind] - 16384 ) / 2 ] ;
					
			}
			
			// - 运算 
			if ( com == 5 ) {
				
				if ( behind == 0 )
					cur->ax[front] = cur->ax[front] / im ;
				
				else
					cur->ax[front] = cur->ax[front] / data[ ( cur->ax[behind] - 16384 ) / 2 ] ;
					
			}
		
		
		/* 计数器 */
			cur->ip = cur->ip + 4 ;
		
		
		return ;
	}


/* 逻辑运算指令 -- 6 7 8 */
	void logic ( int com , int front , int behind , int im , MTC* cur , int* data )
	{
		
		/* 计算 ir */ 
			cur->ir = 256*com + 16*front + behind ;
		
		
		/* 三种运算符 */
		
			// && 运算 
			if ( com == 6 ) {
				
				if ( behind == 0 )
					cur->ax[front] = cur->ax[front] && im ;
				
				else
					cur->ax[front] = cur->ax[front] && data[ ( cur->ax[behind] - 16384 ) / 2 ] ;
			}
			
			// || 运算 
			if ( com == 7 ) {
				
				if ( behind == 0 )
					cur->ax[front] = cur->ax[front] || im ;
				
				else
					cur->ax[front] = cur->ax[front] || data[ ( cur->ax[behind] - 16384 ) / 2 ] ;
			}
			
			// ! 运算 
			if ( com == 8 ) {
				
				if ( behind == 0 )
					cur->ax[front] = ! cur->ax[front] ;
				
				else
					data[ ( cur->ax[behind] - 16384 ) / 2 ] = ! data[ ( cur->ax[behind] - 16384 ) / 2 ] ;
			}
		
		
		/* 计数器 */
			cur->ip = cur->ip + 4 ;
		
		
		return ;
	}


/* 比较指令 -- 9 */
	void compare ( int com , int front , int behind , int im , MTC* cur , int* data )
	{
		
		/* 计算 ir */ 
			cur->ir = 256*com + 16*front + behind ;
		
		
		/* 寄存器 1 2 3 4 与 立即数作比较 */
			if ( behind == 0 ) {
				
				if ( cur->ax[front] > im )
					cur->flag = 1 ;
				
				else if ( cur->ax[front] == im )
					cur->flag = 0 ;
				
				else if ( cur->ax[front] < im )
					cur->flag = -1 ;
					
			}
		
		/* 寄存器 1 2 3 4 和 寄存器 5 6 7 8 作比较 */
			else {
				
				if ( cur->ax[front] > data[ ( cur->ax[behind] - 16384 ) / 2 ] )
					cur->flag = 1 ;
				
				else if ( cur->ax[front] == data[ ( cur->ax[behind] - 16384 ) / 2 ] )
					cur->flag = 0 ;
				
				else if ( cur->ax[front] < data[ ( cur->ax[behind] - 16384 ) / 2 ] )
					cur->flag = -1 ;
					
			}
		
		
		/* 计数器 */
			cur->ip = cur->ip + 4 ;
		
		
		return ;
	}


/* 跳转指令 -- 10 */
	void skip ( int com , int front , int behind , int im , MTC* cur , int* data )
	{
		
		/* 计算 ir */ 
			cur->ir = 256*com + 16*front + behind ;
		
		
		/* 三种跳转指令 */
			if ( behind == 0 )
				cur->ip = cur->ip + im ;
			
			else if ( behind == 1 && cur->flag == 0 )
				cur->ip = cur->ip + im ;
			
			else if ( behind == 2 && cur->flag == 1 )
				cur->ip = cur->ip + im ;
			
			else if ( behind == 3 && cur->flag == -1 )
				cur->ip = cur->ip + im ;
		
		/* 不跳转 */
			else
				cur->ip = cur->ip + 4 ;
		
		
		return ;
	}


/* 输入输出指令 -- 11 12 */
	void putIn_Out ( int com , int front , int behind , int im , MTC* cur , int* data )
	{
		
		/* 计算 ir */ 
			cur->ir = 256*com + 16*front + behind ;
		
		
		/* 输入 */
			if ( com == 11 ) {
				
				printf ( "in:\n" ) ;
				scanf ( "%d" , & cur->ax[front] ) ;
			}
		
		/* 输出 */
			else if ( com == 12 ) {
				
				printf ( "out: %d\n" , cur->ax[front] ) ;
			}
		
		/* 计数器 */
			cur->ip = cur->ip + 4 ;
		
		
		return ;
	}


/* 输出寄存器状态 */
	void printState ( int com , MTC* cur , int* code , int* data )
	{
		
		/* 打印寄存器状态 */ 
			printf ( "ip = %d\n" , cur->ip ) ;
			printf ( "flag = %d\n" , cur->flag ) ;
			printf ( "ir = %d\n" , cur->ir ) ;
			printf ( "ax1 = %d ax2 = %d ax3 = %d ax4 = %d\n" , cur->ax[1] , cur->ax[2] , cur->ax[3] , cur->ax[4] ) ;
			printf ( "ax5 = %d ax6 = %d ax7 = %d ax8 = %d\n" , cur->ax[5] , cur->ax[6] , cur->ax[7] , cur->ax[8] ) ;
		
		
		/* 停机指令 , 打印 code 和 data */
			if ( com == 0 ) {
				
				int i ; // 动态下标
				
			/* 打印 code */ 
				printf ( "\ncodeSegment :\n" ) ;
				 
				for ( i = 0 ; i < 128 ; i++ ) {
					
					printf ( "%d" , code[i] ) ;
					
					if ( ( i+1 ) % 8 == 0 )
						printf ( "\n" ) ;
					else
						printf ( " " ) ;
					
				}
			
			/* 打印 code */
				printf ( "\ndataSegment :\n" ) ;
				
				for ( i = 0 ; i < 256 ; i++ ) {
					
					printf ( "%d" , data[i] ) ;
					
					if ( ( i+1 ) % 16 == 0 )
						printf ( "\n" ) ;
					else
						printf ( " " ) ;
							
				}
			
			}
		
		
		return ;
	}
