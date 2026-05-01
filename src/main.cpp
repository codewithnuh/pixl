#include <iostream>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "../vendor/stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../vendor/stb/stb_image_write.h"
#include <fstream>
#include <iomanip>
#include <sstream>
#include <filesystem>
namespace fs = std::filesystem;

struct ImageRecord
{
    std::string fileName;
    std::string extension;
    int width;
    int height;
    int channels;
    double sizeBytes;
    bool isValid = false;
};

std::string formatPrettySize(double bytes)
{
    if (bytes < 0)
        return "unknown";
    const char *units[] = {"B", "KB", "MB", "GB", "TB"};
    int unitIndex = 0;
    while (bytes >= 1024 && unitIndex < 4)
    {
        bytes /= 1024.0;
        unitIndex++;
    }
    std::stringstream ss;
    // Show 2 decimal places if it's MB or higher, otherwise 0
    ss << std::fixed << std::setprecision(unitIndex > 1 ? 2 : 0) << bytes << " " << units[unitIndex];
    return ss.str();
}

ImageRecord getImageInfo(const char *filePath)
{
    ImageRecord info;
    fs::path p(filePath);
    if (fs::exists(p))
    {
        info.fileName = p.filename().string();
        info.extension = p.extension().string();
        info.sizeBytes = static_cast<double>(fs::file_size(p)); //
    }
    else
    {
        return info; // isValid remains false
    }
    if (stbi_info(filePath, &info.width, &info.height, &info.channels))
    {
        info.isValid = true;
    }

    return info;
}
double getFileSize(const char *filePath)
{
    try
    {
        return static_cast<double>(fs::file_size(filePath));
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return -1.0;
    }
}
int main(int argc, char *argv[])

{
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <image_path>" << std::endl;
        return 1; // Non-zero means error
    }
    const char *imgPath = argv[1];
    std::cout << "Loading: " << imgPath << std::endl;
    int width, height, channels;
    unsigned char *imageData = stbi_load(imgPath, &width, &height, &channels, 0);
    if (imageData == nullptr)
    {
        std::cerr << "ERROR: Failed to load image: " << imgPath << std::endl;
        std::cerr << "Common reasons:" << std::endl;
        std::cerr << "  - File doesn't exist" << std::endl;
        std::cerr << "  - Not a supported format (PNG, JPG, BMP, etc.)" << std::endl;
        std::cerr << "  - Corrupted image file" << std::endl;
        return 1;
    }
    std::cout << "✓ Successfully loaded image" << std::endl;
    std::cout << "Dimensions: " << width << " x " << height << std::endl;
    std::cout << "Channels: " << channels << std::endl;
    ;

    double fileSize = getFileSize(imgPath);
    if (fileSize >= 0)
    {
        // Calculation: 1 KB = 1024 Bytes.[cite: 1]
        std::cout << "File Size:  " << (fileSize / 1024.0) << " KB" << std::endl;
    }
    else
    {
        std::cout << "File Size:  Unknown (Error accessing file)" << std::endl;
    }

    ImageRecord myImg = getImageInfo(argv[1]);

    if (!myImg.isValid)
    {
        std::cerr << "Could not read image info for: " << argv[1] << std::endl;
        return 1;
    }

    // DISPLAY RESULTS
    std::cout << "-----------------------------" << std::endl;
    std::cout << " FILE INFO" << std::endl;
    std::cout << "-----------------------------" << std::endl;
    std::cout << "Name:      " << myImg.fileName << std::endl;
    std::cout << "Type:      " << myImg.extension << std::endl;
    std::cout << "Size:      " << formatPrettySize(myImg.sizeBytes) << std::endl;
    std::cout << "Dimensions:" << myImg.width << " x " << myImg.height << std::endl;
    std::cout << "Channels:  " << myImg.channels << " (" << (myImg.channels == 4 ? "RGBA" : "RGB") << ")" << std::endl;
    std::cout << "-----------------------------" << std::endl;
    stbi_image_free(imageData);
    return 0;
}