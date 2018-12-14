/* O PROGRAMA RECEBE UM VÍDEO DA REDE (COMUNICAÇÃO WIFI)
QUE É PROCESSADO E COMUNICA UMA CONDIÇÃO DE PARADA CASO UM OBJETO
SEJA IDENTIFICADO NA ÁREA DE INTERESSE. A CONDIÇÃO DE PARADA É MANDADA
PARA O APLICATIVO DO APARELHO CORRESPONDENTE QUE DEVE ESTAR CONECTADO A REDE
(CONFORME INDICA O LINK DA LINHA 87 QUE PODE SER ALTERADO DE ACORDO)

OBS: PODE-SE ALTERAR A LINHA 66 POR UM OUTRO LINK CORRESPONDENTE AO VÍDEO RECEBIDO DO SPARKFUN
*/

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>


#include <string>
#include <cstring>
#include <cstdlib>
#include <sstream>

#include <fstream>
#include <cstring>

using namespace std;
using namespace cv;

/// Global variables

Point3_<uchar>* p;
int qntddPix = 0;
int parada = 0;

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

void sendData (){
	string url = "http://data.sparkfun.com/input/7JKYyK4Zw1FLpaX20JyN?private_key=mz1MA10BXgsYbNg0yqXz&comands=";
	cout << url << endl;
	parada+= 1;
	ostringstream convert;
	convert << parada;
	string value = convert.str();
	url += value;
	string n = "\""; 
	url = n + url + n;
	string comand1 = "start chrome.exe " + url;
	//string comand2 = "tskill iexplorer.exe";
	cout << url << endl << comand1;
	
	system (strdup(comand1.c_str()));
	
}

/** @function main */
int main()
{
	/// Load an image
	VideoCapture cap ("http://192.168.2.235:8080/videofeed");

	int ch, iter = 0;
	do
	{
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

		/// Create a Trackbar for user to enter threshold
		lowThreshold = 60;

		/// Show the image
		CannyThreshold(0, 0);

		ch = waitKey(10);
		
		if(iter%2 == 1)
		{
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
						//pixel preto
					}
					else //á um pixel branco
					{
						qntddPix++;
					}
				}
				//myfile << endl;
			}
			//myfile.close();
			if (qntddPix > 5) {
				cout << "PARE" << endl;
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

	return 0;
}
