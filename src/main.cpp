#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <filesystem>
#include <optional>

#define STB_IMAGE_IMPLEMENTATION
#include "../vendor/stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../vendor/stb/stb_image_write.h"

namespace fs = std::filesystem;

enum class ImageFormat
{
    PNG,
    JPG,
    JPEG,
    WEBP
};

std::optional<ImageFormat> parseFormat(std::string format)
{
    std::transform(format.begin(), format.end(), format.begin(), ::tolower);

    if (format == "png")
        return ImageFormat::PNG;
    if (format == "jpg")
        return ImageFormat::JPG;
    if (format == "jpeg")
        return ImageFormat::JPEG;
    if (format == "webp")
        return ImageFormat::WEBP;

    return std::nullopt;
}

struct ImageRecord
{
    std::string fileName;
    std::string extension;
    int width = 0;
    int height = 0;
    int channels = 0;
    double sizeBytes = 0;
    bool isValid = false;
};

std::string formatPrettySize(double bytes)
{
    const char *units[] = {"B", "KB", "MB", "GB", "TB"};
    int i = 0;

    while (bytes >= 1024 && i < 4)
    {
        bytes /= 1024;
        i++;
    }

    std::stringstream ss;
    ss << std::fixed << std::setprecision(i > 1 ? 2 : 0)
       << bytes << " " << units[i];

    return ss.str();
}

ImageRecord getImageInfo(const char *filePath)
{
    ImageRecord info;
    fs::path p(filePath);

    if (!fs::exists(p))
        return info;

    info.fileName = p.filename().string();
    info.extension = p.extension().string();
    info.sizeBytes = (double)fs::file_size(p);

    if (stbi_info(filePath, &info.width, &info.height, &info.channels))
        info.isValid = true;

    return info;
}

class ImagePathValidator
{
    fs::path inputPath;
    fs::path outputPath;

public:
    ImagePathValidator(fs::path in, fs::path out)
        : inputPath(std::move(in)), outputPath(std::move(out)) {}

    bool validate()
    {
        if (!fs::exists(inputPath) || !fs::is_regular_file(inputPath))
            return false;

        return true;
    }

    fs::path getInput() const { return inputPath; }
    fs::path getOutput() const { return outputPath; }
};

bool saveImage(const fs::path &output,
               unsigned char *data,
               int w, int h, int c)
{
    std::string ext = output.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    if (ext == ".png")
        return stbi_write_png(output.string().c_str(), w, h, c, data, w * c);

    if (ext == ".jpg" || ext == ".jpeg")
        return stbi_write_jpg(output.string().c_str(), w, h, c, data, 90);

    return false;
}

class ImageEngine
{
    ImagePathValidator validator;
    int w = 0, h = 0, c = 0;
    unsigned char *data = nullptr;

public:
    ImageEngine(ImagePathValidator v)
        : validator(std::move(v)) {}

    bool process()
    {
        if (!validator.validate())
            return false;

        auto input = validator.getInput();
        auto output = validator.getOutput();

        data = stbi_load(input.string().c_str(), &w, &h, &c, 0);
        if (!data)
            return false;

        bool ok = saveImage(output, data, w, h, c);

        stbi_image_free(data);
        data = nullptr;

        return ok;
    }
};

void printHelp()
{
    std::cout << "Image Tool Usage:\n"
              << "  app <input> <output>\n"
              << "  app --help\n";
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printHelp();
        return 1;
    }

    std::string arg = argv[1];

    if (arg == "--help")
    {
        printHelp();
        return 0;
    }

    if (argc < 3)
    {
        printHelp();
        return 1;
    }

    fs::path input = argv[1];
    fs::path output = argv[2];

    ImagePathValidator validator(input, output);
    ImageEngine engine(validator);

    if (!engine.process())
    {
        std::cerr << "Failed to process image\n";
        return 1;
    }

    std::cout << "Image converted successfully\n";
    return 0;
}