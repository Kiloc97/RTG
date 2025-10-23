# RTG Programming Portfolio

C++과 Python을 활용한 다양한 프로그래밍 예제 모음집입니다.

## 프로젝트 구조

```
📁 RTG/
├── 📁 Smartpoint/          # 스마트 포인터 & 로그 파일 관리
├── 📁 STLContainer/        # STL 컨테이너 구현 (순환 버퍼)
├── 📁 Multithreading/      # 멀티스레딩 & 병렬 처리
├── 📁 Flask/              # Flask 웹 API 서버
├── 📄 Smartpoint.py       # Python 로그 관리 클래스
└── 📄 README.md
```

## 주요 기능

### 1. 🔧 Smartpoint ([Smartpoint/Smartpoint.cpp](Smartpoint/Smartpoint.cpp))
- **로그 파일 관리 시스템**
- `std::unique_ptr`을 활용한 메모리 안전성
- 멀티 파일 동시 관리 (error.log, debug.log, info.log)
- 자동 타임스탬프 추가
- RAII 패턴으로 안전한 파일 핸들링

```cpp
LogFileManager manager;
manager.openLogFile("error.log");
manager.writeLog("error.log", "Database connection failed");
```

### 2. 🔄 STL Container ([STLContainer/STLContainer.cpp](STLContainer/STLContainer.cpp))
- **순환 버퍼(Circular Buffer) 구현**
- STL 호환 Forward Iterator
- 고정 크기 버퍼로 메모리 효율성
- STL 알고리즘 지원 (`std::max_element`, `std::accumulate`)

```cpp
CircularBuffer<double> tempBuffer(5);
tempBuffer.push_back(23.5);
double maxTemp = *std::max_element(tempBuffer.begin(), tempBuffer.end());
```

### 3. ⚡ Multithreading ([Multithreading/Multithreading.cpp](Multithreading/Multithreading.cpp))
- **병렬 데이터 처리**
- 템플릿 기반 ParallelProcessor 클래스
- 이미지 처리 시뮬레이션 (밝기 조절, 제곱, 문자열 변환)
- 성능 측정 및 속도 향상 비교

```cpp
ParallelProcessor<int> processor;
auto result = processor.parallel_map(data, [](int x){ return x * x; });
```

### 4. 🌐 Flask Web API ([Flask/](Flask/))
- **도서관 관리 REST API**
- JWT 인증 시스템
- 회원가입/로그인, 도서 등록/검색, 대출 관리
- RESTful 엔드포인트 설계

**주요 엔드포인트:**
- `POST /auth/signup` - 회원가입
- `POST /auth/login` - 로그인 (JWT 토큰 발급)
- `GET /books` - 도서 검색
- `POST /loans` - 도서 대출

### 5. 🐍 Python Log Manager ([Smartpoint.py](Smartpoint.py))
- **Python 버전 로그 관리**
- 스레드 안전성 (`threading.Lock`)
- UTF-8 인코딩 지원
- C++ 버전과 호환되는 API 설계

## 기술 스택

### C++
- **표준:** C++17
- **컴파일러:** MinGW-w64 GCC
- **라이브러리:** STL, `<thread>`, `<chrono>`, `<memory>`

### Python
- **버전:** Python 3.x
- **프레임워크:** Flask, Flask-RESTful, Flask-JWT-Extended
- **라이브러리:** `threading`, `datetime`, `requests`

## 빌드 및 실행

### C++ 프로젝트
```bash
# 컴파일
g++ -std=c++17 -o program source.cpp

# 실행
./program
```

### Python 프로젝트
```bash
# Flask 서버 실행
cd Flask/
python Flasksever.py

# 클라이언트 테스트
python example.py
```

## 주요 특징

✅ **메모리 안전성** - 스마트 포인터와 RAII 패턴  
✅ **동시성** - 멀티스레딩과 스레드 안전성  
✅ **STL 호환성** - 표준 라이브러리와 seamless 통합  
✅ **모던 C++** - C++17 기능 활용  
✅ **웹 API** - RESTful 설계와 JWT 인증  
✅ **크로스 플랫폼** - Python/C++ 멀티 언어 지원  

## 개발 환경

- **IDE:** Visual Studio Code
- **OS:** Windows (MinGW-w64)
- **버전 관리:** Git

---