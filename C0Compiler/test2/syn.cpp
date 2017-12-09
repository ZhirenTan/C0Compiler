#include "glob.h"
#include "err.h"



int wd = -1;
symtable maintable;
int  value;     //���泣����ֵ�������ӳ���0��ʾvoid��1��ʾint
int  address;   //�����ĵ�ַ����������ֵ�ĵ�ַ
int  paranum;   //ȫ�ֱ������������������壬��Ϊ�����Ĳ�������ֻ���ڲ������ӳ���ִ�н���֮���֪��������ֻ���ں���Ų�����ű���Ϣ
int  kind;      //���ڴ���ı�־��������
char name[ MAXIDENLEN ]; //��־�������֣��������������ͺ���
int hsymid; //��һ����������
char nowitem[ 200 ];
int returnnum = 0; //��¼�Ƿ��з���ֵ
char procname[ 100 ];
int hiscnt = 1;
int sislob = 1;
int factclass = 0;
#define INT 1
#define CHAR 2
#define subfuncindex(n) maintable.findextable[(n)]

//fill in symbol table
//name:     symbol name
//kind:     the kind of name(see syn.h)
//value:    the value of const
//address:  the address of the var
//para:     the formal parameter of function
int arrnum(char *name){
    int n;
    int m = maintable.findextable[ 1 ];
    n = maintable.findextable[ maintable.ftotal ];
    //���ҷ��ű����Ƿ���
    while ( n < maintable.index ) {
        if ( strcmp(maintable.element[ n ].name, name) == 0 )
            return maintable.element[ n ].paranum;
        n++;
    }

    //����ֳ�����ű����޶��壬�����������ű�
    if ( n == maintable.index ) {
        n = 0;
        while ( n < m ) {
            if ( strcmp(maintable.element[ n ].name, name) == 0 )
                return maintable.element[ n ].paranum;
            n++;
        }
        //���ű���û�ж���
        if ( n == m ) {
            printf("\"%s\" ", name);
            //error(VARNOTDEFINE);
            return -1;
        }
    }
}

void pushtable(char* name, int kind, int value, int address, int paranum, int isVec = 0){

    //whether table is full
    if ( maintable.index >= MAXSYMTABLENUM ){
        error(OUTOFTABLE);
        exit(0);
    }

    //whether conflict
    //function
    if ( kind == FUNCTION ) {
        int iter = 1;
        for ( iter = 1; iter <= maintable.ftotal; iter++ ) {
            if ( !strcmp(maintable.element[ maintable.findextable[ iter ] ].name, name) ) {
                error(SYMBOLCONFLICT);
                return;
            }
        }
        maintable.findextable[ ++maintable.ftotal ] = maintable.index;
    }
    else{
        //others
        int iter = maintable.findextable[ maintable.ftotal ];
        for ( ; iter < maintable.index; iter++ ) {
            if ( !strcmp(maintable.element[ iter ].name, name) ) {
                error(SYMBOLCONFLICT);
                return;
            }
        }
        //if not para and non-glob-var, compare with global var.
        if ( kind != PARA && sislob) {
            iter = 0;
            while ( iter < maintable.findextable[ 1 ] ) {
                if ( !strcmp(maintable.element[ iter ].name, name) ) {
                    error(SYMBOLCONFLICT);
                    return;
                }
                iter++;
            }
        }
    }

    strcpy(maintable.element[ maintable.index ].name, name);
    maintable.element[ maintable.index ].kind = kind;
    maintable.element[ maintable.index ].value = value;
    maintable.element[ maintable.index ].address = maintable.index;
    maintable.element[ maintable.index ].paranum = isVec ? isVec : paranum;
    if (maintable.index <= hiscnt) {
        symtablehis << "----------------------------------------------------------------" << endl;
    }
    hiscnt = maintable.index;
    symtablehis << maintable.index << "\t" << name << "\t" << kind << "\t" << value << "\t" << address << "\t" << paranum << "\t" << isVec << endl;
    maintable.index++;
}

void insertproc(int paranum)
{
    int x = maintable.findextable[ maintable.ftotal ];   //array[i]��ָ����ǵ�ǰ���ڱ������ĺ����ڷ��ű��е�����
    maintable.element[ x ].paranum = paranum;//���뺯������������Ϣ
    return;
}

