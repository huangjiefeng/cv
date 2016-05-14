

#include "CSnapScreen.h"
#include "jpeglib.h"
#include <io.h>
#include <stdio.h>

ScreenSnapping::ScreenSnapping()
{
	_imageBuffer = NULL;
	_imageBufferLength = 0;

	isLoop=true;
	qualityLevel=50;
	setPosition(0,0,0,0);
	setFrameRate(1); //每秒的帧频

	_jpegFilePath = "";

}


ScreenSnapping::~ScreenSnapping()
{
	if( _imageBuffer )
	{
		delete _imageBuffer;
		_imageBuffer = NULL;
	}
	DeleteFile( _jpegFilePath.c_str() );
}

void ScreenSnapping::deleverImage(BYTE * image,long length)
{

	//if(pFilter->m_pOutputPin->IsConnected()==false) return ;
	//BYTE * pBuffer;
	//BOOL hr=pFilter->GetSampleBuffer(&pBuffer);
	//if(!hr) return ;
	//memcpy(pBuffer,image,length);
	//pFilter->DeliverHoldingSample(length);
}
void ScreenSnapping::setPosition(int inLeft,int inUp,int inRight,int inDown)
{
	bool captureWindowChange = false;
	if( inLeft!=left||inUp!=top||inRight!=right||inDown!=bottom )
		captureWindowChange = true;

	if( captureWindowChange )
	{
		
		left=inLeft;
		top=inUp;
		right=inRight;
		bottom=inDown;

		printf("Reset Capture Window Area[%d,%d,%d,%d]\n" , left , top , right , bottom );
		if( _imageBuffer )
		{
			//释放原有的缓存
			delete _imageBuffer;
			_imageBuffer = NULL;
			_imageBufferLength = 0;
		}

		if( left>=0 && top>=0 && right>=0 &&bottom>=0 &&getWidth()>0 && getHeight()>0 )
		{
			printf("Window width:%d , hight:%d\n" , this->getWidth() , this->getHeight() );
			_imageBufferLength = this->getWidth()*this->getHeight()*3;
			_imageBuffer = new BYTE[ _imageBufferLength ];
			ZeroMemory( _imageBuffer , _imageBufferLength );
		}
	}

}


void ScreenSnapping::setFrameRate(int framerate)
{
	frameRate=framerate;
}



long ScreenSnapping::snap(BYTE * image)
{
	
	long length=0;
	

	if( _imageBuffer )
	{
		HBITMAP hBitmap=copyScreen2Bitmap();
		length=getJPEGfromBitmap(hBitmap,_imageBuffer);
		DeleteObject(hBitmap);
	}

	return length;
}


long ScreenSnapping::snapScreen()
{
	long length=0;
	
	if( _imageBuffer )
	{
		ZeroMemory( _imageBuffer , _imageBufferLength );
		HBITMAP hBitmap=copyScreen2Bitmap();
		if( hBitmap==NULL )
		{
			printf("BMP截屏失败！\n");
		}

		length=getJPEGfromBitmap(hBitmap,_imageBuffer);
		//this->SaveBitmapToFile( hBitmap , "D:\\bitmap.bmp" );
		_imageLength = length;
		DeleteObject(hBitmap);
		return length;
	}else
	{
		printf("图片缓存为空！\n");
	}

	return -1;
}

void ScreenSnapping::RGB32toRGB24(BYTE * image,int width,int height)
{
	BYTE * temp=new BYTE[width*height*3];

	for(int i=0,j=0;i<width*height*4;i+=4,j+=3)
	{
		temp[j]=image[i];
		temp[j+1]=image[i+1];
		temp[j+2]=image[i+2];
	}
	memcpy(image,temp,width*height*3);

	delete []temp;
//	for(long i=0,j=0;i<width*height*4;i+=4,j+=3)
//	{
//		temp[j]=image[i+2];
//		temp[j+1]=image[i+1];
//		temp[j+2]=image[i];
//	}
//	    
//   //逐行翻转过来
//	for(int i=0;i<height;i++)
//		memcpy(image+i*width*3,temp+(height-1-i)*width*3,width*3);
//	delete []temp;
}

