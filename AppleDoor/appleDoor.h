//
//  appleDoor.h
//  appledoor
//
//  Created by Nahush Kulkarni on 4/19/14.
//  Copyright (c) 2014 Nahush Kulkarni. All rights reserved.
//

#ifndef include_stdio_h
#define include_stdio_h
#include <stdio.h>
#endif
#ifndef include_dirent_h
#define include_dirent_h
#include <dirent.h>
#endif
#ifndef include_string_h
#define include_string_h
#include <string.h>
#endif
#ifndef include_unistd_h
#define include_unistd_h
#include <unistd.h>
#endif
#ifndef include_sysstat_h
#define include_sysstat_h
#include <sys/stat.h>
#endif
#ifndef appledoor_utilities_h
#include "utilities.h"
#endif
#ifndef appledoor_mindPalace_h
#include "mindPalace.h"
#endif


#ifndef appledoor_appleDoor_h
#define appledoor_appleDoor_h

//static char* rootpwd="/Users/nahushrk/Desktop/kb";

#define MKDIR_PERM S_IRWXU|S_IRGRP|S_IROTH

char* ad_getPathFromName(char* path,char* name)
{
    return appendString(appendString(path, "/"),name);
}
char* ad_getPathOfFName(char* fname)
{
    DIR *dirP=opendir(rootpwd);
    struct dirent *dirEntries=NULL;
    while(((dirEntries=readdir(dirP))!=NULL) && (dirEntries->d_type&DT_DIR))
    {
        if(((strcmp(dirEntries->d_name, ".")==0)||(strcmp(dirEntries->d_name, "..")==0)))
            continue;
        if(strcmp(dirEntries->d_name, fname)==0)
        {
            closedir(dirP);
            return ad_getPathFromName(rootpwd, fname);
        }
    }
    closedir(dirP);
    return "";
}
char* ad_createFNameFromFrame(Frame *frame)
{
    char* fname=frame->frameName;
    fname=appendString(fname, ",");
    char fid[10];
    sprintf(fid, "%d",frame->frameID);
    return appendString(fname, fid);
}
char* ad_createINameFromImage(Image* image)
{
    char* iname="";
    iname=appendString(iname, &image->symbol);
    iname=appendString(iname, "_");
    iname=appendString(iname, image->imageName);
    iname=appendString(iname, ",");
    char iid[10];
    sprintf(iid, "%d",image->imageID);
    return appendString(iname, iid);
}
unsigned int ad_frameIsInKB(char* frameName)
{
    DIR *dirP=opendir(rootpwd);
    struct dirent *dirEntries = NULL;
    while((dirEntries=readdir(dirP))!=NULL)
    {
        if(dirEntries->d_type&DT_DIR)
        {
            if(!(strcmp(dirEntries->d_name, "."))||(!strcmp(dirEntries->d_name, "..")))
            {
                continue;
            }
            if(!strcmp(frameName, dirEntries->d_name))
            {
                return 1;
            }
        }
        
    }
    return 0;
}
unsigned int ad_imageIsInFrame(Frame* frame,char* iname)
{
    DIR *dirP=opendir(appendString(rootpwd, frame->frameName));
    struct dirent *dirEntries = NULL;
    while((dirEntries=readdir(dirP))!=NULL)
    {
        if(dirEntries->d_type&DT_DIR)
        {
            if(!(strcmp(dirEntries->d_name, "."))||(!strcmp(dirEntries->d_name, "..")))
            {
                continue;
            }
            if(!strcmp(iname, dirEntries->d_name))
            {
                closedir(dirP);
                return 1;
            }
        }
        
    }
    closedir(dirP);
    return 0;
}
void ad_writeImageToLocation(Image *image,char* path)
{
    if(image==NULL)
    {
        return;
    }
    Image *imageParser=image;
    char* imageWritePath=ad_getPathFromName(path, ad_createINameFromImage(image));
    mkdir(imageWritePath, MKDIR_PERM);
    if(image->imageDefination!=NULL)
    {
        while(imageParser->imageDefination!=NULL)
        {
            ad_writeImageToLocation(imageParser->imageDefination,imageWritePath);
            imageParser=imageParser->imageDefination;
        }
    }
    if(image->nextImage!=NULL)
    {
        imageParser=image;
        while(imageParser->nextImage!=NULL)
        {
            ad_writeImageToLocation(imageParser->nextImage, path);
            //mkdir(ad_getPathFromName(path, imageParser->imageName), MKDIR_PERM);
            imageParser=imageParser->nextImage;
        }
    }
}
void ad_writeNewFrame(Frame* newFrame)
{
    if(newFrame==NULL)
    {
        return;
    }
    char* framePath=ad_getPathFromName(rootpwd, ad_createFNameFromFrame(newFrame));
    mkdir(framePath, MKDIR_PERM);
    if(newFrame->frameDefinations!=NULL)
    {
        Image *imageParser=newFrame->frameDefinations;
        while(imageParser->nextImage!=NULL)
        {
            ad_writeImageToLocation(imageParser, framePath);
            imageParser=imageParser->nextImage;
        }
        if(imageParser!=NULL)
        {
            ad_writeImageToLocation(imageParser, framePath);
        }
    }
}
unsigned int ad_addNewImageToFrame(Image* image,Frame *frame)
{
    return 0;
    //return !mkdir(iname, MKDIR_PERM);
}
unsigned int ad_directoryIsNotEmpty(char* path)
{
    DIR *dirPointer=opendir(path);
    struct dirent *dirDescEntry=NULL;
    while((dirDescEntry=readdir(dirPointer))!=NULL)//traverse all contents of dir
    {
        if(dirDescEntry->d_type&DT_DIR && !((strcmp(dirDescEntry->d_name, ".")==0)||(strcmp(dirDescEntry->d_name, "..")==0)))//is a dir and not . or ..
        {
            closedir(dirPointer);
            return 1;
        }
    }
    closedir(dirPointer);
    return 0;
}
Image* ad_getImageParametersFromDirectory(char* imagePath)
{
    Image *newImage=(Image*)malloc(sizeof(Image));
    newImage->imageDefination=NULL;
    newImage->nextImage=NULL;
    newImage->parentImage=NULL;
    int indexOfImage=indexOfLastOccuranceOfCharacter('/', imagePath)+1;
    newImage->symbol=substring(imagePath, indexOfImage, indexOfImage+1)[0];
    int indexOfNameEnd=indexOfLastOccuranceOfCharacter(',', imagePath);
    newImage->imageName=substring(imagePath, indexOfImage+2, indexOfNameEnd);
    newImage->imageID=strToInt(substring(imagePath, indexOfNameEnd+1, (int)strlen(imagePath)));
    return newImage;
}
Image* ad_deserializeImage(char* imagePath)
{
    Image *newImage=ad_getImageParametersFromDirectory(imagePath);
    DIR *dirP=opendir(imagePath);
    struct dirent *dirEntries=NULL;
    while(((dirEntries=readdir(dirP))!=NULL) && (dirEntries->d_type&DT_DIR))
    {
        if(((strcmp(dirEntries->d_name, ".")==0)||(strcmp(dirEntries->d_name, "..")==0)))
            continue;
        if(ad_directoryIsNotEmpty(ad_getPathFromName(imagePath, dirEntries->d_name)))//directory has description of its own
        {
            mp_addImageToDescriptionOfParent(ad_deserializeImage(ad_getPathFromName(imagePath, dirEntries->d_name)), newImage);
        }
        else
        {
            Image* imageParser=newImage;
            while(imageParser->nextImage!=NULL)
            {
                imageParser=imageParser->nextImage;
            }
            mp_addImageToDescriptionOfParent(ad_deserializeImage(ad_getPathFromName(imagePath, dirEntries->d_name)), newImage);
        }
        //imageParser->nextImage=ad_deserializeImage(ad_getPathFromName(imagePath, dirEntries->d_name));
    }
    closedir(dirP);
    return newImage;
}
Frame* ad_getFrameParametersFromDirectory(char* framePath)
{
    Frame* newFrame=(Frame*)malloc(sizeof(Frame));
    newFrame->frameDefinations=NULL;
    newFrame->nextFrame=NULL;
    newFrame->previousFrame=NULL;
    int indexOfFrame=indexOfLastOccuranceOfCharacter('/', framePath);
    int indexOfFNameEnd=indexOfLastOccuranceOfCharacter(',', framePath);
    newFrame->frameName=substring(framePath, indexOfFrame+1, indexOfFNameEnd);
    newFrame->frameID=strToInt(substring(framePath, indexOfFNameEnd+1, (int)strlen(framePath)));
    return newFrame;
}
Frame* ad_deSerializeFrame(char* framePath)
{
    Frame* newFrame=ad_getFrameParametersFromDirectory(framePath);
    //Image* imagePointer=newFrame->frameDefinations;
    DIR *dirP=opendir(framePath);
    struct dirent *dirEntries=NULL;
    while(((dirEntries=readdir(dirP))!=NULL) && (dirEntries->d_type&DT_DIR))
    {
        if(((strcmp(dirEntries->d_name, ".")==0)||(strcmp(dirEntries->d_name, "..")==0)))
            continue;
        if(newFrame->frameDefinations==NULL)
        {
            newFrame->frameDefinations=ad_deserializeImage(ad_getPathFromName(framePath, dirEntries->d_name));
        }
        else
        {
            Image *tmpPtr=newFrame->frameDefinations;
            if(tmpPtr!=NULL)
            {
                while(tmpPtr->nextImage!=NULL)
                {
                    tmpPtr= tmpPtr->nextImage;
                }
            }
            tmpPtr->nextImage=ad_deserializeImage(ad_getPathFromName(framePath, dirEntries->d_name));
        }
        
    }
    closedir(dirP);
    return newFrame;
}
void ad_loadAllFNamesInDictionary()
{
    mp_initializeIndex();
    DIR *dirP=opendir(rootpwd);
    struct dirent *dirEntries = NULL;
    char* fname="";
    unsigned int fid=0;
    Frame *tmpFrame=NULL;
    while((dirEntries=readdir(dirP))!=NULL)
    {
        if(dirEntries->d_type&DT_DIR)
        {
            if(!(strcmp(dirEntries->d_name, "."))||(!strcmp(dirEntries->d_name, "..")))
            {
                continue;
            }
            tmpFrame=ad_getFrameParametersFromDirectory(ad_getPathFromName(rootpwd, dirEntries->d_name));
            mp_addFrameToIndex(tmpFrame->frameName, tmpFrame->frameID);
        }
    }
}

#endif
