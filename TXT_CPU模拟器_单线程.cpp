#include<stdio.h>
#include<stdlib.h>


/**   �� �� �� �� ��   ********************************************************************************************************/


/* ����Ĵ����ṹ�� struct -- MTC */
typedef struct mtc
{
	/* ��������� */
		int ip ;

	/* ��־�Ĵ��� */
		int flag ;

	/* ���ƼĴ��� */
		int ir ;

	/* ���ݼĴ��� 1 , 2 , 3 , 4 �� ��ַ�Ĵ��� 5 , 6 , 7 , 8 */
		int* ax ;  // 0��λ����,��������

}  MTC ;                                                  // TIP : MTCΪ��������ƴ������ĸ 



/**  �� �� �� �� ��   **********************************************************************************************************/


/* 1.11 �ռ����� (malloc) �� ��������/��ʼ�� �Լ��ͷ�(free) */

	/* txt   : �ı��ݴ�ռ� ���� �� �������� ------------ Line : 160 ~ 191 */
		int** generateTxtSpace ( FILE* fPtr , int line ) ;

	/* code  : ����ģ��ռ� ���� �� �������� ------------ Line : 194 ~ 212 */
		int* generateCodeSpace ( int** txt , int line ) ;

	/* data  : ����ģ��ռ� ���� �� ��ʼ�� -------------- Line : 215 ~ 229 */
		int* generateDataSpace (  ) ;

	/* struct: �Ĵ����ṹ�� ���� �� ��ʼ�� -------------- Line : 232 ~ 251 */
		MTC* generateStruct (  ) ;
	
	/* �ͷ�����ռ� ------------------------------------- Line : 254 ~ 269 */
		void Free ( int** txt , int line , MTC* cur , int* code , int* data ) ;


/* 2.22 ����������� ( ���̸��� ) */

	/* �������� ----------------------------------------- Line : 272 ~ 308 */
		int countLine ( FILE* fPtr ) ;

	/* ���������� --------------------------------------- Line : 311 ~ 370 */ 
		int countImmediateValue ( int** txt , int c , int l ) ;


/* 3.33 ָ����� */
 
	/* ͣ��ָ�� ------ 0 -------------------------------- Line : 373 ~ 386 */
		void stop       ( int com , int front , int behind , int im , MTC* cur , int* data ) ;

	/* ���ݴ���ָ�� -- 1 -------------------------------- Line : 389 ~ 413 */
		void transport  ( int com , int front , int behind , int im , MTC* cur , int* data ) ;

	/* ��������ָ�� -- 2 3 4 5 -------------------------- Line : 416 ~ 476 */
		void arithmetic ( int com , int front , int behind , int im , MTC* cur , int* data ) ;

	/* �߼�����ָ�� -- 6 7 8 ---------------------------- Line : 479 ~ 525 */
		void logic      ( int com , int front , int behind , int im , MTC* cur , int* data ) ;

	/* �Ƚ�ָ�� ------ 9 -------------------------------- Line : 538 ~ 570 */
		void compare    ( int com , int front , int behind , int im , MTC* cur , int* data ) ;

	/* ��תָ�� ------ 10 ------------------------------- Line : 573 ~ 600 */
		void skip       ( int com , int front , int behind , int im , MTC* cur , int* data ) ;

	/* �������ָ�� -- 11 12 ---------------------------- Line : 603 ~ 629 */
		void putIn_Out  ( int com , int front , int behind , int im , MTC* cur , int* data ) ;


/* 4.44 ��������ܼ����� */
 
	/* ����Ĵ���״̬ ----------------------------------- Line : 632 ~ 681 */
		void printState ( int com , MTC* cur , int* code , int* data ) ;
	


/**   �� �� ��   ***************************************************************************************************************/


