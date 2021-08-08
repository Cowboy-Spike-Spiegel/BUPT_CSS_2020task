#include<windows.h>
#include<process.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>


/* ȫ�ֱ������� -------------------------------------------------- */
	
	// ������ 
	HANDLE hMutex ;
	
	// ͬ���� 
	int Lock ;
	int end1 ;
	int end2 ; 
	
	// ���üĴ��� 
	int* codeSegment ;
	int* dataSegment ;
	
	// �̴߳������� 
	int** target1 ;
	int** target2 ;
	
	// �̲߳��� 
	int* ip ;
	int* flag ;
	int** ax ;
	
	
/* �Ӻ������� ---------------------------------------------------- */ 
	
	// �̺߳���
	unsigned __stdcall Fun1Proc ( void* pArguments ) ;
	unsigned __stdcall Fun2Proc ( void* pArguments ) ;
	
	// ָ���
	void stop       (                        int index ) ;
	void transport  ( int** target , int i , int index ) ;
	void arithmetic ( int** target , int i , int index ) ;
	void logic      ( int** target , int i , int index ) ;
	void compare    ( int** target , int i , int index ) ;
	void skip       ( int** target , int i , int index ) ;
	void putIn_Out  ( int** target , int i , int index ) ;
	void lock       (                        int index ) ;
	void release    (                        int index ) ;
	void funcSleep  ( int** target , int i , int index ) ;
	
	// �ڴ��������
	void spaceApply () ;
	void spaceRelease ( int line1 , int line2 ) ; 
	int generate ( FILE* fPtr , int posPlus ) ;
	
	// print ����
	void mainPrint () ;
	void funcPrint ( int** target , int i , int index ) ; 


/* ������ -------------------------------------------------------- */

int main ( int argc , char *argv[ ] )
{ 
	spaceApply () ;
	
	// �ļ�����
	FILE *fPtr1 =  fopen ( "dict1.dic" , "r" ) ;
	int line1 = generate ( fPtr1 , 0 ) ;
	FILE *fPtr2 =  fopen ( "dict2.dic" , "r" ) ;
	int line2 = generate ( fPtr2 , 64 ) ;
	
	srand ( (unsigned) time ( NULL ) ) ;
	Lock = rand() % 2 + 1 ; // lock �����ʼ��
	
	// �����߳� 1 , 2
	HANDLE hThread1 , hThread2 ;
	hThread1 = (HANDLE) _beginthreadex ( NULL , 0 , Fun1Proc , NULL , 0 , NULL ) ;
	hThread2 = (HANDLE) _beginthreadex ( NULL , 0 , Fun2Proc , NULL , 0 , NULL ) ;
	
	// �ر��߳� 1 , 2
	WaitForSingleObject ( hThread1 , INFINITE ) ;
	CloseHandle ( hThread1 ) ;
	WaitForSingleObject ( hThread2 , INFINITE ) ;	
	CloseHandle ( hThread2 ) ;
	
	// ����Ĵ���
	mainPrint () ;
	
	spaceRelease ( line1 , line2 ) ;
	
	return 0 ;
}


