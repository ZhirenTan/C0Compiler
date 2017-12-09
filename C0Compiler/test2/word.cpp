#include "glob.h"
#include "err.h"

using namespace std;

int prllen;
const int kwdnum = 28;
char *word[] = {
    "begin", "$call", "case", "char", "const",//0~4
    "default", "do", "else", "$end", "float",  //5~9
    "for", "if", "int", "main", "$odd",//10~14
    "printf", "$procedure", "$read", "return", "$repeat",  //15~19
    "scanf", "switch", "$then", "until", "$var",//20~24
    "void", "while", "$write"//25~27
};
char *wsym[] = {
    "BEGINTK", "CALLTK", "CASETK", "CHARTK", "CONSTTK",
    "DEFAULTTK", "DOTK", "ELSETK", "ENDTK", "FLOATTK",
    "FORTK", "IFTK", "INTTK", "MAINTK", "ODDTK",
    "PRINTFTK", "PROCETK", "READTK", "RETURNTK", "RPTTK",
    "SCANFTK", "SWITCHTK", "THENTK", "UTLTK", "VARTK",
    "VOIDTK", "WHILETK", "WRITETK"
};

int ch = ' ';//the

int line[ 500 ], backupline[ 500 ];
int lnum = 0, cnum = 0, llen = 0;
FILE* src = NULL;

//define id, num and sym
string id = "";
int num;
string sym;
double dnum;
int symid;
string prid;


int getsym(){
    prid = id;
    id = "";
    while ( isspace(ch) && ch != EOF ) {
        getch();
    }
    if ( ch == EOF ){
        symid = -1;
        return -1;
    }
    //when the ch is alpha
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //!!!   this version donnot has lenth limit
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    if ( isalpha(ch) || ch == '_' ){
        string tempstr;
        while ( isalpha(ch) || isdigit(ch) || ch == '_' ) {
            ch = tolower(ch);
            tempstr += ch;
            getch();
        }
        id = tempstr;
        //seatch reserved word table
        int iter = 0;
        for ( iter = 0; iter < kwdnum; iter++ ) {
            if ( stringcmp(word[ iter ], (char*)tempstr.c_str()) ) {
                sym = wsym[ iter ];
                symid = iter + 1;
                break;
            }
        }
        if ( iter == kwdnum ) {
            sym = "IDEN";
            symid = IDEN;
        }
    }
    else if ( isdigit(ch) ){
        
        sym = "INTCON";
        symid = INTCON;
        num = 0;
        while ( isdigit(ch) ){
            id += ch;
            num = num * 10 + (int)( ch - '0' );
            getch();
        }
        if ( ch == '.' ) {
            id += ch;
            sym = "FLOATCON";
            symid = -1;
            double t = 0.1;
            dnum = num;
            getch();
            while ( isdigit(ch) ){
                id += ch;
                dnum = dnum + (int)( ch - '0' ) * t;
                t /= 10;
                getch();
            }
        }
    }
    else if ( ch == '=' ){
        id += ch;
        sym = "ASSIGN";
        symid = ASSIGN;
        getch();
        if ( ch == '=' ) {
            id += ch;
            sym = "EQL";
            symid = EQL;
            getch();
        }
    }
    else if ( ch == '<' ){
        id += ch;
        getch();
        if ( ch == '=' ) {
            id += ch;
            sym = "LEQ";
            symid = LEQ;
            getch();
        }
        else{
            sym = "LSS";
            symid = LSS;
        }
    }
    else if ( ch == '>' ){
        id += ch;
        getch();
        if ( ch == '=' ) {
            id += ch;
            sym = "GEQ";
            symid = GEQ;
            getch();
        }
        else{
            sym = "GRE";
            symid = GRE;
        }
    }
    else if ( ch == '!' ){
        id += ch;
        getch();
        if ( ch == '=' ) {
            id += ch;
            sym = "NEQ";
            symid = NEQ;
            getch();
        }
    }
    else if ( ch == '\"' ){
        string tempstr;
        getch();
        while ( ch == 32 || ch == 33 || (ch <= 126 && ch >= 35) ) {
            tempstr += ch;
            getch();
        }
        if ( ch == '\"' ) {
            getch();
            sym = "STRCON";
            symid = STRCON;
            id = tempstr;
        }
        else{
            error(DOUBLEQUOTESLACK);
            return -1;
        }
    }
    else if ( ch == '\'' ){
        getch();
        if ( ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '_' || isalnum(ch) ) {
            id += ch;
            getch();
        }
        else{
            error(UNACCEPTATLECHAR);
            return -1;
        }
        if ( ch == '\'' ){
            sym = "CHARCON";
            symid = CHARCON;
            getch();
        }
        else{
            error(SINGLEQUOTESLACK);
            return -1;
        }
    }
    else {
        if ( ch == '+' ){
            sym = "PLUS";
            symid = PLUS;
        }
        else if ( ch == '-' ){
            sym = "MINU";
            symid = MINU;
        }
        else if ( ch == '*' ){
            sym = "MULT";
            symid = MULT;
        }
        else if ( ch == '/' ){
            sym = "DIV";
            symid = DIV;
        }
        else if ( ch == ',' ){
            sym = "COMMA";
            symid = COMMA;
        }
        else if ( ch == ':' ){
            sym = "COLON";
            symid = COLON;
        }
        else if ( ch == ';' ){
            sym = "SEMICN";
            symid = SEMICN;
        }
        else if ( ch == '{' ){
            sym = "LBRACE";
            symid = LBRACE;
        }
        else if ( ch == '[' ){
            sym = "LBRACK";
            symid = LBRACK;
        }
        else if ( ch == '(' ){
            sym = "LPARENT";
            symid = LPARENT;
        }
        else if ( ch == '}' ){
            sym = "RBRACE";
            symid = RBRACE;
        }
        else if ( ch == ']' ){
            sym = "RBRACK";
            symid = RBRACK;
        }
        else if ( ch == ')' ){
            sym = "RPARENT";
            symid = RPARENT;
        }
        id += ch;
        getch();
    }
    if ( !strcmp(sym.c_str(), "INTCON") ) {
        laxrst << sym << ' ' << num << endl;
    }
    else if ( !strcmp(sym.c_str(), "FLOATCON") ){
        laxrst << sym << ' ' << dnum << endl;
    }
    else{
        laxrst << sym << ' ' << id << endl;
    }

    return 0;
}

