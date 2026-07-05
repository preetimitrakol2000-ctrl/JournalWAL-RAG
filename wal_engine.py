import ctypes
import os
import sys

class WalBridge:
    def __init__(self, log_filename: str):
        if not os.path.exists("./libwal.so") and not os.path.exists("./libwal.dll"):
            if sys.platform.startswith("win"):
                os.system("gcc -shared -o libwal.dll wal_engine.c")
                lib_path = "./libwal.dll"
            else:
                os.system("gcc -shared -fPIC -o libwal.so wal_engine.c")
                lib_path = "./libwal.so"
        else:
            lib_path = "./libwal.dll" if sys.platform.startswith("win") else "./libwal.so"

        self.lib = ctypes.CDLL(lib_path)
        self.lib.append_wal_record.argtypes = [ctypes.c_char_p, ctypes.c_int, ctypes.c_int, ctypes.c_char_p]
        self.lib.append_wal_record.restype = ctypes.c_int
        self.lib.recover_wal_count.argtypes = [ctypes.c_char_p]
        self.lib.recover_wal_count.restype = ctypes.c_int
        
        self.log_path = log_filename.encode('utf-8')

    def log_insertion(self, action_counter: int, chunk_id: int, signature_hash: str) -> int:
        return self.lib.append_wal_record(self.log_path, action_counter, chunk_id, signature_hash.encode('utf-8'))

    def audit_rebuild_records(self) -> int:
        return self.lib.recover_wal_count(self.log_path)
