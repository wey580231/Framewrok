#ifndef RELATED_GLOBAL_H
#define RELATED_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(RELATED_LIBRARY)
#  define RELATEDSHARED_EXPORT Q_DECL_EXPORT
#else
#  define RELATEDSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // RELATED_GLOBAL_H
