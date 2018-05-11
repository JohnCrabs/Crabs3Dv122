#include <cstring>
#include "templates.h"

/*
 * Read the words in a sequence of character string.
 */
int wordCounter(const char* text) {
    int count = 0;
    int i = 0;

    while(i < (int)strlen(text) || text[i] != '\0') {
        while (!isgraph(text[i]) && text[i] != '\0')
            i++;
        if(isgraph(text[i])) {
            count++;
            while (isgraph(text[i]) && text[i] != '\0') {
                i++;
            }
        }
    }

    return count;
}

/*
 * Return a specific word from text.
 */
std::string wordReturn(const char* text, const int index) {
    int count = 0;
    int i = 0;
    std::string word = "";

    while(i < (int)strlen(text) || text[i] != '\0') {
        while (!isgraph(text[i]) && text[i] != '\0')
            i++;
        if(isgraph(text[i])) {
            count++;
            if(count == index) {
                while (isgraph(text[i]) && text[i] != '\0') {
                    word += text[i];
                    i++;
                }
                break;
            } else {
                while (isgraph(text[i]) && text[i] != '\0')
                    i++;
            }
        }
    }

    return word;
}
