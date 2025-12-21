#ifndef SEGMENTATIONLIB_H
#define SEGMENTATIONLIB_H

#include <opencv2/opencv.hpp>
#include <vector>

/**
 * @brief Library for image segmentation algorithms
 * 
 * Provides comprehensive segmentation techniques including adaptive thresholding,
 * watershed segmentation, GrabCut, and contour detection/analysis.
 */
class SegmentationLib {
public:
    /**
     * @brief Contour properties structure
     */
    struct ContourProperties {
        double area;
        double perimeter;
        cv::Point2f centroid;
        cv::Rect boundingBox;
        double circularity;  // 4π×area/perimeter²
        std::vector<cv::Point> contour;
    };

    // ==========================================================================
    // THRESHOLDING VARIANTS
    // ==========================================================================

    /**
     * @brief Apply adaptive thresholding (local threshold calculation)
     * @param input Source grayscale image
     * @param output Binary output image
     * @param maxValue Maximum value for thresholded pixels
     * @param method Adaptive method (ADAPTIVE_THRESH_MEAN_C or ADAPTIVE_THRESH_GAUSSIAN_C)
     * @param blockSize Size of neighborhood area (must be odd)
     * @param C Constant subtracted from mean/weighted mean
     */
    static void applyAdaptiveThreshold(const cv::Mat& input, cv::Mat& output,
                                      double maxValue = 255,
                                      int method = cv::ADAPTIVE_THRESH_GAUSSIAN_C,
                                      int blockSize = 11,
                                      double C = 2);

    /**
     * @brief Apply multi-level thresholding (Otsu with multiple levels)
     * @param input Source grayscale image
     * @param output Segmented image with multiple levels
     * @param levels Number of threshold levels (2-5)
     */
    static void applyMultiLevelThreshold(const cv::Mat& input, cv::Mat& output,
                                        int levels = 3);

    // ==========================================================================
    // WATERSHED SEGMENTATION
    // ==========================================================================

    /**
     * @brief Apply watershed segmentation with automatic marker detection
     * @param input Source color image
     * @param output Segmented image with boundaries
     * @param distThreshold Distance threshold for marker detection (0.0-1.0)
     */
    static void applyWatershedAuto(const cv::Mat& input, cv::Mat& output,
                                   double distThreshold = 0.5);

    /**
     * @brief Apply watershed segmentation with manual markers
     * @param input Source color image
     * @param markers Marker image (CV_32S, different regions labeled with different integers)
     * @param output Segmented image with boundaries
     */
    static void applyWatershedManual(const cv::Mat& input, cv::Mat& markers,
                                    cv::Mat& output);

    /**
     * @brief Create automatic markers for watershed
     * @param input Source image
     * @param markers Output marker image (CV_32S)
     * @param distThreshold Distance threshold (0.0-1.0)
     */
    static void createWatershedMarkers(const cv::Mat& input, cv::Mat& markers,
                                      double distThreshold = 0.5);

    // ==========================================================================
    // GRABCUT SEGMENTATION
    // ==========================================================================

    /**
     * @brief Apply GrabCut segmentation with rectangle initialization
     * @param input Source color image
     * @param output Binary mask (foreground/background)
     * @param rect Rectangle containing foreground object
     * @param iterations Number of iterations for refinement
     */
    static void applyGrabCut(const cv::Mat& input, cv::Mat& output,
                            const cv::Rect& rect, int iterations = 5);

    /**
     * @brief Apply GrabCut with mask initialization
     * @param input Source color image
     * @param mask Input/output mask (CV_8UC1: GC_BGD, GC_FGD, GC_PR_BGD, GC_PR_FGD)
     * @param iterations Number of iterations
     */
    static void applyGrabCutWithMask(const cv::Mat& input, cv::Mat& mask,
                                    int iterations = 5);

    // ==========================================================================
    // CONTOUR DETECTION & ANALYSIS
    // ==========================================================================

    /**
     * @brief Find all contours in binary image
     * @param input Binary source image
     * @param contours Output vector of contours
     * @param hierarchy Output contour hierarchy
     * @param mode Contour retrieval mode
     * @param method Contour approximation method
     */
    static void findAllContours(const cv::Mat& input,
                               std::vector<std::vector<cv::Point>>& contours,
                               std::vector<cv::Vec4i>& hierarchy,
                               int mode = cv::RETR_EXTERNAL,
                               int method = cv::CHAIN_APPROX_SIMPLE);

    /**
     * @brief Calculate properties for a single contour
     * @param contour Input contour
     * @return Contour properties structure
     */
    static ContourProperties calculateContourProperties(
        const std::vector<cv::Point>& contour);

    /**
     * @brief Calculate properties for all contours
     * @param contours Vector of contours
     * @return Vector of contour properties
     */
    static std::vector<ContourProperties> calculateAllContourProperties(
        const std::vector<std::vector<cv::Point>>& contours);

    /**
     * @brief Draw contours on image
     * @param image Image to draw on
     * @param contours Contours to draw
     * @param color Line color
     * @param thickness Line thickness (-1 for filled)
     * @param drawBoundingBoxes Whether to draw bounding boxes
     * @param drawCentroids Whether to draw centroids
     */
    static void drawContoursWithInfo(cv::Mat& image,
                                    const std::vector<std::vector<cv::Point>>& contours,
                                    const cv::Scalar& color = cv::Scalar(0, 255, 0),
                                    int thickness = 2,
                                    bool drawBoundingBoxes = true,
                                    bool drawCentroids = true);

    /**
     * @brief Filter contours by area
     * @param contours Input contours
     * @param filtered Output filtered contours
     * @param minArea Minimum area threshold
     * @param maxArea Maximum area threshold (negative for no limit)
     */
    static void filterContoursByArea(
        const std::vector<std::vector<cv::Point>>& contours,
        std::vector<std::vector<cv::Point>>& filtered,
        double minArea, double maxArea = -1);

    /**
     * @brief Filter contours by circularity
     * @param contours Input contours
     * @param filtered Output filtered contours
     * @param minCircularity Minimum circularity (0-1, circle=1)
     * @param maxCircularity Maximum circularity
     */
    static void filterContoursByCircularity(
        const std::vector<std::vector<cv::Point>>& contours,
        std::vector<std::vector<cv::Point>>& filtered,
        double minCircularity = 0.0, double maxCircularity = 1.0);

    // ==========================================================================
    // UTILITY FUNCTIONS
    // ==========================================================================

    /**
     * @brief Convert image to grayscale if needed
     * @param input Source image
     * @param output Grayscale output
     */
    static void convertToGrayscale(const cv::Mat& input, cv::Mat& output);

    /**
     * @brief Check if image is valid for segmentation
     * @param image Image to check
     * @return true if valid, false otherwise
     */
    static bool isValidImage(const cv::Mat& image);

    /**
     * @brief Colorize segmented image
     * @param labels Label image (CV_32S)
     * @param output Colorized output image
     */
    static void colorizeLabels(const cv::Mat& labels, cv::Mat& output);
};

#endif // SEGMENTATIONLIB_H
