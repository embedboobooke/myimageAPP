
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <QtGui>
#include "bmp.h"
#include "list.h"
#include "imageviewer.h"

#define true 1
#define false 0
//typedef int bool;

long LineBytes;
RGBQUAD *ipRGB;
long m_dwHeight;
long m_dwWidth;
unsigned short m_wBitCount;
unsigned char *m_pImage;
FILE *fpSrc;
FILE *fpDest;
FileHeader bf;
InfoHeader bi;

bool m_b4AreaTracked = false;

int *area_size;      //记录每个细胞区域的面积
int *area_num_point; //记录每个细胞区域的周长
int label_of_img;    //记录细胞区域的个数
int m_iround;        //记录当前设置的细胞最小周长
int m_iMaxLabel;
int AreaNum;
int m_iAvgArea;
int m_iAvgRound;

char msg_round[256];//信息输出
char msg_area[256];//信息输出
int avg_round;//平均周长
int avg_area;//平均面积

void ImageViewer::fourTraceClick()
{
    fpSrc = fopen("binaryGroup.bmp", "rb");

    fread(&(bf.bfType), sizeof(bf.bfType), 1, fpSrc);
    fread(&(bf.bfSize), sizeof(bf.bfSize), 1, fpSrc);
    fread(&(bf.bfReserved1), sizeof(bf.bfReserved1), 1, fpSrc);
    fread(&(bf.bfReserved2), sizeof(bf.bfReserved2), 1, fpSrc);
    fread(&(bf.bfOffBits), sizeof(bf.bfOffBits), 1, fpSrc);

    fread(&(bi.biSize), sizeof(bi.biSize), 1, fpSrc);
    fread(&(bi.biWidth), sizeof(bi.biWidth), 1, fpSrc);
    fread(&(bi.biHeight), sizeof(bi.biHeight), 1, fpSrc);
    fread(&(bi.biPlanes), sizeof(bi.biPlanes), 1, fpSrc);
    fread(&(bi.biBitCount), sizeof(bi.biBitCount), 1, fpSrc);
    fread(&(bi.biCompression), sizeof(bi.biCompression), 1, fpSrc);
    fread(&(bi.biSizeImage), sizeof(bi.biSizeImage), 1, fpSrc);
    fread(&(bi.biXPelsPerMeter), sizeof(bi.biXPelsPerMeter), 1, fpSrc);
    fread(&(bi.biYPelsPerMeter), sizeof(bi.biYPelsPerMeter), 1, fpSrc);
    fread(&(bi.biClrUsed), sizeof(bi.biClrUsed), 1, fpSrc);
    fread(&(bi.biClrImportant), sizeof(bi.biClrImportant), 1, fpSrc);

    ipRGB = (RGBQUAD *)malloc((bf.bfOffBits - 54) * 1);
    fseek(fpSrc, 54, SEEK_SET);
    fread(ipRGB, bf.bfOffBits - 54, 1, fpSrc);

    LineBytes = (long)WIDTHBYTES(bi.biWidth * bi.biBitCount);
    m_pImage = (unsigned char *)malloc(bf.bfSize - bf.bfOffBits);
    fseek(fpSrc, bf.bfOffBits, SEEK_SET);
    fread(m_pImage, bf.bfSize - bf.bfOffBits, 1, fpSrc);

    m_dwWidth = bi.biWidth;
    m_dwHeight = bi.biHeight;
    m_wBitCount = bi.biBitCount;

    FourAreaTrackPlasm(0, 10000);

    free(m_pImage);
    free(ipRGB);

    //QPixmap dst("ContourFollowing.bmp");
    //imageLabel->setPixmap(dst);

    //QMessageBox::information(this, tr("information"), tr(msg_round).arg(fileName));

    /*QMessageBox msgBox;
    msgBox.setText(msg_round);
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    */
#if 0
    int ret = msgBox.exec();

    switch (ret) {
       case QMessageBox::Save:
           // Save was clicked
           break;
       case QMessageBox::Discard:
           // Don't Save was clicked
           break;
       case QMessageBox::Cancel:
           // Cancel was clicked
           break;
       default:
           // should never be reached
           break;
     }
#endif
}


