#include "stdafx.h"
#include "windows.h"
#include "stdlib.h"
//#include "iostream.h"

#define BUF_SIZE 64
#define SAMPLE_RATE   (44100)


#ifndef M_PI
#define M_PI  (3.14159265)
#endif

#define yes 0
#define no 1

TCHAR MapObjATPRT8C_Value[]=TEXT("ATPRT8C_Value_MappingO");
TCHAR MapObjATPRT8C_Info[]=TEXT("ATPRT8C_Info_MappingO");

HANDLE hMapFile_Value;
LPCTSTR pBuf_Value;
HANDLE hMapFile_Info;
LPCTSTR pBuf_Info;

struct ChannelsBufferValue
{
    double channel1[BUF_SIZE];
    double channel2[BUF_SIZE];
    double channel3[BUF_SIZE];
    double channel4[BUF_SIZE];
    double channel5[BUF_SIZE];
    double channel6[BUF_SIZE];
    double channel7[BUF_SIZE];
    double channel8[BUF_SIZE];
};

struct ComunicationStatus
{
    double CanIRead; // 0 don't , 1 you can
    double CanIWrite; // 0 don't , 1 you can
    double ATPtime;
    double ATPtimeStart;
    double ATPtimeStop;
    double ATPtimeStep;
    double errorMsg;
    double tecla;
};

ChannelsBufferValue ChannelsBufferATP;
ChannelsBufferValue ChannelsBufferForSend;
ComunicationStatus ComunicationOfInfo;

double ATPtimebuffer[BUF_SIZE+2];
double timestepaudio = 1.0/SAMPLE_RATE;
double timebufferATP = 0;
double Audiotime = 0;

int bufferCount=2;

//------------------------------------------------------------------------------
extern "C" 
{
    void  outsix_(char *, int *); 
    void c_teste_i__(double xdata_ar[], double xin_ar[], double xout_ar[], double xvar_ar[]);
    void c_teste_m__(double xdata_ar[], double xin_ar[], double xout_ar[], double xvar_ar[]);
}

void startup(LPCSTR lpApplicationName)
{
    // additional information
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;

    // set the size of the structures
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // start the program up
    CreateProcessA
    (
        lpApplicationName,   // the path
        NULL,                // Command line
        NULL,                   // Process handle not inheritable
        NULL,                   // Thread handle not inheritable
        FALSE,                  // Set handle inheritance to FALSE
        CREATE_NEW_CONSOLE,     // Opens file in a separate console
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi           // Pointer to PROCESS_INFORMATION structure
    );
        // Close process and thread handles. 
    //    CloseHandle(pi.hProcess);
    //   CloseHandle(pi.hThread);
}

