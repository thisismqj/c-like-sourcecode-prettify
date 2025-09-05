#include <stdio.h>
#include <stdlib.h>
#include "mpa.h"
SCRes *scRes;
Node::Node(): son{0}, bro{0}, fa{0} {}
Node::~Node() {
    if (son) {
        Node *p=son, *pp;
        while (p) {
            pp=p->bro;
            delete p;
            p=pp;
        }
    }
    if (fa) {
        Node *pS=fa->son;
        if (pS==this) {fa->son=pS->bro;}
        else {
            while (pS->bro!=this) pS=pS->bro;
            pS->bro=pS->bro->bro;
        }
    }
}
void Node::addChild(Node *chd) {
    Node *s=son;
    if (!s) {
        son = chd;
    } else {
        while (s->bro) s=s->bro;
        s->bro = chd;
    }

}
Node * Node::newChild() {
    Node *chd=new Node;
    addChild(chd);
    return chd;
}
void Node::preOrdTraverse(void (*fun)(Node *n, int d), int dep) {
    fun(this, dep);
    if (son) son->preOrdTraverse(fun, dep+1);
    if (bro) bro->preOrdTraverse(fun, dep);
}
void Node::postOrdTraverse(void (*fun)(Node *n, int d), int dep) {
    if (son) son->postOrdTraverse(fun, dep+1);
    fun(this, dep);
    if (bro) bro->postOrdTraverse(fun, dep);
}
static int parsePos;
static inline Token getToken() {return scRes->tokens[parsePos++];}
static void printInd(int cnt) {
    while (cnt-->0) printf("  ");
}
static void parseError(const char *s) {
    printf("Syntax error: %s\n", s);
}
bool parseArgs(Node *fa) {
    Node *n = new Node;
    n->typ = NodeType::args;
    int oldPos = parsePos;
    if (!parseExpr(n)) {
        fa->addChild(n);
        return 1;
    }
    while (1) {
        if (getToken()!=TOK_COMMA) {
            --parsePos;
            fa->addChild(n);
            return 1;
        }
        if (!parseExpr(n)) {
            parsePos=oldPos;
            parseError("expression expected!");
            delete n; return 0;
        }
    }
}
bool parseAtomExpr(Node *fa) {
    Node *n = new Node;
    n->typ = NodeType::atomExpr;
    int oldPos=parsePos;
    switch (getToken()) {
    case TOK_IDENT: {
        n->typ=NodeType::id;
        Token ttok = getToken();
        if (ttok==TOK_LP) {
            n->typ = NodeType::funcCall;
            Node *chd = n->newChild();
            chd->typ = NodeType::id;
            chd->tokenPos=parsePos-2;
            parseArgs(n);
            Token tttok = getToken();
            if (tttok==TOK_RP) {
                fa->addChild(n);
                return 1;
            } else {
                parsePos=oldPos;
                parseError("')' expected!");
                delete n; return 0;
            }
        } else --parsePos;
        n->tokenPos=parsePos-1;
        fa->addChild(n);
        return 1; }
        break;
    case TOK_SUB:
        n->typ=NodeType::neg;
        if (parseAtomExpr(n)) {
            fa->addChild(n);
            return 1;
        } else {
            parsePos = oldPos;
            parseError("atomic expression expected!");
            delete n; return 0;
        }
        break;
    case TOK_NOT:
        n->typ=NodeType::_not;
        if (parseAtomExpr(n)) {
            fa->addChild(n);
            return 1;
        } else {
            parsePos = oldPos;
            parseError("atomic expression expected!");
            delete n; return 0;
        }
        break;
    case TOK_LIT_INT:
        n->typ=NodeType::litInt;
        n->tokenPos=parsePos-1;
        fa->addChild(n);
        return 1;
        break;
    case TOK_LIT_CHAR:
        n->typ=NodeType::litChar;
        n->tokenPos=parsePos-1;
        fa->addChild(n);
        return 1;
        break;
    case TOK_LIT_FLOAT:
        n->typ=NodeType::litFloat;
        n->tokenPos=parsePos-1;
        fa->addChild(n);
        return 1;
        break;
    case TOK_LIT_STRING:
        n->typ=NodeType::litString;
        n->tokenPos=parsePos-1;
        fa->addChild(n);
        return 1;
        break;
    case TOK_LP:
        n->typ=NodeType::atomExpr;
        if (!parseExpr(n)) {
            --parsePos;
            parseError("expression expected!");
            delete n; parsePos=oldPos; return 0;
        }
        if (TOK_RP!=getToken()) {
            --parsePos;
            parseError("')' expected!");
            delete n; parsePos=oldPos; return 0;
        }
        fa->addChild(n);
        return 1;
    default:
        --parsePos;
        delete n;
        return 0;
    }
}
bool parseType(Node *fa) {
    Node *n = new Node;
    n->typ = NodeType::type;
    Token tok=getToken();
    switch (tok) {
    case TOK_INT:
        n->typ = NodeType::typeInt;
        fa->addChild(n);
        return 1;
        break;
    case TOK_FLOAT:
        n->typ = NodeType::typeFloat;
        fa->addChild(n);
        return 1;
        break;
    case TOK_CHAR:
        n->typ = NodeType::typeChar;
        fa->addChild(n);
        return 1;
        break;
    default:
        --parsePos;
        delete n;
        return 0;
    };
}
bool parseFac(Node *fa) {
    Node *n=new Node;
    n->typ=NodeType::fac;
    int oldPos=parsePos;
    if (!parseAtomExpr(n)) return 0;
    Node *p = n, *np;
    while (1) {
        Token tok=getToken();
        switch (tok) {
        case TOK_MUL:
            np = new Node;
            np->typ=NodeType::mulFac;
            np->addChild(p);
            if (!parseAtomExpr(np)) {
                parseError("atomic expression expected!");
                parsePos=oldPos; delete np; return 0;
            }
            p=np;
            break;
        case TOK_DIV:
            np = new Node;
            np->typ=NodeType::divFac;
            np->addChild(p);
            if (!parseAtomExpr(np)) {
                parseError("atomic expression expected!");
                parsePos=oldPos; delete np; return 0;
            }
            p=np;
            break;
        case TOK_MOD:
            np = new Node;
            np->typ=NodeType::modFac;
            np->addChild(p);
            if (!parseAtomExpr(np)) {
                parseError("atomic expression expected!");
                parsePos=oldPos; delete np; return 0;
            }
            p=np;
            break;
        default:
            --parsePos;
            fa->addChild(p);
            return 1;
            break;
        }
    }
    return 1;
}
bool parseAddExpr(Node *fa) {
    Node *n=new Node;
    n->typ=NodeType::addExpr;
    int oldPos=parsePos;
    if (!parseFac(n)) return 0;
    Node *p = n, *np;
    while (1) {
        Token tok=getToken();
        switch (tok) {
        case TOK_ADD:
            np = new Node;
            np->typ=NodeType::addAddExpr;
            np->addChild(p);
            if (!parseFac(np)) {
                parseError("factor expected!");
                parsePos=oldPos; delete np; return 0;
            }
            p=np;
            break;
        case TOK_SUB:
            np = new Node;
            np->typ=NodeType::subAddExpr;
            np->addChild(p);
            if (!parseFac(np)) {
                parseError("factor expected!");
                parsePos=oldPos; delete np; return 0;
            }
            p=np;
            break;
        default:
            --parsePos;
            fa->addChild(p);
            return 1;
            break;
        }
    }
    return 1;
}
bool parseRelExpr(Node *fa) {
    Node *n=new Node;
    n->typ=NodeType::relExpr;
    int oldPos=parsePos;
    if (!parseAddExpr(n)) return 0;
    Node *p = n, *np;
    while (1) {
        Token tok=getToken();
        switch (tok) {
        case TOK_LT:
            np = new Node;
            np->typ=NodeType::relLtExpr;
            np->addChild(p);
            if (!parseAddExpr(np)) {
                parseError("arimetic expression expected!");
                parsePos=oldPos; delete np; return 0;
            }
            p=np;
            break;
        case TOK_GT:
            np = new Node;
            np->typ=NodeType::relGtExpr;
            np->addChild(p);
            if (!parseAddExpr(np)) {
                parseError("arimetic expression expected!");
                parsePos=oldPos; delete np; return 0;
            }
            p=np;
            break;
        case TOK_LEQ:
            np = new Node;
            np->typ=NodeType::relLeqExpr;
            np->addChild(p);
            if (!parseAddExpr(np)) {
                parseError("arimetic expression expected!");
                parsePos=oldPos; delete np; return 0;
            }
            p=np;
            break;
        case TOK_GEQ:
            np = new Node;
            np->typ=NodeType::relGeqExpr;
            np->addChild(p);
            if (!parseAddExpr(np)) {
                parseError("arimetic expression expected!");
                parsePos=oldPos; delete np; return 0;
            }
            p=np;
            break;
        default:
            --parsePos;
            fa->addChild(p);
            return 1;
            break;
        }
    }
    return 1;
}
bool parseCondExpr(Node *fa) {
    Node *n=new Node;
    n->typ=NodeType::condExpr;
    int oldPos=parsePos;
    if (!parseRelExpr(n)) return 0;
    Node *p = n, *np;
    while (1) {
        Token tok=getToken();
        switch (tok) {
        case TOK_EQ:
            np = new Node;
            np->typ=NodeType::condEqExpr;
            np->addChild(p);
            if (!parseRelExpr(np)) {
                parseError("relative expression expected!");
                parsePos=oldPos; delete np; return 0;
            }
            p=np;
            break;
        case TOK_NEQ:
            np = new Node;
            np->typ=NodeType::condNeqExpr;
            np->addChild(p);
            if (!parseRelExpr(np)) {
                parseError("relative expression expected!");
                parsePos=oldPos; delete np; return 0;
            }
            p=np;
            break;
        default:
            --parsePos;
            fa->addChild(p);
            return 1;
            break;
        }
    }
    return 1;
}
bool parseAndExpr(Node *fa) {
    Node *n=new Node;
    n->typ=NodeType::andExpr;
    int oldPos=parsePos;
    if (!parseCondExpr(n)) return 0;
    Node *p = n, *np;
    while (1) {
        Token tok=getToken();
        switch (tok) {
        case TOK_LOGICAL_AND:
            np = new Node;
            np->typ=NodeType::andAndExpr;
            np->addChild(p);
            if (!parseCondExpr(np)) {
                parseError("conditional expression expected!");
                parsePos=oldPos; delete np; return 0;
            }
            p=np;
            break;
        default:
            --parsePos;
            fa->addChild(p);
            return 1;
            break;
        }
    }
    return 1;
}
bool parseOrExpr(Node *fa) {
    Node *n=new Node;
    n->typ=NodeType::orExpr;
    int oldPos=parsePos;
    if (!parseAndExpr(n)) return 0;
    Node *p = n, *np;
    while (1) {
        Token tok=getToken();
        switch (tok) {
        case TOK_LOGICAL_OR:
            np = new Node;
            np->typ=NodeType::orOrExpr;
            np->addChild(p);
            if (!parseAndExpr(np)) {
                parseError("conditional expression expected!");
                parsePos=oldPos; delete np; return 0;
            }
            p=np;
            break;
        default:
            --parsePos;
            fa->addChild(p);
            return 1;
            break;
        }
    }
    return 1;
}
bool parseExpr(Node *fa) {
    Node *n=new Node;
    n->typ=NodeType::expr;
    int oldPos=parsePos;
    if (!parseOrExpr(n)) return 0;
    Token tok=getToken();
    if (tok!=TOK_ASSIGN) {
        --parsePos;
        fa->addChild(n);
        return 1;
    } else {
        n->typ=NodeType::assignExpr;
        if (!parseExpr(n)) {
            parseError("expression expected!");
            parsePos=oldPos;
            delete n;
            return 0;
        } else {
            fa->addChild(n);
            return 1;
        }
    }
}
bool parseExprOpt(Node *fa) {
    Node *n=fa->newChild();
    n->typ=NodeType::exprOpt;
    parseExpr(n);
    return 1;
}
bool parseStmt(Node *fa) {
    Node *n=new Node();
    n->typ=NodeType::stmt;
    int oldPos=parsePos;
    Token tok=getToken();
    switch (tok) {
    case TOK_LBRACE:
        delete n;
        --parsePos;
        if (parseCompStmt(fa)) {
            return 1;
        } else {
            parsePos=oldPos;
            return 0;
        }
        break;
    case TOK_RETURN:
        n->typ=NodeType::returnStmt;
        if (parseExpr(n)) {
            Token ttok=getToken();
            if (ttok==TOK_SEMI) {fa->addChild(n); return 1;}
            else {
                parsePos=oldPos;
                parseError("';' expected!");
                delete n; return 0;
            }
        } else {
            parsePos=oldPos;
            parseError("expression expected!");
            delete n; return 0;
        }
        break;
    case TOK_IF: {
        n->typ=NodeType::ifStmt;
        Token ttok=getToken();
        if (ttok==TOK_LP) {
            if (parseExpr(n)) {
                Token tttok=getToken();
                if (tttok==TOK_RP) {
                    if (!parseStmt(n)) {
                        parsePos=oldPos;
                        parseError("statement expected!");
                        delete n; return 0;
                    } else {
                        Token ttttok=getToken();
                        if (ttttok==TOK_ELSE) {
                            n->typ=NodeType::ifElseStmt;
                            if (!parseStmt(n)) {
                                parsePos=oldPos;
                                parseError("statement expected!");
                                delete n; return 0;
                            }
                        } else --parsePos;
                    }
                    fa->addChild(n); return 1;
                } else {
                    parsePos=oldPos;
                    parseError("')' expected!");
                    delete n; return 0;
                }
            } else {
                parsePos=oldPos;
                parseError("expression expected!");
                delete n; return 0;
            }
        } else {
            parsePos=oldPos;
            parseError("'(' expected!");
            delete n; return 0;
        } }
        break;
    case TOK_WHILE: {
        n->typ=NodeType::whileStmt;
        Token ttok=getToken();
        if (ttok==TOK_LP) {
            if (parseExpr(n)) {
                Token tttok=getToken();
                if (tttok==TOK_RP) {
                    if (!parseStmt(n)) {
                        parsePos=oldPos;
                        parseError("statement expected!");
                        delete n; return 0;
                    }
                    fa->addChild(n); return 1;
                } else {
                    parsePos=oldPos;
                    parseError("')' expected!");
                    delete n; return 0;
                }
            } else {
                parsePos=oldPos;
                parseError("expression expected!");
                delete n; return 0;
            }
        } else {
            parsePos=oldPos;
            parseError("'(' expected!");
            delete n; return 0;
        } }
        break;
    case TOK_DO:
        n->typ=NodeType::doWhileStmt;
        if (!parseStmt(n)) {
            parsePos=oldPos;
            parseError("statement expected!");
            delete n; return 0;
        } else {
            Token ttok=getToken();
            if (ttok!=TOK_WHILE) {
                parsePos=oldPos;
                parseError("while expected!");
                delete n; return 0;
            } else {
                Token tttok=getToken();
                if (tttok==TOK_LP) {
                    if (parseExpr(n)) {
                        Token ttttok=getToken();
                        if (ttttok==TOK_RP) {
                            Token tttttok=getToken();
                            if (tttttok==TOK_SEMI) {
                                fa->addChild(n); return 1;
                            } else {
                                parsePos=oldPos;
                                parseError("';' expected!");
                                delete n; return 0;
                            }
                        } else {
                            parsePos=oldPos;
                            parseError("')' expected!");
                            delete n; return 0;
                        }
                    } else {
                        parsePos=oldPos;
                        parseError("expression expected!");
                        delete n; return 0;
                    }
                } else {
                    parsePos=oldPos;
                    parseError("'(' expected!");
                    delete n; return 0;
                }
            }
        }
        break;
    case TOK_FOR: {
        n->typ=NodeType::forStmt;
        Token ttok=getToken();
        if (ttok==TOK_LP) {
            parseExprOpt(n);
            Token tttok=getToken();
            if (tttok==TOK_SEMI) {
                parseExprOpt(n);
                Token ttttok=getToken();
                if (ttttok==TOK_SEMI) {
                    parseExprOpt(n);
                    Token tttttok=getToken();
                    if (tttttok==TOK_RP) {
                        if (!parseStmt(n)) {
                            parsePos=oldPos;
                            parseError("statement expected!");
                            delete n; return 0;
                        } else {
                            fa->addChild(n); return 1;
                        }
                    } else {
                        parsePos=oldPos;
                        parseError("')' expected!");
                        delete n; return 0;
                    }

                } else {
                    parsePos=oldPos;
                    parseError("';' expected!");
                    delete n; return 0;
                }
            } else {
                parsePos=oldPos;
                parseError("';' expected!");
                delete n; return 0;
            }
        } else {
            parsePos=oldPos;
            parseError("'(' expected!");
            delete n; return 0;
        }
        }
        break;
    default:
        --parsePos;
        if (parseExpr(n)) {
            Token ttok=getToken();
            if (ttok!=TOK_SEMI) {
                parsePos=oldPos;
                parseError("';' expected!");
                delete n; return 0;
            } else {
                fa->addChild(n);
                return 1;
            }
        } else {
            delete n; return 0;
        }
    }
}
bool parseStmts(Node *fa) {
    Node *n = new Node;
    n->typ = NodeType::stmts;
    while (parseStmt(n)) ;
    fa->addChild(n);
    return 1;
}
bool parseId(Node *fa) {
    Node *n = new Node;
    n->typ = NodeType::id;
    Token tok = getToken();
    if (tok==TOK_IDENT) {
        n->tokenPos=parsePos-1;
        fa->addChild(n);
        return 1;
    } else {
        delete n; --parsePos; return 0;
    }
}
bool parseParam(Node *fa) {
    Node *n = new Node;
    n->typ = NodeType::param;
    int oldPos=parsePos;
    if (parseType(n)) {
        if (parseId(n)) {
            fa->addChild(n); return 1;
        } else {
            parsePos=oldPos;
            parseError("identifier expected!");
            delete n; return 0;
        }
    } else {delete n; parsePos=oldPos; return 0;}
}
bool parseParams(Node *fa) {
    Node *n = new Node;
    int oldPos = parsePos;
    n->typ = NodeType::params;
    if (!parseParam(n)) {
        fa->addChild(n);
        return 1;
    }
    while (1) {
        if (getToken()!=TOK_COMMA) {
            --parsePos;
            fa->addChild(n);
            return 1;
        }
        if (!parseParam(n)) {
            parsePos=oldPos;
            parseError("param expected!");
            delete n; return 0;
        }
    }
}
bool parseVarSeq(Node *fa) {
    Node *n = new Node;
    n->typ = NodeType::varSeq;
    int oldPos = parsePos;
    if (!parseId(n)) return 0;
    while (1) {
        if (getToken()!=TOK_COMMA) {
            --parsePos;
            fa->addChild(n);
            return 1;
        } 
        if (!parseId(n)) {
            parsePos=oldPos;
            parseError("identifier expected!");
            delete n; return 0;
        }
    }
    return 1;
}
bool parseLocalVarDef(Node *fa) {
    Node *n = new Node;
    n->typ = NodeType::localVarDef;
    int oldPos=parsePos;
    if (!parseType(n)) {
        parsePos=oldPos;
        delete n; return 0;
    } else {
        if (!parseVarSeq(n)) {
            parsePos=oldPos;
            parseError("variable sequence expected!");
            delete n; return 0;
        } else {
            Token tok=getToken();
            if (tok!=TOK_SEMI) {
                parsePos=oldPos;
                parseError("';' expected!");
                delete n; return 0;
            }
            fa->addChild(n);
            return 1;
        }
    }
}
bool parseLocalVarDefs(Node *fa) {
    Node *n = new Node;
    n->typ = NodeType::localVarDefs;
    while (parseLocalVarDef(n))
        n->typ=NodeType::localVarDefs;
    fa->addChild(n);
    return 1;
}
bool parseCompStmt(Node *fa) {
    Token tok=getToken();
    int oldPos = parsePos;
    if (tok==TOK_LBRACE) {
        Node *n = new Node;
        n->typ = NodeType::compStmt;
        parseLocalVarDefs(n);
        parseStmts(n);
        Token ttok = getToken();
        if (ttok==TOK_RBRACE) {
            fa->addChild(n);
            return 1;
        } else {
            parsePos=oldPos;
            parseError("'}' expected!");
            delete n; return 0;
        }
    } else {--parsePos; return 0;}
}
bool parseExtVarDef(Node *fa) {
    Node *n = new Node;
    n->typ = NodeType::extVarDef;
    int oldPos=parsePos;
    if (!parseType(n)) {
        parsePos=oldPos;
        delete n; return 0;
    } else {
        if (!parseVarSeq(n)) {
            parsePos=oldPos;
            delete n; return 0;
        } else {
            Token tok=getToken();
            if (tok!=TOK_SEMI) {
                parsePos=oldPos;
                delete n; return 0;
            }
            fa->addChild(n);
            return 1;
        }
    }
}
bool parseFuncDef(Node *fa) {
    Node *n = new Node;
    n->typ = NodeType::funcDef;
    int oldPos = parsePos;
    if (!parseType(n)) {
        parsePos = oldPos;
        delete n; return 0;
    } else {
        if (!parseId(n)) {
            parsePos = oldPos;
            delete n; return 0;
        } else {
            Token tok = getToken();
            if (TOK_LP==tok) {
                parseParams(n);
                Token ttok = getToken();
                if (TOK_RP==ttok) {
                    if (!parseCompStmt(n)) {
                        parsePos = oldPos;
                        parseError("function body expected!");
                        delete n; return 0;
                    } else {
                        fa->addChild(n);
                        return 1;
                    }
                } else {
                    parsePos = oldPos;
                    parseError("')' expected!");
                    delete n; return 0;
                }
            } else {
                parsePos = oldPos;
                delete n; return 0;
            }
        }
    }
}
bool parseExtDef(Node *fa) {
    Node *n = new Node;
    n->typ = NodeType::extDef;
    if (parseExtVarDef(n)) {
        fa->addChild(n);
        return 1;
    } else if (parseFuncDef(n)) {
        fa->addChild(n);
        return 1;
    } else {
        delete n;
        return 0;
    }
}
bool parseSrc(Node *fa) {
    Node *n = new Node;
    n->typ = NodeType::src;
    while (parseExtDef(n)) ;
    fa->addChild(n);
    return 1;
}
