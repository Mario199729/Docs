#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
char *read_sentence()
{
    int size = 10;
    char *str = calloc(size, sizeof(char));
    char symb = getchar();
    int count = 0;

    if (symb == ' ' || symb == '\t')
    {
        symb = getchar();
    }

    while (symb != '\n' && symb != '.')
    {
        if (count >= size)
        {
            size += size;
            str = realloc(str, size * sizeof(char));
        }

        str[count] = symb;
        count = count + 1;
        symb = getchar();
    }

    if (symb != '\n')
    {
        str[count] = symb;
        str[count + 1] = '\0';
    }
    else
    {
        str[count] = '\0';
    }

    return str;
}

int main()
{
    int size = 10;
    char **text = calloc(size, sizeof(char *));
    int sizeText = 0;
    char *str;

    do
    {
        str = read_sentence();

        if (sizeText >= size)
        {
            size += size;
            text = realloc(text, size * sizeof(char *));
        }

        text[sizeText] = str;
        sizeText++;
    } while (strcmp(text[sizeText - 1], "Fin."));

    regex_t regexCompiled;
    size_t maxGroups = 3;
    regmatch_t groupArray[maxGroups];

    if (regcomp(&regexCompiled, "(\\w+)@[A-Za-z0-9_-]+:\\s?~\\s?# (.*)", REG_EXTENDED))
    {
        printf("Regex not compiled!\n");
        return 0;
    }

    for (int i = 0; i < sizeText; i++)
    {
        if (!regexec(&regexCompiled, text[i], maxGroups, groupArray, 0))
        {
            for (int j = groupArray[1].rm_so; j < groupArray[1].rm_eo; j++)
            {
                printf("%c", text[i][j]);
            }

            printf(" - ");

            for (int k = groupArray[2].rm_so; k < groupArray[2].rm_eo; k++)
            {
                printf("%c", text[i][k]);
            }

            printf("\n");
        }
    }

    regfree(&regexCompiled);

    for (int i = 0; i < sizeText; i++)
    {
        free(text[i]);
    }

    free(text);

    return 0;
}