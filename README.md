# Cocos2d-x Mobile & WebGL Porting Lab

Cocos2d-x 4.0 기반 C++ 게임 프로젝트를 만들고, Windows 실행 확인부터 Android 빌드와 WebGL 포팅 가능성까지 실험하는 학습용 저장소입니다.

현재 예제 프로젝트는 `PoopDodge`입니다. 저장소 안에 Cocos2d-x 엔진 소스를 함께 포함해서, 이 저장소만 clone해도 별도 엔진 복사 없이 빌드할 수 있게 구성했습니다.

## 현재 게임

`PoopDodge`는 위에서 떨어지는 갈색 삼각형을 피하면서 오래 살아남는 간단한 피하기 게임입니다.

- 배경: 단색
- 플레이어: 위아래로 긴 타원형
- 장애물: 위에서 떨어지는 갈색 삼각형
- 점수: 생존 시간
- 난이도: 시간이 지날수록 장애물 낙하 속도와 개수 증가

## 개발 환경

Visual Studio 2022 C++ 도구와 CMake가 설치되어 있어야 합니다.

현재 Windows 빌드는 Cocos2d-x 4.0에 포함된 prebuilt 라이브러리에 맞춰 Win32(x86) 기준으로 확인했습니다. Win64/x64 빌드는 이 프로젝트에서 사용하지 않습니다.

기본 창 크기는 `960 x 640`입니다.

## 빌드 방법

처음 빌드하거나 CMake 설정이 바뀐 경우 아래 명령을 사용합니다.

```cmd
cd cocos2d-x\projects\PoopDodge
cmake -S . -B build-win32 -A Win32
cmake --build build-win32 --config Debug
```

이미 `build-win32` 폴더가 만들어진 뒤 C++ 코드만 수정했다면 마지막 빌드 명령만 다시 실행하면 됩니다.

```cmd
cmake --build build-win32 --config Debug
```

## 실행 방법

빌드가 끝나면 아래 명령으로 실행합니다.

```cmd
.\build-win32\bin\PoopDodge\Debug\PoopDodge.exe
```

## 플레이 방법

- `Enter`: 게임 시작 / 다시 시작
- `←`, `→`: 플레이어 좌우 이동
- 화면의 `Retry` 버튼: 게임 오버 후 다시 시작

갈색 삼각형을 피하면서 최대한 오래 살아남는 것이 목표입니다. 시간이 지날수록 삼각형이 더 빨리, 더 많이 떨어집니다.

## 문서

- [작업 진행도](docs/Process.md)
- [Windows 빌드 오류 정리](docs/build_error.md)

## 다음 방향

Windows Win32 실행 확인을 기준점으로 삼고, 이후 Android 빌드와 WebGL 포팅 가능성을 차례대로 실험할 예정입니다.
