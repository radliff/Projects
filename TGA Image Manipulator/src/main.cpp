#include <iostream>
#include <ostream>
#include <fstream>
#include <vector>
#include "Image.h"
#include <bits/stdc++.h>
#include <stdexcept>
#include <cstring>
using namespace std;

void Multiply(Image image1, Image image2){
    float roundFactor = 0.5f;
    // create float vectors to store each channel
    vector<float> blueC(image1.pixels.size());
    vector<float> greenC(image1.pixels.size());
    vector<float> redC(image1.pixels.size());


    for (int i = 0; i < image1.pixels.size(); i++) {
        // normalize pixel values
        blueC[i] = (((image1.pixels[i].blue / 255.0f) * (image2.pixels[i].blue / 255.0f)) * 255.0f) + 0.5f;
        greenC[i] = (((image1.pixels[i].green / 255.0f) * (image2.pixels[i].green / 255.0f)) * 255.0f) + 0.5f;
        redC[i] = (((image1.pixels[i].red / 255.0f) * (image2.pixels[i].red / 255.0f)) * 255.0f) + 0.5f;
    }

    Image resultImage = image1;
    for (int i = 0; i < resultImage.pixels.size(); i++){
        // convert from float to char by assigning to pixel vector (unsigned char vector)
        resultImage.pixels[i].blue = blueC[i];
        resultImage.pixels[i].green = greenC[i];
        resultImage.pixels[i].red = redC[i];
    }
    resultImage.write("output/part1.tga");
}

Image MultiplyO(Image image1, Image image2){
    float roundFactor = 0.5f;
    // create float vectors to store each channel
    vector<float> blueC(image1.pixels.size());
    vector<float> greenC(image1.pixels.size());
    vector<float> redC(image1.pixels.size());


    for (int i = 0; i < image1.pixels.size(); i++) {
        // normalize pixel values
        blueC[i] = (((image1.pixels[i].blue / 255.0f) * (image2.pixels[i].blue / 255.0f)) * 255.0f) + 0.5f;
        greenC[i] = (((image1.pixels[i].green / 255.0f) * (image2.pixels[i].green / 255.0f)) * 255.0f) + 0.5f;
        redC[i] = (((image1.pixels[i].red / 255.0f) * (image2.pixels[i].red / 255.0f)) * 255.0f) + 0.5f;
    }

    Image resultImage = image1;
    for (int i = 0; i < resultImage.pixels.size(); i++){
        // convert from float to char by assigning to pixel vector (unsigned char vector)
        resultImage.pixels[i].blue = blueC[i];
        resultImage.pixels[i].green = greenC[i];
        resultImage.pixels[i].red = redC[i];
    }

    return resultImage;
}

Image Subtract(Image image1, Image image2){
    Image resultImage;
    resultImage = image1;
    for (int i = 0; i < image1.pixels.size(); i++){
        // check for possible negative values & clamp
        if (image1.pixels[i].blue < image2.pixels[i].blue){
            resultImage.pixels[i].blue = 0;
        } else {
            resultImage.pixels[i].blue = image1.pixels[i].blue - image2.pixels[i].blue;
        }
        if (image1.pixels[i].green < image2.pixels[i].green){
            resultImage.pixels[i].green = 0;
        } else {
            resultImage.pixels[i].green = image1.pixels[i].green- image2.pixels[i].green;
        }
        if (image1.pixels[i].red < image2.pixels[i].red){
            resultImage.pixels[i].red = 0;
        } else {
            resultImage.pixels[i].red = image1.pixels[i].red - image2.pixels[i].red;
        }
    }
    return resultImage;
}

void Subtract(Image &image1, Image &image2, const string& filePath){
    Image resultImage;
    resultImage = image1;
    for (int i = 0; i < image1.pixels.size(); i++){
        // check for possible negative values & clamp
        if (image1.pixels[i].blue < image2.pixels[i].blue){
            resultImage.pixels[i].blue = 0;
        } else {
            resultImage.pixels[i].blue = image1.pixels[i].blue - image2.pixels[i].blue;
        }
        if (image1.pixels[i].green < image2.pixels[i].green){
            resultImage.pixels[i].green = 0;
        } else {
            resultImage.pixels[i].green = image1.pixels[i].green- image2.pixels[i].green;
        }
        if (image1.pixels[i].red < image2.pixels[i].red){
            resultImage.pixels[i].red = 0;
        } else {
            resultImage.pixels[i].red = image1.pixels[i].red - image2.pixels[i].red;
        }
    }
    resultImage.write(filePath);
}

