#include<windows.h>
#include<process.h>
#include<stdio.h>   
#include<stdlib.h>
#include<time.h> 


/* 0.00 �Ĵ������� ---------------------------------------------------- */

	/* ����Ĵ����ṹ�� struct -- segment */
		typedef struct Segment
		{
			HANDLE hMutex ;     //  ���̻������ 
			int lock ;          //  ͬ���� 
			int end1 ;          //  �߳� 1 �����ź� 
			int end2 ;          //  �߳� 2 �����ź� 
			int** txt1 ;        //  ���� 1
			int line1 ;         //  ���� 1 ���� 
			int** txt2 ;        //  ���� 2 
			int line2 ;         //  ���� 2 ���� 
			int* codeSegment ;  //  ����Ĵ��� 
			int* dataSegment ;  //  ���ݼĴ��� 
		}  segment ;
	
 
	/* ����Ĵ����ṹ�� struct -- MTC */
		typedef struct mtc
		{
			int id ;    // ���� ID 
			int ip ;    // ��������� 
			int flag ;  // ��־�Ĵ��� 
			int ir ;    // ����Ĵ��� 
			int* ax ;   // �Ĵ���, 0��λ����,��������
		}  MTC ;
 

	/* �����в������ݽṹ�� */
		typedef struct Number
		{
			int com ;    // 01 ~ 08 λתʮ���� 
			int front ;  // 09 ~ 12 λתʮ���� 
			int behind ; // 13 ~ 16 λתʮ���� 
			int im ;     // 17 ~ 32 λתʮ���� 
		}  number ; 

	
/* 1.11 �ռ����� (malloc) �� ��������/��ʼ�� �Լ��ͷ�(free) ----------- */

	/* txt   : �ı��ݴ�ռ� ���� �� �������� */
	int** generateTxt ( FILE* fPtr , int line ) ;

	/* code  : ����ģ��ռ� ���� �� �������� */
	int* generateCode ( int** txt1 , int** txt2 , int line1 , int line2 ) ;

	/* data  : ����ģ��ռ� ���� �� ��ʼ�� */
	int* generateData (  ) ;

	/* struct: �Ĵ����ṹ�� ���� �� ��ʼ�� */
	MTC* generateStruct ( int id ) ;
	
	/* main : �ͷ�����ռ� */
	void mainFree ( segment* solid ) ;


/* 2.22 ����������� ( ���̸��� ) ------------------------------------- */

	/* �������� */
	int countLine ( FILE* fPtr ) ;

	/* ���������� */ 
	int countImmediateValue ( int** txt , int c , int l ) ;


/* 3.33 �̺߳��� ------------------------------------------------------ */ 

	unsigned __stdcall Fun1Proc ( void* pArguments ) ;
	unsigned __stdcall Fun2Proc ( void* pArguments ) ;


/* 4.44 ָ����� ---------------------------------------------------- */
 
	/* ͣ��ָ�� ------ 0 */
	void stop       ( number* num , MTC* cur , int* data ) ;

	/* ���ݴ���ָ�� -- 1 */
	void transport  ( number* num , MTC* cur , int* data ) ;

	/* ��������ָ�� -- 2 3 4 5 */
	void arithmetic ( number* num , MTC* cur , int* data ) ;

	/* �߼�����ָ�� -- 6 7 8 */
	void logic      ( number* num , MTC* cur , int* data ) ;

	/* �Ƚ�ָ�� ------ 9 */
	void compare    ( number* num , MTC* cur , int* data ) ;

	/* ��תָ�� ------ 10 */
	void skip       ( number* num , MTC* cur , int* data ) ;

	/* �������ָ�� -- 11 12 */
	void putIn_Out  ( number* num , MTC* cur , int* data ) ;
	
	/* ���� ---------- 13 */
	void lock       ( number* num , MTC* cur , segment* solid ) ;
	
	/* �ͷ� ---------- 14 */
	void release    ( number* num , MTC* cur , segment* solid ) ;
	 
	/* ����˯�� ------ 15 */ 
	void funcSleep  ( number* num , MTC* cur ) ;
	

