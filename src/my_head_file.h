#include "stdafx.h"
#include "HD_Player_3D.h"
#include "HD_Player_3DDlg.h"

#include <math.h>
#include <stdio.h>
#include <fstream>
#include <Windows.h>
#include <cstdio>
#include <string.h>
#include <cstring>
#include <iostream>
using namespace std;

const UINT nID_Timer=111;
const UINT nID_Timer_Mpeg2=222;
const UINT nID_Timer_Mpeg2_Play=2222;

const UINT nID_Timer_264_Decode=333;
const UINT nID_Timer_264_Play=3333;
const UINT nID_Timer_264_Play_Buffer=33333;

const UINT nID_Timer_YUV_Read=444;

//|||||||||||||||||||||||||||||||||||||||||||==========================================YUV->RGB转换的函数
unsigned char *clp = NULL;
unsigned char *clp1;
long int crv_tab[256];
long int cbu_tab[256];
long int cgu_tab[256];
long int cgv_tab[256];
long int tab_76309[256];

//===========================================YUV420转RGB算法
void init_dither_tab ()
{
  long int crv, cbu, cgu, cgv;
  int i;

  crv = 104597;
  cbu = 132201;                 /* fra matrise i global.h */
  cgu = 25675;
  cgv = 53279;

  for (i = 0; i < 256; i++)
  {
    crv_tab[i] = (i - 128) * crv;
    cbu_tab[i] = (i - 128) * cbu;
    cgu_tab[i] = (i - 128) * cgu;
    cgv_tab[i] = (i - 128) * cgv;
    tab_76309[i] = 76309 * (i - 16);
  }
  if (!(clp = (unsigned char *)malloc(sizeof(unsigned char)*1024)))
  {
    AfxMessageBox(_T("安排解码码表内存失败"));
  }
  clp1 = clp;

  clp += 384;

  for (i = -384; i < 640; i++)
    clp[i] = (i < 0) ? 0 : ((i > 255) ? 255 : i);
}
//===========================================YUV420转RGB算法

void ConvertYUVtoRGB (unsigned char *src0, unsigned char *src1, unsigned char *src2,unsigned char *dst_ori,int width,int height)
{

  int y11, y21;
  int y12, y22;
  int y13, y23;
  int y14, y24;
  int u, v;
  int i, j;
  int c11, c21, c31, c41;
  int c12, c22, c32, c42;
  unsigned int DW;
  unsigned int *id1, *id2;
  unsigned char *py1, *py2, *pu, *pv;
  unsigned char *d1, *d2;

  d1 = dst_ori;
  d1 += width * height * 3 - width * 3;
  d2 = d1 - width * 3;

  py1 = src0;
  pu = src1;
  pv = src2;
  py2 = py1 + width;

  id1 = (unsigned int *) d1;
  id2 = (unsigned int *) d2;

  for (j = 0; j < height; j += 2)
  {
    /* line j + 0 */
    for (i = 0; i < width; i += 4)
    {
      u = *pu++;
      v = *pv++;
      c11 = crv_tab[v];
      c21 = cgu_tab[u];
      c31 = cgv_tab[v];
      c41 = cbu_tab[u];
      u = *pu++;
      v = *pv++;
      c12 = crv_tab[v];
      c22 = cgu_tab[u];
      c32 = cgv_tab[v];
      c42 = cbu_tab[u];

      y11 = tab_76309[*py1++];  /* (255/219)*65536 */
      y12 = tab_76309[*py1++];
      y13 = tab_76309[*py1++];  /* (255/219)*65536 */
      y14 = tab_76309[*py1++];

      y21 = tab_76309[*py2++];
      y22 = tab_76309[*py2++];
      y23 = tab_76309[*py2++];
      y24 = tab_76309[*py2++];

      /* RGBR */
      DW = ((clp[(y11 + c41) >> 16])) |
        ((clp[(y11 - c21 - c31) >> 16]) << 8) |
        ((clp[(y11 + c11) >> 16]) << 16) |
        ((clp[(y12 + c41) >> 16]) << 24);
      *id1++ = DW;

      /* GBRG */
      DW = ((clp[(y12 - c21 - c31) >> 16])) |
        ((clp[(y12 + c11) >> 16]) << 8) |
        ((clp[(y13 + c42) >> 16]) << 16) |
        ((clp[(y13 - c22 - c32) >> 16]) << 24);
      *id1++ = DW;

      /* BRGB */
      DW = ((clp[(y13 + c12) >> 16])) |
        ((clp[(y14 + c42) >> 16]) << 8) |
        ((clp[(y14 - c22 - c32) >> 16]) << 16) |
        ((clp[(y14 + c12) >> 16]) << 24);
      *id1++ = DW;

      /* RGBR */
      DW = ((clp[(y21 + c41) >> 16])) |
        ((clp[(y21 - c21 - c31) >> 16]) << 8) |
        ((clp[(y21 + c11) >> 16]) << 16) |
        ((clp[(y22 + c41) >> 16]) << 24);
      *id2++ = DW;

      /* GBRG */
      DW = ((clp[(y22 - c21 - c31) >> 16])) |
        ((clp[(y22 + c11) >> 16]) << 8) |
        ((clp[(y23 + c42) >> 16]) << 16) |
        ((clp[(y23 - c22 - c32) >> 16]) << 24);
      *id2++ = DW;

      /* BRGB */
      DW = ((clp[(y23 + c12) >> 16])) |
        ((clp[(y24 + c42) >> 16]) << 8) |
        ((clp[(y24 - c22 - c32) >> 16]) << 16) |
        ((clp[(y24 + c12) >> 16]) << 24);
      *id2++ = DW;
    }
    id1 -= (9 * width) >> 2;
    id2 -= (9 * width) >> 2;
    py1 += width;
    py2 += width;
  }
}
//|||||||||||||||||||||||||||||||||||||||||||==========================================YUV->RGB转换的函数