Image Screen(Image image1, Image image2){
    // create float vectors to store each channel
    vector<float> blueC(image1.pixels.size());
    vector<float> greenC(image1.pixels.size());
    vector<float> redC(image1.pixels.size());

    Image resultImage;
    resultImage = image1;
    for (int i = 0; i < image1.pixels.size(); i++) {
        // normalize pixel values and perform screen blending
        blueC[i] = 1 - (1 - (image1.pixels[i].blue / 255.0f)) * (1 - (image2.pixels[i].blue / 255.0f));
        greenC[i] = 1 - (1 - (image1.pixels[i].green / 255.0f)) * (1 - (image2.pixels[i].green / 255.0f));
        redC[i] = 1 - (1 - (image1.pixels[i].red / 255.0f)) * (1 - (image2.pixels[i].red / 255.0f));
    }

    for (int i = 0; i < image1.pixels.size(); i++){
        blueC[i] = (blueC[i] * 255.0f) + 0.5f;
        greenC[i] = (greenC[i] * 255.0f) + 0.5f;
        redC[i] = (redC[i] * 255.0f) + 0.5f;

        // writing screen operated pixels onto resultImage
        resultImage.pixels[i].blue = blueC[i];
        resultImage.pixels[i].green = greenC[i];
        resultImage.pixels[i].red = redC[i];
    }
    return resultImage;
}

Image Overlay(Image image1, Image image2){
    float roundFactor = 0.5f;

    // create float vectors to store each channel
    vector<float> blueC(image2.pixels.size());
    vector<float> greenC(image2.pixels.size());
    vector<float> redC(image2.pixels.size());

    // create float vectors to store each channel from image1
    vector<float> blueC1(image2.pixels.size());
    vector<float> greenC1(image2.pixels.size());
    vector<float> redC1(image2.pixels.size());

    Image resultImage;
    resultImage = image1;
    // normalizing pixel values
    for (int i = 0; i < image2.pixels.size(); i++){
        blueC[i] = image2.pixels[i].blue / 255.0f;
        greenC[i] = image2.pixels[i].green / 255.0f;
        redC[i] = image2.pixels[i].red / 255.0f;

        blueC1[i] = image1.pixels[i].blue / 255.0f;
        greenC1[i] = image1.pixels[i].green / 255.0f;
        redC1[i] = image1.pixels[i].red / 255.0f;

        // first condition: P2 <= 0.5; 2 * NP1 * NP2
        // second condition: P2 > 0.5; 1 - [2 * (1-NP1) * (1-NP2)]
        // un-normalize pixel values & add rounding factor
        if (blueC[i] <= 0.5){
            blueC[i] = (2 * (blueC[i] * blueC1[i]) * 255.0f) + 0.5f;
        } else {
            blueC[i] = ((1 - (2 * (1 - blueC[i]) * (1 - blueC1[i]))) * 255.0f) + 0.5f;
        }

        if (greenC[i] <= 0.5){
            greenC[i] = ((2 * (greenC[i] * greenC1[i])) * 255.0f) + 0.5f;
        } else {
            greenC[i] = ((1 - (2 * (1 - greenC[i]) * (1 - greenC1[i]))) * 255.0f) + 0.5f;
        }

        if (redC[i] <= 0.5){
            redC[i] = (2 * (redC[i] * redC1[i])) * (255.0f) + 0.5f;
        } else {
            redC[i] = ((1 - (2 * (1 - redC[i]) * (1 - redC1[i]))) * 255.0f) + 0.5f;
        }

        resultImage.pixels[i].blue = blueC[i];
        resultImage.pixels[i].green = greenC[i];
        resultImage.pixels[i].red = redC[i];
    }
    return resultImage;
}

