/*
PROGRAMA DE TESTE

MOSTRA CONTORNO DO VÍDEO PEGO PELA WEBCAM (OU CÂMERA DEFAULT DO DEVICE USADO)
E CRIA UMA MATRIZ DE 0s e 1s CORRESPONDENTE AO CONTORNO

OBS: ALTERAR VARIAÉVEL PATH (LINHA 31) PONDO O ENDERECO DESEJADO PARA 
SE SALVAR AS MATRIZEZ (.txt)

OBS 2: ESTE PROGRAMA PODE SER COMPILADO ATRAVÉS DA BIBLIOTECA CEKEIKON
CUJO MANUAL DE INSTALAÇÃO DE FÁCIL ENTENDIMENTO ENCONTRA-SE NO SEGUINTE LINK:
http://www.lps.usp.br/hae/software/cekeikon5.html
NA PARTE "MANUAL CEKEIKON 5"
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


int main()
{
	/// Load an image
	VideoCapture cap (0); // 0 = VÍDEO PEGO PELA CÂMERA DEFAULT
						  //PODE-SE ALTERAR POR UM VÍDEO SALVO NO MESMO DIRETÓRIO DO PROGRAMA
						  //COMO "VideoTeste.mp4" por exemplo

	int ch, iter = 0;
	do
	{
		cout << "Para acompanhamento, número da iteração correspondente ao número do frame" << endl << endl;
		cout << "Iteracao: " << ++iter << endl;
		
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
		lowThreshold = 30;

		/// Show the image
		CannyThreshold(0, 0);

		/// Wait until user exit program by pressing a key
		//waitKey(0);

		//cout << (int)src.at<uchar>(1,1) << " " << endl;

		ch = waitKey(10);
		//cout << ch << " ";

		if(iter % 100 == 1)
		{
			ch = 112;
			strcpy(bufferItoa, "");
			itoa (iter, bufferItoa, 10);

			strcpy(bufferPath, "");

			strcat(bufferPath, path);
			strcat(bufferPath, bufferItoa);
			strcat(bufferPath, ".txt");

			myfile.open (bufferPath);
			
			for (int l = 0; l < dst.rows; l++)
			{
				for (int c = 0; c < dst.cols; c++)
				{
					p = dst.ptr<Point3_<uchar> >(l, c);
					if(p->x + p->y + p->z < 1) //SOMA DOS PIXELS RGB
					{
						pix = 0;
					}
					else
					{
						pix = 111;
					}
					ident( pix, strIntIdent);
					myfile << strIntIdent;
				}
				myfile << endl;
			}
			myfile.close();
		}
		
		CannyThreshold(0, 0);

		if(ch == 112)  //P de pause
		{
			while(1)
			{
				ch = waitKey(100);
				if(ch == 103) break; //G de go (play)
			}
		}
	}
	while (ch != 27); //ESC

	return 0;
}
