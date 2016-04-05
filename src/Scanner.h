#pragma once

#include <atomic>
#include <string>
#include <thread>

#include <RecFusion.h>

#include "SensorData.h"

class Scanner
{
public:
  Scanner();
  ~Scanner();

  int getNumSensor();
  bool setLicense(std::string file);
  bool hasLicense();
  std::string getRFVersion();

  void start();
  void stop();
  void processFrames();

private:
  static const int MAX_NUM_SENSORS = 3;
  int numSensor_;
  RecFusion::Sensor* sensors_[MAX_NUM_SENSORS];
  std::atomic<RecFusion::Reconstruction*> reconstruction_;
  SensorData* sensorsData_[MAX_NUM_SENSORS];
  bool hasLicense_;

  std::atomic<bool> recInProgress_{ false };
  std::thread* thr;

};
