#include<stdio.h>
#include<stdlib.h>


/* ȫ�ֱ������岢��ʼ�� *************************** */
	
	/* ��������� */
	int ip = 0 ;
	
	/* ��־�Ĵ��� */
	int flag = 0 ;
	
	/* ���ƼĴ��� */
	int ir = 0 ;
	
	/* ���ݼĴ��� 1 , 2 , 3 , 4 �͵�ַ�Ĵ��� 5 , 6 , 7 , 8 */
	int ax[9] = {0} ;  // 0��λ����,��������
	
	/* ���봢��ռ�--ģ�� */
	int* codeSegment = NULL ;
	
	/* ���ݴ���ռ�--ģ�� */
	int* dataSegment = NULL ;
	

/* �ݴ�������岢��ʼ�� *************************** */
	
	
	/* ��ȡ���ַ����� */ 
	char ch = 0 ;
	
	/* ָ������--��Ӧ������ */
	int command[8] = {0} ;
	
	/* ǰһ���Ĵ���--��Ӧ�������� */
	int registerFront[4] = {0} ;
	int front = 0 ; // ʮ����ת�� 
	
	/* ��һ���Ĵ���--��Ӧ�������� */
	int registerBehind[4] = {0} ;
	int behind = 0 ; // ʮ����ת�� 
	
	/* ������:ֻ�赱��ʮ���ƴ洢 */
	int immediateNumber[16] = {0} ;
	int immediateValue = 0 ;



/* �������� ************************************** */
	
	
	/* �������ռ� */
	void applyCodeSpace (  ) ;
	
	/* �������ݿռ� */
	void applyDataSpace (  ) ;
	
	/* ���ɴ��봢��ռ� */
	void generateCode ( FILE* fPtr ) ; 
	
	/* ��ȡָ������ */
	int scanCommand ( FILE* fPtr ) ;
	
	/* ��ȡǰһ���Ĵ���״̬ */
	int scanFront ( FILE* fPtr ) ;
	
	/* ��ȡ��һ���Ĵ���״̬ */
	int scanBehind ( FILE* fPtr ) ;
	
	/* ��ȡ������ */ 
	int scanImmediateValue ( FILE* fPtr ) ;
	
	/* ��ȡ���� */
	void scan ( FILE* fPtr , int com ) ;
	
	/* ͣ��ָ�� -- 0 */
	void stop ( FILE* fPtr , int com ) ;
	
	/* ���ݴ���ָ�� -- 1 */
	void transport ( FILE* fPtr , int com ) ;
	
	/* ��������ָ�� -- 2 3 4 5 */
	void arithmetic ( FILE* fPtr , int com ) ;
	
	/* �߼�����ָ�� -- 6 7 8 */
	void logic ( FILE* fPtr , int com ) ;
	
	/* �Ƚ�ָ�� -- 9 */
	void compare ( FILE* fPtr , int com ) ;
	
	/* ��תָ�� -- 10 */
	void skip ( FILE* fPtr , int com ) ;
	
	/* �������ָ�� -- 11 12 */
	void putIn_Out ( FILE* fPtr , int com ) ;
	
	/* �ݴ��������� */
	void reset (  ) ;
	
	/* ����Ĵ���״̬ */
	void printState ( int Flag ) ;


/**********************************************************************/


/* ���������� */

int main( int argc, char *argv[] )
{
	
	/* ���ļ� */ 
	FILE *fPtr =  fopen ( "dict.dic" , "r" ) ;
	
	/* �����������ݿռ� */
	applyCodeSpace (  ) ;
	applyDataSpace (  ) ;
	
	/* ���� Code */
	generateCode ( fPtr ) ;
	
	/* ѭ�������� -- different from 'flag' */
	int Flag = 1 ;
	
	/* ѭ������ */
	while ( Flag )	{
		
		/* ��ȡָ������ */
		int com = scanCommand ( fPtr ) ;
		
		/* ƥ��ָ����� */
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
			
		/* ����Ĵ���״̬ */
		printState ( Flag ) ;
			
	}
	
	free ( codeSegment ) ;
	free ( dataSegment ) ;
	
	/* �ر��ļ� */
	fclose ( fPtr ) ;
	
	
	return 0 ;
}


/***********************************************************************/


/* �Ӻ��� */


/* �������ռ� */
	void applyCodeSpace (  )
	{
		
		codeSegment = ( int* ) malloc ( sizeof ( int ) * 128 ) ;
		
		int i ;
		for ( i = 0 ; i < 128 ; i++ ) // ��ʼ�� 
			codeSegment[i] = 0 ;
		
		
		return ;
	}


