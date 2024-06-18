/*
 * This program reads in a given valid two channel wav file, reverses the audio,
 * and writes it back out to a given file.
 *
 * Author: Breanna Zinky
 * Date: 9/12/2023
 *
 */

#include <stdio.h>
#include "wav.h" // Include the library header
#include <stdlib.h>

int main(int argc, char **argv)
{
/********************** READ IN FILE SECTION ***************************/
// Declare pointer for the wavFile struct.
wavFile* wavFilePtr;
// Declare pointer for the wavHeader struct.
wavHeader* wavHeaderPtr;

// Read in command line arguments for file names
// Take a file name to read from the command line
char* readFileName = argv[1];
// Take a file name to write to from the command line
char* writeFileName = argv[2];

// Check that the given arguments for the file names are not null.
if (writeFileName == NULL || readFileName == NULL){
	printf("File names to read from or write to were not given. Exiting.\n");
	exit(1);
}

// Call the function to read in the file contents.
// From calling this function, we will have a memory address of the wav_file struct
// which has the memory addresses of the header, size, and data bytes from the read in file.
wavFilePtr = setWavFile(readFileName);
// Check that the returned pointer is not null.
if (wavFilePtr == NULL){
	printf("Failure to read in file. Exiting. \n");
	exit(1);
}

/******************* VALIDATE FILE HEADER SECTION **************************/
// Call the header creator function to get a wav_header struct to hold data about the header.
wavHeaderPtr = createHeader(wavFilePtr);
// Check that the returned pointer is not null.
if(wavHeaderPtr == NULL){
	printf("Failure to create header. Exiting.");
	exit(1);
}

// Check for 'RIFF' in bytes 0-3
if ((wavHeaderPtr->riff[0] != 'R') || (wavHeaderPtr->riff[1] != 'I') || (wavHeaderPtr->riff[2] != 'F') || (wavHeaderPtr->riff[3] != 'F')){
	printf("Invalid file; the header does not include RIFF. Exiting. \n");
	exit(1);
}

// The integer in bytes 4-7 must be the file size - 8 
size_t* fileSizePtr = wavFilePtr->fileSizePtr;
size_t fileSize = *fileSizePtr;
size_t headerSize = fileSize - 8;

if (headerSize != wavHeaderPtr->numBytes){
	printf("invalid file; the header does not include the correct number of bytes (file size). Exiting. \n");
	exit(1);
}

// The file type in bytes 8-11 must be 'WAVE'
if ((wavHeaderPtr->fileType[0] != 'W') || (wavHeaderPtr->fileType[1] != 'A') || (wavHeaderPtr->fileType[2] != 'V') || (wavHeaderPtr->fileType[3] != 'E')){
	printf("Invalid file; not a WAV file. Exiting. \n");
	exit(1);
}

// The file has 2 channels
if (wavHeaderPtr->numChannels != 2){
	printf("Invalid file; it does not have 2 channels. Exiting. \n");
	exit(1);
}

// The format type is the value 1
if (wavHeaderPtr->formatType != 1){
	printf("Invalid file; the format type is not 1. Exiting. \n");
	exit(1);
}

/********************* REVERSE CODE SECTION ************************/
// Find sample size
short sampleSize = wavHeaderPtr->bitsPerSample;
// int end will be our incremental variable for moving from the last sample inwards.
int end = ((wavHeaderPtr->dataSectionSize) - sampleSize); // End = total (data section) size - sample size

// int start will be our incremental variable for moving from the start to the end of samples.
int start = 0;

// Loop though until the data section has been fully reversed
// Reverses the samples bit by bit by looping through them like an array. Uses a temporary array to store the starting sample that is replaced first.
while (start < end){
	char temp[sampleSize]; // Make a temporary array to hold a sample	
	// Fill the temporary array with the bytes in the starting sample
	for (int i = 0; i < sampleSize; i++){
	temp[i] = wavFilePtr->dataPtr[start + i];
	}
	// Set the starting sample to be the ending sample by replacing each byte
	for (int x = 0; x < sampleSize; x++){
	wavFilePtr->dataPtr[start + x] = wavFilePtr->dataPtr[end + x];
	}
	// Now set the bytes in the ending sample to the bytes in the starting sample that we stored in a temp array
	for (int j = 0; j < sampleSize; j++){
	wavFilePtr->dataPtr[end + j] = temp[j];
	}
	// Decrement end and increment start down/up a sample
	end-= sampleSize;
	start += sampleSize;
} 

/********************** PRINT FILE INFO **********************************/
printf("Success!");
printf("\nThe input file name is: %s", readFileName);
printf("\nThe output file name is: %s", writeFileName);
printf("\nThe sample rate of the wav file is: %u", wavHeaderPtr->sampleRate);
printf("\nThe file size is: %lu bytes", fileSize);
printf("\nThe number of channels in the wav file is: %hu\n", wavHeaderPtr->numChannels);

/********************* WRITE NEW FILE BACK TO DISK ***********************/
// Write the file back to disk. Call this function and give it the pointer to the struct,
// and the write file name/path.
int error;
error = newWavFile(wavFilePtr, writeFileName);
if (error != 0){
	printf("Error writing to file.");
	return error;
}
/************************************************************************/
// Free the memory
free(wavFilePtr->headerPtr);
free(wavFilePtr);

return 0;
}
