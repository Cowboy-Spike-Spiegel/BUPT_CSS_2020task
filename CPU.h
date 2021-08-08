#include<windows.h>
#include<process.h>
#include<stdio.h>   
#include<stdlib.h>
#include<time.h> 


/* 0.00 寄存器定义 ---------------------------------------------------- */

	/* 定义寄存器结构体 struct -- segment */
		typedef struct Segment
		{
			HANDLE hMutex ;     //  进程互斥对象 
			int lock ;          //  同步锁 
			int end1 ;          //  线程 1 结束信号 
			int end2 ;          //  线程 2 结束信号 
			int** txt1 ;        //  代码 1
			int line1 ;         //  代码 1 行数 
			int** txt2 ;        //  代码 2 
			int line2 ;         //  代码 2 行数 
			int* codeSegment ;  //  代码寄存器 
			int* dataSegment ;  //  数据寄存器 
		}  segment ;
	
 
	/* 定义寄存器结构体 struct -- MTC */
		typedef struct mtc
		{
			int id ;    // 核心 ID 
			int ip ;    // 程序计数器 
			int flag ;  // 标志寄存器 
			int ir ;    // 命令寄存器 
			int* ax ;   // 寄存器, 0号位舍弃,便于引用
		}  MTC ;
 

	/* 定义行参数数据结构体 */
		typedef struct Number
		{
			int com ;    // 01 ~ 08 位转十进制 
			int front ;  // 09 ~ 12 位转十进制 
			int behind ; // 13 ~ 16 位转十进制 
			int im ;     // 17 ~ 32 位转十进制 
		}  number ; 

	
/* 1.11 空间生成 (malloc) 并 存入数据/初始化 以及释放(free) ----------- */

	/* txt   : 文本暂存空间 生成 并 存入数据 */
	int** generateTxt ( FILE* fPtr , int line ) ;

	/* code  : 代码模拟空间 生成 并 存入数据 */
	int* generateCode ( int** txt1 , int** txt2 , int line1 , int line2 ) ;

	/* data  : 数据模拟空间 生成 并 初始化 */
	int* generateData (  ) ;

	/* struct: 寄存器结构体 生成 并 初始化 */
	MTC* generateStruct ( int id ) ;
	
	/* main : 释放申请空间 */
	void mainFree ( segment* solid ) ;


/* 2.22 计算外包函数 ( 过程复杂 ) ------------------------------------- */

	/* 计算行数 */
	int countLine ( FILE* fPtr ) ;

	/* 计算立即数 */ 
	int countImmediateValue ( int** txt , int c , int l ) ;


/* 3.33 线程函数 ------------------------------------------------------ */ 

	unsigned __stdcall Fun1Proc ( void* pArguments ) ;
	unsigned __stdcall Fun2Proc ( void* pArguments ) ;


/* 4.44 指令集函数 ---------------------------------------------------- */
 
	/* 停机指令 ------ 0 */
	void stop       ( number* num , MTC* cur , int* data ) ;

	/* 数据传送指令 -- 1 */
	void transport  ( number* num , MTC* cur , int* data ) ;

	/* 算术运算指令 -- 2 3 4 5 */
	void arithmetic ( number* num , MTC* cur , int* data ) ;

	/* 逻辑运算指令 -- 6 7 8 */
	void logic      ( number* num , MTC* cur , int* data ) ;

	/* 比较指令 ------ 9 */
	void compare    ( number* num , MTC* cur , int* data ) ;

	/* 跳转指令 ------ 10 */
	void skip       ( number* num , MTC* cur , int* data ) ;

	/* 输入输出指令 -- 11 12 */
	void putIn_Out  ( number* num , MTC* cur , int* data ) ;
	
	/* 上锁 ---------- 13 */
	void lock       ( number* num , MTC* cur , segment* solid ) ;
	
	/* 释放 ---------- 14 */
	void release    ( number* num , MTC* cur , segment* solid ) ;
	 
	/* 函数睡眠 ------ 15 */ 
	void funcSleep  ( number* num , MTC* cur ) ;
	

/* 5.55 窗口输出总集函数 ---------------------------------------------- */
 
	/* func : 输出寄存器状态 */
	void funcPrint ( int com , MTC* cur , segment* solid ) ;
	
	/* main : 输出 dataSegment 和 codeSegment */
	void mainPrint ( segment* solid ) ;  
