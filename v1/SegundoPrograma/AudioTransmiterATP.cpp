#include "stdafx.h"
#include "windows.h"
#include "conio.h"
#include "stdlib.h"
#include "portaudio.h"

#if PA_USE_ASIO   //isso aqui tem que ta nas FLAGs de compilação
#include "pa_asio.h"
#endif

#define BUF_SIZE 64
#define SAMPLE_RATE   (44100)

#ifndef M_PI
#define M_PI  (3.14159265)
#endif

#define yes 0
#define no 1

TCHAR MapObjATPRT8C_Value[]=TEXT("ATPRT8C_Value_MappingO");
TCHAR MapObjATPRT8C_Info[]=TEXT("ATPRT8C_Info_MappingO");

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

    ComunicationStatus ComunicationOfInfo;
    ChannelsBufferValue ChannelsBufferRecived;

/*******************************************************************/
static void PrintSupportedStandardSampleRates(
        const PaStreamParameters *inputParameters,
        const PaStreamParameters *outputParameters )
{
    static double standardSampleRates[] = {
        8000.0, 9600.0, 11025.0, 12000.0, 16000.0, 22050.0, 24000.0, 32000.0,
        44100.0, 48000.0, 88200.0, 96000.0, 192000.0, -1 /* negative terminated  list */
    };
    int     i, printCount;
    PaError err;

    printCount = 0;
    for( i=0; standardSampleRates[i] > 0; i++ )
    {
        err = Pa_IsFormatSupported( inputParameters, outputParameters, standardSampleRates[i] );
        if( err == paFormatIsSupported )
        {
            if( printCount == 0 )
            {
                printf( "\t%8.2f", standardSampleRates[i] );
                printCount = 1;
            }
            else if( printCount == 4 )
            {
                printf( ",\n\t%8.2f", standardSampleRates[i] );
                printCount = 1;
            }
            else
            {
                printf( ", %8.2f", standardSampleRates[i] );
                ++printCount;
            }
        }
    }
    if( !printCount )
        printf( "None\n" );
    else
        printf( "\n" );
}

