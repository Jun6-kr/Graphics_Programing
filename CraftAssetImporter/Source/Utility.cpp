#include "Utility.h"

void WriteString(std::ofstream& output, const std::string& value)
{

}

// 입력 경로의 형태를 유지해서 출력 경로에 폴더를 생성
fs::path RelativeOutput(const fs::path & inputRoot, const fs::path & outputRoot, const fs::path & inputPath, const char* extension)
{
	// 상대 경로 값
	// inputRoot 경로를 제외한 경로 값을 반환
	fs::path relative = fs::relative(inputPath, inputRoot);

	// 새 애셋에 맞게 확장자 교체
	relative.replace_extension(extension);

	// 출력 경로에 사용할 새로운 경로를 조합해서 반환
	return outputRoot / relative;
}
