// JPG, PNG 등의 이미지 파일을 원하는 형식으로 변환

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

int main(int argc, char** argv)
{
	// 실행 명령어 포함 실행 인자 수가 3개인지 확인
	if (argc < 3)
	{
		std::cout << "Usage: CraftAssetImporter <source_dir> <output_dir>\n";

		return 1;
	}

	// 원본 애셋 경로
	fs::path sourceRoot = argv[1];

	// 임포트 처리 후에 저장하는 경로
	fs::path outputRoot = argv[2];

	// 오류 확인 - 원본 경로가 존재하는 지 확인
	if (!fs::exists(sourceRoot))
	{
		std::cerr << "Source directory does not exist: " << sourceRoot << "\n";

		return 2;
	}

	// 경로 출력
	std::cout << "Source: " << sourceRoot << "\n";
	std::cout << "Imported: " << outputRoot << "\n";

	// 원본 경로로 이동해서 파일 순회 처리
	for (const auto& entry : fs::recursive_directory_iterator(sourceRoot))
	{
		// 파일인지 확인
		if (!entry.is_regular_file())
		{
			continue;
		}

		// 확인용으로 파일 이름 출력
		std::cout << entry.path() << "\n";
	}

	return 0;
}