/* 5.55 ��������ܼ����� ---------------------------------------------- */
 
	/* func : ����Ĵ���״̬ */
	void funcPrint ( int com , MTC* cur , segment* solid ) ;
	
	/* main : ��� dataSegment �� codeSegment */
	void mainPrint ( segment* solid ) ;  
	

/* -------------------------------------------------------------------- */

/* ������ */

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


/* -------------------------------------------------------------------- */


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


/* txt : �ı��ݴ�ռ� ���� �� �������� */
	int** generateTxt ( FILE* fPtr , int line )
	{
		
		/* ���� txt�ռ� : ��ά��һά */
			int** txt = ( int** ) malloc ( sizeof ( int* ) * ( line ) ) ;
			
			int i , j ;
			char ch ;
			for ( i = 0 ; i < line ; i++ )
				txt[i] = ( int* ) malloc ( sizeof ( int ) * 32 ) ;
		
		/* ���� txt ���� */
			for ( i = 0 ; i < line ; i++ ) {
				
				for ( j = 0 ; j < 32 ; j++ ) {
					ch = fgetc ( fPtr ) ;
					txt[i][j] = ch - 48 ;
					
				}
		
				/* �жϽ������� */
				if ( i == line-1 )
					break ;
				else
					while ( ch != 10 )
						ch = fgetc ( fPtr ) ; 
			}
		
		
		return txt ;
	}


/* code : ����ģ��ռ� ���� �� �������� */
	int* generateCode ( int** txt1 , int** txt2 , int line1 , int line2 )
	{
		
		/* �������ռ� */
			int* code = ( int* ) malloc ( sizeof ( int ) * 128 ) ;
			
			int i , j ;
			for ( i = 0 ; i < 128 ; i++ ) // ��ʼ��
				code[i] = 0 ;
		
		/* ���� code ���� */
			for ( i = 0 ; i < line1 ; i++ )
				for ( j = 0 ; j < 32 ; j++ )
					code[i] = 2*code[i] + txt1[i][j] ;
			
			for ( i = 64 ; i < line2 + 64 ; i++ )
				for ( j = 0 ; j < 32 ; j++ )
					code[i] = 2*code[i] + txt2[i-64][j] ;
		
			
		return code ;
	}


/* data : ����ģ��ռ� ���� �� ��ʼ�� */
	int* generateData (  )
	{
		
		/* �������ݿռ� */
			int* data = ( int* ) malloc ( sizeof ( int ) * 256 ) ;
		
		/* ��ʼ�� */
			int i ;
			for ( i = 0 ; i < 256 ; i++ )
				data[i] = 0 ;
			
		
		return data ;
	}


/* struct : �Ĵ����ṹ�� ���� �� ��ʼ�� */
	MTC* generateStruct ( int id )
	{
		
		/* ����ṹ��ռ� */
		MTC* cur = ( MTC* ) malloc ( sizeof ( MTC ) ) ;
		
		/* ��ʼ�� */
			cur->id = id ;    //  id ��ʼ�� 
			
			if ( id == 1 )    //  ip ��ʼ�� 
				cur->ip = 0 ;
			else
				cur->ip = 256 ;
				
			cur->flag = 0 ;  // flag ��ʼ�� 
			cur->ir = 0 ;     //  ir ��ʼ�� 
			
			cur->ax = ( int* ) malloc ( sizeof ( int ) * 9 ) ;  // ax ��ʼ�� 
			int  i ;
			for ( i = 0 ; i < 9 ; i++ )
				cur->ax[i] = 0 ;
		
		
		return cur ;
	}


/* main : �ͷ�����ռ� */
	void mainFree ( segment* solid )
	{
		
		int i ;
		for ( i = 0 ; i < solid->line1 ; i++ )  //  �ͷ� txt1 
			free ( solid->txt1[i] ) ;
		free ( solid->txt1 ) ;
		for ( i = 0 ; i < solid->line2 ; i++ )  //  �ͷ� txt2 
			free ( solid->txt2[i] ) ;
		free ( solid->txt2 ) ;
		
		free ( solid->codeSegment ) ;  //  �ͷŴ���Ĵ��� 
		free ( solid->dataSegment ) ;  //  �ͷ����ݼĴ��� 
		
		free ( solid ) ;  //  �ͷŹ�̬����Ĵ��� 
		
		
		return ;
	}