///read one character in source file and store in 'ch'
void getch(){
    int i = 0;
    prllen = llen;
    if ( cnum == llen && ~line[ llen ] ) {

        ch = fgetc(src);
        while ( ch == ' ' || ch == '\n' || ch == '\t' ){
            if (ch == '\n') {
                lnum++;
            }
            backupline[ i++ ] = ch;  //����Դ�ļ����У����ڷ�������ʱ���
            ch = fgetc(src);
        }
        //������˵��հ��ַ����һ���ַ�
        for ( llen = 0; ch != '\n' && ch != EOF; llen++ ) {
            line[ llen ] = ch;
            backupline[ i++ ] = ch;   //����Դ�ļ����У����ڷ�������ʱ���
            ch = fgetc(src);
        }
        if ( ch == EOF )
            line[ llen ] = EOF;
        else
            line[ llen ] = '\n';
        backupline[ i ] = '\0';

        cnum = 0;
        ch = '\n';
        lnum++;
        /*midcoderst << "##LINE " << lnum << ": ";
        for ( int t = 0; t < llen; t++ ){
            midcoderst << (char)line[ t ];
        }
        midcoderst << endl;*/
    }
    else{
        ch = line[ cnum++ ];
    }
}


//return:
//  true:   a > b
//  false:  a < b
bool stringcmp(char *a, char *b){
    int i = 0;
    while ( true ){
        if (( a[ i ] == 0) && (b[ i ] == 0 )) {
            return true;
        }
        else if ( a[ i ] != b[ i ] ){
            return false;
        }
        else{
            i++;
        }

    }
}