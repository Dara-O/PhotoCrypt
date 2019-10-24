/*
    PROGRAM NAME: PhotoCrypt
    DATE CREATED:  20 October 2019
    LAST DATE MODIFIED: 24 October 2019
    PROGRAMMER: ISAAC (DARA) OGUNMOLA

    DESCRIPTION:
        This program Encrypts pictures by changing their RGB values.
        It only works with 24-bit, '.bmp' photos.
        It requires a user-provided encryption key to encrypt and decrypt photos
        It has user-input validation techniques

*/

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <limits>

using namespace std;

struct Pixel
{
    uint8_t b; // blue channel
    uint8_t g; // green channel
    uint8_t r; // red channel

    // constructor
    Pixel(uint8_t B, uint8_t G, uint8_t R)
    :b(B), g(G), r(R)
    {
    }


};

class BMPImage
{
private:
    int padding;
    uint8_t paddingDump;

public:

    BMPImage(string fileName)
    {


        // check if the file extension is .bmp
        if(fileName.substr(fileName.length()-4, 4) != ".bmp")
        {
            cout << "***FILE INVALID***\n";
            cout << "Please pass in a .bmp file.\n";
            exit(0);
        }

        // open the file for reading
        fstream file(fileName, ios::in | ios::binary);

        // check if the file was opened successfully
        if (!file)
        {
            // Print an error and exit
            cerr << "*** FILE ERROR ***\n"
                 << "Uh oh, " << fileName << "could not be opened for writing!"
                 << endl;
            exit(1);
        }

        // read to signature
        file.read((char*)&signature, 2);

        // check if it is a TRUE .bmp file
        if(signature != 19778)
        {
            cout << "*** FILE INVALID ***\n";
            cout << "This is not a true bmp file.\n";
            exit(0);
        }

        // read to fileSize
        file.read((char*)&fileSize, 4);

        // read to reserved
        file.read((char*)&reserved, 4);

        // read to dataOffSet
        file.read((char*)&dataOffSet, 4);

        // read to infoHeaderSize
        file.read((char*)&infoHeaderSize, 4);

        // check if the file is corrupt in some way
        if(infoHeaderSize != 40)
        {
            cout << "*** FILE CORRUPT ***\n";
            cout << "The file is corrupt.\n";
        }

        // read to width
        file.read((char*)&width, 4);

        // calculate the padding
        padding = (4 - (width*3)%4)%4;

        // read to height
        file.read((char*)&height, 4);

        // read to planes
        file.read((char*)&planes, 2);

        // read to colorDepth
        file.read((char*)&colorDepth, 2);

        // check if colour depth is 24
        if(colorDepth != 24)
        {
            cout << "*** COLOUR DEPTH ERROR ***\n";
            cout << "The colour depth of the image is: " << colorDepth << endl;
            cout << "This application only supports a colour depth of 24 bits\n.";
            exit(0);
        }

        // read to compression
        file.read((char*)&compression, 4);

        // read to image size
        file.read((char*)&imageSize, 4);

        // read to XpixelsPerM
        file.read((char*)&XpixelsPerM, 4);

        // read to YpixelsPerM
        file.read((char*)&YPixelsPerM, 4);

        // read to colorsUsed
        file.read((char*)&colorsUsed, 4);

        //read to importantColors
        file.read((char*)&importantColors, 4);

        // populate the pixel array
        // outer loop for each row
        for(uint32_t scanlineIndex = 0; scanlineIndex < height; ++scanlineIndex)
        {
            // inner loop for each pixel
            for(uint32_t pixelIndex = 0; pixelIndex < width; ++pixelIndex)
            {
                uint8_t b;
                uint8_t g;
                uint8_t r;

                file.read((char*)&b, 1);
                file.read((char*)&g, 1);
                file.read((char*)&r, 1);

                pixelArray.push_back(Pixel(b, g, r));
            }

            // take the padding out
            for(uint8_t i = 0; i < padding; ++i)
                file.read((char*)&paddingDump, 1);
        }

    }

    //BITMAPFILEHEADER

    uint16_t signature; // =BM
    uint32_t fileSize;  // in bytes
    uint32_t reserved;  // =0
    uint32_t dataOffSet;// the offset from the start of the file where the pixel array starts

    //BITMAPINFOHEADER

    uint32_t infoHeaderSize;    // = 40
    uint32_t width;             // in pixels (check this value to know how much padding is added)
    uint32_t height;            // in pixels
    uint16_t planes;            // = 1
    uint16_t colorDepth;        // for the purpose of this app it must be 24
    uint32_t compression;       // should be 0
    uint32_t imageSize;         //width x height
    uint32_t XpixelsPerM;
    uint32_t YPixelsPerM;
    uint8_t colorsUsed;
    uint32_t importantColors;   //=0  if all colors important

