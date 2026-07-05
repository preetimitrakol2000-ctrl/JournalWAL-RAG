#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int action_id;
    int chunk_reference_id;
    char check_signature[64];
} LogRecord;

#ifdef _WIN32
    __declspec(dllexport) int append_wal_record(const char* file_path, int act_id, int chunk_id, const char* signature);
    __declspec(dllexport) int recover_wal_count(const char* file_path);
#endif

int append_wal_record(const char* file_path, int act_id, int chunk_id, const char* signature) {
    FILE* file = fopen(file_path, "ab");
    if (!file) return -1;

    LogRecord record;
    record.action_id = act_id;
    record.chunk_reference_id = chunk_id;
    strncpy(record.check_signature, signature, sizeof(record.check_signature) - 1);

    size_t written = fwrite(&record, sizeof(LogRecord), 1, file);
    fclose(file);
    return (written == 1) ? 0 : -2;
}

int recover_wal_count(const char* file_path) {
    FILE* file = fopen(file_path, "rb");
    if (!file) return 0;

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fclose(file);

    return (int)(size / sizeof(LogRecord));
}
