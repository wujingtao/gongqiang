#ifndef MYDOCKWIDGET_GLOBAL_H
#define MYDOCKWIDGET_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef MYDOCK_LIB
# define MYDOCK_EXPORT Q_DECL_EXPORT
#else
# define MYDOCK_EXPORT Q_DECL_IMPORT
#endif

#endif // MYDOCKWIDGET_GLOBAL_H
