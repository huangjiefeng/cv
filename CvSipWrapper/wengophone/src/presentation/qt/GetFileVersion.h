#ifndef __VERINFO_H__ 
#define __VERINFO_H__

#if defined( _MSC_VER ) && ( _MSC_VER >= 1020 )
	#pragma once
#endif

/////////////////////////////////////////////////////////////////////////////

#ifndef	_INC_SHLWAPI
	#include < Shlwapi.h >
#endif
#pragma comment( lib, "shlwapi.lib" )

#ifndef VER_H
	#include < WinVer.h >
#endif
#pragma comment( lib, "Version.lib " )

#ifndef _T
	#ifndef _INC_TCHAR
		#include < TChar.h >
	#endif
#endif

#ifndef	ASSERT
	#ifndef _INC_CRTDBG
		#include < CrtDbg.h >
	#endif
	#define ASSERT( x )			_ASSERTE( x )
#endif

/////////////////////////////////////////////////////////////////////////////

//#define _wslPackVersion( major, minor )		MAKELONG( minor, major )

#ifndef		_free
	#define _free( p )			{ if( p != NULL ){ free( p ); p = NULL; } }
#endif

#ifndef		ASSERT_RETURN
	#define ASSERT_RETURN( x )	{ ASSERT( 0 ); return x; }
#endif

/////////////////////////////////////////////////////////////////////////////

#ifndef DLLVER_MAJOR_MASK

	typedef struct _DLLVERSIONINFO2 {
		DLLVERSIONINFO	info1;
		DWORD			dwFlags;				// No flags currently defined
		ULONGLONG		ullVersion;
	} DLLVERSIONINFO2;

#endif // DLLVER_MAJOR_MASK

#ifndef MAKEDLLVERULL

	#define MAKEDLLVERULL( major, minor, build, qfe )\
		( ( (ULONGLONG)(major) << 48 ) |			 \
		(   (ULONGLONG)(minor) << 32 ) |			 \
		(   (ULONGLONG)(build) << 16 ) |			 \
		(   (ULONGLONG)(  qfe) <<  0 ) )

#endif // MAKEDLLVERULL


/////////////////////////////////////////////////////////////////////////////

STDAPI_( HRESULT ) DllGetVersion( IN HMODULE hModule, OUT DLLVERSIONINFO* lpDVI );

/////////////////////////////////////////////////////////////////////////////

typedef enum _VI_CP {
	VI_CP_ASCII				= 0,	// 7-bit ASCII 
	VI_CP_JAPAN				= 932,	// Japan (Shift - JIS X-0208) 
	VI_CP_KOREA				= 949,	// Korea (Shift - KSC 5601) 
	VI_CP_TAIWAN			= 950,	// Taiwan (Big5) 
	VI_CP_UNICODE			= 1200,	// Unicode 
	VI_CP_LATIN2			= 1250,	// Latin-2 (Eastern European) 
	VI_CP_CYRILLIC			= 1251,	// Cyrillic 
	VI_CP_MULTILNG			= 1252,	// Multilingual 
	VI_CP_GREEK				= 1253,	// Greek 
	VI_CP_TURKISH			= 1254,	// Turkish 
	VI_CP_HEBREW			= 1255,	// Hebrew 
	VI_CP_ARABIC			= 1256	// Arabic 
} VI_CP;

typedef enum _VI_STR {
	VI_STR_COMMENTS			= 0,	// Comments
	VI_STR_COMPANYNAME		= 1,	// CompanyName
	VI_STR_FILEDESCRIPTION	= 2,	// FileDescription
	VI_STR_FILEVERSION		= 3,	// FileVersion
	VI_STR_INTERNALNAME		= 4,	// InternalName
	VI_STR_LEGALCOPYRIGHT	= 5,	// LegalCopyright
	VI_STR_LEGALTRADEMARKS	= 6,	// LegalTrademarks
	VI_STR_ORIGINALFILENAME	= 7,	// OriginalFilename
	VI_STR_PRIVATEBUILD		= 8,	// PrivateBuild
	VI_STR_PRODUCTNAME		= 9,	// ProductName
	VI_STR_PRODUCTVERSION	= 10,	// ProductVersion
	VI_STR_SPECIALBUILD		= 11,	// SpecialBuild
	VI_STR_OLESELFREGISTER	= 12	// OLESelfRegister
} VI_STR;

/*
HIWORD( m_vsffi.dwFileVersionMS )
LOWORD( m_vsffi.dwFileVersionMS )
HIWORD( m_vsffi.dwFileVersionLS )
LOWORD( m_vsffi.dwFileVersionLS )
*/

class CGetFileVersion
{
public:	// Construction/destruction:

	CGetFileVersion( void );	
	virtual ~CGetFileVersion( void );

public:	// Implementation:
	
	BOOL	Open( IN LPCTSTR lpszFileName );
	BOOL	Open( IN HINSTANCE hInstance );
	void	Close( void );

	BOOL	QueryStringValue( IN LPCTSTR lpszString, OUT LPTSTR lpszValue, IN INT nBuf ) const;
	BOOL	QueryStringValue( IN INT nIndex, OUT LPTSTR lpszValue, IN INT nBuf ) const;
	LPCTSTR	GetVerStringName( IN INT nIndex );	

