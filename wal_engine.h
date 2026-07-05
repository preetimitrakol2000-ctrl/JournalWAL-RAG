#ifndef WAL_ENGINE_H
#define WAL_ENGINE_H

typedef struct LogRecord LogRecord;
int append_wal_record(const char* file_path, int act_id, int chunk_id, const char* signature);
int recover_wal_count(const char* file_path);

#endif
