// Day40_ThreadTest.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <Windows.h>
//#include <vector>
//#include <map>
//#include <winnt.h>
//using namespace std;
#define TICKET_NUM 1000
#define SOLED_OUT -111111
const int len=10;
CRITICAL_SECTION g_csThread;
//int *iresult=new int[TICKET_NUM+len-1];
int iresult[TICKET_NUM]={};		//���������ս������
int iindex;
//volatile long iindex=-1;
//vector<int> ivec;			//��̬���鷽��
int idistance=TICKET_NUM;
int iTicketNum=0;					//���������ս������

DWORD WINAPI ThreadFun(PVOID pPM){	
	
		while(iTicketNum<TICKET_NUM){
			EnterCriticalSection(&g_csThread);
			int itemp=iTicketNum;
			
			//���������ս������			
			if(iTicketNum<TICKET_NUM){	
				
				printf("%d������Ʊ��%d\n",(int)pPM,itemp);
				
				//iresult[itemp]++;				
				_asm  mov         eax,dword ptr [itemp]  			 
				_asm  inc		  dword ptr iresult[eax*4]
				//Sleep(1);												
				//iTicketNum++;
				_asm  inc		  dword ptr[iTicketNum];				
			}
			LeaveCriticalSection(&g_csThread);  		
			/*int itemp=idistance;		
			printf("%d������Ʊ��%d\n",i,itemp);					
			EnterCriticalSection(&g_csThread);
			iresult[iindex]=itemp;
			iindex++;*/
			//ivec.push_back(itemp);				
			//long iindextemp=InterlockedIncrement(&iindex);	
			//LeaveCriticalSection(&g_csThread);
			//idistance--;				
			
			Sleep(0);		
		}	
	
	return 0;
}
int _tmain(int argc, _TCHAR* argv[])
{		
	InitializeCriticalSection(&g_csThread);	
	HANDLE hThread[len];
	for(int i=0;i<len;i++){
		hThread[i]=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadFun,(int*)i,0,NULL);				
	}
	WaitForMultipleObjects(len,hThread,true,INFINITE);
	for(int i=0;i<len;i++){
		CloseHandle(hThread[i]);
	}	
	//���������ս������
	int iLeakTotal=0;
	int iRepeatTotal=0;
	printf("\n");
	for(int i=0;i<TICKET_NUM;i++){
		if(iresult[i]==0){	
			iLeakTotal++;
			printf("©ƱƱ�ţ�%d\n",i);
		}					
	}
	printf("\n");
	for(int i=0;i<TICKET_NUM;i++){
		if(iresult[i]>1){
			iRepeatTotal+=iresult[i]-1;
			printf("��ƱƱ�ţ�%d	����������%d\n",i,iresult[i]);
		}
	}
	printf("\n������Ʊ����%d\n",iTicketNum);
	printf("©Ʊ��������%d\n",iLeakTotal);
	printf("��Ʊ��������%d\n",iRepeatTotal);

	//��̬�����������
	/*	int *aresult=new int[iindex];
	for(int i=0;i<iindex;i++){
	aresult[i]=iresult[i];		
	}
	int iTResult[TICKET_NUM];
	for(int i=0;i<TICKET_NUM;i++){
	iTResult[i]=i+1;
	}
	printf("\n������...\n");
	int iSoldOut=0;
	for(int i=0;i<iindex;i++){
	for(int j=0;j<TICKET_NUM;j++){
	if(aresult[i]==iTResult[j]){
	iTResult[j]=SOLED_OUT;
	iSoldOut++;				
	}							
	}
	}	
	printf("\n©Ʊ��ϸ��Ϣ��\n");
	for(int i=0;i<TICKET_NUM;i++){
	if(iTResult[i]!=SOLED_OUT)
	printf("Ʊ��:%d\n",iTResult[i]);
	}	
	printf("\n��Ʊ��ϸ��Ϣ��\n");
	int itotal=0;
	for(int i=1;i<=TICKET_NUM;i++){
	int temp=0;
	for(int j=0;j<iindex;j++){
	if(aresult[j]==i){
	temp++;
	}			
	}
	if(temp>1){
	itotal+=temp-1;			
	printf("Ʊ�ţ�%d ����������%d\n",i,temp);
	}			
	}
	printf("\n������Ʊ����%d\n",iindex);
	printf("©Ʊ��������%d\n",TICKET_NUM-iSoldOut);
	printf("��Ʊ��������%d\n",itotal);	*/	
	

	//��̬���鷽��
	//vector<int> vecticket;
	//for(int i=1;i<=TICKET_NUM;i++){
	//	vecticket.push_back(i);
	//}
	//printf("\n������Ϣ��\n");
	//for(vector<int>::iterator iter=ivec.begin();iter!=ivec.end();iter++){
	//	for(vector<int>::iterator itert=vecticket.begin();itert!=vecticket.end();){
	//		if(*iter==*itert){
	//			vecticket.erase(itert);
	//		}
	//		else
	//			itert++;				
	//	}
	//}
	//printf("\n������Ʊ����%d\n",ivec.size());
	//printf("\n©Ʊ��������%d\n",vecticket.size());
	//for(vector<int>::iterator itert=vecticket.begin();itert!=vecticket.end();itert++){
	//	printf("Ʊ��:%d\n",*itert);
	//}	
	//map<int,int> imap;
	//int itotal=0;
	//for(int i=1;i<=TICKET_NUM;i++){
	//	int temp=0;
	//	for(vector<int>::iterator iter=ivec.begin();iter!=ivec.end();iter++){
	//		if(*iter==i){
	//			temp++;
	//		}			
	//	}
	//	if(temp>1){
	//		itotal+=temp-1;
	//		imap.insert(make_pair(i,temp));
	//		//printf("Ʊ�ţ�%d �ظ�������%d\n",i,temp);
	//	}			
	//}
	//	printf("��Ʊ��������%d\n",itotal);		
	//	for(map<int,int>::iterator iter=imap.begin();iter!=imap.end();iter++){			
	//		printf("Ʊ�ţ�%d ����������%d\n",iter->first,iter->second);
	//	}
	while(1);
	return 0;
	
}
