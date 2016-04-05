#include "Scanner.h"

#include <chrono>
#include <iostream>

using RecFusion::Mesh;
using RecFusion::RecFusionSDK;
using RecFusion::Reconstruction;
using RecFusion::ReconstructionParams;
using RecFusion::Sensor;
using RecFusion::Vec3;
using RecFusion::Vec3i;

Scanner::Scanner()
  : reconstruction_(nullptr)
{
  setLicense("License.dat"); // TODO: parametrize this
  for (int i = 0; i < MAX_NUM_SENSORS; ++i) {
    sensors_[i] = new Sensor();
    sensorsData_[i] = nullptr;
  }
  numSensor_ = sensors_[0]->deviceCount();

  for (int i = 0; i < numSensor_; ++i) {
    if (!sensors_[i]->open(i)) {
      std::cout << "[ERROR] Couldn't open sensor with num.: " << i << std::endl;
    }
    else {
      sensorsData_[i] = new SensorData(*sensors_[i]);
    }
  }
}

Scanner::~Scanner()
{
  for (int i = 0; i < MAX_NUM_SENSORS; ++i) {
    delete sensors_[i];
    delete sensorsData_[i];
  }
  delete reconstruction_.load();
}

void Scanner::start()
{
  if (recInProgress_) { return; }
  
  recInProgress_ = true;
  thr = new std::thread([&] {
    while (recInProgress_) {
      std::chrono::milliseconds time(50);
      std::this_thread::sleep_for(time);
      processFrames();
    }
  });
  // Reset reconstruction's parameters
  delete reconstruction_.load();
  ReconstructionParams parameters(numSensor_);
  for (int i = 0; i < numSensor_; ++i) {
    SensorData* data = sensorsData_[i];
    auto depth_image = data->depth_image;
    auto color_image = data->color_image;
    parameters.setImageSize(color_image->width(), color_image->height(), depth_image->width(), depth_image->height(), i);
    parameters.setIntrinsics(data->K, i);
  }
  parameters.setVolumePosition(Vec3(0, 0, 1000));
  parameters.setVolumeResolution(Vec3i(256, 256, 256));
  parameters.setVolumeSize(Vec3(500, 500, 500));

  reconstruction_ = new Reconstruction(parameters);
  recInProgress_ = true;
}

void Scanner::stop()
{
  if (!recInProgress_) { return; }
  recInProgress_ = false;

  Mesh* mesh = new Mesh;
  if (!reconstruction_.load()->getMesh(mesh)) {
    std::cout << "[ERROR] Retrieving mesh failed." << std::endl;
    return;
  }
  // If no valid license - display reconstructed mesh
  if (!hasLicense()) {

  }
  // If valid license - save mesh
  else {
    std::string meshFile = "data/mesh.ply"; // TODO: parametrize this!
    if (mesh->save(meshFile.c_str(), Mesh::PLY)) {
      std::cout << "[INFO@Scanner]: Saved mesh as PLY (" << meshFile.c_str() << ")" << std::endl;
    }
  }
 // delete reconstruction_;
}

void Scanner::processFrames()
{
  if (!recInProgress_) return;

  for (int i = 0; i < numSensor_; ++i) {
    if (!sensorsData_[i]->HasRegularImages())
      return;
  }
  // Grab images from sensor
  bool isDataOk[3];
  for (int i = 0; i < numSensor_; ++i) {
    isDataOk[i] = sensors_[i]->readImage(*(sensorsData_[i]->depth_image), *(sensorsData_[i]->color_image), 40);
  }
  // Process images
  for (int i = 0; i < numSensor_; ++i)
  {
    if (!isDataOk[i]) continue;
    auto data = sensorsData_[i];

    // Get image size
    int width = data->color_image->width();
    int height = data->color_image->height();

    if (recInProgress_ && reconstruction_.load())
    {
      // Add frame to reconstruction
      bool isRecOk = false;
      bool isFrameOk = reconstruction_.load()->addFrame(
        i,
        *data->depth_image, *data->color_image,
        &isRecOk,
        data->scene_image,
        0,
        &data->T);

      if (isFrameOk && isRecOk)
      {
        std::cout << "All ok!" << std::endl;
        // Prepare rendered image of reconstructed scene
       /*QImage image(data->scene_image->data(), width, height, QImage::Format_RGB888);
       imgScene.append({image, width, height});*/
      }
    }
    // Display captured images in GUI
    /*QImage image(data->color_image->data(), width, height, QImage::Format_RGB888);
    imgCamera.append({image, width, height});*/
  }
  //emit sendImages(imgCamera, imgScene);
}

int Scanner::getNumSensor() {
  return numSensor_;
}

bool Scanner::hasLicense() {
  return hasLicense_;
}

bool Scanner::setLicense(std::string file)
{
  const char* name = file.c_str();
  hasLicense_ = RecFusionSDK::setLicenseFile(name);
  return hasLicense_;
}

std::string Scanner::getRFVersion()
{
  return std::to_string(RecFusionSDK::majorVersion())
    + "."
    + std::to_string(RecFusionSDK::minorVersion());
}
