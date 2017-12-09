#include <string.h>
#include "glob.h"

int codeindex = 0;


//ɨ�������Ԫʽ���飬�������еĻ�����
void scan()
{
    //�ҵ�������ʼ
    while ( strcmp(midcode[ codeindex ].op, "func") != 0 ) codeindex++;

    while ( codeindex < midcodeiter ) {
        combine();
        delpublic();
        codeindex++;
    }
    delsetlab();
    return;
}

//�ϲ�����
void combine() {
    int i = codeindex, num1, num2, temp;
    char sum[ 30 ];
    while ( strcmp(midcode[ i ].op, "+") == 0 || strcmp(midcode[ i ].op, "-") == 0 || strcmp(midcode[ i ].op, "*") == 0 || strcmp(midcode[ i ].op, "/") == 0 ) {
        if ( isconst(midcode[ i ].var1) && isconst(midcode[ i ].var2) ) {
            num1 = atoi(midcode[ i ].var1);
            num2 = atoi(midcode[ i ].var2);
            if ( strcmp(midcode[ i ].op, "+") == 0 ) temp = num1 + num2;
            if ( strcmp(midcode[ i ].op, "-") == 0 ) temp = num1 - num2;
            if ( strcmp(midcode[ i ].op, "*") == 0 ) temp = num1*num2;
            if ( strcmp(midcode[ i ].op, "/") == 0 ) temp = num1 / num2;
            sprintf(sum, "%d", temp);
            strcpy(midcode[ i ].op, "=");
            strcpy(midcode[ i ].var1, sum);
            strcpy(midcode[ i ].var2, "  ");
        }
        i++;
    }
}

//ɾ�������ӱ��ʽ
void delpublic()
{
    int i, j, h, k;
    //���ֻ�����
    for ( i = codeindex; strcmp(midcode[ i ].op, "+") == 0 || strcmp(midcode[ i ].op, "-") == 0 || strcmp(midcode[ i ].op, "*") == 0 || strcmp(midcode[ i ].op, "/") == 0 || strcmp(midcode[ i ].op, "=") == 0; i++ ) {
        if ( i >= midcodeiter ){
            return;
        }
        if ( midcode[ i ].var3[ 0 ] == '$' ) {
            for ( j = i + 1; strcmp(midcode[ j ].op, "+") == 0 || strcmp(midcode[ j ].op, "-") == 0 || strcmp(midcode[ j ].op, "*") == 0 || strcmp(midcode[ j ].op, "/") == 0 || strcmp(midcode[ j ].op, "=") == 0; j++ ) {
                if ( j >= midcodeiter ){
                    return;
                }
                //Ѱ�ҹ����ӱ��ʽ
                if ( strcmp(midcode[ j ].op, midcode[ i ].op) == 0 && strcmp(midcode[ j ].var1, midcode[ i ].var1) == 0 && strcmp(midcode[ j ].var2, midcode[ i ].var2) == 0 && midcode[ j ].var3[ 0 ] == '$' ) {
                    //�޸ı���������
                    for ( h = j + 1; strcmp(midcode[ h ].op, "+") == 0 || strcmp(midcode[ h ].op, "-") == 0 || strcmp(midcode[ h ].op, "*") == 0 || strcmp(midcode[ h ].op, "/") == 0 || strcmp(midcode[ h ].op, "=") == 0; h++ ) {
                        if ( h >= midcodeiter ){
                            return;
                        }
                        if ( strcmp(midcode[ h ].var1, midcode[ j ].var3) == 0 )
                            strcpy(midcode[ h ].var1, midcode[ i ].var3);
                        if ( strcmp(midcode[ h ].var2, midcode[ j ].var3) == 0 )
                            strcpy(midcode[ h ].var2, midcode[ i ].var3);
                    }
                    for ( k = j; k< midcodeiter; k++ ) {
                        strcpy(midcode[ k ].op, midcode[ k + 1 ].op);     //ɾ��
                        strcpy(midcode[ k ].var1, midcode[ k + 1 ].var1);
                        strcpy(midcode[ k ].var2, midcode[ k + 1 ].var2);
                        strcpy(midcode[ k ].var3, midcode[ k + 1 ].var3);
                    }
                    midcodeiter--;
                    j--;
                }
            }
        }
    }
}

//ɾ��������ת����
void delsetlab()
{
    int i, k, j, t, flag;
    char temp[ 30 ][ 10 ];
    for ( i = 0; i < midcodeiter; i++ ) {
        if ( i >= midcodeiter ){
            return;
        }
        if ( strcmp(midcode[ i ].op, "lab:") == 0 ) {
            j = 0;
            flag = i;
            i = i + 1;
            while ( strcmp(midcode[ i ].op, "lab:") == 0 ) {
                strcpy(temp[ j++ ], midcode[ i ].var3);
                for ( k = i; k<midcodeiter; k++ ) {
                    strcpy(midcode[ k ].op, midcode[ k + 1 ].op);     //ɾ��
                    strcpy(midcode[ k ].var1, midcode[ k + 1 ].var1);
                    strcpy(midcode[ k ].var2, midcode[ k + 1 ].var2);
                    strcpy(midcode[ k ].var3, midcode[ k + 1 ].var3);
                }
                midcodeiter--;
            }
            if ( j == 0 ) continue;
            for ( k = 0; k <= midcodeiter; k++ ) {
                if ( k >= midcodeiter ){
                    return;
                }
                if ( strcmp(midcode[ k ].op, "jmp") == 0 || strcmp(midcode[ k ].op, "jne") == 0 ) {
                    for ( t = 0; t<j; t++ ) {
                        if ( strcmp(midcode[ k ].var3, temp[ t ]) == 0 )
                            strcpy(midcode[ k ].var3, midcode[ flag ].var3);
                    }
                }
            }
        }
    }
}

//�ж��ǲ�������
int isconst(char name[])
{
    int i = 0;
    if ( name[ i ] == '-' ) i++;
    while ( name[ i ] != '\0' ) {
        if ( name[ i ]>'9' || name[ i ]<'0' ) return 0;
        i++;
    }
    return 1;
}

//��ӡ�Ż������Ԫʽ
void printOptimize()
{
    optcodefile = fopen("optMidCode.txt", "w");
    int i = 0;
    while ( i<midcodeiter ) {
        fprintf(optcodefile, "%s,\t", midcode[ i ].op);
        fprintf(optcodefile, "%s,\t", midcode[ i ].var1);
        fprintf(optcodefile, "%s,\t", midcode[ i ].var2);
        fprintf(optcodefile, "%s;\n", midcode[ i ].var3);
        if ( strcmp(midcode[ i ].op, "end") == 0 )
            fprintf(optcodefile, "\n\n");
        i++;
    }
    fclose(optcodefile);
    return;
}