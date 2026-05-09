# Cocos2d-x Mobile & WebGL Porting Lab

Cocos2d-x 4.0 기반 C++ 게임 프로젝트를 만들고, Windows 실행 확인부터 Android 빌드와 WebGL 포팅 가능성까지 실험하는 학습용 저장소입니다.

현재 예제 프로젝트는 `PoopDodge`입니다. 저장소 안에 Cocos2d-x 엔진 소스를 함께 포함해서, 이 저장소만 clone해도 별도 엔진 복사 없이 빌드할 수 있게 구성합니다.

---

## Windows Debug 빌드

Visual Studio 2022 C++ 도구와 CMake가 설치되어 있어야 합니다.

```cmd
cd cocos2d-x\projects\PoopDodge
cmake -S . -B build-win32 -A Win32
cmake --build build-win32 --config Debug
```

빌드가 끝나면 실행 파일은 아래 위치에 생성됩니다.

```cmd
.\build-win32\bin\PoopDodge\Debug\PoopDodge.exe
```

---