//|||||||||||||||||||||||||||||||||||||||||||==========================================位图文件的读写函数
//==============================================================================
//typedef unsigned char       BYTE;
BYTE *pBmpBuf;//读入图像数据的指针
RGBQUAD *pColorTable;//颜色表指针

int bmpWidth;//图像的宽
int bmpHeight;//图像的高
int biBitCount;//图像类型
int biSize;//图像文件大小

//==============================================================================
/***********************************************************************
* 函数名称：
* readBmp()
*
*函数参数：
*  char *bmpName -文件名字及路径
*
*返回值：
*   0为失败,1为成功
*
*说明：给定一个图像文件名及其路径，读图像的位图数据、宽、高、颜色表及每像素
*      位数等数据进内存,存放在相应的全局变量中
***********************************************************************/
bool readBmp(char *bmpName)
{
	//二进制读方式打开指定的图像文件
    FILE *fp=fopen(bmpName,"rb");
	if(fp==0) return 0;
	
	
//	//跳过位图文件头结构BITMAPFILEHEADER
//	fseek(fp, sizeof(BITMAPFILEHEADER),0);
	BITMAPFILEHEADER head_file;
	fread(&head_file,sizeof(BITMAPFILEHEADER),1,fp);
	biSize=head_file.bfSize;	
	
	//定义位图信息头结构变量，读取位图信息头进内存，存放在变量head中
	BITMAPINFOHEADER head_info;  
	fread(&head_info, sizeof(BITMAPINFOHEADER), 1,fp); 
	
	//获取图像宽、高、每像素所占位数等信息
	bmpWidth = head_info.biWidth;
	bmpHeight = head_info.biHeight;
	biBitCount = head_info.biBitCount;
	
	//定义变量，计算图像每行像素所占的字节数（必须是4的倍数）
	int lineByte=(bmpWidth * biBitCount/8+3)/4*4;
	
	//灰度图像有颜色表，且颜色表表项为256
	if(biBitCount==8){
		//申请颜色表所需要的空间，读颜色表进内存
		pColorTable=new RGBQUAD[256];
		fread(pColorTable,sizeof(RGBQUAD),256,fp);
	}
	
	//申请位图数据所需要的空间，读位图数据进内存
//	pBmpBuf=new unsigned char[lineByte * bmpHeight];
	pBmpBuf=new BYTE[lineByte * bmpHeight];
	fread(pBmpBuf,1,lineByte * bmpHeight,fp);
	
	//关闭文件
	fclose(fp);
	
	return 1;
}