/* �������ݿռ� */
	void applyDataSpace (  )
	{
		
		dataSegment = ( int* ) malloc ( sizeof ( int ) * 256 ) ;
		
		int i ;
		for ( i = 0 ; i < 256 ; i++ ) // ��ʼ�� 
			dataSegment[i] = 0 ;
		
		
		return ;
	}

/* ������� code */
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


/* ��ȡָ������ */
	int scanCommand ( FILE* fPtr )
	{
		
		/* ��̬�±� */
		int i ;
		
		/* ���� */ 
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
	
	
/* ��ȡǰһ���Ĵ���״̬ */
	int scanFront ( FILE* fPtr )
	{
		
		/* ��̬�±� */
		int i ;
		
		/* ǰһ���Ĵ��� */ 
		int fr ;
		
		for ( fr = 0 , i = 0 ; i < 4 ; i++ ) {
			
			ch = fgetc ( fPtr ) ;
			registerFront[i] = ch - 48 ;
			
			/* ����Ĵ���ʮ���ƺ��� */
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


/* ��ȡ��һ���Ĵ���״̬ */
	int scanBehind ( FILE* fPtr )
	{
		
		/* ��̬�±� */
		int i ;
		
		/* ��һ���Ĵ��� */ 
		int be ;
		 
		for ( be = 0 , i = 0 ; i < 4 ; i++ ) {
			
			ch = fgetc ( fPtr ) ;
			registerBehind[i] = ch - 48 ;
			
			/* ����Ĵ���ʮ���ƺ��� */ 
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


/* ��ȡ������ */ 
	int scanImmediateValue ( FILE* fPtr )
	{
		int im = 0 ; // ��������ʼ��
		int Flag = 1 ; //�������������
		int i ; // ��̬�±�
		
		/* ��ȡ������ */ 
		for ( i = 0 ; i < 16 ; i++ ) {
			
			ch = fgetc ( fPtr ) ;
			immediateNumber[i] = ch - 48 ;
			
		}
		
		/* ��������������ֵ */
		if ( immediateNumber[0] == 1 ) {
			
			Flag = 0 ; // ���Ϊ����
			
			/* copyԭ��������ı����� */
			int copy[16] = {0} ;
			for ( i = 0 ; i < 16 ; i++ )
				copy[i] = immediateNumber[i] ;
			
			/* ȡ�� */
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
			
			/* ��������������ֵ */
			for ( i = 1 ; i < 16 ; i++ )
				im = 2*im + copy[i] ;
		}
		
		else
			/* ���������� */
			for ( i = 1 ; i < 16 ; i++ )
				im = 2*im + immediateNumber[i] ;
		
		/* �����Ϊ����ȡ�� */ 
		if ( Flag == 0 )
			im = -im ;
		
		
		return im ; 
	}

	
/* ��ȡ */
	void scan ( FILE* fPtr , int com )
	{	
		
		/* ���� front */
		front = scanFront ( fPtr ) ;
		
		
		/* ���� behind */
		behind = scanBehind ( fPtr ) ;
		
		
		/* ���� ir */
		ir = 16*16*com + 16*front + behind ;
		
		
		/* ���� immediateValue */
		immediateValue =  scanImmediateValue ( fPtr ) ;
		
		
		/* �Ե�����ֱ�����з� */ 
		while ( ch != 10 && ch != 0 && com != 0 )
			ch = fgetc ( fPtr ) ;
		
		
		return ;
	}


/* ͣ��ָ�� -- 0 */
	void stop ( FILE* fPtr , int com )
	{
		
		/* ��ȡ��ʣ������ */
		scan ( fPtr , com ) ;
		
		
		/* ������ */
		ip = ip + 4 ;
		
		
		/* �ݴ��������� */
		reset (  ) ;
		
		
		return ;
	}
	
	
/* ���ݴ���ָ�� -- 1 */
	void transport ( FILE* fPtr , int com )
	{
		
		/* ��ȡ��ʣ������ */
		scan ( fPtr , com ) ;
		
	
		/* ��������� */
		 
			/* ���������������ݼĴ��� */
			if ( behind == 0 )
				ax[front] = immediateValue ;
			
			/* �����ݼĴ������ݴ����ַ�Ĵ�����ָ��Ԫ */ 
			else if ( 0 < behind && behind <= 4 )
				dataSegment[ ( ax[front] - 16384 ) / 2 ] = ax[behind] ;
			
			/* ����ַ�Ĵ�����ָ�����ݴ������ݼĴ��� */
			else if ( behind > 4 )
				ax[front] = dataSegment[ ( ax[behind] - 16384 ) / 2 ] ;
		
		
		/* ������ */
		ip = ip + 4 ;
		
	
		/* �ݴ��������� */
		reset (  ) ; 
	
		
		return ;
	}
	
	
/* ��������ָ�� -- 2 3 4 5 */
	void arithmetic ( FILE* fPtr , int com )
	{
		
		/* ��ȡ��ʣ������ */
		scan ( fPtr , com ) ;
		
		
		/* ѡ���������� */
		
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
		
		
		/* ������ */
		ip = ip + 4 ;
		
		
		/* �ݴ��������� */
		reset (  ) ;
		
		
		return ;
	}
	
	
/* �߼�����ָ�� -- 6 7 8 */
	void logic ( FILE* fPtr , int com )
	{
		
		/* ��ȡ��ʣ������ */
		scan ( fPtr , com ) ;
		
		
		/* ѡ���������� */
			
			/* "�߼���" ���� */
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
		
		
		/* ������ */
		ip = ip + 4 ;
		
		
		/* �ݴ��������� */
		reset (  ) ;
		
		
		return ;
	}
	
	
/* �Ƚ�ָ�� -- 9 */
	void compare ( FILE* fPtr , int com )
	{
		
		/* ��ȡ��ʣ������ */
		scan ( fPtr , com ) ;
		
		
		/* �Ƚϴ�С */
		
			/* �Ĵ��� front���������ȴ�С */
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
		
		
		/* ������ */
		ip = ip + 4 ;
		
		
		/* �ݴ��������� */
		reset (  ) ;
		
		
		return ;
	}
	
	
/* ��תָ�� -- 10 */
	void skip ( FILE* fPtr , int com )
	{
		
		/* ��ȡ��ʣ������ */
		scan ( fPtr , com ) ;
		
		/* ��תָ�� */ 
		if ( behind == 0 || behind == 1 && flag == 0 || behind == 2 && flag == 1 || behind == 3 && flag == -1 ) {
			
			/* �޸� ip */
			ip = ip + immediateValue ;
			
			/* ���� */ 
			int cnt ;
			
			/* �޸�ָ�� */
			rewind ( fPtr ) ;
			for ( ch = 0 , cnt = 0 ; cnt < 8 * ( ip ) || ch != 10 ; ) {
			
				/* ��ȡ��ǰ�ַ�������ָ�� */ 
				ch = fgetc ( fPtr ) ; 
				
				/* ���˿ո�ͻ��� */ 
				if ( ch >= '0' && ch <= '9' )
					cnt++ ;
				
			}
			
		}
		
		
		/* �������� */ 
		else
			ip = ip + 4 ;
		
		
		/* �ݴ��������� */
		reset (  ) ;
		
		
		return ;
	}
	
	
/* �������ָ�� -- 11 12 */
	void putIn_Out ( FILE* fPtr , int com )
	{
		
		/* ��ȡ��ʣ������ */
		scan ( fPtr , com ) ;
		
		
		/* ����������� */
		
			/* ���� */
			if ( com == 11 ) {
				
				printf ( "in:\n" ) ;
				scanf ( "%d" , & ax[front] ) ;
			}
			
			else if ( com == 12 ) {
				
				printf ( "out: %d\n" , ax[front] ) ;
			}
		
		
		/* ������ */
		ip = ip + 4 ; 
		
		
		/* �ݴ��������� */
		reset (  ) ;
		
		
		return ;
	}
	
	
/* ����Ĵ���״̬ */
	void printState ( int Flag )
	{
		
		/* ��ӡ�Ĵ���״̬ */ 
		printf ( "ip = %d\n" , ip ) ;
		printf ( "flag = %d\n" , flag ) ;
		printf ( "ir = %d\n" , ir ) ;
		printf ( "ax1 = %d ax2 = %d ax3 = %d ax4 = %d\n" , ax[1] , ax[2] , ax[3] , ax[4] ) ;
		printf ( "ax5 = %d ax6 = %d ax7 = %d ax8 = %d\n" , ax[5] , ax[6] , ax[7] , ax[8] ) ;
		
		if ( Flag == 0 ) {
			
			int i ; // ��̬�±� 
			
			/* ��ӡ CodeSegment */ 
			printf ( "\ncodeSegment :\n" ) ;
			
			for ( i = 0 ; i < 128 ; i++ ) {
				
				printf ( "%d" , codeSegment[i] ) ;
				
				if ( ( i+1 ) % 8 == 0 )
					printf ( "\n" ) ;
				else
					printf ( " " ) ;
				
			}
			
			/* ��ӡ CodeSegment */
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
	
	
/* �ݴ��������� -- 1-12�����Ҫ���� */
	void reset (  )
	{
		
		/* ��̬�±� */
		int i = 0 ;
		
		/* ���� */ 
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
