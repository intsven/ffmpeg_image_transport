// -*-c++-*---------------------------------------------------------------------------------------
// Copyright 2023 Bernd Pfrommer <bernd.pfrommer@gmail.com>
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <unistd.h>

#include <ffmpeg_image_transport/ffmpeg_encoder.hpp>
#include <opencv2/imgproc/imgproc.hpp>

void packetReady(const ffmpeg_image_transport::FFMPEGPacketConstPtr & pkt)
{
  (void)pkt;
  std::cout << " got packet!" << std::endl;
}

void test_encoder(int numFrames)
{
  ffmpeg_image_transport::FFMPEGEncoder enc;
  enc.setCodec("h264_nvenc");
  enc.setProfile("main");
  enc.setPreset("ll");
  enc.setQMax(10);
  enc.setBitRate(8242880);
  enc.setGOPSize(2);
  enc.setFrameRate(100, 1);
  cv::Mat mat = cv::Mat::zeros(1440, 1024, CV_8UC3);
  enc.initialize(mat.cols, mat.rows, packetReady);
  for (int i = 0; i < numFrames; i++) {
    mat = cv::Mat::zeros(1440, 1024, CV_8UC3);  // clear image
    cv::putText(
      mat, std::to_string(i), cv::Point(mat.cols / 2, mat.rows / 2), cv::FONT_HERSHEY_COMPLEX,
      2 /* font size */, cv::Scalar(255, 0, 0) /* col */, 2 /* weight */);
    const rclcpp::Time t = rclcpp::Clock().now();
    std_msgs::msg::Header header;
    header.stamp = t;
    enc.encodeImage(mat, header, t);
  }
}

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  int numFrames = 10;
  int opt;
  while ((opt = getopt(argc, argv, "n:")) != -1) {
    switch (opt) {
      case 'n':
        numFrames = atoi(optarg);
        break;
      default:
        std::cout << "unknown option: " << opt << std::endl;
        return (-1);
    }
  }

  test_encoder(numFrames);
  return (0);
}
