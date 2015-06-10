#ifndef EXTSERIALPORT_GLOBAL_H
#define EXTSERIALPORT_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef EXTSERIALPORT_LIB
# define EXTSERIALPORT_EXPORT Q_DECL_EXPORT
#else
# define EXTSERIALPORT_EXPORT Q_DECL_IMPORT
#endif

#endif // EXTSERIALPORT_GLOBAL_H
