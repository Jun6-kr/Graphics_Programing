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
| Q-015 | Camera | Camera 수에 따라 Constant Buffer 수도 달라져야 하는가? | `Renderer` |
| Q-016 | Shader | HLSL cbuffer의 Slot은 선언 순서로 정해지는가? | `DefaultVS.hlsl` |
| Q-017 | Pipeline | `SV_POSITION`은 무엇이며 반환값은 NDC인가? | `DefaultVS.hlsl` |
| Q-018 | Math | 그래픽스에서 투영은 무엇인가? | Projection Matrix |
| Q-019 | Buffer | 이번에 늘어난 것은 Vertex Buffer인가? | `Renderer` |
| Q-020 | Architecture | RenderCommand와 Camera 데이터는 왜 별도로 전달하는가? | `Renderer`, `TestLevel` |
| Q-021 | Projection | FOV와 Focal Length는 화면의 물체 크기에 어떤 영향을 주는가? | Perspective Projection |
| Q-022 | Buffer | Camera Buffer에 Projection을 추가하면 생성 크기도 바꿔야 하는가? | `Renderer::CreateCameraBuffer()` |
| Q-023 | Camera | Camera View용 이동·회전 역행렬은 왜 `T⁻¹ × R⁻¹` 순서인가? | `Matrix4::InverseTranslationRotation()` |
| Q-024 | Depth | 2D 화면에서 Depth Buffer는 왜 필요하고 어떻게 동작하는가? | `Renderer::CreateDepthStencilView()` |
| Q-025 | Depth | 깊이순 정렬만으로 Depth Buffer를 대체할 수 있는가? | Render Command 정렬·Depth Test |
| Q-026 | Pipeline | Color Buffer와 G-Buffer는 같은 것이며 Forward·Deferred의 차이는 무엇인가? | Render Target·G-Buffer |
| Q-027 | Texture | `D3D11_TEXTURE2D_DESC`와 Mip Level은 무엇을 설명하는가? | Depth Stencil Texture |
| Q-028 | Resource View | 데이터 Resource와 이를 바라보는 View를 왜 분리하고 무엇을 바인딩하는가? | Texture·RTV·DSV·SRV |
| Q-029 | Texture Mapping | Mapping과 UV 좌표는 모델의 점을 Texture에 어떻게 대응시키는가? | `Vertex`, Input Layout, Vertex Shader |

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

## 2026-10-19

<a id="q-015"></a>
### Q-015. Camera 수에 따라 Constant Buffer 수도 달라져야 하는가?

**몰랐던 부분**

- Camera가 한 개일 때와 여러 개일 때 Camera Buffer 객체 수도 달라져야 하는지 불명확했습니다.

**알게 된 내용**

- Camera 개수와 D3D Constant Buffer 객체 개수가 반드시 일치할 필요는 없습니다.
- 하나의 Dynamic Buffer를 Camera별 Render Pass 전에 갱신해 재사용할 수 있습니다.
- 여러 Camera 데이터를 동시에 참조하거나 병렬 Command 생성, Frame 중 데이터 보존이 필요하다면 Camera별 Buffer나 Ring Buffer가 필요할 수 있습니다.
- 핵심 기준은 Camera 개수 자체가 아니라 Render 순서와 데이터 수명입니다.

**관련 코드·문서**

- `CraftEngine/Graphics/Renderer.h`
- `CraftEngine/Graphics/Renderer.cpp`
- `Game/Test/TestLevel.cpp`
- `DevLog/daily/2026-10-19.md`

**남은 질문**

- 여러 Camera가 서로 다른 Viewport와 Render Target을 사용할 때의 Render Pass 구조는 아직 구현하지 않았습니다.

<a id="q-016"></a>
### Q-016. HLSL cbuffer의 Slot은 선언 순서로 정해지는가?

**몰랐던 부분**

- `register`를 적지 않았을 때 선언 순서가 곧 `b0`, `b1` 순서가 되는지 혼동했습니다.
- 선언 순서와 명시적인 `register(bN)` 중 무엇이 우선하는지 확인이 필요했습니다.

**알게 된 내용**