//look up symbol table
//flag=1ʱ��˵��Ҫ�ڷֳ�����������ң�flag=0ʱ���ڵ�ǰ���ڷֳ�����ű��в���
//flag2=1ʱ��ֻ�ڵ�ǰ��������������
int searchst(char* name, int flag, int flag2 = 0){
    int n;
    int m = maintable.findextable[ 1 ];
    if ( flag == 1 ) {
        n = 1;
        while ( n <= maintable.ftotal ) {
            if ( strcmp(maintable.element[ maintable.findextable[ n ] ].name, name) == 0 )
                break;
            n++;
        }

        //���������־��û�ж���
        if ( n > maintable.ftotal ) {
            printf("\"%s\" ", name);
            //error(FUNCTIONNOTFOUND);
            return -1 * FUNCTIONNOTFOUND;
        }

        //���������ʵ�θ������βθ�����ƥ��
        if ( maintable.element[ maintable.findextable[ n ] ].paranum != paranum ) {
            printf("\"%s\" ", name);
            //error(FORMALPARANUMUNMATCH);
            return -1 * FORMALPARANUMUNMATCH;
        }

        //���������ʶ��������
        if ( maintable.element[ maintable.findextable[ n ] ].value == 0 )
            return -1 * FUNCTIONRETURNNULL;

        return maintable.element[ maintable.findextable[ n ] ].address;
    }
    else{
        n = maintable.findextable[ maintable.ftotal ];
        //���ҷ��ű����Ƿ���
        while ( n < maintable.index ) {
            if ( strcmp(maintable.element[ n ].name, name) == 0 )
                break;
            n++;
        }

        //����ֳ�����ű����޶��壬�����������ű�
        if ( n == maintable.index ) {
            n = 0;
            while ( n < m ) {
                if ( strcmp(maintable.element[ n ].name, name) == 0 )
                    break;
                n++;
            }
            //���ű���û�ж���
            if ( n == m ) {
                printf("\"%s\" ", name);
                //error(VARNOTDEFINE);
                return -1;
            }
        }
        if ( maintable.element[ n ].kind == INTTK || maintable.element[ n ].kind == CHARTK )
            return maintable.element[ n ].address;
        if ( maintable.element[ n ].kind == CINTCON || maintable.element[ n ].kind == CCHARCON )
            return -2;    //const ���ظ�������ַ
        if ( maintable.element[ n ].kind == PARA )
            return maintable.element[ n ].address;    //������־
        return -1;
    }
}



//clear symbol table
void flushst(){
    int x = maintable.index - 1;
    int y;
    while ( ( maintable.element[ x ].kind == INTTK || maintable.element[ x ].kind == CHARTK || maintable.element[ x ].kind == CINTCON
        || maintable.element[ x ].kind == CCHARCON || maintable.element[ x ].kind == PARA ) && strcmp(maintable.element[ x ].name, procname) != 0 ) {
        maintable.element[ x ].kind = 0;
        maintable.element[ x ].address = 0;
        maintable.element[ x ].paranum = 0;
        maintable.element[ x ].value = 0;
        y = 0;
        while ( y<30 )
            maintable.element[ x ].name[ y++ ] = '\0';
        x--;
    }
    maintable.index = x + 1;
    return;
}

//9.������    ::= �ۣ�����˵�����ݣۣ�����˵������{���з���ֵ�������壾|���޷���ֵ�������壾}����������
void program(){
    getsym();

    if ( symid == CONSTTK ) {
        //���ó���˵���ӳ���
        conststate();
        //����Ƿֺ�,����������
    }

    //����������з���ֵ�ĺ�������ͬ������ͷ�������Ԥ��3�����������ж�
    while ( symid == INTTK || symid == CHARTK ) {
        int tempid = symid;//�ָ��ֳ���
        char tempch = ch;//�ָ��ֳ���
        int tempcnum = cnum;//�ָ��ֳ���
        kind = symid;
        getsym();
        strcpy(name, id.c_str());//��������

        //�����ʶ���Ϸ�
        if ( symid != IDEN ) {
            error(IDENTIFIERLACK);//notdone
            continue;
        }
        getsym();
        //����Ƕ��Ż��߷ֺţ����Ǳ�������
        if ( symid == COMMA || symid == LBRACK ) {
            symid = tempid;
            cnum = tempcnum;
            ch = tempch;
            varstate();
            continue;
        }
        else if ( symid == SEMICN ) {
            value = 0;
            address = 0;
            paranum = 0;
            pushtable(name, kind, value, address, paranum);
            if ( kind == INTTK ) insmidcode("int", "  ", "  ", name);
            else if ( kind == CHARTK ) insmidcode("char", "  ", "  ", name);
            getsym();
            continue;
        }
        else{
            symid = tempid;
            cnum = tempcnum;
            ch = tempch;
            break;
        }
    }
    if ( symid == CONSTTK ) {
        errnum++;
        printf("Error %d: Line %d, ��������λ�ô���\n", errnum, lnum);
        conststate();
        //����Ƿֺ�,����������
    }
    sislob = 0;
    //���뺯������
    while ( symid == INTTK || symid == CHARTK || symid == VOIDTK ) {
        int temp = symid;

        //19.���з���ֵ�������壾  ::=  ������ͷ������(������������)��  ��{����������䣾��}��
        if ( symid == INTTK || symid == CHARTK ) {
            defhead();
            if ( symid != LPARENT ) {
                error(LEFTPARENTLACK);
                continue;
            }
            varcnt = 0;     //��ʱ�������¿�ʼ����
            returnnum = 0;//���Ƿ���ڷ���ֵ�ı�ǳ�ʼ����0��ʾ�޷���ֵ����δ����return
            kind = FUNCTION;//��ʾ��ǰ�ĺ������ӳ��򣬼���������
            value = 1;//1���ں�����˵����ʾ����ֵΪInt 0����ֵΪvoid
            address = 0;
            paranum = 0;
            pushtable(name, kind, value, address, paranum);
            strcpy(procname, name);//����������
            if ( temp == INTTK ) insmidcode("func", "int", "  ", procname);
            if ( temp == CHARTK ) insmidcode("func", "char", "  ", procname);
            getsym();
            parametertable();
            //ȱ'('��������')'����С����
            if ( symid != RPARENT ){
                error(RIGHTPARENTLACK);
                continue;
            }
            getsym();
            //ȱ'{'��������䣾'}'���������
            if ( symid != LBRACE ) {
                error(LEFTBRACELACK);
                continue;
            }
            getsym();
            complexsentence();
            //ȱ'{'��������䣾'}'���Ҵ�����
            if ( symid != RBRACE ) {
                error(RIGHTBRACELACK);
                continue;
            }

            //����ȱ�ٷ���ֵ
            if ( returnnum == 0 ) {
                error(NONERETURN);
                continue;
            }
            getsym();
            insmidcode("end", "  ", "  ", procname);//����������Ԫʽ����
            flushst();//������ű�������յ��Ǵ��Ӻ����ķ��ű�,������Ӻ�����
        }

        if ( symid == VOIDTK ) {
            getsym();
            //�����һ��������main�����ǣ���������    ::= void main '(' ')'  '{'��������䣾'}'
            if ( symid == MAINTK ) {
                varcnt = 0;        //��ʱ�������¿�ʼ����
                strcpy(name, "main");
                kind = FUNCTION;
                value = 0;
                address = 0;
                paranum = 0;
                pushtable(name, kind, value, address, paranum);
                strcpy(procname, name);
                insmidcode("func", "  ", "  ", procname);
                getsym();
                if ( symid != LPARENT ) {
                    error(LEFTPARENTLACK);
                    continue;
                }
                getsym();
                if ( symid != RPARENT ) {
                    error(RIGHTPARENTLACK); 
                    continue;
                }
                getsym();
                if ( symid != LBRACE ) {
                    error(LEFTBRACELACK);
                    continue;
                }
                getsym();
                complexsentence();
                mf++;    //�������ĸ���+1
                insmidcode("end", "  ", "  ", procname);
                if ( symid != RBRACE )
                {
                    error(RIGHTBRACELACK);
                    continue;
                }
                return;    //Դ�������������
            }

            //20.���޷���ֵ�������壾  ::= void����ʶ������(������������)��  ��{����������䣾��}��
            if ( symid != IDEN ) {
                error(IDENTIFIERLACK);
                continue;
            }
            varcnt = 0;          //��ʱ�������¿�ʼ����
            strcpy(name, id.c_str());

            kind = FUNCTION;
            value = 0;
            address = 0;
            paranum = 0;
            pushtable(name, kind, value, address, paranum);
            strcpy(procname, name);
            insmidcode("func", "void", "  ", procname);
            getsym();
            if ( symid != LPARENT ) {
                error(LEFTPARENTLACK);
                continue;
            }
            getsym();
            parametertable();
            if ( symid != RPARENT ) {
                error(RIGHTPARENTLACK);
                continue;
            }
            getsym();
            if ( symid != LBRACE ) {
                error(LEFTBRACELACK);
                continue;
            }
            getsym();
            complexsentence();
            if ( symid != RBRACE ) {
                error(RIGHTBRACELACK);
                continue;
            }
            getsym();
            insmidcode("end", "  ", "  ", procname);
            flushst();
        }
    }
}


