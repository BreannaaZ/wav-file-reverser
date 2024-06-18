#ifndef WAV_H_ // INCLUDE GUARD
#define WAV_H_
#include "file_lib.h"

/*
 * Header file of wav.c that includes the two structs for the wav file;
 * holding the header and the byte array sections.
 * Includes functions to create a header struct, read in a wav file into a struct,
 * and write out a wav file uiing the struct.
 *
 * Author: Breanna Zinky
 * Date: 9/12/2023
 *
 */

// Struct for a wav file header called a wav_header
// This struct will hold all the pieces of the header
typedef struct wav_header{
char riff[4]; // Bytes 0-3
unsigned int numBytes; // Bytes 4-7
char fileType[4]; // Bytes 8-11
char formatChunk[4]; // Bytes 12-15
unsigned int formatDataLength; // Bytes 16-19; a 32 bit integer
unsigned short formatType; // Bytes 20-21; a 16 bit integer
unsigned short numChannels; // Bytes 22-23; 16-bit integer
unsigned int sampleRate; // Bytes 24-27; 32-bit integer
unsigned int sampleRateBitsChannels; // Bytes 28-31
unsigned short bitsPerSampleChannelsDataTypeSize; // Bytes 32-33
unsigned short bitsPerSample; // Bytes 34-35
char dataHeader[4]; // Bytes 36-39
unsigned int dataSectionSize; // Bytes 40-43
} wavHeader; 

// Struct for a wav file that has a pointer to the header, the file size,
// and a pointer to the data - essentially holds all the bytes of the wav file
typedef struct wav_file{
size_t* fileSizePtr;
char* headerPtr;
char* dataPtr;
} wavFile;

/*
* The createHeader function will take the first 44 bytes of a file's contents and create a header from it,
* by writing it into a struct.
*
* Args:
* A pointer to a struct that holds the different sections of the wavFile as byte arrays.
* Returns:
* A pointer to a struct that holds the different parts of the header of the wav file.
*/
wavHeader* createHeader(wavFile*);

/* 
* The setWavFile function loads a wav file by calling the read_file function from the file library,
* then takes the returned bytes and creates a new wav_file. It must then set the header, data size,
* and pointer to the data section. It will then return a pointer to the new wavFile (wav_file struct).
*
* Args:
* A char* (used as a string) of the wav file name to read in.
* Returns:
* A pointer to a struct of the wav file with the different sections as byte arrays, and the file size.
*/
wavFile* setWavFile(char*);

/*
* The newWavFile function takes a wav_file and a file path, and prepares a byte array of the file. It then
* calls upon the write_file function to write the new audio file to disk.
*
* Args:
* A pointer to a struct of the wav file with the different sections as byte arrays
* A char* (used as a string) of the wav file name to write out.
* Returns:
* An error number.
*/ 
int newWavFile(wavFile*, char*);

#endif // END OF INCLUDE GUARD - WAV_H_