    // WE WILL SKIP THE COLOR TABLE BECAUSE OUR COLOR DEPTH IS > 8

    // BECAUSE THE COLOR DEPTH IS 24, THERE ARE 3 BYTES PER PIXEL
    // ONE FOR RGB, BUT REVERSED
    // IF THE WIDTH IS NOT DIVISIBLE BY 4, PADDING IS ADDED,
    // THIS IS TO BE CALCULATED AND AVOIDED


    vector<Pixel> pixelArray; // what we are here for


    // METHODS
    void write(string fileName)
    {
        if(fileName.substr(fileName.length()-4, 4) != ".bmp")
        {
            fileName += ".bmp";
        }

        fstream file(fileName, ios::out | ios::binary);

        // write the signature
        file.write((char*)&signature, 2);

        // write the fileSize
        file.write((char*)&fileSize, 4);

        // write the reserved
        file.write((char*)&reserved, 4);

        // write the dataOffset
        file.write((char*)&dataOffSet, 4);

        // write the size
        file.write((char*)&infoHeaderSize, 4);

        //write the width
        file.write((char*)&width, 4);

        //write the height
        file.write((char*)&height, 4);

        // write the planes
        file.write((char*)&planes, 2);

        // write the bitCount
        file.write((char*)&colorDepth, 2);

        // write the compression
        file.write((char*)&compression, 4);

        // write the imageSize
        file.write((char*)&imageSize, 4);

        // write the XpixelsPerM
        file.write((char*)&XpixelsPerM, 4);

        // write the YpixelsPerM
        file.write((char*)&YPixelsPerM, 4);

        // write the colorsUsed
        file.write((char*)&colorsUsed, 4);

        // write the importantColors
        file.write((char*)&importantColors, 4);

        // write the pixelArray
        // outer loop for each row
        for(int scanlineIndex = 0; scanlineIndex < height; ++scanlineIndex)
        {
            // inner loop for each pixel
            for(int pixelIndex = 0; pixelIndex < width; ++pixelIndex)
            {
                file.write((char*)&pixelArray[pixelIndex + scanlineIndex*width].b, 1);
                file.write((char*)&pixelArray[pixelIndex + scanlineIndex*width].g, 1);
                file.write((char*)&pixelArray[pixelIndex + scanlineIndex*width].r, 1);
            }

            // making sure paddingDump is 0
            paddingDump = 0;

            // write the padding
            for(uint8_t i = 0; i < padding; ++i)
                file.write((char*)&paddingDump, 1);
        }
    }
};

// encryption function
void encrypt(vector<Pixel>& pixelArray, int key)
{
    srand(key);
    uint8_t factor = 0;

    for(int i = 0; i < pixelArray.size(); ++i)
    {
        // encrypt blue channel
        factor = rand() % 256; // to generate a new factor
        uint8_t &blueChannel = (pixelArray[i].b);
        blueChannel =  factor + blueChannel > 255 ? (factor + blueChannel)%256 : factor + blueChannel;

        //encrypt green Channel
        factor = rand() % 256; // to generate a new factor
        uint8_t &greenChannel = (pixelArray[i].g);
        greenChannel =  factor + greenChannel > 255 ? (factor + greenChannel)%256 : factor + greenChannel;

        //encrypt red channel
        factor = rand() % 256; // to generate a new factor
        uint8_t &redChannel = (pixelArray[i].r);
        redChannel =  factor + redChannel > 255 ? (factor + redChannel)%256 : factor + redChannel;
    }
}

// decryption function
void decrypt(vector<Pixel>& pixelArray, int key)
{
    srand(key);
    uint8_t factor = 0 ;

    for(int i = 0; i < pixelArray.size(); ++i)
    {
        // decrypt blue channel
        factor = rand() % 256;
        uint8_t &blueChannel = (pixelArray[i].b);
        blueChannel = blueChannel - factor < 0 ? 256 + (blueChannel - factor) : blueChannel - factor;

        // decrypt the green channel
        factor = rand() % 256;
        uint8_t &greenChannel = (pixelArray[i].g);
        greenChannel = greenChannel - factor < 0 ? 256 + (greenChannel - factor) : greenChannel - factor;

        // decrypt the red channel
        factor = rand() % 256;
        uint8_t &redChannel = (pixelArray[i].r);
        redChannel = redChannel - factor < 0 ? 256 + (redChannel - factor) : redChannel - factor;
    }
}

