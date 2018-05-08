#include <QString>
#include <QMessageBox>
#include <QDebug>

#include "solverclass.h"

SolverClass::SolverClass(
    QString lhs_num, QChar _lhs_op,
    QString rhs_num, QChar _rhs_op, int _precision,
    solver_enum op)
    : lhs_op(_lhs_op)
    , rhs_op(_rhs_op) , precision(_precision)
{
    qDebug() << "start calculation, please wait...";

    QString lhs_res = lhs_num;
    QString rhs_res = rhs_num;

    if (!lhs_res.contains('.')) lhs_res.append('.');
    if (!rhs_res.contains('.')) rhs_res.append('.');

    int llen = lhs_res.indexOf('.');
    int rlen = rhs_res.indexOf('.');

    //qDebug() << "lenA:  " << lhs_res.length() << " p: " << llen;
    //qDebug() << "lenB:  " << rhs_res.length() << " p: " << rlen;

    if (rhs_res.length() - lhs_res.length() < 1)
    minus_flag = true ; else
    minus_flag = false;
    if (rlen-rhs_res.length() < 1)
    {
        for (int i = 0; i < (rlen-llen); ++i)
        lhs_res = lhs_res.insert(0,"0");
        int alen = (rhs_res.length() - llen);
        for (int i = 0; i < alen-4; ++i)
        rhs_res = rhs_res.append("0");
    }
    for (int i = 0; i < llen; ++i)
    rhs_res = rhs_res.append("0");

    //lhs_res = reverse(lhs_res);
    //rhs_res = reverse(rhs_res);

    //qDebug() << "lhs: " << lhs_res;
    //qDebug() << "rhs: " << rhs_res;

    if (op == solver_enum::solve_add) result = add(lhs_res,rhs_res); else
    if (op == solver_enum::solve_sub) result = sub(lhs_res,rhs_res);

    qDebug() << "result: " << result;
    qDebug() << "done.";
}

QString SolverClass::reverse(QString str)
{
    QString rev;
    QChar   c;
    int     len = str.length();
    for (int i = 0; i < len; ++i) {
        c = str.at(len-i-1);
        rev.append(c);
    }
    return rev;
}

