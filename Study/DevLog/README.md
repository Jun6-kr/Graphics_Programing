# Graphics Programming Development Log

Graphics_Programing 프로젝트에서 **새로 구현하거나 추가한 내용**을 Git commit 기준으로 날짜별 기록합니다.

기존 개념 복습 문서와 분리하여 운영합니다.

## Structure

```text
Study/
├─ 2026-09-22.md
├─ 2026-09-23.md
└─ DevLog/
   ├─ README.md
   └─ daily/
      └─ YYYY-MM-DD.md
```

## Categories

- **Work**: 해당 날짜의 commit에서 확인된 구현/변경 작업
- **Solved**: commit 또는 diff에서 문제와 해결 과정이 직접 확인되는 경우
- **Decision**: 코드에서 확인되는 주요 기술 선택. 이유가 확인되지 않으면 이유를 임의로 작성하지 않음
- **Issue**: diff에서 직접 확인되는 미완료 사항 또는 명시적 문제
- **Next**: commit에 다음 작업이 명시된 경우 기록
- **Review Checklist**: 코드상 다시 확인할 필요가 있는 항목

## Update workflow

1. Git commit의 작성 일자를 기준으로 `daily/YYYY-MM-DD.md`에 기록합니다.
2. 같은 날짜의 commit은 하나의 Daily 문서에 합칩니다.
3. 기존 기록은 보존하며 같은 의미의 항목은 중복 작성하지 않습니다.
4. 가능한 경우 관련 commit SHA와 변경 파일을 함께 기록합니다.
5. commit/diff에서 확인되지 않은 의도나 이유는 임의로 추측하지 않습니다.
6. 단순 Study/문서 commit은 구현 DevLog에서 제외할 수 있습니다.

## Daily logs

- [2026-09-22](daily/2026-09-22.md) — 프로젝트 초기 구성, Win32 Window/Engine 루프, Direct3D11 Renderer 기본 파이프라인, 기본 Shader 및 Math 코드
