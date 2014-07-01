/*************************************************************************
	> File Name: bmp_to_c.cpp
	> Author: wgjak47
	> Mail: wgjak47@gmail.com 
	> Created Time: 2014年05月14日 星期三 21时06分15秒
 ************************************************************************/

#include<cstdio>
#include<cstring>
#include<cstdlib>
#define f_int(a) fread(&(a),sizeof(unsigned int),1,fp)
#define f_short(a) fread(&(a),sizeof(unsigned short),1,fp)
using namespace std;//
class bmp
{
    private:
        unsigned short indentifer;//1：图像文件头
        unsigned int file_size;//2-3 文件大小
        unsigned int data_offset;//6-7 文件开始到位数据的偏移量; 4-5 必须为零
        unsigned int header_size;//8-9 文件头
        unsigned int width;//10-11宽度
        unsigned int height;//12-13长度
        unsigned short planes;//14总为1
        unsigned short bpp;//15 图像位数 一般为24
        unsigned int compression;//16-17 压缩说明
        unsigned int data_size;//18-19 位图大小 4的倍数
        unsigned int hresolution;//20-21 水平分辨率
        unsigned int vresolution;//22-23 垂直分辨率
        unsigned int colors;//24-25颜色索引数
        unsigned int important_colors;//26-27 重要的颜色索引数目
    public:
    bmp(FILE *fp){
        int short_size=sizeof(unsigned short),int_size=sizeof(unsigned int);
        f_short(indentifer);
        f_int(file_size);
        fseek(fp,4,SEEK_CUR);//处理零
        f_int(data_offset);
        f_int(header_size);
        f_int(width);
        f_int(height);
        f_short(planes);
        f_short(bpp);
        f_int(compression);
        f_int(data_size);
        f_int(hresolution);
        f_int(vresolution);
        f_int(colors);
        f_int(important_colors);
    }
    void print_to_file(FILE *fp,FILE *to){
        unsigned char r,g,b,b1,b2;
        fseek(fp,data_offset,SEEK_SET);
        int reg = 0,padding = (width*3)%4;
        fprintf(to,"char s[]={\n");
        for (int i = 0; i < height; i++){
            for (int j = 0; j < width; j++){
                fread(&b,sizeof(unsigned char),1,fp);
                fread(&g,sizeof(unsigned char),1,fp);
                fread(&r,sizeof(unsigned char),1,fp);
                b1 = b>>3;
                b1 |= (g & 0x1C ) << 3;
                b2 = r & 0xF8;
                b2 |= g>>5;
                fprintf(fp, "0x%.2x, 0x%.2x, ", b1, b2);
                //fprintf(to,"0x%.2x, 0x%.2x, 0x%.2x, ",r,g,b);
                if ((++reg)%4 == 0) 
                    fprintf(to,"\n");
            }
            if (padding != 0)
            fseek(fp,padding,SEEK_CUR);
        }
        fprintf(to,"\n}");
    }
    ~bmp()
    {
    }
};//
bool check_read(FILE *fp,FILE *to)
{
    if (fp == NULL)
        printf("input FILE can't open!!\n");
    if (to == NULL)
        printf("output FILE can't open!!\n");
    return true;
}
int main(int argc,char *args[])
{
    FILE *fp,*to;
    const char *input_file = args[1],*output_file = args[2];
    if (argc < 2 ||!strcmp(args[1],"help"))
    {
        printf("This is bmp to 16bit C arrays problem.\nUsage: bmp_to_c [input_file] [output_file]\n");
        return 0;
    }
    fp = fopen("args[1]","rb+");
    to = fopen("args[2]","a+");
    if (check_read(fp,to)) return 1;
    bmp zzz(fp);
    zzz.print_to_file(fp,to);
    fclose(fp);
    fclose(to);
    return 0;   
}
