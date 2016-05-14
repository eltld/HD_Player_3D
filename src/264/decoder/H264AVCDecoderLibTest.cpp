/*
********************************************************************************

NOTE - One of the two copyright statements below may be chosen
       that applies for the software.

********************************************************************************

This software module was originally developed by

Heiko Schwarz    (Fraunhofer HHI),
Tobias Hinz      (Fraunhofer HHI),
Karsten Suehring (Fraunhofer HHI)

in the course of development of the ISO/IEC 14496-10:2005 Amd.1 (Scalable Video
Coding) for reference purposes and its performance may not have been optimized.
This software module is an implementation of one or more tools as specified by
the ISO/IEC 14496-10:2005 Amd.1 (Scalable Video Coding).

Those intending to use this software module in products are advised that its
use may infringe existing patents. ISO/IEC have no liability for use of this
software module or modifications thereof.

Assurance that the originally developed software module can be used
(1) in the ISO/IEC 14496-10:2005 Amd.1 (Scalable Video Coding) once the
ISO/IEC 14496-10:2005 Amd.1 (Scalable Video Coding) has been adopted; and
(2) to develop the ISO/IEC 14496-10:2005 Amd.1 (Scalable Video Coding): 

To the extent that Fraunhofer HHI owns patent rights that would be required to
make, use, or sell the originally developed software module or portions thereof
included in the ISO/IEC 14496-10:2005 Amd.1 (Scalable Video Coding) in a
conforming product, Fraunhofer HHI will assure the ISO/IEC that it is willing
to negotiate licenses under reasonable and non-discriminatory terms and
conditions with applicants throughout the world.

Fraunhofer HHI retains full right to modify and use the code for its own
purpose, assign or donate the code to a third party and to inhibit third
parties from using the code for products that do not conform to MPEG-related
ITU Recommendations and/or ISO/IEC International Standards. 

This copyright notice must be included in all copies or derivative works.
Copyright (c) ISO/IEC 2005. 

********************************************************************************

COPYRIGHT AND WARRANTY INFORMATION

Copyright 2005, International Telecommunications Union, Geneva

The Fraunhofer HHI hereby donate this source code to the ITU, with the following
understanding:
    1. Fraunhofer HHI retain the right to do whatever they wish with the
       contributed source code, without limit.
    2. Fraunhofer HHI retain full patent rights (if any exist) in the technical
       content of techniques and algorithms herein.
    3. The ITU shall make this code available to anyone, free of license or
       royalty fees.

DISCLAIMER OF WARRANTY

These software programs are available to the user without any license fee or
royalty on an "as is" basis. The ITU disclaims any and all warranties, whether
express, implied, or statutory, including any implied warranties of
merchantability or of fitness for a particular purpose. In no event shall the
contributor or the ITU be liable for any incidental, punitive, or consequential
damages of any kind whatsoever arising from the use of these programs.

This disclaimer of warranty extends to the user of these programs and user's
customers, employees, agents, transferees, successors, and assigns.

The ITU does not represent or warrant that the programs furnished hereunder are
free of infringement of any third-party patents. Commercial implementations of
ITU-T Recommendations, including shareware, may be subject to royalty fees to
patent holders. Information regarding the ITU-T patent policy is available from 
the ITU Web site at http://www.itu.int.

THIS IS NOT A GRANT OF PATENT RIGHTS - SEE THE ITU-T PATENT POLICY.

********************************************************************************
*/




#include "H264AVCDecoderLibTest.h"
#include "H264AVCDecoderTest.h"
#include <time.h>
#include <windows.h>


//===========================================================================
extern int counter_264_1;
extern int counter_264_2;
extern int flag_264[408];

//===========================================================================
extern char filename_264[200];
extern int counter_264;
extern int width_264;
extern int height_264;

extern BOOL volatile Decoding_264;
extern unsigned char * buffer_264;
extern unsigned char * Dec_Buffer_264;
//===========================================================================

//=======================用于264视频的缓冲播放模式===========================
extern int Dec_264_No;//100 与标志位一致
extern int Dec_264_Decode_flag[100];
extern int Dec_264_Display_flag[100];
extern int Dec_264_No_Decode;
extern int Dec_264_No_Display;
extern int Dec_264_Offset;
//=======================用于264视频的缓冲播放模式===========================



int
decoder_264()
{

	//counter_264=0;/////////////////////

	clock_t start,end;
	double duration;
	start = clock();

	int argc = 4;
	char **argv;
	argv = (char **)malloc(100);
	for(int i=0;i<4;i++)
	{
		argv[i] = (char *)malloc(100);
		ZeroMemory(argv[i],100);
	}
	
	argv[1] = filename_264;
	argv[2] = "E:\\264_DEC.yuv";
	argv[3] = "8";
	

  H264AVCDecoderTest* pcH264AVCDecoderTest = NULL;
  DecoderParameter    cParameter;

  printf("JMVC %s Decoder\n\n\n",_JMVC_VERSION_);

  RNOKRS( cParameter.init( argc, argv ), -2 );
//TMM_EC {{
	UInt	nCount	=	1;

  WriteYuvIf*                 pcWriteYuv;

  RNOKS( WriteYuvToFile::createMVC( pcWriteYuv, cParameter.cYuvFile, cParameter.uiNumOfViews ) );

  ReadBitstreamFile *pcReadBitstreamFile;
  RNOKS( ReadBitstreamFile::create( pcReadBitstreamFile ) ); 
  RNOKS( pcReadBitstreamFile->init( cParameter.cBitstreamFile ) );  
//TMM_EC }}
	for( UInt n = 0; n < nCount; n++ )
  {
    RNOKR( H264AVCDecoderTest::create   ( pcH264AVCDecoderTest ), -3 );
    RNOKR( pcH264AVCDecoderTest->init   ( &cParameter, (WriteYuvToFile*)pcWriteYuv, pcReadBitstreamFile ),          -4 );
    RNOKR( pcH264AVCDecoderTest->go     (),                       -5 );
    RNOKR( pcH264AVCDecoderTest->destroy(),                       -6 );
  }
//TMM_EC {{
	if( NULL != pcWriteYuv )              
  {
    RNOK( pcWriteYuv->destroy() );  
  }

  if( NULL != pcReadBitstreamFile )     
  {
    RNOK( pcReadBitstreamFile->uninit() );  
    RNOK( pcReadBitstreamFile->destroy() );  
  }
//TMM_EC }}
	end = clock();
	duration = (end -start)/CLOCKS_PER_SEC;
	printf("the total time is %f sec\n",duration);

  return 0;
}