/* �������� */ 
	int countLine ( FILE* fPtr )
	{
		
		int line = 0 ; // ������ʼ�� 0
		
		char ch ;
		int i , sum ;  // ��̬�±� �� countLine����֮��
			
		/* ѭ�� line ���� */
			int Flag = 1 ;
			while ( Flag ) {
			
				for ( sum = 0 , i = 0 ; i < 8 ; i++ ) {
					
					ch = fgetc ( fPtr ) ;  // �����ַ���
					sum = sum + ch - 48 ; // ���� 8λ��֮��
					
				}
				
				line++ ; // ��������
				
				if ( sum == 0 )  // ǰ 8 λ��Ϊ 0 , ��Ϊ 0
					Flag = 0 ;
				
				/* �Ե� \n ���� ���ڵ������ַ� */
				ch = fgetc ( fPtr ) ;
				while ( ch != 10 && ch != 0 )
					ch = fgetc ( fPtr ) ;
				
			}
		
			rewind ( fPtr ) ;  // �����ļ�ָ�� 
		
		
		return line ;
	}


/* ���������� */ 
	int countImmediateValue ( int** txt , int c , int l )
	{
		
		/* �������� */
			int im = 0 ;   // ��������ʼ��
			int Flag = 1 ; //�������������
			int i ;        // ��̬�±�
		
		/* ������Ϊ�� */
			if ( txt[c][l] == 0 ) 
				for ( i = 1 ; i < 16 ; i++ )
					im = 2*im + txt[c][l+i] ;
			
			else {
				
				Flag = 0 ; // ���Ϊ����
				
				/* copyԭ��������ı�ԭ���� */
					int* copy = ( int* ) malloc ( sizeof ( int ) * 15 ) ;
					for ( i = 1 ; i < 16 ; i++ )
						copy[i-1] = txt[c][l+i] ;
				
				/* ȡ�� */
					for ( i = 0 ; i < 15 ; i++ ) {
						if ( copy[i] == 0 )
							copy[i] = 1 ;
						
						else if ( copy[i] == 1 )
							copy[i] = 0 ;	
					}
				
				/* +1 */
					copy[14]++ ;
					for ( i = 14 ; i > 0 ; i-- )  // ��λ 
						
						if ( copy[i] > 1 ) {
							copy[i-1] = copy[i-1] + 1 ;
							copy[i] = copy[i] - 2 ;
						}
						
				/* ����������ֵ */
					for ( i = 0 ; i < 15 ; i++ )
						im = 2*im + copy[i] ;
			
				free ( copy ) ;
				
			}
			
		/* �����Ϊ����ȡ�� */ 
			if ( Flag == 0 )
				im = -im ;
		
		
		return im ; 
	}


/* ͣ��ָ�� -- 0 */
	void stop ( number* num , MTC* cur , int* data )
	{
		
		/* ���� ir */ 
			cur->ir = 256*num->com + 16*num->front + num->behind ;
		
		
		/* ������ */
			cur->ip = cur->ip + 4 ;
		
		
		return ;
	}
	
	
/* ���ݴ���ָ�� -- 1 */
	void transport ( number* num , MTC* cur , int* data )
	{
		
		/* ���� ir */ 
			cur->ir = 256*num->com + 16*num->front + num->behind ;
		
		
		/* ���ִ���ָ�� */ 
			if ( num->behind == 0 )
				cur->ax[num->front] = num->im ;
			
			else if ( num->behind >= 1 && num->behind <= 4 )
				data[ ( cur->ax[num->front] - 16384 ) / 2 ] = cur->ax[num->behind] ;
			
			else if ( num->behind >= 5 && num->behind <= 8 )
				cur->ax[num->front] = data[ ( cur->ax[num->behind] - 16384 ) / 2 ] ;
		
			
		/* ������ */
			cur->ip = cur->ip + 4 ;
		
		
		return ;
	}


