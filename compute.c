#include"CPU.h"

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
