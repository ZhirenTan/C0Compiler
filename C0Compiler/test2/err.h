#ifndef exc0compiler_err_h
#define exc0compiler_err_h

#define NOSUCHFILE            999
#define FILEINCOMPLETE      0
#define DOUBLEQUOTESLACK    1
#define UNACCEPTATLECHAR    2
#define SINGLEQUOTESLACK    3
#define OUTOFTABLE          4
#define SYMBOLCONFLICT      5
#define CSTDEFINEFAIL       6   //��������ʧ��
#define VARNOTINIT          7   //����δ��ʼ��
#define UNKNOWRIGHTSYM      8   //�Ⱥ��ұ��ַ��Ƿ�
#define SEMICOLONLACK       9   //��ʧ������
#define KEYWORDERROR        10  //�ؼ��ִ���
#define IDENTIFIERLACK      11  //��ʧ��־��
#define RIGHTBRACKLACK      12  //��ʧ��]��
#define FUNCTIONNOTFOUND    13  //���ú���δ����
#define FORMALPARANUMUNMATCH 14 //�βθ�����ƥ��
#define VARNOTDEFINE        15  //δ�������
#define LEFTPARENTLACK      16  //��ʧ������
#define RIGHTPARENTLACK     17  //��ʧ������
#define IMMLACK             18  //��ʧ������
#define RIGHTBRACELACK      19  //��ʧ��}��
#define FUNCTIONRETURNNULL  20  //�����޷���ֵȴ�����ڲ���
#define EXPRESSIONERROR     21  //���ʽȱʧ�����
#define UNACCEPTABLESENTENCE 22 //���Ӳ��Ϸ�
#define ASSIGNCONST         23  //��������ֵ
#define LEFTBRACELACK       24  //ȱ�١�{��
#define NONERETURN          25  //ȱ�ٷ���ֵ
#define PLUSMINULACK        26  //ȱ�١�+����-��
#define MAINLACK            27  //ȱ��main����
#define MOREWORD            28  //main���������ж����ַ�
#define CONSTNOTINIT        29    //����û�г�ʼ��

extern void error(int _errsig, int signal = 0);
extern int errnum;
#endif