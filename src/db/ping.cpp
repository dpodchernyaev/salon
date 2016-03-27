#include <QDebug>
#include <QUrl>

#ifdef Q_OS_WIN
	#define __INTERLOCKED_DECLARED /* чтобы не было проблем с новым MinGW */
	#include <winsock.h>
	#include <windowsx.h>
	#undef __INTERLOCKED_DECLARED
#else
	#include <QProcess>
#endif

#include "db/ping.h"

#ifdef Q_OS_WIN
	HINSTANCE Ping::hIcmp = 0;
#endif

/**
 * @brief Ping::Ping
 * Конструктор
 * @param timeOut таймаут
 */
Ping::Ping(const uint timeOut)
{
	timeOutMs = timeOut;

#ifdef Q_OS_WIN
	WSAData wsaData;
	if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0)
	{
		qDebug() << "WSAStartup error!" ;
		return;
	}

	if (hIcmp == 0)
	{
		hIcmp = LoadLibrary("ICMP.DLL");
	}
#endif
}

/**
 * @brief Ping::~Ping
 * Деструктор
 */
Ping::~Ping()
{
#ifdef Q_OS_WIN
	WSACleanup();
#endif
}

/**
 * @brief Ping::doPing
 * пинг до сервера
 * @note unix -> запускается ping
 *		 windows -> http://tangentsoft.net/wskfaq/examples/dllping.html
 * @param srvHostStr ip сервера
 * @return сервер пингуется??
 */
bool
Ping::doPing(const QString &srvHostStr)
{
#ifdef Q_OS_WIN
	return doPingWin(srvHostStr);
#else
	return doPingLin(srvHostStr);
#endif
}

/**
 * @brief Ping::doPingUrl
 * Пинг полной url строки
 * @param url
 * @param timeOut
 * @return сервер пингуется??
 */
bool
Ping::doPingUrl(const QString &url,
			   const uint timeOut)
{
	if (timeOutMs != timeOut)
	{
		timeOutMs = timeOut;
	}

	return doPing(QUrl(url).host());
}

#ifdef Q_OS_WIN
/**
 * @brief Ping::doPingWin
 * Пинг под windows
 * @param srvHostStr хост
 * @return сервер пингуется??
 */
bool
Ping::doPingWin(const QString &srvHostStr)
{
	// Check ICMP.DLL
	if (hIcmp == 0)
	{
		qWarning() << "Unable to locate ICMP.DLL!" ;
		return false;
	}

	// Look up an IP address for the given host name
	struct hostent* phe;
	if ((phe = gethostbyname(srvHostStr.toUtf8().data())) == 0)
	{
		qWarning() << "Could not find IP address for " <<
					  srvHostStr.toUtf8().data();
		return false;
	}

	// Get handles to the functions inside ICMP.DLL that we'll need
	typedef HANDLE (WINAPI* pfnHV)(VOID);
	typedef BOOL (WINAPI* pfnBH)(HANDLE);
	typedef DWORD (WINAPI* pfnDHDPWPipPDD)(HANDLE, DWORD, LPVOID, WORD,
	PIP_OPTION_INFORMATION, LPVOID, DWORD, DWORD); // evil, no?
	pfnHV pIcmpCreateFile;
	pfnBH pIcmpCloseHandle;
	pfnDHDPWPipPDD pIcmpSendEcho;
	pIcmpCreateFile = (pfnHV)GetProcAddress(hIcmp, "IcmpCreateFile");
	pIcmpCloseHandle = (pfnBH)GetProcAddress(hIcmp, "IcmpCloseHandle");
	pIcmpSendEcho = (pfnDHDPWPipPDD)GetProcAddress(hIcmp, "IcmpSendEcho");
	if ((pIcmpCreateFile == 0) || (pIcmpCloseHandle == 0) ||
			(pIcmpSendEcho == 0))
	{
		qWarning() << "Failed to get proc addr for function.";
		return false;
	}

	// Open the ping service
	HANDLE hIP = pIcmpCreateFile();
	if (hIP == INVALID_HANDLE_VALUE)
	{
		qWarning() << "Unable to open ping service.";
		return false;
	}
	// Build ping packet
	char acPingBuffer[64];
	memset(acPingBuffer, '\xAA', sizeof(acPingBuffer));
	PIP_ECHO_REPLY pIpe = (PIP_ECHO_REPLY)GlobalAlloc(
			GMEM_FIXED | GMEM_ZEROINIT,
	sizeof(IP_ECHO_REPLY) + sizeof(acPingBuffer));
	if (pIpe == 0)
	{
		qWarning() << "Failed to allocate global ping packet buffer.";
		return false;
	}
	pIpe->Data = acPingBuffer;
	pIpe->DataSize = sizeof(acPingBuffer);

	bool result;
	// Send the ping packet
	DWORD dwStatus = pIcmpSendEcho(hIP, *((DWORD*)phe->h_addr_list[0]),
								   acPingBuffer, sizeof(acPingBuffer), NULL, pIpe,
								   sizeof(IP_ECHO_REPLY) + sizeof(acPingBuffer),
								   timeOutMs);
	if (dwStatus != 0)
	{
		result = true;
	}
	else
	{
		qWarning() << "Error obtaining info from ping packet.";
		result = false;
	}

	// Shut down...
	GlobalFree(pIpe);
	return result;
}
#else
/**
 * @brief Ping::doPingLin
 * Пинг под linux
 * @param srvHostStr хост
 * @return сервер пингуется??
 */
bool
Ping::doPingLin(const QString &srvHostStr)
{
	QProcess p;
	bool res;
	QStringList params;
	params << "-c 1"
		   << QString("-t %1").arg((timeOutMs / 1000) + 1)
		   << srvHostStr;

	p.start("ping", params);
	p.waitForFinished(timeOutMs);

	if (p.state() == QProcess::NotRunning)
	{
		res = ! p.exitCode();
	}
	else
	{
		p.terminate();
		qWarning() << "Ping for " << srvHostStr << ":" <<  p.readAll();
		res = false;
	}

	return res;
}
#endif