//18.�����ͱ�ʶ����   ::=    int | float | char
//22.��������         ::=    ��������
//23.��������    ::=  �����ͱ�ʶ��������ʶ����{,�����ͱ�ʶ��������ʶ����}| ���գ�
void parametertable()
{
    int i = 0;     //��¼��������
    int temp;
    if ( symid == INTTK || symid == CHARTK ) {
        do{
            if ( symid == COMMA )
                getsym();
            temp = symid;
            defhead();    //���������ͱ�������ʱ��ȫ��ͬ�ģ�ֻ��������ԷֺŽ�β
            kind = PARA;     //4��ʾΪ��������
            value = 0;
            address = i;  //��ַΪi����������λ�ã���ַȫ��Ϊ��Ե�ַ��
            paranum = 0;
            pushtable(name, kind, value, address, paranum);  //����������������ű�
            if ( temp == INTTK ) insmidcode("para", "int", "  ", name);
            else if ( temp == CHARTK ) insmidcode("para", "char", "  ", name);
            i++;//����������һ
        } while ( symid == COMMA );//����Ƕ��ţ����������Ĳ���
    }
    paranum = i;//��ǰ�Ĳ�������
    insertproc(paranum);//���뺯���Ĳ�������
    return;
}

//10.������˵���� ::=  const���������壾;{ const���������壾;}
//ATTENTION:�������ֱ�Ӷ�������
void conststate(){
    while ( symid == CONSTTK ) {
        getsym();
        int tclass;
        if ( symid == INTTK || symid == CHARTK ) {
            kind = ( symid == INTTK ) ? CINTCON : CCHARCON;
            address = 0;
            paranum = 0;
            tclass = symid;
            getsym();
            constdef(tclass);
            while ( symid == COMMA ) {
                getsym();
                constdef(tclass);
            }
            //�����������һ���ַ�һ���Ƿֺ�
            if ( symid != SEMICN ) {
                error(SEMICOLONLACK);
                return;
            }
            hsymid = symid;
            getsym();
        }
        else{
            error(KEYWORDERROR);
            return;
        }
    }
    return;
}

