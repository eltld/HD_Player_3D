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





#include "H264AVCVideoIoLib.h"
#include "WriteYuvToFile.h"
#include <stdlib.h>
#include <windows.h>


//#include "..\..\..\my_head_file.h"
extern void ConvertYUVtoRGB (unsigned char *src0, unsigned char *src1, unsigned char *src2,unsigned char *dst_ori,int width,int height);
extern bool saveBmp(char *bmpName, unsigned char *imgBuf, int width, int height, int biBitCount, RGBQUAD *pColorTable);


/*
extern unsigned char *buffer1;
extern int counter;
extern int counter1;
extern int counter2;
extern int flag[408];
*/

/*
//================================================
extern unsigned char * buffer_264;
extern int counter_264;
extern int counter_264_1;
extern int counter_264_2;
extern int flag_264[408];

extern unsigned char * Dec_Buffer_264;
extern BOOL volatile Decoding_264;
//================================================
*/

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

//=======================用于264视频播放的同步功能===========================
extern HANDLE h_264_Mutex[100];
extern int Dec_264_No_Decode_total;
extern int Dec_264_No_Display_total;
extern DWORD n_Thread_ID_Dec;//线程ID 全局变量
extern DWORD n_Thread_ID_Play;//线程ID 全局变量

#define WM_READY_TO_PLAY WM_USER+1
//=======================用于264视频播放的同步功能===========================

/*
//===========================================================================
//extern unsigned char Dec_Buffer_264[640*3072*3/2];
extern unsigned char Dec_Buffer_264[640*3072*3];
extern BOOL volatile Decoding_264;
extern int Decoded_Num_264;
//===========================================================================
*/




WriteYuvToFile::WriteYuvToFile()
: m_uiWidth         ( 0 )
, m_uiHeight        ( 0 )
, m_bInitDone       ( false )
, m_bFileInitDone   (false)
{
}


WriteYuvToFile::~WriteYuvToFile()
{
}



ErrVal WriteYuvToFile::create( WriteYuvIf*& rpcWriteYuv, const std::string& rcFileName )
{
  ROT( 0 == rcFileName.size() );

  WriteYuvToFile* pcWriteYuvToFile;

  pcWriteYuvToFile = new WriteYuvToFile;

  rpcWriteYuv = pcWriteYuvToFile;

  ROT( NULL == pcWriteYuvToFile )

  RNOKS( pcWriteYuvToFile->xInit( rcFileName ) );

  return Err::m_nOK;
}


ErrVal WriteYuvToFile::xInit( const std::string& rcFileName )
{
  if( Err::m_nOK != m_cFile.open( rcFileName, LargeFile::OM_WRITEONLY ) )
  { 
    std::cerr << "failed to open YUV output file " << rcFileName.data() << std::endl;
    return Err::m_nERR;
  }

  return Err::m_nOK;
}


ErrVal WriteYuvToFile::destroy()
{
  m_cTempBuffer.deleteData();
  
  if( m_cFile.is_open() )
  {
    RNOK( m_cFile.close() );
  }

  delete this;
  return Err::m_nOK;
}


ErrVal WriteYuvToFile::writeFrame( const UChar *pLum,
                             const UChar *pCb,
                             const UChar *pCr,
                             UInt uiLumHeight,
                             UInt uiLumWidth,
                             UInt uiLumStride )
{
  m_uiWidth  = uiLumWidth;
  m_uiHeight = uiLumHeight;

  RNOKS( xWriteFrame( pLum, pCb, pCr, uiLumHeight, uiLumWidth, uiLumStride ) );

  return Err::m_nOK;
}


