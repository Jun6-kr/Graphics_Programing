#include "TextureImporter.h"
#include "Utility.h"

bool WriteTextureAsset(const uint8_t* pixels, uint32_t width, uint32_t height, const fs::path& outputPath)
{
    // 부모 경로의 폴더 생성
    fs::create_directories(outputPath.parent_path());

    // 파일 열기(쓰기 모드로)
    std::ofstream output(outputPath, std::ios::binary);
    if (!output.is_open())
    {
        std::cerr << "Failed to create texture assets: " << outputPath << "\n";
        
        return false;
    }

    // 애셋 시그니처 작성
    output.write(Craft::cTextureMagic, sizeof(Craft::cTextureMagic));

    // 애셋 버전 기록
    WritePod(output, Craft::assetVersion);

    // 텍스처 에셋 데이터 기록
    WritePod(output, width);
    WritePod(output, height);

    // 채널 4개 변수
    const uint32_t channels = 4;
    WritePod(output, channels);

    // 픽셀 데이터 배열 기록
    // 배열 전체 바이트 크기
    const auto byteCount = static_cast<std::streamsize>(width * height * channels);

    output.write(reinterpret_cast<const char*>(pixels), byteCount);

    return true;
}

bool ImportTexture(const fs::path& inputPath, const fs::path& outputPath)
{
    // 이미지 데이터 저장용 변수
    int width = 0;
    int height = 0;
    int channelCount = 0;

    // 이미지 로드해서 픽셀 데이터(배열) 저장
    stbi_uc* pixels = stbi_load(inputPath.string().c_str(), &width, &height, &channelCount, STBI_rgb_alpha);

    // pixels가 null이면 로드 실패
    if (nullptr == pixels)
    {
        std::cerr << "Failed to import texture: " << inputPath << "\n";
        return false;
    }

    // 롣드한 데이터를 애셋으로 기록
    const bool succeeded = WriteTextureAsset(pixels, static_cast<uint32_t>(width), static_cast<uint32_t>(height), outputPath);
    
    // 로드한 이미지 배열 해제
    stbi_image_free(pixels);

    // 성공한 경우에는 성공 메시지 출력
    if (succeeded)
    {
        std::cout << "Imported texture: " << inputPath << " -> " << outputPath << "\n";
    }

    return succeeded;
}