// addition || subtraction
QString SolverClass::addORsub(QString lhs, QString rhs)
{
    QString nr;
    int p1, p2, p3, p6 = 0;

    struct struct_table {
        unsigned char  num_id;
        //
        unsigned char sub_2;
        unsigned char sub_3;
        unsigned char sub_mark;
        //
        unsigned char add_1;
        unsigned char add_mark;
        //
        unsigned char mul_1;
        unsigned char mul_mark;
        //
        QString div_1;
        unsigned char div_mark;
        //
        QString  sqrt_1;
        unsigned sqrt_mark;
    } struct_table[] = {
        0, 0, 0, 0,  0, 0,  0, 0,  "error" , 0,  "0.0000", 0,
        0, 1, 9, 1,  1, 0,  0, 0,  "0.0000", 0,  "1.0000", 0,
        0, 2, 8, 1,  2, 0,  0, 0,  "0.0000", 0,  "1.4142", 0,
        0, 3, 7, 1,  3, 0,  0, 0,  "0.0000", 0,  "1.7321", 0,
        0, 4, 6, 1,  4, 0,  0, 0,  "0.0000", 0,  "2.0000", 0,
        0, 5, 5, 0,  5, 0,  0, 0,  "0.0000", 0,  "2.2361", 0,
        0, 6, 4, 0,  6, 0,  0, 0,  "0.0000", 0,  "2.4495", 0,
        0, 7, 3, 0,  7, 0,  0, 0,  "0.0000", 0,  "2.6458", 0,
        0, 8, 2, 0,  8, 0,  0, 0,  "0.0000", 0,  "2.8284", 0,
        0, 9, 1, 0,  9, 0,  0, 0,  "0.0000", 0,  "3.0000", 0,
        //
        1, 0, 1, 0,  0, 1,  0, 0,  "error" , 0,  "3.1623", 0,
        1, 1, 0, 0,  1, 1,  1, 0,  "1.0000", 0,  "3.3166", 0,
        1, 2, 9, 1,  2, 1,  2, 0,  "0.5000", 0,  "3.4641", 0,
        1, 3, 8, 1,  3, 1,  3, 0,  "0.3333", 0,  "3.6056", 0,
        1, 4, 7, 1,  4, 1,  4, 0,  "0.2500", 0,  "3.7417", 0,
        1, 5, 6, 1,  5, 1,  5, 0,  "0.2000", 0,  "3.8730", 0,
        1, 6, 5, 1,  6, 1,  6, 0,  "0.1666", 0,  "4.0000", 0,
        1, 7, 4, 1,  7, 1,  7, 0,  "0.1429", 0,  "4.1231", 0,
        1, 8, 3, 1,  8, 1,  8, 0,  "0.1250", 0,  "4.2426", 0,
        1, 9, 2, 1,  9, 1,  9, 0,  "0.1111", 0,  "4.3589", 0,
        //
        2, 0, 2, 0,  0, 2,  0, 0,  "error" , 0,  "4.4721", 0,
        2, 1, 1, 0,  1, 2,  2, 0,  "2.0000", 0,  "4.5826", 0,
        2, 2, 0, 0,  2, 2,  4, 0,  "1.0000", 0,  "4.6904", 0,
        2, 3, 9, 1,  3, 2,  6, 0,  "0.6666", 0,  "4.7958", 0,
        2, 4, 8, 1,  4, 2,  8, 0,  "0.5000", 0,  "4.8990", 0,
        2, 5, 7, 1,  5, 2,  0, 1,  "0.4000", 0,  "5.0000", 0,
        2, 6, 6, 1,  6, 2,  2, 1,  "0.3333", 0,  "5.0990", 0,
        2, 7, 5, 1,  7, 2,  4, 1,  "0.2857", 0,  "5.1962", 0,
        2, 8, 4, 1,  8, 2,  6, 1,  "0.2500", 0,  "5.2915", 0,
        2, 9, 3, 1,  9, 2,  8, 1,  "0.2222", 0,  "5.3652", 0,
        //
        3, 0, 3, 0,  0, 3,  0, 0,  "error" , 0,  "5.4772", 9,
        3, 1, 2, 0,  1, 3,  3, 0,  "3.0000", 0,  "5.5678", 0,
        3, 2, 1, 0,  2, 3,  6, 0,  "1.5000", 0,  "5.6569", 0,
        3, 3, 0, 0,  3, 3,  9, 0,  "1.0000", 0,  "5.7446", 0,
        3, 4, 9, 1,  4, 3,  2, 1,  "0.7599", 0,  "5.8310", 0, 
        3, 5, 8, 1,  5, 3,  5, 1,  "0.6000", 0,  "5.9161", 0,
        3, 6, 7, 1,  6, 3,  6, 3,  "0.5000", 0,  "6.0000", 0,
        3, 7, 6, 1,  7, 3,  1, 2,  "0.4286", 0,  "6.0828", 0,
        3, 8, 5, 1,  8, 3,  4, 2,  "0.3750", 0,  "6.1644", 0,
        3, 9, 4, 1,  9, 3,  7, 2,  "0.3333", 0,  "6.2450", 0,
        //
        4, 0, 4, 0,  0, 4,  0, 0,  "error" , 0,  "6.3246", 0,
        4, 1, 3, 0,  1, 4,  4, 0,  "4.0000", 0,  "6.4031", 0,
        4, 2, 2, 0,  2, 4,  8, 0,  "2.0000", 0,  "6.4807", 0,
        4, 3, 1, 0,  3, 4,  2, 1,  "1.3333", 0,  "6.5574", 0,
        4, 4, 0, 0,  4, 4,  6, 1,  "1.0000", 0,  "6.6333", 0,
        4, 5, 9, 1,  5, 4,  0, 2,  "0.8000", 0,  "6.7082", 0,
        4, 6, 8, 1,  6, 4,  4, 2,  "0.6666", 0,  "6.7823", 0,
        4, 7, 7, 1,  7, 4,  8, 2,  "0.5714", 0,  "6.8557", 0,
        4, 8, 6, 1,  8, 4,  2, 3,  "0.5000", 0,  "6.9282", 0,
        4, 9, 5, 1,  9, 4,  6, 3,  "0.4444", 0,  "7.0000", 0,
        //
        5, 0, 5, 0,  0, 5,  0, 0,  "error" , 0,  "7.0711", 0,
        5, 1, 4, 0,  1, 5,  5, 0,  "5.0000", 0,  "7.1414", 0,
        5, 2, 3, 0,  2, 5,  0, 1,  "2.5000", 0,  "7.2111", 0,
        5, 3, 2, 0,  3, 5,  5, 1,  "1,6667", 0,  "7.2801", 0,
        5, 4, 1, 0,  4, 5,  0, 2,  "1.2500", 0,  "7.3485", 0,
        5, 5, 0, 0,  5, 5,  5, 2,  "1.0000", 0,  "7.4162", 0,
        5, 6, 9, 1,  6, 5,  0, 3,  "0.8333", 0,  "7.4833", 0,
        5, 7, 8, 1,  7, 5,  5, 3,  "0.7143", 0,  "7.5500", 0,
        5, 8, 7, 1,  8, 5,  0, 4,  "0.6250", 0,  "7.6158", 0,
        5, 9, 8, 1,  9, 5,  5, 4,  "0.5555", 0,  "7.6811", 0,
        //
        6, 0, 6, 0,  0, 6,  0, 0,  "error" , 0,  "7.7460", 0,
        6, 1, 5, 0,  1, 6,  6, 0,  "6.0000", 0,  "7.8103", 0,
        6, 2, 4, 0,  2, 6,  2, 1,  "3.0000", 0,  "7.8740", 0,
        6, 3, 3, 0,  3, 6,  8, 1,  "2.0000", 0,  "7.9373", 0,
        6, 4, 2, 0,  4, 6,  4, 2,  "1.5000", 0,  "8.0000", 0,
        6, 5, 1, 0,  5, 6,  0, 3,  "1.2000", 0,  "8.0623", 0,
        6, 6, 0, 0,  6, 6,  6, 3,  "1.0000", 0,  "8.1240", 0,
        6, 7, 9, 1,  7, 6,  2, 4,  "0.8571", 0,  "8.1854", 0,
        6, 8, 8, 1,  8, 6,  8, 4,  "0.7500", 0,  "8.2462", 0,
        6, 9, 7, 1,  9, 6,  4, 5,  "0.6666", 0,  "8.3066", 0,
        //
        7, 0, 7, 0,  0, 7,  0, 0,  "error" , 0,  "8.3666", 0,
        7, 1, 6, 0,  1, 7,  7, 0,  "7.0000", 0,  "8.4262", 0,
        7, 2, 5, 0,  2, 7,  4, 1,  "3.5000", 0,  "8.4853", 0,
        7, 3, 4, 0,  3, 7,  1, 2,  "2.3333", 0,  "8.5440", 0,
        7, 4, 3, 0,  4, 7,  8, 2,  "1.7500", 0,  "8.6023", 0,
        7, 5, 2, 0,  5, 7,  5, 3,  "1.4000", 0,  "8.6603", 0,
        7, 6, 1, 0,  6, 7,  2, 4,  "1.1667", 0,  "8.7178", 0,
        7, 7, 0, 0,  7, 7,  9, 4,  "1.0000", 0,  "8.7750", 0,
        7, 8, 9, 0,  8, 7,  6, 5,  "0.8750", 0,  "8.8318", 0,
        7, 9, 8, 0,  9, 7,  3, 6,  "0.7777", 0,  "8.8882", 0,
        //
        8, 0, 8, 0,  0, 8,  0, 0,  "0.0000", 0,  "8.9443", 0,
        8, 1, 7, 0,  1, 8,  8, 0,  "8.0000", 0,  "9.0000", 0,
        8, 2, 6, 0,  2, 8,  6, 1,  "4.0000", 0,  "9.0554", 0,
        8, 3, 5, 0,  3, 8,  4, 2,  "2.6667", 0,  "9.1104", 0,
        8, 4, 4, 0,  4, 8,  2, 3,  "2.0000", 0,  "9.1652", 0,
        8, 5, 3, 0,  5, 8,  0, 4,  "1.6000", 0,  "9.2195", 0,
        8, 6, 2, 0,  6, 8,  8, 4,  "1.3333", 0,  "", 0,
        8, 7, 1, 0,  7, 8,  6, 5,  "1.1429", 0,  "", 0,
        8, 8, 0, 0,  8, 8,  4, 6,  "1.0000", 0,  "", 0,
        8, 9, 9, 1,  9, 8,  2, 7,  "0.8888", 0,  "", 0,
        //
        9, 0, 9, 0,  0, 9,  0, 0,  "0.0000", 0,  "", 0,
        9, 1, 8, 0,  1, 9,  9, 0,  "9,0000", 0,  "", 0,
        9, 2, 7, 0,  2, 9,  8, 1,  "4.5000", 0,  "", 0,
        9, 3, 6, 0,  3, 9,  7, 2,  "3.0000", 0,  "", 0,
        9, 4, 5, 0,  4, 9,  6, 3,  "2.2500", 0,  "", 0,
        9, 5, 4, 0,  5, 9,  5, 4,  "1.8000", 0,  "", 0,
        9, 6, 3, 0,  6, 9,  4, 5,  "1.5000", 0,  "", 0,
        9, 7, 2, 0,  7, 9,  3, 6,  "1.2857", 0,  "", 0,
        9, 8, 1, 0,  8, 9,  2, 7,  "1.1250", 0,  "", 0,
        9, 9, 0, 0,  9, 9,  1, 8,  "1.0000", 0,  "", 0,
    };
    
    for (int p = 0; p < precision; ++p)
    {
        if (p >= lhs.length()) break;
        
        if (lhs.toLatin1().at(p) == '.'
        ||  rhs.toLatin1().at(p) == '.') {
            nr.append(QLatin1Char('.'));
            //qDebug() << "9nr: " << nr;
            continue;
        }
        
        QChar c1 = lhs.toLatin1().at(p);
        QChar c2 = rhs.toLatin1().at(p);
        
        if (!c1.isDigit() || !c2.isDigit()) {
            QMessageBox::information(0,
            "Error !!!",
            "char not digit.");
            return "";
        }
        
        p1 = QString(c1).toInt();
        p2 = QString(c2).toInt();
        
        //qDebug() << "p1: " << p1;
        //qDebug() << "p2: " << p2;

        if((lhs_op == '+' && rhs_op == '+')
        || (lhs_op == '-' && rhs_op == '-'))
        {
            p1 = p1 + p2;
            if (p1 == 0) {
                nr.append(QString::number(0));
                //qDebug() << "8nr: " << nr;
                p6 = 0;
            }
            else if (p1 == 10) {
                nr.append(QString::number(0));
                //qDebug() << "7nr: " << nr;
                p6 = 1;
            }
            else if (p1 > 10) {
                p1 = p1 + p6 - 10;
                p6 = 1;
                nr.append(QString::number(p1));
                //qDebug() << "6nr: " << nr;
            }
            else if (p1 < 10) {
                p1 = p1 + p6;
                p6 = 0;
                nr.append(QString::number(p1));
                //qDebug() << "5nr: " << nr;
            }
            
            if (lhs_op == '+' && rhs_op == '+') minus_flag = false; else
            if (lhs_op == '-' && rhs_op == '-') minus_flag = true ;
        }
        else
        if((lhs_op == '+' && rhs_op == '-')
        || (lhs_op == '-' && rhs_op == '+'))
        {
            if (p1 == 0 && p2 == 0) {
                if (p6 == 1)
                p3  = 9 ; else
                p3  = 0 ;
                nr.append(QString::number(p3));
                qDebug() << "Anr: " << nr;
            }
            else if (p1 == 2 && p2 == 5) {
                p3 = 10 - p2;
                p6 = 1;
                
                if (p3 + p6 > 9) {
                    p3 = 0;
                    p6 = 0;
                }
                else {
                    p3 = 10 - (10 - p2);
                    p6 = 0;
                }
                
                nr.append(QString::number(p3));
                qDebug() << "0nr: " << nr;
            }
            else if (p1 > 0 && p2 == 0) {
                p3 = 10 - p1;
                p6 = 1;
                nr.append(QString::number(p3));
                qDebug() << "4nr: " << nr;
            }
            else if (p1 > 0 && p2 > 0) {
                p3 = p1 - p2;
                nr.append(QString::number(p3));
                qDebug() << "4nr: " << nr;
            }
        }
    }
    
    nr = reverse(nr);
    nr.remove(QRegExp("^[0]*"));
    
    if (nr.trimmed().length() < 1)
    nr = "0";
    
    if (minus_flag == true)
    nr = nr.insert(0,"-");
    
    //qDebug() << "nr: " << nr;
    return nr;
}

