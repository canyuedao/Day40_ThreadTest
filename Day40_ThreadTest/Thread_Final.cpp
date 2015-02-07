// Thread.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include "windows.h"
#define SELLER_COUNTER 10				// 售票员数量
#define TICKET_COUNTER 100000			// 售票总数

HANDLE ahThreadHandle[SELLER_COUNTER];	// 卖票线程句柄数组
int iNumber = 0;						// 票号
int iCount = 0;							// 卖票张数统计，用于检验线程同步出错
CRITICAL_SECTION cs;					// 临界区

int aiTichetState[TICKET_COUNTER]={};	// 数组元素的值表示该票的卖出计数，用于统计重复票

// 线程多个返回数据组成的结构体
struct ThreadTotalData	
{
	int iThisThreadTicketRepeatTotal;		// 本线程重复票数
	int iThisThreadTicketBackTotal;			// 本线程回退票数
};

//***********************************************************
// 功能：线程函数
// 输入：线程编号
// 输出：线程多个返回数据组成结构体的指针
//************************************************************
DWORD WINAPI threadFunction(LPVOID pParamary)
{
	ThreadTotalData *pThreadTotalData = new ThreadTotalData();	// 定义线程多个返回数据组成结构体的实例对象并初始化，把它的地址赋到pThreadTotalData指针
	//EnterCriticalSection(&cs);	// 只会调用一次，主要用于对应进入循环的第一次LeaveCriticalSection
	while(iNumber<TICKET_COUNTER)
	{
		// iNumber读和加一必须在临界区依赖完成,因为中间一旦有其他线程插入并读会造成同一张票卖多次
		int iBackTicket=0;			// 本次循环卖票回退票总数
		int iCurrentNumber= iNumber;// 为了保持当前要卖的票号不被其他线程偷改,缓存到局部变量
		//iNumber++;				// 以下是iNumber++的汇编实现
		{
			__asm mov eax,dword ptr [iCurrentNumber];
			__asm push eax;	// 为了防止Sleep(0)修改eax把它缓存到局部变量
			Sleep(0);				// 为了放大票号回退的出现概率,此处人为制造一次必然的线程切出
			if (iCurrentNumber<iNumber)iBackTicket=iNumber- iCurrentNumber;	// 计算回退票总数,0表示没有回退
			__asm pop eax;
			__asm	add         eax,1; 
			__asm	mov         dword ptr [iNumber],eax;
		}
		//LeaveCriticalSection(&cs);

		char pszTicketInformation[256] ={};	// 字符串缓冲区,用于存放组装后的字符串
		int iIndex = sprintf(pszTicketInformation,"我是第%d号售票员卖第%d张票",(int)pParamary,iCurrentNumber);	// iIndex用于指定字符串后续写入的位置偏移字节数

		if (aiTichetState[iCurrentNumber])	// 如果当前票的卖出计数已经不为零,这次肯定要重复卖了
		{
			iIndex += sprintf(pszTicketInformation+iIndex,",此票已经重复第%d次",aiTichetState[iCurrentNumber]);
			pThreadTotalData->iThisThreadTicketRepeatTotal++;	// 本线程重复票总数加一
		} 
		aiTichetState[iCurrentNumber]++;	// 本票卖出次数加一
		
		if (iBackTicket)	// 回退票数不为0时
		{
			iIndex += sprintf(pszTicketInformation+iIndex,",此处回退了%d张票", iBackTicket);
			pThreadTotalData->iThisThreadTicketBackTotal += iBackTicket;	// 本线程回退票总数加一
		}

		iIndex = sprintf(pszTicketInformation+iIndex,"\n");	// 字符串最后要有换行
		printf("%s",pszTicketInformation);	// 打印组装好的字符串
		
		//EnterCriticalSection(&cs);
		iCount++;						// 此临界区用于保证卖票计数不出错(主要用于防止回退)
		//LeaveCriticalSection(&cs);

		//EnterCriticalSection(&cs);		// 进入本线程最重要的临界区,iNumber<TICKET_COUNTER判断一定要和iNumber++在临界区依赖,否则会造成卖最后一张票时重复而多卖
	}
	
	//LeaveCriticalSection(&cs);		// 只会调用一次,用于对应循环退出前最后一次的EnterCriticalSection;
	return (DWORD)pThreadTotalData;	// 返回该线程多个返回值组成的结构体的指针
}

int _tmain(int argc, _TCHAR* argv[])
{
	int iTicketBackTotal=0;
	int iTicketRepeatTotal=0;
	int iTicketSkipTotal = 0;
	//InitializeCriticalSection(&cs);
	DWORD dThreadID;
	// 生成多个线程并保存他们的线程句柄 
	for (int i = 0;i<SELLER_COUNTER;i++)
	{
		ahThreadHandle[i] =  CreateThread(0,0,threadFunction,(LPVOID*)i,0,&dThreadID);
	}
	// 死等,直到所有线程退出(当卖票线程多于64时会出错)
	WaitForMultipleObjects(SELLER_COUNTER,ahThreadHandle,true,INFINITE);
	// 收集汇总每个线程的返回数据
	for (int i=0; i<SELLER_COUNTER; i++)
	{
		DWORD dResult;	// 用于存放线程返回值
		GetExitCodeThread(ahThreadHandle[i],&dResult);							// 线程返回值放到dResult
		ThreadTotalData * pThreadTotalData = (ThreadTotalData *)dResult;		// 强转为该线程多个返回值组成的结构体的指针
		iTicketRepeatTotal += pThreadTotalData->iThisThreadTicketRepeatTotal;	// 汇总每个线程的重复票总数
		iTicketBackTotal += pThreadTotalData->iThisThreadTicketBackTotal;		// 汇总每个线程的回退票总数
		delete pThreadTotalData;												// 一定要释放堆内存空间
	}
	// 统计漏票总数
	for (int i=0;i<TICKET_COUNTER;i++)
	{
		if(!aiTichetState[i]) iTicketSkipTotal++;
	}

	printf("回退的票总计为：%d  ",iTicketBackTotal);
	printf("卖出的票总计为：%d  ",iCount);
	printf("重复的票总计为：%d  ",iTicketRepeatTotal);
	printf("漏的票总计为：%d  ",iTicketSkipTotal);

	getchar();	// 回车退出程序
	return 0;
}

