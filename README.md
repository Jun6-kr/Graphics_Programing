# CraftEngine Graphics Programming Log

이 `Graphics_Programing` 저장소는 **Direct3D 11 / Win32 기반 그래픽스 프로그래밍 구현 기록**을 관리합니다.

이 프로젝트의 기록은 두 트랙으로 나눕니다.

- **Review**: 이전에 구현한 내용을 다시 설명하고 개념을 정리한 복습 기록
- **DevLog**: 그날 새로 배우면서 실제 코드에 추가한 구현 내용

DevLog는 Git commit과 diff를 근거로 작성하며, 코드에서 확인되지 않는 의도는 임의로 보완하지 않습니다.

## Project scope

현재 프로젝트는 `CraftEngine`이라는 작은 그래픽스/게임 엔진 구조를 직접 구현하면서 다음 영역을 학습하는 것을 목표로 합니다.

- Win32 Window 생성 및 Message 처리
- Engine Loop / Frame 제어
- Direct3D 11 Device / DeviceContext
- DXGI Swap Chain / Back Buffer
- Render Target / Depth-Stencil
- Vertex / Index Buffer
- Shader / Input Layout
- Viewport / Rasterizer / Output Merger
- 이후 Texture, Transform, Camera 등 렌더링 파이프라인 확장

## Structure

```text
README.md                       # 프로젝트 및 DevLog 운영 기준

Study/
├─ 2026-09-22.md              # 기존 Review 기록
└─ 2026-09-23.md              # 기존 Review 기록

DevLog/
├─ TEMPLATE.md                 # 일일 기록 템플릿
└─ daily/
   └─ YYYY-MM-DD.md            # 날짜별 구현 기록
```

기존 Review 문서는 `Study/` 위치를 유지하고, DevLog는 저장소 루트의 `DevLog/` 디렉터리에서 별도로 관리합니다.

## DevLog 작성 기준

### Implementation

해당 날짜의 commit에서 실제로 추가하거나 변경한 코드입니다.

예:

- `CreateSwapChain()` 구현
- Vertex Buffer 생성
- HLSL Shader 추가
- Viewport 바인딩

### Learned

그 구현을 통해 새로 다룬 기술 개념입니다.

단, commit/diff만으로 학습 의도를 확인할 수 없는 경우에는 코드에서 직접 확인되는 범위까지만 기록합니다.

예:

- Swap Chain과 HWND 연결
- RTV를 통한 Render Target 지정
- Vertex/Index Buffer의 역할

### Problem / Solved

코드 변경이나 commit 메시지에서 실제 문제와 해결 과정이 확인되는 경우 기록합니다.

문제가 명확하지 않으면 임의로 만들어내지 않습니다.

### Decision

구현에서 확인되는 주요 기술 선택을 기록합니다.

예:

- `D3D_DRIVER_TYPE_HARDWARE`
- Double Buffering
- `DXGI_SWAP_EFFECT_FLIP_DISCARD`
- Shader Model 5.0

선택한 **이유**가 commit이나 대화에 남아 있지 않으면 이유를 추측해서 작성하지 않습니다.

### Issue

미완성 코드, TODO, 잘못된 설정 가능성, 후속 확인이 필요한 항목을 기록합니다.

확정된 버그와 단순 검토 필요 항목은 구분해서 작성합니다.

### Next

다음 구현 단계가 commit, 코드, 또는 해당 날짜의 학습 흐름에서 명확하게 확인되는 경우 기록합니다.

### Review Checklist

나중에 직접 다시 확인할 가치가 있는 코드나 개념을 체크리스트로 남깁니다.

## Update workflow

1. 실제 구현 commit의 작성 날짜를 기준으로 `DevLog/daily/YYYY-MM-DD.md`를 사용합니다.
2. 같은 날짜의 구현 commit은 하나의 문서에 누적합니다.
3. 문서만 추가하는 DevLog commit은 구현 내역으로 다시 기록하지 않습니다.
4. 가능한 경우 commit SHA와 관련 파일을 함께 기록합니다.
5. 기존 기록은 보존하고 같은 내용은 중복 작성하지 않습니다.
6. 코드에서 확인되지 않은 의도나 학습 내용을 사실처럼 적지 않습니다.
7. Review에서 정리한 개념과 DevLog의 구현 기록은 서로 섞지 않습니다.

## Daily logs

- [2026-09-22](DevLog/daily/2026-09-22.md) — CraftEngine 초기 구성, Win32 Window/Engine Loop, Direct3D11 기본 렌더링 파이프라인, Shader 및 Math 코드
- [2026-09-28](DevLog/daily/2026-09-28.md) — Vector2 사칙연산 확장, Vector3·Matrix4 클래스 추가
- [2026-09-30](DevLog/daily/2026-09-30.md) — Window Resize 처리, Matrix4 이동·크기 변환, 회전 API 스텁
- [2026-10-01](DevLog/daily/2026-10-01.md) — Matrix4 축별·복합 회전 구현, Window 크기 상태 갱신, Clear Color 변경
- [2026-10-06](DevLog/daily/2026-10-06.md) — Transform 클래스, World Matrix, Dynamic Constant Buffer 및 Vertex Shader 변환 추가
- [2026-10-07](DevLog/daily/2026-10-07.md) — Engine·Game 프로젝트 분리, Render Command 처리, Transform Constant Buffer 바인딩
- [2026-10-08](DevLog/daily/2026-10-08.md) — Component·Actor·Level 구조, 지연 추가·삭제, Gameplay 수명주기와 Render 제출 연결
