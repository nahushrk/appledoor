//
//  mindPalace.h
//  appledoor
//
//  Created by Nahush Kulkarni on 4/18/14.
//  Copyright (c) 2014 Nahush Kulkarni. All rights reserved.
//

#ifndef include_time_h
#define include_time_h
#include <time.h>
#endif
#ifndef include_stdlib_h
#define include_stdlib_h
#include <stdlib.h>
#endif
#ifndef include_stdbool_h
#define include_stdbool_h
#include <stdbool.h>
#endif
#ifndef appledoor_utilities_h
#include "utilities.h"
#endif

#ifndef appledoor_mindPalace_h
#define appledoor_mindPalace_h

typedef struct
{
    unsigned int FID;
    char* FName;
}FIDFNameDictionary;

typedef struct image
{
    unsigned int imageID;
    char symbol;
    char *imageName;
    struct image *imageDefination;
    struct image *parentImage;
    struct image *nextImage;
}Image;
typedef struct frame
{
    unsigned int frameID;
    char *frameName;
    Image *frameDefinations;
    struct frame *nextFrame;
    struct frame *previousFrame;
}Frame;



//--------------------------------GLOBAL PROGRAM VARIABLES================================//
//for timing
static double kbClock;
static struct timeval timeValueStruct;

//================================GLOBAL PROGRAM VARIABLES--------------------------------//
//--------------------------------GLOBAL DATA================================//
Frame *kb = NULL;
Frame *kbParser = NULL;
static unsigned int lastTimeStamp;
static unsigned int kbCreationTime;
static FIDFNameDictionary indexOfFIDFNames[40];//FID,FName
unsigned int numberOfFramesInMemory=0;

//================================GLOBAL DATA--------------------------------//
//--------------------------------FUNCTION DECLARATIONS================================//
static void startTimingMain();
static unsigned int clockTillNow();
static void initializeKB();
static void uninitializeKB();
void ad_writeNewFrame(Frame* newFrame);
Frame* ad_deSerializeFrame(char* framePath);
char* ad_getPathFromName(char* path,char* name);
char* ad_createFNameFromFrame(Frame *frame);

//================================FUNCTION DECLARATIONS--------------------------------//


//--------------------------------SETTER & GETTER================================//
//FOR indexOfFIDFNames
unsigned int mp_addFrameToIndex(char* fname, int fid)//return 1 if success ELSE 0
{
    int i=0;
    //check if fname is already in index
    while(indexOfFIDFNames[i].FID!=0)
    {
        if(strcmp(indexOfFIDFNames[i].FName,fname)==0)
        {
            return 0;
        }
        i++;
    }
    indexOfFIDFNames[i].FName=fname;
    indexOfFIDFNames[i].FID=fid;
    numberOfFramesInMemory++;
    return 1;
}
void updateLastTimeStamp()
{
    int i=0;
    //check if fname is already in index
    while(indexOfFIDFNames[i].FID!=0)
    {
        if(indexOfFIDFNames[i].FID>lastTimeStamp)
        {
            lastTimeStamp=indexOfFIDFNames[i].FID;
        }
        i++;
    }
}
unsigned int mp_getFIDOfFName(char* fname)
{
    int i=0;
    while(indexOfFIDFNames[i].FID!=0)
    {
        if(strcmp(indexOfFIDFNames[i].FName,fname)==0)
        {
            return indexOfFIDFNames[i].FID;
        }
        i++;
    }
    return 0;
}
char* mp_getFnameOfFID(unsigned int Fid)
{
    int i=0;
    while(indexOfFIDFNames[i].FID!=0)
    {
        if(indexOfFIDFNames[i].FID==Fid)
        {
            return indexOfFIDFNames[i].FName;
        }
        i++;
    }
    return "";
}
unsigned int mp_frameIsInKB(char* fname)//return 1 if exists ELSE 0
{
    int i=0;
    while(indexOfFIDFNames[i].FID!=0)
    {
        if(strcmp(indexOfFIDFNames[i].FName,fname)==0)
        {
            return 1;
        }
        i++;
    }
    return 0;
}
//
//================================SETTER & GETTER--------------------------------//


