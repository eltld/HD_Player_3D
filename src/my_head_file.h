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

//|||||||||||||||||||||||||||||||||||||||||||==========================================YUV->RGBת���ĺ���
unsigned char *clp = NULL;
unsigned char *clp1;
long int crv_tab[256];
long int cbu_tab[256];
long int cgu_tab[256];
long int cgv_tab[256];
long int tab_76309[256];

//===========================================YUV420תRGB�㷨
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
    AfxMessageBox(_T("���Ž�������ڴ�ʧ��"));
  }
  clp1 = clp;

  clp += 384;

  for (i = -384; i < 640; i++)
    clp[i] = (i < 0) ? 0 : ((i > 255) ? 255 : i);
}
//===========================================YUV420תRGB�㷨

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
//|||||||||||||||||||||||||||||||||||||||||||==========================================YUV->RGBת���ĺ���

//|||||||||||||||||||||||||||||||||||||||||||==========================================λͼ�ļ��Ķ�д����
//==============================================================================
//typedef unsigned char       BYTE;
BYTE *pBmpBuf;//����ͼ�����ݵ�ָ��
RGBQUAD *pColorTable;//��ɫ��ָ��

int bmpWidth;//ͼ��Ŀ�
int bmpHeight;//ͼ��ĸ�
int biBitCount;//ͼ������
int biSize;//ͼ���ļ���С

//==============================================================================
/***********************************************************************
* �������ƣ�
* readBmp()
*
*����������
*  char *bmpName -�ļ����ּ�·��
*
*����ֵ��
*   0Ϊʧ��,1Ϊ�ɹ�
*
*˵��������һ��ͼ���ļ�������·������ͼ���λͼ���ݡ����ߡ���ɫ��ÿ����
*      λ�������ݽ��ڴ�,�������Ӧ��ȫ�ֱ�����
***********************************************************************/
bool readBmp(char *bmpName)
{
	//�����ƶ���ʽ��ָ����ͼ���ļ�
    FILE *fp=fopen(bmpName,"rb");
	if(fp==0) return 0;
	
	
//	//����λͼ�ļ�ͷ�ṹBITMAPFILEHEADER
//	fseek(fp, sizeof(BITMAPFILEHEADER),0);
	BITMAPFILEHEADER head_file;
	fread(&head_file,sizeof(BITMAPFILEHEADER),1,fp);
	biSize=head_file.bfSize;	
	
	//����λͼ��Ϣͷ�ṹ��������ȡλͼ��Ϣͷ���ڴ棬����ڱ���head��
	BITMAPINFOHEADER head_info;  
	fread(&head_info, sizeof(BITMAPINFOHEADER), 1,fp); 
	
	//��ȡͼ����ߡ�ÿ������ռλ������Ϣ
	bmpWidth = head_info.biWidth;
	bmpHeight = head_info.biHeight;
	biBitCount = head_info.biBitCount;
	
	//�������������ͼ��ÿ��������ռ���ֽ�����������4�ı�����
	int lineByte=(bmpWidth * biBitCount/8+3)/4*4;
	
	//�Ҷ�ͼ������ɫ������ɫ�����Ϊ256
	if(biBitCount==8){
		//������ɫ������Ҫ�Ŀռ䣬����ɫ����ڴ�
		pColorTable=new RGBQUAD[256];
		fread(pColorTable,sizeof(RGBQUAD),256,fp);
	}
	
	//����λͼ��������Ҫ�Ŀռ䣬��λͼ���ݽ��ڴ�
//	pBmpBuf=new unsigned char[lineByte * bmpHeight];
	pBmpBuf=new BYTE[lineByte * bmpHeight];
	fread(pBmpBuf,1,lineByte * bmpHeight,fp);
	
	//�ر��ļ�
	fclose(fp);
	
	return 1;
}

/***********************************************************************
* �������ƣ�
* saveBmp()
*
*����������
*  char *bmpName -�ļ����ּ�·��
*  unsigned char *imgBuf  -�����̵�λͼ����
*  int width   -����Ϊ��λ������λͼ�Ŀ�
*  int  height  -����Ϊ��λ������λͼ��
*  int biBitCount   -ÿ������ռλ��
*  RGBQUAD *pColorTable  -��ɫ��ָ��

*����ֵ��
*   0Ϊʧ��,1Ϊ�ɹ�
*
*˵��������һ��ͼ��λͼ���ݡ����ߡ���ɫ��ָ�뼰ÿ������ռ��λ������Ϣ��
*      ����д��ָ���ļ���
***********************************************************************/
bool saveBmp(char *bmpName, unsigned char *imgBuf, int width, int height, 
			 int biBitCount, RGBQUAD *pColorTable)
{
	//���λͼ����ָ��Ϊ0,��û�����ݴ���,��������
	if(!imgBuf)
		return 0;
	
	//��ɫ���С,���ֽ�Ϊ��λ,�Ҷ�ͼ����ɫ��Ϊ1024�ֽ�,��ɫͼ����ɫ���СΪ0
	int colorTablesize=0;
	if(biBitCount==8)
		colorTablesize=1024;

	//���洢ͼ������ÿ���ֽ���Ϊ4�ı���
	int lineByte=(width * biBitCount/8+3)/4*4;
	
	//�Զ�����д�ķ�ʽ���ļ�
	FILE *fp=fopen(bmpName,"wb");
	if(fp==0) return 0;
	
	//����λͼ�ļ�ͷ�ṹ��������д�ļ�ͷ��Ϣ
	BITMAPFILEHEADER fileHead;
	fileHead.bfType = 0x4D42;//bmp����
	
	//bfSize��ͼ���ļ�4����ɲ���֮��
	fileHead.bfSize= sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
		+ colorTablesize + lineByte*height;
	fileHead.bfReserved1 = 0;
	fileHead.bfReserved2 = 0;
	
	//bfOffBits��ͼ���ļ�ǰ������������ռ�֮��
	fileHead.bfOffBits=54+colorTablesize;
	
	//д�ļ�ͷ���ļ�
	fwrite(&fileHead, sizeof(BITMAPFILEHEADER),1, fp);
	
	//����λͼ��Ϣͷ�ṹ��������д��Ϣͷ��Ϣ
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
	//дλͼ��Ϣͷ���ڴ�
	fwrite(&head, sizeof(BITMAPINFOHEADER),1, fp);
	
	//����Ҷ�ͼ��,����ɫ��,д���ļ� 
	if(biBitCount==8)
		fwrite(pColorTable, sizeof(RGBQUAD),256, fp);
	
	//дλͼ���ݽ��ļ�
	fwrite(imgBuf, height*lineByte, 1, fp);
	
	//�ر��ļ�
	fclose(fp);
	
	return 1;
}
//|||||||||||||||||||||||||||||||||||||||||||==========================================λͼ�ļ��Ķ�д����



//=================================================���� HOOK
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
		::AfxMessageBox(_T("����Ҽ�����"));		
		return 1;
	}
	else
	{
		return ::CallNextHookEx(g_hMouse,code,wParam,lParam);
	}	
}
//=================================================���� HOOK
