// Day40_ThreadTest.cpp : 定义控制台应用程序的入口点。
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
int iresult[TICKET_NUM]={};		//不加锁最终解决方法
int iindex;
//volatile long iindex=-1;
//vector<int> ivec;			//动态数组方法
int idistance=TICKET_NUM;
int iTicketNum=0;					//不加锁最终解决方法

DWORD WINAPI ThreadFun(PVOID pPM){	
	
		while(iTicketNum<TICKET_NUM){
			EnterCriticalSection(&g_csThread);
			int itemp=iTicketNum;
			
			//不加锁最终解决方法			
			if(iTicketNum<TICKET_NUM){	
				
				printf("%d窗口卖票：%d\n",(int)pPM,itemp);
				
				//iresult[itemp]++;				
				_asm  mov         eax,dword ptr [itemp]  			 
				_asm  inc		  dword ptr iresult[eax*4]
				//Sleep(1);												
				//iTicketNum++;
				_asm  inc		  dword ptr[iTicketNum];				
			}
			LeaveCriticalSection(&g_csThread);  		
			/*int itemp=idistance;		
			printf("%d窗口卖票：%d\n",i,itemp);					
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
	//不加锁最终解决方法
	int iLeakTotal=0;
	int iRepeatTotal=0;
	printf("\n");
	for(int i=0;i<TICKET_NUM;i++){
		if(iresult[i]==0){	
			iLeakTotal++;
			printf("漏票票号：%d\n",i);
		}					
	}
	printf("\n");
	for(int i=0;i<TICKET_NUM;i++){
		if(iresult[i]>1){
			iRepeatTotal+=iresult[i]-1;
			printf("重票票号：%d	卖出张数：%d\n",i,iresult[i]);
		}
	}
	printf("\n卖出总票数：%d\n",iTicketNum);
	printf("漏票总张数：%d\n",iLeakTotal);
	printf("重票总张数：%d\n",iRepeatTotal);

	//静态数组加锁方法
	/*	int *aresult=new int[iindex];
	for(int i=0;i<iindex;i++){
	aresult[i]=iresult[i];		
	}
	int iTResult[TICKET_NUM];
	for(int i=0;i<TICKET_NUM;i++){
	iTResult[i]=i+1;
	}
	printf("\n计算中...\n");
	int iSoldOut=0;
	for(int i=0;i<iindex;i++){
	for(int j=0;j<TICKET_NUM;j++){
	if(aresult[i]==iTResult[j]){
	iTResult[j]=SOLED_OUT;
	iSoldOut++;				
	}							
	}
	}	
	printf("\n漏票详细信息：\n");
	for(int i=0;i<TICKET_NUM;i++){
	if(iTResult[i]!=SOLED_OUT)
	printf("票号:%d\n",iTResult[i]);
	}	
	printf("\n重票详细信息：\n");
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
	printf("票号：%d 卖出张数：%d\n",i,temp);
	}			
	}
	printf("\n卖出总票数：%d\n",iindex);
	printf("漏票总张数：%d\n",TICKET_NUM-iSoldOut);
	printf("重票总张数：%d\n",itotal);	*/	
	

	//动态数组方法
	//vector<int> vecticket;
	//for(int i=1;i<=TICKET_NUM;i++){
	//	vecticket.push_back(i);
	//}
	//printf("\n调试信息：\n");
	//for(vector<int>::iterator iter=ivec.begin();iter!=ivec.end();iter++){
	//	for(vector<int>::iterator itert=vecticket.begin();itert!=vecticket.end();){
	//		if(*iter==*itert){
	//			vecticket.erase(itert);
	//		}
	//		else
	//			itert++;				
	//	}
	//}
	//printf("\n卖出总票数：%d\n",ivec.size());
	//printf("\n漏票总张数：%d\n",vecticket.size());
	//for(vector<int>::iterator itert=vecticket.begin();itert!=vecticket.end();itert++){
	//	printf("票号:%d\n",*itert);
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
	//		//printf("票号：%d 重复张数：%d\n",i,temp);
	//	}			
	//}
	//	printf("重票总张数：%d\n",itotal);		
	//	for(map<int,int>::iterator iter=imap.begin();iter!=imap.end();iter++){			
	//		printf("票号：%d 卖出张数：%d\n",iter->first,iter->second);
	//	}
	while(1);
	return 0;
	
}