- `register(bN)`을 명시하면 선언 위치와 관계없이 지정한 Slot에 배정됩니다.
- `Buffer1 : register(b1)`을 먼저 선언하고 `Buffer2 : register(b0)`을 나중에 선언해도 각각 `b1`, `b0`입니다.
- `register`를 생략하면 Compiler가 Slot을 자동 배정할 수 있으므로 C++ Binding과의 계약을 선언 순서에 의존하지 않는 편이 안전합니다.
- HLSL의 `bN`과 C++의 `VSSetConstantBuffers(N, ...)`가 일치해야 합니다.

**관련 코드·문서**

- `CraftEngine/HLSLShaders/DefaultVS.hlsl`
- `CraftEngine/Graphics/Renderer.cpp`
- `DevLog/daily/2026-10-19.md`

**남은 질문**

> 없음.

<a id="q-017"></a>
### Q-017. `SV_POSITION`은 무엇이며 Vertex Shader 반환값은 NDC인가?

**몰랐던 부분**

- `SV_POSITION`이 반드시 전달해야 하는 값인지 불명확했습니다.
- Vertex Shader가 반환하는 위치를 NDC로 이해하고 있었습니다.

**알게 된 내용**

- `SV_POSITION`은 Rasterizer가 정점 위치로 해석하는 System-Value Semantic입니다.
- 삼각형을 Rasterize하려면 Vertex Shader가 최종 위치를 `SV_POSITION`으로 전달해야 합니다.
- Vertex Shader 출력은 Clip Space의 `(x, y, z, w)`이며 아직 NDC가 아닙니다.
- 이후 GPU가 `x/w`, `y/w`, `z/w` Perspective Divide를 수행한 결과가 NDC입니다.
- Direct3D NDC는 X와 Y가 `-1 ~ 1`, Z가 `0 ~ 1`입니다.

**관련 코드·문서**

- `CraftEngine/HLSLShaders/DefaultVS.hlsl`
- `DevLog/daily/2026-10-19.md`

**남은 질문**

- 2026-10-22에 Projection Matrix를 추가해 View Space 좌표를 Clip Space로 변환하도록 연결했습니다.
- Clip Space 이후 Perspective Divide와 Viewport Transform이 실제 Pipeline에서 수행되는 시점을 다시 확인할 필요가 있습니다.

<a id="q-018"></a>
### Q-018. 그래픽스에서 투영은 무엇인가?

**몰랐던 부분**

- 실제 3차원 좌표를 화면 좌표로 바꾸는 “투영”이 어떤 계산인지 알지 못했습니다.

**알게 된 내용**

- Projection은 Camera/View Space의 시야 절두체를 GPU가 처리할 수 있는 표준 Clip Space로 옮기는 변환입니다.
- Perspective Projection은 거리, 시야각, Aspect Ratio, Near/Far Plane을 반영합니다.
- Projection 결과의 `w`로 좌표를 나누는 Perspective Divide가 멀리 있는 물체를 작게 보이게 만듭니다.
- 단순히 Z를 제거하는 과정이 아니며 깊이 정보는 Depth Test를 위해 유지·변환됩니다.
- 세로 FOV를 `θ`라고 하면 정규화된 Y Scale은 `S_y = 1 / tan(θ/2)`입니다.
- 삼각형의 닮음으로 `y_NDC = y × S_y / z`가 나오며, X는 `S_x = S_y / aspect`를 사용합니다.
- Projection Matrix는 Clip Space의 `w`에 View Space 깊이를 담아 GPU의 Perspective Divide 후 `x/z`, `y/z` 관계가 나오도록 구성합니다.

**관련 코드·문서**

- `CraftEngine/HLSLShaders/DefaultVS.hlsl`
- `DevLog/daily/2026-10-19.md`
- `DevLog/daily/2026-10-20.md`

**남은 질문**

- Near/Far를 Direct3D NDC Z `0 ~ 1`로 옮기는 항을 코드로 구현했지만, 식의 유도 과정은 후속 복습이 필요합니다.

<a id="q-019"></a>
### Q-019. 이번에 늘어난 것은 Vertex Buffer인가?

**몰랐던 부분**

- Shader에 전달하는 Buffer가 추가됐다는 점 때문에 Vertex Buffer가 늘어난 것으로 표현했습니다.

**알게 된 내용**

- 이번에 추가된 것은 Camera View Matrix를 전달하는 Constant Buffer입니다.
- Vertex Buffer는 Mesh의 정점 위치와 속성을 저장합니다.
- Constant Buffer는 World, View 같은 Shader Parameter를 전달합니다.
- 둘 다 GPU Buffer지만 Pipeline에서 연결되는 Stage와 데이터의 의미가 다릅니다.

