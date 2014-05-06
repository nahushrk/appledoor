//
//  main.c
//  appledoor
//
//  Created by Nahush Kulkarni on 2/23/14.
//  Copyright (c) 2014 Nahush Kulkarni. All rights reserved.
//

#ifndef include_mindPalace_h
#define include_mindPalace_h
#include "mindPalace.h"
#endif
#ifndef include_appleDoor_h
#define include_appleDoor_h
#include "appleDoor.h"
#endif

/*
static void startTimingMain();
static unsigned int clockTillNow();
static void initializeKB();
static void uninitializeKB();



char* ad_getPathFromName(char* path,char* name);
char* ad_getPathOfFName(char* fname);
char* ad_createFNameFromFrame(Frame *frame);
char* ad_createINameFromImage(Image* image);
unsigned int ad_frameIsInKB(char* frameName);
unsigned int ad_imageIsInFrame(Frame* frame,char* iname);
void ad_writeImageToLocation(Image *image,char* path);
void ad_writeNewFrame(Frame* newFrame);
unsigned int ad_addNewImageToFrame(Image* image,Frame *frame);
unsigned int ad_directoryIsNotEmpty(char* path);
Image* ad_getImageParametersFromDirectory(char* imagePath);
Image* ad_deserializeImage(char* imagePath);
Frame* ad_getFrameParametersFromDirectory(char* framePath);
Frame* ad_deSerializeFrame(char* framePath);
unsigned int ad_serializeImage(Image *img);
void ad_loadAllFNamesInDictionary();
*/


//--------------
void func(char path[])
{
    DIR *dirp;
    struct dirent *direntp;
    
    dirp=opendir(path);
    //chdir(path);
    
    while((direntp=readdir(dirp))!=NULL)
    {
        if(direntp->d_type&DT_DIR)//true than returns zero
        {
            if((strcmp(direntp->d_name,".")==0)||(strcmp(direntp->d_name,"..")==0))// if true retrns zero
            {
                continue;
            }
            printf("%s\n",direntp->d_name);
            char *a=appendString("/",direntp->d_name);
            func(appendString(path, a));
        }
        else
            if((strcmp(direntp->d_name,".")==0)||(strcmp(direntp->d_name,"..")==0))// if true retrns zero
            {
                continue;
            }
    }
}
//--------------

static void initializeKB()
{
    startTimingMain();
    kbCreationTime=kbClock;
    //load all frame names on disk to fidfnamedictionary
    ad_loadAllFNamesInDictionary();
    updateLastTimeStamp();
    
    //kb=(Frame*)malloc(sizeof(Frame));
    //kb->nextFrame=NULL;
}
static void uninitializeKB()
{
    printf("\nTotal Time : %dms\n",clockTillNow());
    free(kb);
    //free(kbParser);
    //free(knowledgeBase);
    //free(indexOfFIDFNames);
    //free(&kbCreationTime);
    //free(&timeValueStruct);
}



typedef struct attributes
{
    char symbol;
    unsigned int ID;
}XATTR;

XATTR getAttributesFromFName(char* fname)
{
    XATTR att;//=malloc(sizeof(XATTR));
    int i=0;
    while(fname[i]!='_')
    {
        i++;
    }
    att.symbol=fname[++i];
    char* IDstr=malloc((sizeof(fname)/sizeof(char))-i*sizeof(char));
    int j=0;
    i++;
    while(fname[i]!='\0')
    {
        IDstr[j]=fname[i];
        j++;i++;
    }
    att.ID=strToInt(IDstr);
    free(IDstr);
    return att;
}

char* trimmAttributes(char* iname)
{
    int i=0;
    while(iname[i]!='_')
    {
        i++;
    }
    char* fname=malloc(i*sizeof(char));
    i=0;
    while(iname[i]!='_')
    {
        fname[i]=iname[i];
        i++;
    }
    return fname;
}
char* insertSymlinkIdentifier(char* path)
{
    char *newPath=malloc(sizeof(char)*(sizeof(path)+1));
    newPath[0]=path[0];
    newPath[1]='.';
    int i=1;
    while(path[i]!='\0')
    {
        newPath[i+1]=path[i];
        i++;
    }
    return newPath;
}
#define BYTE sizeof(char)
char* getNewLastName(char* fullRelativePath)
{
    int i=0;
    int loc=0;
    while(fullRelativePath[i]!='\0')
    {
        if(fullRelativePath[i]=='/')
        {
            loc=i;
        }
        i++;
    }
    i=0;
    char *fname=malloc((sizeof(fullRelativePath)/BYTE)-i);
    while(fullRelativePath[loc]!='\0')
    {
        fname[i]=fullRelativePath[loc];
        loc++;i++;
    }
    return fname;
}

int main(int argc, const char * argv[])
{
    //initializeKB();
    //chdir(rootpwd);
    
    
    
    //XATTR x=getAttributesFromFName("/nahush_#123");
    //printf("%s",trimmAttributes("/nahush_#123"));
    //printf("%s",insertSymlinkIdentifier("/nahush"));
    printf("%s",getNewLastName("a/b/nahush"));
    
    
    
    
    
    
    //ad_writeNewFrame(mp_createFrameFromInputString("knowledgeBase[!_technology;#_information[!_structured[!_complex];!_nonStructured[!_complex];@_store;#_computer[@_usedBy];];]"));
    //ad_writeNewFrame(mp_createFrameFromInputString("nahush[!_human;]"));
    //printf("%s\n",substring("abcd", indexOfLastOccuranceOfCharacter('b', "abcd"),4));
    
    
    //mp_printAllFrameNames();
    
    //ad_writeImageToLocation(mp_createNewImageFromDefinitionString("#_information[!_structured[!_complex];!_nonStructured[!_complex];@_store;#_computer[@_usedBy];];"), rootpwd);
    //Frame* a=ad_deSerializeFrame("/Users/nahushrk/Desktop/kb/a,12");
    //printf("%d",ad_addNewFrameToKB(mp_createNewFrame("nahush")));
    //printf("%d",ad_frameIsInKB(mp_createNewFrame("nahush")));
    //printf("%d",ad_addNewImageToFrame(mp_createNewImage("!_awesome"),mp_createNewFrame("nahush")));
    //ad_getImageParametersFromDirectory("/Users/nahushrk/Desktop/kb/nahush/!_awesome,3");
    //Image* a=ad_deserializeImage("/Users/nahushrk/Desktop/kb/@_z,11");
    //printf("%d",ad_directoryHasDescription("/Users/nahushrk/Desktop/kb/b"));
    
    //mkdir("nahush",MKDIR_PERM);
    //func(buf);
    // insert code here...
    //uninitializeKB();
    return 0;
}

