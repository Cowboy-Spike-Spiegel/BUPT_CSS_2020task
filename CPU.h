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
