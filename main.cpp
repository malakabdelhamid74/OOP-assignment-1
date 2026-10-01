#include "Image_Class.h"
#include <cstdlib>
using namespace std;

Image currentImage;

string inputFileName;
string savingFileName;
string newName;
int optionNum;
int filterOptionNum;
int angle;
double percentage;
int angle;
int desired_width, desired_height;



void ShowMenu();
void InvertColors(Image &inputImage);
void AdjustBrightness(Image &inputImage , int chosenOption , double p );
void BlackWhite(Image &inputImage);
void RotateImage(Image &inputImage);
void greyscale(Image& inputimage);
void flip(Image& image, int option);

void paintPixels (int i , int j, Image &image);
void frame (Image &image);
Image resize (Image &image, int w, int h);

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

            case 14:
                frame(currentImage);
                cout<<"the image frame was applied successfully!\n";
                system(inputFileName.c_str());
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
                cout<<"the image is saved successfully!\n";
                system(savingFileName.c_str());
                break;

            case 17 ://f7
                cout<< "OPTIONS\n 1.Darken\n 2.Lighten\nPlease enter the option number and percentage :";
                cin >> filterOptionNum>> percentage;
                AdjustBrightness(currentImage, filterOptionNum, percentage);
                system(inputFileName.c_str());
                cout<<"the image brightness is adjusted successfully!\n";
                break;

           

            case 18:
                cout<<"Enter the new width: ";
                cin>>desired_width;
                cout<<"Enter the new height: ";
                cin>>desired_height;
                currentImage= resize(currentImage, desired_width, desired_height);
                cout<<"the image was resized successfully!\n";
                system(inputFileName.c_str());
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
            " 11.Greyscale\n"
            " 12.Black White\n"
            " 13.Invert Colors\n"
            " 14.frame\n"
            " 15.Flip Image\n"
            " 16.Rotate Image\n"
            " 17.Adjust Brightness\n"
            " 18.resize\n"

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
//pixel painting function
void paintPixels (int i , int j, Image &image){
    image(i,j,0)=48;
    image(i,j,1)=92;
    image(i,j,2)=222;
}
// frame function
void frame (Image &image){
    // el frame el kbeer
    for (int i = 0 ; i<image.width; i++){
        for (int j = 0 ; j<10; j++){
            paintPixels(i,j,image);

        }
    }

    for (int i = 0 ; i<10; i++){
        for (int j = 0 ; j<image.height; j++){
            paintPixels(i,j,image);
        }
    }

    for (int i = 0 ; i<image.width; i++){
        for (int j = image.height-10 ; j< image.height; j++){
            paintPixels(i,j,image);
        }
    }

    for (int i = image.width-1 ; i>=image.width-10; i--){
        for (int j = 0 ; j<image.height; j++){
            paintPixels(i,j,image);
        }
    }
    // el frame el so8ayer
    for (int i = 13 ; i<image.width-13; i++){
        for (int j = 13 ; j<18; j++){
            paintPixels(i,j,image);
        }
    }

    for (int i = 13 ; i<18; i++){
        for (int j = 13 ; j<image.height-13; j++){
            paintPixels(i,j,image);

        }
    }

    for (int i = 13 ; i<image.width-13; i++){
        for (int j = image.height-18 ; j< image.height-13; j++){
            paintPixels(i,j,image);
        }
    }

    for (int i = image.width-14 ; i>=image.width-18; i--){
        for (int j = 13 ; j<image.height-13; j++){
            paintPixels(i,j,image);

        }
    }
}

//resize function
Image resize (Image &image, int w, int h){
    Image new_image(w,h);

    for (int i = 0; i<new_image.width; i++){
        for (int j = 0; j<new_image.height;j++){
            int pw = i *((double) image.width / new_image.width);
            int ph = j *((double) image.height / new_image.height);
            for (int k =0; k<new_image.channels; k++){
                new_image(i, j, k) = image(pw, ph, k);

            }
        }
    }
    return new_image;

}