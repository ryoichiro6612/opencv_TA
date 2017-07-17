#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>

#define FLAG 1 // 0 or 1

char preset_file[] = "fruits.jpg";

void my_image_processing(IplImage *input, IplImage *processed);

int main( int argc, char *argv[] )
{
	char *input_file;
	IplImage *input, *processed; // image structure

	if(argc == 2){
		input_file = argv[1];
	}else{
		input_file = preset_file;
	}

	// read an image from the specified file
	if((input = cvLoadImage(input_file, 1)) == NULL){
		fprintf(stderr, "cannot open %s\n", input_file);
		exit(0);
	}

	// memory allocation for the 2nd image structure
	processed = cvCreateImage(cvGetSize(input), IPL_DEPTH_8U, 1);

	my_image_processing(input, processed);

	// create windows
	cvNamedWindow("original image", 1 );
	cvNamedWindow("processed image", 1);

	cvShowImage("original image", input);
	cvShowImage("processed image", processed);

	// wait key input
	cvWaitKey(0);

	// destroy windows
	cvDestroyWindow("original image");
	cvDestroyWindow("processed image");

	// save the processed result
	cvSaveImage("processed.jpg",processed);

	// release image structures
	cvReleaseImage(&input);
	cvReleaseImage(&processed);

	return 0;
}

void my_image_processing(IplImage *input, IplImage *processed){

	if(FLAG){
		// use built-in function

		IplImage *temp = cvCreateImage(cvGetSize(input),IPL_DEPTH_8U,3);

		cvCvtColor(input, temp, CV_BGR2YCrCb);
		cvSplit(temp, processed, 0,0,0);

		cvReleaseImage(&temp);

	}else{
		// the same procedure can be implemeted with explict access to IplImage structrures

		unsigned char *ptr1, *ptr2;
		// pixel values should be regarded as unsigned chars

		for(int j = 0; j < input->height; j++){
			for(int i = 0; i < input->width; i++){
				ptr1 = (unsigned char *)(input->imageData     + input->widthStep     * j + input->nChannels     * i);
				ptr2 = (unsigned char *)(processed->imageData + processed->widthStep * j + processed->nChannels * i);

				double y = 0.114*(double)(*ptr1) + 0.587*(double)(*(ptr1+1))
				+ 0.299*(double)(*(ptr1+2));

				if(y > 255) y = 255;
				if(y < 0) y = 0;

				*ptr2 = (unsigned char)y;
			}
		}
	}
	return;
}