HBITMAP ScreenSnapping::copyScreen2Bitmap()
{
	
	// 屏幕和内存设备描述表
	HDC       hScrDC = NULL, hMemDC = NULL;      
	// 位图句柄
	HBITMAP    hBitmap, hOldBitmap;   
	
	// 选定区域坐标
	int       nX, nY, nX2, nY2;      

	// 位图宽度和高度
	int       nWidth, nHeight;      
	
    // 屏幕分辨率
	int       xScrn, yScrn; 	

	DWORD deviceNum = 0;
	DISPLAY_DEVICE device;
	device.cb = sizeof(DISPLAY_DEVICE );
	// 获得选定区域坐标
	nX = left;
	nY = top;
	nX2 = right;
	nY2 = bottom;

	//选取显示器
	//while( EnumDisplayDevices(NULL , deviceNum++ , &device,NULL ) )
	//{
	//	hScrDC = CreateDC( device.DeviceName , NULL , NULL , NULL );
	//	// 获得屏幕分辨率
	//	xScrn = GetDeviceCaps(hScrDC, HORZRES);
	//	yScrn = GetDeviceCaps(hScrDC, VERTRES);
	//	
	//	//如果左边界的x坐标大于或等于当前屏幕分辨率，则选择下一个device(存在多个屏幕)
	//	//同时两个x坐标都减去当前的屏幕水平分辨率
	//	if( nX>=xScrn )
	//	{
	//		printf("第%d块显示器的分辨率%d,%d", deviceNum,  xScrn, yScrn );
	//		nX -= xScrn;
	//		nX2 -= xScrn;
	//		printf("当前的窗口坐标%d,%d,%d,%d\n" , nX , nY , nX2, nY2 );
 //           DeleteDC(hScrDC);
	//	}else
	//	{
	//		//采用当前的设备
	//		break;
	//	}
	//}
    hScrDC = GetDC(NULL);
	if( hScrDC==NULL )
	{
		printf("获取屏幕设备描述表时发生错误!\n");
		return NULL;
	}

	//printf("对第%d块显示器进行操作，名称为%s\n", deviceNum , device.DeviceName );
	printf("窗口坐标为%d,%d,%d,%d\n" , nX , nY , nX2, nY2 );


	////为屏幕创建设备描述表
	////hScrDC = CreateDC("DISPLAY", NULL, NULL, NULL);
	////int iBits = GetDeviceCaps(hScrDC, BITSPIXEL) * GetDeviceCaps(hScrDC, PLANES);

	//为屏幕设备描述表创建兼容的内存设备描述表
	hMemDC = CreateCompatibleDC(hScrDC);


	// 获得屏幕分辨率
	//xScrn = GetDeviceCaps(hScrDC, HORZRES);
	//yScrn = GetDeviceCaps(hScrDC, VERTRES);

	//printf("屏幕分辨率:%d x %d\n" , xScrn , yScrn );
	////确保选定区域是可见的
	//if (nX<0)	nX = 0;
	//if (nY<0)	nY = 0;
	//if (nX2>xScrn)
	//{
	//	nX2 = xScrn;
	//	right=xScrn;
	//}
	//if (nY2>yScrn)
	//{
	//	nY2 = yScrn;
	//	bottom=yScrn;
	//}

	nWidth = nX2 - nX;
	nHeight = nY2 - nY;

	// 创建一个与屏幕设备描述表兼容的位图
	hBitmap = CreateCompatibleBitmap(hScrDC, nWidth, nHeight);
	// 把新位图选到内存设备描述表中
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
	// 把屏幕设备描述表拷贝到内存设备描述表中
	//BitBlt(hMemDC, 0, 0, 320, 240,hScrDC, nX, nY, SRCCOPY);
	BitBlt(hMemDC, 0, 0, nWidth, nHeight,hScrDC, nX, nY, SRCCOPY);
	//得到屏幕位图的句柄
	hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);	
	//清除 
	DeleteDC(hScrDC);
	DeleteDC(hMemDC);
	DeleteObject(hOldBitmap);
	// 返回位图句柄
	return hBitmap;
}

