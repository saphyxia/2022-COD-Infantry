#ifndef _MATRIX_H_
#define _MATRIX_H_

#include<math.h>
#define DF float
//#include "Include.h"

//�������ܣ�����ӷ�
void MatrixAdd( float* fMatrixA,float* fMatrixB,float* Result,
		unsigned int m,unsigned int n );

//�������ܣ��������
void MatrixSub( float* fMatrixA,float* fMatrixB,float* Result,
		unsigned int m,unsigned int n );

//�������ܣ�����˷�
void MatrixMultiply( float* fMatrixA,unsigned int uRowA,unsigned int uColA,
	
float* fMatrixB,unsigned int uRowB,unsigned int uColB,float* MatrixResult );


//�������ܣ�����ת��
void MatrixTranspose(float* fMatrixA,unsigned int m,unsigned n,float* fMatrixB);
void MatrixProduct(float* A, int m, int n, float* B, int k, float* C)							 ;
//�������ܣ���λ��������?//����������
void MatrixE(float* fMatrixA,unsigned int n);

//�������ܣ�2�׾�������ʽ��ֵ
double MatrixDet2(float* fMatrixA);

//�������ܣ�2�׾�������
int MatrixInverse2(float* fMatrixA,float* fMatrixB);

//�������ܣ���������
int MatrixInverse(float* fMatrixA,int n,float ep);
void UD(float * A,int  n,float * U,float * D);

//�������ܣ��������
DF Norm(float*fMatrixA,int iRow,int iCol);
#endif

