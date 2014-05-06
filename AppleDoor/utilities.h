//
//  utilities.h
//  appledoor
//
//  Created by Nahush Kulkarni on 4/18/14.
//  Copyright (c) 2014 Nahush Kulkarni. All rights reserved.
//

#include <string.h>

#ifndef appledoor_utilities_h
#define appledoor_utilities_h

static char* rootpwd="/Users/nahushrk/Desktop/kb";

#define NO 0
#define FALSE 0
#define YES 1
#define TRUE 1

#define SIZE_OF_ARRAY(x) (sizeof(x)/sizeof(x[0]))

char* substring(char* string,unsigned int startIndex, unsigned int endIndex)/*startIndex:index before the character to begin copying from, endIndes:indexof character till where to copy including the character*/
{
    if(startIndex>=strlen(string) || startIndex==endIndex)
    {
        return "";
    }
    if(endIndex>strlen(string))
    {
        endIndex=(unsigned int)strlen(string);
    }
    //newImage->imageName=malloc(sizeof(char)*strlen(iname)-2);
    //memcpy(newImage->imageName, &iname[2], strlen(iname)-2);
    char* substring=malloc(sizeof(char)*(unsigned int)(endIndex-startIndex+1));
    memcpy(substring, &string[startIndex], endIndex-startIndex);
    substring[endIndex-startIndex]='\0';
    return substring;
}

char* appendString(char* before,char* after)
{
    char *result=malloc(strlen(after)+strlen(before)+1);
    strcpy(result, before);
    strcat(result, after);
    return result;
}

unsigned int indexOfLastOccuranceOfCharacter(char c,char* string)
{
    int i=0;
    int index=-1;
    while(string[i]!='\0')
    {
        if(string[i]==c)
            index=i;
        i++;
    }
    return index;
}

int strToInt(char* string)
{
    int v=0;
    int i=0;
    while(string[i]!='\0')
    {
        if((int)string[i]<'0' || (int)string[i]>'9')
            break;
        v=v*10+((int)string[i]-'0');
        i++;
    }
    return v;
}

#endif