void ScreenSnapping::jpegCodec(int image_width,int image_height, BYTE * inRGBBuffer,BYTE * outImage,long & length)
{
	struct jpeg_compress_struct cinfo;
	
	struct jpeg_error_mgr jerr;
	/* More stuff */

	//FILE * outfile;		/* target file */
	JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
	int row_stride;		/* physical row width in image buffer */

	
	cinfo.err = jpeg_std_error(&jerr);
	/* Now we can initialize the JPEG compression object. */
	jpeg_create_compress(&cinfo);
	
	/*if ((outfile = fopen(filename, "wb")) == NULL) {
		fprintf(stderr, "can't open %s\n", filename);
		exit(1);
	}*/
	
	FILE * outfile = NULL;
	if( _jpegFilePath!="" )
	{	
		//保存jpeg文件
		outfile=fopen(_jpegFilePath.c_str(),"wb");
		if (outfile==NULL) 
		{
			printf("打开文件[%s]失败\n" ,_jpegFilePath.c_str());
			return;
		}

	}

	jpeg_stdio_dest(&cinfo, outfile);
	//jpeg_stdio_dest(&cinfo, outImage);
	
	cinfo.image_width = image_width; 	/* image width and height, in pixels */
	cinfo.image_height = image_height;
	cinfo.input_components = 3;		/* # of color components per pixel */
	cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */
	
	jpeg_set_defaults(&cinfo);
	
	jpeg_set_quality(&cinfo, qualityLevel, TRUE /* limit to baseline-JPEG values */);

	
	jpeg_start_compress(&cinfo, TRUE);


	//由于BMP图像是翻转的，每个像素的表示变成了BGR，因此需要进行像素置换
	for( int i=0 ; i<image_width*image_height*3; i+=3 )
	{
		BYTE temp = *(inRGBBuffer+i);
		*(inRGBBuffer+i)=*(inRGBBuffer+i+2);
		*(inRGBBuffer+i+2)=temp;
	}



	row_stride = image_width * 3;	/* JSAMPLEs per row in image_buffer */
	while (cinfo.next_scanline < cinfo.image_height) {
		//由于jpg文件的图像是倒的，所以改了一下读的顺序
		row_pointer[0] = &inRGBBuffer[(cinfo.image_height - cinfo.next_scanline - 1) * row_stride];
		//row_pointer[0] = & inRGBBuffer[cinfo.next_scanline * row_stride];
		(void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}

	
	jpeg_finish_compress(&cinfo);
	

	length = _filelength(fileno(outfile));

	fclose(outfile );

	outfile = fopen( _jpegFilePath.c_str() , "rb" );
	if( outfile!=NULL )
	{
		int size = fread( outImage , length , 1 , outfile );
	}

	jpeg_destroy_compress(&cinfo);
	
	fclose(outfile);
	
}


long ScreenSnapping::getJPEGfromBitmap(HBITMAP hBitmap,BYTE * outImage)
{
	 if(hBitmap==NULL ||outImage==NULL) return 0;

 /*
	  //lpFileName 为位图文件名
	HDC            hDC;         
	//设备描述表
	int            iBits;      
	//当前显示分辨率下每个像素所占字节数
	WORD            wBitCount;   
	//位图中每个像素所占字节数
	//定义调色板大小， 位图中像素字节大小 ，
	//位图文件大小 ， 写入文件字节数
	DWORD           dwPaletteSize=0,dwBmBitsSize;
	BITMAP          Bitmap;        
    
	//位图文件头结构
	BITMAPINFOHEADER   bi;            
	//位图信息头结构 
	LPBITMAPINFOHEADER lpbi;          
	//指向位图信息头结构
	HANDLE           hPal,hOldPal=NULL;
	//定义文件，分配内存句柄，调色板句柄

	//计算位图文件每个像素所占字节数
	hDC = CreateDC("DISPLAY",NULL,NULL,NULL);
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
	DeleteDC(hDC);

	if(iBits<16) return 0;
	wBitCount=iBits;

	//计算调色板大小
	if (wBitCount<= 8)
		dwPaletteSize = (1<<wBitCount) *
		sizeof(RGBQUAD);

	//设置位图信息头结构
	GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap);
	bi.biSize            = sizeof(BITMAPINFOHEADER);
	bi.biWidth           = Bitmap.bmWidth;
	bi.biHeight          = Bitmap.bmHeight;
	bi.biPlanes          = 1;
	bi.biBitCount         = wBitCount;
	bi.biCompression      = BI_RGB;
	bi.biSizeImage        = 0;
	bi.biXPelsPerMeter     = 0;
	bi.biYPelsPerMeter     = 0;
	bi.biClrUsed         = 0;
	bi.biClrImportant      = 0;

	dwBmBitsSize = ((Bitmap.bmWidth *wBitCount+31)/32)* 4*Bitmap.bmHeight ;
	//为位图内容分配内存
	//hDib  = GlobalAlloc(GHND,dwBmBitsSize+dwPaletteSize);

	//lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	lpbi=(LPBITMAPINFOHEADER) new BYTE[dwBmBitsSize+dwPaletteSize];

	*lpbi = bi;

	//// 处理调色板   
	hPal = GetStockObject(DEFAULT_PALETTE);
	if (hPal)
	{
		hDC  = GetDC(NULL);
		hOldPal = SelectPalette(hDC,(HPALETTE) hPal, FALSE);
		RealizePalette(hDC);
	}
	// 获取该调色板下新的像素值
	int temp=GetDIBits(hDC, hBitmap, 0, (UINT) Bitmap.bmHeight,	(LPSTR)lpbi,(BITMAPINFO *)	&bi, DIB_RGB_COLORS);		

	 if (hOldPal)
	{
      SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);
      RealizePalette(hDC);
      ReleaseDC(NULL, hDC);
    }

	long length=0;   	
    if(wBitCount==32)	RGB32toRGB24((BYTE *)lpbi,Bitmap.bmWidth,Bitmap.bmHeight);
    if(wBitCount==16)  ;
*/
	 HDC hDC;
	 //当前分辨率下每象素所占字节数
	 int iBits;
	 //位图中每象素所占字节数
	 WORD wBitCount;
	 //定义调色板大小， 位图中像素字节大小 ，位图文件大小 ， 写入文件字节数 
	 DWORD dwPaletteSize=0, dwBmBitsSize=0, dwDIBSize=0, dwWritten=0; 
	 //位图属性结构 
	 BITMAP Bitmap;  
	 //位图文件头结构
	 BITMAPFILEHEADER bmfHdr;  
	 //位图信息头结构 
	 BITMAPINFOHEADER bi;  
	 //指向位图信息头结构  
	 LPBITMAPINFOHEADER lpbi;  
	 //定义文件，分配内存句柄，调色板句柄 
	 HANDLE fh, hDib, hPal,hOldPal=NULL; 
	 //压缩成jpeg后的图片数据大小
	 long length = 0;

	 //计算位图文件每个像素所占字节数 
	 hDC = CreateDC("DISPLAY", NULL, NULL, NULL);
	 iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES); 
	 DeleteDC(hDC); 
	 if (iBits <= 1)  wBitCount = 1; 
	 else if (iBits <= 4)  wBitCount = 4; 
	 else if (iBits <= 8)  wBitCount = 8; 
	 else      wBitCount = 24; 

	 GetObject(hBitmap, sizeof(Bitmap), (LPSTR)&Bitmap);
	 bi.biSize   = sizeof(BITMAPINFOHEADER);
	 bi.biWidth   = Bitmap.bmWidth;
	 bi.biHeight   = Bitmap.bmHeight;
	 bi.biPlanes   = 1;
	 bi.biBitCount  = wBitCount;
	 bi.biCompression = BI_RGB;
	 bi.biSizeImage  = 0;
	 bi.biXPelsPerMeter = 0;
	 bi.biYPelsPerMeter = 0;
	 bi.biClrImportant = 0;
	 bi.biClrUsed  = 0;

	 dwBmBitsSize = ((Bitmap.bmWidth * wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;

	 //为位图内容分配内存 
	 hDib = GlobalAlloc(GHND,dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER)); 
	 lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib); 
	 *lpbi = bi;

	 // 处理调色板  
	 hPal = GetStockObject(DEFAULT_PALETTE); 
	 if (hPal) 
	 { 
		 hDC = ::GetDC(NULL); 
		 hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE); 
		 RealizePalette(hDC); 
	 }

	 // 获取该调色板下新的像素值 
	 GetDIBits(hDC, hBitmap, 0, (UINT) Bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER) 
		 +dwPaletteSize, (BITMAPINFO *)lpbi, DIB_RGB_COLORS); 

	 //恢复调色板  
	 if (hOldPal) 
	 { 
		 ::SelectPalette(hDC, (HPALETTE)hOldPal, TRUE); 
		 RealizePalette(hDC); 
		 ::ReleaseDC(NULL, hDC); 
	 }


	 // 设置位图文件头 
	 bmfHdr.bfType = 0x4D42; // "BM" 
	 dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;  
	 bmfHdr.bfSize = dwDIBSize; 
	 bmfHdr.bfReserved1 = 0; 
	 bmfHdr.bfReserved2 = 0; 
	 bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize; 

     //jpeg压缩
	jpegCodec(Bitmap.bmWidth,Bitmap.bmHeight,(BYTE *)lpbi+ sizeof(BITMAPINFOHEADER)+dwPaletteSize,outImage,length);
	
	//清除  
	GlobalUnlock(hDib); 
	GlobalFree(hDib); 

	return length;
}

