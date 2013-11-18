#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <pthread.h>
#define n(x,y,channel) newImage.at<Vec3b>(x,y)[channel]

using namespace cv;
using namespace std;

static void help()
{
    cout << "This is a simple box filtering program. Just load an image and drag \n"
            "the trackbar to apply different iterations of box filtering. :-) \n"
            "Press Escape to quit the program." << endl;
}

Mat img;
Mat newImage;
int boxfilterIterations=0;

static void onTrackbar(int, void*);


int main( int argc, char** argv )
{
    //read the image
    char* filename = argc >= 2 ? argv[1] : (char*)"fruits.jpg";
    Mat img0 = imread(filename, -1);
    if(img0.empty())
    {
        cout << "Couldn't open the image " << filename << ". Make sure fruits.jpg is in the same folder as the compiled program or "
             "insert an image as a parameter. \n" << endl;
        return 0;
    }

    help();

    namedWindow( "image", 1 );

    img = img0.clone();
    //show the original image
    imshow("image", img);
    createTrackbar("Box filter iterations", "image", &boxfilterIterations, 10, onTrackbar);

    for(;;)
    {
        //wait for a user input
        char c = (char)waitKey();


        //escape
        if( c == 27 )
            break;
        }


    return 0;
}

//calculate lower part of the image
void *calculateLowerPart(void *null) {
    //handle any other pixel that is neither an edge nor a corner
    for(int iterations=0; iterations<boxfilterIterations; iterations++) {
            for(int channel=0; channel<3; channel++) {

         for (int x=(int)floor(img.rows/2); x<img.rows; x++){
            for(int y=1; y<img.cols; y++) {
                        //format: (rows,cols)
                            n(x,y,channel) =
                                    (n(x,y,channel)
                                     +n(x,y+1,channel)
                                     +n(x,y-1,channel)
                                     +n(x+1,y,channel)
                                     +n(x+1,y+1,channel)
                                     +n(x+1,y-1,channel)
                                     +n(x-1,y,channel)
                                     +n(x-1,y+1,channel)
                                     +n(x-1,y-1,channel))/9;
                                                    }
                                            }
            }
    }
    return 0;
}

//calculate upper part of the image
void *calculateUpperPart(void *null) {
    //handle any other pixel that is neither an edge nor a corner
    for(int iterations=0; iterations<boxfilterIterations; iterations++) {
            for(int channel=0; channel<3; channel++) {

                for (int x=1; x<(int)ceil(img.rows/2); x++){
                   for(int y=1; y<img.cols; y++) {
                               //format: (rows,cols)
                                   n(x,y,channel) =
                                           (n(x,y,channel)
                                            +n(x,y+1,channel)
                                            +n(x,y-1,channel)
                                            +n(x+1,y,channel)
                                            +n(x+1,y+1,channel)
                                            +n(x+1,y-1,channel)
                                            +n(x-1,y,channel)
                                            +n(x-1,y+1,channel)
                                            +n(x-1,y-1,channel))/9;
                                                               }
                                                           }
            }
    }
    return 0;
}


static void onTrackbar(int, void*) {

    if(boxfilterIterations==0)
        imshow("image", img);

    else{

    newImage=img.clone();
    //iterations for filtering

   //split the image into two logical parts and calculate each part in a seperate thread
   pthread_t t1=0;
   pthread_t t2=0;
   pthread_create(&t1, NULL, calculateUpperPart, NULL);
   pthread_create(&t2, NULL, calculateLowerPart, NULL);


    for(int iterations=0; iterations<boxfilterIterations; iterations++) {
        //handle corners and edges seperately
            for(int channel=0; channel<3; channel++) {

                //Corners

                //upper left corner
                n(0,0,channel) =
                        (n(0,0,channel)
                         +n(0,1,channel)
                         +n(1,0,channel)
                         +n(1,1,channel))/4;
                //lower left corner
                n(newImage.rows,0,channel) =
                        (n(newImage.rows,0,channel)
                         +n(newImage.rows,1,channel)
                         +n(newImage.rows-1,0,channel)
                         +n(newImage.rows-1,1,channel))/4;
                //lower right corner
                n(newImage.rows,newImage.cols,channel) =
                        (n(newImage.rows,newImage.cols,channel)
                         +n(newImage.rows,newImage.cols-1,channel)
                         +n(newImage.rows-1,newImage.cols,channel)
                         +n(newImage.rows-1,newImage.cols-1,channel))/4;
                //upper right corner
                n(0,newImage.cols,channel) =
                        (n(0,newImage.cols,channel)
                         +n(0,newImage.cols-1,channel)
                         +n(1,newImage.cols,channel)
                         +n(1,newImage.cols-1,channel))/4;


                //Edges

                //lower edge
                for(int k=1; k<newImage.cols; k++) {
                    n(0,k,channel) =
                            (n(0,k,channel)
                             +n(0,k-1,channel)
                             +n(1,k,channel)
                             +n(1,k-1,channel)
                             +n(0,k+1,channel)
                             +n(1,k+1,channel))/6;

                    n(newImage.rows,k,channel) =
                            (n(newImage.rows,k,channel)
                             +n(newImage.rows,k-1,channel)
                             +n(newImage.rows-1,k,channel)
                             +n(newImage.rows-1,k-1,channel)
                             +n(newImage.rows,k+1,channel)
                             +n(newImage.rows-1,k+1,channel))/6;
                    }

                //right edge
                for(int k=1; k<newImage.rows; k++) {
                    n(k,newImage.cols,channel) =
                            (n(k,newImage.cols,channel)
                             +n(k,newImage.cols-1,channel)
                             +n(k-1,newImage.cols,channel)
                             +n(k-1,newImage.cols-1,channel)
                             +n(k+1,newImage.cols,channel)
                             +n(k+1,newImage.cols-1,channel))/6;
                //left edge
                    n(k,0,channel) =
                            (n(k,0,channel)
                             +n(k,1,channel)
                             +n(k-1,0,channel)
                             +n(k-1,1,channel)
                             +n(k+1,0,channel)
                             +n(k+1,1,channel))/6;
                    }
            }
            //wait for the threads to finish
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    }

    imshow("image", newImage);

}
}