**관련 코드·문서**

- `CraftEngine/Graphics/Renderer.h`
- `CraftEngine/Graphics/Renderer.cpp`
- `CraftEngine/HLSLShaders/DefaultVS.hlsl`
- `DevLog/daily/2026-10-19.md`

**남은 질문**

> 없음.

<a id="q-020"></a>
### Q-020. RenderCommand와 Camera 데이터는 왜 별도로 전달하는가?

**몰랐던 부분**

- `RenderCommand`는 물체마다 존재하지만 Camera는 물체와 무관하므로 데이터를 어떻게 전달해야 하는지 불명확했습니다.
- 상용 Engine에서는 Renderer가 Camera에 존재하는지 궁금했습니다.

**알게 된 내용**

- World Matrix, Mesh, Shader는 물체별 정보이므로 현재 `RenderCommand`에 저장됩니다.
- View Matrix는 같은 Camera로 그리는 여러 물체가 공유하므로 Camera/View 단위 데이터로 별도 제출할 수 있습니다.
- 일반적으로 Camera는 View·Projection과 Render Target 같은 렌더링 조건을 제공하고 중앙 Renderer나 Render Pipeline이 Camera 기준으로 장면을 그립니다.
- Renderer가 Camera에 소유된다는 구조가 모든 상용 Engine에 공통으로 적용되는 것은 아닙니다.

**관련 코드·문서**

- `CraftEngine/Graphics/Renderer.h`
- `CraftEngine/Graphics/Renderer.cpp`
- `Game/Test/TestLevel.cpp`
- `DevLog/daily/2026-10-19.md`

**남은 질문**

- Camera 객체와 Render View 또는 Render Pass의 책임 경계는 Camera 클래스 도입 시 결정해야 합니다.

## 2026-10-20

<a id="q-021"></a>
### Q-021. FOV와 Focal Length는 화면의 물체 크기에 어떤 영향을 주는가?

**몰랐던 부분**

- FOV가 커질 때 같은 물체가 화면에서 커지는지 작아지는지 혼동했습니다.
- FOV와 정규화된 Focal Length의 관계를 Projection Scale과 연결하지 못했습니다.

**알게 된 내용**

- 세로 반높이를 1로 둔 Projection Plane에서는 `d = 1 / tan(FOV_y/2)`입니다.
- FOV가 커지면 `d`와 Projection Scale이 작아지고 더 넓은 영역이 보이므로 같은 물체는 화면에서 작아집니다.
- FOV가 작아지면 `d`와 Projection Scale이 커지고 좁은 영역을 확대하므로 같은 물체는 크게 보입니다.
- 이 관계는 물체 크기와 Camera까지의 거리를 고정했을 때 성립합니다.
- 여기서 `d`는 정규화된 Projection Plane의 Scale이며 실제 Lens의 mm 단위 초점거리와 그대로 같은 값은 아닙니다.

**관련 코드·문서**

- `DevLog/daily/2026-10-20.md`
- 사용자 제공 Perspective Projection 학습 자료

**남은 질문**

- 실제 Camera Lens의 Sensor 크기·Focal Length와 Engine의 FOV 변환 관계는 별도 학습이 필요합니다.

## 2026-10-22

<a id="q-022"></a>
### Q-022. Camera Buffer에 Projection을 추가하면 생성 크기도 바꿔야 하는가?

**몰랐던 부분**

- `CameraBufferData`에 Projection Matrix를 추가한 뒤에도 Buffer를 `sizeof(Matrix4)` 크기로 생성해도 되는지 확인이 필요했습니다.
- Vertex Buffer와 Vertex Shader에 연결되는 Constant Buffer의 차이가 혼동되었습니다.

**알게 된 내용**

- 오늘 변경한 것은 Mesh 정점을 담는 Vertex Buffer가 아니라 Vertex Shader의 `b1`에 연결되는 Camera Constant Buffer입니다.
- `CameraBufferData`는 View와 Projection 두 `Matrix4`를 가지므로 총 128바이트입니다.
- 64바이트만 할당하고 `sizeof(CameraBufferData)`인 128바이트를 복사하면 할당 범위를 넘어 쓰게 되므로 생성 크기도 구조체 전체 크기로 맞춰야 합니다.
- D3D11 Constant Buffer의 `ByteWidth`는 16바이트 배수여야 합니다. 이는 모든 종류의 Buffer가 무조건 16바이트여야 한다는 뜻은 아닙니다.
- 현재 `Matrix4`는 64바이트이고 두 행렬을 가진 `CameraBufferData`는 128바이트이므로 정렬 조건을 만족합니다.