//包含了最小值和最大值的4邻域边界跟踪细胞
void ImageViewer::FourAreaTrackPlasm(int minvalue, int maxvalue)
{
  double m_dCellCompact;//细胞紧凑度
  int m_iCellArea;      //细胞面积
  int m_iCellPerimeter; //细胞周长
  Boud_List *m_lp4AreaCellBoudList;  //保存4邻域跟踪后的链表信息，
                                     //该链表存储每个细胞构成像素点的所有坐标

  int i,j,m;

  int Min_Round = minvalue;
  int Max_Round = maxvalue;

  //起始点
  int StartX,StartY;

  //当前点
  int CurrentX,CurrentY;

  //前一点
  int PrevX,PrevY;

  //用来标定当前点的正上方和左边的点
  int fup,fleft;

  //该数组用来标定每一点所属的区域
  short img_label[240][320];

  short  img[240][320];

  //建立一个数组，用来判断图象中的点是否已经被选择为边界点,为真就是边界点，否则不是边界点
  bool boudary_points[240][320];

  //用来标记边界的个数
  int label;


  //法线方向
  int BeginDirection;

  //边界点的个数
  int NumPoint;


  //四邻域方向
  //当前的邻域方向如图
  //                 3
  //                 ^
  //                 |
  //        2<----当前点----> 0
  //                 |
  //                 V
  //                 1
  int Direction1[4][2] ={{0,1},{1,0},{0,-1},{-1,0}};

  //基本信息的初始化

  //可以考虑把图像分割为多个子图像，然后每个子图像分别进行轮廓的提取


  //图象的宽度和高度
  long lHeight,lWidth;

  lHeight = m_dwHeight;
  lWidth =  m_dwWidth;


  label = 1;

  unsigned char *lpImage=NULL;//指向图像像素的指针

  long lLineBytes = LineBytes;

  //需要把数字图象首先灰度化，然后进行阀值分割，再进行腐蚀掉小的噪声点，最后进行边界跟踪
  for(i=0;i<lHeight;i++)
  {
    for(j=0;j<lWidth;j++)
    {
      lpImage=m_pImage+lLineBytes*(lHeight-1-i)+j;

      img[i][j] = *lpImage;//转存到二维数组中

      img_label[i][j] = 0;//所有点都尚未标识区域

      boudary_points[i][j] = false;//尚无边界点


      if(img[i][j]==0)//黑点
      {
    if(i==0&&j!=0&&j!=319)//第1行，除了最左上角和最右上角的点
    {
      fup=255;//白点
      fleft=img[i][j-1];
    }

    if(j==0&&i!=0&&i!=239)//第1列，除了最左上角和最左下角的点
    {
      fleft=255;//白点
      fup = img[i-1][j];
    }

    if(i!=0&&j==319&&i!=239)//最后1列，除了最右上角和最右下角的点
    {
      fleft = img[i][j-1];
      fup = img[i-1][j];
    }

    if(i==239&&j!=0&&j!=319)//最后1行，除了最左下角和最右下角的点
    {
      fleft = img[i][j-1];
      fup = img[i-1][j];
    }

    if(i==0&&j==0)//最左上角的点
    {
      fleft=255;//白点
      fup=255;//白点
    }

    if(i!=0&&j!=0&&i!=239&&j!=319)//中间点
    {
      fup = img[i-1][j];
      fleft = img[i][j-1];
    }

    if(i==0&&j==319)//最右上角的点
    {
      fup = 255;
      fleft = img[i][j-1];
    }

    if(j==0&&i==239)//最左下角点
    {
      fup = img[i-1][j];
      fleft = 255;
    }

    if(j==319&&i==239)//最右下角点
    {
      fup = img[i-1][j];
      fleft = img[i][j-1];
    }

    if(fup==fleft&&fup==0)
      img_label[i][j] = img_label[i-1][j];//up point

    if(fleft==0&&fup==255)
      img_label[i][j] = img_label[i][j-1];//left point

    if(fleft==255&&fup==0)
      img_label[i][j] = img_label[i-1][j];//up point

    if(fup==fleft&&fup==0&&img_label[i-1][j]==img_label[i][j-1])
      img_label[i][j] = img_label[i-1][j];

    if(fup==fleft&&fup==255)//new area point
    {
      img_label[i][j]=label;//new area label
      label++;//label add 1
    }

    if(fup==fleft&&fup==0&&img_label[i-1][j]!=img_label[i][j-1])//标号往小值赶
    {
      if(img_label[i-1][j]>=img_label[i][j-1])//up > left
      {
        img_label[i][j]=img_label[i][j-1];//left point
        img_label[i-1][j]=img_label[i][j-1];//up point = left point
      }
      else//up < left
      {
        img_label[i][j]=img_label[i-1][j];//up point
        img_label[i][j-1]=img_label[i-1][j];//left point = up point
      }
    }

      }
    }
  }


  //获取第1次得到的区域的个数，这个值偏大
  int Max_label = label;


  //定义一个数组存放每个区域的起点的坐标
  //这个数组的大小要动态确定
  int start_point[76800][2];

  int *label_kind;
  label_kind = (int *)malloc((label + 1) * sizeof(int));

  int *area_point;
  area_point = (int *)malloc((label + 1) * sizeof(int));

  //这个数组用来计算区域的个数
  for(m=0;m<label+1;m++)
  {
    label_kind[m] = 0;
    area_point[m] = 0;
  }


  //顺序记录区域起点的坐标
  int flag=1;

  //标定区域的实际个数以及每个区域的起始坐标点
  //这个算法的运算量太大
  for(i=0;i<=239;i++)
  {
    for(j=0;j<=319;j++)
    {
      for(m=0;m<=Max_label;m++)
      {
    //物体点且标号值存在,必然是从小到大
    if(m==img_label[i][j]&&img[i][j]==0)
    {
      if(label_kind[m]==0)//第1次发现这种标号
      {
        //记录下的坐标
        start_point[flag][0]=i;
        start_point[flag][1]=j;
        flag++;//顺序前移
      }
      label_kind[m]++;//累加此种类型标号的数量
      break;
    }
      }
    }
  }

  //用来保存区域的面积的大小
  int *area_size;
  area_size = (int *)malloc((Max_label + 1) * sizeof(int));

  //计算区域的个数,这个区域数目也并非是真正的数目,其中有一些在物体中，
  //但和其他点的label不一样的点
  label=0;
  for(m=0;m<=Max_label;m++)
  {
    if(label_kind[m]>0)//该区域存在
    {
      label++;//区域数累加,it's not real label
    }

    area_size[m]=0;//every area's area is 0
  }


  //有多少个区域就要扫描多少次,注意它是从1开始的
  int scan_times=1;

  //用来移动数组area_point的指针
  int count=1;

  //用来计算所有区域的边界点的个数
  int total_points=0;

  int real_label;
  //对每一个区域分别进行4邻域的边界跟踪

  //armstrong added 把每个封闭区域的头链接起来
  Boud_List *listhead;
  listhead = (Boud_List *)malloc(sizeof(Boud_List));
  listhead->lnext = NULL;
  listhead->phead = NULL;
  //armstrong added 把每个封闭区域的头链接起来


  //这一段程序的耗时最多
  while(scan_times<=label)
  {
    //找到开始点
    StartX = start_point[scan_times][0];
    StartY = start_point[scan_times][1];

    //表示该区域不在其他区域之中，为独立区域，且从一个物体点开始
    if(label_kind[img_label[StartX][StartY]]>0&&img[StartX][StartY]==0)
    {
      boudary_points[StartX][StartY] = true;//开始点是边界点
      NumPoint = 1;//当前边界点个数

      //armstrong added 创建链表   11-10
      Boudary *head,*PrevP,*CurrentP,*pnew,*p;
      head = (Boudary *)malloc(sizeof(Boudary));

      //此处一定要这样处理,因为在数组中存放是逐行再逐列,数组中的行其实就是实际像素点所在的列,
      //而数组中的列其实就是实际像素所在的行
      head->x = StartY;
      head->y = StartX;
      head->next = NULL;
      head->head = head;
      //armstrong added 创建链表   11-10


      //找到边界的第2点
      for(i=0; i<4; i++)
      {
    if(img[StartX+Direction1[i][0]][StartY+Direction1[i][1]]==0)//非空白点
    {
      CurrentX =  StartX+Direction1[i][0];//记录当前点的坐标
      CurrentY =  StartY+Direction1[i][1];
      boudary_points[CurrentX][CurrentY] = true;//第2点也是边界点
      NumPoint++;//边界点个数递增

      //armstrong added   11-10
      p = (Boudary *)malloc(sizeof(Boudary));
      p->x = CurrentY;
      p->y = CurrentX;
      head->next = p;//和第1个结点链接起来
      p->next = NULL;
      p->head = head;
      //armstrong added  11-10

      break;
    }
      }
      //最初的开始点
      PrevX = StartX;
      PrevY = StartY;


      //armstrong  added 11-10
      PrevP = (Boudary *)malloc(sizeof(Boudary));
      PrevP = head;  //开始扫描前，前一个结点指向head结点
      CurrentP = (Boudary *)malloc(sizeof(Boudary));
      CurrentP = p;  //开始扫描前，当前结点指向第2个结点
      //armstrong  added 11-10


      //找其他的边界点
      if(i==4)
      {
    //该起始点是孤立点，请重新选择起点
      }
      else
      {
    while(1)//当找到起点
    {
      //边界已经闭合
      if((CurrentX == StartX)&&(CurrentY == StartY))
      {
        //边界跟踪成功完毕,要求记录此时区域的边界点的个数

        //边界点周长大于预设值
        if(NumPoint>Min_Round && NumPoint<Max_Round)
        {
          total_points = total_points + NumPoint;
          //这个数组中具体存放每个区域的周长
          area_point[count] = NumPoint;
          count++;

          //armstrong added  把每个封闭区域的头链接起来
          if(listhead->lnext==NULL)//只有头结点
          {
        Boud_List *blist;
        blist = (Boud_List *)malloc(sizeof(Boud_List));
        listhead->lnext = blist;//让头结点指向第1个结点
        blist->phead = head;//第1个结点指向的第1个封闭区域的头结点
        blist->lnext = NULL;
          }
          else//已经超过1个结点
          {
        Boud_List *blist,*newlist;
        blist = (Boud_List *)malloc(sizeof(Boud_List));
        blist = listhead;
        while(blist->lnext)//检索链表，直到某个结点的lnext为空
        {
          blist = blist->lnext;
        }

        newlist = (Boud_List *)malloc(sizeof(Boud_List));
        blist->lnext = newlist;//将该结点加入到链表的尾部
        newlist->phead = head;//并将该结点指向当前封闭区域的头结点
        newlist->lnext = NULL;
          }
          //armstrong added  把每个封闭区域的头链接起来

          //				break;
        }

        break;
      }
      else//找下一个边界点
      {
        //首先确定当前标准方向的外法线方向
        if((CurrentX ==PrevX)&&(CurrentY ==(PrevY+1)))
          BeginDirection = 3;//方向3

        if((CurrentX ==PrevX)&&(CurrentY ==(PrevY-1)))
          BeginDirection = 1;//方向1

        if((CurrentX ==(PrevX+1))&&(CurrentY ==PrevY))
          BeginDirection = 0;//方向0

        if((CurrentX ==(PrevX-1))&&(CurrentY ==PrevY))
          BeginDirection = 2;//方向2

        i=BeginDirection;//记录开始扫描方向
        while(1)
        {
          //外法线的方向为优先考虑
          if(img[CurrentX+Direction1[i][0]][CurrentY+Direction1[i][1]]==0&&CurrentX+Direction1[i][0]>=0&&CurrentX+Direction1[i][0]<240&&CurrentY+Direction1[i][1]>=0&&CurrentY+Direction1[i][1]<320)
          {

        //如果这个点尚未选中，即新的当前点是边界点
        if(!boudary_points[CurrentX+Direction1[i][0]][CurrentY+Direction1[i][1]])
        {
          //该点是目标点
          boudary_points[CurrentX+Direction1[i][0]][CurrentY+Direction1[i][1]] = true;

          //如果当前点的label值和四邻域中的点的label值不等，则校正四邻域中点label的值
          //并且置这种label的个数的值为0

          if(img_label[CurrentX+Direction1[i][0]][CurrentY+Direction1[i][1]]!=img_label[CurrentX][CurrentY])
          {
            //留着再扫描一遍，如果这个值变成了0，则相应的label值要变
            //记录需要修正的label值，将要更新的值直接给label_kind;
            //以后在扫描时，只要把这个值取反即可

            if(img_label[CurrentX][CurrentY] < img_label[CurrentX+Direction1[i][0]][CurrentY+Direction1[i][1]])
            {
              label_kind[img_label[CurrentX+Direction1[i][0]][CurrentY+Direction1[i][1]]] = -img_label[CurrentX][CurrentY];

              //当前边界点的label值立即被更新
              img_label[CurrentX+Direction1[i][0]][CurrentY+Direction1[i][1]] = img_label[CurrentX][CurrentY];
            }

          }

          //armstrong added 11-10
          pnew = (Boudary *)malloc(sizeof(Boudary));
          pnew->x = CurrentY+Direction1[i][1];
          pnew->y = CurrentX+Direction1[i][0];
          pnew->next = NULL;
          pnew->head = head;

          CurrentP->next = pnew;//把它加入到当前链表的尾部
          PrevP = CurrentP;//更新前一个结点指针
          CurrentP = pnew;//更新当前结点指针
          //armstrong added 11-10


          //边界点数目递增
          NumPoint++;
        }

        //修改前一点坐标
        PrevX = CurrentX;
        PrevY = CurrentY;

        //新的当前点
        CurrentX =  CurrentX+Direction1[i][0];
        CurrentY =  CurrentY+Direction1[i][1];
        break;
          }
          else
          {
        i=(i+1)%4;//循环寻找下一个点
          }
        }
      }
    }
      }
    }
    scan_times++;//扫描次数累加

  }//while  end

  //计算面积
  for(i=0; i<=239; i++)
  {
    for(j=0; j<=319; j++)
    {
      if(img[i][j]==0)
      {
    //区域中应该是相同label的点的label值都要更新
    if(label_kind[img_label[i][j]]<0)
    {
      img_label[i][j] = -label_kind[img_label[i][j]];
    }
    //区域的面积加1
    //例如区域的label值为2,此时area_size[2]加1
    area_size[img_label[i][j]]++;

    //is a boudary pix
    if (boudary_points[i][j])
    {
      lpImage = m_pImage + lLineBytes * (lHeight - i - 1) + j;
      *lpImage = (unsigned char)0;
    }
    else
    {
      lpImage = m_pImage + lLineBytes * (lHeight - i - 1) + j;
      *lpImage = (unsigned char)255;
    }
      }
      else
      {
    lpImage = m_pImage + lLineBytes * (lHeight - i - 1) + j;
    *lpImage = (unsigned char)255;
      }
    }
  }

  fpDest = fopen("ContourFollowing.bmp", "wb");

  fwrite(&(bf.bfType), sizeof(bf.bfType), 1, fpDest);
  fwrite(&(bf.bfSize), sizeof(bf.bfSize), 1, fpDest);
  fwrite(&(bf.bfReserved1), sizeof(bf.bfReserved1), 1, fpDest);
  fwrite(&(bf.bfReserved2), sizeof(bf.bfReserved2), 1, fpDest);
  fwrite(&(bf.bfOffBits), sizeof(bf.bfOffBits), 1, fpDest);

  fwrite(&(bi.biSize), sizeof(bi.biSize), 1, fpDest);
  fwrite(&(bi.biWidth), sizeof(bi.biWidth), 1, fpDest);
  fwrite(&(bi.biHeight), sizeof(bi.biHeight), 1, fpDest);
  fwrite(&(bi.biPlanes), sizeof(bi.biPlanes), 1, fpDest);
  fwrite(&(bi.biBitCount), sizeof(bi.biBitCount), 1, fpDest);
  fwrite(&(bi.biCompression), sizeof(bi.biCompression), 1, fpDest);
  fwrite(&(bi.biSizeImage), sizeof(bi.biSizeImage), 1, fpDest);
  fwrite(&(bi.biXPelsPerMeter), sizeof(bi.biXPelsPerMeter), 1, fpDest);
  fwrite(&(bi.biYPelsPerMeter), sizeof(bi.biYPelsPerMeter), 1, fpDest);
  fwrite(&(bi.biClrUsed), sizeof(bi.biClrUsed), 1, fpDest);
  fwrite(&(bi.biClrImportant), sizeof(bi.biClrImportant), 1, fpDest);

  fseek(fpDest, 54, SEEK_SET);
  fwrite(ipRGB, bf.bfOffBits - 54, 1, fpDest);

  fwrite(lpImage, bf.bfSize - bf.bfOffBits, 1, fpDest);

  QPixmap dst("ContourFollowing.bmp");
  imageLabel->setPixmap(dst);

  int total_area=0;

  int area_num=0;

  for(m=1;m<Max_label+1;m++)
  {
    //面积和阀值的比较
    if(area_size[m]>Min_Round &&area_size[m]<Max_Round)
    {
      total_area = total_area+area_size[m];
      area_num++;
    }
    else
    {
      //把所有label值等于m的面积中的点换为baidian
      area_size[m] = 0;

      /*
      if(remove)
      {
    for(i=0; i<=239; i++)
    {
      for(j=0; j<=319; j++)
      {
        if(img_label[i][j]==m)
        {
          lpImage = m_pImage+lLineBytes*(lHeight-1-i)+j;
          *lpImage = 255;
        }
      }
    }
      }
      */
    }
  }

  real_label = count-1;//真实的区域的个数

  if(real_label>0)
    avg_round =  total_points/real_label;//平均周长
  else
    avg_round = 0;

  if(area_num>0)
    avg_area =  total_area/area_num;//平均面积
  else
    avg_area = 0;

  //把几个关键值传到图像类的成员变量中
  if(real_label>0)
  {
      SetLabel(real_label);//设置符合周长要求的区域的个数
      SetNumPoint(area_point);//设置周长动态数组
      SetMinRound(Min_Round);//设置最小周长
      SetAvgRound(avg_round);//设置平均周长
      sprintf(msg_round,"周长大于%d小于%d的细胞区域%d个,平均周长%d个像素\n",\
                               Min_Round,Max_Round,real_label,avg_round);
      QMessageBox::information(this, tr("information"), tr(msg_round).arg(fileName));
  }


  if(area_num>0)
  {
      SetAreaNum(area_num);//设置符合要求的面积个数
      SetAreaSize(area_size,Max_label);//设置面积动态数组
      SetMaxLabel(Max_label);//设置最大的区域数目
      SetAvgArea(avg_area);//设置平均面积值
      sprintf(msg_area,"面积大于%d小于%d的细胞区域%d个,平均面积%d个像素\n",\
                               Min_Round,Max_Round,area_num,avg_area);
      QMessageBox::information(this, tr("information"), tr(msg_area).arg(fileName));
  }

  if(real_label==area_num&&area_num==1&&avg_area!=0)//只有1个区域
  {
       //细胞的紧凑度
       m_dCellCompact = (double)(avg_round*avg_round)/(double)avg_area;
       //细胞面积
       m_iCellArea = avg_area;
       //细胞周长
       m_iCellPerimeter = avg_round;
  }
  else
  {
    m_dCellCompact = 0;
    m_iCellArea = 0;
    m_iCellPerimeter = 0;
  }

  m_lp4AreaCellBoudList = new Boud_List;
  m_lp4AreaCellBoudList = listhead;//保存获取的细胞区域的链表结构


    Set4AreaTracked(true);//设置4邻域跟踪完成

  if(real_label>0&&area_num>0)//如果均满足要求则输出提示信息
  {
    strcat(msg_round,msg_area);

    //AfxMessageBox(msg_round,MB_ICONINFORMATION|MB_OK);
  }
  else
  {
    //AfxMessageBox("没有满足条件的区域!");
  }

  free(label_kind);
  free(area_point);
  free(area_size);
}

