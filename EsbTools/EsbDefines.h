#ifndef ESBROOT_ESBTOOLS_ESBMACROS_H
#define ESBROOT_ESBTOOLS_ESBMACROS_H 1

#include <sstream>

#define THROW(msg)\
        std::ostringstream strb; \
        strb << msg << " file: " << __FILE__ << " line: " << __LINE__ << endl;\
        cerr << strb.str() << endl;\
        throw invalid_argument(strb.str()); \


#endif