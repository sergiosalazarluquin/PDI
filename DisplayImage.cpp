/*
Titulo: DisplayImage.cpp
Autor: Sergio F. Salazar Luquin
Fecha: Noviembre 10, 2015
Descripcion:
   Programa para mostrar las diferentes etapas del procesamiento de imagenes en OpenCV
   para utilizar la tecnica de filtrado temporal de imagenes.
*/

#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/highgui/highgui_c.h"

#include <stdlib.h>
#include <stdio.h>

using namespace cv;
using namespace std;


/// Variables Globales

// Variables para Binarizado
int const threshold_value = 127;
int const max_BINARY_value = 255;
int const BINARY_TYPE = 0;

// Variables para el calculo de histograma
int histSize = 256;

// Variables para la reduccin de ruido
int MAX_KERNEL_LENGTH = 31;


int main(int argc, char* argv[])
{

    // Abre la muestra de video para darle tratamiento de imagen.
    VideoCapture cap("/home/luko/Material/TUD-Campus.mp4"); 

    // Exit si no puede abrir el archivo
    if ( !cap.isOpened() ) 
    {
         printf("No se pudo abrir el archivo de video\n");
         return -1;
    }

    // Obtenemos los FPS del video.
    double fps = cap.get(CV_CAP_PROP_FPS); 
    printf("FPS: %.2lf\n",fps);

    // Creacion de Ventanas
    namedWindow("Original",CV_WINDOW_AUTOSIZE);  // Video Original
    namedWindow("Contraste",CV_WINDOW_AUTOSIZE); // Video con Mejora de contraste
    namedWindow("Histograma Original", WINDOW_AUTOSIZE );  // Histograma Original
    namedWindow("Histograma Contraste", WINDOW_AUTOSIZE );  // Histograma con Mejora de contraste

    // Posicion de ventanas
    cvMoveWindow("Original", 0, 0);                  // Primer cuadro
    cvMoveWindow("Contraste", 710, 0);               // Segundo cuadro
    cvMoveWindow("Histograma Original", 0, 600);     // Tercer Cuadro
    cvMoveWindow("Histograma Contraste", 710, 600);  // Cuarto cuadro

    //Loop para abrir frame por frame
    while(1)
    {
        Mat frame, frame_gray, frame_bin, histo_dst;
	float range[] = {0, 255};
	const float *ranges[] = { range };

	// Leer el nuevo frame del video.
        bool bSuccess = cap.read(frame); 

        // Rompemos el loop si ocurre un error al abrir el siguiente frame 
         if (!bSuccess) 
        {
	    printf("No se pudo abrir el siguiente frame de la secuencia de video \n");
            break;
        }

	//Conversion a escala de grises para tratamiento
	cvtColor( frame, frame_gray, COLOR_RGB2GRAY ); 
	//Binarizado 
	//threshold( frame_gray, frame_bin, threshold_value, max_BINARY_value,BINARY_TYPE ); 

	// Calculo del histograma Original
    	MatND hist;
    	calcHist( &frame_gray, 1, 0, Mat(), hist, 1, &histSize, ranges, true, false );
	//  Impresion del histograma
 	int hist_w = 512; int hist_h = 400;
    	int bin_w = cvRound( (double) hist_w/histSize );
 
    	Mat histImage( hist_h, hist_w, CV_8UC1, Scalar( 0,0,0) );
    	normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
     
    	for( int i = 1; i < histSize; i++ )
    	{
      		line( histImage, Point( bin_w*(i-1), hist_h - cvRound(hist.at<float>(i-1)) ) ,
                	       Point( bin_w*(i), hist_h - cvRound(hist.at<float>(i)) ),
                	       Scalar( 255, 0, 0), 2, 8, 0  );
    	}     


	// Mejora de contraste (equalizacion) en escala de grises
	equalizeHist(frame_gray,histo_dst);

	// Calculo del histograma con mejora de contraste

    	MatND hist_;
    	calcHist( &histo_dst, 1, 0, Mat(), hist_, 1, &histSize, ranges, true, false );
	//  Impresion del histograma
 	
    	Mat histImage_( hist_h, hist_w, CV_8UC1, Scalar( 0,0,0) );
    	normalize(hist_, hist_, 0, histImage_.rows, NORM_MINMAX, -1, Mat() );
     
    	for( int i = 1; i < histSize; i++ )
    	{
      		line( histImage_, Point( bin_w*(i-1), hist_h - cvRound(hist_.at<float>(i-1)) ) ,
                	       Point( bin_w*(i), hist_h - cvRound(hist_.at<float>(i)) ),
                	       Scalar( 255, 0, 0), 2, 8, 0  );
    	}     

	// Muestra de imagenes
        imshow("Original", frame); 
        imshow("Contraste", histo_dst); 
  	imshow("Histograma Original", histImage);
  	imshow("Histograma Contraste", histImage_);


	/* TIEMPO DE DELAY PARA REPRODUCIR FPS */
        if(waitKey(100) == 27) //ESpera for tecla 'esc', si la presiona antes se sale
       {
	   printf("ESCK fue presionado por el usuario\n");
           break; 
       }
    }

    ////////////////////// Segunda Parte. Reduccion de ruido
     waitKey();

    // Abre la muestra de video para darle tratamiento de imagen.
    VideoCapture cap2("/home/luko/Material/TUD-Campus.mp4"); 

    // Exit si no puede abrir el archivo
    if ( !cap2.isOpened() ) 
    {
         printf("No se pudo abrir el archivo de video\n");
         return -1;
    }

    // Obtenemos los FPS del video.
    double fps2 = cap2.get(CV_CAP_PROP_FPS); 
    printf("FPS: %.2lf\n",fps2);

    // Creacion de Ventanas
    namedWindow("Original_",CV_WINDOW_AUTOSIZE);      // Video Original
    namedWindow("Binarizado",CV_WINDOW_AUTOSIZE);     // Video Binarizado
    namedWindow("Contraste_", WINDOW_AUTOSIZE );      // Video con Mejora deContraste
    namedWindow("Reduccion Ruido", WINDOW_AUTOSIZE ); // Video con Reduccion de Ruido 

    // Posicion de ventanas
    cvMoveWindow("Original_", 0, 0);                  // Primer cuadro
    cvMoveWindow("Binarizado", 710, 0);               // Segundo cuadro
    cvMoveWindow("Contraste_", 0, 600);               // Tercer Cuadro
    cvMoveWindow("Reduccion Ruido", 710, 600);        // Cuarto cuadro

    //Loop para abrir frame por frame
    while(1)
    {
        Mat frame2, frame_gray2, frame_bin2, histo_dst2;
	float range2[] = {0, 255};
	const float *ranges2[] = { range2 };

	// Leer el nuevo frame del video.
        bool bSuccess = cap2.read(frame2); 

        // Rompemos el loop si ocurre un error al abrir el siguiente frame 
         if (!bSuccess) 
        {
	    printf("No se pudo abrir el siguiente frame de la secuencia de video \n");
            break;
        }

	//Conversion a escala de grises para tratamiento
	cvtColor( frame2, frame_gray2, COLOR_RGB2GRAY ); 
	//Binarizado 
	threshold( frame_gray2, frame_bin2, threshold_value, max_BINARY_value,BINARY_TYPE ); 


	// Mejora de contraste (equalizacion) en escala de grises
	equalizeHist(frame_gray2,histo_dst2);
	
	Mat ruido_dst;
    
   	ruido_dst = histo_dst2.clone();
 
     	// Aplicando una Gaussiana para reduccion de ruido
    	for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
        { 
            GaussianBlur( histo_dst2, ruido_dst, Size( i, i ), 0, 0 );
        }
	// Muestra de imagenes
	imshow("Original_", frame2); 
        imshow("Binarizado", frame_bin2); 
  	imshow("Contraste_", histo_dst2);
        imshow( "Reduccion Ruido", ruido_dst );

	/* TIEMPO DE DELAY PARA REPRODUCIR FPS */
        if(waitKey(100) == 27) //ESpera for tecla 'esc', si la presiona antes se sale
       {
	   printf("ESCK fue presionado por el usuario\n");
           break; 
       }
    }

    return 0;

}