**관련 코드·문서**

- `CraftEngine/Graphics/Renderer.h`
- `CraftEngine/Graphics/Renderer.cpp`
- `CraftEngine/HLSLShaders/DefaultVS.hlsl`
- `DevLog/daily/2026-10-22.md`

**남은 질문**

- HLSL Constant Buffer의 16바이트 Register Packing과 C++ 구조체 Padding은 작은 Scalar·Vector 값을 추가할 때 다시 확인해야 합니다.

<a id="q-023"></a>
### Q-023. Camera View용 이동·회전 역행렬은 왜 `T⁻¹ × R⁻¹` 순서인가?

**몰랐던 부분**

- Camera의 Position과 Rotation으로 View Matrix를 만들 때 왜 값을 그대로 사용하지 않고 역변환하는지 연결이 필요했습니다.
- 순방향 Camera Transform과 역변환에서 행렬 순서가 왜 뒤집히는지 불명확했습니다.

**알게 된 내용**

- View 변환은 Camera를 실제로 이동시키는 계산이 아니라 World의 점을 Camera 기준 좌표로 다시 표현하는 계산입니다.
- 현재 행벡터 규약에서 Scale을 제외한 Camera World Transform은 `R × T`입니다.
- 곱의 역행렬은 `(R × T)⁻¹ = T⁻¹ × R⁻¹`이므로 적용 순서도 반대로 뒤집힙니다.
- 이동의 역변환은 Position의 부호를 반대로 한 `Translation(-position)`입니다.
- 순수 회전 행렬은 정규 직교 행렬이므로 `R⁻¹ = Rᵀ`가 성립해 전치로 역회전을 구할 수 있습니다.

**관련 코드·문서**

- `CraftEngine/Math/Matrix4.cpp`
- `Matrix4::InverseTranslationRotation()`
- `CraftEngine/Component/CameraComponent.cpp`
- `DevLog/daily/2026-10-22.md`

**남은 질문**

- Camera에 Scale을 허용할지, 허용한다면 일반 역행렬을 어떻게 처리할지는 아직 결정하지 않았습니다.

## 2026-10-23

<a id="q-024"></a>
### Q-024. 2D 화면에서 Depth Buffer는 왜 필요하고 어떻게 동작하는가?

**몰랐던 부분**

- 모니터는 2차원인데 3차원 물체의 앞뒤 관계를 어떤 데이터로 유지하는지 연결이 필요했습니다.
- 이미 저장된 Depth 값과 새 Fragment의 값 중 어떤 것을 남기는지 확인이 필요했습니다.

**알게 된 내용**

- Depth Buffer는 화면의 각 Pixel 위치에 Color가 아니라 Camera 기준 깊이값을 저장하는 2차원 Texture입니다.
- 현재 일반적인 Direct3D Projection에서는 Near가 NDC Z `0`, Far가 `1`에 가까우므로 값이 작을수록 Camera에 가깝습니다.
- Frame 시작 시 Depth를 가장 먼 값인 `1.0`으로 초기화합니다.
- 기본 `LESS` 비교에서는 새 Fragment의 Depth가 저장된 값보다 작을 때만 Color와 Depth를 갱신합니다.
- 모니터에 실제 깊이축이 생기는 것은 아니며, GPU가 Pixel별 가시성을 결정하기 위해 깊이를 수치로 보관합니다.

**관련 코드·문서**

- `CraftEngine/Graphics/Renderer.cpp`
- `Renderer::BeginScene()`
- `Renderer::CreateDepthStencilView()`
- `DevLog/daily/2026-10-23.md`

**남은 질문**

- Reversed-Z를 사용할 때 Clear 값과 비교 함수가 어떻게 달라지는지는 후속 학습이 필요합니다.

<a id="q-025"></a>
### Q-025. 깊이순 정렬만으로 Depth Buffer를 대체할 수 있는가?

**몰랐던 부분**

- 먼 물체부터 그려 가까운 물체가 나중에 덮어쓰게 하면 별도의 Depth Buffer가 필요 없는지 궁금했습니다.

