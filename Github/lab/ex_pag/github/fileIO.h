#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#define PNG_DEBUG 3
#include <png.h>
#include "structs.h"
int readFile(char *nameFile, PNG *image);
int writeFile(char *nameFile, PNG *image);