int ScreenSnapping::SaveBitmapToFile( HBITMAP hBitmap , LPSTR lpFileName )
{     
	HDC hDC;
	//当前分辨率下每象素所占字节数
	int iBits;
	//位图中每象素所占字节数
	WORD wBitCount;
	//定义调色板大小， 位图中像素字节大小 ，位图文件大小 ， 写入文件字节数 
	DWORD dwPaletteSize=0, dwBmBitsSize=0, dwDIBSize=0, dwWritten=0; 
	//位图属性结构 
	BITMAP Bitmap;  
	//位图文件头结构
	BITMAPFILEHEADER bmfHdr;  
	//位图信息头结构 
	BITMAPINFOHEADER bi;  
	//指向位图信息头结构  
	LPBITMAPINFOHEADER lpbi;  
	//定义文件，分配内存句柄，调色板句柄 
	HANDLE fh, hDib, hPal,hOldPal=NULL; 

	//计算位图文件每个像素所占字节数 
	hDC = CreateDC("DISPLAY", NULL, NULL, NULL);
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES); 
	DeleteDC(hDC); 
	if (iBits <= 1)  wBitCount = 1; 
	else if (iBits <= 4)  wBitCount = 4; 
	else if (iBits <= 8)  wBitCount = 8; 
	else      wBitCount = 24; 

	GetObject(hBitmap, sizeof(Bitmap), (LPSTR)&Bitmap);
	bi.biSize   = sizeof(BITMAPINFOHEADER);
	bi.biWidth   = Bitmap.bmWidth;
	bi.biHeight   = Bitmap.bmHeight;
	bi.biPlanes   = 1;
	bi.biBitCount  = wBitCount;
	bi.biCompression = BI_RGB;
	bi.biSizeImage  = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrImportant = 0;
	bi.biClrUsed  = 0;

	dwBmBitsSize = ((Bitmap.bmWidth * wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;

	//为位图内容分配内存 
	hDib = GlobalAlloc(GHND,dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER)); 
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib); 
	*lpbi = bi;

	// 处理调色板  
	hPal = GetStockObject(DEFAULT_PALETTE); 
	if (hPal) 
	{ 
		hDC = ::GetDC(NULL); 
		hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE); 
		RealizePalette(hDC); 
	}

	// 获取该调色板下新的像素值 
	GetDIBits(hDC, hBitmap, 0, (UINT) Bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER) 
		+dwPaletteSize, (BITMAPINFO *)lpbi, DIB_RGB_COLORS); 

	//恢复调色板  
	if (hOldPal) 
	{ 
		::SelectPalette(hDC, (HPALETTE)hOldPal, TRUE); 
		RealizePalette(hDC); 
		::ReleaseDC(NULL, hDC); 
	}

	//创建位图文件  
	fh = CreateFile(lpFileName, GENERIC_WRITE,0, NULL, CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL); 

	if (fh == INVALID_HANDLE_VALUE)  return FALSE; 

	// 设置位图文件头 
	bmfHdr.bfType = 0x4D42; // "BM" 
	dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;  
	bmfHdr.bfSize = dwDIBSize; 
	bmfHdr.bfReserved1 = 0; 
	bmfHdr.bfReserved2 = 0; 
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize; 
	// 写入位图文件头 
	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL); 
	// 写入位图文件其余内容 
	WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL); 
	//清除  
	GlobalUnlock(hDib); 
	GlobalFree(hDib); 
	CloseHandle(fh);

	return dwWritten;
} 

void ScreenSnapping::setStorageFilePath( const std::string& filePath )
{
	printf("set jpeg storage file:%s" , filePath.c_str() );
	_jpegFilePath = filePath;
}