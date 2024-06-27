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


double timestepaudio = 1.0/SAMPLE_RATE;
double Audiotime = 0;

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

struct ChannelsBufferAdjus
{
    double channel1[BUF_SIZE+2];
    double channel2[BUF_SIZE+2];
    double channel3[BUF_SIZE+2];
    double channel4[BUF_SIZE+2];
    double channel5[BUF_SIZE+2];
    double channel6[BUF_SIZE+2];
    double channel7[BUF_SIZE+2];
    double channel8[BUF_SIZE+2];
    double ATPtimeV[BUF_SIZE+2];
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
    double final;
    double bufferC;
    double MAX1;
	double MAX2;
	double MAX3;
	double MAX4;
	double MAX5;
	double MAX6;
	double MAX7;
	double MAX8;
	double ATPtimeV[BUF_SIZE];
};

  ComunicationStatus ComunicationOfInfo;
  ChannelsBufferValue ChannelsBufferRecived;
  ChannelsBufferAdjus ChannelsBufferAdjust;

  HANDLE hMapFile_Value;
  LPCTSTR pBuf_Value;
  HANDLE hMapFile_Info;
  LPCTSTR pBuf_Info;

  size_t sizeBuffInfo;
  size_t sizeBuffValue;


typedef struct
{
    float buffer[BUF_SIZE][8];
}
paTestData;

