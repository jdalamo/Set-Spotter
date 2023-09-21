//
//  FrameProcessorWrapper.mm
//  Set-Finder
//
//  Created by JD del Alamo on 7/19/23.
//

#import <opencv2/opencv.hpp>
#import <opencv2/imgcodecs/ios.h>
#import "FrameProcessorWrapper.hpp"
#import "FrameProcessor.hpp"
#import <Foundation/Foundation.h>

@implementation FrameProcessorWrapper

- (FrameProcessorWrapper*) init: (int) maxThreads {
   // Create C++ instance
   FrameProcessor* frameProcessor = new FrameProcessor(maxThreads);
   _frameProcessor = (void*)frameProcessor;

   return self;
}

- (UIImage*) process: (UIImage*) image {
   // Convert UIImage to OpenCV Mat type
   cv::Mat convertedImage;
   UIImageToMat(image, convertedImage, true);

   // Convert colorspace
   cv::Mat frame;
   cv::cvtColor(convertedImage, frame, cv::COLOR_RGBA2RGB);

   FrameProcessor* frameProcessor = (FrameProcessor*)_frameProcessor;
   frameProcessor->process(frame);

   return MatToUIImage(frame);
}

@end