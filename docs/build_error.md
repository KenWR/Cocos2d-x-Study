# Cocos2d-x Windows 빌드 오류 정리

Cocos2d-x 4.0은 오래된 Windows 빌드 구성을 기준으로 만들어져 있어, 최신 Visual Studio 2022와 Windows SDK 환경에서 그대로 빌드하면 몇 가지 호환성 문제가 발생했다.

## 1. x64 빌드와 Win32 라이브러리 충돌

### 오류

처음에는 기본 설정으로 x64 빌드를 시도했지만, Cocos2d-x 4.0에 포함된 Windows용 prebuilt 라이브러리들은 대부분 `win32` 폴더에 있는 x86 라이브러리였다.

앱은 x64로 빌드되는데 링크 대상 라이브러리는 x86이어서 서로 아키텍처가 맞지 않았고, 그 결과 외부 심볼을 찾지 못하는 링커 오류가 발생했다.

예시:

```text
'x86' 라이브러리 컴퓨터 종류가 'x64' 대상 컴퓨터 종류와 충돌합니다.
```

```text
error LNK2019: 확인할 수 없는 외부 기호
fatal error LNK1120: 확인할 수 없는 외부 참조
```

### 원인

C++ 소스 코드는 x64로 컴파일될 수 있지만, 최종 실행 파일을 만들려면 외부 `.lib` 파일들도 같은 아키텍처여야 한다.

이번 프로젝트에서는 아래와 같은 라이브러리들이 `win32` 경로에서 연결되었다.

```text
cocos2d/external/glfw3/prebuilt/win32/glfw3.lib
cocos2d/external/Box2D/prebuilt/win32/debug/libbox2d.lib
cocos2d/external/chipmunk/prebuilt/win32/debug-lib/libchipmunk.lib
cocos2d/external/win32-specific/gles/prebuilt/glew32.lib
```

즉, 앱 타깃은 x64인데 외부 라이브러리는 x86이라 링커가 연결할 수 없었다.

### 해결

CMake 생성 시 `-A Win32` 옵션을 사용해 앱도 Win32(x86)로 빌드하도록 맞췄다.

```powershell
cmake -S . -B build-win32 -A Win32
cmake --build build-win32 --config Debug
```

## 2. Win32 API 상수 컴파일 오류

### 오류

`UIEditBoxImpl-win32.cpp`에서 `GWL_WNDPROC`, `GWL_USERDATA`가 선언되지 않았다는 컴파일 오류가 발생했다.

예시:

```text
error C2065: 'GWL_WNDPROC': 선언되지 않은 식별자입니다.
error C2065: 'GWL_USERDATA': 선언되지 않은 식별자입니다.
```

### 원인

Cocos2d-x 내부 코드가 `SetWindowLongPtrW`를 사용하면서 `GWL_WNDPROC`, `GWL_USERDATA` 같은 오래된 상수를 함께 사용하고 있었다.

현재 MSVC/Windows SDK 조합에서는 `SetWindowLongPtrW`에 맞는 포인터 크기용 상수인 `GWLP_WNDPROC`, `GWLP_USERDATA`를 사용하는 쪽이 맞다.

### 해결

`cocos2d-x/projects/PoopDodge/cocos2d/cocos/ui/UIEditBox/UIEditBoxImpl-win32.cpp`에서 아래처럼 수정했다.

```cpp
GWL_WNDPROC  -> GWLP_WNDPROC
GWL_USERDATA -> GWLP_USERDATA
```

## 3. MSVC PDB 동시 쓰기 오류

### 오류

MSVC 병렬 컴파일 중 여러 `cl.exe` 프로세스가 같은 `.pdb` 파일에 동시에 접근하면서 `C1041` 오류가 발생했다.

예시:

```text
error C1041: 프로그램 데이터베이스 '...cocos2d.pdb'을(를) 열 수 없습니다.
여러 CL.EXE에서 동일한 .PDB 파일에 쓰는 경우 /FS를 사용하십시오.
```

### 원인

Cocos2d-x CMake 설정은 MSVC에서 `/MP` 옵션을 사용해 여러 소스 파일을 병렬 컴파일하도록 되어 있었다.

하지만 이 환경에서는 여러 컴파일 프로세스가 같은 PDB 파일에 동시에 쓰려고 하면서 충돌이 발생했다.

### 해결

`cocos2d-x/projects/PoopDodge/cocos2d/cmake/Modules/CocosConfigDefine.cmake`에서 MSVC 컴파일 옵션을 수정했다.

```cmake
PUBLIC /MP
```

를 아래처럼 변경했다.

```cmake
PUBLIC /FS
```

`/FS`는 PDB 파일 쓰기를 직렬화해 여러 컴파일 프로세스가 같은 디버그 정보 파일에 접근할 때 발생하는 충돌을 방지한다.

## 정리

이번 빌드 오류들은 프로젝트 게임 코드 자체의 문제라기보다는 Cocos2d-x 4.0의 오래된 Windows 빌드 설정과 현재 개발 환경 사이의 차이에서 발생한 호환성 문제였다.

최종적으로 아래 조치를 적용해 Windows Debug 빌드를 통과시켰다.

- CMake 생성 시 `-A Win32`를 사용해 앱과 prebuilt 라이브러리의 아키텍처를 맞춤
- `GWL_*` 상수를 `GWLP_*` 상수로 수정
- MSVC 컴파일 옵션을 `/MP`에서 `/FS`로 변경해 PDB 충돌 방지
