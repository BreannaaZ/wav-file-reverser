/*
 * The library file for wav.h, that includes functions
 * that reads in a wav file into a struct, sets the header of
 * a wav file to a struct, and writes a wav file back out.
 * 
 * Author: Breanna Zinky
 * Date: 9/12/2023
 *
 */

#include "wav.h" // Include the header with structs / function prototypes
#include "file_lib.h" // Header for functions to read/write the file into/from a char*.
#include <stdio.h>
#include <stdlib.h>

/*
 * The createHeader function will take the first 44 bytes of a file's contents and create a header from it,
 * by writing it into a struct.
 *
 * Args:
 * A pointer to a struct that holds the different sections of the wavFile as byte arrays.
 * Returns: 
 * A pointer to a struct that holds the different parts of the header of the wav file.
 *
 */
wavHeader* createHeader(wavFile* wavFilePtr){	
// Pointer to wavHeader struct
wavHeader* originalHeaderPtr; // Will be pointing to the space already malloc'd for wavFilePtr.
// Now set the start of wavHeader struct to the read in header from wavFile
originalHeaderPtr = (wavHeader*) wavFilePtr->headerPtr;
return originalHeaderPtr;
}

/*
 * The setWavFile function loads a wav file by calling the read_file function from the file library,
 * then takes the returned bytes and creates a new wav_file. It must then set the header, data size,
 * and pointer to the data section. It will then return a pointer to the new wav_file.
 * 
 * Args:
 * A char* (used as a string) of the wav file name to read in.
 * Returns: 
 * A pointer to a struct of the wav file with the different sections as byte arrays, and the file size.
 *
 */
wavFile* setWavFile(char* fileName){
// Declare the wav_file pointer and set it to point to originalWavFile.
// wavFilePtr holds the memory address of originalWavFile.
wavFile* wavFilePtr = malloc(sizeof(*wavFilePtr)); 

// Read in the wav file by calling the read_file function. 
size_t size;
char* file_contents = read_file(fileName, &size);

// We can now use file_contents like an array of chars
// Set the header, data size, and pointer to the data section.
wavFilePtr->headerPtr = file_contents;
wavFilePtr->fileSizePtr = &size;
wavFilePtr->dataPtr = file_contents + 44;
// Return the wavFile pointer.
return wavFilePtr;
}

/*
 * The newWavFile function takes a wav_file and a file path, and prepares a byte array of the file. It then
 * calls upon the write_file function to write the new audio file to disk.
 *
 * Args:
 * A pointer to a struct of the wav file with the different sections as byte arrays
 * A char* (used as a string) of the wav file name to write out.
 * Returns:
 * An error number.
 *
 */
int newWavFile(wavFile* newWavFilePtr, char* filePath){
// Find size of the wav file
size_t* sizePtr = newWavFilePtr->fileSizePtr;
// Create a pointer to point to the data from the file. Set it to point at the start, so where the header starts.
char* byteArrayPtr;
byteArrayPtr = newWavFilePtr->headerPtr;
// Write file
int error = write_file(filePath, byteArrayPtr, *sizePtr); 
return error;
}
