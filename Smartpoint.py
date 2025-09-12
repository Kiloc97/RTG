from datetime import datetime
import os
import threading

#LogFileManager manager
class LogFileManager:
    def __init__(self):
        self._files = {}
        self._Lock = threading.Lock()

    #로그 파일 열기
    def openLogFile(self, filename: str):
        with self._Lock:
            if filename not in self._files:
                try:
                    f = open(filename, 'a', encoding='utf-8')
                    self._files[filename] = f
                except Exception as e:
                    raise RuntimeError(f"파일 열기 실패: {filename} ({e})")
                
    #로그 파일 쓰기
    def writeLog(self, filename: str, message: str):
        Dtime = datetime.now().strftime("[%y-%m-%d %h:%m:%s]")
        with self._Lock:
            if filename not in self._files:
                raise RuntimeError(f"파일이 열려있지 않음: {filename}")
            try:
                self._files[filename].write(f"{Dtime} {message}\n")
                self._files[filename].flush()
            except Exception as e:
                raise RuntimeError(f"로그 쓰기 실패: {filename} ({e})")
            
    #로그 파일 읽기
    def readLogs(self, filename: str):
        pass


    #로그 파일 닫기
    def closeLogFile(self, filename: str):
        pass


