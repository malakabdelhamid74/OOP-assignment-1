#include "Image_Class.h"
using namespace std;

Image currentImage;

string inputFileName;
string savingFileName;
int optionNum;
int filterOptionNum;
double percentage;

void ShowMenu();
void InvertColors(Image &inputImage);
void AdjustBrightness(Image &inputImage , int chosenOption , double p );


int main() {
    cout << "Welcome to our Image Editor\n";

        cout<<"Please enter the image file name: ";
        cin >> inputFileName;
    currentImage.loadNewImage(inputFileName);

    while (true) {
        ShowMenu();
        cin >> optionNum;
        switch (optionNum) {
            case 1: //load a new image
                cout<<"Please enter the new image file name: ";
                cin >> inputFileName;
                currentImage.loadNewImage(inputFileName);
                break;
            case 2: //save
                cout<<"Please enter the file name to save: ";
                cin>> savingFileName;
                currentImage.saveImage(savingFileName);
                cout<<"the image is saved successfully\n";
                break;;
            case 13 : //f3
                InvertColors(currentImage);

                cout<< "the image colors is inverted\n";
                break;
            case 17 ://f7
                cout<< "OPTIONS\n 1.Darken\n 2.Lighten\nPlease enter the option number and percentage :";
                cin >> filterOptionNum>> percentage;
                AdjustBrightness(currentImage, filterOptionNum, percentage);

                cout<<"the image brightness is adjusted\n";
                break;
            case  0:
                return 0  ;
            default:
                cout<<"Invalid option number";
        }
    }
}


void InvertColors(Image &inputImage) {

    for (int i = 0; i< inputImage.width ;++i) {
        for (int j = 0; j< inputImage.height;++j) {
            for (int k = 0; k < 3; ++k) {
                inputImage(i,j,k)=255-inputImage(i,j,k);

            }
        }
    }

}


void AdjustBrightness(Image &inputImage ,int chosenOption , double p) {


        for (int i = 0; i< inputImage.width ;++i) {
            for (int j = 0; j< inputImage.height;++j) {
                for (int k = 0; k < 3; ++k) {
                    if (chosenOption==1) {
                        int darkenedChannelValue = inputImage(i,j,k) -static_cast<int> (p/100.0 * inputImage(i,j,k)) ;
                        inputImage(i,j,k) = darkenedChannelValue;
                    }
                    else if (chosenOption==2) {
                        int lightenedChannelValue = inputImage(i,j,k) + ( static_cast<int> ( p/100.0 * inputImage(i,j,k) ) ) ;
                        if (lightenedChannelValue >255) {lightenedChannelValue=255;}
                        inputImage(i,j,k) = lightenedChannelValue;
                    }
                }
            }
        }

}

void ShowMenu() {
    cout << "\n1.Load a new image\n"
            "2.Save\n"
            "FILTERS\n"
            " 13.Invert Colors\n"
            " 17.Adjust Brightness\n"
            "0.Exit\n"
            "Please enter the option number: ";
}





