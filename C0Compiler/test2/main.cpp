#include "glob.h"
#include "err.h"
int mf;
ofstream laxrst, midcoderst, asmrst, symtablehis;
FILE* optcodefile;
int main(int argc, char **argv){
    char filename[500] = "in.c";
    printf("Input the source file:\n");
    char tempsrc[ 500 ];
    gets(tempsrc);
    if ( strlen(tempsrc) > 2 )
        if ( tempsrc[ 0 ] == '\"' ){
            strcpy(filename, tempsrc + 1 );
            filename[ strlen(filename) - 1 ] = 0;
        }else            
            strcpy(filename, tempsrc);
    src = fopen(filename, "r");
    if ( src == NULL )//没有读取到文件
    {
        error(NOSUCHFILE);
        system("pause");
        return 0;
    }
    laxrst.open("laxrst.txt");
    midcoderst.open("midcode.txt");
    asmrst.open("asmrst.asm");
    symtablehis.open("symbolTable.txt");
    symtablehis << "index" << "\t" << "name" << "\t" << "kind" << "\t" << "value" << "\t" << "address" << "\t" << "paranum" << "\t" << "isVec" << endl;
    getch();
    program();

    if ( mf < 1 ) error(MAINLACK);                         //没有定义主函数
    do  getsym(); while ( ch == '\n' || ch == ' ' || ch == '\t' );
    if ( symid != -1 )  error(MOREWORD);                //主函数后有多余的代码

    if ( errnum == 0 ) {
        printOptimize();     //打印优化后的四元式
        printf("四元式 生成完成...\n");
        midcode2asm();   //生成汇编码
        printf("汇编指令 生成完成...\n");
        scan();          //扫描四元式结构数组，完成优化
        printOptimize();
        printf("优化后的四元式 生成完成...\n");
    }

    laxrst.close();
    asmrst.close();
    midcoderst.close();
    symtablehis.close();
    fclose(src);
    
    printf("\n-----------------------------\n");
    if ( errnum == 0 )
    {
        printf("Compile Success.\n");
    }
    printf("Errors:\t%d\tTotal Line: %d\n", errnum, lnum);
    system("pause");
    return 0;
}