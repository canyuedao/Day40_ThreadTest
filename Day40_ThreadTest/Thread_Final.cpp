// Thread.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include "stdafx.h"
#include "windows.h"
#define SELLER_COUNTER 10				// ��ƱԱ����
#define TICKET_COUNTER 100000			// ��Ʊ����

HANDLE ahThreadHandle[SELLER_COUNTER];	// ��Ʊ�߳̾������
int iNumber = 0;						// Ʊ��
int iCount = 0;							// ��Ʊ����ͳ�ƣ����ڼ����߳�ͬ������
CRITICAL_SECTION cs;					// �ٽ���

int aiTichetState[TICKET_COUNTER]={};	// ����Ԫ�ص�ֵ��ʾ��Ʊ����������������ͳ���ظ�Ʊ

// �̶߳������������ɵĽṹ��
struct ThreadTotalData	
{
	int iThisThreadTicketRepeatTotal;		// ���߳��ظ�Ʊ��
	int iThisThreadTicketBackTotal;			// ���̻߳���Ʊ��
};

//***********************************************************
// ���ܣ��̺߳���
// ���룺�̱߳��
// ������̶߳������������ɽṹ���ָ��
//************************************************************
DWORD WINAPI threadFunction(LPVOID pParamary)
{
	ThreadTotalData *pThreadTotalData = new ThreadTotalData();	// �����̶߳������������ɽṹ���ʵ�����󲢳�ʼ���������ĵ�ַ����pThreadTotalDataָ��
	//EnterCriticalSection(&cs);	// ֻ�����һ�Σ���Ҫ���ڶ�Ӧ����ѭ���ĵ�һ��LeaveCriticalSection
	while(iNumber<TICKET_COUNTER)
	{
		// iNumber���ͼ�һ�������ٽ����������,��Ϊ�м�һ���������̲߳��벢�������ͬһ��Ʊ�����
		int iBackTicket=0;			// ����ѭ����Ʊ����Ʊ����
		int iCurrentNumber= iNumber;// Ϊ�˱��ֵ�ǰҪ����Ʊ�Ų��������߳�͵��,���浽�ֲ�����
		//iNumber++;				// ������iNumber++�Ļ��ʵ��
		{
			__asm mov eax,dword ptr [iCurrentNumber];
			__asm push eax;	// Ϊ�˷�ֹSleep(0)�޸�eax�������浽�ֲ�����
			Sleep(0);				// Ϊ�˷Ŵ�Ʊ�Ż��˵ĳ��ָ���,�˴���Ϊ����һ�α�Ȼ���߳��г�
			if (iCurrentNumber<iNumber)iBackTicket=iNumber- iCurrentNumber;	// �������Ʊ����,0��ʾû�л���
			__asm pop eax;
			__asm	add         eax,1; 
			__asm	mov         dword ptr [iNumber],eax;
		}
		//LeaveCriticalSection(&cs);

		char pszTicketInformation[256] ={};	// �ַ���������,���ڴ����װ����ַ���
		int iIndex = sprintf(pszTicketInformation,"���ǵ�%d����ƱԱ����%d��Ʊ",(int)pParamary,iCurrentNumber);	// iIndex����ָ���ַ�������д���λ��ƫ���ֽ���

		if (aiTichetState[iCurrentNumber])	// �����ǰƱ�����������Ѿ���Ϊ��,��ο϶�Ҫ�ظ�����
		{
			iIndex += sprintf(pszTicketInformation+iIndex,",��Ʊ�Ѿ��ظ���%d��",aiTichetState[iCurrentNumber]);
			pThreadTotalData->iThisThreadTicketRepeatTotal++;	// ���߳��ظ�Ʊ������һ
		} 
		aiTichetState[iCurrentNumber]++;	// ��Ʊ����������һ
		
		if (iBackTicket)	// ����Ʊ����Ϊ0ʱ
		{
			iIndex += sprintf(pszTicketInformation+iIndex,",�˴�������%d��Ʊ", iBackTicket);
			pThreadTotalData->iThisThreadTicketBackTotal += iBackTicket;	// ���̻߳���Ʊ������һ
		}

		iIndex = sprintf(pszTicketInformation+iIndex,"\n");	// �ַ������Ҫ�л���
		printf("%s",pszTicketInformation);	// ��ӡ��װ�õ��ַ���
		
		//EnterCriticalSection(&cs);
		iCount++;						// ���ٽ������ڱ�֤��Ʊ����������(��Ҫ���ڷ�ֹ����)
		//LeaveCriticalSection(&cs);

		//EnterCriticalSection(&cs);		// ���뱾�߳�����Ҫ���ٽ���,iNumber<TICKET_COUNTER�ж�һ��Ҫ��iNumber++���ٽ�������,�������������һ��Ʊʱ�ظ�������
	}
	
	//LeaveCriticalSection(&cs);		// ֻ�����һ��,���ڶ�Ӧѭ���˳�ǰ���һ�ε�EnterCriticalSection;
	return (DWORD)pThreadTotalData;	// ���ظ��̶߳������ֵ��ɵĽṹ���ָ��
}

int _tmain(int argc, _TCHAR* argv[])
{
	int iTicketBackTotal=0;
	int iTicketRepeatTotal=0;
	int iTicketSkipTotal = 0;
	//InitializeCriticalSection(&cs);
	DWORD dThreadID;
	// ���ɶ���̲߳��������ǵ��߳̾�� 
	for (int i = 0;i<SELLER_COUNTER;i++)
	{
		ahThreadHandle[i] =  CreateThread(0,0,threadFunction,(LPVOID*)i,0,&dThreadID);
	}
	// ����,ֱ�������߳��˳�(����Ʊ�̶߳���64ʱ�����)
	WaitForMultipleObjects(SELLER_COUNTER,ahThreadHandle,true,INFINITE);
	// �ռ�����ÿ���̵߳ķ�������
	for (int i=0; i<SELLER_COUNTER; i++)
	{
		DWORD dResult;	// ���ڴ���̷߳���ֵ
		GetExitCodeThread(ahThreadHandle[i],&dResult);							// �̷߳���ֵ�ŵ�dResult
		ThreadTotalData * pThreadTotalData = (ThreadTotalData *)dResult;		// ǿתΪ���̶߳������ֵ��ɵĽṹ���ָ��
		iTicketRepeatTotal += pThreadTotalData->iThisThreadTicketRepeatTotal;	// ����ÿ���̵߳��ظ�Ʊ����
		iTicketBackTotal += pThreadTotalData->iThisThreadTicketBackTotal;		// ����ÿ���̵߳Ļ���Ʊ����
		delete pThreadTotalData;												// һ��Ҫ�ͷŶ��ڴ�ռ�
	}
	// ͳ��©Ʊ����
	for (int i=0;i<TICKET_COUNTER;i++)
	{
		if(!aiTichetState[i]) iTicketSkipTotal++;
	}

	printf("���˵�Ʊ�ܼ�Ϊ��%d  ",iTicketBackTotal);
	printf("������Ʊ�ܼ�Ϊ��%d  ",iCount);
	printf("�ظ���Ʊ�ܼ�Ϊ��%d  ",iTicketRepeatTotal);
	printf("©��Ʊ�ܼ�Ϊ��%d  ",iTicketSkipTotal);

	getchar();	// �س��˳�����
	return 0;
}

