#ifndef MDEFS_H
#define MDEFS_H
const int SYMTABLE_SIZE=20480, TOKEN_SIZE=5120;
enum Token {
    // 基础类型
    TOK_INT,        // int 关键字
    TOK_FLOAT,      // float 关键字
    TOK_CHAR,       // char 关键字
    TOK_VOID,       // void 关键字
    // 控制流
    TOK_IF,         // if 关键字
    TOK_ELSE,       // else 关键字
    TOK_WHILE,      // while 关键字
    TOK_FOR,        // for 关键字
    TOK_RETURN,     // return 关键字
    TOK_DO,         // do 关键字
    TOK_BREAK,      // break 关键字
    TOK_CONTINUE,   // continue 关键字
    // 运算符
    TOK_EQ,         // == 运算符
    TOK_ASSIGN,     // = 赋值
    TOK_ADD,        // + 加法
    TOK_SUB,        // - 减法/负号
    TOK_MUL,        // * 乘法
    TOK_DIV,        // / 除法
    TOK_MOD,        // % 取模
    TOK_NEQ,        // != 不等于
    TOK_LT,         // < 小于
    TOK_GT,         // > 大于
    TOK_LEQ,        // <= 小于等于
    TOK_GEQ,        // >= 大于等于
    TOK_LOGICAL_AND,// && 逻辑与
    TOK_LOGICAL_OR, // || 逻辑或
    TOK_NOT,        // ! 逻辑非
    
    // 符号
    TOK_LP,         // ( 左括号
    TOK_RP,         // ) 右括号
    TOK_LBRACE,     // { 左大括号
    TOK_RBRACE,     // } 右大括号
    TOK_LBRACKET,   // [ 左方括号
    TOK_RBRACKET,   // ] 右方括号
    TOK_SEMI,       // ; 分号
    TOK_COMMA,      // , 逗号
    TOK_DOT,        // . 结构体成员访问 (待实现)
    TOK_ARROW,      // -> 指针成员访问
    // 字面量
    TOK_LIT_INT,    // 整数字面量（如 123）
    TOK_LIT_FLOAT,  // 浮点字面量（如 3.14）
    TOK_LIT_CHAR,   // 字符字面量（如 'a'）
    TOK_LIT_STRING, // 字符串字面量（如 "hello"）
    // 其他
    TOK_IDENT,      // 标识符（如变量名、函数名）
    TOK_SPACE,      // 空格/换行/制表符（可选，取决于词法分析器是否需要处理）
    TOK_EOF,         // 文件结束标记
    TOK_ERROR
} ;
const char tokenName[][20] = {
    // 基础类型
    "int",          // TOK_INT
    "float",        // TOK_FLOAT
    "char",         // TOK_CHAR
    "void",         // TOK_VOID
    // 控制流
    "if",           // TOK_IF
    "else",         // TOK_ELSE
    "while",        // TOK_WHILE
    "for",          // TOK_FOR
    "return",       // TOK_RETURN
    "do",           // TOK_DO
    "break",        // TOK_BREAK
    "continue",     // TOK_CONTINUE
    // 运算符
    "==",           // TOK_EQ
    "=",            // TOK_ASSIGN
    "+",            // TOK_ADD
    "-",            // TOK_SUB
    "*",            // TOK_MUL
    "/",            // TOK_DIV
    "%",            // TOK_MOD
    "!=",           // TOK_NEQ
    "<",            // TOK_LT
    ">",            // TOK_GT
    "<=",           // TOK_LEQ
    ">=",           // TOK_GEQ
    "&&",           // TOK_LOGICAL_AND
    "||",           // TOK_LOGICAL_OR
    "!",            // TOK_NOT
    // 符号
    "(",            // TOK_LP
    ")",            // TOK_RP
    "{",            // TOK_LBRACE
    "}",            // TOK_RBRACE
    "[",            // TOK_LBRACKET
    "]",            // TOK_RBRACKET
    ";",            // TOK_SEMI
    ",",            // TOK_COMMA
    ".",            // TOK_DOT
    "->",           // TOK_ARROW
    // 字面量
    "int_literal",  // TOK_LIT_INT
    "float_literal",// TOK_LIT_FLOAT
    "char_literal", // TOK_LIT_CHAR
    "string_literal",// TOK_LIT_STRING
    // 其他
    "identifier",   // TOK_IDENT
    "space",        // TOK_SPACE
    "EOF",          // TOK_EOF
    "error"         // TOK_ERROR
} ;

#endif
