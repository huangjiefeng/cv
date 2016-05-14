#include "StdAfx.h"
#include "rgb2yuv420.h"

static int RGB2YUV_YR[256], RGB2YUV_YG[256], RGB2YUV_YB[256];
static int RGB2YUV_UR[256], RGB2YUV_UG[256], RGB2YUV_UBVR[256];
static int                  RGB2YUV_VG[256], RGB2YUV_VB[256];

//void InitLookupTable();

/************************************************************************
 *
 *  int RGB2YUV420 (int x_dim, int y_dim, 
 *				unsigned char *bmp, 
 *				unsigned char *yuv, int flip)
 *
 *	Purpose :	It takes a 24-bit RGB bitmap and convert it into
 *				YUV (4:2:0) format
 *
 *  Input :		x_dim	the x dimension of the bitmap
 *				y_dim	the y dimension of the bitmap
 *				bmp		pointer to the buffer of the bitmap
 *				yuv		pointer to the YUV structure
 *              flip    0: RGB order in bmp; 1: BGR order in bmp
 *
 *
 *
 ************************************************************************/
/*
int RGB2YUV420 (int x_dim, int y_dim, 
			 unsigned char *bmp, 
			 unsigned char *yuv, int flip)
{
	int i, j;
	unsigned char *r, *g, *b;
	unsigned char *y, *u, *v;
	unsigned char *uu, *vv;
	unsigned char *pu1, *pu2,*pu3,*pu4;
	unsigned char *pv1, *pv2,*pv3,*pv4;

	if(flip==0){
		r=bmp;
		g=bmp+1;
		b=bmp+2;
	}
	else{
		b=bmp;
		g=bmp+1;
		r=bmp+2;
	}

	y=yuv;
	uu=new unsigned char[x_dim*y_dim];
	vv=new unsigned char[x_dim*y_dim];
	u=uu;
	v=vv;
	for (i=0;i<y_dim;i++){
		for (j=0;j<x_dim;j++){
			*y++=( RGB2YUV_YR[*r]  +RGB2YUV_YG[*g]+RGB2YUV_YB[*b]+1048576)>>16;
			*u++=(-RGB2YUV_UR[*r]  -RGB2YUV_UG[*g]+RGB2YUV_UBVR[*b]+8388608)>>16;
			*v++=( RGB2YUV_UBVR[*r]-RGB2YUV_VG[*g]-RGB2YUV_VB[*b]+8388608)>>16;

   			r+=3;
			g+=3;
			b+=3;
		}
	}

	//dimension reduction for U and V components
	u=yuv+x_dim*y_dim;
	v=u+x_dim*y_dim/4;

	pu1=uu;
	pu2=pu1+1;
	pu3=pu1+x_dim;
	pu4=pu3+1;

	pv1=vv;
	pv2=pv1+1;
	pv3=pv1+x_dim;
	pv4=pv3+1;
	for(i=0;i<y_dim;i+=2){
		for(j=0;j<x_dim;j+=2){
			*u++=int(*pu1+*pu2+*pu3+*pu4)>>2;
			*v++=int(*pv1+*pv2+*pv3+*pv4)>>2;
			pu1+=2;
			pu2+=2;
			pu3+=2;
			pu4+=2;
			pv1+=2;
			pv2+=2;
			pv3+=2;
			pv4+=2;
		}
		pu1+=x_dim;
		pu2+=x_dim;
		pu3+=x_dim;
		pu4+=x_dim;
		pv1+=x_dim;
		pv2+=x_dim;
		pv3+=x_dim;
		pv4+=x_dim;
	}

	delete [] uu;
	delete [] vv;
	return 0;
}
*/
int RGB2YUV420 (int x_dim, int y_dim, 
			 unsigned char *bmp, 
			 unsigned char *yuv, int flip)
{
	int i, j;
	unsigned char *r, *g, *b;
	unsigned char *y, *u, *v;
	unsigned char *uu, *vv;
	unsigned char *pu1, *pu2,*pu3,*pu4;
	unsigned char *pv1, *pv2,*pv3,*pv4;

	unsigned int startoffset=(y_dim-1)*x_dim*3;//dsh  bmp扫描方式为从下到上，从左到右 
	if(flip==0){
		r=bmp+startoffset;
		g=bmp+1+startoffset;
		b=bmp+2+startoffset;
	}
	else{
		b=bmp+startoffset;
		g=bmp+1+startoffset;
		r=bmp+2+startoffset;
	}

	y=yuv;
	uu=new unsigned char[x_dim*y_dim];
	vv=new unsigned char[x_dim*y_dim];
	u=uu;
	v=vv;
	for (i=0;i<y_dim;i++){
		for (j=0;j<x_dim;j++){
			*y++=( RGB2YUV_YR[*r]  +RGB2YUV_YG[*g]+RGB2YUV_YB[*b]+1048576)>>16;
			*u++=(-RGB2YUV_UR[*r]  -RGB2YUV_UG[*g]+RGB2YUV_UBVR[*b]+8388608)>>16;
			*v++=( RGB2YUV_UBVR[*r]-RGB2YUV_VG[*g]-RGB2YUV_VB[*b]+8388608)>>16;

   			r+=3;
			g+=3;
			b+=3;
		}
		r=r-x_dim*3*2;
		g=g-x_dim*3*2;
		b=b-x_dim*3*2;
	}

	//dimension reduction for U and V components
	u=yuv+x_dim*y_dim;
	v=u+x_dim*y_dim/4;

	pu1=uu;
	pu2=pu1+1;
	pu3=pu1+x_dim;
	pu4=pu3+1;

	pv1=vv;
	pv2=pv1+1;
	pv3=pv1+x_dim;
	pv4=pv3+1;
	for(i=0;i<y_dim;i+=2){
		for(j=0;j<x_dim;j+=2){
			*u++=int(*pu1+*pu2+*pu3+*pu4)>>2;
			*v++=int(*pv1+*pv2+*pv3+*pv4)>>2;
			pu1+=2;
			pu2+=2;
			pu3+=2;
			pu4+=2;
			pv1+=2;
			pv2+=2;
			pv3+=2;
			pv4+=2;
		}
		pu1+=x_dim;
		pu2+=x_dim;
		pu3+=x_dim;
		pu4+=x_dim;
		pv1+=x_dim;
		pv2+=x_dim;
		pv3+=x_dim;
		pv4+=x_dim;
	}

	delete [] uu;
	delete [] vv;
	return 0;
}

void InitLookupTable()
{
	int i;

	for (i = 0; i < 256; i++) RGB2YUV_YR[i] = (int)((float)65.481 * (i<<8));
	for (i = 0; i < 256; i++) RGB2YUV_YG[i] = (int)((float)128.553 * (i<<8));
	for (i = 0; i < 256; i++) RGB2YUV_YB[i] = (int)((float)24.966 * (i<<8));
	for (i = 0; i < 256; i++) RGB2YUV_UR[i] = (int)((float)37.797 * (i<<8));
	for (i = 0; i < 256; i++) RGB2YUV_UG[i] = (int)((float)74.203 * (i<<8));
	for (i = 0; i < 256; i++) RGB2YUV_VG[i] = (int)((float)93.786 * (i<<8));
	for (i = 0; i < 256; i++) RGB2YUV_VB[i] = (int)((float)18.214 * (i<<8));
	for (i = 0; i < 256; i++) RGB2YUV_UBVR[i] = (int)((float)112 * (i<<8));
}




