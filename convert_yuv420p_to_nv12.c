#
/*
*CONVERTS YUV420p color format to nv12 (raster form)
*Takes .yuv input and gives .yuv file
*Command format ./Convert_yuv420p_to_nv12 <Width> <Height> <input yuv path> <output yuv path>
Example command ./Convert_yuv420p_to_nv12 1280 720 ../videos/bbb_sunflower_720p.yuv ../videos/output/bbb_sunflower_720p_nv12.yuv
*
*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARG_EXE 0
#define ARG_WIDTH 1
#define ARG_HEIGHT 2
#define ARG_INFILEPATH 3
#define ARG_OUTFILEPATH 4

int precheck(int argc , char *argv[]);

int main(int argc , char *argv[])
{

    int precheck_notok_flag=0;
    char *mem_framebuf=NULL;
    int frame_size,Inheight,Inwidth,temp;
    int y_plane_size,u_start_position,v_start_position;
    FILE *In_fp=NULL, *Out_fp=NULL;
    int x,y;

    //precheck_notok_flag=(int)precheck(argc , argv);
    precheck_notok_flag=precheck(argc , argv);
    
    if(precheck_notok_flag){
    printf("\n Exiting...\n");
    return 0;
    }
    printf("\nPrecheck successful");

    Inheight=atoi(argv[ARG_HEIGHT]);
    Inwidth=atoi(argv[ARG_WIDTH]);
    y_plane_size=Inheight*Inwidth;
    u_start_position = y_plane_size;
    v_start_position = u_start_position + (y_plane_size/4);

    frame_size = y_plane_size + y_plane_size/2; //y Plane size + UV plane size
    mem_framebuf = calloc(sizeof(char),frame_size);
    if(!mem_framebuf)
    {
        printf("Failed to allocate frame buffer\n");
        return 0;
    }
    In_fp=fopen(argv[ARG_INFILEPATH],"r");
    {
        if(In_fp == NULL)
        {
            printf("Cannot open input file for reading\n");
            return 0;
        }
    }
        Out_fp=fopen(argv[ARG_OUTFILEPATH],"w");
    {
        if(Out_fp == NULL)
        {
            printf("Cannot open output file for writing\n");
            return 0;
        }
    }
    
    temp=fread(mem_framebuf,sizeof(char),frame_size,In_fp);
    if(temp < frame_size)
    {
        printf("\nError : Cannot read information for 0th frame");
        return 0;
    }
    while(temp == frame_size)
    {
        //Y channel
        fwrite(mem_framebuf,1,Inheight*Inwidth,Out_fp);

        for(y =0 ;y < Inheight/2 ; y++)
        {
            for(x = 0 ; x< Inwidth/2 ; x++)
            {
                //u channel
                fwrite(&(mem_framebuf[u_start_position+(y*(Inwidth/2))+x]),1,1,Out_fp);
                //v channel
                fwrite(&(mem_framebuf[v_start_position+(y*(Inwidth/2))+x]),1,1,Out_fp);
            }
        }     
       temp = fread(mem_framebuf,sizeof(char),frame_size,In_fp);
    }

    printf("\nconversion complete from yuv420p to nv12\n");
    

    free(mem_framebuf);
    fclose(In_fp);
    fclose(Out_fp);
    return 0;
}//End of main

int precheck(int argc , char *argv[]){
    
int InWidth=0, InHeight=0, OutHeight =0, OutWidth =0;
char InFilePath[256],OutFilePath[256];  
FILE *In_fp = NULL,*Out_fp=NULL;  
char check_flag=0;
int temp1,temp2;

//Format
if(argc!=(ARG_OUTFILEPATH+1))
{
    printf("\n ERROR : Format is not as expected.\n");
    printf("Please give command in format ./convert_yuv420_to_nv12 <input width> <input height> <In File Path> <Out File Path>\n");
    return 1;
}
//File paths
strcpy(InFilePath, argv[ARG_INFILEPATH]);
strcpy(OutFilePath, argv[ARG_OUTFILEPATH]);
In_fp = fopen(InFilePath,"r");
if(In_fp == NULL){
    printf("\nERROR : Input file '%s' doesnot exist.\n",InFilePath);
    return 1;
    }
Out_fp = fopen(OutFilePath,"a");

if(Out_fp == NULL){
    printf("\n ERROR : Cannot create output file '%s'n",OutFilePath);
    return 1;
}    
//Resolutions(Input)
if (atoi(argv[ARG_HEIGHT])==0 ||  atoi(argv[ARG_WIDTH])==0 ) {
    printf("\nERROR : The resolution parameters cannot be 0. Please give a valid number\n");
   return 1;
 } 
 fclose(In_fp);
fclose(Out_fp);

 return 0;  
}