**알게 된 내용**

- 단순 장면은 뒤에서 앞으로 정렬하는 Painter's Algorithm으로 앞뒤를 표현할 수 있습니다.
- 하지만 물체나 삼각형이 서로 교차하거나 가림 관계가 순환하면 물체 단위 정렬만으로 정확한 순서를 만들 수 없습니다.
- 불투명 물체는 보통 Depth Test와 Depth Write를 사용하고, 앞에서 뒤로 정렬해 불필요한 Pixel Shader 실행을 줄일 수 있습니다.
- 투명 물체는 일반적으로 Depth Test는 유지하되 Depth Write를 조절하고 뒤에서 앞으로 정렬합니다.
- 실제 Renderer가 정렬과 Depth Buffer를 함께 사용하는 이유는 정확성과 성능, 투명도 처리 목적이 서로 다르기 때문입니다.

**관련 코드·문서**

- `CraftEngine/Graphics/Renderer.cpp`
- `Renderer::DrawScene()`
- `DevLog/daily/2026-10-23.md`

**남은 질문**

- Render Command를 불투명·투명 Queue로 나누고 각각 다른 정렬 기준과 Depth State를 적용하는 과정이 필요합니다.

<a id="q-026"></a>
### Q-026. Color Buffer와 G-Buffer는 같은 것이며 Forward·Deferred의 차이는 무엇인가?

**몰랐던 부분**

- 일반 Color Buffer를 G-Buffer라고 부를 수 있는지 혼동했습니다.
- Deferred Renderer를 여러 Draw Call의 결과를 나중에 합치는 방식으로만 이해했습니다.

**알게 된 내용**

- 현재 Back Buffer의 Render Target은 최종 Color를 기록하는 Color Buffer이며 그 자체를 일반적으로 G-Buffer라고 부르지는 않습니다.
- G-Buffer는 Deferred Rendering의 Geometry Pass에서 Albedo, Normal, Material 속성, Depth 등 조명 계산에 필요한 기하 정보를 여러 Render Target에 저장한 묶음입니다.
- Forward Rendering은 일반적으로 각 물체를 그릴 때 Material과 Lighting을 계산해 최종 Color Target에 직접 기록합니다.
- Deferred Rendering은 먼저 Geometry를 G-Buffer에 기록하고, 이후 Lighting Pass가 G-Buffer를 읽어 화면의 조명을 계산합니다.
- 여러 Pass의 결과를 합치는 것은 Deferred의 특징 중 하나지만, 핵심은 Geometry 기록과 Lighting 계산을 분리하는 데 있습니다.

**관련 코드·문서**

- `CraftEngine/Graphics/Renderer.cpp`
- `Renderer::CreateRenderTargetView()`
- `DevLog/daily/2026-10-23.md`

**남은 질문**

- CraftEngine은 현재 Forward 방식의 단일 Color Target 단계이며 G-Buffer와 Lighting Pass는 구현하지 않았습니다.

<a id="q-027"></a>
### Q-027. `D3D11_TEXTURE2D_DESC`와 Mip Level은 무엇을 설명하는가?

**몰랐던 부분**

- Texture2D를 생성할 때 Descriptor의 각 값과 Mip Level이 Depth Buffer에 어떤 의미인지 정리가 필요했습니다.

**알게 된 내용**

- `D3D11_TEXTURE2D_DESC`는 Texture의 Width·Height, Mip 개수, Array 크기, Format, Sample 수, Usage, Bind 용도를 설명합니다.
- Descriptor는 Texture 데이터 자체가 아니라 GPU Resource를 어떤 구조와 용도로 생성할지 전달하는 설정입니다.
- Mip은 같은 Texture를 해상도별로 축소해 둔 Level이며 일반 Color Texture의 축소 Sampling 품질과 Cache 효율에 사용됩니다.
- 현재 Depth Stencil은 화면과 같은 크기의 한 Level만 필요하므로 `MipLevels = 1`을 사용합니다.
- `DXGI_FORMAT_D24_UNORM_S8_UINT`는 24비트 Depth와 8비트 Stencil을 한 Texture에 저장합니다.

**관련 코드·문서**

- `CraftEngine/Graphics/Renderer.cpp`
- `Renderer::CreateDepthStencilView()`
- `DevLog/daily/2026-10-23.md`

**남은 질문**

