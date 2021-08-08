#include<stdio.h>
#include<stdlib.h>


/* 全局变量定义并初始化 *************************** */
	
	/* 程序计算器 */
	int ip = 0 ;
	
	/* 标志寄存器 */
	int flag = 0 ;
	
	/* 进制寄存器 */
	int ir = 0 ;
	
	/* 数据寄存器 1 , 2 , 3 , 4 和地址寄存器 5 , 6 , 7 , 8 */
	int ax[9] = {0} ;  // 0号位舍弃,便于引用
	
	/* 代码储存空间--模拟 */
	int* codeSegment = NULL ;
	
	/* 数据储存空间--模拟 */
	int* dataSegment = NULL ;
	

/* 暂存变量定义并初始化 *************************** */
	
	
	/* 读取的字符变量 */ 
	char ch = 0 ;
	
	/* 指令种类--对应操作码 */
	int command[8] = {0} ;
	
	/* 前一个寄存器--对应操作对象 */
	int registerFront[4] = {0} ;
	int front = 0 ; // 十进制转换 
	
	/* 后一个寄存器--对应操作对象 */
	int registerBehind[4] = {0} ;
	int behind = 0 ; // 十进制转换 
	
	/* 立即数:只需当作十进制存储 */
	int immediateNumber[16] = {0} ;
	int immediateValue = 0 ;



/* 函数声明 ************************************** */
	
	
	/* 申请代码空间 */
	void applyCodeSpace (  ) ;
	
	/* 申请数据空间 */
	void applyDataSpace (  ) ;
	
	/* 生成代码储存空间 */
	void generateCode ( FILE* fPtr ) ; 
	
	/* 读取指令类型 */
	int scanCommand ( FILE* fPtr ) ;
	
	/* 读取前一个寄存器状态 */
	int scanFront ( FILE* fPtr ) ;
	
	/* 读取后一个寄存器状态 */
	int scanBehind ( FILE* fPtr ) ;
	
	/* 读取立即数 */ 
	int scanImmediateValue ( FILE* fPtr ) ;
	
	/* 读取函数 */
	void scan ( FILE* fPtr , int com ) ;
	
	/* 停机指令 -- 0 */
	void stop ( FILE* fPtr , int com ) ;
	
	/* 数据传送指令 -- 1 */
	void transport ( FILE* fPtr , int com ) ;
	
	/* 算术运算指令 -- 2 3 4 5 */
	void arithmetic ( FILE* fPtr , int com ) ;
	
	/* 逻辑运算指令 -- 6 7 8 */
	void logic ( FILE* fPtr , int com ) ;
	
	/* 比较指令 -- 9 */
	void compare ( FILE* fPtr , int com ) ;
	
	/* 跳转指令 -- 10 */
	void skip ( FILE* fPtr , int com ) ;
	
	/* 输入输出指令 -- 11 12 */
	void putIn_Out ( FILE* fPtr , int com ) ;
	
	/* 暂存数据重置 */
	void reset (  ) ;
	
	/* 输出寄存器状态 */
	void printState ( int Flag ) ;


/**********************************************************************/


/* 主操作函数 */

int main( int argc, char *argv[] )
{
	
	/* 打开文件 */ 
	FILE *fPtr =  fopen ( "dict.dic" , "r" ) ;
	
	/* 申请代码和数据空间 */
	applyCodeSpace (  ) ;
	applyDataSpace (  ) ;
	
	/* 生成 Code */
	generateCode ( fPtr ) ;
	
	/* 循环控制器 -- different from 'flag' */
	int Flag = 1 ;
	
	/* 循环操作 */
	while ( Flag )	{
		
		/* 读取指令类型 */
		int com = scanCommand ( fPtr ) ;
		
		/* 匹配指令并操作 */
		switch ( com ) {
			 
			case 0  : stop ( fPtr , com ) ; Flag = 0 ; break ;
			case 1  : transport ( fPtr , com )       ; break ;
			case 2  : arithmetic ( fPtr , com )      ; break ;
			case 3  : arithmetic ( fPtr , com )      ; break ;
			case 4  : arithmetic ( fPtr , com )      ; break ;
			case 5  : arithmetic ( fPtr , com )      ; break ;
			case 6  : logic ( fPtr , com )           ; break ; 
			case 7  : logic ( fPtr , com )           ; break ;
			case 8  : logic ( fPtr , com )           ; break ;
			case 9  : compare ( fPtr , com )         ; break ;
			case 10 : skip ( fPtr , com )            ; break ;
			case 11 : putIn_Out ( fPtr , com )       ; break ;
			case 12 : putIn_Out ( fPtr , com )       ; break ;

		}
			
		/* 输出寄存器状态 */
		printState ( Flag ) ;
			
	}
	
	free ( codeSegment ) ;
	free ( dataSegment ) ;
	
	/* 关闭文件 */
	fclose ( fPtr ) ;
	
	
	return 0 ;
}