//------------------------------------------------------------------------------
void c_teste_i__(double xdata_ar[], double xin_ar[], double xout_ar[], double xvar_ar[])
{
  	double ran;

	double MAX1 = xdata_ar[0];
	double MAX2 = xdata_ar[1];
	double MAX3 = xdata_ar[0];
	double MAX4 = xdata_ar[1];
	double MAX5 = xdata_ar[0];
	double MAX6 = xdata_ar[1];
	double MAX7 = xdata_ar[0];
	double MAX8 = xdata_ar[1];

	double ATPtime = xin_ar[0];
	double ATPtimeStart = xin_ar[1];
	double ATPtimeStop = xin_ar[2];
	double ATPtimeStep = xin_ar[3];

    char* text = "Inicia o programa. Primeira interacao.\n";
    int len = strlen(text);
    outsix_(text, &len);
 //crio uma matrix apenas para analisar o funcionamento
	for(int j=0;j<BUF_SIZE;j++){
		ran = j;
		ChannelsBufferForSend.channel1[j] = ran;
		ran = j+BUF_SIZE;
		ChannelsBufferForSend.channel2[j] = ran;
		ran = j+BUF_SIZE*2;
		ChannelsBufferForSend.channel3[j] = ran;
		ran = j+BUF_SIZE*3;
		ChannelsBufferForSend.channel4[j] = ran;
		ran = j+BUF_SIZE*4;
		ChannelsBufferForSend.channel5[j] = ran;
		ran = j+BUF_SIZE*5;
		ChannelsBufferForSend.channel6[j] = ran;
		ran = j+BUF_SIZE*6;
		ChannelsBufferForSend.channel7[j] = ran;
		ran = j+BUF_SIZE*7;
		ChannelsBufferForSend.channel8[j] = ran;
	}
 // atribuo as Informaçoes na estrutura
	ComunicationOfInfo.CanIRead = no; // 0 don't , 1 you can
	ComunicationOfInfo.CanIWrite = no; // 0 don't , 1 you can
	ComunicationOfInfo.ATPtime = ATPtime;
	ComunicationOfInfo.ATPtimeStart = ATPtimeStart;
	ComunicationOfInfo.ATPtimeStop = ATPtimeStop;
	ComunicationOfInfo.ATPtimeStep = ATPtimeStep;
	ComunicationOfInfo.errorMsg = 0;

 //Windows Programming with API

 //Create File Mapping for Values to be send from ATP to ASIO driver
	  hMapFile_Value = CreateFileMapping
	    (
	    INVALID_HANDLE_VALUE,
	    NULL,
	    PAGE_READWRITE,
	    0,
	    BUF_SIZE,
	    MapObjATPRT8C_Value
	    );
	  //End Create File Mapping
	  //Check if creation success
	  if(hMapFile_Value == NULL || hMapFile_Value == INVALID_HANDLE_VALUE){
	    printf("\nCannot create file mapping: %d",GetLastError());
	    exit(1);
	  }else{
	  	printf("\nCreate File Mapping success!\n");
	  }
	  //Map View of File
	  pBuf_Value = (LPTSTR)MapViewOfFile
	    (
	    hMapFile_Value,
	    FILE_MAP_ALL_ACCESS,
	    0,
	    0,
	    BUF_SIZE
	    );
	  //Check if Create success
	  if(pBuf_Value==NULL){
	    printf("\nCannot Create Map View of File: %d",GetLastError());
	    exit(1);
	  }
 //Create File Mapping for Informations to comunicate betwenn ATP and ASIO controler
	  hMapFile_Info = CreateFileMapping
	    (
	    INVALID_HANDLE_VALUE,
	    NULL,
	    PAGE_READWRITE,
	    0,
	    BUF_SIZE,
	    MapObjATPRT8C_Info
	    );
	  //End Create File Mapping
	  //Check if creation success
	  if(hMapFile_Info == NULL || hMapFile_Info == INVALID_HANDLE_VALUE){
	    printf("\nCannot create file mapping: %d",GetLastError());
	    exit(1);
	  }else{
	  	printf("\nCreate File Mapping success!\n");
	  }
	  //Map View of File
	  pBuf_Info = (LPTSTR)MapViewOfFile
	    (
	    hMapFile_Info,
	    FILE_MAP_ALL_ACCESS,
	    0,
	    0,
	    BUF_SIZE
	    );
	  //Check if Create success
	  if(pBuf_Info==NULL){
	    printf("\nCannot Create Map View of File: %d",GetLastError());
	    exit(1);
	  }

 // continue... crio uma variavel local a apenas para enviar os dados menos extenso
 /*
  ChannelsBufferValue a = ChannelsBufferForSend;

  printf("channel1 channel2 channel3 channel4 channel5 channel6 channel7 channel8:\n");
  for(int i=0;i<BUF_SIZE;i++){
    printf("   %8f       %8f       %8f       %8f        %8f       %8f       %8f       %8f   \n"
      ,a.channel1[i],a.channel2[i],a.channel3[i],a.channel4[i]
      ,a.channel5[i],a.channel6[i],a.channel7[i],a.channel8[i]);
  }
  system("PAUSE");
 */

  CopyMemory((PVOID)pBuf_Value, &ChannelsBufferForSend,sizeof(ChannelsBufferForSend));

  CopyMemory((PVOID)pBuf_Info, &ComunicationOfInfo,sizeof(ComunicationOfInfo));

  //Send Information to shared memory
  printf("\nSending Matrix and Informations...\n");

 //  verifica o time step do ATP
      if(ATPtimeStep<timestepaudio){
    	text = "-------------------------------------------------------------------------------------";
    	len = strlen(text);
    	outsix_(text, &len);
    	text = "The ATP time step must be greater than 1/44100 \n";
    	len = strlen(text);
    	outsix_(text, &len);
    	return;
    }else if(ATPtimeStep>timestepaudio*BUF_SIZE/2){
    	text = "-------------------------------------------------------------------------------------";
    	len = strlen(text);
    	outsix_(text, &len);
    	text = "The ATP time step must be smaller than 64/44100/2 \n";
    	len = strlen(text);
    	outsix_(text, &len);
    	text = "Reduce the step time or increase the buffer \n";
    	len = strlen(text);
    	outsix_(text, &len);
    	return;
    }

  startup("C:\\Users\\LAPES357\\Desktop\\myteste\\AudioTransmiterATP.exe");

  printf("\nEsperando o outro programa responder...\n");
  double CanIstart = 1;
  while(CanIstart){ 
	CopyMemory(&ComunicationOfInfo,(PVOID)pBuf_Info,sizeof(ComunicationOfInfo));
	if(ComunicationOfInfo.CanIWrite == 0) CanIstart = 0;
  }

	ATPtimebuffer[0] = 0;
	ATPtimebuffer[1] = ATPtimeStep;
	Audiotime = ATPtime;
	timebufferATP = ATPtime;

    return;
}
//------------------------------------------------------------------------------
void c_teste_m__(double xdata_ar[], double xin_ar[], double xout_ar[], double xvar_ar[])
{
	double MAX1 = xdata_ar[0];
	double MAX2 = xdata_ar[1];
	double MAX3 = xdata_ar[2];
	double MAX4 = xdata_ar[3];
	double MAX5 = xdata_ar[4];
	double MAX6 = xdata_ar[5];
	double MAX7 = xdata_ar[6];
	double MAX8 = xdata_ar[7];

	double ATPtime = xin_ar[0];
	double ATPtimeStart = xin_ar[1];
	double ATPtimeStop = xin_ar[2];
	double ATPtimeStep = xin_ar[3];

	double channel1 = xin_ar[4];
	double channel2 = xin_ar[5];
	double channel3 = xin_ar[6];
	double channel4 = xin_ar[7];
	double channel5 = xin_ar[8];
	double channel6 = xin_ar[9];
	double channel7 = xin_ar[10];
	double channel8 = xin_ar[11];

  
    ChannelsBufferATP.channel1[bufferCount]=channel1/MAX1;
    ChannelsBufferATP.channel2[bufferCount]=channel2/MAX2;
    ChannelsBufferATP.channel3[bufferCount]=channel3/MAX3;
    ChannelsBufferATP.channel4[bufferCount]=channel4/MAX4;
    ChannelsBufferATP.channel5[bufferCount]=channel5/MAX5;
    ChannelsBufferATP.channel6[bufferCount]=channel6/MAX6;
    ChannelsBufferATP.channel7[bufferCount]=channel7/MAX7;
    ChannelsBufferATP.channel8[bufferCount]=channel8/MAX8;


    ATPtimebuffer[bufferCount] = ATPtime;
	
	if(timebufferATP>=(Audiotime+timestepaudio*BUF_SIZE)){
		int j = 0;
		// é feito uma reamostragem do sinal 
		for(int i=0;i<BUF_SIZE;i++){

			if(Audiotime>=ATPtimebuffer[j+1]){
				j++;
			}

			double deltatime = (Audiotime-ATPtimebuffer[j])/ATPtimeStep;

			ChannelsBufferForSend.channel1[i] = ((ChannelsBufferATP.channel1[j+1]-ChannelsBufferATP.channel1[j])*deltatime) + ChannelsBufferATP.channel1[j];
			ChannelsBufferForSend.channel2[i] = ((ChannelsBufferATP.channel2[j+1]-ChannelsBufferATP.channel2[j])*deltatime) + ChannelsBufferATP.channel2[j];
			ChannelsBufferForSend.channel3[i] = ((ChannelsBufferATP.channel3[j+1]-ChannelsBufferATP.channel3[j])*deltatime) + ChannelsBufferATP.channel3[j];
			ChannelsBufferForSend.channel4[i] = ((ChannelsBufferATP.channel4[j+1]-ChannelsBufferATP.channel4[j])*deltatime) + ChannelsBufferATP.channel4[j];
			ChannelsBufferForSend.channel5[i] = ((ChannelsBufferATP.channel5[j+1]-ChannelsBufferATP.channel5[j])*deltatime) + ChannelsBufferATP.channel5[j];
			ChannelsBufferForSend.channel6[i] = ((ChannelsBufferATP.channel6[j+1]-ChannelsBufferATP.channel6[j])*deltatime) + ChannelsBufferATP.channel6[j];
			ChannelsBufferForSend.channel7[i] = ((ChannelsBufferATP.channel7[j+1]-ChannelsBufferATP.channel7[j])*deltatime) + ChannelsBufferATP.channel7[j];
			ChannelsBufferForSend.channel8[i] = ((ChannelsBufferATP.channel8[j+1]-ChannelsBufferATP.channel8[j])*deltatime) + ChannelsBufferATP.channel8[j];
			
			Audiotime += timestepaudio;
		}
	    	
		ChannelsBufferATP.channel1[0]=ChannelsBufferATP.channel1[bufferCount-1];
	    ChannelsBufferATP.channel2[0]=ChannelsBufferATP.channel2[bufferCount-1];
		ChannelsBufferATP.channel3[0]=ChannelsBufferATP.channel3[bufferCount-1];
	    ChannelsBufferATP.channel4[0]=ChannelsBufferATP.channel4[bufferCount-1];
	    ChannelsBufferATP.channel5[0]=ChannelsBufferATP.channel5[bufferCount-1];
	    ChannelsBufferATP.channel6[0]=ChannelsBufferATP.channel6[bufferCount-1];
	    ChannelsBufferATP.channel7[0]=ChannelsBufferATP.channel7[bufferCount-1];
	    ChannelsBufferATP.channel8[0]=ChannelsBufferATP.channel8[bufferCount-1];

		ChannelsBufferATP.channel1[1]=ChannelsBufferATP.channel1[bufferCount];
	    ChannelsBufferATP.channel2[1]=ChannelsBufferATP.channel2[bufferCount];
		ChannelsBufferATP.channel3[1]=ChannelsBufferATP.channel3[bufferCount];
	    ChannelsBufferATP.channel4[1]=ChannelsBufferATP.channel4[bufferCount];
	    ChannelsBufferATP.channel5[1]=ChannelsBufferATP.channel5[bufferCount];
	    ChannelsBufferATP.channel6[1]=ChannelsBufferATP.channel6[bufferCount];
	    ChannelsBufferATP.channel7[1]=ChannelsBufferATP.channel7[bufferCount];
	    ChannelsBufferATP.channel8[1]=ChannelsBufferATP.channel8[bufferCount];

	    ATPtimebuffer[0]=ATPtimebuffer[bufferCount-1];
	    ATPtimebuffer[1]=ATPtimebuffer[bufferCount];

		bufferCount = 2;
       
		int CanIWrite = no; 
  		while(CanIWrite){
			CopyMemory(&ComunicationOfInfo,(PVOID)pBuf_Info,sizeof(ComunicationOfInfo)); //read
			if(ComunicationOfInfo.CanIWrite == yes) CanIWrite = yes;
			if(ComunicationOfInfo.errorMsg < 0){
				printf("\nUm Erro ocorreu na comunicação com os canais de audio\n");
				printf("\nERROR: %d \n", (int)ComunicationOfInfo.errorMsg);
				system("PAUSE");
				exit(-1);
			}
  		}

  	    CopyMemory((PVOID)pBuf_Value, &ChannelsBufferForSend,sizeof(ChannelsBufferForSend)); //write

  	    ComunicationOfInfo.CanIRead = yes; // digo ao outro software que ele pode ler
  	    ComunicationOfInfo.CanIWrite = no; // neste momento nao posso escrever, preciso esperar
  	    ComunicationOfInfo.ATPtime = ATPtime;
 		CopyMemory( (PVOID)pBuf_Info, &ComunicationOfInfo,  sizeof(ComunicationOfInfo)); //write

  	}else{
  	    bufferCount++;
  	}

	timebufferATP += ATPtimeStep; 

  	if(ATPtime>=ATPtimeStop-ATPtimeStep){
  		
  	//	system("PAUSE");
  	//	UnmapViewOfFile(pBuf);

  		ComunicationOfInfo.CanIRead = no; // digo ao outro software que ele pode ler
  	    ComunicationOfInfo.CanIWrite = no; // neste momento nao posso escrever, preciso esperar
  	    ComunicationOfInfo.ATPtime = ATPtime;
 		CopyMemory( (PVOID)pBuf_Info, &ComunicationOfInfo,  sizeof(ComunicationOfInfo)); //write

  		CloseHandle(hMapFile_Value);
  		CloseHandle(hMapFile_Info);
  	}
  	
 	xvar_ar[0] = 0;
 	xvar_ar[1] = 0;
    xout_ar[0] = xvar_ar[0];
    xout_ar[1] = xvar_ar[1];

    return;
}
//------------------------------------------------------------------------------