int main( int argc , char* argv[] )
{
	/* ���ļ� */ 
		FILE *fPtr =  fopen ( "dict.dic" , "r" ) ;
	
	/* �������� ( ͣ��ָ��������ڵ��ı����� ) */
		int line = countLine ( fPtr ) ;
	
	/* �ռ����ɺͳ�ʼ�� -- ���� malloc���� */
		int** txt = generateTxtSpace ( fPtr , line ) ;  // �ı��ݴ�ռ� ���� �� ��������
		int* code = generateCodeSpace ( txt , line ) ;  // ����ģ��ռ� ���� �� ��������
		int* data = generateDataSpace (  ) ;            // ����ģ��ռ� ���� �� ��ʼ��
		MTC* cur = generateStruct (  ) ;                // �Ĵ����ṹ�� ���� �� ��ʼ��
	
	/* �в������� */	
		int com      = 0 ;  // 01 ~ 08 λתʮ���� 
		int front          = 0 ;  // 09 ~ 12 λתʮ���� 
		int behind         = 0 ;  // 13 ~ 16 λתʮ���� 
		int im = 0 ;  // 17 ~ 32 λתʮ���� 
	
	/* ִ�� */
		int i , j ; // ��̬�±�
		for ( i = 0 ; i < line ; ) {
			
			for ( com = 0 ,j = 0 ; j < 8 ; j++ )        // ���� command          ( ����ʮ������ )
				com = 2*com + txt[i][j] ;
			
			for ( front = 0 ; j < 12 ; j++ )            // ���� front    ( ǰһ���Ĵ���ʮ������ )
				front = 2*front + txt[i][j] ;
			
			for ( behind = 0 ; j < 16 ; j++ )           // ���� behind   ( ��һ���Ĵ���ʮ������ )
				behind = 2*behind + txt[i][j] ;
			
			im = countImmediateValue ( txt , i , j ) ;  // ���� immediateValue ( ������ʮ������ )
			
			/* ѡ��ָ����� */
			switch ( com ) {
			 	
				case 0  : stop       ( com , front , behind , im , cur , data ) ; break ; // ͣ��     -- 0 
				case 1  : transport  ( com , front , behind , im , cur , data ) ; break ; // ���ݴ��� -- 1
				case 2  : arithmetic ( com , front , behind , im , cur , data ) ; break ; // ���� '+' -- 2
				case 3  : arithmetic ( com , front , behind , im , cur , data ) ; break ; // ���� '-' -- 3
				case 4  : arithmetic ( com , front , behind , im , cur , data ) ; break ; // ���� '*' -- 4
				case 5  : arithmetic ( com , front , behind , im , cur , data ) ; break ; // ���� '/' -- 5
				case 6  : logic      ( com , front , behind , im , cur , data ) ; break ; // �߼� '&&'-- 6 
				case 7  : logic      ( com , front , behind , im , cur , data ) ; break ; // �߼� '||'-- 7
				case 8  : logic      ( com , front , behind , im , cur , data ) ; break ; // �߼� '!' -- 8
				case 9  : compare    ( com , front , behind , im , cur , data ) ; break ; // �Ƚ�     -- 9
				case 10 : skip       ( com , front , behind , im , cur , data ) ; break ; // ��ת     --10
				case 11 : putIn_Out  ( com , front , behind , im , cur , data ) ; break ; // ���� in  --11
				case 12 : putIn_Out  ( com , front , behind , im , cur , data ) ; break ; // ��� out --12
			}
			
			i = cur->ip/4 ; // �������±� i 
			
			printState ( com , cur , code , data ) ; // ����Ĵ���״̬ 
		}
		
		Free ( txt , line , cur , code , data ) ; // �ͷ�����ռ� 
	
	
	return 0 ;
}



/**   �� �� ��   ****************************************************************************************************************/


/* txt : �ı��ݴ�ռ� ���� �� �������� */
	int** generateTxtSpace ( FILE* fPtr , int line )
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
	int* generateCodeSpace ( int** txt , int line )
	{
		
		/* �������ռ� */
			int* code = ( int* ) malloc ( sizeof ( int ) * 128 ) ;
			
			int i , j ;
			for ( i = 0 ; i < 128 ; i++ ) // ��ʼ��
				code[i] = 0 ;
		
		/* ���� code ���� */
			for ( i = 0 ; i < line ; i++ )
				for ( j = 0 ; j < 32 ; j++ )
					code[ i ] = 2*code[i] + txt[i][j] ;
		
			
		return code ;
	}


