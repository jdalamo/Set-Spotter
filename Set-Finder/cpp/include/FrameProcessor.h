//
//  FrameProcessor.hpp
//  Set-Finder
//
//  Created by JD del Alamo on 7/19/23.
//

#pragma once

#include "SetGame.h"
#include "ThreadPool.h"

#include <opencv2/opencv.hpp>

#include <pthread.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace tp = ThreadPool;

typedef std::vector<cv::Point> Contour;
typedef std::tuple<int, Contour> IndexedContour;

class ClassifyShapeArg : public tp::PoolTaskArg<std::vector<IndexedContour>> {
public:
   ClassifyShapeArg(
      const std::vector<cv::Vec4i>& _hierarchy,
      cv::Mat& _frame,
      std::unordered_map<int, std::vector<SetGame::Shape>>& _cardIndexToShapesMap,
      pthread_mutex_t* _mapMutex) :
         hierarchy(_hierarchy),
         frame(_frame),
         cardIndexToShapesMap(_cardIndexToShapesMap),
         mapMutex(_mapMutex) {}

   ClassifyShapeArg() = delete;

   const std::vector<cv::Vec4i>& hierarchy; // Read-only
   cv::Mat& frame; // Read-only
   std::unordered_map<int, std::vector<SetGame::Shape>>&
      cardIndexToShapesMap; // Write
   pthread_mutex_t* mapMutex;
};

class FrameProcessor {
public:
   FrameProcessor(
      int maxThreads) : _threadPool(maxThreads) {}

   void process(cv::Mat& frame);

   enum class ProcessingMode {
      REAL_TIME_HIGHLIGHT,
      REAL_TIME_NUMBER,
      USER_INPUT
   }

private:
   bool cardFilter(
      const IndexedContour& indexedContour,
      const std::vector<Contour>& contours,
      const std::vector<cv::Vec4i>& hierarchy,
      std::unordered_set<int>& cardIndices) const;

   bool shapeFilter(
      const IndexedContour& indexedContour,
      const std::vector<cv::Vec4i>& hierarchy,
      const std::unordered_set<int>& cardIndices) const;

   static void classifyShapes(
      void* voidArg);

   static void classifyShape(
      const IndexedContour& indexedShape,
      const std::vector<cv::Vec4i>& hierarchy,
      const cv::Mat& frame,
      std::unordered_map<int, std::vector<SetGame::Shape>>& cardIndexToShapeMap,
      pthread_mutex_t* mapMutex);

   static void scaleContour(
      Contour& contour,
      const float scalar);

   static cv::Point scalePoint(
      const cv::Point& point,
      const int cx,
      const int cy,
      const float scalar);

   static double colorDifference(
      const cv::Scalar& color1,
      const cv::Scalar& color2);

   static std::tuple<int, int, int> bgrToHsv(
      const int b,
      const int g,
      const int r);

   std::vector<SetGame::Set> getSets(
      const std::vector<SetGame::Card> indexedCards) const;

private:
   bool _initialized = false;
   tp::ThreadPool _threadPool;
   float _minCardArea = 0;
   float _maxCardArea = 0;
   float _minShapeArea = 0;
   float _maxShapeArea = 0;
};