- 일반 Texture의 Mip Chain 생성 방식과 Depth Pyramid·Hi-Z는 후속 학습이 필요합니다.

## 2026-10-27

<a id="q-028"></a>
### Q-028. 데이터 Resource와 이를 바라보는 View를 왜 분리하고 무엇을 바인딩하는가?

**몰랐던 부분**

- “데이터와 그 데이터를 바라보는 View를 바인딩한다”는 설명에서 Resource와 View가 각각 무엇을 담당하는지 정리가 필요했습니다.

**알게 된 내용**

- Resource는 실제 데이터를 저장합니다. 예를 들어 `ID3D11Texture2D`는 Pixel 또는 Depth 값을 보관할 GPU Texture입니다.
- View는 Resource 자체를 복제하지 않고, Pipeline이 해당 Resource를 어떤 용도·Format·Subresource 범위로 해석할지 설명합니다.
- 같은 Texture Resource도 용도와 생성 Flags가 허용하면 RTV, DSV, SRV처럼 서로 다른 View를 통해 사용할 수 있습니다.
- Output Merger에는 Color Texture 자체가 아니라 RTV를, Depth Texture 자체가 아니라 DSV를 바인딩합니다.
- Shader에서 일반 Texture를 읽으려면 Texture Resource로 SRV를 만든 다음 `PSSetShaderResources()` 등으로 해당 Shader Stage에 바인딩합니다.
- Texture의 값을 어떤 방식으로 보간하고 Addressing할지는 별도의 Sampler State가 담당합니다.

```text
실제 데이터              Pipeline이 보는 방식          바인딩 위치
Texture2D Color     →    RenderTargetView        →    Output Merger
Texture2D Depth     →    DepthStencilView        →    Output Merger
Texture2D Image     →    ShaderResourceView      →    Vertex/Pixel Shader
```

**관련 코드·문서**

- `CraftEngine/Graphics/Renderer.cpp`
- `CraftEngine/Graphics/Texture.h`
- `CraftEngine/Graphics/Texture.cpp`
- `DevLog/daily/2026-10-27.md`

**남은 질문**

- 현재 `Texture` 클래스는 CPU 이미지 로드까지만 구현됐으며 Texture2D, SRV와 Sampler의 생성·바인딩은 후속 구현이 필요합니다.

## 2026-10-28

<a id="q-029"></a>
### Q-029. Mapping과 UV 좌표는 모델의 점을 Texture에 어떻게 대응시키는가?

**몰랐던 부분**

- “대응시킨다”는 표현과 Mapping의 의미를 Vertex·Texture 데이터 흐름에 연결할 필요가 있었습니다.
- 모델의 점에 색상 정보가 함께 있다는 설명과 UV 좌표의 관계가 불명확했습니다.

**알게 된 내용**

- Mapping은 한 데이터 영역의 값을 다른 영역의 값에 대응시키는 과정입니다.
- Texture Mapping에서는 Mesh의 각 Vertex에 2차원 Texture의 어느 위치를 대응시킬지 UV 좌표로 지정합니다.
- 보통 `U`는 Texture의 가로축, `V`는 세로축이며 정규화된 `0 ~ 1` 범위를 사용합니다.
- UV 자체는 Color가 아니라 Texture 위치를 가리키는 좌표입니다. 실제 Color는 Pixel Shader가 UV로 Texture를 Sampling해 얻습니다.
- Vertex는 Position뿐 아니라 UV, Normal, Tangent, Vertex Color 같은 여러 속성을 함께 가질 수 있습니다. Vertex Color와 UV는 서로 다른 속성입니다.
- 삼각형 내부의 UV는 Rasterizer가 Vertex별 UV를 보간해 Pixel Shader로 전달합니다.

```text
Mesh Vertex Position ↔ Vertex UV
                            ↓
                    Texture의 위치 지정
                            ↓
                   Pixel Shader Sampling
                            ↓
                         Color
```

**관련 코드·문서**

- `CraftEngine/Graphics/Vertex.h`
- `CraftEngine/Graphics/StaticMesh.cpp`
- `CraftEngine/Graphics/Shader.cpp`
- `CraftEngine/HLSLShaders/DefaultVS.hlsl`
- `DevLog/daily/2026-10-28.md`

**남은 질문**

- 현재 Vertex Shader는 UV를 입력받지만 Pixel Shader로 전달하지 않으며 Texture Sampling도 아직 연결되지 않았습니다.