/***********************************************************************/


/* 子函数 */


/* 申请代码空间 */
	void applyCodeSpace (  )
	{
		
		codeSegment = ( int* ) malloc ( sizeof ( int ) * 128 ) ;
		
		int i ;
		for ( i = 0 ; i < 128 ; i++ ) // 初始化 
			codeSegment[i] = 0 ;
		
		
		return ;
	}


/* 申请数据空间 */
	void applyDataSpace (  )
	{
		
		dataSegment = ( int* ) malloc ( sizeof ( int ) * 256 ) ;
		
		int i ;
		for ( i = 0 ; i < 256 ; i++ ) // 初始化 
			dataSegment[i] = 0 ;
		
		
		return ;
	}

/* 储存代码 code */
	void generateCode ( FILE* fPtr )
	{
		int i , j , sum ;
		char ch ;
		for ( i = 0 ; ; i++ )
		{
			for ( j = 0 ; j < 32 ; j++ ) {
				ch = fgetc ( fPtr ) ;
				codeSegment[i] = 2*codeSegment[i] + ch - 48 ;
			}
			
			if ( codeSegment[i] == 0 )
				break ;
			
			while ( ch != 10 )
				ch = fgetc ( fPtr ) ;
		}
		
		rewind ( fPtr ) ;
		
		return ;
	}


/* 读取指令类型 */
	int scanCommand ( FILE* fPtr )
	{
		
		/* 动态下标 */
		int i ;
		
		/* 命令 */ 
		int com ; 
		 
		for ( com = 0 , i = 0 ; i < 8 ; i++ ) {
			
			ch = fgetc ( fPtr ) ;
			command[i] = ch - 48 ;
			
			if ( i == 4 && command[i] == 1 )
				com = com + 8 ;
			
			else if ( i == 5 && command[i] == 1 )
				com = com + 4 ;
			
			else if ( i == 6 && command[i] == 1 )
				com = com + 2 ;
			
			else if ( i == 7 && command[i] == 1 )
				com = com + 1 ;
			
		}
		
		
		return com ;
	}
	
	
/* 读取前一个寄存器状态 */
	int scanFront ( FILE* fPtr )
	{
		
		/* 动态下标 */
		int i ;
		
		/* 前一个寄存器 */ 
		int fr ;
		
		for ( fr = 0 , i = 0 ; i < 4 ; i++ ) {
			
			ch = fgetc ( fPtr ) ;
			registerFront[i] = ch - 48 ;
			
			/* 计算寄存器十进制号码 */
			if ( i == 0 && registerFront[i] == 1 )
				fr = fr + 8 ;
			
			else if ( i == 1 && registerFront[i] == 1 )
				fr = fr + 4 ;
			
			else if ( i == 2 && registerFront[i] == 1 )
				fr = fr + 2 ;
			
			else if ( i == 3 && registerFront[i] == 1 )
				fr = fr + 1 ;
			
		}
		
		
		return fr ;
	}