ErrVal WriteYuvToFile::xWriteFrame( const UChar *pLum,
                              const UChar *pCb,
                              const UChar *pCr,
                              UInt uiHeight,
                              UInt uiWidth,
                              UInt uiStride )
{

  UInt    y;
  const UChar*  pucSrc;

  pucSrc = pLum;
  for( y = 0; y < uiHeight; y++ )
  {
    RNOK( m_cFile.write( pucSrc, uiWidth ) );
    pucSrc += uiStride;
  }

 
  uiStride >>= 1;
  uiHeight >>= 1;
  uiWidth  >>= 1;

  pucSrc = pCb;
  for( y = 0; y < uiHeight; y++ )
  {
    RNOK( m_cFile.write( pucSrc, uiWidth ) );
    pucSrc += uiStride;
  }


  pucSrc = pCr;
  for( y = 0; y < uiHeight; y++ )
  {
    RNOK( m_cFile.write( pucSrc, uiWidth ) );
    pucSrc += uiStride;
  }
  
  return Err::m_nOK;
}

ErrVal WriteYuvToFile::createMVC( WriteYuvIf*& rpcWriteYuv, const std::string& rcFileName, UInt numOfViews )
{
  ROT( 0 == rcFileName.size() );

  WriteYuvToFile* pcWriteYuvToFile;

  pcWriteYuvToFile = new WriteYuvToFile;

  rpcWriteYuv = pcWriteYuvToFile;

  ROT( NULL == pcWriteYuvToFile )

  return Err::m_nOK;
}


ErrVal WriteYuvToFile::xInitMVC( const std::string& rcFileName, UInt *vcOrder, 
                                 UInt uiNumOfViews )  // remove active view SEI
{

  std::string cFileName = rcFileName;
  std::string cTemp;
  char t[10];
  UInt view_id;

  m_cFileMVC = new LargeFile[uiNumOfViews];

  UInt pos = cFileName.rfind(".");
  cFileName.erase(pos);
  cTemp = cFileName.append("_");
  if (vcOrder == NULL) uiNumOfViews=0;
  for(UInt i = 0; i < uiNumOfViews; i++)
  {
    view_id = vcOrder ? vcOrder[i] : 0 ; // Dec. 1 fix 
	
    sprintf(t, "%d", view_id);
    cFileName = cTemp;
    cFileName.append(t);
    cFileName.append(".yuv");
    
//    if( Err::m_nOK != m_cFileMVC[i].open( cFileName, LargeFile::OM_WRITEONLY ) )
//    { 
//      std::cerr << "failed to open YUV output file " << cFileName.data() << std::endl;
//      return Err::m_nERR;
//    }
  }
  
  m_bFileInitDone = true;

  return Err::m_nOK;
}

ErrVal WriteYuvToFile::destroyMVC(UInt uiNumOfViews)
{
  m_cTempBuffer.deleteData();
 
  for(UInt i = 0; i < uiNumOfViews; i++)
  { 
    if( m_cFileMVC[i].is_open() )
    {
      RNOK( m_cFileMVC[i].close() );
    }
  }
  delete m_cFileMVC;

  delete this;
  return Err::m_nOK;
}

ErrVal WriteYuvToFile::writeFrame( const UChar *pLum,
                                   const UChar *pCb,
                                   const UChar *pCr,
                                   UInt uiLumHeight,
                                   UInt uiLumWidth,
                                   UInt uiLumStride,
                                   UInt ViewCnt)
{
  m_uiWidth  = uiLumWidth;
  m_uiHeight = uiLumHeight;

  
  RNOKS( xWriteFrame( pLum, pCb, pCr, uiLumHeight, uiLumWidth, uiLumStride, ViewCnt ) );

  return Err::m_nOK;
}


