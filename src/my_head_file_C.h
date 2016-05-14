#include <windows.h>

int main(int argc,char *argv[],char * InputFilePath,unsigned char * OutputBuffer);

char * InputFilePath;

HANDLE hThread_Mpeg2Dec;

HANDLE hThread_264Dec;
HANDLE hThread_264_Play;


//#define WM_READY_TO_PLAY WM_USER+1