void Addition(Image image1, Image image2){

    // create float vectors to store each channel
    vector<float> blueC(image1.pixels.size());
    vector<float> greenC(image1.pixels.size());
    vector<float> redC(image1.pixels.size());

    Image resultImage;
    resultImage = image1;
    for (int i = 0; i < image1.pixels.size(); i++){
        blueC[i] = image1.pixels[i].blue + image2.pixels[i].blue;
        greenC[i] = image1.pixels[i].green + image2.pixels[i].green;
        redC[i] = image1.pixels[i].red + image2.pixels[i].red;

        if (blueC[i] > 255){
            blueC[i] = 255;
        }
        if (greenC[i] > 255){
            greenC[i] = 255;
        }
        if (redC[i] > 255){
            redC[i] = 255;
        }

        resultImage.pixels[i].blue = blueC[i];
        resultImage.pixels[i].green = greenC[i];
        resultImage.pixels[i].red = redC[i];
    }

    resultImage.write("output/part6.tga");
}
void DiffChecker(Image image1, Image image2){
    vector<char> diff;
    for (int i = 0; i < image1.pixels.size(); i++){
        if (image1.pixels[i].blue != image2.pixels[i].blue){
            diff.push_back(image1.pixels[i].blue);
            diff.push_back(image2.pixels[i].blue);
        }
        if (image1.pixels[i].green != image2.pixels[i].green){
            diff.push_back(image1.pixels[i].green);
            diff.push_back(image2.pixels[i].green);
        }
        if (image1.pixels[i].red != image2.pixels[i].red){
            diff.push_back(image1.pixels[i].red);
            diff.push_back(image2.pixels[i].red);
        }
    }
    for (int i = 0; i < diff.size() - 1; i++){
        cout << int(diff[i]) << " " << int(diff[i + 1]) << endl;
    }
}

Image reverse(Image image1){
    reverse(image1.pixels.begin(), image1.pixels.end());
    return image1;
}

Image scaleRed(Image image, int scaleNumber){
    float redPixels;
    for (int i = 0; i < image.pixels.size(); i++){
        redPixels = scaleNumber * (image.pixels[i].red / 255.0f);
        if (redPixels > 1){
            redPixels = 1;
        }
        redPixels = (redPixels * 255.0f) + 0.5f;
        image.pixels[i].red = redPixels;
    }
    return image;
}

Image scaleBlue(Image image, int scaleNumber){
    float bluePixels;
    for (int i = 0; i < image.pixels.size(); i++){
        bluePixels = scaleNumber * (image.pixels[i].blue / 255.0f);
        if (bluePixels > 1){
            bluePixels = 1;
        }
        bluePixels = (bluePixels * 255.0f) + 0.5f;
        image.pixels[i].blue = bluePixels;
    }
    return image;
}

Image scaleGreen(Image image, int scaleNumber){
    float greenPixels;
    for (int i = 0; i < image.pixels.size(); i++){
        greenPixels = scaleNumber * (image.pixels[i].green / 255.0f);
        if (greenPixels > 1){
            greenPixels = 1;
        }
        greenPixels = (greenPixels * 255.0f) + 0.5f;
        image.pixels[i].green = greenPixels;
    }
    return image;
}

Image addRed(Image image, int number){
    float redPixel;
    Image resultImage;

    for (int i = 0; i < image.pixels.size(); i++){
       redPixel = number + image.pixels[i].red;
       if (redPixel > 255.0f){
           redPixel = 255;
       } else if (redPixel < 0){
           redPixel = 0;
       }
       image.pixels[i].red = redPixel;
    }
    return image;
}

Image addBlue(Image image, int number){
    float bluePixel;

    for (int i = 0; i < image.pixels.size(); i++){
        bluePixel = number + image.pixels[i].blue;
        if (bluePixel > 255.0f){
            bluePixel = 255;
        } else if (bluePixel < 0){
            bluePixel = 0;
        }
        image.pixels[i].blue = bluePixel;
    }
    return image;
}

