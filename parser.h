#ifndef PARSER_H
#define PARSER_H
#include <iostream>
#include <string>
#include <fstream>
#include "QString"
#include "QVector"
using namespace std;
enum stmtTypes {IF, REPEAT, ASSIGN, READ, WRITE, ERROR};
typedef struct{
    bool Rect;
    int x;
    int y;
    string text;
    int parent;
}Node;
typedef struct{
    bool isFriend;
    int x1;
    int y1;
    int x2;
    int y2;
}Edge;
typedef struct{
    bool isFriend;
    int x;
    int y;
}parentNode;
class Parser
{
private:
    string token;
    ifstream file;
    QVector<Node>Nodes;
    QVector<Edge>Edges;
    Edge *currentEdge = NULL;
    int currentx=0;
    int currenty=0;
    QVector<parentNode> parentNodes;
    int counter = 0;
public:
    Parser();
    stmtTypes getStmtType(string s);
    void match(string s) ;
    void program();
    void stmt_seq();
    void stmt();
    void if_stmt();
    void repeat_stmt();
    void assign_stmt();
    void read_stmt();
    void write_stmt();
    void exp();
    void comparison_op();
    void simple_exp();
    void addop();
    void term();
    void mulop();
    void factor();
    void pre_update_edge();
    void post_update_edge(bool isFriend);
    void _3bas();
    void process_edges();
    QVector<Node> get_nodes();
    QVector<Edge> get_edges();
};

#endif // PARSER_H