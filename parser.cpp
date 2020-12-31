#include "parser.h"
#include "QDebug"
Parser::Parser()
{

}
int c =0;
stmtTypes Parser::getStmtType(string s)
{
    s = s.substr(s.find(",") + 1, s.length() - s.find(",")+1);
    qDebug() << QString::fromStdString( s)<<"\n";
    if (s == "IF")
        return IF;
    if (s == "IDENTIFIER")
        return ASSIGN;
    if (s == "READ")
        return READ;
    if (s == "WRITE")
        return WRITE;
    if (s == "REPEAT")
        return REPEAT;
    else
    {
        cout << s << "\n";
        return ERROR;
    }
}
void  Parser::match(string s)
{
    if (s == token)
    {
        file >> token;
    }
    else
    {
        cout << endl
             << "-- Program not found" << endl;
        //exit(1);
    }
}
// program -> stmt-seq
void  Parser::program()
{
    stmt_seq();
    cout << endl
         << "-- Program found" << endl;
}
// stmt-seq -> stmt {; stmt}
void  Parser::stmt_seq()
{
    stmt();
    while (token == ";,SEMICOLON")
    {
        match(";,SEMICOLON");
        stmt();
    }
}
// stmt -> if-stmt | repeat-stmt | assign-stmt | read-stmt | write-stmt
void  Parser::stmt()
{
    switch (getStmtType(token))
    {
    case READ:
        read_stmt();
        break;
    case WRITE:
        write_stmt();
        break;
    case IF:
        if_stmt();
        break;
    case REPEAT:
        repeat_stmt();
        break;
    case ASSIGN:
        assign_stmt();
        break;
    case ERROR:

        cout << "no stmt found\n";
        //exit(1);
        break;
    default:
        break;
    }
}

// if-stmt -> if exp then stmt-seq [else stmt-seq] end
void  Parser::if_stmt()
{
    match("if,IF");
    cout << "if" << " -> ";
    pre_update_edge();
    Nodes.append({true,currentx,currenty,"if"});
    post_update_edge(false);
    currenty+=100;
    exp();
    match("then,THEN");
    currentx += 150;
    stmt_seq();
    if (token == "else,ELSE")
        stmt_seq();
    match("end,END");
    cout << "- if statement found" << endl;
    currenty -=100;
    currentx -=150;

}

// repeat-stmt -> repeat stmt-seq until exp
void  Parser::repeat_stmt()
{
    match("repeat,REPEAT");
    pre_update_edge();
    Nodes.append({true,currentx,currenty,"repeat"});
    post_update_edge(false);
    currenty+=100;
    cout << "repeat - > ";
    stmt_seq();
    cout << "\n     ->";
    match("until,UNTIL");
    exp();
    currentx +=150;
    cout << "- repeat found" << endl;
    currenty -=100;
//    currentx -=150;
}

// assign-stmt -> identifier := exp
void  Parser::assign_stmt()
{
    pre_update_edge();
    Nodes.append({true,currentx,currenty,"assign\n   "+token.substr(0, token.find(","))});
    post_update_edge(false);
    cout << "assign ";
    cout << token.substr(0, token.find(",")) << " -> ";
    match(token.substr(0, token.find(",")) + ",IDENTIFIER");
    match(":=,ASSIGN");
    exp();
    cout << "\n";
    cout << "- assignment found" << endl;
    currentx +=150;
}

// read-stmt -> read identifier
void  Parser::read_stmt()
{
    pre_update_edge();
    Nodes.append({true,currentx,currenty,"read\n"});
    post_update_edge(true);
    currentx += 150;
    match("read,READ");
    cout << "read -> ";
    cout << token.substr(0, token.find(",")) << "\n";
    token.erase(0, token.find(",") + 1);
    match("IDENTIFIER");
    cout << "- read found" << endl;
}

// write-stmt -> write exp
void  Parser::write_stmt()
{
    pre_update_edge();
    Nodes.append({true, currentx, currenty, "write"});
    post_update_edge(false);
//    currenty +=100;
    match("write,WRITE");
    cout << "write -> ";
    exp();
    cout << "\n";
    cout << "- write found" << endl;
//    currenty -=100;
}
// exp -> simple-exp [comparison-op simple-exp]
void  Parser::exp()
{
    c=0;
    currenty +=100;
    simple_exp();
    if (token == "<,LESSTHAN" || token == "=,EQUAL")
    {
        currenty -= 100;
        comparison_op();
        currenty +=100;
        currentx += 150;
        simple_exp();
        Node tmp1 = Nodes.back();
        Nodes.pop_back();
        Node tmp2 = Nodes.back();
        Nodes.pop_back();
        Node tmp3 = Nodes.back();
        Nodes.pop_back();
        Nodes.push_back(tmp2);
        Nodes.push_back(tmp1);
        Nodes.push_back(tmp3);
    }
    currenty -= 100;
}

// comparison-op -> < | =
void  Parser::comparison_op()
{
    if (token == "<,LESSTHAN")
    {
        pre_update_edge();
        Nodes.append({false, currentx, currenty, "op\n(<)"});
        post_update_edge(false);
        match("<,LESSTHAN");
        cout << " < -> ";
    }
    if (token == "=,EQUAL")
    {
        pre_update_edge();
        Nodes.append({false,currentx,currenty,"op\n(=)"});
        post_update_edge(false);
        match("=,EQUAL");
        cout << " = -> ";
    }
}