ErrVal WriteYuvToFile::xWriteFrame( const UChar *pLum,
                                    const UChar *pCb,
                                    const UChar *pCr,
                                    UInt uiHeight,
                                    UInt uiWidth,
                                    UInt uiStride, 
                                    UInt ViewCnt)
{

	UInt    y;
	const UChar*  pucSrc;

	extern int width_264,height_264;

	width_264 = uiWidth;
	height_264 = uiHeight;


	//=============================================================请求 Mutex
	if(counter_264==0)
	{
		::WaitForSingleObject(h_264_Mutex[Dec_264_No_Decode],INFINITE);
	}
	//=============================================================请求 Mutex

	pucSrc = pLum;//////////////////////////============================ Y
	for( y = 0; y < uiHeight; y++ )
	{
		//========================================================================================================
		//memcpy(buffer_264+ counter_264*width_264*height_264*3/2+y*uiWidth,pucSrc,uiWidth);
		memcpy(buffer_264
			+Dec_264_No_Decode*width_264*height_264*8*3/2
			+counter_264*width_264*height_264*3/2
			+y*uiWidth,pucSrc,uiWidth);
		//========================================================================================================
		pucSrc += uiStride;
	}


	uiStride >>= 1;
	uiHeight >>= 1;
	uiWidth  >>= 1;

	pucSrc = pCb;//////////////////////////============================ U
	for( y = 0; y < uiHeight; y++ )
	{
		//========================================================================================================
		//memcpy(buffer_264+ counter_264*width_264*height_264*3/2+width_264*height_264+y*uiWidth,pucSrc,uiWidth);
		memcpy(buffer_264
			+Dec_264_No_Decode*width_264*height_264*8*3/2
			+counter_264*width_264*height_264*3/2
			+width_264*height_264+y*uiWidth,pucSrc,uiWidth);
		//========================================================================================================
		pucSrc += uiStride;
	}


	pucSrc = pCr;//////////////////////////============================ V
	for( y = 0; y < uiHeight; y++ )
	{
		//========================================================================================================
		//memcpy(buffer_264+ counter_264*width_264*height_264*3/2+width_264*height_264*5/4+y*uiWidth,pucSrc,uiWidth);
		memcpy(buffer_264
			+Dec_264_No_Decode*width_264*height_264*8*3/2
			+counter_264*width_264*height_264*3/2
			+width_264*height_264*5/4+y*uiWidth,pucSrc,uiWidth);
		//========================================================================================================
		pucSrc += uiStride;
	}

	/*
	::ConvertYUVtoRGB(
		(unsigned char*)buffer_264,
		(unsigned char*)buffer_264+width_264*height_264,
		(unsigned char*)buffer_264+width_264*height_264*5/4,
		Dec_Buffer_264+counter_264*width_264*height_264*3,
		width_264,
		height_264);
	*/ // YUV2BGR 转移到 Display_264 函数中

	counter_264++;
	if(counter_264>=8)
	{
		/*
		::saveBmp("D:\\My Documents\\Visual Studio 2008\\Projects_201007\\test_264.bmp",
		Dec_Buffer_264,
		width_264,
		height_264*8,
		24,0);
		*/ //用以测试解码的YUV数据 结果转成BGR数据后 验证结果正确

		counter_264=0;

		//=============================================================释放 Mutex
		::ReleaseMutex(h_264_Mutex[Dec_264_No_Decode]);

		Dec_264_No_Decode++;
		Dec_264_No_Decode_total++;
		if(Dec_264_No_Decode>=Dec_264_No)
		{
			Dec_264_No_Decode=Dec_264_No_Decode%Dec_264_No;
		}

		//=======================================线程通信
		if(Dec_264_No_Decode_total==99)
		{
			::PostThreadMessage(n_Thread_ID_Play,WM_READY_TO_PLAY,0,0);
		}
		//=======================================线程通信
		//=============================================================释放 Mutex

		//=========================================================
		/*
		Dec_264_Decode_flag[Dec_264_No_Decode]++;

		Dec_264_No_Decode++;
		if(Dec_264_No_Decode>=Dec_264_No)
		{
			Dec_264_No_Decode=Dec_264_No_Decode%Dec_264_No;
		}
		*/
		//=========================================================
		
		//Decoding_264=0;//通知定时器
	}	


	//=================================== HJJ ===================================
	/*
	flag_264[counter_264]=1;
	counter_264++;

	if(counter_264==400)
	{
		counter_264=0;
		counter_264_1=400;
	}
	*/
	//=================================== HJJ ===================================


	return Err::m_nOK;
}