//11.���������壾   ::=   int����ʶ��������������{,����ʶ��������������}    | char����ʶ���������ַ���{,����ʶ���������ַ���}
//ATTENTION:���ﲻ����int��char, int, �����߽���tclass����
void constdef(int tclass){
    //without int or char
    char temp[ 200 ];
    //�ṹ��IDEN = XXXX
    if ( symid == IDEN ) {
        strcpy(name, id.c_str());
        getsym();
        //if is '='
        if ( symid == ASSIGN ) {
            getsym();
            // if is '+' or '-'
            if ( symid == PLUS || symid == MINU ) {
                int symb = symid;
                getsym();
                // if is integer
                if ( tclass == INTTK && symid == INTCON ) {
                    if ( symb == PLUS ) {
                        value = num;
                    }
                    else{
                        value = 0 - num;
                    }
                    pushtable(name, kind, value, address, paranum);
                    sprintf(temp, "%d", value);
                    insmidcode("const", "int", temp, name);
                }

                // if is char
            }
            else if ( symid == CHARCON ){
                value = id.c_str()[ 0 ];
                pushtable(name, kind, value, address, paranum);
                sprintf(temp, "%d", value);
                insmidcode("const", "char", temp, name);
            }
            else if ( symid == INTCON ){
                value = num;
                pushtable(name, kind, value, address, paranum);
                sprintf(temp, "%d", value);
                insmidcode("const", "int", temp, name);
            }
            else{
                error(UNKNOWRIGHTSYM);
                return;
            }
        }else{
            error(CONSTNOTINIT);
            return;
        }
    }
    else{
        error(CSTDEFINEFAIL);
    }
    getsym();
    return;
}

//15.������ͷ����   ::=  int����ʶ���� | char����ʶ����
//ATTENTION: ��������int/char�ͱ�ʶ���Լ�������һ�����ʣ���
void defhead()
{
    if ( symid == INTTK || symid == CHARTK ) {
        kind = symid;
        getsym();

        if ( symid != IDEN ) {
            error(IDENTIFIERLACK, 1);
            return;
        }
        else {
            strcpy(name, id.c_str());//��������������
        }
    }
    else{
        error(KEYWORDERROR, 1);
        return;
    }
    getsym();
    return;
}

//16.������˵����  ::= ���������壾;{���������壾;}
void varstate()
{
    vardef();
    //ȱ��';'
    if ( symid != SEMICN ) {
        error(SEMICOLONLACK, 1);
    }
    getsym();
    return;
}


//17.���������壾  ::= �����ͱ�ʶ����(����ʶ����|����ʶ������[�����޷�����������]��){,(����ʶ����|����ʶ������[�����޷�����������]�� )}
void vardef()
{
    if ( symid == INTTK || symid == CHARTK ) {
        kind = symid;
        getsym();

        if ( symid != IDEN ) {
            error(IDENTIFIERLACK, 2);
            return;
        }
        else {
            strcpy(name, id.c_str());//��������������
        }
    }
    else{
        error(KEYWORDERROR, 2);
        return;
    }
    getsym();
    int isVec = ( symid == LBRACK );
    value = 0;
    address = 0;
    paranum = 0;
    if ( isVec ) {    //���������
        getsym();
        if ( symid != INTCON ) {
            error(IMMLACK);
            return;
        }
        else{
            pushtable(name, kind, value, address, paranum, num);
            if ( kind == INTTK )
                insmidcode("inta", 0, num, name);
            else if ( kind == CHARTK )
                insmidcode("chara", 0, num, name);
            getsym();
            if ( symid != RBRACK ) {
                error(RIGHTBRACKLACK, 1);
                return;
            }
            else{
                getsym();
            }
        }
    }
    else{
        pushtable(name, kind, value, address, paranum, isVec);
        if ( kind == INTTK )
            insmidcode("int", "  ", "  ", name);
        else if ( kind == CHARTK )
            insmidcode("char", "  ", "  ", name);
    }

    //��������Ƕ��ţ���ô��������δ����
    while ( symid == COMMA ) {
        getsym();
        //���ֲ��Ϸ��ı�־��
        if ( symid != IDEN ) {
            error(IDENTIFIERLACK, 2);
            return;
        }
        strcpy(name, id.c_str());
        getsym();
        isVec = ( symid == LBRACK );
        if ( isVec ) {    //���������
            getsym();
            if ( symid != INTCON ) {
                error(IMMLACK);
                return;
            }
            else{
                pushtable(name, kind, value, address, paranum, num);
                if ( kind == INTTK )
                    insmidcode("int", 0, num, name);
                else if ( kind == CHARTK )
                    insmidcode("char", 0, num, name);
                getsym();
                if ( symid != RBRACK ) {
                    error(RIGHTBRACKLACK);
                    return;
                }
                else{
                    getsym();
                }
            }
        }
        else{
            pushtable(name, kind, value, address, paranum, isVec);
            if ( kind == INTTK )
                insmidcode("int", "  ", "  ", name);
            else if ( kind == CHARTK )
                insmidcode("char", "  ", "  ", name);
        }
    }
    return;
}

