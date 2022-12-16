#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "MATRIX_METHODS.h"
#include <math.h>
//비트맵 헤더를 한묶음으로
typedef struct tagBITMAPHEADER {
	BITMAPFILEHEADER bf;
	BITMAPINFOHEADER bi;
	RGBQUAD hRGB[256]; //이 코드에서는 필요없음 (8bit에만 필요)
}BITMAPHEADER;

//비트맵을 읽어와서 화소정보의 포인터를 리턴
BYTE* loadBitmapFile(int bytesPerPixel, BITMAPHEADER* bitmapHeader, int* imgWidth, int*imgHeight, char* filename);

//비트맵 파일 쓰기
void writeBitmapFile(int bytesPerPixel, BITMAPHEADER outputHeader, BYTE* output, int imgSize, char* filename);
double** constructHaarMatrixRecursive(int n);
double** multiplyTwoMatrices(double** A, int m, int n, double** B, int l, int k);
double** haarMatrixNormalize(double** H, int m, int n);
double** addTwoMatrices(double** A, int m, int n, double** B, int l, int k);
BYTE* returnByte(BYTE* IMG, int imgHeight, int imgWidth, int bytesPerPixel, int imgSize, double** Mat);


int main() {
	/*******************************************************************/
	/*************************** Read image  ***************************/
	/*******************************************************************/

	BITMAPHEADER originalHeader;	//비트맵의 헤더부분을 파일에서 읽어 저장할 구조체
	BITMAPHEADER outputHeader;		//변형을 가한 헤더부분을 저장할 구조체
	int imgSize, imgWidth, imgHeight;					//이미지의 크기를 저장할 변수
	int bytesPerPixel = 3;			//number of bytes per pixel (1 byte for R,G,B respectively)
	double** A;
	double** B;
	double** Ahat;
	double** Bhat;
	double** H;
	double** Htransposed;

	double** Hlow;
	double** Hhigh;
	double** HlowT;
	double** HhighT;
	double** ll;
	double** lh;
	double** hl;
	double** hh;
	double** HlAHlt;
	double** HlAHht;
	double** HhAHlt;
	double** HhAHht;
	double** IDHWTA;

	double** Hll;
	double** Hlh;
	double** HllT;
	double** HlhT;
	double** HlAHltl;
	double** HlAHhtl;
	double** HhAHltl;
	double** HhAHhtl;
	double** llll;
	double** lllh;
	double** llhl;
	double** llhh;

	double** newLl;
	double** newIDHWTA;

	int k = 512;

	BYTE* image = loadBitmapFile(bytesPerPixel, &originalHeader, &imgWidth, &imgHeight, "image_lena_24bit.bmp"); 

	if (image == NULL) return 0;

	imgSize = imgWidth * imgHeight; 
	BYTE* output = (BYTE*)malloc(bytesPerPixel * sizeof(BYTE) * imgSize);				
	outputHeader = originalHeader;									
	int n = imgHeight;

	
	A = (double **)malloc(sizeof(double*) * imgHeight);
	for (int i = 0; i < imgHeight; i++) {
		A[i] = (double*)malloc(sizeof(double) * imgWidth);
	}
	Hlow = (double**)malloc(sizeof(double*) * (imgHeight) / 2);
	Hhigh = (double**)malloc(sizeof(double*) * (imgHeight) / 2);

	for (int i = 0; i < imgHeight / 2; i++) {
		Hlow[i] = (double*)malloc(sizeof(double) * imgWidth);
		Hhigh[i] = (double*)malloc(sizeof(double) * imgWidth);
	}

	Bhat = (double**)malloc(sizeof(double*) * imgHeight);
	for (int i = 0; i < imgHeight; i++) {
		Bhat[i] = (double*)malloc(sizeof(double) * imgWidth);
	}

	Hll = (double**)malloc(sizeof(double*) * (imgHeight) / 4);
	Hlh = (double**)malloc(sizeof(double*) * (imgHeight) / 4);

	for (int i = 0; i < imgHeight / 4; i++) {
		Hll[i] = (double*)malloc(sizeof(double) * imgWidth);
		Hlh[i] = (double*)malloc(sizeof(double) * imgWidth);
	}





	/*******************************************************************/
	/*********************** Perform question 1 ************************/
	/*******************************************************************/

	for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgWidth; j++) {
			A[i][j] = (double)image[(i * imgWidth + j) * bytesPerPixel];
		}
	}


	


	H = haarMatrixNormalize(constructHaarMatrixRecursive(n), n, n);
	Htransposed = transposeMatrix(H, n, n);
	B = multiplyTwoMatrices(multiplyTwoMatrices(Htransposed, n, n, A, n, n), n, n, H, n, n);
	k = 512;
	for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgHeight; j++) {
			Bhat[i][j] = 0.0;
		}
	}
	for (int i = 0; i < k; i++) {
		for (int j = 0; j < k; j++) {
			Bhat[i][j] = B[i][j];
		}
	}

	Ahat = multiplyTwoMatrices(multiplyTwoMatrices(H, n, n, Bhat, n, n), n, n, Htransposed, n, n);


	/*******************************************************************/
	/******************* Write reconstructed image  ********************/
	/*******************************************************************/

	output = returnByte(output, imgHeight, imgWidth, bytesPerPixel, imgSize, Ahat);
	writeBitmapFile(bytesPerPixel, outputHeader, output, imgSize, "output_image1_512.bmp");
	

	/*******************************************************************/
	/*************** Perform question 2 - first image ******************/
	/*******************************************************************/
	
	k = 256;

	for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgHeight; j++) {
			Bhat[i][j] = 0.0;
		}
	}
	for (int i = 0; i < k; i++) {
		for (int j = 0; j < k; j++) {
			Bhat[i][j] = B[i][j];
		}
	}

	Ahat = multiplyTwoMatrices(multiplyTwoMatrices(H, n, n, Bhat, n, n), n, n, Htransposed, n, n);


	/*******************************************************************/
	/******************* Write reconstructed image  ********************/
	/*******************************************************************/


	output = returnByte(output, imgHeight, imgWidth, bytesPerPixel, imgSize, Ahat);
	writeBitmapFile(bytesPerPixel, outputHeader, output, imgSize, "output_image1_256.bmp");

	/*******************************************************************/

	k = 128;
	for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgHeight; j++) {
			Bhat[i][j] = 0.0;
		}
	}
	for (int i = 0; i < k; i++) {
		for (int j = 0; j < k; j++) {
			Bhat[i][j] = B[i][j];
		}
	}

	Ahat = multiplyTwoMatrices(multiplyTwoMatrices(H, n, n, Bhat, n, n), n, n, Htransposed, n, n);


	/*******************************************************************/
	/******************* Write reconstructed image  ********************/
	/*******************************************************************/


	output = returnByte(output, imgHeight, imgWidth, bytesPerPixel, imgSize, Ahat);
	writeBitmapFile(bytesPerPixel, outputHeader, output, imgSize, "output_image1_128.bmp");

	/*******************************************************************/

	k = 64;
	for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgHeight; j++) {
			Bhat[i][j] = 0.0;
		}
	}
	for (int i = 0; i < k; i++) {
		for (int j = 0; j < k; j++) {
			Bhat[i][j] = B[i][j];
		}
	}

	Ahat = multiplyTwoMatrices(multiplyTwoMatrices(H, n, n, Bhat, n, n), n, n, Htransposed, n, n);


	/*******************************************************************/
	/******************* Write reconstructed image  ********************/
	/*******************************************************************/


	output = returnByte(output, imgHeight, imgWidth, bytesPerPixel, imgSize, Ahat);
	writeBitmapFile(bytesPerPixel, outputHeader, output, imgSize, "output_image1_64.bmp");




	printf("problem 2 finish\n");

	/*******************************************************************/
	/*********************** Perform question 3 ************************/
	/*******************************************************************/

	for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgWidth; j++) {
			A[i][j] = (double)image[(i * imgWidth + j) * bytesPerPixel];
		}
	}





	H = haarMatrixNormalize(constructHaarMatrixRecursive(n), n, n);
	Htransposed = transposeMatrix(H, n, n);
	B = multiplyTwoMatrices(multiplyTwoMatrices(Htransposed, n, n, A, n, n), n, n, H, n, n);

	for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgHeight; j++) {
			Bhat[i][j] = 0.0;
		}
	}
	for (int i = 0; i < k; i++) {
		for (int j = 0; j < k; j++) {
			Bhat[i][j] = B[i][j];
		}
	}

	Ahat = multiplyTwoMatrices(multiplyTwoMatrices(H, n, n, Bhat, n, n), n, n, Htransposed, n, n);




	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (i < n / 2) {
				Hlow[i][j] = Htransposed[i][j];
			}
			else {
				Hhigh[i - (n / 2)][j] = Htransposed[i][j];
			}
		}
	}
	HlowT = transposeMatrix(Hlow, n/2 , n);
	HhighT = transposeMatrix(Hhigh, n/2, n);

	HlAHlt = multiplyTwoMatrices(multiplyTwoMatrices(Hlow, n / 2, n, A, n, n), n / 2, n, HlowT, n, n / 2);
	HlAHht = multiplyTwoMatrices(multiplyTwoMatrices(Hlow, n / 2, n, A, n, n), n / 2, n, HhighT, n, n / 2);
	HhAHlt = multiplyTwoMatrices(multiplyTwoMatrices(Hhigh, n / 2, n, A, n, n), n / 2, n, HlowT, n, n / 2);
	HhAHht = multiplyTwoMatrices(multiplyTwoMatrices(Hhigh, n / 2, n, A, n, n), n / 2, n, HhighT, n, n / 2);
	
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (i < n / 2) {
				if (j < n / 2) {
					B[i][j] = HlAHlt[i][j];
				}
				else {
					B[i][j] = HlAHht[i][j-(n/2)];
				}
			}
			else {
				if (j < n / 2) {
					B[i][j] = HhAHlt[i-(n/2)][j];
				}
				else {
					B[i][j] = HhAHht[i-(n/2)][j-(n/2)];
				}
			}
		}
	}

	ll = multiplyTwoMatrices(multiplyTwoMatrices(HlowT, n, n / 2, HlAHlt, n / 2, n / 2),n,n/2, Hlow, n/2, n);
	lh = multiplyTwoMatrices(multiplyTwoMatrices(HlowT, n, n / 2, HlAHht, n / 2, n / 2), n, n / 2, Hhigh, n / 2, n);
	hl = multiplyTwoMatrices(multiplyTwoMatrices(HhighT, n, n / 2, HhAHlt, n / 2, n / 2), n, n / 2, Hlow, n / 2, n);
	hh = multiplyTwoMatrices(multiplyTwoMatrices(HhighT, n, n / 2, HhAHht, n / 2, n / 2), n, n / 2, Hhigh, n / 2, n);

	IDHWTA = addTwoMatrices(addTwoMatrices(addTwoMatrices(ll, n, n, lh, n, n), n, n, hl, n, n), n, n, hh, n, n);

	/*******************************************************************/
	/******************* Write reconstructed image  ********************/
	/*******************************************************************/

	output = returnByte(output, imgHeight, imgWidth, bytesPerPixel, imgSize, B);
	writeBitmapFile(bytesPerPixel, outputHeader, output, imgSize, "output_B.bmp");

	output = returnByte(output, imgHeight, imgWidth, bytesPerPixel, imgSize, IDHWTA);
	writeBitmapFile(bytesPerPixel, outputHeader, output, imgSize, "output_IDHWTA.bmp");

	output = returnByte(output, imgHeight, imgWidth, bytesPerPixel, imgSize, ll);
	writeBitmapFile(bytesPerPixel, outputHeader, output, imgSize, "output_ll.bmp");

	output = returnByte(output, imgHeight, imgWidth, bytesPerPixel, imgSize, lh);
	writeBitmapFile(bytesPerPixel, outputHeader, output, imgSize, "output_lh.bmp");


	output = returnByte(output, imgHeight, imgWidth, bytesPerPixel, imgSize, hl);
	writeBitmapFile(bytesPerPixel, outputHeader, output, imgSize, "output_hl.bmp");

	output = returnByte(output, imgHeight, imgWidth, bytesPerPixel, imgSize, hh);
	writeBitmapFile(bytesPerPixel, outputHeader, output, imgSize, "output_hh.bmp");
	printf("problem 3-(b) finish\n");

	/*******************************************************************/

	for (int i = 0; i < n/2; i++) {
		for (int j = 0; j < n; j++) {
			if (i < n / 4) {
				Hll[i][j] = Hlow[i][j];
			}
			else {
				Hlh [i - (n / 4)] [j] = Hlow[i][j];
			}
		}
	}
	HllT = transposeMatrix(Hll, n / 4, n);
	HlhT = transposeMatrix(Hlh, n / 4, n);

	HlAHltl = multiplyTwoMatrices(multiplyTwoMatrices(Hll, n / 4, n, A, n, n), n / 4, n, HllT, n, n / 4);
	HlAHhtl = multiplyTwoMatrices(multiplyTwoMatrices(Hll, n / 4, n, A, n, n), n / 4, n, HlhT, n, n / 4);
	HhAHltl = multiplyTwoMatrices(multiplyTwoMatrices(Hlh, n / 4, n, A, n, n), n / 4, n, HllT, n, n / 4);
	HhAHhtl = multiplyTwoMatrices(multiplyTwoMatrices(Hlh, n / 4, n, A, n, n), n / 4, n, HlhT, n, n / 4);

	llll = multiplyTwoMatrices(multiplyTwoMatrices(HllT, n, n / 4, HlAHltl, n / 4, n / 4), n, n / 4, Hll, n / 4, n);
	lllh = multiplyTwoMatrices(multiplyTwoMatrices(HllT, n, n / 4, HlAHhtl, n / 4, n / 4), n, n / 4, Hlh, n / 4, n);
	llhl = multiplyTwoMatrices(multiplyTwoMatrices(HlhT, n, n / 4, HhAHltl, n / 4, n / 4), n, n / 4, Hll, n / 4, n);
	llhh = multiplyTwoMatrices(multiplyTwoMatrices(HlhT, n, n / 4, HhAHhtl, n / 4, n / 4), n, n / 4, Hlh, n / 4, n);

	newLl = addTwoMatrices(addTwoMatrices(addTwoMatrices(llll, n, n, lllh, n, n), n, n, llhl, n, n), n, n, llhh, n, n);

	newIDHWTA = addTwoMatrices(addTwoMatrices(addTwoMatrices(newLl, n, n, lh, n, n), n, n, hl, n, n), n, n, hh, n, n);

	/*******************************************************************/
	/******************* Write reconstructed image  ********************/
	/*******************************************************************/

	output = returnByte(output, imgHeight, imgWidth, bytesPerPixel, imgSize, newIDHWTA);
	writeBitmapFile(bytesPerPixel, outputHeader, output, imgSize, "output_newIDHWTA.bmp");


	output = returnByte(output, imgHeight, imgWidth, bytesPerPixel, imgSize, llll);
	writeBitmapFile(bytesPerPixel, outputHeader, output, imgSize, "output_llll.bmp");


	output = returnByte(output, imgHeight, imgWidth, bytesPerPixel, imgSize, lllh);
	writeBitmapFile(bytesPerPixel, outputHeader, output, imgSize, "output_lllh.bmp");

	output = returnByte(output, imgHeight, imgWidth, bytesPerPixel, imgSize, llhl);
	writeBitmapFile(bytesPerPixel, outputHeader, output, imgSize, "output_llhl.bmp");

	
	output = returnByte(output, imgHeight, imgWidth, bytesPerPixel, imgSize, llhh);
	writeBitmapFile(bytesPerPixel, outputHeader, output, imgSize, "output_llhh.bmp");	

	printf("problem 3-(d) finish\n");
	/*******************************************************************/
	/************************** memery free ****************************/
	/*******************************************************************/
	free(image);
	free(output);
	for (int i = 0; i < imgHeight; i++) {

		free(A[i]);
		free(B[i]);
		free(H[i]);
		free(Htransposed[i]);
	}
	free(A);
	free(B);
	free(H);
	free(Htransposed);
	

	return 0;
}

