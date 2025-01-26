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
#include <ffmpeg_image_transport/ffmpeg_subscriber_segments.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>

void packetReady(const ffmpeg_image_transport::FFMPEGPacketConstPtr & pkt)
{
  std::cout << " header stamp: " << rclcpp::Time(pkt->header.stamp).seconds() << std::endl;
  std::cout << " got packet of size: " << pkt->data.size() << std::endl;
}

void test_subscriber(const std::string & codec, int numFrames)
{
  ffmpeg_image_transport::FFMPEGSubscriberSegments sub;
  
  // Print out codec and number of frames
  std::cout << "codec: " << codec << std::endl;
  std::cout << "numFrames: " << numFrames << std::endl;
}

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  int numFrames = 10;
  int opt;
  std::string codec("hevc_nvenc");
  while ((opt = getopt(argc, argv, "n:c:")) != -1) {
    switch (opt) {
      case 'n':
        numFrames = atoi(optarg);
        break;
      case 'c':
        codec = optarg;
        break;
      default:
        std::cout << "unknown option: " << opt << std::endl;
        return (-1);
    }
  }
  // sample usage: ./test_encoder -c libx264

  test_subscriber(codec, numFrames);
  return (0);
}