/* data : ����ģ��ռ� ���� �� ��ʼ�� */
	int* generateDataSpace (  )
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
	MTC* generateStruct (  )
	{
		
		/* ����ṹ��ռ� */
		MTC* cur = ( MTC* ) malloc ( sizeof ( MTC ) ) ;
		
		/* ��ʼ�� */
			cur->ip = 0 ;
			cur->flag = 0 ;
			cur->ir = 0 ;
			
			cur->ax = ( int* ) malloc ( sizeof ( int ) * 9 ) ;
			int  i ;
			for ( i = 0 ; i < 9 ; i++ )
				cur->ax[i] = 0 ;
		
		
		return cur ;
	}


/* �ͷ�����ռ� */
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
	void stop ( int com , int front , int behind , int im , MTC* cur , int* data )
	{
		
		/* ���� ir */ 
			cur->ir = 256*com + 16*front + behind ;
		
		
		/* ������ */
			cur->ip = cur->ip + 4 ;
		
		
		return ;
	}
	
	
/* ���ݴ���ָ�� -- 1 */
	void transport ( int com , int front , int behind , int im , MTC* cur , int* data )
	{
		
		/* ���� ir */ 
			cur->ir = 256*com + 16*front + behind ;
		
		
		/* ���ִ���ָ�� */ 
			if ( behind == 0 )
				cur->ax[front] = im ;
			
			else if ( behind >= 1 && behind <= 4 )
				data[ ( cur->ax[front] - 16384 ) / 2 ] = cur->ax[behind] ;
			
			else if ( behind >= 5 && behind <= 8 )
				cur->ax[front] = data[ ( cur->ax[behind] - 16384 ) / 2 ] ;
		
			
		/* ������ */
			cur->ip = cur->ip + 4 ;
		
		
		return ;
	}


/* ��������ָ�� -- 2 3 4 5 */
	void arithmetic ( int com , int front , int behind , int im , MTC* cur , int* data )
	{
		
		/* ���� ir */ 
			cur->ir = 256*com + 16*front + behind ;
		
		
		/* ��������� */
			
			// + ���� 
			if ( com == 2 ) {
				
				if ( behind == 0 )
					cur->ax[front] = cur->ax[front] + im ;
				
				else
					cur->ax[front] = cur->ax[front] + data[ ( cur->ax[behind] - 16384 ) / 2 ] ;
					
			}
			
			// - ���� 
			if ( com == 3 ) {
				
				if ( behind == 0 )
					cur->ax[front] = cur->ax[front] - im ;
				
				else
					cur->ax[front] = cur->ax[front] - data[ ( cur->ax[behind] - 16384 ) / 2 ] ;
					
			}
			
			// * ���� 
			if ( com == 4 ) {
				
				if ( behind == 0 )
					cur->ax[front] = cur->ax[front] * im ;
				
				else
					cur->ax[front] = cur->ax[front] * data[ ( cur->ax[behind] - 16384 ) / 2 ] ;
					
			}
			
			// - ���� 
			if ( com == 5 ) {
				
				if ( behind == 0 )
					cur->ax[front] = cur->ax[front] / im ;
				
				else
					cur->ax[front] = cur->ax[front] / data[ ( cur->ax[behind] - 16384 ) / 2 ] ;
					
			}
		
		
		/* ������ */
			cur->ip = cur->ip + 4 ;
		
		
		return ;
	}


/* �߼�����ָ�� -- 6 7 8 */
	void logic ( int com , int front , int behind , int im , MTC* cur , int* data )
	{
		
		/* ���� ir */ 
			cur->ir = 256*com + 16*front + behind ;
		
		
		/* ��������� */
		
			// && ���� 
			if ( com == 6 ) {
				
				if ( behind == 0 )
					cur->ax[front] = cur->ax[front] && im ;
				
				else
					cur->ax[front] = cur->ax[front] && data[ ( cur->ax[behind] - 16384 ) / 2 ] ;
			}
			
			// || ���� 
			if ( com == 7 ) {
				
				if ( behind == 0 )
					cur->ax[front] = cur->ax[front] || im ;
				
				else
					cur->ax[front] = cur->ax[front] || data[ ( cur->ax[behind] - 16384 ) / 2 ] ;
			}
			
			// ! ���� 
			if ( com == 8 ) {
				
				if ( behind == 0 )
					cur->ax[front] = ! cur->ax[front] ;
				
				else
					data[ ( cur->ax[behind] - 16384 ) / 2 ] = ! data[ ( cur->ax[behind] - 16384 ) / 2 ] ;
			}
		
		
		/* ������ */
			cur->ip = cur->ip + 4 ;
		
		
		return ;
	}


