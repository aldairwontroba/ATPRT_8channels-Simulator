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

//Ajuste de amplitude para cada canal
#define amplitude1 0.865 
#define amplitude2 0.865
#define amplitude3 0.9
#define amplitude4 0.9
#define amplitude5 0.887
#define amplitude6 0.887
#define amplitude7 0.887 
#define amplitude8 0.887

//Ajuste de fase para cada canal : o erro é <0.5 graus para 60Hz
#define atraso1 0.0 //seconds
#define atraso2 0.0 //seconds 
#define atraso3 0.00045 //seconds
#define atraso4 0.00045 //seconds
#define atraso5 0.00022 //seconds
#define atraso6 0.00022 //seconds
#define atraso7 0.00022 //seconds
#define atraso8 0.00023 //seconds

int atrasochannel1HIGH; 
int atrasochannel2HIGH; 
int atrasochannel3HIGH; 
int atrasochannel4HIGH; 
int atrasochannel5HIGH; 
int atrasochannel6HIGH; 
int atrasochannel7HIGH; 
int atrasochannel8HIGH; 

double atrasochannel1LOW;
double atrasochannel2LOW;
double atrasochannel3LOW;
double atrasochannel4LOW;
double atrasochannel5LOW;
double atrasochannel6LOW;
double atrasochannel7LOW;
double atrasochannel8LOW;

double timestepaudio = 1.0/SAMPLE_RATE;