//21.��������䣾   ::=  �ۣ�����˵�����ݣۣ�����˵�����ݣ�����У�
void complexsentence()
{
    if ( symid == CONSTTK ) {
        //���ó���˵���ӳ���
        conststate();
        //����Ƿֺ�,����������
        while ( hsymid == SEMICN ) {
            if ( symid == CONSTTK )
                conststate();
            else break;
        }
    }

    //�ж��Ƿ�Ϊ����˵��
    //���ò��ֱ������ڻָ��ֳ�
    while ( symid == INTTK || symid == CHARTK ) {
        int tempid = symid;//�ָ��ֳ���
        char tempch = ch;//�ָ��ֳ���
        int tempcnum = cnum;//�ָ��ֳ���
        kind = symid;
        getsym();
        strcpy(name, id.c_str());//��������

        //�����ʶ���Ϸ�
        if ( symid != IDEN ) {
            error(IDENTIFIERLACK, 2);
            return;
        }
        getsym();
        //����Ƕ��Ż��߷ֺţ����Ǳ�������
        if ( symid == COMMA || symid == LBRACK) {
            symid = tempid;
            cnum = tempcnum;
            ch = tempch;
            varstate();
            continue;
        }
        else if ( symid == SEMICN ) {
            value = 0;
            address = 0;
            paranum = 0;
            pushtable(name, kind, value, address, paranum);
            if ( kind == INTTK ) insmidcode("int", "  ", "  ", name);
            else if ( kind == CHARTK ) insmidcode("char", "  ", "  ", name);
            getsym();
            continue;
        }
        else{
            error(SEMICOLONLACK, 1);
            return;
        }
    }

    //�����
    sentencelist();
    return;
}

//37.������У�   ::= ������䣾��
void sentencelist()
{
    sentence();
    //��䣬��������������µĶ�������ô�������
    while ( symid == IFTK || symid == WHILETK || symid == FORTK || symid == LBRACE
        || symid == IDEN || symid == RETURNTK || symid == SCANFTK || symid == PRINTFTK ) {
        sentence();
    }
}

//30.��������䣾  ::=  if ��(������������)������䣾��else����䣾��
void ifsentence()
{
    char label1[ 10 ], label2[ 10 ], conditionvalue[ 30 ];
    strcpy(label1, nextlab());  //���������if��������label1��label1��if�Ľ�������
    strcpy(label2, nextlab());  //�������else����else�Ľ���λ����label2
    getsym();
    //�١�(��
    if ( symid != LPARENT ) {
        error(LEFTPARENTLACK, 1);
        return;
    }
    getsym();
    //��������
    condition();
    strcpy(conditionvalue, nowitem);  //���������ֵ��nowitem����,�˴�Ӧ���Ǹ���ʱ����$_x
    insmidcode("jne", "  ", "  ", label1);  //�Ƚϣ�Ϊ�ٵ�ʱ����ת
    if ( symid != RPARENT ) {
        error(RIGHTPARENTLACK, 1);
        return;
    }
    getsym();
    sentence();
    insmidcode("jmp", "  ", "  ", label2);//��ִ��else�Ĳ���
    insmidcode("lab:", "  ", "  ", label1);
    if ( symid == ELSETK ) {
        getsym();
        sentence();
    }
    insmidcode("lab:", "  ", "  ", label2);
    return;
}

//31.��������    ::=  �����ʽ������ϵ������������ʽ���������ʽ��
void condition()
{
    char place1[ 30 ], place2[ 30 ];
    expression();
    strcpy(place1, nowitem);  //����������һ�����ʽ
    //��ϵ�����
    if ( symid == LSS || symid == LEQ || symid == GRE || symid == GEQ || symid == NEQ || symid == EQL ) {
        if ( symid == LSS ) {
            getsym();
            expression();
            strcpy(place2, nowitem);
            insmidcode("<", place1, place2, "  ");
        }
        else if ( symid == LEQ ) {
            getsym();
            expression();
            strcpy(place2, nowitem);
            insmidcode("<=", place1, place2, "  ");
        }
        else if ( symid == GRE ) {
            getsym();
            expression();
            strcpy(place2, nowitem);
            insmidcode(">", place1, place2, "  ");
        }
        else if ( symid == GEQ ) {
            getsym();
            expression();
            strcpy(place2, nowitem);
            insmidcode(">=", place1, place2, "  ");
        }
        else if ( symid == NEQ ) {
            getsym();
            expression();
            strcpy(place2, nowitem);
            insmidcode("!=", place1, place2, "  ");
        }
        else if ( symid == EQL ) {
            getsym();
            expression();
            strcpy(place2, nowitem);
            insmidcode("==", place1, place2, "  ");
        }
        return;
    }
    strcpy(place2, "0");
    insmidcode("!=", place1, place2, "  ");
    return;
}

