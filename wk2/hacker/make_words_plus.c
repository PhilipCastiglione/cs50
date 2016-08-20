/**
 * make a sweet file with added numbers
 */
#include <stdio.h>

int main(void)
{
    // read words
    FILE* words = fopen("words", "r");
    FILE* words_num = fopen("words_num", "w+");
    
    // go through every line in words and inject the word plus each
    // digit into words_num
    char line[255];
    int read_result;
    do
    {
        read_result = fscanf(words, "%s", line);
        for (int i = 48; i < 48 + 10; i++)
        {
            fputs(line, words_num);
            fputc((char) i, words_num);
            fputc('\n', words_num);
        }
    }
    while (read_result == 1);
    
    fclose(words);
    fclose(words_num);
}