# CraftEngine Questions & Answers

구현과 복습 과정에서 생긴 질문을 한 곳에서 확인하기 위한 통합 문서입니다.

- 코드에 남긴 질문은 바로 아래의 `[CODEX 답변]`으로도 확인할 수 있습니다.
- 질문의 문서 원본은 이 파일에서만 관리합니다.
- 날짜가 확인되는 질문은 날짜별로, 날짜를 확정할 수 없는 기존 코드 질문은 별도 묶음으로 기록합니다.
- 아직 결정하지 않은 내용은 답을 만들어내지 않고 `남은 질문`에 보존합니다.

## Index

| ID | 구분 | 질문 | 관련 위치 |
|---|---|---|---|
| Q-001 | Win32 | `CS_HREDRAW`, `CS_VREDRAW`는 Render Loop 설정인가? | `Win32Window.cpp` |
| Q-002 | DXGI | `DXGI_MODE_DESC`는 Factory가 관리하는 버퍼 설정인가? | `Renderer.cpp` |
| Q-003 | D3D11 Buffer | `WRITE_DISCARD`와 `Map()`은 GPU 버퍼를 어떻게 갱신하는가? | `Renderer.cpp` |
| Q-004 | Shader | HLSL Compile의 `main`은 무엇인가? | `Shader.cpp` |
| Q-005 | Mesh | Index의 시작점과 Winding Order는 어떤 의미인가? | `StaticMesh.cpp` |
| Q-006 | Mesh | 사각형을 왜 삼각형 두 개로 만드는가? | `StaticMesh.cpp` |
| Q-007 | Vector | 내적의 기하학적 의미와 조명에서의 용도는 무엇인가? | `Vector2.cpp` |
| Q-008 | Matrix | Y축 회전 행렬에서 `sin`의 부호가 반대인 이유는 무엇인가? | `Matrix4.cpp` |
| Q-009 | Transform | 회전 행렬의 X축 기저와 Actor의 Right/Forward는 어떤 관계인가? | `Matrix4.h`, `Transform.cpp` |
| Q-010 | Camera | LookAt View Matrix는 어떻게 만드는가? | `Matrix4::LookAt()` |
| Q-011 | Vector | 외적 순서가 왜 Right/Left 방향을 바꾸는가? | `Vector3::Cross()` |
| Q-012 | Transform | World Matrix의 S × R × T 순서가 왜 중요한가? | `Transform::Update()` |
| Q-013 | Matrix | 정규 직교 회전 행렬에서 왜 전치가 역행렬인가? | `Matrix4` |
| Q-014 | Architecture | 버퍼 생성은 Renderer에 요청하는 설계가 올바른가? | `Renderer.h` |

## 기존 코드 질문

<a id="q-001"></a>
### Q-001. `CS_HREDRAW`, `CS_VREDRAW`는 Render Loop 설정인가?

**몰랐던 부분**

- Window Class의 다시 그리기 Style이 지속적인 화면 갱신을 담당하는지 혼동했습니다.

**알게 된 내용**

- Window 너비나 높이가 바뀌면 Client 영역 전체를 무효화해 다시 그리도록 하는 Class Style입니다.
- 그 결과 `WM_PAINT`가 발생할 수 있지만, 매 Frame 화면을 그리는 Render Loop 자체를 설정하지는 않습니다.

**관련 코드·문서**

- `CraftEngine/Core/Win32Window.cpp`

**남은 질문**

> 없음.

<a id="q-002"></a>
### Q-002. `DXGI_MODE_DESC`는 Factory가 관리하는 버퍼 설정인가?

**몰랐던 부분**

- `DXGI_MODE_DESC BufferDesc`가 Factory 소유 버퍼의 설정인지 불명확했습니다.

**알게 된 내용**

- Factory가 직접 관리하는 설정이라기보다 Swap Chain Back Buffer의 표시 Mode를 설명하는 값입니다.
- 너비, 높이, Refresh Rate, Pixel Format, Scanline 순서, Scaling 정보를 담고 `CreateSwapChain()`이 이를 사용해 Buffer를 구성합니다.

**관련 코드·문서**

- `CraftEngine/Graphics/Renderer.cpp`
- `Renderer::CreateSwapChain()`

**남은 질문**

> 없음.