double AudiotimeCh1 = 0;
double AudiotimeCh2 = 0;
double AudiotimeCh3 = 0;
double AudiotimeCh4 = 0;
double AudiotimeCh5 = 0;
double AudiotimeCh6 = 0;
double AudiotimeCh7 = 0;
double AudiotimeCh8 = 0;

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
    double channel1[BUF_SIZE*2];
    double channel2[BUF_SIZE*2];
    double channel3[BUF_SIZE*2];
    double channel4[BUF_SIZE*2];
    double channel5[BUF_SIZE*2];
    double channel6[BUF_SIZE*2];
    double channel7[BUF_SIZE*2];
    double channel8[BUF_SIZE*2];
    double ATPtimeV[BUF_SIZE*2];
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
    double tecla1;
    double tecla2;
    double tecla3;
    double tecla4;  
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


    int CanIstart = no;
    while(CanIstart){ 
      CopyMemory(&ComunicationOfInfo,(PVOID)pBuf_Info, sizeBuffInfo); //read
      if (ComunicationOfInfo.final) return paComplete;
      if(ComunicationOfInfo.CanIRead == yes){
        CanIstart = yes;
      }
      if(kbhit()){
        char tecla = getch();
        switch(tecla){

          case 'q':
            ComunicationOfInfo.tecla1 = 1;
          break;
          case 'w':
            ComunicationOfInfo.tecla1 = 2;
          break;
          case 'e':
            ComunicationOfInfo.tecla1 = 3;
          break;
          case 'r':
            ComunicationOfInfo.tecla1 = 4;
          break;
          case 't':
            ComunicationOfInfo.tecla1 = 5;
          break;
          case 'p':
            ComunicationOfInfo.tecla1 = 0;
          break;


          case 'a':
            ComunicationOfInfo.tecla2 = 1;
          break;
          case 's':
            ComunicationOfInfo.tecla2 = 2;
          break;
          case 'd':
            ComunicationOfInfo.tecla2 = 3;
          break;
          case 'f':
            ComunicationOfInfo.tecla2 = 4;
          break;
          case 'g':
            ComunicationOfInfo.tecla2 = 5;
          break;
          case 'h':
            ComunicationOfInfo.tecla2 = 6;
          break;
          case 'j':
            ComunicationOfInfo.tecla2 = 7;
          break;
          case 'k':
            ComunicationOfInfo.tecla2 = 8;
          break;
          case 'l':
            ComunicationOfInfo.tecla2 = 0;
          break;


          case 'z':
            ComunicationOfInfo.tecla3 = 0;
          break;
          case 'x':
            ComunicationOfInfo.tecla3 = 1;
          break;
          case 'c':
            ComunicationOfInfo.tecla3 = 2;
          break;


          case '1':
            ComunicationOfInfo.tecla4 = 1;
          break;
          case '2':
            ComunicationOfInfo.tecla4 = 2;
          break;
          case '3':
            ComunicationOfInfo.tecla4 = 3;
          break;
          case '4':
            ComunicationOfInfo.tecla4 = 4;
          break;
          case '5':
            ComunicationOfInfo.tecla4 = 5;
          break;
          case '6':
            ComunicationOfInfo.tecla4 = 6;
          break;
          case '7':
            ComunicationOfInfo.tecla4 = 7;
          break;
          case '8':
            ComunicationOfInfo.tecla4 = 8;
          break;
          case '9':
            ComunicationOfInfo.tecla4 = 9;
          break;
          case '0':
            ComunicationOfInfo.tecla4 = 0;
          break;

          default:

          break;
        }
    }
    }

    CopyMemory( &ChannelsBufferRecived, (PVOID)pBuf_Value, sizeBuffValue); //read
    
    int position = (int)ComunicationOfInfo.bufferC;

    //printf("position %d\n", position);

    for( i=0; i<=position; i++ )
    { 
      ChannelsBufferAdjust.channel1[i+3+atrasochannel1HIGH] = ChannelsBufferRecived.channel1[i] * amplitude1 /ComunicationOfInfo.MAX1;
	    ChannelsBufferAdjust.channel2[i+3+atrasochannel2HIGH] = ChannelsBufferRecived.channel2[i] * amplitude2 /ComunicationOfInfo.MAX2;
	    ChannelsBufferAdjust.channel3[i+3+atrasochannel3HIGH] = ChannelsBufferRecived.channel3[i] * amplitude3 /ComunicationOfInfo.MAX3;
	    ChannelsBufferAdjust.channel4[i+3+atrasochannel4HIGH] = ChannelsBufferRecived.channel4[i] * amplitude4 /ComunicationOfInfo.MAX4;
	    ChannelsBufferAdjust.channel5[i+3+atrasochannel5HIGH] = ChannelsBufferRecived.channel5[i] * amplitude5 /ComunicationOfInfo.MAX5;
	    ChannelsBufferAdjust.channel6[i+3+atrasochannel6HIGH] = ChannelsBufferRecived.channel6[i] * amplitude6 /ComunicationOfInfo.MAX6;
	    ChannelsBufferAdjust.channel7[i+3+atrasochannel7HIGH] = ChannelsBufferRecived.channel7[i] * amplitude7 /ComunicationOfInfo.MAX7;
	    ChannelsBufferAdjust.channel8[i+3+atrasochannel8HIGH] = ChannelsBufferRecived.channel8[i] * amplitude8 /ComunicationOfInfo.MAX8;
    
    	ChannelsBufferAdjust.ATPtimeV[i+3] = ComunicationOfInfo.ATPtimeV[i];

      // printf("ATPtimeV %.8f\n",  ComunicationOfInfo.ATPtimeV[i]);
	    //system("PAUSE");
    }

    ComunicationOfInfo.CanIRead = no; // buffer vazio, espero preencher
    ComunicationOfInfo.CanIWrite = yes; // digo ao outro software que ele pode escrever

    CopyMemory((PVOID)pBuf_Info, &ComunicationOfInfo, sizeBuffInfo); //write

    int jch1 = 1;
    int jch2 = 1;
    int jch3 = 1;
    int jch4 = 1;
    int jch5 = 1;
    int jch6 = 1;
    int jch7 = 1;
    int jch8 = 1;

	 // é feito uma reamostragem do sinal 
	for( i=0; i<framesPerBuffer; i++ ){
			//printf("jch1 %d\n", jch1);
			//printf("AudiotimeCh1 %.8f\n", AudiotimeCh1);

		if(AudiotimeCh1>=ChannelsBufferAdjust.ATPtimeV[jch1+1]){
			jch1++;
		}
    if(AudiotimeCh2>=ChannelsBufferAdjust.ATPtimeV[jch2+1]){
      jch2++;
    }
    if(AudiotimeCh3>=ChannelsBufferAdjust.ATPtimeV[jch3+1]){
      jch3++;
    }
    if(AudiotimeCh4>=ChannelsBufferAdjust.ATPtimeV[jch4+1]){
      jch4++;
    }
    if(AudiotimeCh5>=ChannelsBufferAdjust.ATPtimeV[jch5+1]){
      jch5++;
    }
    if(AudiotimeCh6>=ChannelsBufferAdjust.ATPtimeV[jch6+1]){
      jch6++;
    }
    if(AudiotimeCh7>=ChannelsBufferAdjust.ATPtimeV[jch7+1]){
      jch7++;
    }
    if(AudiotimeCh8>=ChannelsBufferAdjust.ATPtimeV[jch8+1]){
      jch8++;
    }
	    	//printf("jch1 %d\n", jch1);
			  //printf("ChannelsBufferAdjust.ATPtimeV %.8f\n", ChannelsBufferAdjust.ATPtimeV[jch1]);
		  	//system("PAUSE");

		double deltatimeCh1 = (AudiotimeCh1-ChannelsBufferAdjust.ATPtimeV[jch1])/ComunicationOfInfo.ATPtimeStep;
		double deltatimeCh2 = (AudiotimeCh2-ChannelsBufferAdjust.ATPtimeV[jch2])/ComunicationOfInfo.ATPtimeStep;
		double deltatimeCh3 = (AudiotimeCh3-ChannelsBufferAdjust.ATPtimeV[jch3])/ComunicationOfInfo.ATPtimeStep;
		double deltatimeCh4 = (AudiotimeCh4-ChannelsBufferAdjust.ATPtimeV[jch4])/ComunicationOfInfo.ATPtimeStep;
		double deltatimeCh5 = (AudiotimeCh5-ChannelsBufferAdjust.ATPtimeV[jch5])/ComunicationOfInfo.ATPtimeStep;
		double deltatimeCh6 = (AudiotimeCh6-ChannelsBufferAdjust.ATPtimeV[jch6])/ComunicationOfInfo.ATPtimeStep;
		double deltatimeCh7 = (AudiotimeCh7-ChannelsBufferAdjust.ATPtimeV[jch7])/ComunicationOfInfo.ATPtimeStep;
		double deltatimeCh8 = (AudiotimeCh8-ChannelsBufferAdjust.ATPtimeV[jch8])/ComunicationOfInfo.ATPtimeStep;

		data->buffer[i][0] = ((ChannelsBufferAdjust.channel1[jch1+1]-ChannelsBufferAdjust.channel1[jch1])*deltatimeCh1) + ChannelsBufferAdjust.channel1[jch1];
		data->buffer[i][1] = ((ChannelsBufferAdjust.channel2[jch2+1]-ChannelsBufferAdjust.channel2[jch2])*deltatimeCh2) + ChannelsBufferAdjust.channel2[jch2];
		data->buffer[i][2] = ((ChannelsBufferAdjust.channel3[jch3+1]-ChannelsBufferAdjust.channel3[jch3])*deltatimeCh3) + ChannelsBufferAdjust.channel3[jch3];
		data->buffer[i][3] = ((ChannelsBufferAdjust.channel4[jch4+1]-ChannelsBufferAdjust.channel4[jch4])*deltatimeCh4) + ChannelsBufferAdjust.channel4[jch4];
		data->buffer[i][4] = ((ChannelsBufferAdjust.channel5[jch5+1]-ChannelsBufferAdjust.channel5[jch5])*deltatimeCh5) + ChannelsBufferAdjust.channel5[jch5];
		data->buffer[i][5] = ((ChannelsBufferAdjust.channel6[jch6+1]-ChannelsBufferAdjust.channel6[jch6])*deltatimeCh6) + ChannelsBufferAdjust.channel6[jch6];
		data->buffer[i][6] = ((ChannelsBufferAdjust.channel7[jch7+1]-ChannelsBufferAdjust.channel7[jch7])*deltatimeCh7) + ChannelsBufferAdjust.channel7[jch7];
		data->buffer[i][7] = ((ChannelsBufferAdjust.channel8[jch8+1]-ChannelsBufferAdjust.channel8[jch8])*deltatimeCh8) + ChannelsBufferAdjust.channel8[jch8];
		
		AudiotimeCh1 += timestepaudio;
    AudiotimeCh2 += timestepaudio;
    AudiotimeCh3 += timestepaudio;
    AudiotimeCh4 += timestepaudio;
    AudiotimeCh5 += timestepaudio;
    AudiotimeCh6 += timestepaudio;
    AudiotimeCh7 += timestepaudio;
    AudiotimeCh8 += timestepaudio;
	}

	
	//printf("j %d\n", j);
	//printf("ATPtimeV %.8f\n", ChannelsBufferAdjust.ATPtimeV[j]);
	//printf("Audiotime %.8f\n", Audiotime);
	//system("PAUSE");

  for( i=0; i<3; i++)
  {
    //Level 0 padrao
    ChannelsBufferAdjust.channel1[i]=ChannelsBufferAdjust.channel1[position+1+i];
    ChannelsBufferAdjust.channel2[i]=ChannelsBufferAdjust.channel2[position+1+i];
    ChannelsBufferAdjust.channel3[i]=ChannelsBufferAdjust.channel3[position+1+i];
    ChannelsBufferAdjust.channel4[i]=ChannelsBufferAdjust.channel4[position+1+i];
    ChannelsBufferAdjust.channel5[i]=ChannelsBufferAdjust.channel5[position+1+i];
    ChannelsBufferAdjust.channel6[i]=ChannelsBufferAdjust.channel6[position+1+i];
    ChannelsBufferAdjust.channel7[i]=ChannelsBufferAdjust.channel7[position+1+i];
    ChannelsBufferAdjust.channel8[i]=ChannelsBufferAdjust.channel8[position+1+i];
    ChannelsBufferAdjust.ATPtimeV[i]=ChannelsBufferAdjust.ATPtimeV[position+1+i];
  }
  //Level for 3... compensação de fase
	int countastrasoCh1 = atrasochannel1HIGH;
	int countastrasoCh2 = atrasochannel2HIGH;
	int countastrasoCh3 = atrasochannel3HIGH;
	int countastrasoCh4 = atrasochannel4HIGH;
	int countastrasoCh5 = atrasochannel5HIGH;
	int countastrasoCh6 = atrasochannel6HIGH;
	int countastrasoCh7 = atrasochannel7HIGH;
	int countastrasoCh8 = atrasochannel8HIGH;

	i=3;
    while((countastrasoCh1+countastrasoCh2+countastrasoCh3+countastrasoCh4+countastrasoCh5+countastrasoCh6+countastrasoCh7+countastrasoCh8) >=1 )
    {
    	if(countastrasoCh1 >= 1){
    		ChannelsBufferAdjust.channel1[i]=ChannelsBufferAdjust.channel1[position+i+1];
    		countastrasoCh1--;
    	}
    	if(countastrasoCh2 >= 1){
			ChannelsBufferAdjust.channel2[i]=ChannelsBufferAdjust.channel2[position+i+1];
			countastrasoCh2--;
    	}
    	if(countastrasoCh3 >= 1){
			ChannelsBufferAdjust.channel3[i]=ChannelsBufferAdjust.channel3[position+i+1];
			countastrasoCh3--;
    	}
    	if(countastrasoCh4 >= 1){
			ChannelsBufferAdjust.channel4[i]=ChannelsBufferAdjust.channel4[position+i+1];
			countastrasoCh4--;
    	}
    	if(countastrasoCh5 >= 1){
			ChannelsBufferAdjust.channel5[i]=ChannelsBufferAdjust.channel5[position+i+1];
			countastrasoCh5--;
    	}
    	if(countastrasoCh6 >= 1){
			ChannelsBufferAdjust.channel6[i]=ChannelsBufferAdjust.channel6[position+i+1];
			countastrasoCh6--;
    	}
    	if(countastrasoCh7 >= 1){
			ChannelsBufferAdjust.channel7[i]=ChannelsBufferAdjust.channel7[position+i+1];
			countastrasoCh7--;
    	}
    	if(countastrasoCh8 >= 1){
			ChannelsBufferAdjust.channel8[i]=ChannelsBufferAdjust.channel8[position+i+1];
			countastrasoCh8--;
    	}
    	i++;
    }