/* 读取后一个寄存器状态 */
	int scanBehind ( FILE* fPtr )
	{
		
		/* 动态下标 */
		int i ;
		
		/* 后一个寄存器 */ 
		int be ;
		 
		for ( be = 0 , i = 0 ; i < 4 ; i++ ) {
			
			ch = fgetc ( fPtr ) ;
			registerBehind[i] = ch - 48 ;
			
			/* 计算寄存器十进制号码 */ 
			if ( i == 0 && registerBehind[i] == 1 )
				be = be + 8 ;
			
			else if ( i == 1 && registerBehind[i] == 1 )
				be = be + 4 ;
			
			else if ( i == 2 && registerBehind[i] == 1 )
				be = be + 2 ;
			
			else if ( i == 3 && registerBehind[i] == 1 )
				be = be + 1 ;
			
		}
		
		
		return be ;
	}


/* 读取立即数 */ 
	int scanImmediateValue ( FILE* fPtr )
	{
		int im = 0 ; // 立即数初始化
		int Flag = 1 ; //立即数正负标记
		int i ; // 动态下标
		
		/* 读取立即数 */ 
		for ( i = 0 ; i < 16 ; i++ ) {
			
			ch = fgetc ( fPtr ) ;
			immediateNumber[i] = ch - 48 ;
			
		}
		
		/* 化简立即数绝对值 */
		if ( immediateNumber[0] == 1 ) {
			
			Flag = 0 ; // 标记为负数
			
			/* copy原数组而不改变数组 */
			int copy[16] = {0} ;
			for ( i = 0 ; i < 16 ; i++ )
				copy[i] = immediateNumber[i] ;
			
			/* 取反 */
			for ( i = 1 ; i < 16 ; i++ ) {
				
				if ( copy[i] == 0 )
					copy[i] = 1 ;
				
				else if ( copy[i] == 1 )
					copy[i] = 0 ;
					
			}
			
			/* +1 */
			copy[15]++ ;
			for ( i = 15 ; i > 0 ; i-- ) {
				
				if ( copy[i] > 1 ) {
					
					copy[i-1] = copy[i-1] + 1 ;
					copy[i] = copy[i] - 2 ;
					
				}
			}
			
			/* 计算立即数绝对值 */
			for ( i = 1 ; i < 16 ; i++ )
				im = 2*im + copy[i] ;
		}
		
		else
			/* 计算立即数 */
			for ( i = 1 ; i < 16 ; i++ )
				im = 2*im + immediateNumber[i] ;
		
		/* 若标记为负则取负 */ 
		if ( Flag == 0 )
			im = -im ;
		
		
		return im ; 
	}

	
/* 读取 */
	void scan ( FILE* fPtr , int com )
	{	
		
		/* 计算 front */
		front = scanFront ( fPtr ) ;
		
		
		/* 计算 behind */
		behind = scanBehind ( fPtr ) ;
		
		
		/* 计算 ir */
		ir = 16*16*com + 16*front + behind ;
		
		
		/* 计算 immediateValue */
		immediateValue =  scanImmediateValue ( fPtr ) ;
		
		
		/* 吃掉所有直到换行符 */ 
		while ( ch != 10 && ch != 0 && com != 0 )
			ch = fgetc ( fPtr ) ;
		
		
		return ;
	}


/* 停机指令 -- 0 */
	void stop ( FILE* fPtr , int com )
	{
		
		/* 读取行剩余数字 */
		scan ( fPtr , com ) ;
		
		
		/* 计数器 */
		ip = ip + 4 ;
		
		
		/* 暂存数据重置 */
		reset (  ) ;
		
		
		return ;
	}
	
	
/* 数据传送指令 -- 1 */
	void transport ( FILE* fPtr , int com )
	{
		
		/* 读取行剩余数字 */
		scan ( fPtr , com ) ;
		
	
		/* 分情况传输 */
		 
			/* 将立即数传入数据寄存器 */
			if ( behind == 0 )
				ax[front] = immediateValue ;
			
			/* 将数据寄存器内容传入地址寄存器所指向单元 */ 
			else if ( 0 < behind && behind <= 4 )
				dataSegment[ ( ax[front] - 16384 ) / 2 ] = ax[behind] ;
			
			/* 将地址寄存器所指向内容传入数据寄存器 */
			else if ( behind > 4 )
				ax[front] = dataSegment[ ( ax[behind] - 16384 ) / 2 ] ;
		
		
		/* 计数器 */
		ip = ip + 4 ;
		
	
		/* 暂存数据重置 */
		reset (  ) ; 
	
		
		return ;
	}
	
	