/* This routine will be called by the PortAudio engine when audio is needed.
** It may called at interrupt level on some machines so don't do anything
** that could mess up the system like calling malloc() or free().
*/
static int patestCallback( const void *inputBuffer, void *outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo* timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void *userData )
{
    paTestData *data = (paTestData*)userData;
    float *out = (float*)outputBuffer;
    unsigned long i;

    /* avoid unused variable warnings */
    (void) inputBuffer;
    (void) timeInfo;
    (void) statusFlags;

    if (ComunicationOfInfo.final){
      CopyMemory((PVOID)pBuf_Info, &ComunicationOfInfo, sizeBuffInfo); //write
      return paComplete;
    } 

    int CanIstart = no;
    while(CanIstart){ 
      CopyMemory(&ComunicationOfInfo,(PVOID)pBuf_Info, sizeBuffInfo); //read
      if(ComunicationOfInfo.CanIRead == yes) CanIstart = yes;
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

    CopyMemory((PVOID)pBuf_Info, &ComunicationOfInfo, sizeBuffInfo); //write
    
    int position = (int)ComunicationOfInfo.bufferC;

    //printf("position %d\n", position);

    for( i=0; i<=position; i++ )
    { 
      ChannelsBufferAdjust.channel1[i+2] = ChannelsBufferRecived.channel1[i]/ComunicationOfInfo.MAX1;
      ChannelsBufferAdjust.channel2[i+2] = ChannelsBufferRecived.channel2[i]/ComunicationOfInfo.MAX2;
      ChannelsBufferAdjust.channel3[i+2] = ChannelsBufferRecived.channel3[i]/ComunicationOfInfo.MAX3;
      ChannelsBufferAdjust.channel4[i+2] = ChannelsBufferRecived.channel4[i]/ComunicationOfInfo.MAX4;
      ChannelsBufferAdjust.channel5[i+2] = ChannelsBufferRecived.channel5[i]/ComunicationOfInfo.MAX5;
      ChannelsBufferAdjust.channel6[i+2] = ChannelsBufferRecived.channel6[i]/ComunicationOfInfo.MAX6;
      ChannelsBufferAdjust.channel7[i+2] = ChannelsBufferRecived.channel7[i]/ComunicationOfInfo.MAX7;
      ChannelsBufferAdjust.channel8[i+2] = ChannelsBufferRecived.channel8[i]/ComunicationOfInfo.MAX8;

      ChannelsBufferAdjust.ATPtimeV[i+2] = ComunicationOfInfo.ATPtimeV[i];

    //printf("ATPtimeV %.8f\n",  ComunicationOfInfo.ATPtimeV[i]);
	//system("PAUSE");
    }

    int j = 0;
	// é feito uma reamostragem do sinal 
	for( i=0; i<framesPerBuffer; i++ ){

	//	printf("j %d\n", j);
	//	printf("Audiotime %.8f\n", Audiotime);

		if(Audiotime>=ChannelsBufferAdjust.ATPtimeV[j+1]){
			j++;
		}

	//	printf("j %d\n", j);
	//	system("PAUSE");

		double deltatime = (Audiotime-ChannelsBufferAdjust.ATPtimeV[j])/ComunicationOfInfo.ATPtimeStep;

		data->buffer[i][0] = ((ChannelsBufferAdjust.channel1[j+1]-ChannelsBufferAdjust.channel1[j])*deltatime) + ChannelsBufferAdjust.channel1[j];
		data->buffer[i][1] = ((ChannelsBufferAdjust.channel2[j+1]-ChannelsBufferAdjust.channel2[j])*deltatime) + ChannelsBufferAdjust.channel2[j];
		data->buffer[i][2] = ((ChannelsBufferAdjust.channel3[j+1]-ChannelsBufferAdjust.channel3[j])*deltatime) + ChannelsBufferAdjust.channel3[j];
		data->buffer[i][3] = ((ChannelsBufferAdjust.channel4[j+1]-ChannelsBufferAdjust.channel4[j])*deltatime) + ChannelsBufferAdjust.channel4[j];
		data->buffer[i][4] = ((ChannelsBufferAdjust.channel5[j+1]-ChannelsBufferAdjust.channel5[j])*deltatime) + ChannelsBufferAdjust.channel5[j];
		data->buffer[i][5] = ((ChannelsBufferAdjust.channel6[j+1]-ChannelsBufferAdjust.channel6[j])*deltatime) + ChannelsBufferAdjust.channel6[j];
		data->buffer[i][6] = ((ChannelsBufferAdjust.channel7[j+1]-ChannelsBufferAdjust.channel7[j])*deltatime) + ChannelsBufferAdjust.channel7[j];
		data->buffer[i][7] = ((ChannelsBufferAdjust.channel8[j+1]-ChannelsBufferAdjust.channel8[j])*deltatime) + ChannelsBufferAdjust.channel8[j];
		
		Audiotime += timestepaudio;
	}

	
	//printf("j %d\n", j);
	//printf("ATPtimeV %.8f\n", ChannelsBufferAdjust.ATPtimeV[j]);
	//printf("Audiotime %.8f\n", Audiotime);
	//system("PAUSE");

	ChannelsBufferAdjust.channel1[0]=ChannelsBufferAdjust.channel1[position+1];
    ChannelsBufferAdjust.channel2[0]=ChannelsBufferAdjust.channel2[position+1];
	ChannelsBufferAdjust.channel3[0]=ChannelsBufferAdjust.channel3[position+1];
    ChannelsBufferAdjust.channel4[0]=ChannelsBufferAdjust.channel4[position+1];
    ChannelsBufferAdjust.channel5[0]=ChannelsBufferAdjust.channel5[position+1];
    ChannelsBufferAdjust.channel6[0]=ChannelsBufferAdjust.channel6[position+1];
    ChannelsBufferAdjust.channel7[0]=ChannelsBufferAdjust.channel7[position+1];
    ChannelsBufferAdjust.channel8[0]=ChannelsBufferAdjust.channel8[position+1];

	ChannelsBufferAdjust.channel1[1]=ChannelsBufferAdjust.channel1[position+2];
    ChannelsBufferAdjust.channel2[1]=ChannelsBufferAdjust.channel2[position+2];
	ChannelsBufferAdjust.channel3[1]=ChannelsBufferAdjust.channel3[position+2];
    ChannelsBufferAdjust.channel4[1]=ChannelsBufferAdjust.channel4[position+2];
    ChannelsBufferAdjust.channel5[1]=ChannelsBufferAdjust.channel5[position+2];
    ChannelsBufferAdjust.channel6[1]=ChannelsBufferAdjust.channel6[position+2];
    ChannelsBufferAdjust.channel7[1]=ChannelsBufferAdjust.channel7[position+2];
    ChannelsBufferAdjust.channel8[1]=ChannelsBufferAdjust.channel8[position+2];

	ChannelsBufferAdjust.ATPtimeV[0]=ChannelsBufferAdjust.ATPtimeV[position+1];
	ChannelsBufferAdjust.ATPtimeV[1]=ChannelsBufferAdjust.ATPtimeV[position+2];


    for( i=0; i<framesPerBuffer; i++ )
    {
        *out++ = data->buffer[i][0];  /* 0 */
        *out++ = data->buffer[i][1];  /* 1 */
        *out++ = data->buffer[i][2];  /* 2 */
        *out++ = data->buffer[i][3];  /* 3 */
        *out++ = data->buffer[i][4];  /* 4 */
        *out++ = data->buffer[i][5];  /* 5 */
        *out++ = data->buffer[i][6];  /* 6 */
        *out++ = data->buffer[i][7];  /* 7 */
    }
    return paContinue;
}

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
  paTestData data;
  PaError err;

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
      4096
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
      1024
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

            if(deviceInfo->maxOutputChannels != 8) goto errorC;

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
  outputParameters.channelCount = deviceInfo->maxOutputChannels;
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
  ComunicationOfInfo.final = false;

  Audiotime = 0;
  ChannelsBufferAdjust.ATPtimeV[0] = 0;
  ChannelsBufferAdjust.ATPtimeV[1] = 0;

  sizeBuffInfo = sizeof(ComunicationOfInfo);
  
  sizeBuffValue = sizeof(ChannelsBufferRecived);

  CopyMemory((PVOID)pBuf_Info, &ComunicationOfInfo,sizeBuffInfo); //write

  ChannelsBufferAdjust.channel1[0] = 0;
  ChannelsBufferAdjust.channel2[0] = 0;
  ChannelsBufferAdjust.channel3[0] = 0;
  ChannelsBufferAdjust.channel4[0] = 0;
  ChannelsBufferAdjust.channel5[0] = 0;
  ChannelsBufferAdjust.channel6[0] = 0;
  ChannelsBufferAdjust.channel7[0] = 0;
  ChannelsBufferAdjust.channel8[0] = 0;

  ChannelsBufferAdjust.channel1[1] = 0;
  ChannelsBufferAdjust.channel2[1] = 0;
  ChannelsBufferAdjust.channel3[1] = 0;
  ChannelsBufferAdjust.channel4[1] = 0;
  ChannelsBufferAdjust.channel5[1] = 0;
  ChannelsBufferAdjust.channel6[1] = 0;
  ChannelsBufferAdjust.channel7[1] = 0;
  ChannelsBufferAdjust.channel8[1] = 0;
 
  printf("\nSending Matrix Information...%d %d\n",sizeBuffValue, sizeBuffInfo);
 
 // inicio a stream de audio  ----------------------------------------- PORTAUDIO    ASIO ---/

    err = Pa_OpenStream(
        &stream,
        NULL, /* no input */
        &outputParameters,
        SAMPLE_RATE,
        BUF_SIZE,
        paClipOff,      /* we won't output out of range samples so don't bother clipping them */
        patestCallback, 
        &data ); 

  if( err != paNoError ) goto error;

 // abre a comunicação com o portaudio/asio ---------------------------  PORTAUDIO   ASIO  ---/

  err = Pa_StartStream( stream );
  if( err != paNoError ) goto error;

  //system("PAUSE");

 // inicio parte principal -----------------------------------------------------------------/

  //double CodeIsRunning = 1;
  printf("\nEscrevendo nos canais de audio... Simulando em ATPRT_8channels...\n");

  //while(1){}

  Pa_Sleep( ComunicationOfInfo.ATPtimeStop *1000 );


  while(ComunicationOfInfo.ATPtime <= ComunicationOfInfo.ATPtimeStop)
  {
    Pa_Sleep( ComunicationOfInfo.ATPtimeStep *1000 );
    CopyMemory(&ComunicationOfInfo,(PVOID)pBuf_Info, sizeBuffInfo); //read
  }


  //      err = Pa_WriteStream( stream, buffer, BUF_SIZE );
  //      if( err != paNoError ) printf("\n...ERROR...underflow detect...ERROR..\n");

  finish:

    ComunicationOfInfo.final = true;

	Pa_Sleep( ComunicationOfInfo.ATPtimeStep *1000 );

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
  errorC:
    printf("\nO numero de canais precisa ser igual a 8\n");
    return err;
}