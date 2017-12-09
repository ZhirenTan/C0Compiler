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
    if ( src == NULL )//û�ж�ȡ���ļ�
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

    if ( mf < 1 ) error(MAINLACK);                         //û�ж���������
    do  getsym(); while ( ch == '\n' || ch == ' ' || ch == '\t' );
    if ( symid != -1 )  error(MOREWORD);                //���������ж���Ĵ���

    if ( errnum == 0 ) {
        printOptimize();     //��ӡ�Ż������Ԫʽ
        printf("��Ԫʽ �������...\n");
        midcode2asm();   //���ɻ����
        printf("���ָ�� �������...\n");
        scan();          //ɨ����Ԫʽ�ṹ���飬����Ż�
        printOptimize();
        printf("�Ż������Ԫʽ �������...\n");
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