#include <cstdio>
#include <iostream>

#define SAME_ENDIAN 0
#define DIFF_ENDIAN 1

#define LITTLE_ENDIAN_PROCESSOR 0
#define BIG_ENDIAN_PROCESSOR 1

#define PROCESSOR_NONE  83
#define PROCESSOR_INTEL 84
#define PROCESSOR_DEC   85
#define PROCESSOR_MIPS  86

#define FORMAT_CHAR  -1
#define FORMAT_BYTE   1
#define FORMAT_INT_16 2
#define FORMAT_FLOAT  4

#define SIZE_8_BIT  1
#define SIZE_16_BIT 2
#define SIZE_32_BIT 4

/*************/
/* Templates */
/*************/

/*
 * Return the system reading Endian
 */
namespace system_endian {
    const unsigned one = 1U;

    inline bool little_endial() {
        return reinterpret_cast<const char*>(&one) + sizeof(unsigned) - 1;
    }

    inline bool big_endian() {
        return !little_endial();
    }
}

/*
 * This function Swap the bits on a given number.
 * For example the sequence:
 * 0000 0101
 *
 * can be read with 2 methods.
 * 1) RIGHT -> LEFT  (little endian) = 1*1 + 0*2 + 1*4 + 0*8 + 0*16 + 0*32 + 0*64 + 0*128 =   5
 * 2) LEFT  -> RIGHT (big endian)    = 0*1 + 0*2 + 0*4 + 0*8 + 0*16 + 1*32 + 0*64 + 1*128 = 160
 */
template <typename T>
T swapEndian(const T numIn) {
    T numOut;
    int size = sizeof(T);

    char* numToConvert = (char*) &numIn;
    char* numToReturn = (char*) &numOut;

    for(int i = 0, j = size-1; j>=0; i++, j--)
        numToReturn[i] = numToConvert[j];

    return numOut;
}

/*
 * This function return the number of a bit 0 or 1.
 */
template <typename T>
T returnByte(const T numIn, const int byte) {
    T numOut;

    int size = sizeof(T);

    char* numToConvert = (char*) &numIn;
    char* numToReturn = (char*) &numOut;

    for(int i = 0; i < size; i++)
        numToReturn[i] = 0;

    numToReturn[0] = numToConvert[byte - 1];

    return numOut;
}

/*
 * Read a Word From File Without knowing the Endian
 */
template <typename T>
void readFromFile(T* word, const int bytes, FILE* file) {
    char* wordToRead = (char*) word;

    int size = sizeof(T);
    if(size > bytes)
        for(int i = 0; i < bytes; i++)
            wordToRead[i+1] = 0;

    for(int i = 0; i < bytes; i++) {
        wordToRead[i] = 0;
        fread(&wordToRead[i], 1, 1, file);
    }

}

/*
 * Read a Word From File with Endian
 */
template <typename T>
void readFromFileWithEndian(T* word, const int bytes, FILE* file, const int flag) {
    char* wordToRead = (char*) word;

    int size = sizeof(T);
    if(size > bytes)
        for(int i = 0; i < bytes; i++)
            wordToRead[i+1] = 0;

    /* If SAME_ENDIAN */
    if(flag == SAME_ENDIAN)
        for(int i = 0; i < bytes; i++) {
            wordToRead[i] = 0;
            fread(&wordToRead[i], 1, 1, file);
        }

    /* If DIFF_ENDIAN */
    else if (flag == DIFF_ENDIAN) {
        if(bytes > SIZE_8_BIT)
            for(int i = bytes-1; i >= 0; i--) {
                wordToRead[i] = 0;
                fread(&wordToRead[i], 1, 1, file);
            }
        else
            for(int i = 0; i < bytes; i++) {
                wordToRead[i] = 0;
                fread(&wordToRead[i], 1, 1, file);
            }
    }
}

/*******************/
/* Other Functions */
/*******************/

/*
 * Read the words in a sequence of character string.
 */
int wordCounter(const char* text);

/*
 * Return a specific word from text.
 */
std::string wordReturn(const char* text, const int index);

