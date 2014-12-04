#ifndef MAIA_GLOBAL_H
#define MAIA_GLOBAL_H

// CORE includes
#include <QtCore/qglobal.h>

#if defined(MAIA_BUILD_SHARED)
#  define MAIASHARED_EXPORT Q_DECL_EXPORT
#elif defined(MAIA_USING_SHARED)
#  define MAIASHARED_EXPORT Q_DECL_IMPORT
#else
#  define MAIASHARED_EXPORT
#endif

#endif // MAIA_GLOBAL_H