/* 算术运算指令 -- 2 3 4 5 */
	void arithmetic ( FILE* fPtr , int com )
	{
		
		/* 读取行剩余数字 */
		scan ( fPtr , com ) ;
		
		
		/* 选择运算种类 */
		
			// +
			if ( com == 2 ) {
				
				if ( behind == 0 )
					ax[front] = ax[front] + immediateValue ;
				
				else
					ax[front] = ax[front] + dataSegment[ ( ax[behind] - 16384 ) / 2 ] ;
			}
			
			// -
			if ( com == 3 ) {
				
				if ( behind == 0 )
					ax[front] = ax[front] - immediateValue ;
				
				else
					ax[front] = ax[front] - dataSegment[ ( ax[behind] - 16384 ) / 2 ] ;
			}
			
			// *
			if ( com == 4 ) {
				
				if ( behind == 0 )
					ax[front] = ax[front] * immediateValue ;
				
				else
					ax[front] = ax[front] * dataSegment[ ( ax[behind] - 16384 ) / 2 ] ;
			}
			
			// -
			if ( com == 5 ) {
				
				if ( behind == 0 )
					ax[front] = ax[front] / immediateValue ;
				
				else
					ax[front] = ax[front] / dataSegment[ ( ax[behind] - 16384 ) / 2 ] ;
			}
		
		
		/* 计数器 */
		ip = ip + 4 ;
		
		
		/* 暂存数据重置 */
		reset (  ) ;
		
		
		return ;
	}
	
	
/* 逻辑运算指令 -- 6 7 8 */
	void logic ( FILE* fPtr , int com )
	{
		
		/* 读取行剩余数字 */
		scan ( fPtr , com ) ;
		
		
		/* 选择运算种类 */
			
			/* "逻辑与" 运算 */
			// &&
			if ( com == 6 ) {
				
				if ( behind == 0 )
					ax[front] = ax[front] && immediateValue ;
				
				else
					ax[front] = ax[front] && dataSegment[ ( ax[behind] - 16384 ) / 2 ] ;
			}
			
			// ||
			if ( com == 7 ) {
				
				if ( behind == 0 )
					ax[front] = ax[front] || immediateValue ;
				
				else
					ax[front] = ax[front] || dataSegment[ ( ax[behind] - 16384 ) / 2 ] ;
			}
			
			// !
			if ( com == 8 ) {
				
				if ( behind == 0 )
					ax[front] = ! ax[front] ;
				
				else
					dataSegment[ ( ax[behind] - 16384 ) / 2 ] = ! dataSegment[ ( ax[behind] - 16384 ) / 2 ] ;
			}
		
		
		/* 计数器 */
		ip = ip + 4 ;
		
		
		/* 暂存数据重置 */
		reset (  ) ;
		
		
		return ;
	}
	
	
/* 比较指令 -- 9 */
	void compare ( FILE* fPtr , int com )
	{
		
		/* 读取行剩余数字 */
		scan ( fPtr , com ) ;
		
		
		/* 比较大小 */
		
			/* 寄存器 front和立即数比大小 */
			if ( behind == 0 ) {
				
				if ( ax[front] > immediateValue )
					flag = 1 ;
				
				else if ( ax[front] == immediateValue )
					flag = 0 ;
				
				else if ( ax[front] < immediateValue )
					flag = -1 ;
				
			}
			
			else {
			
				if ( ax[front] > dataSegment[ ( ax[behind] - 16384 ) / 2 ] )
					flag = 1 ;
				
				else if ( ax[front] == dataSegment[ ( ax[behind] - 16384 ) / 2 ] )
					flag = 0 ;
				
				else if ( ax[front] < dataSegment[ ( ax[behind] - 16384 ) / 2 ] )
					flag = -1 ;
			}
		
		
		/* 计数器 */
		ip = ip + 4 ;
		
		
		/* 暂存数据重置 */
		reset (  ) ;
		
		
		return ;
	}
	
	
