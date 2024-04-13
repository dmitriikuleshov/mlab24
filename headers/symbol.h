#ifndef LW_SYMBOL_H
#define LW_SYMBOL_H


enum OperationType {
    MINUS,
    PLUS,
    MULT,
    DIV,
    POW,
    UNARY_MINUS
};


enum SymbolType {
    NONE,
    NUMBER,
    VARIABLE,
    OPERATION,
    LEFT_BR,
    RIGHT_BR,
};


typedef struct {
    enum SymbolType type;
    double val;
    char variable;
    enum OperationType operation;
} Symbol;


#endif //LW_SYMBOL_H