<a id="q-003"></a>
### Q-003. `WRITE_DISCARD`와 `Map()`은 GPU 버퍼를 어떻게 갱신하는가?

**몰랐던 부분**

- `D3D11_MAP_WRITE_DISCARD`가 Resource 자체를 폐기하는지 혼동했습니다.
- `mapped.pData`에 쓴 값이 언제 `transformBuffer`의 내용으로 사용되는지 불명확했습니다.

**알게 된 내용**

- `WRITE_DISCARD`는 버퍼 객체를 제거하는 것이 아니라 기존 내용을 보존할 필요가 없다고 Driver에 알립니다.
- GPU가 이전 영역을 사용하는 중이면 Driver가 CPU에 새 쓰기 영역을 제공할 수 있어 대기를 줄일 수 있습니다.
- `Map()`이 돌려준 `mapped.pData`에 CPU가 값을 쓰고 `Unmap()`한 뒤, 이후 Draw에서 GPU가 바인딩된 Buffer의 새 내용을 읽습니다.

**관련 코드·문서**

- `CraftEngine/Graphics/Renderer.cpp`
- Transform Constant Buffer 갱신 코드

**남은 질문**

> 없음.

<a id="q-004"></a>
### Q-004. HLSL Compile의 `main`은 무엇인가?

**몰랐던 부분**

- `D3DCompileFromFile()`에 전달하는 `"main"`이 Shader 파일의 시작점 함수 이름인지 확인이 필요했습니다.

**알게 된 내용**

- Compiler가 Compile을 시작할 HLSL Entry Point 함수 이름입니다.
- 이름은 반드시 `main`일 필요가 없으며 HLSL 파일의 함수 이름과 일치해야 합니다.
- `vs_5_0`, `ps_5_0` 같은 Target이 같은 Entry Point를 어느 Shader Stage로 Compile할지 결정합니다.

**관련 코드·문서**

- `CraftEngine/Graphics/Shader.cpp`

**남은 질문**

> 없음.

<a id="q-005"></a>
### Q-005. Index의 시작점과 Winding Order는 어떤 의미인가?

**몰랐던 부분**

- 삼각형 Index가 어느 정점에서 시작하는지가 앞면 판정에 영향을 주는지 혼동했습니다.

**알게 된 내용**

- 시작 정점 자체보다 정점을 순회하는 Winding Order가 중요합니다.
- `0, 1, 2`와 `1, 2, 0`처럼 순환 이동한 순서는 같은 방향의 면을 만들지만, 순서를 뒤집으면 앞면과 뒷면 판정이 바뀝니다.

**관련 코드·문서**

- `CraftEngine/Graphics/StaticMesh.cpp`
- `StaticMesh::CreateTriangleMesh()`

**남은 질문**

- 현재 Rasterizer의 Front Counter-Clockwise와 Culling 설정을 함께 확인할 필요가 있습니다.

<a id="q-006"></a>
### Q-006. 사각형을 왜 삼각형 두 개로 만드는가?

**몰랐던 부분**

- 정점 네 개를 사각형 Primitive 하나로 직접 그리지 않는 이유가 불명확했습니다.

**알게 된 내용**

- Direct3D의 기본 Primitive Topology에는 점, 선, 삼각형이 있고 독립된 사각형 Primitive는 없습니다.
- Triangle List에서 `{ 0, 1, 2 }`, `{ 0, 2, 3 }` 두 삼각형이 대각선 정점 0과 2를 공유해 사각형 영역을 채웁니다.

**관련 코드·문서**

- `CraftEngine/Graphics/StaticMesh.cpp`
- `StaticMesh::CreateQuadMesh()`

**남은 질문**

> 없음.

<a id="q-007"></a>
### Q-007. 내적의 기하학적 의미와 조명에서의 용도는 무엇인가?

**몰랐던 부분**

- 내적 계산이 기하학적으로 무엇을 뜻하는지, 조명 연산과 어떤 관계가 있는지 연결되지 않았습니다.

**알게 된 내용**

- 내적은 한 벡터가 다른 벡터 방향으로 얼마나 향하는지 나타내며 투영과 방향 유사도의 의미를 가집니다.
- 정규화한 두 벡터의 내적은 `cos(theta)`이므로 같은 방향은 1, 수직은 0, 반대 방향은 -1입니다.
- Lambert 확산광에서는 정규화한 표면 법선 `N`과 빛 방향 `L`의 내적으로 표면이 빛을 얼마나 정면으로 받는지 계산합니다.

