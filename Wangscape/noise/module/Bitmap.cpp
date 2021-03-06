#include "Bitmap.h"
#include "logging/Logging.h"
#include "OptionsManager.h"
#include <boost/filesystem.hpp>

namespace noise
{
namespace module
{

namespace {
static std::map<std::string, Bitmap::ImagePtr> loadedImages;
}


Bitmap::Bitmap() :
    Module::Module(GetSourceModuleCount()),
    mRegion(0.0, 0.0, 1.0, 1.0),
    mMaxScale(false)
{
}

const std::string & Bitmap::GetFilename() const
{
    return mFilename;
}

void Bitmap::SetFilename(const std::string & filename)
{
    // Load and process file if not already done
    const auto it = loadedImages.find(filename);
    if (it != loadedImages.cend())
    {
        setCurrentImage(filename, it->second);
        return;
    }

    const sf::Image sf_bitmap = loadNewImage(filePath(filename));
    const auto bitmap_d = convertSfImageToImageGrey(sf_bitmap);

    loadedImages.insert({filename, bitmap_d});
    setCurrentImage(filename, bitmap_d);
}

sf::Rect<double> Bitmap::GetRegion() const
{
    return sf::Rect<double>();
}

void Bitmap::SetRegion(sf::Rect<double> region)
{
    mRegion = region;
}

double Bitmap::GetDefaultValue() const
{
    return mDefaultValue;
}

void Bitmap::SetDefaultValue(double default_value)
{
    mDefaultValue = default_value;
}

bool Bitmap::GetMaxScale() const
{
    return mMaxScale;
}

void Bitmap::SetMaxScale(bool max_scale)
{
    mMaxScale = max_scale;
    if (mImage && mMaxScale)
        updateMax();
}

double Bitmap::GetValue(double x, double y, double z) const
{
    if (!mRegion.contains(x, y))
        return mDefaultValue;
    if (mImage->n_rows == 0 || mImage->n_cols == 0)
    {
        logError() << "Tried to call Bitmap::GetValue with zero-pixel resolution";
        throw std::runtime_error("Invalid Bitmap module configuration");
    }

    const sf::Vector2<double> offset = calculateOffset(x, y);
    size_t x_image = static_cast<size_t>(offset.x);
    size_t y_image = static_cast<size_t>(offset.y);

    if (mMaxScale)
        return getPixel(x_image, y_image) / mMaxValue;
    else
        return getPixel(x_image, y_image);
}

void Bitmap::setCurrentImage(const std::string & filename, ImagePtr image)
{
    mFilename = filename;
    mImage = image;
    if (mMaxScale)
        updateMax();
}

sf::Image Bitmap::loadNewImage(const std::string & file_path)
{
    sf::Image sf_bitmap;
    logInfo() << "Loading image at " << file_path << " into Bitmap module";
    if (!sf_bitmap.loadFromFile(file_path))
    {
        logError() << "Could not load image";
        throw std::runtime_error("Invalid Bitmap filename");
    }
    return sf_bitmap;
}

Bitmap::ImagePtr Bitmap::convertSfImageToImageGrey(const sf::Image & sf_bitmap)
{
    auto bitmap = imageFromSFImage(sf_bitmap);
    auto bitmap_gs = imageToGreyscale(bitmap);
    auto bitmap_d = std::make_shared<ImageGreyFloat>(arma::conv_to<ImageGreyFloat>::from(bitmap_gs));
    return bitmap_d;
}

double Bitmap::getPixel(size_t x, size_t y) const
{
    if (!mImage)
    {
        logError() << "Tried to call Bitmap::getPixel without valid image";
        throw std::runtime_error("Bitmap not initialised");
    }
    return (*mImage)(y,x);
}

sf::Vector2<double> Bitmap::calculateOffset(double x, double y) const
{
    sf::Vector2<double> offset = {x, y};

    offset.x -= mRegion.left;
    offset.y -= mRegion.top;

    offset.x /= mRegion.width;
    offset.y /= mRegion.height;

    offset.x *= mImage->n_cols;
    offset.y *= mImage->n_rows;

    return offset;
}

std::string Bitmap::filePath(const std::string& filename) const
{
    const auto& base_path = getOptionsManager().getOptions().paths.directory;
    return (boost::filesystem::path(base_path) / filename).string();
}

void Bitmap::updateMax()
{
    mMaxValue = mImage->max();
}

} // namespace module
} // namespace noise
