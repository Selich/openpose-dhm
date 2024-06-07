#include <openpose/producer/imageSingleReader.hpp>
#include <openpose/filestream/fileStream.hpp>
#include <openpose/utilities/fastMath.hpp>
#include <openpose/utilities/fileSystem.hpp>
#include <openpose_private/utilities/openCvMultiversionHeaders.hpp>

namespace op
{
    ImageSingleReader::ImageSingleReader(const std::string& imagePath,
                                         const std::string& cameraParameterPath,
                                         const bool undistortImage,
                                         const int numberViews) :
        Producer{ProducerType::ImageDirectory, cameraParameterPath, undistortImage, numberViews},
        mImagePath{imagePath}
    {
    }

    ImageSingleReader::~ImageSingleReader()
    {
    }

    Matrix ImageSingleReader::getRawFrame()
    {
        try
        {
            // Read frame
            auto frame = loadImage(mImagePath.c_str(), CV_LOAD_IMAGE_COLOR);
            // Check frame integrity. This function also checks width/height changes. However, if it is performed
            // after setWidth/setHeight this is performed over the new resolution (so they always match).
            checkFrameIntegrity(frame);
            // Update size
            mResolution = Point<int>{frame.cols(), frame.rows()};
            // Return final frame
            return frame;
        }
        catch (const std::exception& e)
        {
            error(e.what(), __LINE__, __FUNCTION__, __FILE__);
            return Matrix();
        }
    }

    std::vector<Matrix> ImageSingleReader::getRawFrames()
    {
        try
        {
            std::vector<Matrix> rawFrames;
            for (auto i = 0; i < positiveIntRound(Producer::get(ProducerProperty::NumberViews)); i++)
                rawFrames.emplace_back(getRawFrame());
            return rawFrames;
        }
        catch (const std::exception& e)
        {
            error(e.what(), __LINE__, __FUNCTION__, __FILE__);
            return {};
        }
    }

    double ImageSingleReader::get(const int capProperty)
    {
        try
        {
            if (capProperty == CV_CAP_PROP_FRAME_WIDTH)
            {
                if (Producer::get(ProducerProperty::Rotation) == 0. || Producer::get(ProducerProperty::Rotation) == 180.)
                    return mResolution.x;
                else
                    return mResolution.y;
            }
            else if (capProperty == CV_CAP_PROP_FRAME_HEIGHT)
            {
                if (Producer::get(ProducerProperty::Rotation) == 0. || Producer::get(ProducerProperty::Rotation) == 180.)
                    return mResolution.y;
                else
                    return mResolution.x;
            }
            else if (capProperty == CV_CAP_PROP_POS_FRAMES)
                return 0; // Since it's a single image
            else if (capProperty == CV_CAP_PROP_FRAME_COUNT)
                return 1; // Only one image
            else if (capProperty == CV_CAP_PROP_FPS)
                return -1.;
            else
            {
                opLog("Unknown property", Priority::Max, __LINE__, __FUNCTION__, __FILE__);
                return -1.;
            }
        }
        catch (const std::exception& e)
        {
            error(e.what(), __LINE__, __FUNCTION__, __FILE__);
            return 0.;
        }
    }

    void ImageSingleReader::set(const int capProperty, const double value)
    {
        try
        {
            if (capProperty == CV_CAP_PROP_FRAME_WIDTH)
                mResolution.x = {(int)value};
            else if (capProperty == CV_CAP_PROP_FRAME_HEIGHT)
                mResolution.y = {(int)value};
            else if (capProperty == CV_CAP_PROP_POS_FRAMES)
                opLog("Cannot set position for a single image.", Priority::Max, __LINE__, __FUNCTION__, __FILE__);
            else if (capProperty == CV_CAP_PROP_FRAME_COUNT || capProperty == CV_CAP_PROP_FPS)
                opLog("This property is read-only.", Priority::Max, __LINE__, __FUNCTION__, __FILE__);
            else
                opLog("Unknown property", Priority::Max, __LINE__, __FUNCTION__, __FILE__);
        }
        catch (const std::exception& e)
        {
            error(e.what(), __LINE__, __FUNCTION__, __FILE__);
        }
    }
}