//32.��ѭ����䣾   ::=   while '('��������')'����䣾
//                   | for'('����ʶ�����������ʽ��;��������;����ʶ����������ʶ����(+|-)��������')'����䣾
void loopsentence()
{
    int s = 0;
    char names[ 30 ], names1[ 30 ], names2[ 30 ], place2[ 30 ], place3[ 30 ];
    char label1[ 10 ], label2[ 10 ], op_flag[ 5 ];
    strcpy(label1, nextlab());
    strcpy(label2, nextlab());
    if ( symid == WHILETK ) {
        getsym();
        if ( symid != LPARENT ) {
            error(LEFTPARENTLACK, 1);
            return;
        }
        insmidcode("lab:", "  ", "  ", label1);
        getsym();
        condition();
        if ( symid != RPARENT ) {
            error(RIGHTPARENTLACK, 1);
            return;
        }
        insmidcode("jne", "  ", "  ", label2);
        getsym();
        sentence();
        insmidcode("jmp", "  ", "  ", label1);
        insmidcode("lab:", "  ", "  ", label2);
        return;
    }

    if ( symid == FORTK ) {    //for|(...)
        getsym();
        if ( symid != LPARENT ) {//for(|...)
            error(LEFTPARENTLACK, 1);
            return;
        }
        getsym();

        if ( symid != IDEN ) {    //for(i|=x;...;...)
            error(UNACCEPTABLESENTENCE);//���Ϸ��ľ���
            return;
        }
        strcpy(names, id.c_str());
        getsym();
        if ( symid != ASSIGN ) {    //for(i=|..;...;...)
            error(UNACCEPTABLESENTENCE);
            return;
        }
        s = searchst(names, 0);
        if ( s < 0 ) {     //����ǳ���
            if ( s == -1 ) error(VARNOTDEFINE, 1);  //"="����ǲ��Ϸ��ı�ʶ��
            else    error(ASSIGNCONST);
            return;
        }
        getsym();
        expression();    //for(i=a+b|;...;...)
        insmidcode("=", nowitem, "  ", names);
        if ( symid != SEMICN ) {
            error(SEMICOLONLACK, 2);
            return;
        }
        getsym();//for(i=a+b;|...;...)
        insmidcode("lab:", "  ", "  ", label1);
        condition();//for(i=a+b;...;|...)
        insmidcode("jne", "  ", "  ", label2);
        if ( symid != SEMICN ) {
            error(SEMICOLONLACK, 2);
            return;
        }
        getsym();        //for(i=a+b;...;|...)
        //����ʶ����������ʶ����(+|-)��������!!!!!!!!!!!
        if ( symid != IDEN ) {
            error(IDENTIFIERLACK, 3);
            return;
        }
        strcpy(names1, id.c_str());
        s = searchst(names1, 0);
        if ( s < 0 ) {     //����ǳ���
            if ( s == -1 ) error(VARNOTDEFINE, 1);  //"="����ǲ��Ϸ��ı�ʶ��
            else    error(ASSIGNCONST);
            return;
        }
        getsym();
        if ( symid != ASSIGN ) {
            error(UNACCEPTABLESENTENCE);
            return;
        }
        getsym();
        if ( symid != IDEN ) {
            error(IDENTIFIERLACK, 3);
            return;
        }
        strcpy(names2, id.c_str());
        s = searchst(names2, 0);
        if ( s < 0 ) {     //����ǳ���
            if ( s == -1 ) error(VARNOTDEFINE, 1);  //"="����ǲ��Ϸ��ı�ʶ��
            else    error(ASSIGNCONST);
            return;
        }

        getsym();
        if ( symid != PLUS && symid != MINU ) {
            error(PLUSMINULACK);
            return;
        }
        if ( symid == PLUS ) strcpy(op_flag, "+ ");
        else if ( symid == MINU ) strcpy(op_flag, "- ");

        getsym();
        if ( symid != INTCON || num == 0 ) {
            error(UNKNOWRIGHTSYM, 1);     //�Ⱥ��ұ߲��ǺϷ�������
            return;
        }
        strcpy(place2, id.c_str());

        getsym();
        if ( symid != RPARENT ) {
            error(RIGHTPARENTLACK, 1);
            return;
        }

        getsym();
        sentence();
        strcpy(place3, nextvar());
        insmidcode(op_flag, names2, place2, place3);
        insmidcode("=", place3, "  ", names1);

        insmidcode("jmp", "  ", "  ", label1);
        insmidcode("lab:", "  ", "  ", label2);
        return;
    }
}

//28.����䣾    ::= ��������䣾����ѭ����䣾����{��������У���}�������з���ֵ����������䣾;
//|���޷���ֵ����������䣾;������ֵ��䣾;��������䣾;����д��䣾;�����գ�;����������䣾;
void sentence(){
    int s;

    //�������
    if ( symid == IFTK ) {
        ifsentence();
        return;
    }

    //ѭ�����
    if ( symid == WHILETK || symid == FORTK ) {
        loopsentence();
        return;
    }

    //{������С�}
    if ( symid == LBRACE ) {
        getsym();
        sentencelist();
        //ȱ��}��
        if ( symid != RBRACE ) {
            error(RIGHTBRACELACK, 1);
            return;
        }
        //��ȱ��}��
        getsym();
        return;
    }

    //����������������ֵ��䣾;
    
    if ( symid == IDEN ) {
        int isVec = 0;
        char names[ MAXIDENLEN ];
        strcpy(names, id.c_str());
        getsym();
        //����ֵ��䣾;
        //29.����ֵ��䣾   ::=  ����ʶ�����������ʽ��|����ʶ����'['<���ʽ>']'�������ʽ��
        char place2[ 100 ];
        if ( symid == LBRACK ){ //���������
            s = searchst(names, 0);
            if ( s == -1 )
            {
                error(VARNOTDEFINE, 1);
                return;
            }
            getsym();
            expression();
            strcpy(place2, nowitem);
            if (isdigit(nowitem[0])){
                int arrnums = 0;
                arrnums = arrnum(names);
                if (atoi(nowitem) >= arrnums){
                    printf("Warning: Line:%d index of array out of boundary, which should be less than %d.\n",lnum, atoi(nowitem));
                }
            }
            isVec = 1;
            if ( symid != RBRACK ) {
                error(RIGHTBRACKLACK, 1);
                return;
            }
            getsym();
        }
        if ( symid == ASSIGN ) {
            s = searchst(names, 0);
            if ( s < -1 ) {     //����ǳ���
                error(ASSIGNCONST);            //"="����ǲ��Ϸ��ı�ʶ��
                return;
            }
            getsym();
            expression();
            if ( isVec ){
                insmidcode("[]=", names, place2, nowitem);
            }
            else{
                insmidcode("=", nowitem, "  ", names);
            }
            
            if ( symid != SEMICN ) {
                error(SEMICOLONLACK, 2);
                return;
            }
            getsym();
            return;
        }

        //�����������
        if ( symid == LPARENT ) {
            getsym();
            valueofpara();  //���㲢��¼������ֵ�Լ�����
            if ( symid != RPARENT ) {
                error(RIGHTPARENTLACK, 1);
                return;
            }
            s = searchst(names, 1);
            if ( s == -1 * FUNCTIONNOTFOUND ) {
                error(FUNCTIONNOTFOUND);
                return;
            }
            else if ( s == -1 * FORMALPARANUMUNMATCH ){
                error(FORMALPARANUMUNMATCH, 1);
                return;
            }
            insmidcode("call", names, "  ", "  ");
            getsym();
            if ( symid != SEMICN ) {
                error(SEMICOLONLACK, 2);
                return;
            }
            getsym();
        }

        else {
            error(UNACCEPTABLESENTENCE);    //���Ϸ��ľ���
            return;
        }
        return;
    }

    //�����
    if ( symid == SCANFTK ) {
        scanfsentence();
        if ( symid != SEMICN ) {
            error(SEMICOLONLACK, 2);
            return;
        }
        getsym();
        return;
    }

    //д���
    if ( symid == PRINTFTK ) {
        printfsentence();
        if ( symid != SEMICN ) {
            error(SEMICOLONLACK, 2);
            return;
        }
        getsym();
        return;
    }

    //�������
    if ( symid == RETURNTK ) {
        returnsentence();
        //��������ȱ������
        if ( symid != SEMICN ) {
            error(SEMICOLONLACK, 2);
            return;
        }
        getsym();
        return;
    }
    if ( symid == SEMICN )
    {
        getsym();
        return;
    }

    //���Ϸ��ľ���
    error(UNACCEPTABLESENTENCE);
    return;
}

