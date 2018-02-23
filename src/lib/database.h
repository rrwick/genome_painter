#ifndef __DATABASE_H__
#define __DATABASE_H__


#include <cstdio>
#include <string>
#include <vector>


#include "sqlite3.h"


#include "lib/common.h"


namespace db {


sqlite3 *open(const char database_fp[]);
void execute(sqlite3 *dbp, const char statement[]);
void start_transaction(sqlite3 *dbp);
void end_transaction(sqlite3 *dbp);
sqlite3_stmt *prepare_statement(const char statement[], sqlite3 *dbp);
void bind_int64(sqlite3_stmt *stmt, int index, common::ullong data, sqlite3 *db);
void bind_double(sqlite3_stmt *stmt, int index, double data, sqlite3 *db);
void step(sqlite3_stmt *stmt, sqlite3 *db);


} // namesspace db


#endif