	BOOL	SetTrans ( IN LANGID wLID = LANG_NEUTRAL, IN WORD wCP = VI_CP_UNICODE );	
	BOOL	SetTransIndex( IN UINT nIndex = 0 );
	INT		FindTrans( IN LANGID wLID, IN WORD wCP ) const;
	DWORD	GetTransByIndex( IN UINT nIndex  ) const;	

public: // Static members:
	
	static BOOL		GetLIDName( IN WORD wLID, OUT LPTSTR lpszName, IN INT nBuf );
	static BOOL		GetCPName( IN WORD wCP, OUT LPCTSTR* ppszName );
	//static DWORD	InstallFile( void );
	
public: // Inline members

	inline const	VS_FIXEDFILEINFO& GetVSFFI( void ) const;		
	inline BOOL		IsValid( void ) const;

	inline WORD		GetFileVersionMajor( void ) const;
	inline WORD		GetFileVersionMinor( void ) const;
	inline WORD		GetFileVersionBuild( void ) const;
	inline WORD		GetFileVersionQFE( void )   const;

	inline WORD		GetProductVersionMajor( void ) const;
	inline WORD		GetProductVersionMinor( void ) const;
	inline WORD		GetProductVersionBuild( void ) const;
	inline WORD		GetProductVersionQFE( void )   const;
	
	inline UINT		GetTransCount( void ) const;
	inline UINT		GetCurTransIndex( void ) const;		
	inline LANGID	GetLIDByIndex( IN UINT nIndex ) const;	
	inline WORD		GetCPByIndex( IN UINT nIndex ) const;	
	inline DWORD	GetCurTrans( void ) const;		
	inline LANGID	GetCurLID( void ) const;	
	inline WORD		GetCurCP( void ) const;

protected:

	BOOL	GetVersionInfo( IN LPCTSTR lpszFileName );
	BOOL	QueryVersionTrans( void );

protected: // Members variables
	
	static LPCTSTR	 s_ppszStr[ 13 ];	// String names
	VS_FIXEDFILEINFO m_vsffi;			// Fixed File Info (FFI)

	LPBYTE		m_lpbyVIB;		// Pointer to version info block (VIB)
	LPDWORD		m_lpdwTrans;	// Pointer to translation array in m_lpbyVIB, LOWORD = LangID and HIWORD = CodePage	
	UINT		m_nTransCur;	// Current translation index
	UINT		m_nTransCnt;	// Translations count
	BOOL		m_bValid;		// Version info is loaded	
};

////////////////////////////////////////////////////////////////////////////////

inline BOOL CGetFileVersion::IsValid( void ) const
	{ return m_bValid; }

inline const VS_FIXEDFILEINFO& CGetFileVersion::GetVSFFI( void ) const
	{ return m_vsffi; }

////////////////////////////////////////////////////////////////////////////////

inline WORD CGetFileVersion::GetFileVersionMajor( void ) const
	{ ASSERT( m_bValid ); return HIWORD( m_vsffi.dwFileVersionMS ); }

inline WORD CGetFileVersion::GetFileVersionMinor( void ) const
	{ ASSERT( m_bValid ); return LOWORD( m_vsffi.dwFileVersionMS ); }

inline WORD CGetFileVersion::GetFileVersionBuild( void ) const
	{ ASSERT( m_bValid ); return HIWORD( m_vsffi.dwFileVersionLS ); }

inline WORD CGetFileVersion::GetFileVersionQFE( void ) const
	{ ASSERT( m_bValid ); return LOWORD( m_vsffi.dwFileVersionLS ); }

inline WORD CGetFileVersion::GetProductVersionMajor( void ) const
	{ ASSERT( m_bValid ); return HIWORD( m_vsffi.dwProductVersionMS ); }

inline WORD CGetFileVersion::GetProductVersionMinor( void ) const
	{ ASSERT( m_bValid ); return LOWORD( m_vsffi.dwProductVersionMS ); }

inline WORD CGetFileVersion::GetProductVersionBuild( void ) const
	{ ASSERT( m_bValid ); return HIWORD( m_vsffi.dwProductVersionLS ); }

inline WORD CGetFileVersion::GetProductVersionQFE( void ) const
	{ ASSERT( m_bValid ); return LOWORD( m_vsffi.dwProductVersionLS ); }

////////////////////////////////////////////////////////////////////////////////
// Translation functions

inline UINT CGetFileVersion::GetTransCount( void ) const
	{ ASSERT( m_bValid ); return m_nTransCnt; }

inline UINT CGetFileVersion::GetCurTransIndex( void ) const
	{ ASSERT( m_bValid ); return m_nTransCur; }

inline LANGID CGetFileVersion::GetLIDByIndex( IN UINT nIndex  ) const
	{ return LOWORD( GetTransByIndex( nIndex ) ); }	

inline WORD CGetFileVersion::GetCPByIndex( IN UINT nIndex  ) const
	{ return HIWORD( GetTransByIndex( nIndex ) ); }	

inline DWORD CGetFileVersion::GetCurTrans( void ) const
	{ return GetTransByIndex( GetCurTransIndex() ); }

inline LANGID CGetFileVersion::GetCurLID( void ) const
	{ return GetLIDByIndex( GetCurTransIndex() ); }

inline WORD CGetFileVersion::GetCurCP( void ) const
	{ return GetCPByIndex( GetCurTransIndex() ); }
	
/////////////////////////////////////////////////////////////////////////////
#endif //__VER_H__
/////////////////////////////////////////////////////////////////////////////