// escrevo nos canais de audio
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
      //exit(1);
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
      //exit(1);
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
      //exit(1);
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
      //exit(1);
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
  outputParameters.suggestedLatency = 0.02; /* ignored by Pa_IsFormatSupported() */
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

  ChannelsBufferAdjust.ATPtimeV[0] = 0;
  ChannelsBufferAdjust.ATPtimeV[1] = 0;

  atrasochannel1HIGH = (int)(atraso1 / ComunicationOfInfo.ATPtimeStep);
  atrasochannel2HIGH = (int)(atraso2 / ComunicationOfInfo.ATPtimeStep);
  atrasochannel3HIGH = (int)(atraso3 / ComunicationOfInfo.ATPtimeStep);
  atrasochannel4HIGH = (int)(atraso4 / ComunicationOfInfo.ATPtimeStep);
  atrasochannel5HIGH = (int)(atraso5 / ComunicationOfInfo.ATPtimeStep);
  atrasochannel6HIGH = (int)(atraso6 / ComunicationOfInfo.ATPtimeStep);
  atrasochannel7HIGH = (int)(atraso7 / ComunicationOfInfo.ATPtimeStep);
  atrasochannel8HIGH = (int)(atraso8 / ComunicationOfInfo.ATPtimeStep);

  atrasochannel1LOW = (atraso1 - atrasochannel1HIGH * ComunicationOfInfo.ATPtimeStep);
  atrasochannel2LOW = (atraso2 - atrasochannel2HIGH * ComunicationOfInfo.ATPtimeStep);
  atrasochannel3LOW = (atraso3 - atrasochannel3HIGH * ComunicationOfInfo.ATPtimeStep);
  atrasochannel4LOW = (atraso4 - atrasochannel4HIGH * ComunicationOfInfo.ATPtimeStep);
  atrasochannel5LOW = (atraso5 - atrasochannel5HIGH * ComunicationOfInfo.ATPtimeStep);
  atrasochannel6LOW = (atraso6 - atrasochannel6HIGH * ComunicationOfInfo.ATPtimeStep);
  atrasochannel7LOW = (atraso7 - atrasochannel7HIGH * ComunicationOfInfo.ATPtimeStep);
  atrasochannel8LOW = (atraso8 - atrasochannel8HIGH * ComunicationOfInfo.ATPtimeStep);

  AudiotimeCh1 = - atrasochannel1LOW;
  AudiotimeCh2 = - atrasochannel2LOW;
  AudiotimeCh3 = - atrasochannel3LOW;
  AudiotimeCh4 = - atrasochannel4LOW;
  AudiotimeCh5 = - atrasochannel5LOW;
  AudiotimeCh6 = - atrasochannel6LOW;
  AudiotimeCh7 = - atrasochannel7LOW;
  AudiotimeCh8 = - atrasochannel8LOW;

  printf("atrasochannel1HIGH %d\n", atrasochannel1HIGH);
  printf("atrasochannel1LOW %.8f\n", atrasochannel1LOW);
  printf("AudiotimeCh8 %.8f\n", AudiotimeCh8);

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
    Pa_Sleep( ComunicationOfInfo.ATPtimeStep *1000 * BUF_SIZE);
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
  	system("PAUSE");
    return err;
}