#ifndef exc0compiler_glob_h
#define exc0compiler_glob_h
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <map>
#include <ctype.h>

using namespace std;

///======================================lax

extern const int kwdnum;
extern char *word[];

extern char *wsym[];

#define BEGINTK     1
#define CALLTK      2
#define CASETK      3
#define CHARTK      4
#define CONSTTK     5
#define DEFAULTTK   6
#define DOTK        7
#define ELSETK      8
#define ENDTK       9
#define FLOATTK     10
#define FORTK       11
#define IFTK        12
#define INTTK       13
#define MAINTK      14
#define ODDTK       15
#define PRINTFTK    16
#define PROCETK     17
#define READTK      18
#define RETURNTK    19
#define RPTTK       20
#define SCANFTK     21
#define SWITCHTK    22
#define THENTK      23
#define UTLTK       24
#define VARTK       25
#define VOIDTK      26
#define WHILETK     27
#define WRITETK     28
#define IDEN        29
#define INTCON      30
#define CHARCON     31
#define STRCON      32
#define PLUS        33
#define MINU        34
#define MULT        35
#define PERIOD      36
#define DIV         37
#define COLON       38
#define LSS         39
#define QMARK       40
#define LEQ         41
#define DQMARK      42
#define GRE         43
#define LPARENT     44
#define GEQ         45
#define RPARENT     46
#define EQL         47
#define LBRACK      48
#define NEQ         49
#define RBRACK      50
#define ASSIGN      51
#define LBRACE      52
#define SEMICN      53
#define RBRACE      54
#define COMMA       55

#define CINTCON     56
#define CCHARCON    57

extern ofstream laxrst, midcoderst, asmrst, symtablehis;
extern int ch;//the

extern int line[ 500 ], backupline[ 500 ];
extern int lnum , cnum , llen, prllen;
extern FILE* src;

//define id, num and sym
extern string id, prid;
extern int num;
extern string sym;
extern double dnum;
extern int symid;

//declare functioint getsym();
void getch();
int getsym();
bool stringcmp(char* a, char* b);


///======================================syn
void program();
void defhead();
void varstate();
void vardef();
void constdef(int tclass);
void conststate();
void sentencelist();
void complexsentence();
void sentence();
void condition();
void loopsentence();
void valueofpara();
void scanfsentence();
void printfsentence();
void parametertable();
void returnsentence();
void expression();
void item();
void factor();
void lower(char* s);
//the word tobe dealwith
extern int wd;

#define MAXIDENLEN      30
#define MAXSYMTABLENUM  200

//define symbol table
typedef struct{
    char name[ MAXIDENLEN ];  //identifier name
    int kind;               //identifier kind (define as follow)
    int value;              //1���ں�����˵����ʾ����ֵΪInt 0����ֵΪvoid
    int address;            //address
    int paranum;            //
}symb;
#define CONST       0   //����
#define VARIABLE    1   //����
#define FUNCTION    2   //����
#define PARA        3   //����
/****************************
*                  symbol table
*  name    kind    value   address     paranum
*
*
*
*
*
***************************/
typedef struct {
    symb element[ MAXSYMTABLENUM ];
    int index;
    int ftotal;                 //�ֳ�������
    int findextable[ MAXSYMTABLENUM ];//�ֳ�����������
}symtable;
extern symtable maintable;
extern int  value;     //���泣����ֵ�������ӳ���0��ʾvoid��1��ʾint
extern int  address;   //�����ĵ�ַ����������ֵ�ĵ�ַ
extern int  paranum;   //ȫ�ֱ������������������壬��Ϊ�����Ĳ�������ֻ���ڲ������ӳ���ִ�н���֮���֪��������ֻ���ں���Ų�����ű���Ϣ
extern int  kind;      //���ڴ���ı�־��������
extern char name[ MAXIDENLEN ]; //��־�������֣��������������ͺ���
extern int hsymid; //��һ����������
extern char nowitem[ 200 ];
extern int returnnum; //��¼�Ƿ��з���ֵ
extern char procname[ 100 ];
extern int mf; //����������


///=====================================��Ԫʽ
void insmidcode(char* op, char* t1, char* t2, char* t3);
void insmidcode(char* op, int t1, int t2, char* t3);
void insmidcode(char* op, char t1, int t2, char* t3);
char* nextlab();
char* nextvar();
#define MAXMIDCODE 1000
/****************************
* op:      symbol of operator
* var1:    op1
* var2:    op2
* var3:    distination var
* e.g.
*      =   , 2 ,   , temp  ==> temp = 2;
*       []= , a , i , t       ==> a[i] = t;
*      int ,   ,   , a     ==> int  a;
*      const,int,5 , a     ==> const int a = 5;
*      char,   , 30, a     ==> char a[30];
*      fupa,   ,   , a     ==> a is a function parameter
*      call, f ,   , a     ==> a = f()
*      call, f ,   ,       ==> f()
*      <=.., a , b ,       ==> a <= b
*      jne ,   ,   , lable ==> if not satisfy(==false) then jump
*      jmp ,   ,   , label ==> jump to label
*      lab:,   ,   , labx  ==> set label
*      geta, a , n , b     ==> b = a[n]
*      ret ,   ,   , (a)   ==> return a / return
*      prt , a , b , symb  ==> print("a", b)
*      scf ,   ,   , a     ==> scanf(a)
*      func,int,   , f     ==> start of function int f()
*      para,int,   , a     ==> f(int a, ...)
*      end ,   ,   , f     ==> end of function f
****************************/
typedef struct{
    char op[ 10 ];
    char var1[ 200 ];
    char var2[ 30 ];
    char var3[ 30 ];
}fourvarcode;
extern fourvarcode midcode[ MAXMIDCODE ];
extern int midcodeiter;
extern int labelcnt;
extern int varcnt;

///=====================================�Ż�
extern FILE* optcodefile;
void printOptimize();
int isconst(char name[]);
void delsetlab();
void delpublic();
void combine();
void scan();


///=====================================���
extern int mi;
extern int sp;
extern int x;
void midcode2asm();
int findvartable(char *name);
void midcode2asm();
void insertaddress(int kind, int addr = -1, int nmi = -1);
void pushstack(char* item = "0", int lenth = 1);
void funcasm();
int varaddr(char *name);
void dataseg();
void jmpasm();
void printint();
void callasm();
void setlabasm();
void addasm();
void subasm();
void mulasm();
void divasm();
void greasm();
void geqasm();
void lssasm();
void leqasm();
void eqlasm();
void neqasm();
void assasm();
void aassasm();
void assaasm();
void scfasm();
void prtasm();
void fupaasm();
void retasm();
void paraasm();
void jneasm();
void intcharasm();
void constdefasm();
void intcharaasm();
void savesreg();
void loadsreg();
void cnt(char * name);
void cntopt();
#endif