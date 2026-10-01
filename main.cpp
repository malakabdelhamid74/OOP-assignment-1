#include "Image_Class.h"
using namespace std;

Image currentImage;

string inputFileName;
string savingFileName;
int optionNum;
int filterOptionNum;
int angle;
double percentage;

void ShowMenu();
void InvertColors(Image &inputImage);
void AdjustBrightness(Image &inputImage , int chosenOption , double p );
void BlackWhite(Image &inputImage);
void RotateImage(Image &inputImage);
void greyscale(Image& inputimage);
void flip(Image& image, int option);


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
                break;

              case 11: // f1
                greyscale(currentImage);
                cout << "Greyscale filter is applied" << endl;
                break;
 
            case 12 :
                BlackWhite(currentImage);
                break;
            case 13 : //f3
                InvertColors(currentImage);
                cout<< "the image colors is inverted\n";
                break;

               case 15: // f5
                cout << "options\n 1.horizontal\n 2.Vertical\n please enter the option number:" << endl;
                cin >> filterOptionNum;
                flip(currentImage, filterOptionNum);
                cout << "Image is flipped" << endl;
                break; 
            case 16 :
                RotateImage(currentImage);
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
            " 11.Greyscale\n"
            " 12.Black White\n"
            " 13.Invert Colors\n"
            " 15.Flip Image\n"
            " 16.Rotate Image\n"
            " 17.Adjust Brightness\n"
            "0.Exit\n"
            "Please enter the option number: ";
}
void BlackWhite(Image &inputImage) {

    for (int i = 0; i < inputImage.width; ++i) {
        for (int j = 0; j < inputImage.height; ++j) {

            int R = inputImage(i, j, 0);
            int G = inputImage(i, j, 1);
            int B = inputImage(i, j, 2);

            int average = (R + G + B) / 3;

            if (average >= 127) {
                average = 255;
            }
            else {
                average = 0;
            }

            for (int k = 0; k < inputImage.channels; ++k) {
                inputImage(i, j, k) = average;
            }
        }
    }
}

void RotateImage(Image &inputImage) {

    cout << "Enter rotation angle (90, 180, or 270): ";
    cin >> angle;

    if (angle == 90) {

        Image rotated(inputImage.height, inputImage.width);

        for (int i = 0; i < inputImage.width; ++i) {
            for (int j = 0; j < inputImage.height; ++j) {
                for (int k = 0; k < inputImage.channels; ++k) {

                    rotated(inputImage.height - 1 - j, i, k)
                        = inputImage(i, j, k);
                }
            }
        }

        inputImage = rotated;
    }

    else if (angle == 180) {

        Image rotated(inputImage.width, inputImage.height);

        for (int i = 0; i < inputImage.width; ++i) {
            for (int j = 0; j < inputImage.height; ++j) {
                for (int k = 0; k < inputImage.channels; ++k) {

                    rotated(inputImage.width - 1 - i,
                            inputImage.height - 1 - j, k)
                        = inputImage(i, j, k);
                }
            }
        }

        inputImage = rotated;
    }

    else if (angle == 270) {

        Image rotated(inputImage.height, inputImage.width);

        for (int i = 0; i < inputImage.width; ++i) {
            for (int j = 0; j < inputImage.height; ++j) {
                for (int k = 0; k < inputImage.channels; ++k) {

                    rotated(j, inputImage.width - 1 - i, k)
                        = inputImage(i, j, k);
                }
            }
        }

        inputImage = rotated;
    }

    else {
        cout << "Invalid rotation angle\n";
    }
}
void greyscale(Image& inputimage){


for (int i = 0; i < inputimage.width; i++){
    for (int j = 0; j < inputimage.height; j++){

        unsigned int average = 0;

        for (int k = 0; k<3; k++){
            average = average + inputimage(i, j, k);
        }

        average = average / 3;
        inputimage(i, j, 0) = average;
        inputimage(i, j, 1) = average;
        inputimage(i, j, 2) = average;
    }
}
}
void flip(Image& image, int option){

if (option == 1) {  // horizontal
    for (int i = 0; i < image.width / 2; i++) {
        for (int j = 0; j < image.height; j++) {
            for (int k = 0; k < 3; k++) {

                int temp = image(i, j, k);
                image(i, j, k) = image((image.width - 1) - i, j, k);
                image((image.width - 1) - i, j, k) = temp;

            }
        }
    }
}

else if (option == 2) {  // vertical
    for (int i = 0; i < image.width; i++) {
        for (int j = 0; j < image.height / 2; j++) {
            for (int k = 0; k < 3; k++) {

                int temp = image(i, j, k);
                image(i, j, k) = image(i, (image.height - 1) - j, k);
                image(i, (image.height - 1) - j, k) = temp;

            }
        }
    }
}

else {
    cout << "Invalid option" << endl;
}
}