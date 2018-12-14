/*PROGRAMA PARA TESTE

TAL PROGRAMA PEGA O VÍDEO CUJO LINK ESTA DISPONÍVEL DO YOUTUBE NO INSTRUCATABLE. 
TAL VÍDEO DEVE SER BAIXADO OU TROCADO POR UM OUTRO VÍDEO DE NATUREZA SEMELHANTE
E DEVE ESTAR SALVO NO MESMO DIRETÓRIO DESTE PROGRAMA.

 O CÓDIGO PAUSA O VÍDEO QUANDO UM OBJETO É IDENTIFICADO NA ÁREA INFERIOR DO VÍDEO
(COMANDO DE PARADA POIS HÁ UM OBJETO NO CAMINHO)

OBS: DEVE-SE ALTERAR A STRING (PARÂMETRO DA LINHA 100) PELO NOME DO VÍDEO SALVO

OBS2: PODE SER NECESSÁRIO MUDAR A ÁREA DE INTERESSE QUE ESPECIFICA A ÁREA DE INTERESSE
DE ONDE SE PAUSA O VÍDEO QUANDO UM CONTORNO (OBSTÁCULO) É IDENTIFICADO NELA
PARA ISSO BASTA ALTERAR A POSIÇÃO DA MATRIZ QUE É VERIFICADA NOS FORS DA LINHA 162
*/

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include <fstream>
#include <cstring>

using namespace std;
using namespace cv;

/// Global variables

//my variables
ofstream myfile;
char path[] = "C:/Users/Victor Lamarca/coisas/openCV DragonBoard/txts/";
char bufferItoa[33] = "";
char bufferPath [200] = "";
char strIntIdent[6];
char bufferIdent[6];
int pix = 0;
Point3_<uchar>* p;
bool pare = false;
int qntddPix = 0;

//original variables
Mat src, src_gray;
Mat dst, detected_edges;

int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;
String window_name = "Edge Map";

/**
 * @function CannyThreshold
 * @brief Trackbar callback - Canny thresholds input with a ratio 1:3
 */
void CannyThreshold(int, void*)
{
	/// Reduce noise with a kernel 3x3
	blur( src_gray, detected_edges, Size(3, 3) );

	/// Canny detector
	Canny( detected_edges, detected_edges, lowThreshold, lowThreshold * ratio, kernel_size );

	/// Using Canny's output as a mask, we display our result
	dst = Scalar::all(0);

	src.copyTo( dst, detected_edges);
	imshow( window_name, dst );
}

void ident(int x, char* str)
{

	itoa (x, bufferIdent, 10);
	if(x < 10)
	{
		strcpy(str, "   ");
	}
	else if(x < 100)
	{
		strcpy(str, "  ");
	}
	else if (x < 1000)
	{
		strcpy(str, " ");
	}
	strcat(str, bufferIdent);
}


/** @function main */
int main()
{

	cout << "HEY HOW" << endl;

	/// Load an image
	VideoCapture cap ("Video Andando Cadeira.mp4");

	int ch, iter = 0;
	//cout << "aki 1" << endl;
	do
	{
		//cout << "Iteracao: " << ++iter << endl;
		iter++;
		
		if (!cap.read (src))
		{
			cout << "erro" << endl;
		};

		if( !src.data )
		{
			break;
		}


		/// Create a matrix of the same type and size as src (for dst)
		dst.create( src.size(), src.type() );

		/// Convert the image to grayscale
		cvtColor( src, src_gray, CV_BGR2GRAY );

		/// Create a window
		namedWindow( window_name, WINDOW_NORMAL ); //WINDOW_AUTOSIZE WINDOW_NORMAL
		//namedWindow ("vito idiota");

		/// Create a Trackbar for user to enter threshold
		//createTrackbar( "Min Threshold:", "vito idiota" , &lowThreshold, max_lowThreshold, CannyThreshold );
		lowThreshold = 60;

		/// Show the image
		CannyThreshold(0, 0);

		/// Wait until user exit program by pressing a key
		//waitKey(0);

		//cout << (int)src.at<uchar>(1,1) << " " << endl;

		ch = waitKey(10);
		//cout << ch << " ";
		
		//cout << "Num antes " << qntddPix << endl;
		if(iter%2 == 1)
		{
			//ch = 112;
			/*strcpy(bufferItoa, "");
			itoa (iter, bufferItoa, 10);

			strcpy(bufferPath, "");

			strcat(bufferPath, path);
			strcat(bufferPath, bufferItoa);
			strcat(bufferPath, ".txt");*/

			myfile.open (bufferPath);
			
			//pare = false;
			qntddPix = 0;
			for (int l = (int)(5.0/20.0*dst.rows); l < (int)(15.0/20.0*dst.rows); l++)
			{
				for (int c = dst.cols-30; c < dst.cols; c++)
				{
					p = dst.ptr<Point3_<uchar> >(l, c);
					if(p->x + p->y + p->z
							< 1)
					{
						//pixel é preto
					}
					else
					{
						qntddPix++;
					}
					//ident( pix, strIntIdent);
					//myfile << strIntIdent;*/
				}
				//myfile << endl;
			}
			//myfile.close();
			if (qntddPix > 200) {
				cout << "PARE em: " << iter << endl;
				cout << "Quantidade Pixels: " << qntddPix << endl;
				ch = 112;
			}
		}
		
		CannyThreshold(0, 0);

		if(ch == 112)  //P
		{
			while(1)
			{
				ch = waitKey(100);
				if(ch == 103) break; //G
			}
		}
	}
	while (ch != 27); //ESC
	//cout << "aki fim" << endl;

	return 0;
}
