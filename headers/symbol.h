#ifndef C_LABS_SYMBOL_H
#define C_LABS_SYMBOL_H


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


#endif //C_LABS_SYMBOL_H