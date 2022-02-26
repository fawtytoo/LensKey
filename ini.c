//  lenskey

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

typedef struct
{
    char        *name;
    char        *start;
    int         lines;
} SECTION;

typedef struct stat     STATUS;

char        *buffer = NULL;
SECTION     *section = NULL;
int         count = 0;

char *skipSpace(char *pos)
{
    while (*pos == ' ' || *pos == '\t')
        pos++;

    if (*pos == 0) // end of line
        return NULL;

    return pos;
}

char *getValuePos(char *pos, char *key)
{
    int         length;

    if ((pos = skipSpace(pos)) == NULL)
        return NULL;

    if (*pos == '#')
        return NULL;

    length = strlen(key);

    if (strncasecmp(pos, key, length))
        return NULL;

    if ((pos = skipSpace(pos + length)) == NULL)
        return NULL;

    if (*pos++ != '=')
        return NULL;

    if ((pos = skipSpace(pos)) == NULL)
        return NULL;

    return pos;
}

int Cfg_Open(char *file)
{
    FILE        *handle;
    STATUS      status;
    char        *posOld, *posNew;

    if (stat(file, &status) < 0)
        return 1;

    if (!(handle = fopen(file, "rb")))
        return 1;

    posOld = buffer = malloc(status.st_size);
    fread(buffer, 1, status.st_size, handle);
    fclose(handle);

    while ((posNew = strchr(posOld, '\n')))
    {
        *posNew = 0;
        if (*posOld == '[' && *(posNew - 1) == ']')
        {
            *(posNew - 1) = 0;
            count++;
            section = realloc(section, sizeof(SECTION) * count);
            section[count - 1].start = posNew + 1;
            section[count - 1].name = posOld + 1;
            section[count - 1].lines = 0;
        }
        else if (count)
            section[count - 1].lines++;

        posOld = posNew + 1;
    }
    if (count == 0)
        return 1;

    return 0;
}

void Cfg_Close(void)
{
    if (section)
        free(section);

    if (buffer)
        free(buffer);
}

char *getValue(char *cat, char *key)
{
    int         i, j;
    char        *value, *pos;
    static int  start = 0;

    if (start > 0)
        if (strcasecmp(section[start].name, cat))
            start = 0;

    for (i = start; i < count; i++)
        if (strcasecmp(section[i].name, cat) == 0)
        {
            start = i;
            pos = section[i].start;
            for (j = 0; j < section[i].lines; j++)
            {
                if ((value = getValuePos(pos, key)))
                    return value;

                pos += strlen(pos) + 1;
            }
            break;
        }

    return NULL;
}

int Cfg_GetInteger(char *cat, char *key, int this)
{
    char        *value, n[11];
    int         number;

    if ((value = getValue(cat, key)) == NULL)
        return this;

    number = atoi(value);
    sprintf(n, "%i", number);

    if (strcmp(value, n))
        return this;

    return number;
}

//
