#ifndef PROTOCOLS_H
#define PROTOCOLS_H

#define DB_FOLDER_PATH "db"
#define TABLES_FILE_PATH "db/tables"

#define TABLE_INFO_SCHEMA_FORMAT "(%s:%s:%i)"
#define COLUMN_SCHEMA_FORMAT "[%s:%c]"

typedef struct {
    char * name;
    char type;
} Column;
typedef struct {
    char * name;
    char * path;
    int columnCount;

    char * rawInfo;
    char * rawColumns;

    Column * columns;

    FILE * file;

} Table;

typedef enum {
    EQUALS,
    GREATER,
    GREATER_EQ,
    LESSER,
    LESSER_EQ,
    LIKE
} Comparison;



#endif // PROTOCOLS_H