/* ��������ָ�� -- 2 3 4 5 */
	void arithmetic ( number* num , MTC* cur , int* data )
	{
		
		/* ���� ir */ 
			cur->ir = 256*num->com + 16*num->front + num->behind ;
		
		
		/* ��������� */
			
			// + ���� 
			if ( num->com == 2 ) {
				
				if ( num->behind == 0 )
					cur->ax[num->front] = cur->ax[num->front] + num->im ;
				
				else
					cur->ax[num->front] = cur->ax[num->front] + data[ ( cur->ax[num->behind] - 16384 ) / 2 ] ;
					
			}
			
			// - ���� 
			if ( num->com == 3 ) {
				
				if ( num->behind == 0 )
					cur->ax[num->front] = cur->ax[num->front] - num->im ;
				
				else
					cur->ax[num->front] = cur->ax[num->front] - data[ ( cur->ax[num->behind] - 16384 ) / 2 ] ;
					
			}
			
			// * ���� 
			if ( num->com == 4 ) {
				
				if ( num->behind == 0 )
					cur->ax[num->front] = cur->ax[num->front] * num->im ;
				
				else
					cur->ax[num->front] = cur->ax[num->front] * data[ ( cur->ax[num->behind] - 16384 ) / 2 ] ;
					
			}
			
			// - ���� 
			if ( num->com == 5 ) {
				
				if ( num->behind == 0 )
					cur->ax[num->front] = cur->ax[num->front] / num->im ;
				
				else
					cur->ax[num->front] = cur->ax[num->front] / data[ ( cur->ax[num->behind] - 16384 ) / 2 ] ;
					
			}
		
		
		/* ������ */
			cur->ip = cur->ip + 4 ;
		
		
		return ;
	}


/* �߼�����ָ�� -- 6 7 8 */
	void logic ( number* num , MTC* cur , int* data )
	{
		
		/* ���� ir */ 
			cur->ir = 256*num->com + 16*num->front + num->behind ;
		
		
		/* ��������� */
		
			// && ���� 
			if ( num->com == 6 ) {
				
				if ( num->behind == 0 )
					cur->ax[num->front] = cur->ax[num->front] && num->im ;
				
				else
					cur->ax[num->front] = cur->ax[num->front] && data[ ( cur->ax[num->behind] - 16384 ) / 2 ] ;
			}
			
			// || ���� 
			if ( num->com == 7 ) {
				
				if ( num->behind == 0 )
					cur->ax[num->front] = cur->ax[num->front] || num->im ;
				
				else
					cur->ax[num->front] = cur->ax[num->front] || data[ ( cur->ax[num->behind] - 16384 ) / 2 ] ;
			}
			
			// ! ���� 
			if ( num->com == 8 ) {
				
				if ( num->behind == 0 )
					cur->ax[num->front] = ! cur->ax[num->front] ;
				
				else
					data[ ( cur->ax[num->behind] - 16384 ) / 2 ] = ! data[ ( cur->ax[num->behind] - 16384 ) / 2 ] ;
			}
		
		
		/* ������ */
			cur->ip = cur->ip + 4 ;
		
		
		return ;
	}


/* �Ƚ�ָ�� -- 9 */
	void compare ( number* num , MTC* cur , int* data )
	{
		
		/* ���� ir */ 
			cur->ir = 256*num->com + 16*num->front + num->behind ;
		
		
		/* �Ĵ��� 1 2 3 4 �� ���������Ƚ� */
			if ( num->behind == 0 ) {
				
				if ( cur->ax[num->front] > num->im )
					cur->flag = 1 ;
				
				else if ( cur->ax[num->front] == num->im )
					cur->flag = 0 ;
				
				else if ( cur->ax[num->front] < num->im )
					cur->flag = -1 ;
					
			}
		
		/* �Ĵ��� 1 2 3 4 �� �Ĵ��� 5 6 7 8 ���Ƚ� */
			else {
				
				if ( cur->ax[num->front] > data[ ( cur->ax[num->behind] - 16384 ) / 2 ] )
					cur->flag = 1 ;
				
				else if ( cur->ax[num->front] == data[ ( cur->ax[num->behind] - 16384 ) / 2 ] )
					cur->flag = 0 ;
				
				else if ( cur->ax[num->front] < data[ ( cur->ax[num->behind] - 16384 ) / 2 ] )
					cur->flag = -1 ;
					
			}
		
		
		/* ������ */
			cur->ip = cur->ip + 4 ;
		
		
		return ;
	}


