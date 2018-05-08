#ifndef SOLVERCLASS_H
#define SOLVERCLASS_H

#include "calcparser_global.h"

#include <QObject>
#include <QString>

enum solver_enum {
    solve_add = 1,
    solve_sub = 2,
};

class CALCPARSERSHARED_EXPORT SolverClass
{
public:
    SolverClass(
        QString lhs_num = 0, QChar _lhs_op = '+',
        QString rhs_num = 0, QChar _rhs_op = '+', int _precision = 5,
        //
        solver_enum op  = solver_enum::solve_add );
    
    QChar   lhs_op;  // left  hand value operator
    QChar   rhs_op;  // right hand value operator

    QString result;  // holds the calc. result
    
    bool minus_flag = false;
    int  precision  = 5;
    
    QString reverse (QString);
    QString getMathOperation(QString,QString);
    
    QString addORsub(QString,QString);
    QString add(QString,QString);
    QString sub(QString,QString);
    QString mul(QString,QString);
    
};

#endif // SOLVERCLASS_H