/* �Ӻ������� ---------------------------------------------------- */ 
	
	/* �߳� 1 */
	unsigned __stdcall Fun1Proc ( void* pArguments )
	{
		end1 = 1 ; // ��Ϊ���ź� 
		
		int i ;
		int Flag ;
		for ( i = 0 , Flag = 1 ; Flag ; ) {
			
			switch ( target1[i][0] ) {
			 	
				case 0  :
						stop       (               0 ) ;
						break ;
						
				case 1  :
						transport  ( target1 , i , 0 ) ;
						break ;
						
				case 2  :
						arithmetic ( target1 , i , 0 ) ;
						break ;
						
				case 3  :
						arithmetic ( target1 , i , 0 ) ;
						break ;
						
				case 4  :
						arithmetic ( target1 , i , 0 ) ;
						break ;
						
				case 5  :
						arithmetic ( target1 , i , 0 ) ;
						break ;
						
				case 6  :
						logic      ( target1 , i , 0 ) ;
						break ;
						
				case 7  :
						logic      ( target1 , i , 0 ) ;
						break ;
						
				case 8  :
						logic      ( target1 , i , 0 ) ;
						break ;
						
				case 9  :
						compare    ( target1 , i , 0 ) ;
						break ;
						
				case 10 :
						skip       ( target1 , i , 0 ) ;
						break ;
						
				case 11 :
						putIn_Out  ( target1 , i , 0 ) ;
						break ;
						
				case 12 :
						putIn_Out  ( target1 , i , 0 ) ;
						Lock = 2 ; // �黹����Ȩ 
						break ;
						
				case 13 :
						while ( Lock == 2 && end2 == 1 ) ;
						lock       (               0 ) ;
						break ;
						
				case 14 :
						release    (               0 ) ;
						break ;
						
				case 15 :
						funcSleep  ( target1 , i , 0 ) ;
						break ;
			}
			
			// ����Ĵ���
			funcPrint ( target1 , i , 0 ) ;
			
			//  ͣ��
			if ( target1[i][0] == 0 )
				Flag = 0 ;
			
			// �������� 
			i = ip[0] / 4 ;
		}
		
		end1 = 0 ; // ��Ϊ���ź� 
		
		return 0 ;
	}
	
	/* �߳� 2 */
	unsigned __stdcall Fun2Proc ( void* pArguments )
	{
		end2 = 1 ; // ��Ϊ���ź� 
		
		int i ;
		int Flag ;
		for ( i = 0 , Flag = 1 ; Flag ; ) {
			
			switch ( target2[i][0] ) {
			 	
				case 0  :
						stop       (               1 ) ;
						break ;
						
				case 1  :
						transport  ( target2 , i , 1 ) ;
						break ;
						
				case 2  :
						arithmetic ( target2 , i , 1 ) ;
						break ;
						
				case 3  :
						arithmetic ( target2 , i , 1 ) ;
						break ;
						
				case 4  :
						arithmetic ( target2 , i , 1 ) ;
						break ;
						
				case 5  :
						arithmetic ( target2 , i , 1 ) ;
						break ;
						
				case 6  :
						logic      ( target2 , i , 1 ) ;
						break ;
						
				case 7  :
						logic      ( target2 , i , 1 ) ;
						break ;
						
				case 8  :
						logic      ( target2 , i , 1 ) ;
						break ;
						
				case 9  :
						compare    ( target2 , i , 1 ) ;
						break ;
						
				case 10 :
						skip       ( target2 , i , 1 ) ;
						break ;
						
				case 11 :
						putIn_Out  ( target2 , i , 1 ) ;
						break ;
						
				case 12 :
						putIn_Out  ( target2 , i , 1 ) ;
						Lock = 1 ; // �黹����Ȩ 
						break ;
						
				case 13 :
						while ( Lock == 1 && end1 == 1 ) ;
						lock       (               1 ) ;
						break ;
						
				case 14 :
						release    (               1 ) ;
						break ;
						
				case 15 :
						funcSleep  ( target2 , i , 1 ) ;
						break ;
			}
			
			// ����Ĵ���
			funcPrint ( target2 , i , 1 ) ;
			
			//  ͣ��
			if ( target2[i][0] == 0 )
				Flag = 0 ;
			
			// �������� 
			i = ( ip[1] - 256 ) / 4 ;
		}
		
		end2 = 0 ; // ��Ϊ���ź� 
		
		return 0 ;
	}
	
	/* ͣ��ָ�� -- 0 */
	void stop ( int index )
	{
		// ������
		ip[index] = ip[index] + 4 ;
		
		return ;
	}
	
	/* ���ݴ���ָ�� -- 1 */
	void transport ( int** target , int i , int index )
	{	
		// ���ִ���ָ��
		if ( target[i][2] == 0 )
			ax [index] [ target[i][1]-1 ] = target[i][3] ;
		
		else if ( target[i][2] >= 1 && target[i][2] <= 4 )
			dataSegment[ ( ax [index] [ target[i][1]-1 ] - 16384 ) / 2 ]
			= ax [index] [ target[i][2]-1 ] ;
		
		else if ( target[i][2] >= 5 && target[i][2] <= 8 )
			ax [index] [ target[i][1]-1 ]
			= dataSegment[ ( ax [index] [ target[i][2]-1 ] - 16384 ) / 2 ] ;
		
		// ������
		ip[index] = ip[index] + 4 ;
		
		return ;
	}

	/* ��������ָ�� -- 2 3 4 5 */
	void arithmetic ( int** target , int i , int index )
	{
		/* ��������� */
		
		// + ���� 
		if ( target[i][0] == 2 ) {
			
			if ( target[i][2] == 0 )
				ax [index] [ target[i][1]-1 ]
				= ax [index] [ target[i][1]-1 ] + target[i][3] ;
			
			else
				ax [index] [ target[i][1]-1 ]
				= ax [index] [ target[i][1]-1 ]
				+ dataSegment[ ( ax [index] [ target[i][2]-1 ] - 16384 ) / 2 ] ;	
		}
		
		// - ���� 
		if ( target[i][0] == 3 ) {
			
			if ( target[i][2] == 0 )
				ax [index] [ target[i][1]-1 ]
				= ax [index] [ target[i][1]-1 ]
				- target[i][3] ;
			
			else
				ax [index] [ target[i][1]-1 ]
				= ax [index] [ target[i][1]-1 ]
				- dataSegment[ ( ax [index] [ target[i][2]-1 ] - 16384 ) / 2 ] ;		
		}
			
		// * ���� 
		if ( target[i][0] == 4 ) {
				
			if ( target[i][2] == 0 )
				ax [index] [ target[i][1]-1 ]
				= ax [index] [ target[i][1]-1 ]
				* target[i][3] ;
			
			else
				ax [index] [ target[i][1]-1 ]
				= ax [index] [ target[i][1]-1 ]
				* dataSegment[ ( ax [index] [ target[i][2]-1 ] - 16384 ) / 2 ] ;		
		}
			
		// / ���� 
		if ( target[i][0] == 5 ) {
			
			if ( target[i][2] == 0 )
				ax [index] [ target[i][1]-1 ]
				= ax [index] [ target[i][1]-1 ]
				/ target[i][3] ;
			
			else
				ax [index] [ target[i][1]-1 ]
				= ax [index] [ target[i][1]-1 ]
				/ dataSegment[ ( ax [index] [ target[i][2]-1 ] - 16384 ) / 2 ] ;	
		}
		
		// ������
		ip[index] = ip[index] + 4 ;
		
		return ;
	}

	/* �߼�����ָ�� -- 6 7 8 */
	void logic ( int** target , int i , int index )
	{
		/* ��������� */
		
		// && ���� 
		if ( target[i][0] == 6 ) {
			
			if ( target[i][2] == 0 )
				ax [index] [ target[i][1]-1 ]
				= ax [index] [ target[i][1]-1 ] && target[i][3] ;
			
			else
				ax [index] [ target[i][1]-1 ]
				= ax [index] [ target[i][1]-1 ]
				&& dataSegment[ ( ax [index] [ target[i][2]-1 ] - 16384 ) / 2 ] ;
		}
			
		// || ���� 
		if ( target[i][0] == 7 ) {
			
			if ( target[i][2] == 0 )
				ax [index] [ target[i][1]-1 ]
				= ax [index] [ target[i][1]-1 ] || target[i][3] ;
			
			else
				ax [index] [ target[i][1]-1 ]
				= ax [index] [ target[i][1]-1 ]
				|| dataSegment[ ( ax [index] [ target[i][2]-1 ] - 16384 ) / 2 ] ;
		}
			
		// ! ���� 
		if ( target[i][0] == 8 ) {
			
			if ( target[i][2] == 0 )
				ax [index] [ target[i][1]-1 ]
				= ! ax [index] [ target[i][1]-1 ] ;
			
			else
				dataSegment[ ( ax [index] [ target[i][2]-1 ] - 16384 ) / 2 ]
				= ! dataSegment[ ( ax [index] [ target[i][2]-1 ] - 16384 ) / 2 ] ;
			}
		
		// ������
		ip[index] = ip[index] + 4 ;
		
		return ;
	}

	/* �Ƚ�ָ�� -- 9 */
	void compare ( int** target , int i , int index )
	{
		/* �Ĵ��� 1 2 3 4 �� ���������Ƚ� */
			if ( target[i][2] == 0 ) {
				
				if ( ax [index] [ target[i][1]-1 ] > target[i][3] )
					flag[index] = 1 ;
				
				else if ( ax [index] [ target[i][1]-1 ] == target[i][3] )
					flag[index] = 0 ;
				
				else if ( ax [index] [ target[i][1]-1 ] < target[i][3] )
					flag[index] = -1 ;
					
			}
		
		/* �Ĵ��� 1 2 3 4 �� �Ĵ��� 5 6 7 8 ���Ƚ� */
			else {
				
				if ( ax [index] [ target[i][1]-1 ]
					> dataSegment[ ( ax [index] [ target[i][2]-1 ] - 16384 ) / 2 ] )
						flag[index] = 1 ;
				
				else if ( ax [index] [ target[i][1]-1 ]
					== dataSegment[ ( ax [index] [ target[i][2]-1 ] - 16384 ) / 2 ] )
						flag[index] = 0 ;
					
				else if ( ax [index] [ target[i][1]-1 ]
					< dataSegment[ ( ax [index] [ target[i][2]-1 ] - 16384 ) / 2 ] )
						flag[index] = -1 ;
					
			}
		
		// ������
		ip[index] = ip[index] + 4 ;
		
		return ;
	}

	/* ��תָ�� -- 10 */
	void skip ( int** target , int i , int index )
	{
		/* ������תָ�� */
			if ( target[i][2] == 0 )
				ip[index] = ip[index] + target[i][3] ;
			
			else if ( target[i][2] == 1 && flag[index] == 0 )
				ip[index] = ip[index] + target[i][3] ;
			
			else if ( target[i][2] == 2 && flag[index] == 1 )
				ip[index] = ip[index] + target[i][3] ;
			
			else if ( target[i][2] == 3 && flag[index] == -1 )
				ip[index] = ip[index] + target[i][3] ;
		
		/* ����ת */
			else
				ip[index] = ip[index] + 4 ;
		
		return ;
	}

	/* �������ָ�� -- 11 12 */
	void putIn_Out ( int** target , int i , int index )
	{
		/* ���� */
			if ( target[i][0] == 11 ) {
				
				printf ( "in:\n" ) ;
				scanf ( "%d" , & ax [index] [ target[i][1]-1 ] ) ;
			}
		
		/* ��� */
			else if ( target[i][0] == 12 ) {
				
				printf ( "id = %d out: %d\n\n" , index+1 , ax [index] [ target[i][1]-1 ] ) ;
			}
		
		// ������
		ip[index] = ip[index] + 4 ;
		
		return ;
	}

	/* ���� -- 13 */
	void lock ( int index )
	{
		// ��ס���
		WaitForSingleObject ( hMutex , INFINITE ) ; 
		
		// ������
		ip[index] = ip[index] + 4 ;
		
		return ;
	}

	/* �ͷ� -- 14 */
	void release ( int index )
	{
		// �ͷž��
		ReleaseMutex ( hMutex ) ; 
		
		// ������
		ip[index] = ip[index] + 4 ;
		
		return ;
	}

	/* ˯�� -- 15 */
	void funcSleep ( int** target , int i , int index )
	{
		// ˯����������
		Sleep ( 0 ) ;  // target[i][3]
		
		// ������
		ip[index] = ip[index] + 4 ;
		
		return ;
	}
	
	/* �ڴ����� */ 
	void spaceApply (  )
	{
		int i ;
		
		// codeSegment
		codeSegment = ( int* ) malloc ( sizeof ( int ) * 128 ) ;
		for ( i = 0 ; i < 128 ; i++ )
			codeSegment[i] = 0 ;
		
		// dataSegment 
		dataSegment = ( int* ) malloc ( sizeof ( int ) * 256 ) ;
		dataSegment[0] = 100 ;
		for ( i = 1 ; i < 256 ; i++ )
			dataSegment[i] = 0 ;
		
		// target
		target1 = ( int** ) malloc ( sizeof ( int* ) * 64 ) ;
		target2 = ( int** ) malloc ( sizeof ( int* ) * 64 ) ;
		
		// ip
		ip = ( int* ) malloc ( sizeof ( int ) * 2 ) ;
		ip[0] = 0 ;
		ip[1] = 256 ;
		
		// flag
		flag = ( int* ) malloc ( sizeof ( int ) * 2 ) ;
		flag[0] = 0 ;
		flag[1] = 0 ;
		
		// ax
		ax = ( int** ) malloc ( sizeof ( int* ) * 2 ) ;
		ax[0] = ( int* ) malloc ( sizeof ( int ) * 8 ) ;
		for ( i = 0 ; i < 8 ; i++ )
			ax[0][i] = 0 ;
		ax[1] = ( int* ) malloc ( sizeof ( int ) * 8 ) ;
		for ( i = 0 ; i < 8 ; i++ )
			ax[1][i] = 0 ;
		
		return ;
	}
	
	/* �ڴ��ͷ� */ 
	void spaceRelease ( int line1 , int line2 )
	{
		free ( codeSegment ) ;
		free ( dataSegment ) ;
		
		int i ;
		for ( i = 0 ; i < line1 ; i++ )
			free ( target1[i] ) ;
		free ( target1 ) ;
		for ( i = 0 ; i < line2 ; i++ )
			free ( target1[i] ) ;
		free ( target2 ) ;
		
		free ( ip ) ;
		free ( flag ) ;
		free ( ax[0] ) ;
		free ( ax[1] ) ;
		free ( ax ) ;
		
		return ;
	}
	
	/* �ļ����� */ 
	int generate ( FILE* fPtr , int posPlus )
	{
		
		char ch ;
		int line , code , com , front , behind , im ;
		
		int i ;
		for ( line = 0 ; ; line++ ) {
			
			// ���� target 
			if ( posPlus == 0 )
				target1[line] = ( int* ) malloc ( sizeof ( int ) * 5 ) ;
			else
				target2[line] = ( int* ) malloc ( sizeof ( int ) * 5 ) ;
			
			// ���� com 
			for ( i = 0 , com = 0 ; i < 8 ; i++ ) {
				ch = fgetc ( fPtr ) ;
				com = 2*com + ch - 48 ;
			}
			
			// ���� front 
			for ( front = 0 ; i < 12 ; i++ ) {
				ch = fgetc ( fPtr ) ;
				front = 2*front + ch - 48 ;
			}
			
			// ���� behind 
			for ( behind = 0 ; i < 16 ; i++ ) {
				ch = fgetc ( fPtr ) ;
				behind = 2*behind + ch - 48 ;
			}
			
			// ���� im �� code 
			ch = fgetc ( fPtr ) ;
			code = 2*(256*com + 16*front + behind) + ch - 48 ;
			i++ ;
			
			if ( (ch-48) == 0 ) {
				for ( im = 0 ; i < 32 ; i++ ) {
					ch = fgetc ( fPtr ) ;
					im = 2*im + ch - 48 ;
					code = 2*code + ch - 48 ;
				}
			}
			else {
				for ( im = 0 ; i < 32 ; i++ ) {
					ch = fgetc ( fPtr ) ;
					im = 2*im + !(ch - 48) ;
					code = 2*code + ch - 48 ;
				}
				im = (im+1) * (-1) ; 
			}
			
			codeSegment[line+posPlus] = code ;
			
			
			// ����� target
			if ( posPlus == 0 ) {
				target1[line][0] = com ;
				target1[line][1] = front ;
				target1[line][2] = behind ;
				target1[line][3] = im ;
				target1[line][4] = 256*com + 16*front + behind ;
			}
			else {
				target2[line][0] = com ;
				target2[line][1] = front ;
				target2[line][2] = behind ;
				target2[line][3] = im ;
				target2[line][4] = 256*com + 16*front + behind ;
			}
			
			if ( com == 0 )
				break ;
			
			while ( ch != 0 && ch != '\n' )
				ch = fgetc ( fPtr ) ;
		}
		
		return line+1 ;
	}
	
	/* ������������� */ 
	void mainPrint ()
	{
		int i ;
		
		printf ( "\ncodeSegment:\n" ) ;
		for ( i = 0 ; i < 128 ; i++ ) {
			printf ( "%d" , codeSegment[i] ) ;
			
			if ( i%8 == 7 )
				printf ( "\n" ) ;
			else
				printf ( " " ) ;
		}
		
		printf ( "\ndataSegment:\n" ) ;
		for ( i = 0 ; i < 256 ; i++ ) {
			printf ( "%d" , dataSegment[i] ) ;
			
			if ( i%16 == 15 )
				printf ( "\n" ) ;
			else
				printf ( " " ) ;
		}
		
		return ;
	}
	
	/* �߳�������� */ 
	void funcPrint ( int** target , int i , int index )
	{
		
		printf ( "id = %d\nip = %d\nflag = %d\nir = %d\n"
		"ax1 = %d ax2 = %d ax3 = %d ax4 = %d\n"
		"ax5 = %d ax6 = %d ax7 = %d ax8 = %d\n\n"
		, index+1 , ip[index] , flag[index] , target[i][4] 
		, ax[index][0] , ax[index][1] , ax[index][2] , ax[index][3] 
		, ax[index][4] , ax[index][5] , ax[index][6] , ax[index][7] ) ;
		
		return ;
	}