Image addGreen(Image image, int number){
    float greenPixel;

    for (int i = 0; i < image.pixels.size(); i++){
        greenPixel = number + image.pixels[i].green;
        if (greenPixel > 255.0f){
            greenPixel = 255;
        } else if (greenPixel < 0){
            greenPixel = 0;
        }
        image.pixels[i].green = greenPixel;
    }
    return image;
}

Image onlyRed(Image image){
    for(int i = 0; i < image.pixels.size(); i++){
        image.pixels[i].blue = image.pixels[i].red;
        image.pixels[i].green = image.pixels[i].red;
    }
    return image;
}

Image onlyGreen(Image image){
    for(int i = 0; i < image.pixels.size(); i++){
        image.pixels[i].red = image.pixels[i].green;
        image.pixels[i].blue = image.pixels[i].green;
    }
    return image;
}

Image onlyBlue(Image image){
    for(int i = 0; i < image.pixels.size(); i++){
        image.pixels[i].red = image.pixels[i].blue;
        image.pixels[i].green = image.pixels[i].blue;
    }
    return image;
}

Image combine(Image trackingImage, Image image1, Image image2){
    Image result = trackingImage;

    for (int i = 0; i < image1.pixels.size(); i++){
        result.pixels[i].green = image1.pixels[i].green;
        result.pixels[i].blue = image2.pixels[i].blue;
    }
    return result;
}

bool isValidFile(const char* arg){
    // Case 1: Does the file end in .tga?
    string argument = arg;
    return (argument.length() >= 4 && argument.substr(argument.length() - 4) == ".tga");
}

bool fileExists(const string& filename){
    // Does the file exist?
    ifstream file(filename);
    return file.good();
}

bool isValidMethod(const char* arg){
    vector<const char*> methodNames = {"multiply", "subtract", "onlyred", "onlyblue", "onlygreen", "scalered",
                                 "scaleblue", "scalegreen", "overlay", "combine", "screen", "addred",
                                 "addblue", "addgreen", "flip"};
    for (int i = 0; i < methodNames.size(); i++){
        if(strcmp(arg, methodNames[i]) == 0){
            return true;
        }
    }
    return false;
}


// Error Cases:
// check whether file ends in .tga
// check whether the file exists
//