//36.��ֵ������   ::=   �����ʽ��{,�����ʽ��}�����գ�
void valueofpara()
{
    int j = 0;
    vector<string> paraqueue;
    do {
        if ( symid == COMMA )
            getsym();
        if ( symid == PLUS || symid == MINU || symid == IDEN || symid == LPARENT || symid == CHARCON || symid == INTCON ) {
            expression();
            paraqueue.push_back(nowitem);
            j++;
        }
    } while ( symid == COMMA );
    char tc[ 20 ];
    for ( int i = 0; i < paraqueue.size(); i++ ) {
        strcpy(tc, paraqueue[ i ].c_str());
        insmidcode("fupa", "  ", "  ", tc);
    }
    paranum = j;
    paraqueue.~vector();
    return;
}

//38.������䣾    ::=  scanf ��(������ʶ����{,����ʶ����}��)��
void scanfsentence()
{
    char names[ 30 ];
    int s;
    getsym();
    if ( symid != LPARENT ) {
        error(LEFTPARENTLACK, 1);
        return;
    }

    do{
        getsym();
        if ( symid != IDEN ) {
            error(IDENTIFIERLACK, 3);         //���Ϸ��ı�־��
            return;
        }
        strcpy(names, id.c_str());
        s = searchst(names, 0);
        if ( s < 0 ) {
            if ( s == -1 )
                error(VARNOTDEFINE, 1);            //δ�����־��
            else
                error(ASSIGNCONST, 1);
            return;
        }
        insmidcode("scf", "  ", "  ", names);
        getsym();
    } while ( symid == COMMA );

    if ( symid != RPARENT ) {
        error(RIGHTPARENTLACK, 1);
        return;
    }
    getsym();
    return;
}

//39.��д��䣾    ::=   printf ��(�����ַ�����,�����ʽ����)��|
//                     printf ��(�����ַ�������)��| printf ��(�������ʽ����)��
void printfsentence()
{
    char place1[ 200 ] = "", place2[ 30 ] = ""; //place1���ַ�����place2�Ǳ��ʽ
    getsym();
    if ( symid != LPARENT ) {
        error(LEFTPARENTLACK, 1);
        return;
    }
    getsym();
    if ( symid == STRCON ) {
        strcpy(place1, id.c_str());
        getsym();
        if ( symid == COMMA ) {
            getsym();
            expression();
            strcpy(place2, nowitem);
        }
    }
    else {
        expression();
        strcpy(place2, nowitem);
    }
    if ( symid != RPARENT ) {
        error(RIGHTPARENTLACK, 1);
        return;
    }
    insmidcode("prt", place1, place2, (factclass == CHAR) ? "char" : "int");
    getsym();
    return;
}

//40.��������䣾   ::=  return[��(�������ʽ����)��]
void returnsentence()
{
    char place3[ 30 ];
    getsym();
    if ( symid == LPARENT ) {
        getsym();
        expression();//���ʽ�ӳ���
        strcpy(place3, nowitem);
        if ( symid != RPARENT ) {
            error(RIGHTPARENTLACK, 1);
            return;
        }
        insmidcode("ret", "  ", "  ", place3);
        returnnum++;
        getsym();
        return;
    }
    insmidcode("ret", "  ", "  ", "  ");
    return;
}

