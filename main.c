#include"CPU.h"

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main ( int argc , char *argv[ ] )
{
	
	/* ���� solid : ���� codeSegment , dataSegment */
	segment* solid = ( segment* ) malloc ( sizeof ( segment ) ) ;
	srand ( (unsigned) time ( NULL ) ) ;
	solid->lock = rand() % 2 + 1 ; // lock �����ʼ��  
	
	// ����������	
	solid->hMutex = CreateMutex ( NULL , FALSE , NULL ) ;
	
	// ���� dict1.dic	
	FILE *fPtr1 =  fopen ( "dict1.dic" , "r" ) ;
	solid->line1 = countLine ( fPtr1 ) ;
	solid->txt1 = generateTxt ( fPtr1 , solid->line1 ) ;
	fclose ( fPtr1 ) ;
	
	// ���� dict2.dic 
	FILE *fPtr2 =  fopen ( "dict2.dic" , "r" ) ;
	solid->line2 = countLine ( fPtr2 ) ;
	solid->txt2 = generateTxt ( fPtr2 , solid->line2 ) ;
	fclose ( fPtr2 ) ;
	
	// codeSegment��ʼ�� 
	solid->codeSegment
	= generateCode ( solid->txt1 , solid->txt2 , solid->line1 , solid->line2 ) ;
	
	// dataSegment��ʼ�� 
	solid->dataSegment = generateData (  ) ;
	solid->dataSegment[0] = 100 ;  //  ��ʼ��Ʊ��Ϊ 100 
		
	/* �����߳� 1 , 2 */
	HANDLE hThread1
	= (HANDLE) _beginthreadex ( NULL , 0 , Fun1Proc , solid , 0 , NULL ) ;
	HANDLE hThread2
	= (HANDLE) _beginthreadex ( NULL , 0 , Fun2Proc , solid , 0 , NULL ) ;
	
	/* �ر��߳� 1 , 2  , ���ͷ��ڴ� */
	WaitForSingleObject ( hThread1 , INFINITE ) ;
	CloseHandle ( hThread1 ) ;
	WaitForSingleObject ( hThread2 , INFINITE ) ;	
	CloseHandle ( hThread2 ) ;
	
	/* ��� code �� data ���ͷŹ�̬����Ĵ��� */
	mainPrint ( solid ) ;
	mainFree ( solid ) ;
	
	
	return 0 ;
}
