#include "sqlite3.c"
#include "std.c"

#define SQOK SQLITE_OK

static const char* SQMG(sqlite3 *db) {
	return sqlite3_errmsg(db);
}

static sqlite3* sqOpen_(char *dbname, int mode, int *rc) {
	sqlite3 *db;
	*rc = sqlite3_open_v2(dbname, &db, mode, NULL);
	return db;
}

static sqlite3* sqOpen(char *dbname) {
	sqlite3 *db; int rc;
	db = sqOpen_(dbname, SQLITE_OPEN_READONLY, &rc);
	if( rc != SQOK ) DBERR("Can't open database: %s", dbname);
	DBLOG("open db '%s'", dbname);
	return db;
}

static sqlite3* sqOpenCreate(char *dbname) {
	sqlite3 *db; int rc;
	db = sqOpen_(dbname, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, &rc);
	if( rc != SQOK ) DBERR("Can't open database: %s", dbname);
	DBLOG("open db '%s'", dbname);
	return db;
}

bool sqIsDbExsit(char *dbname) {
	sqlite3 *db; int rc;
	//DBLOG("dbname = %s", dbname);
	db = sqOpen_(dbname, SQLITE_OPEN_READONLY, &rc);
	if (rc == SQOK) {
		sqlite3_close(db);
		return true;
	}
	return false;
}

int sqExec(sqlite3 *db, char *sq) {
	sqlite3_stmt *pStmt;
	int rc;
	rc = sqPrepare(db, sq, -1,&pStmt, NULL);
	if (rc != SQOK) DBERR("%s in %s" , SQMG(db),  __FUNCTION__);
	rc = sqlite3_step(pStmt);
	if (rc != SQLITE_DONE) DBERR("%s in %s" , SQMG(db),  __FUNCTION__);
	sqlite3_finalize(pStmt);
	return rc;
}

static int sqPrepare(
					 sqlite3 *db,            // Database handle
					 const char *zSql,       // SQL statement, UTF-8 encoded
					 int nByte,              // set -1, if zSql end with null, else to set it as nbytes of the buf  // Maximum length of zSql in bytes.
					 sqlite3_stmt **ppStmt,  // OUT: Statement handle
					 const char **pzTail     // OUT: Pointer to unused portion of zSql
					 ){
						 int rc;
						 rc = sqlite3_prepare_v2(db, zSql, -1,
							 ppStmt, NULL
							 );
						 if (rc != SQOK || *ppStmt == NULL ) { // 
							 //ASSERT(false);
							 DBERR("Error in sqPrepare():  %s", sqlite3_errmsg(db));
						 }
						 return rc;
}
