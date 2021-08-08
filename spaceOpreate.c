#include"CPU.h"

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