//25.�����ʽ��    ::= �ۣ������ݣ��{���ӷ�����������}
void expression()
{
    factclass = 0;
    char place1[ 30 ], place2[ 30 ], place3[ 30 ];
    //�У����������
    if ( symid == PLUS || symid == MINU ) {
        factclass = INT;
        if ( symid == PLUS ) {
            getsym();
            item();//���������Ľ�������ȫ�ֱ���nowitem
            strcpy(place3, nowitem);//����Ľ��������ʱ������λ��
        }
        if ( symid == MINU ) {
            getsym();
            item();//��
            strcpy(place1, nowitem);
            strcpy(place3, nextvar());
            insmidcode("-", "0 ", place1, place3); //place3 = 0 - (place1)
        }
    }
    else {
        item();
        strcpy(place3, nowitem);
    }
    while ( symid == PLUS || symid == MINU ) {
        factclass = INT;
        strcpy(place1, place3);
        if ( symid == PLUS ) {
            getsym();
            item();
            strcpy(place2, nowitem);
            strcpy(place3, nextvar());
            insmidcode("+", place1, place2, place3);
            continue;
        }
        else {
            getsym();
            item();
            strcpy(place2, nowitem);
            strcpy(place3, nextvar());
            insmidcode("-", place1, place2, place3);
            continue;
        }
    }
    strcpy(nowitem, place3);//�ѱ��ʽ������ֵ�����p֮��
    return;
}

//26.���     ::= �����ӣ�{���˷�������������ӣ�}
void item()
{
    char place1[ 200 ], place2[ 200 ], place3[ 200 ];
    factor();
    strcpy(place3, nowitem);//���ֲ�����Ϊ�˶Ը�ֻ�и�ֵ�����
    while ( symid == MULT || symid == DIV ) {
        factclass = INT;
        strcpy(place1, place3);
        if ( symid == MULT ) {
            getsym();
            factor();
            strcpy(place2, nowitem);
            strcpy(place3, nextvar());//������ʱ����
            insmidcode("*", place1, place2, place3);
            continue;
        }
        if ( symid == DIV ) {
            getsym();
            factor();
            strcpy(place2, nowitem);
            strcpy(place3, nextvar());
            insmidcode("/", place1, place2, place3);
            continue;
        }
    }
    strcpy(nowitem, place3);  //ÿһ���������ֵ���ڱ���nowitem����
    return;
}


//27.�����ӣ�    ::= ����ʶ����������ʶ������[�������ʽ����]������������|���ַ��������з���ֵ����������䣾|��(�������ʽ����)��
void factor()
{
    int t = -1;
    char names[ 30 ], place3[ 30 ];
    //����ʶ���������з���ֵ����������䣾
    if ( symid == IDEN ) {
        strcpy(names, id.c_str());
        getsym();
        //����������ţ����ǣ��з���ֵ����������䣾
        if ( symid == LPARENT ) {
            getsym();
            valueofpara();
            if ( symid != RPARENT ) {
                error(LEFTPARENTLACK, 2);
                return;
            }
            t = searchst(names, 1);
            if ( t < 0 ) {
                if ( t == -1 * FUNCTIONRETURNNULL )
                    error(FUNCTIONRETURNNULL);   //�����޷���ֵ�����ܳ����ڱ��ʽ��
                else if ( t == -1 * FUNCTIONNOTFOUND )
                    error(FUNCTIONNOTFOUND, 1);
                return;
            }
            strcpy(place3, nextvar());//������ʱ����
            insmidcode("call", names, "  ", place3);//�����õķ���ֵ�������ʱ��������
            strcpy(nowitem, place3);
            getsym();
            return;
        }

        //����Ǳ��������顢
        else if ( symid != LBRACK ){
            t = searchst(names, 0);      //������ҵ���ʶ����Ӧ�ĵ�ַ
            if (t == -1) {
                error(IDENTIFIERLACK, 4);
                return;
            }
            if ( t < -1 ) {
                t = -t / 2;
            }
            strcpy(nowitem, names);
        }
        else if ( symid == LBRACK ){      //���������
            getsym();
            int tclass = factclass;
            expression();
            factclass = tclass;
            char ttt[ 30 ];
            strcpy(ttt, nowitem);
            
            if (isdigit(nowitem[0])){
                int arrnums = 0;
                arrnums = arrnum(names);
                if (atoi(ttt) >= arrnums){
                    printf("Warning: Line:%d index of array out of boundary, which should be less than %d.\n",lnum, atoi(ttt));
                }
            }
            if ( symid != RBRACK ) {
                error(RIGHTBRACKLACK);
                return;
            }
            else{
                strcpy(nowitem, nextvar());
                insmidcode("geta", names, ttt, nowitem);
                getsym();
            }
        }
        return;
    }

    //'('�����ʽ��')'
    if ( symid == LPARENT ) {
        getsym();
        expression();
        if ( symid != RPARENT ) {
            error(RIGHTPARENTLACK, 2);
            return;
        }
        getsym();
        return;
    }

    // �������������ַ���
    if ( symid == PLUS || symid == MINU || symid == INTCON || symid == CHARCON ) {
        int temp = 1;
        if ( symid == PLUS )
            getsym();
        else if ( symid == MINU ) {
            temp = -1;
            getsym();
        }
        if ( symid != INTCON ) {
            if ( symid == CHARCON ){
                if ( factclass != INT ) {
                    factclass = CHAR;
                }
                sprintf(nowitem, "%d", id.c_str()[0]);
                getsym();
                return;
            }
            error(IMMLACK);
            return;
        }
        sprintf(nowitem, "%d", temp * num);
        factclass = INT;
        getsym();
        return;
    }

    error(EXPRESSIONERROR);                  //���ʽȱʧ�����
    return;
}