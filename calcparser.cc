#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/types.h>
#include <limits.h>

#include <QChar>
#include <QMessageBox>
#include "calcparser.h"

extern void yyset_in(FILE*);
extern int yyparse();
extern void get_stackResult(void);

extern QChar opstr;

CalcParser::CalcParser()
{
}

void setup_vars()
{
    opstr = '+';
}
void start_calculation(void)
{
    FILE *f = fopen("source.calc","r");
    if (f == nullptr) {
        QMessageBox::information(0,
        "Fehler !!!",
        "Die Berechnungs-Quelldatei fehlt bzw.\n"
        "konnte nicht geöffnet werden.");
        return;
    }
    
    yyset_in(f);
    
    setup_vars();
    yyparse();
    
    fclose(f);
    get_stackResult();
}
