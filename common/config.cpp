/*******************************************************************************************
      ___  ___  ___ ___
    ___| _ \/ _ \/ __| __|
    / -_)   / (_) \__ \ _|
    \___|_|_\\___/|___/___|

    eROSE Server Develped by Brett19.
    hROSE Team: Brett19 and bdaan
    Year: 2006
    Release Date: Summer-Winter 2006
*******************************************************************************************/
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctype.h>

#include "log.h"

// Find the entry
bool ConfigGetEntry(const char *pcFile, const char *pcNameMust, char **pcResult) {
	char pcBuffer[ 512 ];
	char *pcName, *pcPar, *pcPoint;
	unsigned len;
	*pcResult = 0;
	// Open file
	FILE *fh;
	fh = fopen(pcFile, "r");

	if (fh == NULL) {
		return false;
	}

	do {
		fgets(pcBuffer, 510, fh);
		len = (unsigned)strlen(pcBuffer);

		if (pcBuffer[ len - 1 ] == '\n') {
			--len;
		}

		pcBuffer[ len ] = 0;
		pcName = pcBuffer;

		while (*pcName == ' ' || *pcName == '\t') {
			++pcName;
		}

		if (*pcName == 0 || *pcName == '#') {
			continue;
		}

		pcPoint = pcName;

		while (*pcPoint != ' ' && *pcPoint != '\t' && *pcPoint != '#'
		        && *pcPoint != '=' && *pcPoint != 0) {
			++pcPoint;
		}

		if (*pcPoint == 0 || *pcPoint == '#') {
			continue;
		}

		pcPar = pcPoint;

		while (*pcPar == ' ' || *pcPar == '\t' || *pcPar == '=') {
			++pcPar;
		}

		*pcPoint = 0;
#ifdef _WIN32

		if (!_strcmpi(pcName, pcNameMust)) {
			*pcResult = _strdup(pcPar);
			pcPoint  = *pcResult + strlen(pcPar);

			if (*pcPoint == 0) {
				return true;
			}

			--pcPoint;

			while (*pcPoint == ' ' || *pcPoint == '\t') {
				*pcPoint-- = 0;
			}

			return true;
		}

#else

		if (strcasecmp(pcName, pcNameMust) == 0) {
			*pcResult = strdup(pcPar);
			pcPoint  = (char*) *pcResult + strlen(pcPar);

			if (*pcPoint == 0) {
				return true;
			}

			--pcPoint;

			while (*pcPoint == ' ' || *pcPoint == '\t') {
				*pcPoint-- = 0;
			}

			return true;
		}

#endif
	} while (!feof(fh));

	// Close file
	fclose(fh);
	return true;
}

// -----------------------------------------------------------------------------------------
// Returns a text
// -----------------------------------------------------------------------------------------
char *ConfigGetString(const char *pcFile, const char *pcName, const char *pcDefault) {
	char *pcRet = 0, *pcRetReal, *pcRetOld;

	Log(msg_type::MSG_DEBUG, "looking");
	if (!ConfigGetEntry(pcFile, pcName, &pcRet)) {
		Log(msg_type::MSG_DEBUG, "not found: %s", pcName);
		return strdup(pcDefault);
	}

	if (pcRet == 0) {
		Log(msg_type::MSG_DEBUG, "not found (pcret): %s", pcName);
		return strdup(pcDefault);
	}

	pcRetOld = pcRet;

	if (*pcRet == '"') {
		++pcRet;
		pcRet[ strlen(pcRet) - 1 ] = 0;
	}

	pcRetReal = strdup(pcRet);
	free(pcRetOld);
	return pcRetReal;
}

// Returns a number
unsigned ConfigGetInt(const char *pcFile, const char *pcName, unsigned uDefault) {
	char *pcRet = 0;
	unsigned uRet;

	if (!ConfigGetEntry(pcFile, pcName, &pcRet)) {
		return uDefault;
	}

	if (pcRet == 0) {
		return uDefault;
	}

	uRet = atoi(pcRet);
	free(pcRet);
	return uRet;
}

// -----------------------------------------------------------------------------------------