int main()
{
    // WELCOME MESSAGE
    cout << "Welcome to the photo-encryption program.\n";
    cout << "To encrypt a photo, type '1' and press enter\n";
    cout << "To decrypt a photo, type '2' and press enter\n";
    cout << "Choice: ";

    int choice;
    while(true)
    {
        cin >> choice;

        if(cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
        }

        if(choice != 1 && choice != 2)
        {
            cout << "\n*** Invalid entry ***\n";
            cout << "To encrypt a photo, type '1' and press enter\n";
            cout << "To decrypt a photo, type '2' and press enter\n";
            cout << "Choice: ";
        }
        else
            break;
    }

    // ENCRYPT A PHOTO CHOICE
    if(choice == 1)
    {
        cout << "\n==================================================================\n";
        cout << "File Requirements:\n";
        cout << "\t- The file must be a bitmap image with a colour depth of 24 bits.\n";
        cout << "\t- It must be in the same folder as this application.\n";
        cout << "\t- The file name must end in '.bmp'\n\n";

        cout << "Enter the name of the file\n"
             << "(make sure that it is in the same folder as this application).\n";
        cout << "Filename: ";

        string sourceFileName;
        cin.ignore();
        getline(cin, sourceFileName);

        cout << "\n==================================================================\n";

        cout << "Opening the file...\n";

        // CREATE THE BMPImage Object
        BMPImage pic(sourceFileName);

        // SHAVE OFF THE ".bmp" FROM THE FILE NAME
        sourceFileName.erase(sourceFileName.size()-4);

        cout << "File opened successfully.\n";

        cout << "\n==================================================================\n";

        cout << "Your image has " << pic.pixelArray.size() << " pixels.\n";

        cout << "\n==================================================================\n";

        // GET THE ENCRYPTION KEY
        int key;
        do
        {
            if(cin.fail())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(),'\n');

                cout << "\n*** Invalid input! ***\n";

            }

            cout << "\nPlease enter your encryption key (numbers only): ";
            cin  >> key;

        }while(cin.fail());

        cout << "Key accepted.\n";

        cout << "\n==================================================================\n";

        cout << "Encrypting the Image..\n";

        // ENCRYPTING THE IMAGE
        encrypt(pic.pixelArray, key);

        cout << "Image successfully encrypted.\n";

        cout << "\n==================================================================\n";

        cout << "Creating encrypted image...\n";

        // CREATING THE ENCRYPTED IMAGE
        pic.write(sourceFileName+"_encrypted.bmp");

        cout << "Encrypted image successfully created.\n\n";
        cout << "File name: " << sourceFileName+"_encrypted.bmp" << endl;

        cout << "\n==================================================================\n";
    }

    // DECRYPT A PHOTO CHOICE
    else if(choice == 2)
    {
        cout << "\n==================================================================\n";

        cout << "File Requirements:\n";
        cout << "\t- The file must be a bitmap image with a colour depth of 24 bits.\n";
        cout << "\t- It must be in the same folder as this application.\n";
        cout << "\t- The file name must end in '.bmp'\n\n";

        cout << "\nEnter the file name of the encrypted image: ";

        string encryptedFileName;
        cin.ignore();
        getline(cin, encryptedFileName);

        cout << "\n==================================================================\n";

        cout << "Opening the encrypted Image...\n";

        // CREATE THE BMPImage Object
        BMPImage pic(encryptedFileName);

        // SHAVE OFF THE ".bmp" FROM THE FILE NAME
        encryptedFileName.erase(encryptedFileName.size()-4);

        cout << "Image successfully opened.\n";

        cout << "\n==================================================================\n";

        // GET THE DECRYPTION KEY
        int key;
        do
        {
            if(cin.fail())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(),'\n');

                cout << "\n*** Invalid input! ***\n";

            }

            cout << "\nPlease enter your encryption key (numbers only): ";
            cin  >> key;

        }while(cin.fail());

        cout << "Key accepted.\n";

        cout << "\n==================================================================\n";

        cout << "Decrypting image...\n";

        // DECRPYT THE PHOTO
        decrypt(pic.pixelArray, key);

        cout << "Image successfully decrypted.\n";

        cout << "\n==================================================================\n";

        cout << "Creating decrypted Image...\n";

        // WRITE THE DECRYPTED IMAGE
        pic.write(encryptedFileName+"_decrypted.bmp");

        cout << "Decrypted image successfully created.\n\n";
        cout << "File name: " << encryptedFileName + "_decrypted.bmp\n";

        cout << "\n==================================================================\n";
    }


    // PREVENT THE CONSOLE FROM CLOSING
    string esc;
    cout << "\n\nPress enter to close the program...";
    cin.ignore();
    getline(cin, esc);

    return 0;
}