/***********************************************************************
* 函数名称：
* saveBmp()
*
*函数参数：
*  char *bmpName -文件名字及路径
*  unsigned char *imgBuf  -待存盘的位图数据
*  int width   -像素为单位待存盘位图的宽
*  int  height  -像素为单位待存盘位图高
*  int biBitCount   -每像素所占位数
*  RGBQUAD *pColorTable  -颜色表指针

*返回值：
*   0为失败,1为成功
*
*说明：给定一个图像位图数据、宽、高、颜色表指针及每像素所占的位数等信息，
*      将其写到指定文件中
***********************************************************************/
bool saveBmp(char *bmpName, unsigned char *imgBuf, int width, int height, 
			 int biBitCount, RGBQUAD *pColorTable)
{
	//如果位图数据指针为0,则没有数据传入,函数返回
	if(!imgBuf)
		return 0;
	
	//颜色表大小,以字节为单位,灰度图像颜色表为1024字节,彩色图像颜色表大小为0
	int colorTablesize=0;
	if(biBitCount==8)
		colorTablesize=1024;

	//待存储图像数据每行字节数为4的倍数
	int lineByte=(width * biBitCount/8+3)/4*4;
	
	//以二进制写的方式打开文件
	FILE *fp=fopen(bmpName,"wb");
	if(fp==0) return 0;
	
	//申请位图文件头结构变量，填写文件头信息
	BITMAPFILEHEADER fileHead;
	fileHead.bfType = 0x4D42;//bmp类型
	
	//bfSize是图像文件4个组成部分之和
	fileHead.bfSize= sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
		+ colorTablesize + lineByte*height;
	fileHead.bfReserved1 = 0;
	fileHead.bfReserved2 = 0;
	
	//bfOffBits是图像文件前三个部分所需空间之和
	fileHead.bfOffBits=54+colorTablesize;
	
	//写文件头进文件
	fwrite(&fileHead, sizeof(BITMAPFILEHEADER),1, fp);
	
	//申请位图信息头结构变量，填写信息头信息
	BITMAPINFOHEADER head; 
	head.biBitCount=biBitCount;
	head.biClrImportant=0;
	head.biClrUsed=0;
	head.biCompression=0;
	head.biHeight=height;
	head.biPlanes=1;
	head.biSize=40;
	head.biSizeImage=lineByte*height;
	head.biWidth=width;
	head.biXPelsPerMeter=0;
	head.biYPelsPerMeter=0;
	//写位图信息头进内存
	fwrite(&head, sizeof(BITMAPINFOHEADER),1, fp);
	
	//如果灰度图像,有颜色表,写入文件 
	if(biBitCount==8)
		fwrite(pColorTable, sizeof(RGBQUAD),256, fp);
	
	//写位图数据进文件
	fwrite(imgBuf, height*lineByte, 1, fp);
	
	//关闭文件
	fclose(fp);
	
	return 1;
}
//|||||||||||||||||||||||||||||||||||||||||||==========================================位图文件的读写函数



//=================================================钩子 HOOK
HHOOK g_hKeyBoard=NULL;
LRESULT CALLBACK KeyBoardProc(int code,WPARAM wParam,LPARAM lParam)
{
	if(wParam==VK_ESCAPE)
	{
		return 1;
	}
	else
	{
		return ::CallNextHookEx(g_hKeyBoard,code,wParam,lParam);
	}	
}

HHOOK g_hMouse=NULL;
LRESULT CALLBACK MouseProc(int code,WPARAM wParam,LPARAM lParam)
{
	if(wParam==WM_RBUTTONDOWN)
	{
		::AfxMessageBox(_T("鼠标右键按下"));		
		return 1;
	}
	else
	{
		return ::CallNextHookEx(g_hMouse,code,wParam,lParam);
	}	
}
//=================================================钩子 HOOK
