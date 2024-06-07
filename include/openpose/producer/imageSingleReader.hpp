#ifndef OPENPOSE_PRODUCER_IMAGE_SINGLE_READER_HPP
#define OPENPOSE_PRODUCER_IMAGE_SINGLE_READER_HPP

#include <openpose/core/common.hpp>
#include <openpose/producer/producer.hpp>

namespace op
{
    /**
     * ImageSingleReader is an abstract class to extract pose from a single image. Its interface imitates the
     * cv::VideoCapture class, so it can be used quite similarly to the cv::VideoCapture class. Thus,
     * it is quite similar to VideoReader and WebcamReader.
     */
    class OP_API ImageSingleReader : public Producer
    {
    public:
        /**
         * Constructor of ImageSingleReader. It sets the image path from which the image will be loaded.
         * @param imagePath const std::string parameter with the file path of the image.
         * @param cameraParameterPath const std::string parameter with the folder path containing the camera
         * parameters (only required if imageStereo > 1).
         * @param numberViews const int parameter with the number of images per iteration (>1 would represent
         * stereo processing).
         */
        explicit ImageSingleReader(
            const std::string& imagePath, const std::string& cameraParameterPath = "",
            const bool undistortImage = false, const int numberViews = -1);

        virtual ~ImageSingleReader();

        inline bool isOpened() const
        {
            return !mImagePath.empty();
        }

        inline void release()
        {
            mImagePath.clear();
        }

        double get(const int capProperty);

        void set(const int capProperty, const double value);

    private:
        const std::string mImagePath;
        Point<int> mResolution;

        Matrix getRawFrame();

        std::vector<Matrix> getRawFrames();

        DELETE_COPY(ImageSingleReader);
    };
}

#endif // OPENPOSE_PRODUCER_IMAGE_SINGLE_READER_HPP