**관련 코드·문서**

- `CraftEngine/Math/Vector2.cpp`
- `Vector2::Dot()`

**남은 질문**

- 실제 Pixel Shader에 Normal과 Light Direction을 전달해 Lambert 조명을 구현하는 과정은 후속 학습이 필요합니다.

<a id="q-008"></a>
### Q-008. Y축 회전 행렬에서 `sin`의 부호가 반대인 이유는 무엇인가?

**몰랐던 부분**

- Y축 회전에서 두 `sin` 항 중 하나에 음수가 붙는 이유가 불명확했습니다.

**알게 된 내용**

- Y축 자체가 아니라 XZ 평면을 회전시키므로 X와 Z가 서로 섞이는 두 항의 부호가 반대가 됩니다.
- 현재 행벡터 규약에서는 `x' = x cosθ + z sinθ`, `z' = -x sinθ + z cosθ`가 되므로 `m02`에 `-sinθ`, `m20`에 `+sinθ`를 배치합니다.
- 이 부호 관계가 회전 후에도 축의 직교성과 길이를 유지합니다.

**관련 코드·문서**

- `CraftEngine/Math/Matrix4.cpp`
- `Matrix4::RotationY()`

**남은 질문**

> 없음.

<a id="q-009"></a>
### Q-009. 회전 행렬의 X축 기저와 Actor의 Right/Forward는 어떤 관계인가?

**몰랐던 부분**

- World Right를 회전시키는 것과 회전 행렬의 한 행을 꺼내 쓰는 것이 같은지 혼동했습니다.
- Unreal이 회전 행렬의 X축을 Forward로 사용한다는 설명의 의미가 불명확했습니다.

**알게 된 내용**

- 단위 Right 벡터 `(1, 0, 0)`에 Rotation을 적용하면 Actor의 회전된 Local Right 방향을 얻습니다.
- 현재 행벡터 규약에서는 이 결과가 회전 행렬의 첫 번째 행, 즉 회전된 X축 기저와 같습니다.
- Unreal은 Local X축을 Forward로 정하므로 회전된 X축 기저를 꺼내면 Forward 방향을 얻습니다. 이것은 전체 회전을 X축 하나만으로 계산한다는 뜻은 아닙니다.
- 방향 벡터에는 위치 이동이 필요하지 않으므로 Translation을 적용하지 않습니다.

**관련 코드·문서**

- `CraftEngine/Math/Matrix4.h`
- `CraftEngine/Math/Transform.cpp`
- `Transform::Right()`

**남은 질문**

- CraftEngine에서 Local X축을 Right로 유지할지, 게임 엔진 관례에 맞춰 Forward 정의를 별도로 둘지 정리가 필요합니다.

<a id="q-014"></a>
### Q-014. 버퍼 생성은 Renderer에 요청하는 설계가 올바른가?

**몰랐던 부분**

- 모든 Buffer 생성을 Renderer에 요청하고 Renderer가 Pointer를 반환하는 구조가 정답인지 확인이 필요했습니다.

**알게 된 내용**

- 유일한 정답은 아닙니다. 현재처럼 Resource 객체가 Renderer를 통해 D3D Device에 접근하는 구조는 작은 학습용 Engine에서 사용할 수 있습니다.
- 규모가 커지면 Renderer가 모든 Resource 생성과 소유까지 맡기보다, Device Wrapper나 Resource Factory가 생성 책임을 담당하고 Resource 객체가 수명을 소유하는 구조로 분리할 수 있습니다.
- Raw Pointer 반환 여부, 소유권, 해제 책임을 함께 결정해야 하며 생성 위치만으로 설계의 옳고 그름을 판단할 수 없습니다.

**관련 코드·문서**

- `CraftEngine/Graphics/Renderer.h`
- `CraftEngine/Graphics/StaticMesh.cpp`
- `CraftEngine/Graphics/Shader.cpp`

**남은 질문**

- CraftEngine의 Resource 생성·소유·Cache 정책은 아직 결정하지 않았습니다.

## 2026-10-16

<a id="q-010"></a>
### Q-010. LookAt View 행렬은 어떻게 만드는가?

