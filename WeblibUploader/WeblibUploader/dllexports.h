#ifndef DLL_EXPORTS_H
#define DLL_EXPORTS_H

#ifdef WEBLIB_TEST
#define WEBLIB_DLLSPEC
#else // WEBLIB_TEST

#ifdef WEBLIB_DLL
#define WEBLIB_DLLSPEC Q_DECL_EXPORT
#else
#define WEBLIB_DLLSPEC Q_DECL_IMPORT
#endif

#endif // WEBLIB_TEST


#endif
