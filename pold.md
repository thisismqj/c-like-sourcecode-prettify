src: extDef src | extDef

extDef: extVarDef | funcDef

extVarDef: type varSeq ;

type: int | float | char

varSeq: id, varSeq | id

funcDef: type id ( params ) compStmt

params: param, params | empty

param: type id

funcCall: id ( args )

args: expr, args | empty

stmt: expr ; | compStmt | return expr ; | if ( expr ) stmt | if ( expr ) stmt else stmt |
        while (expr) stmt | do stmt while ( expr ) ; | for (exprOpt; exprOpt; exprOpt) stmt

compStmt: { localVarDefs stmts }

localVarDefs: localVarDef localVarDefs | empty

localVarDef: type varSeq ;

stmts: stmt stmts | empty

expr: orExpr = expr | orExpr

orExpr: andExpr || orExpr | andExpr

andExpr: condExpr && andExpr | condExpr

condExpr: relExpr == condExpr | relExpr != condExpr | relExpr

relExpr: addExpr < relExpr | addExpr <= relExpr | addExpr > relExpr | addExpr >= relExpr | relExpr

addExpr: fac + addExpr | fac - addExpr | fac

fac: atomExpr * fac | atomExpr / fac | atomExpr % fac | atomExpr

atomExpr: id | litInt | litChar | litFloat | litString | funcCall | ( expr ) | -atomExpr | !atomExpr