/* 跳转指令 -- 10 */
	void skip ( FILE* fPtr , int com )
	{
		
		/* 读取行剩余数字 */
		scan ( fPtr , com ) ;
		
		/* 跳转指令 */ 
		if ( behind == 0 || behind == 1 && flag == 0 || behind == 2 && flag == 1 || behind == 3 && flag == -1 ) {
			
			/* 修改 ip */
			ip = ip + immediateValue ;
			
			/* 计数 */ 
			int cnt ;
			
			/* 修改指针 */
			rewind ( fPtr ) ;
			for ( ch = 0 , cnt = 0 ; cnt < 8 * ( ip ) || ch != 10 ; ) {
			
				/* 读取当前字符并后移指针 */ 
				ch = fgetc ( fPtr ) ; 
				
				/* 过滤空格和换行 */ 
				if ( ch >= '0' && ch <= '9' )
					cnt++ ;
				
			}
			
		}
		
		
		/* 正常计数 */ 
		else
			ip = ip + 4 ;
		
		
		/* 暂存数据重置 */
		reset (  ) ;
		
		
		return ;
	}
	
	
/* 输入输出指令 -- 11 12 */
	void putIn_Out ( FILE* fPtr , int com )
	{
		
		/* 读取行剩余数字 */
		scan ( fPtr , com ) ;
		
		
		/* 输入输出分类 */
		
			/* 输入 */
			if ( com == 11 ) {
				
				printf ( "in:\n" ) ;
				scanf ( "%d" , & ax[front] ) ;
			}
			
			else if ( com == 12 ) {
				
				printf ( "out: %d\n" , ax[front] ) ;
			}
		
		
		/* 计数器 */
		ip = ip + 4 ; 
		
		
		/* 暂存数据重置 */
		reset (  ) ;
		
		
		return ;
	}
	
	
/* 输出寄存器状态 */
	void printState ( int Flag )
	{
		
		/* 打印寄存器状态 */ 
		printf ( "ip = %d\n" , ip ) ;
		printf ( "flag = %d\n" , flag ) ;
		printf ( "ir = %d\n" , ir ) ;
		printf ( "ax1 = %d ax2 = %d ax3 = %d ax4 = %d\n" , ax[1] , ax[2] , ax[3] , ax[4] ) ;
		printf ( "ax5 = %d ax6 = %d ax7 = %d ax8 = %d\n" , ax[5] , ax[6] , ax[7] , ax[8] ) ;
		
		if ( Flag == 0 ) {
			
			int i ; // 动态下标 
			
			/* 打印 CodeSegment */ 
			printf ( "\ncodeSegment :\n" ) ;
			
			for ( i = 0 ; i < 128 ; i++ ) {
				
				printf ( "%d" , codeSegment[i] ) ;
				
				if ( ( i+1 ) % 8 == 0 )
					printf ( "\n" ) ;
				else
					printf ( " " ) ;
				
			}
			
			/* 打印 CodeSegment */
			printf ( "\ndataSegment :\n" ) ;
			
			for ( i = 0 ; i < 256 ; i++ ) {
				
				printf ( "%d" , dataSegment[i] ) ;
				
				if ( ( i+1 ) % 16 == 0 )
					printf ( "\n" ) ;
				else
					printf ( " " ) ;
						
			}
			
		}
		
		
		return ;
	}
	
	
/* 暂存数据重置 -- 1-12命令都需要引入 */
	void reset (  )
	{
		
		/* 动态下标 */
		int i = 0 ;
		
		/* 重置 */ 
		ch = 0 ;
		
		for ( i = 0 ; i < 8 ; i++ )
			command[i] = 0 ;
		
		for ( i = 0 ; i < 4 ; i++ )
			registerFront[i] = 0 ;
		front = 0 ;
		
		for ( i = 0 ; i < 4 ; i++ )
			registerBehind[i] = 0 ;
		behind = 0 ;
		
		for ( i = 0 ; i < 16 ; i++ )
			immediateNumber[i] = 0 ;
		immediateValue = 0 ;
		
		
		return ;
	}
