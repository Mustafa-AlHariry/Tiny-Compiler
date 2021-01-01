#include "scanner.h"
Scanner::Scanner()
{
}
QString Scanner::getStringFile(QString directory)
{
    QFile file(directory);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
    }
    QString input;
    QTextStream in(&file);
    QString line = in.readLine();
    while (!line.isNull()) {
        input += line ;
        line = in.readLine();
        input += "\n";
    }
    return (input);
}
QString Scanner::getToken(string l)
{
    QString result = "";
    string mytoken;
    if(l.length() == 0)
        return "";
    bool res_flag = 0;
    int i = 0;
    QString x;
    while (state != DONE)
    {
        switch (state)
        {
        case START:
            if (isSpace(l[i]))
            {
                i++;
                if (i == l.length())
                    state = DONE;
                else
                    state = START;
            }
            else if (isDigit(l[i]))
            {
                state = INNUM;
            }
            else if (isLetter(l[i]))
            {
                state = INID;
            }
            else if (l[i] == ':')
            {
                state = INASSIGN;
            }
            else if (l[i] == '{')
            {
                i++;
                state = INCOMMENT;
            }
            else if (isSymbol(l[i]))
            {
                switch (l[i])
                {
                case ';':
                    result +=";,SEMICOLON\n";
                    break;
                case '(':
                    result +="(,OPENBRACKET\n";
                    break;
                case ')':
                    result +="),CLOSEDBRACKET\n";
                    break;
                case '+':
                     result +="+,PLUS\n";
                    break;
                case '-':
                     result +="-,MINUS\n";
                    break;
                case '/':
                     result +="/,DIV\n";
                    break;
                case '*':
                     result +="*,MULT\n";
                    break;
                case '=':
                     result +="=,EQUAL\n";
                    break;
                case '<':
                    result +="<,LESSTHAN\n";
                    break;
                default:
                    qDebug() << l[i] << " , Symbol \n ";
                    break;
                }
                i++;
                if (i == l.length())
                    state = DONE;
                else
                    state = START;
            }
            else
                state = DONE;
            break;

        case INCOMMENT:
            if (state == INCOMMENT)
            {
                while (l[i] != '}')
                {
                    i++;
                }
                i++;
                if (i == l.length())
                    state = DONE;
                else
                    state = START;
            }
            break;
        case INNUM:
            while (isDigit(l[i]))
            {
                mytoken += l[i];
                i++;
            }
            result += QString::fromStdString(mytoken) + ",NUMBER\n";
            mytoken = "";
            if (i == l.length())
                state = DONE;
            else
                state = START;
            break;
        case INID:
            while (isLetter(l[i]))
            {
                mytoken += l[i];
                i++;
            }

            for (int i = 0; i < 8; i++)
            {
                if (RES_WORDS[i] == mytoken)
                {
                    res_flag = 1;
                    x = QString::fromStdString(RES_WORDS[i]).toUpper();
                }
            }
            if (res_flag)
            {
                result += QString::fromStdString(mytoken) + ","+x+"\n";

            }
            else
            {
                result +=  QString::fromStdString(mytoken) + ",IDENTIFIER\n";

            }
            mytoken = "";
            res_flag = 0;
            if (i == l.length())
                state = DONE;
            else
                state = START;
            break;
        case INASSIGN:
            if (l[i] == ':')
            {
                i += 2;
                result +=  ":=,ASSIGN\n";
                state = START;
            }
            else
            {
                if (i == l.length())
                    state = DONE;
                else
                    state = START;
            }
            break;
        case DONE:
            break;
        }
    }
    state = START;
    return result;
}

bool Scanner::isDigit(char d) { return (d >= '0' && d <= '9'); }

bool Scanner::isLetter(char l) { return (l >= 'a' && l <= 'z' || l >= 'A' && l <= 'Z'); }

bool Scanner::isSpace(char s) { return (s == ' ' || s == '\t' || s == '\n'); }

bool Scanner::isSymbol(char c) { return (c == '+' || '-' || '*' || '=' || '<' || '/' || '(' || ')' || ';'); }