//设置面积动态数组,MaxLabel的值就是最大的label的值
void SetAreaSize(int *area, int MaxLabel)
{
  int i;
  area_size = (int *)malloc((MaxLabel + 1) * sizeof(int));
  for(i = 1; i <= MaxLabel; i++)
  {
    area_size[i] = area[i];
  }
}

//获取面积动态数组
int *GetAreaSize(void)
{
  return area_size;
}

//设置每个图像边界跟踪后得到的区域的个数
void SetLabel(int l)
{
  label_of_img = l;
}

//获取区域的个数
int GetLabel(void)
{
  return label_of_img;
}

//设置每个图像区域跟踪完毕后每个区域的周长
void SetNumPoint(int *a)
{
  int i;
  area_num_point = (int *)malloc((label_of_img+1) * sizeof(int));
  for(i=1; i<label_of_img+1; i++)
    area_num_point[i] = a[i];
}

//获取区域的周长
int *GetNumPoint(void)
{
  return area_num_point;
}

//设置边界跟踪中的最小周长
void SetMinRound(int r)
{
  m_iround = r;
}

//获取设置的最小周长
int GetMinRound(void)
{
  return m_iround;
}

//设置最大的label值
void SetMaxLabel(int l)
{
  m_iMaxLabel = l;
}

//获取最大的label值
int GetMaxLabel(void)
{
  return m_iMaxLabel;
}

//设置面积的数量
void SetAreaNum(int a)
{
  AreaNum = a;
}

//获取面积的数量
int GetAreaNum(void)
{
  return AreaNum;
}

//设置平均的面积值
void SetAvgArea(int avg)
{
  m_iAvgArea = avg;
}

//获取平均的面积值
int GetAvgArea(void)
{
  return m_iAvgArea;
}

//设置平均的周长值
void SetAvgRound(int avg)
{
  m_iAvgRound = avg;
}

//获取平均的周长值
int GetAvgRound(void)
{
  return m_iAvgRound;
}

void Set4AreaTracked(bool b)
{
  m_b4AreaTracked = b;
}