BYTE* loadBitmapFile(int bytesPerPixel, BITMAPHEADER* bitmapHeader, int* imgWidth, int* imgHeight, char* filename)
{
	FILE* fp = fopen(filename, "rb");	//파일을 이진읽기모드로 열기
	if (fp == NULL)
	{
		printf("파일로딩에 실패했습니다.\n");	//fopen에 실패하면 NULL값을 리턴
		return NULL;
	}
	else
	{
		fread(&bitmapHeader->bf, sizeof(BITMAPFILEHEADER), 1, fp);	//비트맵파일헤더 읽기
		fread(&bitmapHeader->bi, sizeof(BITMAPINFOHEADER), 1, fp);	//비트맵인포헤더 읽기
		//fread(&bitmapHeader->hRGB, sizeof(RGBQUAD), 256, fp);	//색상팔렛트 읽기 (24bitmap 에서는 존재하지 않음)

		*imgWidth = bitmapHeader->bi.biWidth;
		*imgHeight = bitmapHeader->bi.biHeight;
		int imgSizeTemp = (*imgWidth) * (*imgHeight);	// 이미지 사이즈를 상위 변수에 할당

		printf("Size of image: Width %d   Height %d\n", bitmapHeader->bi.biWidth, bitmapHeader->bi.biHeight);
		BYTE* image = (BYTE*)malloc(bytesPerPixel * sizeof(BYTE) * imgSizeTemp);	//이미지크기만큼 메모리할당


		fread(image, bytesPerPixel*sizeof(BYTE), imgSizeTemp, fp);//이미지 크기만큼 파일에서 읽어오기

		fclose(fp);
		return image;
	}
}