//--------------------------------FUNCTIONS================================//
void mp_initializeIndex()
{
    //memset(indexOfFIDFNames, 0, sizeof(indexOfFIDFNames));
    int sizeOfArray=SIZE_OF_ARRAY(indexOfFIDFNames);
    for(int i=0;i<sizeOfArray;i++)
    {
        indexOfFIDFNames[i].FID=0;
    }
    numberOfFramesInMemory=0;
}
static void startTimingMain()
{
    gettimeofday(&timeValueStruct, NULL);
    kbClock =(timeValueStruct.tv_sec) * 1000 + (timeValueStruct.tv_usec) / 1000;
}
static unsigned int clockTillNow()
{
    static double timeTillNowInMillSec;
    gettimeofday(&timeValueStruct, NULL);
    timeTillNowInMillSec = (timeValueStruct.tv_sec) * 1000 + (timeValueStruct.tv_usec) / 1000;
    //printf("\nTime in MillSec:%f\n",timeTillNowInMillSec-mainProgramTimeInMill);
    return timeTillNowInMillSec-kbClock+lastTimeStamp;
}
void mp_kbParserNowPointsToLastFrame()
{
    kbParser=kb;
    while(kbParser->nextFrame != NULL)
    {
        kbParser=kbParser->nextFrame;
    }
}
unsigned int mp_addFrameToEndOfKB(Frame* frame)
{
    if(mp_frameIsInKB(frame->frameName)==FALSE)//fname not in kb
    {
        //create new frame from fname
        //Frame *newFrame = createNewFrame(fname);
        //if kb is empty then point kb to newFrame
        if(kb==NULL)
        {
            kb=frame;
        }
        //else then point kbparser to kb, parse kbparser to end of kb, add newFrame
        else if(mp_frameIsInKB(frame->frameName)==FALSE)
        {
            mp_kbParserNowPointsToLastFrame();
            frame->previousFrame=kbParser;
            kbParser->nextFrame=frame;
        }
        else//frame already is in KB
        {
            /*
             //goto frame in KB
             Frame* fparser=kb;
             while(fparser->frameName!=frame->frameName)
             {
             fparser=fparser->nextFrame;
             }
             Image* kbImgParser=fparser->frameDefinations;
             Image* newImgParser=frame->frameDefinations;
             while(newImgParser->nextImage!=NULL)
             {
             while(kbImgParser->nextImage!=NULL || kbImgParser->imageName==newImgParser->imageName)
             {
             
             }
             newImgParser=newImgParser->nextImage;
             }
             */
            //****************************************************************
            //merge new frame with existing frame definition
            //donot update anytimestamps
        }
        return frame->frameID;
    }
    return 0;//if frame already exists, return 0
}
Frame* mp_createNewFrame(char* fName)//creates frame obj,appends it to index, appends to KB
{
    Frame *newFrame = (Frame*)malloc(sizeof(Frame));
    if(mp_frameIsInKB(fName))
    {
        newFrame->frameName=fName;
        newFrame->frameID=mp_getFIDOfFName(fName);
        //getEntireFrameFromDisk
        ad_deSerializeFrame(ad_getPathFromName(rootpwd, ad_createFNameFromFrame(newFrame)));
        mp_addFrameToEndOfKB(newFrame);
    }
    else //Frame not in KB
    {
        usleep(1000);
        //write name and id to frame
        newFrame->frameName=fName;
        newFrame->frameID=clockTillNow();
        //set everything else to null
        newFrame->frameDefinations=NULL;
        newFrame->nextFrame=NULL;
        newFrame->previousFrame=NULL;
        //add fname and fid to index
        mp_addFrameToIndex(fName, newFrame->frameID);
        ad_writeNewFrame(newFrame);
        mp_addFrameToEndOfKB(newFrame);
    }
    return newFrame;
}
unsigned int mp_imageIsInFrame(Frame *tempFrame, char* iname)
{
    Image *imageParser = tempFrame->frameDefinations;
    if(imageParser==NULL)
        return 0;
    while(imageParser->nextImage !=NULL)
    {
        if(imageParser->imageName == iname)
        {
            return 1;
        }
        imageParser=imageParser->nextImage;
    }
    return 0;
}
unsigned int mp_imageIsInImmediateOfParent(char* imageName, Image *parentImage)
{
    if(parentImage->imageDefination==NULL)
        return 0;
    Image *iparser=parentImage->imageDefination;
    while(iparser->nextImage!=NULL || iparser->imageName==imageName)
    {
        if(iparser->imageName==imageName)
        {
            return 1;
        }
        iparser=iparser->nextImage;
    }
    return 0;
}
Image* mp_getImageInImageDefination(char* imageName, Image *parentImage)
{
    if(parentImage->imageDefination==NULL)
        return 0;
    Image *image = parentImage->imageDefination;
    while(image->nextImage!=NULL || image->imageName!=imageName)
    {
        if(image->imageName==imageName)
            return image;
        image=image->nextImage;
    }
    return image;
}
Image* mp_createNewImage(char* iname)
{
    mp_createNewFrame(substring(iname, 2, (int)strlen(iname)));
    Image *newImage = (Image*)malloc((sizeof(Image)));
    usleep(1000);
    newImage->imageID=clockTillNow();
    newImage->symbol=iname[0];
    newImage->imageName=malloc(sizeof(char)*strlen(iname)-2);
    memcpy(newImage->imageName, &iname[2], strlen(iname)-2);
    newImage->imageDefination=NULL;
    newImage->parentImage=NULL;
    newImage->nextImage=NULL;
    return newImage;
}
Image* mp_duplicateImage(Image* orgImage)
{
    Image *imageToBeAdded=(Image*)malloc(sizeof(Image));
    imageToBeAdded->imageID=orgImage->imageID;
    imageToBeAdded->imageName=orgImage->imageName;
    imageToBeAdded->symbol=orgImage->symbol;
    imageToBeAdded->parentImage=NULL;
    imageToBeAdded->imageDefination=NULL;
    while(orgImage->imageDefination!=NULL)
    {
        imageToBeAdded->imageDefination=mp_duplicateImage(orgImage->imageDefination);
        Image *tmp=imageToBeAdded->imageDefination;
        while(tmp->nextImage!=NULL)
        {
            tmp->parentImage=imageToBeAdded;
            tmp=tmp->nextImage;
        }
        tmp->parentImage=imageToBeAdded;
    }
    while(orgImage->nextImage!=NULL)
    {
        imageToBeAdded->nextImage=mp_duplicateImage(orgImage->nextImage);
    }
    return imageToBeAdded;
}
unsigned int mp_mergeImages(Image*newImage, Image *oldImage)
{
    if((oldImage->imageName!=newImage->imageName)&&(oldImage->symbol!=newImage->symbol))
    {
        return 0;
    }
    
    if(newImage->imageDefination==NULL && newImage->nextImage==NULL)
    {
        //if nothing to copy from, quit
        return 0;
    }
    Image *oldParser=oldImage;
    Image *newParser=newImage;
    while(newParser->imageDefination!=NULL)
    {
        newParser=newParser->imageDefination;
        if(mp_imageIsInImmediateOfParent(newParser->imageName,oldParser)!=0)
        {
            if(mp_mergeImages(newParser, mp_getImageInImageDefination(newParser->imageName,oldParser))==0)
                return 0;
        }
        else
        {
            if(oldParser->imageDefination == NULL)
            {
                Image *imageToBeAdded=mp_duplicateImage(newParser);
                oldParser->imageDefination=imageToBeAdded;
                imageToBeAdded->parentImage=oldParser;
            }
            else
            {
                Image *tempOldDefParser=oldParser->imageDefination;
                while(tempOldDefParser->nextImage!=NULL)
                {
                    tempOldDefParser=tempOldDefParser->nextImage;
                }
                Image *imageToBeAdded=mp_duplicateImage(newParser);
                tempOldDefParser->nextImage=imageToBeAdded;
                imageToBeAdded=tempOldDefParser->parentImage;
            }
            //else
            //if oldParser->desc !=null
            //add newImage to end of oldImage->desc->next
        }
    }
    oldParser=oldImage;
    newParser=newImage;
    while(newParser->nextImage!=NULL)
    {
        newParser=newParser->nextImage;
        if(mp_imageIsInImmediateOfParent(newParser->imageName, oldParser->parentImage)!=0)
        {
            if(mp_mergeImages(newParser, mp_getImageInImageDefination(newParser->imageName, oldParser->parentImage))==0)
                return 0;
        }
        else
        {
            while(oldParser->nextImage!=NULL)
            {
                oldParser=oldParser->nextImage;
            }
            Image *imageToBeAdded=mp_duplicateImage(newParser);
            oldParser->nextImage=imageToBeAdded;
            imageToBeAdded->parentImage=oldParser->parentImage;
        }
    }
    return 1;
}
unsigned int mp_addImageToFrame(Image *image,Frame *frame)
{
    if(frame->frameDefinations==NULL)
    {
        frame->frameDefinations=image;
    }
    else//frame->frameDefinations!=NULL
    {
        if(mp_imageIsInFrame(frame, image->imageName)==0)//image is not in frame
        {
            Image *imageParser=frame->frameDefinations;
            while(imageParser->nextImage!=NULL)
            {
                imageParser=imageParser->nextImage;
            }
            imageParser->nextImage=image;
            image->parentImage=imageParser->parentImage;
        }
    }
    if(mp_imageIsInFrame(frame, image->imageName)!=0)
        return 1;
    else
        return 0;
}
unsigned int mp_addImageNameToFrameName(char* imageName/*(<symbol>_<imageName>)*/, char* frameName)
{
    Image *newImage = mp_createNewImage(imageName);
    //if fname DOES NOT exist then create new frame for fname, goto frame
    if(mp_frameIsInKB(frameName)==0)
    {
        mp_addFrameToEndOfKB(mp_createNewFrame(frameName));
        //kbParserNowPointsToLastFrame();
    }
    kbParser=kb;
    while(kbParser->nextFrame != NULL || kbParser->frameName==frameName)
    {
        if(kbParser->frameName==frameName)
            break;
        kbParser=kbParser->nextFrame;
    }
    //if image exists in frame then ignore
    if(mp_imageIsInFrame(kbParser, newImage->imageName) == 1)
    {
        
    }
    //if image does not exist in fname then create image for iname, add image to end of images of fname
    else
    {
        if(kbParser->frameDefinations!=NULL)
        {
            while(kbParser->frameDefinations->nextImage != NULL)
            {
                kbParser->frameDefinations=kbParser->frameDefinations->nextImage;
            }
            kbParser->frameDefinations->nextImage=newImage;
            newImage->nextImage=NULL;
        }
        else
        {
            newImage->nextImage=NULL;
            kbParser->frameDefinations=newImage;
        }
    }
    return 0;
}
unsigned int mp_indexOfFirstOccurenceOfCharInString(char c,char* string)
{
    return (unsigned int)(strchr(string, c)-string)+1;
}
char* mp_getFirstImageStringFromString(char* string)
{
    if(mp_indexOfFirstOccurenceOfCharInString(';', string)>mp_indexOfFirstOccurenceOfCharInString('[', string))
    {
        int balanceBracketCounter=0;
        int parser=0;
        while(parser<strlen(string))
        {
            if(string[parser]=='[')
            {
                balanceBracketCounter++;
            }
            if(string[parser]==']')
            {
                balanceBracketCounter--;
                if(balanceBracketCounter==0)
                {
                    parser+=2;
                    break;
                }
            }
            parser++;
        }
        return substring(string, 0, parser);
    }
    else//; comes before [
    {
        return substring(string, 0, mp_indexOfFirstOccurenceOfCharInString(';', string));
    }
}
unsigned int mp_verifyInputString(char* string)
{
    int parser=0;
    int balanceBracketChecker=0;
    while(string[parser]!='\0')
    {
        if(string[parser]=='[')
            balanceBracketChecker++;
        if(string[parser]==']')
            balanceBracketChecker--;
        if(balanceBracketChecker<0)
        {
            printf("\nINPUT BRACKETS ARE NOT BALANCED.\n");
            return 0;
        }
        parser++;
    }
    if(balanceBracketChecker!=0)
    {
        printf("\nINPUT BRACKETS ARE NOT BALANCED.\n");
        return 0;
    }
    return 1;
}
Image* mp_createImageFromDefinitionString(char* imgString)
{
    Image* newImage=NULL;
    if(mp_indexOfFirstOccurenceOfCharInString(';',imgString)>mp_indexOfFirstOccurenceOfCharInString('[',imgString))//image contains description
    {
        newImage=mp_createNewImage(substring(imgString, 0, mp_indexOfFirstOccurenceOfCharInString('[',imgString)-1));
        imgString=substring(imgString, (int)strlen(newImage->imageName)+3, (int)strlen(imgString)-2);
        while(strcmp(imgString, "")!=FALSE)
        {
            if(newImage->imageDefination==NULL)
            {
                newImage->imageDefination=mp_createImageFromDefinitionString(mp_getFirstImageStringFromString(imgString));
                newImage->imageDefination->parentImage=newImage;
                imgString=substring(imgString, (int)strlen(mp_getFirstImageStringFromString(imgString)), (int)strlen(imgString));
            }
            else
            {
                Image* parser=newImage->imageDefination;
                while(parser->nextImage!=NULL)
                {
                    parser=parser->nextImage;
                }
                parser->nextImage=mp_createImageFromDefinitionString(mp_getFirstImageStringFromString(imgString));
                parser->nextImage->parentImage=parser->parentImage;
                imgString=substring(imgString, (int)strlen(mp_getFirstImageStringFromString(imgString)), (int)strlen(imgString));
            }
        }
        //newImage->imageDefination=createNewImageFromDefinitionString();
        //newImage->imageDefination->parentImage=newImage;
    }
    else//image has next image or single image
    {
        newImage=mp_createNewImage(substring(imgString, 0, mp_indexOfFirstOccurenceOfCharInString(';',imgString)-1));//could be strlen(imgString)
    }
    return newImage;
}
Frame* mp_createFrameFromInputString(char* string)
{
    //check if input statement is balanced
    mp_verifyInputString(string);
    Frame* newFrame=mp_createNewFrame(substring(string, 0, mp_indexOfFirstOccurenceOfCharInString('[',string)-1));
    string=substring(string, mp_indexOfFirstOccurenceOfCharInString('[',string),(unsigned int)strlen(string)-1);
    int imageEndIndex=0;
    char* imageName;
    while(strcmp(string, "")!=0)
    {
        //if(image is just a single expression of form symbol_imageName, then add image directly to frame)
        imageEndIndex=(int)strlen(mp_getFirstImageStringFromString(string));//(int)(strchr(string, ';')-string);
        if(mp_indexOfFirstOccurenceOfCharInString(';',string)<mp_indexOfFirstOccurenceOfCharInString('[',string))
        {
            imageName=substring(string, 0, imageEndIndex-1);
            mp_addImageToFrame(mp_createNewImage(imageName), newFrame);
            string=substring(string, imageEndIndex,(unsigned int)strlen(string));
        }
        else//if(image has description of its own, then create image object from sting)
        {
            mp_addImageToFrame(mp_createImageFromDefinitionString(string), newFrame);
            string=substring(string, imageEndIndex,(unsigned int)strlen(string));
        }
    }
    return newFrame;
}
char* mp_imagesToString(Image* image)
{
    char *string = "";
    string=appendString(&image->symbol,string);
    string=appendString("_",string);
    string=appendString(image->imageName,string);
    if(image->imageDefination!=NULL)
    {
        string=appendString("[",string);
        string=appendString(mp_imagesToString(image->imageDefination),string);
        string=appendString("]",string);
        //string=appendString(";",string);
        //image=image->imageDefination;
    }
    string=appendString(";",string);
    if(image->nextImage!=NULL)
    {
        
        string=appendString(mp_imagesToString(image->nextImage),string);
        //string=appendString(";",string);
        //image=image->nextImage;
    }
    return string;
}
char* mp_frameToString(Frame* frame)
{
    char* displayString=frame->frameName;
    Image *imgParser=frame->frameDefinations;
    displayString=appendString("[", displayString);
    displayString=appendString(mp_imagesToString(imgParser), displayString);
    displayString=appendString("]", displayString);
    return displayString;
}
void mp_addImageToDescriptionOfParent(Image *newImage,Image *parentImage)
{
    if(parentImage->imageDefination==NULL)
    {
        parentImage->imageDefination=newImage;
        newImage->parentImage=parentImage;
        
    }
    else
    {
        Image *imageParser=parentImage->imageDefination;
        while(imageParser->nextImage!=NULL)
        {
            imageParser=imageParser->nextImage;
        }
        imageParser->nextImage=newImage;
        newImage->parentImage=imageParser->parentImage;
    }
    
}
//--------------PRINT FUNCTIONS
void mp_printAllFrameNames()
{
    printf("printing all FIDs & FNames...\n");
    for(int i=0;i<numberOfFramesInMemory;++i)
    {
        printf("%d\t%s\n",indexOfFIDFNames[i].FID,indexOfFIDFNames[i].FName);
    }
    printf("\n");
}
void mp_printAllImages(Image *imageParser,int index)
{
    while(imageParser->nextImage!=NULL)
    {
        for(int i=0;i<index;i++)
        {
            printf("\t");
        }
        printf("%d,%c_%s\n",imageParser->imageID,imageParser->symbol,imageParser->imageName);
        if(imageParser->imageDefination!=NULL)
        {
            mp_printAllImages(imageParser->imageDefination,index+1);
        }
        imageParser=imageParser->nextImage;
    }
    for(int i=0;i<index;i++)
    {
        printf("\t");
    }
    printf("%d,%c_%s\n",imageParser->imageID,imageParser->symbol,imageParser->imageName);
    if(imageParser->imageDefination!=NULL)
    {
        mp_printAllImages(imageParser->imageDefination,index+1);
    }
}
void mp_printAllFramesAndTheirImages()
{
    kbParser=kb;
    while(kbParser->nextFrame!=NULL)
    {
        printf("%d,%s\n",kbParser->frameID,kbParser->frameName);
        if(kbParser->frameDefinations!=NULL)
        {
            mp_printAllImages(kbParser->frameDefinations,1);
        }
        kbParser=kbParser->nextFrame;
    }
    printf("%d,%s\n",kbParser->frameID,kbParser->frameName);
}

#endif


/*--------------DISCARDED FUNCTIONS
 unsigned int mp_frameIsInKB(char* fname)
 {
 if(kb==NULL)
 return 0;
 kbParser=kb;
 while(kbParser->nextFrame !=NULL || kbParser->frameName == fname)
 {
 if(kbParser->frameName == fname)
 {
 return 1;
 }
 kbParser=kbParser->nextFrame;
 }
 return 0;
 }


*/