QString SolverClass::getMathOperation(QString lhs, QString rhs)
{
    QString n1 = reverse(lhs);
    QString n2 = reverse(rhs);

    #if 0
    lhs_op = '+'; if (n1.contains("-")) lhs_op = '-';
    rhs_op = '+'; if (n2.contains("-")) rhs_op = '-';

    n1 = n1.remove(QRegExp("[+-]*"));
    n2 = n2.remove(QRegExp("[+-]*"));
    #endif
    
    if (lhs_op == '+' || rhs_op == '+') return addORsub(n1,n2);
    if (lhs_op == '-' || rhs_op == '-') return addORsub(n1,n2);
}

QString SolverClass::add(QString lhs, QString rhs) { return getMathOperation(lhs, rhs); }
QString SolverClass::sub(QString lhs, QString rhs) { return getMathOperation(lhs, rhs); }

// multiplication
QString SolverClass::mul(QString lhs, QString rhs)
{
    lhs = reverse(lhs);
    rhs = reverse(rhs);
    
    QStringList container;
    
    QString nr;
    int p1, p2;

    lhs_op = '*'; if (lhs.contains("*")) lhs_op = '*';
    rhs_op = '*'; if (rhs.contains("*")) rhs_op = '*';

    lhs = lhs.remove(QRegExp("[\\*]*"));
    rhs = rhs.remove(QRegExp("[\\*]*"));

    qDebug() << lhs;
    qDebug() << rhs;
    
    for (int p = 0; p < precision; ++p)
    {
        if (p >= lhs.size()) break;
        
        if (lhs.toLatin1().at(p) == '.'
        ||  rhs.toLatin1().at(p) == '.') {
            nr.append(QLatin1Char('.'));
            continue;
        }
        
        QChar c1 = lhs.toLatin1().at(p);
        QChar c2 = rhs.toLatin1().at(p);
        
        if (!c1.isDigit() || !c2.isDigit()) {
            qDebug() << "Error: char not digit.";
            exit(1);
        }
        
        p1 = QString(c1).toInt();
        p2 = QString(c2).toInt();
    
        if (p1 >= 0 && p2 == 0
        ||  p1 == 0 && p2 >= 1) {
            p1 = 0;
            nr.append(QString::number(p1));
            qDebug() << "mul01: " << nr;
        }
        else if (p1 > 0 && p2 > 0) {
            p1 = p1 * p2;
            nr.append(QString::number(p1));
            nr.append(" ");
            qDebug() << "mul02: " << nr;
        }
    }
    return nr;
}