/* ��תָ�� -- 10 */
	void skip ( number* num , MTC* cur , int* data )
	{
		
		/* ���� ir */ 
			cur->ir = 256*num->com + 16*num->front + num->behind ;
		
		
		/* ������תָ�� */
			if ( num->behind == 0 )
				cur->ip = cur->ip + num->im ;
			
			else if ( num->behind == 1 && cur->flag == 0 )
				cur->ip = cur->ip + num->im ;
			
			else if ( num->behind == 2 && cur->flag == 1 )
				cur->ip = cur->ip + num->im ;
			
			else if ( num->behind == 3 && cur->flag == -1 )
				cur->ip = cur->ip + num->im ;
		
		/* ����ת */
			else
				cur->ip = cur->ip + 4 ;
		
		
		return ;
	}


/* �������ָ�� -- 11 12 */
	void putIn_Out ( number* num , MTC* cur , int* data )
	{
		
		/* ���� ir */ 
			cur->ir = 256*num->com + 16*num->front + num->behind ;
		
		
		/* ���� */
			if ( num->com == 11 ) {
				
				printf ( "in:\n" ) ;
				scanf ( "%d" , & cur->ax[num->front] ) ;
			}
		
		/* ��� */
			else if ( num->com == 12 ) {
				
				printf ( "id = %d out: %d\n\n" , cur->id , cur->ax[num->front] ) ;
			}
		
		/* ������ */
			cur->ip = cur->ip + 4 ;
		
		
		return ;
	}


/* ���� -- 13 */
	void lock ( number* num , MTC* cur , segment* solid )
	{
		
		/* ���� ir */ 
			cur->ir = 256*num->com + 16*num->front + num->behind ;
		
		/* ��ס��� */
			WaitForSingleObject ( solid->hMutex , INFINITE ) ;
		
		/* ������ */
			cur->ip = cur->ip + 4 ;
		
		
		return ;
	}


/* �ͷ� -- 14 */
	void release ( number* num , MTC* cur , segment* solid )
	{
		
		/* ���� ir */ 
			cur->ir = 256*num->com + 16*num->front + num->behind ;
		
		/* �ͷž�� */
			ReleaseMutex ( solid->hMutex ) ;
		
		/* ������ */
			cur->ip = cur->ip + 4 ;
		
		
		return ;
	}


/* ˯�� -- 15 */
	void funcSleep ( number* num , MTC* cur )
	{
		
		/* ���� ir */ 
			cur->ir = 256*num->com + 16*num->front + num->behind ;
		
		/* ˯���������� */
			Sleep ( num->im ); 
		
		/* ������ */
			cur->ip = cur->ip + 4 ;
		
		
		return ;
	}


/* func : ����Ĵ���״̬ */
	void funcPrint ( int com , MTC* cur , segment* solid )
	{
		
		/* ��ӡ�Ĵ���״̬ */ 
		printf ( "id = %d\nip = %d\nflag = %d\nir = %d\n"
			"ax1 = %d ax2 = %d ax3 = %d ax4 = %d\n"
			"ax5 = %d ax6 = %d ax7 = %d ax8 = %d\n\n"
			, cur->id , cur->ip , cur->flag , cur->ir 
			, cur->ax[1] , cur->ax[2] , cur->ax[3] , cur->ax[4] 
			, cur->ax[5] , cur->ax[6] , cur->ax[7] , cur->ax[8] ) ;
			
		return ;
	}


/* main : ��� dataSegment �� codeSegment */
	void mainPrint ( segment* solid ) 
	{	
		
		int i ;
		/* ��ӡ code */ 
			printf ( "codeSegment :\n" ) ;
			 
			for ( i = 0 ; i < 128 ; i++ ) {
				
				printf ( "%d" , solid->codeSegment[i] ) ;
				
				if ( ( i+1 ) % 8 == 0 )
					printf ( "\n" ) ;
				else
					printf ( " " ) ;
			}
		
		/* ��ӡ data */
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
