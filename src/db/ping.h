/** @addtogroup Core */
/*@{*/
#ifndef FSQT_PING_H_INCLUDED
#define FSQT_PING_H_INCLUDED

#define DEFAULT_FS_PING_TIMEOUT_MS	300

#ifdef Q_OS_WIN
#define __INTERLOCKED_DECLARED /* чтобы не было проблем с новым MinGW */
#include <windows.h>

typedef struct
{
	unsigned char Ttl;                         // Time To Live
	unsigned char Tos;                         // Type Of Service
	unsigned char Flags;                       // IP header flags
	unsigned char OptionsSize;                 // Size in bytes of options data
	unsigned char *OptionsData;                // Pointer to options data
} IP_OPTION_INFORMATION, * PIP_OPTION_INFORMATION;

typedef struct
{
	DWORD Address;                             // Replying address
	unsigned long  Status;                     // Reply status
	unsigned long  RoundTripTime;              // RTT in milliseconds
	unsigned short DataSize;                   // Echo data size
	unsigned short Reserved;                   // Reserved for system use
	void *Data;                                // Pointer to the echo data
	IP_OPTION_INFORMATION Options;             // Reply options
} IP_ECHO_REPLY, * PIP_ECHO_REPLY;

#endif

/* Класс проверки доступности сервера по ip, по средствам icmp.dll для Windows */
class Ping
{
	uint timeOutMs;

public:
	Ping(const uint timeOut = DEFAULT_FS_PING_TIMEOUT_MS);
	~Ping();

	bool doPing(const QString &srvHostStr);
	bool doPingUrl(const QString &url,
				   const uint timeOut = DEFAULT_FS_PING_TIMEOUT_MS);

private:
#ifdef Q_OS_WIN
	/* указатель на icmp.dll. Инициализируется 1 раз
	 * при первом создании объекта Ping */
	static HINSTANCE hIcmp;

	bool doPingWin(const QString&);
#else
	bool doPingLin(const QString&);
#endif
};

#endif /* FSQT_PING_H_INCLUDED */

/*@}*/

