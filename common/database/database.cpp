#include "database.h"
#include "../log.h"

// constructor
CDatabase::CDatabase(char *server , char *username, char *password,
                     char *database, unsigned int port , MYSQL *mysql) {
	Server = server;
	Username = username;
	Password = password;
	Database = database;
	Port = port;
	Mysql = mysql;
	LastPing = time(NULL);
	SQLMutex = PTHREAD_MUTEX_INITIALIZER;
	mysql_init(Mysql);
}

// deconstructor
CDatabase::~CDatabase() {
}

// disconnect from mysql
void CDatabase::Disconnect() {
	mysql_close(Mysql);
}

int CDatabase::Connect() {
	Log(msg_type::MSG_INFO, "Connecting to MySQL");
	Log(msg_type::MSG_DEBUG, "USER: `%s` (%d) PASS: `%s` (%d) DB: `%s` (%d)",
		Username, (int) strlen(Username),
		Password, (int) strlen(Password),
		Database, (int) strlen(Database));

	if (!mysql_real_connect(Mysql, Server, Username, Password, Database, Port, NULL,
	                        0)) {
		Log(msg_type::MSG_FATALERROR, "Error connecting to MySQL server: %s\n",
		    mysql_error(Mysql));
		return -1;
	}

	// Get timeout so we know when to ping - Drakia
	MYSQL_RES *result = QStore("SELECT @@wait_timeout");
	MYSQL_ROW row = mysql_fetch_row(result);
	Timeout = atoi(row[0]) -
	          60; // Set the timeout to 1 minute before the connection will timeout
	QFree();
	Log(msg_type::MSG_INFO, "MySQL Ping Timeout: %d seconds", Timeout);
	return 0;
}

// reconnect to mysql
int CDatabase::Reconnect() {
	Log(msg_type::MSG_INFO, "Reconnecting to Mysql");

	if (!mysql_real_connect(Mysql, Server, Username, Password, Database, Port, NULL,
	                        0)) {
		Log(msg_type::MSG_FATALERROR, "Error reconnecting to MySQL server: %s\n",
		    mysql_error(Mysql));
		return -1;
	}

	return 0;
}

// execute query
bool CDatabase::QExecute(const char *Format, ...) {
	bool Qfail = true;
	char query[80000];
	va_list ap;
	va_start(ap, Format);
	vsprintf(query, Format, ap);
	va_end(ap);
	Log(msg_type::MSG_QUERY, query);
	pthread_mutex_lock(&SQLMutex);

	while (Qfail) {
		if (mysql_query(Mysql, query) != 0) {
			Log(msg_type::MSG_FATALERROR, "Could not execute query: %s", mysql_error(Mysql));

			if (Reconnect() == -1) {
				Log(msg_type::MSG_FATALERROR, "Could not execute query: %s", mysql_error(Mysql));
				pthread_mutex_unlock(&SQLMutex);
				return false;
			} else {
				Qfail = false;
			}
		} else {
			Qfail = false;
		}
	}

	pthread_mutex_unlock(&SQLMutex);
	return true;
}

MYSQL_RES *CDatabase::QStore(const char *Format, ...) {
	bool Qfail = true;
	char query[80000];
	va_list ap;
	va_start(ap, Format);
	vsprintf(query, Format, ap);
	va_end(ap);
	result = NULL;
	Log(msg_type::MSG_QUERY, query);
	//Log( msg_type::MSG_INFO, query );
	pthread_mutex_lock(&SQLMutex);

	while (Qfail) {
		if (mysql_query(Mysql, query) != 0) {
			Log(msg_type::MSG_FATALERROR, "Could not execute query: %s", mysql_error(Mysql));

			if (Reconnect() == -1) {
				Log(msg_type::MSG_FATALERROR, "Could not execute query: %s", mysql_error(Mysql));
				pthread_mutex_unlock(&SQLMutex);
				return 0;
			} else {
				Qfail = false;
			}
		} else {
			Qfail = false;
		}

		// Log( msg_type::MSG_INFO, "mysql answer: %s", mysql_error( Mysql ) );
	}

	result = mysql_store_result(Mysql);
	return result;
}

MYSQL_RES *CDatabase::QUse(const char *Format, ...) {
	bool Qfail = true;
	char query[1024];
	va_list ap;
	va_start(ap, Format);
	vsprintf(query, Format, ap);
	va_end(ap);
	result = NULL;
	Log(msg_type::MSG_QUERY, query);
	pthread_mutex_lock(&SQLMutex);

	while (Qfail) {
		if (mysql_query(Mysql, query) != 0) {
			Log(msg_type::MSG_FATALERROR, "Could not execute query: %s", mysql_error(Mysql));

			if (Reconnect() == -1) {
				Log(msg_type::MSG_FATALERROR, "Could not execute query: %s", mysql_error(Mysql));
				pthread_mutex_unlock(&SQLMutex);
				return 0;
			} else {
				Qfail = false;
			}
		} else {
			Qfail = false;
		}
	}

	result = mysql_use_result(Mysql);
	return result;
}

bool CDatabase::DoSQL(const char *Format, ...) {
	int retval;
	char Buffer[1024];
	va_list ap;
	va_start(ap, Format);
	vsprintf(Buffer, Format, ap);
	va_end(ap);
	retval = mysql_query(Mysql, Buffer);

	if (retval != 0) {
		Log(msg_type::MSG_ERROR, "MySQL Query Error '%s'", mysql_error(Mysql));
	}

	return (retval == 0);
}

void CDatabase::QFree() {
	mysql_free_result(result);
	pthread_mutex_unlock(&SQLMutex);
}

bool CDatabase::Ping() {
	if (time(NULL) > (LastPing + Timeout)) {
		Log(msg_type::MSG_INFO, "MySql Ping");
		LastPing = time(NULL);
		MYSQL_RES *res = QStore("SELECT @@wait_timeout");

		if (res == NULL) {
			Log(msg_type::MSG_ERROR, "MySQL Ping Failed, Attempting reconnect");
			QFree();
			return !Reconnect();
		}

		MYSQL_ROW row = mysql_fetch_row(res);
		Timeout = atoi(row[0]) - 60;
		QFree();
		Log(msg_type::MSG_INFO, "MySql Pong, new Timeout: %d seconds", Timeout);
	}

	return true;
}
