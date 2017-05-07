#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>
#include "bacterium.h"

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
    VideoCapture inVid(0);
    Mat inframe, outframe;

    if (!inVid.isOpened()) {
        cerr << "Oops! We cannot open the file" << endl;
        return -1;
    }
    int rate = 30;
    namedWindow("Input", CV_WINDOW_FREERATIO);
    //resizeWindow("Input", 960, 813);

    inVid >> inframe;
    cvtColor(inframe, outframe, CV_BGR2GRAY);
    tracker track(outframe, 15, 15);

    cout << "initialized" << endl;

    while (true && track.isinited) {
        inVid >> inframe;

        if (inframe.empty())
            break;

        cvtColor(inframe, outframe, CV_BGR2GRAY);

        if (!track.lost) {
            if (!track.update(outframe))
                if (track.lost)
                    putText(inframe, "Bactetrium lost! Please find a new one!", Point(50, 50),
                        FONT_HERSHEY_COMPLEX, 0.8, Scalar(0, 0, 255));
            rectangle(inframe, Rect(track.bact.rangel, track.bact.ranger), Scalar(255, 0, 0));
        }
        else {
            putText(inframe, "Bactetrium lost! Please find a new one!", Point(50, 50),
                FONT_HERSHEY_COMPLEX, 0.8, Scalar(0, 0, 255));
        }

        setMouseCallback("Input", onMouse, &track);
        imshow("Input", inframe);

        cout << "\r Pos" << track.bact.pos;

        if (waitKey(1000 / rate) == 27)
            break;
    }

    inVid.release();

    return 0;
}