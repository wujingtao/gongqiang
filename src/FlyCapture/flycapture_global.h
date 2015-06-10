#ifndef FLYCAPTURE_GLOBAL_H
#define FLYCAPTURE_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef FLYCAPTURE_LIB
# define FLYCAPTURE_EXPORT Q_DECL_EXPORT
#else
# define FLYCAPTURE_EXPORT Q_DECL_IMPORT
#endif

#endif // FLYCAPTURE_GLOBAL_H