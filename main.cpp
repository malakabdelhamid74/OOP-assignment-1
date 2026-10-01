//====================================================================================
//======================================Phase1========================================
// Rawan Osama : 20250233      |||| filter: 1, 5
// Sama Tarek: 20250295        |||| filter: 2, 6
// Malak Abdelhamid : 20250663 |||| filter: 3, 7
// Salma Ayman : 20250880      |||| filter: 4, 8
// SECTION: S29,S30
//====================================================================================
//====================================================================================

#include "Image_Class.h"
#include <cstdlib>
using namespace std;

Image currentImage;

string inputFileName;
string savingFileName;

// for menu input
int optionNum;
char savingOption;
bool isSaved;
char answer;

// for AdjustBrightness function
int filterOptionNum;
int angle;
double percentage;

// for RotateImage function
int desired_width, desired_height;


//malak
void ShowMenu();
void InvertColors(Image &inputImage);
void AdjustBrightness(Image &inputImage , int chosenOption , double p );
//sama
void BlackWhite(Image &inputImage);
void RotateImage(Image &inputImage);
void SaveImage(Image &inputImage);
//Rawan
void greyscale(Image& inputImage);
void flip(Image& image, int option);

//salma

void paintPixels (int i , int j, Image &image);
void frame (Image &image);
Image resize (Image &image, int w, int h);



