#ifndef CALCPARSER_GLOBAL_H
#define CALCPARSER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CALCPARSER_LIBRARY)
#  define CALCPARSERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define CALCPARSERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // CALCPARSER_GLOBAL_H