// simple-exp -> term { addop term }
void  Parser::simple_exp()
{
//    string s;
//    string s2;
//    file >> s;
//    if(s == "+,PLUS" || s == "-,MINUS")
//        currenty +=100;
//    ofstream file2;
//    file2.open("/media/sf_C_DRIVE/Users/moham/QT/Tiny-Compiler/parser_input.txt");
//    getline(file,s2 );
//    s = s+s2;
//    file2 << s;
    currenty += 100;
    term();
    while (token == "+,PLUS" || token == "-,MINUS")
    {
        c++;
        currenty -= 100;
        addop();
        currenty +=100;
        currentx += 150;
        term();
        Node tmp1 = Nodes.back();
        Nodes.pop_back();
        Node tmp2 = Nodes.back();
        Nodes.pop_back();
        Node tmp3 = Nodes.back();
        Nodes.pop_back();
        Nodes.push_back(tmp2);
        Nodes.push_back(tmp1);
        Nodes.push_back(tmp3);
    }
    currenty -= 100;

}


// addop -> + | -
void  Parser::addop()
{
    if (token == "+,PLUS")
    {
        pre_update_edge();
        Nodes.append({false,currentx,currenty,"op\n(+)"});
        post_update_edge(false);
        match("+,PLUS");
        cout << " + -> ";
    }
    if (token == "-,MINUS")
    {
        pre_update_edge();
        Nodes.append({false,currentx,currenty,"op\n(-)"});
        post_update_edge(false);
        match("-,MINUS");
        cout << " - -> ";
    }
}

// term -> factor { mulop factor }
void  Parser::term()
{
    currenty+=100;
    factor();
    while (token == "*,MULT" || token == "/,DIV")
    {
        c++;
        currenty-=100;
        mulop();
        currenty +=100;
        currentx += 150;
        factor();
        Node tmp1 = Nodes.back();
        Nodes.pop_back();
        Node tmp2 = Nodes.back();
        Nodes.pop_back();
        Node tmp3 = Nodes.back();
        Nodes.pop_back();
        Nodes.push_back(tmp2);
        Nodes.push_back(tmp1);
        Nodes.push_back(tmp3);
    }
    currenty -= 100;
}

// mulop -> * | /
void  Parser::mulop()
{
    if (token == "*,MULT")
    {
        currenty -= 100;
        pre_update_edge();
        Nodes.append({false,currentx,currenty,"op\n(*)"});
        post_update_edge(false);
        currenty += 100;
        match("*,MULT");
        cout << " * -> ";
    }
    if (token == "/,DIV")
    {
        pre_update_edge();
        Nodes.append({false,currentx,currenty,"op\n(/)"});
        post_update_edge(false);
        match("/,DIV");
        cout << " / -> ";
    }
}

// factor -> (exp) | number | identifier
void Parser:: factor()
{

    if (token == "(")
    {
        match("(,OPENBRACKET");
        exp();
        match("),CLOSEDBRACKET");
    }
    else if (token.substr(token.find(",") + 1, token.length() -1) == "NUMBER")
    {
        pre_update_edge();
        Nodes.append({false,currentx,currenty,"NUMBER\n"+token.substr(0, token.find(","))});
        cout << token.substr(0, token.find(",")) << " -> ";
        match(token.substr(0, token.find(",")) + ",NUMBER");
    }
    else if (token.substr(token.find(",") + 1, token.length() - token.find(",") + 1) == "IDENTIFIER")
    {
        pre_update_edge();
        Nodes.append({false,currentx,currenty,"IDENTIFIER\n"+token.substr(0, token.find(","))});
        //currenty -=100;
        cout << token.substr(0, token.find(",")) << " -> ";
        match(token.substr(0, token.find(",")) + ",IDENTIFIER");
    }
}

void Parser::_3bas()
{
    file.open("/media/sf_C_DRIVE/Users/moham/QT/Tiny-Compiler/parser_input.txt");

    file >> token;
    cout << token ;
    program();
    process_edges();
    file.close();
}

void Parser::process_edges(){
    int len = Nodes.length();
    for(int i = 0; i < len; i++){
        for(int j = 0; j < len; j++){
            if (j != i){
                if(Nodes[i].Rect && Nodes[j].Rect)
                    if(Nodes[i].y == Nodes[j].y)
                        if((Nodes[i].x - Nodes[j].x) == 150)
                            Edges.append({true, Nodes[j].x, Nodes[j].y, Nodes[i].x, Nodes[i].y});
                        else if((Nodes[j].x - Nodes[i].x) == 150)
                            Edges.append({true, Nodes[i].x, Nodes[i].y, Nodes[j].x, Nodes[j].y});
            }
        }
    }
}

void Parser::pre_update_edge(){
    if (currentEdge != NULL){
        currentEdge->x2 = currentx;
        currentEdge->y2 = currenty;
        Edges.append(*currentEdge);
        currentEdge = NULL;
    }
}

void Parser::post_update_edge(bool isFriend){
    currentEdge = new Edge;
    currentEdge->isFriend = isFriend;
    currentEdge->x1 = currentx;
    currentEdge->y1 = currenty;
}

QVector<Node> Parser::get_nodes()
{
    return Nodes;
}

QVector<Edge> Parser::get_edges()
{
    return Edges;
}