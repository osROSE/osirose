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
#ifndef __ROSE_CONFIG_FUNCTIONS__
#define __ROSE_CONFIG_FUNCTIONS__

// -----------------------------------------------------------------------------------------
// Config load functions
// -----------------------------------------------------------------------------------------
char *ConfigGetString(const char *pcFile, const char *pcName, const char *pcDefault);
unsigned ConfigGetInt(const char *pcFile, const char *pcName, unsigned uDefault);

// -----------------------------------------------------------------------------------------

#endif

