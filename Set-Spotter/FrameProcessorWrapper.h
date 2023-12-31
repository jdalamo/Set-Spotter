//
//  FrameProcessorWrapper.hpp
//  Set-Spotter
//
//  Created by JD del Alamo on 7/19/23.
//

#pragma once

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>

@interface FrameProcessorWrapper : NSObject {
@private
   void* _frameProcessor;
}
- (FrameProcessorWrapper*) init: (int) maxThreads showSets:(bool) showSets;
- (UIImage*) process: (UIImage*) image;
- (bool) getShowSets;
- (void) setShowSets: (bool) show;
- (int) getNumSetsInFrame;
@end
