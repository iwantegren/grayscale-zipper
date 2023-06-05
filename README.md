# Grayscale zipper
## Image Compression Library and UI

This project consists of an image compression library and a user interface (UI) built using Qt Quick/QML. The library allows for encoding and decoding of grayscale images in specific data structure, while the UI provides a convenient way to interact with the library's functionality.

## Launch

To launch the project, use the provided bash script `run.sh`. This script collects the shared library, the project itself, and runs it with the `res/` directory.

> bash run.sh

## Image Compression Library

The image compression library is implemented in C++. It provides functions to encode and decode grayscale images according to the following specifications:

- The input image is represented by the `RawImageData` structure:
```
struct RawImageData
{
    int width; // image width in pixels
    int height; // image height in pixels
    unsigned char * data; // Pointer to image data. data[j * width + i] is color of pixel in row j and column i.
}
```
- Each pixel in the image is represented by values from 0 to 255 shades of gray.
- The image may contain white rows and non-empty rows with white areas.
- The library optimizes storage space by encoding the image as follows:
  - For each row of pixels, the library stores whether it is empty or not.
  - Each non-empty line is encoded as follows:
    - 4 consecutive white pixels are represented by code 0b0.
    - 4 consecutive black pixels are represented by code 0b10.
    - Any other sequence of 4 pixels is encoded as 0b11 + pixel colors.

The library provides encoding and decoding functions to convert the `RawImageData` structure to the specified representation and restore the original image during decoding. The resulting encoded image is saved to a file with the `.barch` extension.

## Image Compression UI

The UI is built using Qt and QML. It allows for launching the program with a directory path parameter (defaulting to the current directory if not specified). The UI features include:

- QML plugin: A plugin containing QML components used in the application, such as the model, button, and dialog.
- C++ model: A model that holds a list of files in the current directory (or the directory specified via command line parameter), with additional attributes like size. The model provides filtering by file extension.
- Displaying a list of files: The program window shows a columnar list of files with their sizes, using the model from the previous point. Only `.bmp`, `.png`, and `.barch` files are displayed.
- Handling file selection: When a file is clicked in the list, the program checks its extension. If it's a `.bmp` or `.barch` file, it is encoded (or decoded) using the image compression library in a separate thread. The list displays "Encoded" or "Decoded" next to the file name during the process. Encoded files are saved with the filename `filename_packed.barch`, while unpacked files use `filename_unpacked.bmp`. If the file is of another type, an error dialog is displayed.
- Error dialog: An error dialog with a single "Ok" button is shown in case of errors.