int main() {

    cout << "Welcome to our Image Editor\n";
    //cout << "Please enter the image file name: ";
    //cin >> inputFileName;
    while (true) {
        cout << "Please enter the image file name: \n";
        cin >> inputFileName;

        try {
            currentImage.loadNewImage(inputFileName);
            cout << "Image loaded successfully!\n\n";
            break;
        }
        catch (const invalid_argument& e) {
            cout << "Error: " << e.what() << "\nPlease try again.\n\n";

        }
    }
    //currentImage.loadNewImage(inputFileName);
    //isSaved=false;

    while (true) {
        ShowMenu();
        cin >> optionNum;
        switch (optionNum) {
            case 1: //load a new image
                cout << "Do you want to save the current image before loading a new one? (y/n): ";
                cin >> answer;

                if (answer == 'y' || answer == 'Y') {
                    SaveImage(currentImage);
                }
                cout << "Please enter the new image file name: ";
                cin >> inputFileName;
                currentImage.loadNewImage(inputFileName);
                isSaved=false;
                break;
            case 2: //save
                if (isSaved) {
                    cout<<"The image is already saved";
                    break;
                }
                cout << "Do you want to save on the same file? (Y/N) : ";
                cin >> savingOption;

                if (savingOption == 'Y' || savingOption == 'y') {
                    savingFileName = inputFileName;
                    currentImage.saveImage(savingFileName);
                    isSaved=true;

                    cout << "The image is saved successfully\n";
                    system(savingFileName.c_str());
                }
                else if (savingOption == 'N' ||savingOption == 'n') {
                    SaveImage(currentImage);
                }else {
                    cout<<"Invalid answer\n";
                }
                break;

          case 11: // f1
                greyscale(currentImage);
                cout << "Greyscale filter is applied" << endl;
                isSaved=false;
                break;
 
            case 12 :
                BlackWhite(currentImage);
                cout<< "the image is black and white\n";
                isSaved=false;
                break;
            case 13 : //f3
                InvertColors(currentImage);
                cout<< "the image colors is inverted\n";
                isSaved=false;
                break;

            case 14:
                cout<<"OPTIONs\n"
                      " 1.Simple\n"
                      " 2.Fancy\n";
                cin>> filterOptionNum;
                frame(currentImage);
                cout<<"the image frame was applied successfully!\n";
                isSaved=false;
                break;

            case 15: // f5
                cout << "options\n 1.horizontal\n 2.Vertical\n please enter the option number:" << endl;
                cin >> filterOptionNum;

                flip(currentImage, filterOptionNum);
                cout << "Image is flipped" << endl;
                isSaved=false;
                break; 
            case 16 :
                RotateImage(currentImage);
                cout<<"the image is rotated!\n";
                isSaved=false;
                break;

            case 17 ://f7
                cout<< "OPTIONS\n 1.Darken\n 2.Lighten\nPlease enter the option number and percentage :";
                cin >> filterOptionNum>> percentage;
                AdjustBrightness(currentImage, filterOptionNum, percentage);
                cout<<"the image brightness is adjusted successfully!\n";
                isSaved=false;
                break;

            case 18:
                cout<<"Enter the new width: ";
                cin>>desired_width;
                cout<<"Enter the new height: ";
                cin>>desired_height;

                currentImage= resize(currentImage, desired_width, desired_height);
                cout<<"the image was resized successfully!\n";
                isSaved=false;
                break;
            case  0:
                cout << "Do you want to save the current image before exiting? (y/n): ";
                cin >> answer;

                if (answer == 'y' || answer == 'Y') {
                    SaveImage(currentImage);
                }
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

        Image rotated(inputImage.height,inputImage.width);

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

        Image rotated(inputImage.height,inputImage.width);

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


//pixel painting function
void paintPixels (int i , int j, Image &image){
    image(i,j,0)=48;
    image(i,j,1)=92;
    image(i,j,2)=222;
}
// frame function
void frame (Image &image){

    if (filterOptionNum==1) {
        // el frame el kbeer
        for (int i = 0 ; i<image.width; i++){
            for (int j = 0 ; j<0.05*image.height; j++){
                paintPixels(i,j,image);

            }
        }

        for (int i = 0 ; i<0.05*image.width; i++){
            for (int j = 0 ; j<image.height; j++){
                paintPixels(i,j,image);
            }
        }

        for (int i = 0 ; i<image.width; i++){
            for (int j = 0.95*image.height ; j< image.height; j++){
                paintPixels(i,j,image);
            }
        }

        for (int i = image.width-1 ; i>=0.95*image.width; i--){
            for (int j = 0 ; j<image.height; j++){
                paintPixels(i,j,image);
            }
        }


    }
    else if (filterOptionNum==2) {
       // el frame el kbeer
            for (int i = 0 ; i<image.width; i++){
                for (int j = 0 ; j<0.05*image.height; j++){
                    paintPixels(i,j,image);

                }
            }

            for (int i = 0 ; i<0.05*image.width; i++){
                for (int j = 0 ; j<image.height; j++){
                    paintPixels(i,j,image);
                }
            }

            for (int i = 0 ; i<image.width; i++){
                for (int j = 0.95*image.height ; j< image.height; j++) {
                    paintPixels(i, j, image);
                }
            }

            for (int i = image.width - 1; i >= 0.95 * image.width; i--) {
                for (int j = 0; j < image.height; j++) {
                    paintPixels(i, j, image);
                }
            }
            // el frame el kbeer
            for (int i = 0; i < image.width; i++) {
                for (int j = 0; j < 0.05 * image.height; j++) {
                    paintPixels(i, j, image);
                }
            }

            for (int i = 0; i < 0.05 * image.width; i++) {
                for (int j = 0; j < image.height; j++) {
                    paintPixels(i, j, image);
                }
            }

            for (int i = 0; i < image.width; i++) {
                for (int j = 0.95 * image.height; j < image.height; j++) {
                    paintPixels(i, j, image);
                }
            }

            for (int i = image.width - 1; i >= 0.95 * image.width; i--) {
                for (int j = 0; j < image.height; j++) {
                    paintPixels(i, j, image);
                }
            }
            // el frame el so8ayer
            for (int i = 0.08 * image.width; i < image.width - (image.width * 0.08); i++) {
                for (int j = image.height * 0.08; j < image.height * 0.1; j++) {
                    paintPixels(i, j, image);
                }
            }

            for (int i = image.width * 0.08; i < image.width * 0.1; i++) {
                for (int j = 0.08 * image.height; j < image.height - (image.height * 0.08); j++) {
                    paintPixels(i, j, image);
                }
            }

            for (int i = image.width * 0.08; i < image.width - (image.width * 0.08); i++) {
                for (int j = image.height * 0.9; j < image.height * 0.92; j++) {
                    paintPixels(i, j, image);
                }
            }

            for (int i = image.width * 0.92; i >= image.width * 0.9; i--) {
                for (int j = image.height * 0.08; j < image.height - (image.height * 0.08); j++) {
                    paintPixels(i, j, image);
                }
            }


    }
}

//resize function
Image resize (Image &image, int w, int h){
    Image new_image(w,h);

    for (int i = 0; i<new_image.width; i++){
        for (int j = 0; j<new_image.height;j++){
            int pw = i *(static_cast<double>(image.width) / new_image.width);
            int ph = j *(static_cast<double> (image.height) / new_image.height);
            for (int k =0; k<new_image.channels; k++){
                new_image(i, j, k) = image(pw, ph, k);

            }
        }
    }
    return new_image;

}

void SaveImage(Image &inputImage) {

    while (true) {
        cout << "Please enter the file name to save: ";
        cin >> savingFileName;

        try {
            inputImage.saveImage(savingFileName);
            isSaved = true;
            cout << "The image is saved successfully!\n";
            system(savingFileName.c_str());
            break;
        }
        catch (const invalid_argument& e) {
            cout << "Error: " << e.what() << "\nPlease specify a valid extension (.jpg, .bmp, .png).\n\n";
        }
    }
}