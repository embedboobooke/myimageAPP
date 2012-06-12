#ifndef BMP_H
#define BMP_H

#define WIDTHBYTES(i) ((i + 31) / 32 * 4)

typedef struct tagFileHeader
{
  unsigned short bfType;
  unsigned int   bfSize;
  unsigned short bfReserved1;
  unsigned short bfReserved2;
  unsigned int   bfOffBits;
}FileHeader;

typedef struct tagInfoHeader
{
  unsigned int   biSize;
  long           biWidth;
  long           biHeight;
  unsigned short biPlanes;
  unsigned short biBitCount;
  unsigned int   biCompression;
  unsigned int   biSizeImage;
  long           biXPelsPerMeter;
  long           biYPelsPerMeter;
  unsigned int   biClrUsed;
  unsigned int   biClrImportant;
}InfoHeader;

typedef struct tagRGBQUAD
{
  unsigned char rgbBlue;
  unsigned char rgbGreen;
  unsigned char rgbReserved;
}RGBQUAD;

//包含了最小值和最大值的4邻域边界跟踪细胞
//void FourAreaTrackPlasm(int minvalue, int maxvalue);

void SetAreaSize(int *area, int MaxLabel);
int *GetAreaSize(void);
void SetLabel(int l);
int GetLabel(void);
void SetNumPoint(int *a);
int *GetNumPoint(void);
void SetMinRound(int r);
int GetMinRound(void);
void SetMaxLabel(int l);
int GetMaxLabel(void);
void SetAreaNum(int a);
int GetAreaNum(void);
void SetAvgArea(int avg);
int GetAvgArea(void);
void SetAvgRound(int avg);
int GetAvgRound(void);
void Set4AreaTracked(bool b);

#endif // BMP_H