//(1) Create a main() with arguments argc and argv
int main(int argc, char* argv[]){
    Image trackingImage;
    //(3) Check if user entered "help" or no arguments were provided
    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        cout << "Project 2: Image Processing, Fall 2023\n" << "\nUsage:\n\t./project2.out [output] [firstImage] [method] [...]";
        return 0;
    }
    if(!isValidFile(argv[1])){
        cout << "Invalid file name.";
        return 1;
    }
    int argIndex = 1;

    //(4) Create a while-loop that runs while argIndex is less than argc
    while (argIndex < argc) {
        if (argIndex >= argc){
            cout << "Missing argument.";
        }
        //(5) Print out each arg in argv
        cout << "argv[" << argIndex << "]: " << argv[argIndex] << endl;

        if (!isValidFile(argv[2])){
            cout << "Invalid file name.";
            return 1;
        }

        if(!fileExists((argv[2]))){
            cout << "File does not exist.";
            return 1;
        }

        if(!isValidMethod(argv[3])){
            cout << "Invalid method name.";
            return 1;
        }
        //(7) Check if the user entered "multiply"
        if (argIndex == 3 && strcmp(argv[argIndex], "multiply") == 0) {
            cout << "argv[" << argIndex << "]: " << argv[argIndex] << endl;
            // reading in the 1st image
            Image image1;

            if (argIndex + 1 >= argc){
                cout << "Missing argument.";
                return 1;
            }

            if (isValidFile(argv[argIndex - 1])){
                image1.read(argv[argIndex - 1]);
            } else {
                cout << "Invalid file name.";
                return 1;
            }

            if(!fileExists(argv[argIndex - 1])){
                cout << "Invalid argument, file does not exist.";
                return 1;
            }
            //(8) Increase the argIndex is user entered "multiply" inorder to reach the following data
            argIndex++;
            cout << "argv[" << argIndex << "]: " << argv[argIndex] << endl;
            // reading in the 2nd image
            Image image2;
            // writing the tracking image to a file
            if (isValidFile(argv[argIndex])){
                image2.read(argv[argIndex]);
            } else {
                cout << "Invalid argument, invalid file name.";
                return 1;
            }

            if(!fileExists(argv[argIndex])){
                cout << "Invalid argument, file does not exist.";
                return 1;
            }
            trackingImage = MultiplyO(image1, image2);
            trackingImage.write("output/out.tga");
            cout << "argv[" << argIndex << "]: " << argv[argIndex] << endl;
        } else if (strcmp(argv[argIndex], "multiply") == 0){

            argIndex++;

            Image image;
            if (isValidFile(argv[argIndex])){
                image.read(argv[argIndex]);
            } else {
                cout << "Invalid argument, invalid file name.";
                return 1;
            }

            if(!fileExists(argv[argIndex])){
                cout << "Invalid argument, file does not exist.";
                return 1;
            }
            trackingImage = MultiplyO(trackingImage,image);
        }
        else if (argIndex == 3 && strcmp(argv[argIndex], "subtract") == 0){
            // reading 1st image
            Image image1;
            if (isValidFile(argv[argIndex - 1])){
                image1.read(argv[argIndex - 1]);
            } else {
                cout << "Invalid argument, invalid file name.";
                return 1;
            }

            if(!fileExists(argv[argIndex - 1])){
                cout << "Invalid argument, file does not exist.";
                return 1;
            }

            // increase argIndex to read following argument
            argIndex++;

            // reading in 2nd image
            Image image2;
            if (isValidFile(argv[argIndex])){
                image2.read(argv[argIndex]);
            } else {
                cout << "Invalid argument, invalid file name.";
                return 1;
            }

            if(!fileExists(argv[argIndex])){
                cout << "Invalid argument, file does not exist.";
                return 1;
            }

            // writing trackingImage
            trackingImage = Subtract(image1, image2);
            trackingImage.write("output/out.tga");

            //(9) Check if the argIndex is greater than argc
            if(argIndex >= argc){
                //(10) Throw an invalid argument is argIndex is greater than argc
                throw invalid_argument("Not enough arguments for subtract()"); // Added this
            }
            cout << "argv[" << argIndex << "]: " << argv[argIndex] << endl;
        } else if(strcmp(argv[argIndex], "subtract") == 0){
            argIndex++;
            Image image2;

            if (isValidFile(argv[argIndex])){
                image2.read(argv[argIndex]);
            } else {
                cout << "Invalid argument, invalid file name.";
                return 1;
            }

            if(!fileExists(argv[argIndex])){
                cout << "Invalid argument, file does not exist.";
                return 1;
            }

            trackingImage = Subtract(trackingImage, image2);
        }

        else if(argIndex == 3 && strcmp(argv[argIndex], "flip") == 0){
            // read in image to flip
            Image image;
            if (isValidFile(argv[argIndex - 1])){
                image.read(argv[argIndex - 1]);
            } else {
                cout << "Invalid argument, invalid file name.";
                return 1;
            }

            if(!fileExists(argv[argIndex - 1])){
                cout << "Invalid argument, file does not exist.";
                return 1;
            }

            trackingImage = reverse(image);
        } else if (strcmp(argv[argIndex], "flip") == 0) {
            trackingImage = reverse(trackingImage);
        }

        // scale red
        else if (argIndex == 3 && strcmp(argv[argIndex], "scalered") == 0){
           Image image;
            if (isValidFile(argv[argIndex - 1])){
                image.read(argv[argIndex - 1]);
            } else {
                cout << "Invalid argument, invalid file name.";
                return 1;
            }

            if(!fileExists(argv[argIndex - 1])){
                cout << "Invalid argument, file does not exist.";
                return 1;
            }
            if (argIndex + 1 >= argc){
                cout << "Missing argument.";
                return 1;
            }
           argIndex++;
            try{
                int scaleFactor = stoi(argv[argIndex]);
                trackingImage = scaleRed(trackingImage, scaleFactor);
            } catch(invalid_argument&) {
                cout << "Invalid argument, expected number.";
                return 1;
            }
        } else if (strcmp(argv[argIndex], "scalered") == 0) {
            if (argIndex + 1 >= argc){
                cout << "Missing argument.";
                return 1;
            }
            argIndex ++;
            try{
                int scaleFactor = stoi(argv[argIndex]);
                trackingImage = scaleRed(trackingImage, scaleFactor);
            } catch(invalid_argument&) {
                cout << "Invalid argument, expected number.";
                return 1;
            }
        }

        // scale blue
        else if (argIndex == 3 && strcmp(argv[argIndex], "scaleblue") == 0){
            Image image;
            image.read(argv[2]);
            argIndex++;
            if (argIndex + 1 >= argc){
                cout << "Missing argument.";
                return 1;
            }
            try{
                int scaleFactor = stoi(argv[argIndex]);
                trackingImage = scaleBlue(trackingImage, scaleFactor);
            } catch(invalid_argument&) {
                cout << "Invalid argument, expected number.";
                return 1;
            }
        } else if (strcmp(argv[argIndex], "scaleblue") == 0) {
            argIndex ++;
            if (argIndex + 1 >= argc){
                cout << "Missing argument.";
                return 1;
            }
            try{
                int scaleFactor = stoi(argv[argIndex]);
                trackingImage = scaleBlue(trackingImage, scaleFactor);
            } catch(invalid_argument&) {
                cout << "Invalid argument, expected number.";
                return 1;
            }
        }

        // scale green
        else if (argIndex == 3 && strcmp(argv[argIndex], "scalegreen") == 0){
            Image image;
            image.read(argv[2]);
            if (argIndex + 1 >= argc){
                cout << "Missing argument.";
                return 1;
            }
            argIndex++;
            try{
                int scaleFactor = stoi(argv[argIndex]);
                trackingImage = scaleGreen(trackingImage, scaleFactor);
            } catch(invalid_argument&) {
                cout << "Invalid argument, expected number.";
                return 1;
            }
        } else if (strcmp(argv[argIndex], "scalegreen") == 0) {
            argIndex ++;
            try{
                int scaleFactor = stoi(argv[argIndex]);
                trackingImage = scaleGreen(trackingImage, scaleFactor);
            } catch(invalid_argument&) {
                cout << "Invalid argument, expected number.";
                return 1;
            }
        }


        // add red
        else if (argIndex == 3 && strcmp(argv[argIndex], "addred") == 0){
            Image image;
            image.read(argv[2]);
            if (argIndex + 1 >= argc){
                cout << "Missing argument.";
                return 1;
            }
            argIndex++;
            try{
                int number = stoi(argv[argIndex]);
                trackingImage = addRed(trackingImage, number);
            } catch(invalid_argument&) {
                cout << "Invalid argument, expected number.";
                return 1;
            }
        } else if (strcmp(argv[argIndex], "addred") == 0){
            if (argIndex + 1 >= argc){
                cout << "Missing argument.";
                return 1;
            }
            argIndex++;
            try{
                int number = stoi(argv[argIndex]);
                trackingImage = addRed(trackingImage, number);
            } catch(invalid_argument&) {
                cout << "Invalid argument, expected number.";
                return 1;
            }
        }

        // add green
        else if (argIndex == 3 && strcmp(argv[argIndex], "addgreen") == 0){
            Image image;
            image.read(argv[2]);
            if (argIndex + 1 >= argc){
                cout << "Missing argument.";
                return 1;
            }
            argIndex++;
            try{
                int number = stoi(argv[argIndex]);
                trackingImage = addGreen(trackingImage, number);
            } catch(invalid_argument&) {
                cout << "Invalid argument, expected number.";
                return 1;
            }
        } else if (strcmp(argv[argIndex], "addgreen") == 0){
            argIndex++;
            try{
                int number = stoi(argv[argIndex]);
                trackingImage = addGreen(trackingImage, number);
            } catch(invalid_argument&) {
                cout << "Invalid argument, expected number.";
                return 1;
            }
        }

        // add blue
        else if (argIndex == 3 && strcmp(argv[argIndex], "addblue") == 0){
            Image image;
            image.read(argv[2]);
            if (argIndex + 1 >= argc){
                cout << "Missing argument.";
                return 1;
            }
            argIndex++;
            try{
                int number = stoi(argv[argIndex]);
                trackingImage = addBlue(trackingImage, number);
            } catch(invalid_argument&) {
                cout << "Invalid argument, expected number.";
                return 1;
            }
        } else if (strcmp(argv[argIndex], "addblue") == 0){
            argIndex++;
            try{
                int number = stoi(argv[argIndex]);
                trackingImage = addBlue(trackingImage, number);
            } catch(invalid_argument&) {
                cout << "Invalid argument, expected number.";
                return 1;
            }
        }

        // only red
        else if (argIndex == 3 && strcmp(argv[argIndex], "onlyred") == 0){
            Image image;
            image.read(argv[2]);
            trackingImage = onlyRed(image);
        } else if (strcmp(argv[argIndex], "onlyred") == 0){
            trackingImage = onlyRed(trackingImage);
        }

        // only green
        else if (argIndex == 3 && strcmp(argv[argIndex], "onlygreen") == 0){
            Image image;
            image.read(argv[2]);
            trackingImage = onlyGreen(image);
        } else if (strcmp(argv[argIndex], "onlygreen") == 0){
            trackingImage = onlyGreen(trackingImage);
        }

        // only blue
        else if (argIndex == 3 && strcmp(argv[argIndex], "onlyblue") == 0){
            Image image;
            image.read(argv[2]);
            trackingImage = onlyBlue(image);
        } else if (strcmp(argv[argIndex], "onlyblue") == 0){
            trackingImage = onlyBlue(trackingImage);
        }

        // overlay
        //(7) Check if the user entered overlay
        else if (argIndex == 3 && strcmp(argv[argIndex], "overlay") == 0) {
            // reading in the 1st image
            Image image1;
            image1.read(argv[argIndex - 1]);
            //(8) Increase the argIndex is user entered "multiply" inorder to reach the following data
            argIndex++;
            cout << "argv[" << argIndex << "]: " << argv[argIndex] << endl;
            // reading in the 2nd image
            Image image2;
            // writing the tracking image to a file
            image2.read(argv[argIndex]);
            trackingImage = Overlay(image1, image2);
        } else if (strcmp(argv[argIndex], "overlay") == 0){
            argIndex++;
            Image image;
            image.read(argv[argIndex]);
            trackingImage = Overlay(trackingImage,image);
        }

        // screen images, trackingImage is bottom layer
        else if (argIndex == 3 && strcmp(argv[argIndex], "screen") == 0){
            // reading in the 1st image
            Image image1;
            image1.read(argv[argIndex - 1]);
            //(8) Increase the argIndex is user entered "multiply" inorder to reach the following data
            argIndex++;
            cout << "argv[" << argIndex << "]: " << argv[argIndex] << endl;
            // reading in the 2nd image
            Image image2;
            // writing the tracking image to a file
            image2.read(argv[argIndex]);
            trackingImage = Screen(image2, image1);
        } else if (strcmp(argv[argIndex], "overlay") == 0){
            argIndex++;
            Image image;
            image.read(argv[argIndex]);
            trackingImage = Overlay(image, trackingImage);
        }

//        // combine
        else if(argIndex == 3 && strcmp(argv[argIndex], "combine") == 0){
            Image image1;
            image1.read(argv[argIndex - 1]);

            argIndex++;
            Image image2;
            image2.read(argv[argIndex]);

            argIndex++;
            Image image3;
            image3.read(argv[argIndex]);
            trackingImage = combine(image1, image2, image3);
        } else if (strcmp(argv[argIndex], "combine") == 0){
            argIndex++;
            Image image2;
            image2.read(argv[argIndex]);

            argIndex++;
            Image image3;
            image3.read(argv[argIndex]);

            trackingImage = combine(trackingImage, image2, image3);
        }
        trackingImage.write(argv[1]);
        //(15) Increase argIndex for each while loop iteration
        argIndex++;
    }
    return 0;
}