void writeBitmapFile(int bytesPerPixel, BITMAPHEADER outputHeader, BYTE* output, int imgSize, char* filename)
{
	FILE* fp = fopen(filename, "wb");

	fwrite(&outputHeader.bf, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&outputHeader.bi, sizeof(BITMAPINFOHEADER), 1, fp);
	//fwrite(&outputHeader.hRGB, sizeof(RGBQUAD), 256, fp); //not needed for 24bitmap
	fwrite(output, bytesPerPixel*sizeof(BYTE), imgSize, fp);
	fclose(fp);
}
double** multiplyTwoMatrices(double** A, int m, int n, double** B, int l, int k) {
	if (n != l) {
		return NULL;
	}
	double** S = allocateMemory(m, k);
	double temp;

	for (int i = 0; i < m; i++) {
		for (int j = 0; j < k; j++) {
			temp = 0.0;
			for (int p = 0; p < n; p++) {
				temp += A[i][p] * B[p][j];
			}
			S[i][j] = temp;
		}
	}
	return S;
}

double** haarMatrixNormalize(double** H, int m, int n){
	double** Hnormalize;
	double normalizingH = 0.0;
	Hnormalize = (double**)malloc(sizeof(double*) * m);
	for (int i = 0; i < m; i++) {
		Hnormalize[i] = (double*)malloc(sizeof(double) * n);
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			normalizingH += (H[j][i] * H[j][i]);
		}

		normalizingH = sqrt(normalizingH);
		
		for (int j = 0; j < m; j++) {
			Hnormalize[j][i] = H[j][i]/ normalizingH;
		}
		normalizingH = 0.0;
	}
	return Hnormalize;
}
double** addTwoMatrices(double** A, int m, int n, double** B, int l, int k) {
	if (m != l || n != k) {
		return NULL;

	}
	double** S = allocateMemory(m, n);
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			S[i][j] = A[i][j] + B[i][j];
		}
	}
	return S;
}

BYTE* returnByte(BYTE* IMG, int imgHeight, int imgWidth, int bytesPerPixel, int imgSize, double** Mat)
{
	BYTE* output = (BYTE*)malloc(bytesPerPixel * sizeof(BYTE) * imgSize);
	for (int i = 0; i < imgHeight; i++)
		for (int j = 0; j < imgWidth; j++) {
			output[(i * imgWidth + j) * bytesPerPixel] = (BYTE)Mat[i][j];
			output[(i * imgWidth + j) * bytesPerPixel + 1] = (BYTE)Mat[i][j];
			output[(i * imgWidth + j) * bytesPerPixel + 2] = (BYTE)Mat[i][j];
		}

	return output;
}