**몰랐던 부분**

- Eye와 Target으로 Forward를 구한 다음 Up을 어떻게 사용해야 하는지 불명확했습니다.
- 외적의 순서와 이동 값이 단순한 `-Eye`가 아닌 이유가 연결되지 않았습니다.

**알게 된 내용**

- 왼손 View에서는 `Forward = normalize(Target - Eye)`로 시야 방향을 만듭니다.
- `Right = normalize(Up × Forward)`, `Up = normalize(Forward × Right)`로 정규 직교 Camera 기저를 만듭니다.
- World 점을 각 Camera 축에 투영하므로 이동 성분은 `-dot(Eye, Right/Up/Forward)`입니다.
- LookAt은 Camera 기저와 Camera World Transform의 역변환을 한 행렬에 직접 구성하는 방법입니다.

**관련 코드·문서**

- `CraftEngine/Math/Matrix4.cpp`
- `Matrix4::LookAt()`
- `DevLog/daily/2026-10-16.md`

**남은 질문**

- Camera가 Eye, Target, Up을 직접 보관할지 Transform의 Rotation에서 Target과 Up을 만들지는 아직 설계하지 않았습니다.

<a id="q-011"></a>
### Q-011. 외적 순서가 왜 Right/Left 방향을 바꾸는가?

**몰랐던 부분**

- 외적 순서를 바꾸는 것이 표기 차이인지 실제 좌표계 방향을 바꾸는지 불명확했습니다.

**알게 된 내용**

- 외적은 `A × B = -(B × A)`이므로 순서를 바꾸면 결과 방향이 반대가 됩니다.
- 현재 기본 축에서 `Up × Forward`는 Right, `Forward × Up`은 Left가 됩니다.
- 좌표계 이름만 외우기보다 실제 기준 벡터를 대입해 원하는 방향이 나오는지 검증하는 편이 안전합니다.

**관련 코드·문서**

- `CraftEngine/Math/Vector3.cpp`
- `Vector3::Cross()`
- `CraftEngine/Math/Matrix4.cpp`
- `Matrix4::LookAt()`
- `DevLog/daily/2026-10-16.md`

**남은 질문**

> 없음.

<a id="q-012"></a>
### Q-012. World Matrix의 S × R × T 순서가 왜 중요한가?

**몰랐던 부분**

- 세 행렬을 모두 곱하기만 하면 순서와 무관하게 같은 결과가 나오는지 불명확했습니다.

**알게 된 내용**

- 현재 프로젝트는 행벡터이므로 `p × S × R × T`가 왼쪽에서 오른쪽으로 적용됩니다.
- Scale과 Rotation으로 Local 원점 기준 형태를 정한 뒤 Translation으로 World 위치에 배치합니다.
- 행렬 곱에는 교환법칙이 성립하지 않습니다. Translation을 Rotation보다 먼저 적용하면 이동 벡터까지 회전해 물체가 원점 주위를 공전할 수 있고, Scale보다 먼저 적용하면 이동 거리까지 확대될 수 있습니다.

**관련 코드·문서**

- `CraftEngine/Math/Transform.cpp`
- `Transform::Update()`
- `DevLog/daily/2026-10-16.md`

**남은 질문**

- 부모·자식 Transform이 추가될 때 Local Matrix와 Parent World Matrix를 어떤 순서로 결합할지 후속 학습이 필요합니다.

<a id="q-013"></a>
### Q-013. 정규 직교 회전 행렬에서는 왜 전치 행렬이 역행렬인가?

**몰랐던 부분**

- 일반 역행렬 계산 대신 행과 열만 교환해도 회전을 되돌릴 수 있는 이유가 불명확했습니다.

**알게 된 내용**

- 회전 행렬의 각 축이 길이 1이고 서로 직교하면 `RᵀR = I`가 성립합니다.
- 따라서 `R⁻¹ = Rᵀ`이며, 비균등 Scale이나 Shear가 섞여 정규 직교 조건이 깨지면 단순 전치를 역행렬로 사용할 수 없습니다.

**관련 코드·문서**

- `CraftEngine/Math/Matrix4.cpp`
- `Matrix4::Transpose()`
- `Matrix4::InverseRotation()`
- `DevLog/daily/2026-10-16.md`

**남은 질문**

> 없음.