/* �Ƚ�ָ�� -- 9 */
	void compare ( int com , int front , int behind , int im , MTC* cur , int* data )
	{
		
		/* ���� ir */ 
			cur->ir = 256*com + 16*front + behind ;
		
		
		/* �Ĵ��� 1 2 3 4 �� ���������Ƚ� */
			if ( behind == 0 ) {
				
				if ( cur->ax[front] > im )
					cur->flag = 1 ;
				
				else if ( cur->ax[front] == im )
					cur->flag = 0 ;
				
				else if ( cur->ax[front] < im )
					cur->flag = -1 ;
					
			}
		
		/* �Ĵ��� 1 2 3 4 �� �Ĵ��� 5 6 7 8 ���Ƚ� */
			else {
				
				if ( cur->ax[front] > data[ ( cur->ax[behind] - 16384 ) / 2 ] )
					cur->flag = 1 ;
				
				else if ( cur->ax[front] == data[ ( cur->ax[behind] - 16384 ) / 2 ] )
					cur->flag = 0 ;
				
				else if ( cur->ax[front] < data[ ( cur->ax[behind] - 16384 ) / 2 ] )
					cur->flag = -1 ;
					
			}
		
		
		/* ������ */
			cur->ip = cur->ip + 4 ;
		
		
		return ;
	}


/* ��תָ�� -- 10 */
	void skip ( int com , int front , int behind , int im , MTC* cur , int* data )
	{
		
		/* ���� ir */ 
			cur->ir = 256*com + 16*front + behind ;
		
		
		/* ������תָ�� */
			if ( behind == 0 )
				cur->ip = cur->ip + im ;
			
			else if ( behind == 1 && cur->flag == 0 )
				cur->ip = cur->ip + im ;
			
			else if ( behind == 2 && cur->flag == 1 )
				cur->ip = cur->ip + im ;
			
			else if ( behind == 3 && cur->flag == -1 )
				cur->ip = cur->ip + im ;
		
		/* ����ת */
			else
				cur->ip = cur->ip + 4 ;
		
		
		return ;
	}


/* �������ָ�� -- 11 12 */
	void putIn_Out ( int com , int front , int behind , int im , MTC* cur , int* data )
	{
		
		/* ���� ir */ 
			cur->ir = 256*com + 16*front + behind ;
		
		
		/* ���� */
			if ( com == 11 ) {
				
				printf ( "in:\n" ) ;
				scanf ( "%d" , & cur->ax[front] ) ;
			}
		
		/* ��� */
			else if ( com == 12 ) {
				
				printf ( "out: %d\n" , cur->ax[front] ) ;
			}
		
		/* ������ */
			cur->ip = cur->ip + 4 ;
		
		
		return ;
	}


/* ����Ĵ���״̬ */
	void printState ( int com , MTC* cur , int* code , int* data )
	{
		
		/* ��ӡ�Ĵ���״̬ */ 
			printf ( "ip = %d\n" , cur->ip ) ;
			printf ( "flag = %d\n" , cur->flag ) ;
			printf ( "ir = %d\n" , cur->ir ) ;
			printf ( "ax1 = %d ax2 = %d ax3 = %d ax4 = %d\n" , cur->ax[1] , cur->ax[2] , cur->ax[3] , cur->ax[4] ) ;
			printf ( "ax5 = %d ax6 = %d ax7 = %d ax8 = %d\n" , cur->ax[5] , cur->ax[6] , cur->ax[7] , cur->ax[8] ) ;
		
		
		/* ͣ��ָ�� , ��ӡ code �� data */
			if ( com == 0 ) {
				
				int i ; // ��̬�±�
				
			/* ��ӡ code */ 
				printf ( "\ncodeSegment :\n" ) ;
				 
				for ( i = 0 ; i < 128 ; i++ ) {
					
					printf ( "%d" , code[i] ) ;
					
					if ( ( i+1 ) % 8 == 0 )
						printf ( "\n" ) ;
					else
						printf ( " " ) ;
					
				}
			
			/* ��ӡ code */
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
