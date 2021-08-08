#include"CPU.h"

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

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