int main()
{

  int     numDevices, defaultDisplayed;
  const   PaDeviceInfo *deviceInfo;
  PaStreamParameters inputParameters, outputParameters;
  PaStream *stream;
  PaError err;

  HANDLE hMapFile_Value;
  LPCTSTR pBuf_Value;
  HANDLE hMapFile_Info;
  LPCTSTR pBuf_Info;

 // abro a comunicaçao de valores com o espaço na memoria
    hMapFile_Value = OpenFileMapping
      (
      FILE_MAP_ALL_ACCESS, //Read,write access
      FALSE, //do not inherit the name
      MapObjATPRT8C_Value //name of mapping object
      );
    if(hMapFile_Value==NULL){
      printf("\nCould not open file mapping object: %d\n",GetLastError());
      system("PAUSE");
      exit(1);
      return 0;
    }
    pBuf_Value = (LPTSTR) MapViewOfFile
      (
      hMapFile_Value,
      FILE_MAP_ALL_ACCESS,
      0,
      0,
      BUF_SIZE
      );
    if (pBuf_Value== NULL){
      printf("Could not map view of file: %d\n", GetLastError());
      system("PAUSE");
      exit(1);
      return 0;
    }

 // abro a comunicaçao de informaçoes com o espaço na memoria
    hMapFile_Info = OpenFileMapping
      (
      FILE_MAP_ALL_ACCESS, //Read,write access
      FALSE, //do not inherit the name
      MapObjATPRT8C_Info //name of mapping object
      );
    if(hMapFile_Info==NULL){
      printf("\nCould not open file mapping object: %d\n",GetLastError());
      system("PAUSE");
      exit(1);
      return 0;
    }
    pBuf_Info = (LPTSTR) MapViewOfFile
      (
      hMapFile_Info,
      FILE_MAP_ALL_ACCESS,
      0,
      0,
      BUF_SIZE
      );
    if (pBuf_Info== NULL){
      printf("Could not map view of file: %d\n", GetLastError());
      system("PAUSE");
      exit(1);
      return 0;
    }

 // abre a comunicação com o portaudio/asio ---------------------------  PORTAUDIO   ASIO  ---/

    err = Pa_Initialize();
    if( err != paNoError )
    {
        printf( "ERROR: Pa_Initialize returned 0x%x\n", err );
        goto error;
    }
    numDevices = Pa_GetDeviceCount();
    if( numDevices < 0 )
    {
        printf( "ERROR: Pa_GetDeviceCount returned 0x%x\n", numDevices );
        err = numDevices;
        goto error;
    }
    printf( "Number of devices = %d\n", numDevices );
    outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
    deviceInfo = Pa_GetDeviceInfo( outputParameters.device );

    printf( "--------------------------------------- device #%d\n", outputParameters.device );
    /* print device info fields */
    #ifdef WIN32
            {   /* Use wide char on windows, so we can show UTF-8 encoded device names */
                wchar_t wideName[MAX_PATH];
                MultiByteToWideChar(CP_UTF8, 0, deviceInfo->name, -1, wideName, MAX_PATH-1);
                wprintf( L"Name                        = %s\n", wideName );
            }
    #else
            printf( "Name                        = %s\n", deviceInfo->name );
    #endif
            printf( "Host API                    = %s\n",  Pa_GetHostApiInfo( deviceInfo->hostApi )->name );
            printf( "Max inputs = %d", deviceInfo->maxInputChannels  );
            printf( ", Max outputs = %d\n", deviceInfo->maxOutputChannels  );

            printf( "Default low input latency   = %8.4f\n", deviceInfo->defaultLowInputLatency  );
            printf( "Default low output latency  = %8.4f\n", deviceInfo->defaultLowOutputLatency  );
            printf( "Default high input latency  = %8.4f\n", deviceInfo->defaultHighInputLatency  );
            printf( "Default high output latency = %8.4f\n", deviceInfo->defaultHighOutputLatency  );

    #if PA_USE_ASIO
    /* ASIO specific latency information */
            if( Pa_GetHostApiInfo( deviceInfo->hostApi )->type == paASIO ){
                long minLatency, maxLatency, preferredLatency, granularity;

                err = PaAsio_GetAvailableLatencyValues( outputParameters.device,
                    &minLatency, &maxLatency, &preferredLatency, &granularity );

                printf( "ASIO minimum buffer size    = %ld\n", minLatency  );
                printf( "ASIO maximum buffer size    = %ld\n", maxLatency  );
                printf( "ASIO preferred buffer size  = %ld\n", preferredLatency  );

                if( granularity == -1 )
                    printf( "ASIO buffer granularity     = power of 2\n" );
                else
                    printf( "ASIO buffer granularity     = %ld\n", granularity  );
            }
    #endif /* PA_USE_ASIO */
            printf( "Default sample rate         = %8.2f\n", deviceInfo->defaultSampleRate );

 /* poll for standard sample rates */
  outputParameters.channelCount = 8; //deviceInfo->maxOutputChannels;
  outputParameters.sampleFormat = paFloat32; // paInt16;
  outputParameters.suggestedLatency = 0.015; /* ignored by Pa_IsFormatSupported() */
  outputParameters.hostApiSpecificStreamInfo = NULL;


      if( outputParameters.channelCount > 0 )
      {
          printf("Supported standard sample rates\n for half-duplex 16 bit %d channel output = \n",
                  outputParameters.channelCount );
          PrintSupportedStandardSampleRates( NULL, &outputParameters );
      }

      printf("----------------------------------------------\n");

  err = Pa_OpenStream(
        &stream,
        NULL, /* no input */
        &outputParameters,
        SAMPLE_RATE,
        BUF_SIZE,
        paClipOff,      /* we won't output out of range samples so don't bother clipping them */
        NULL, /* no callback, use blocking API */
        NULL ); /* no callback, so no callback userData */
  if( err != paNoError ) goto error;

 // abre a comunicação com o portaudio/asio ---------------------------  PORTAUDIO   ASIO  ---/

 // continue... crio uma variavel local a apenas para visualizar os dados menos extenso
      ChannelsBufferValue a;

      printf("\nReceiving Matrix of Values...\n");
      CopyMemory(&a,(PVOID)pBuf_Value,sizeof(a));

      printf("channel1 channel2 channel3 channel4 channel5 channel6 channel7 channel8:\n");
      for(int i=0;i<BUF_SIZE;i++){
        printf("%8.0f %8.0f %8.0f %8.0f %8.0f %8.0f %8.0f %8.0f\n"
          ,a.channel1[i],a.channel2[i],a.channel3[i],a.channel4[i]
          ,a.channel5[i],a.channel6[i],a.channel7[i],a.channel8[i]);
      }
 //    system("PAUSE");
 // continue... crio uma variavel local a apenas para visualizar as informaçoes menos extenso
      ComunicationStatus b;

      printf("\nReceiving Matrix of Information...\n");
      CopyMemory(&b,(PVOID)pBuf_Info,sizeof(b));

      printf("CanIRead CanIWrite ATPtime ATPtimeStart ATPtimeStop ATPtimeStep erroMsg:\n");
      printf("%8.0f %8.0f %8.0f %8.0f %8.0f %8.0f %8.0f\n"
          ,b.CanIRead,b.CanIWrite,b.ATPtime,b.ATPtimeStart,b.ATPtimeStop,b.ATPtimeStep,b.errorMsg);
 //    system("PAUSE");
 // falo pro outro programa que ele pode começar a escrever na memoria

  ComunicationOfInfo = b;
  ComunicationOfInfo.CanIWrite = yes; // digo ao outro software para continuar...

  size_t sizeBuffInfo;
  sizeBuffInfo = sizeof(ComunicationOfInfo);
  size_t sizeBuffValue;
  sizeBuffValue = sizeof(ChannelsBufferRecived);

  CopyMemory((PVOID)pBuf_Info, &ComunicationOfInfo,sizeBuffInfo); //write


  printf("\nSending Matrix Information...%d %d\n",sizeBuffValue, sizeBuffInfo);
 
 // inicio a stream de audio  ----------------------------------------- PORTAUDIO    ASIO ---/
  err = Pa_StartStream( stream );
  if( err != paNoError ) goto error;

  //system("PAUSE");

 // inicio parte principal -----------------------------------------------------------------/

  //double CodeIsRunning = 1;
  printf("\nEscrevendo nos canais de audio... Simulando em ATPRT_8channels...\n");

  int StreamAvailable;
  float buffer[BUF_SIZE][8];

  while(1){

      int CanIstart = no;
      while(CanIstart){ 
        CopyMemory(&ComunicationOfInfo,(PVOID)pBuf_Info,sizeBuffInfo); //read
        if(ComunicationOfInfo.CanIRead == yes) CanIstart = yes;
        if(ComunicationOfInfo.ATPtime>=(ComunicationOfInfo.ATPtimeStop-ComunicationOfInfo.ATPtimeStep)) goto finish;
      	if(kbhit()){
      		char tecla = getch();
      		switch(tecla){
      			case 'c':
      				ComunicationOfInfo.tecla = 1;
      			break;
      			case 'f':
      				ComunicationOfInfo.tecla = 2;
      			break;
      			case 't':
      				ComunicationOfInfo.tecla = 3;
      			break;
      			case 'l':
      				ComunicationOfInfo.tecla = 4;
      			break;
      			default:
      				ComunicationOfInfo.tecla = 0;
      			break;
      		}
      	}
      }

      CopyMemory( &ChannelsBufferRecived, (PVOID)pBuf_Value, sizeBuffValue); //read

      ComunicationOfInfo.CanIRead = no; // buffer vazio, espero preencher
      ComunicationOfInfo.CanIWrite = yes; // digo ao outro software que ele pode escrever

      CopyMemory((PVOID)pBuf_Info, &ComunicationOfInfo,sizeBuffInfo); //write
      
      for(int i=0;i<BUF_SIZE;i++){ //canais modificados para execuçao
        buffer[i][0] = ChannelsBufferRecived.channel1[i];
        buffer[i][1] = ChannelsBufferRecived.channel2[i];
        buffer[i][2] = ChannelsBufferRecived.channel3[i];
        buffer[i][3] = ChannelsBufferRecived.channel4[i];
        buffer[i][4] = ChannelsBufferRecived.channel5[i];
        buffer[i][5] = ChannelsBufferRecived.channel6[i];
        buffer[i][6] = ChannelsBufferRecived.channel7[i];
        buffer[i][7] = ChannelsBufferRecived.channel8[i];
      }

      while(Pa_GetStreamWriteAvailable( stream ) < BUF_SIZE){}

      err = Pa_WriteStream( stream, buffer, BUF_SIZE );
      if( err != paNoError ) printf("\n...ERROR...underflow detect...ERROR..\n");
  }

finish:
  printf("\nFim do processo...\n");

  err = Pa_StopStream( stream );
  if( err != paNoError ) goto error;
        
  err = Pa_CloseStream( stream );
  if( err != paNoError ) goto error;

  err = Pa_Terminate(); 
  if( err != paNoError ) goto error;


  system("PAUSE");
  return 0;

error:
	ComunicationOfInfo.errorMsg = (double)err;
    CopyMemory((PVOID)pBuf_Info, &ComunicationOfInfo,sizeBuffInfo); //write

    if( stream ) {
       Pa_AbortStream( stream );
       Pa_CloseStream( stream );
    }
    Pa_Terminate();
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
    system("PAUSE");
    return err;
}