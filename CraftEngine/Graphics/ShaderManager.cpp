#include "ShaderManager.h"
#include "Shader.h"

namespace Craft
{
    std::shared_ptr<Shader> ShaderManager::LoadShader(const std::wstring& name)
    {
        // 이름 값으로 셰이더 검색
        auto result = shaders.find(name);

        // unordered_map의 find 함수는 검색에 실패했을 때 end iterator를 반환
        if (result != shaders.end())
        {
            // 성공한 경우에는 값(value) 반환
            return result->second;
        }

        // 검색 실패한 경우, 셰이더가 로드된 상태가 아니므로 셰이더를 생성한 뒤에 맵에 추가하고 반환
        auto newShader = std::make_shared<Shader>(name);
        shaders.emplace(name, newShader);
        return newShader;
    }
}
