#include"CPU.h"

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
