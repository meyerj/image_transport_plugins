/*********************************************************************
* Software License Agreement (BSD License)
*
*  Copyright (c) 2012, Willow Garage, Inc.
*  All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above
*     copyright notice, this list of conditions and the following
*     disclaimer in the documentation and/or other materials provided
*     with the distribution.
*   * Neither the name of the Willow Garage nor the names of its
*     contributors may be used to endorse or promote products derived
*     from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
*  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
*  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
*********************************************************************/

#include <string>

#include <sensor_msgs/msg/image.hpp>
#include <sensor_msgs/msg/compressed_image.hpp>
#include <image_transport/simple_publisher_plugin.hpp>

#include <rclcpp/node.hpp>

namespace compressed_image_transport {

using CompressedImage = sensor_msgs::msg::CompressedImage;

class CompressedPublisher : public image_transport::SimplePublisherPlugin<CompressedImage>
{
public:
  CompressedPublisher(): logger_(rclcpp::get_logger("CompressedPublisher")) {}
  ~CompressedPublisher() override = default;

  std::string getTransportName() const override
  {
    return "compressed";
  }

protected:
  // Overridden to set up reconfigure server
  void advertiseImpl(
      rclcpp::Node* node,
      const std::string& base_topic,
      rmw_qos_profile_t custom_qos,
      rclcpp::PublisherOptions options) override;

  void publish(const sensor_msgs::msg::Image& message,
               const PublishFn& publish_fn) const override;

  struct Config {
    // Compression format to use "jpeg", "png" or "tiff".
    std::string format;

    // PNG Compression Level from 0 to 9.  A higher value means a smaller size.
    // Default to OpenCV default of 3
    int png_level;

    // JPEG Quality from 0 to 100 (higher is better quality).
    // Default to OpenCV default of 95.
    int jpeg_quality;

    // TIFF resolution unit
    // Can be one of "none", "inch", "centimeter".
    std::string tiff_res_unit;
    int tiff_xdpi;
    int tiff_ydpi;
  };
  Config get_updated_config_from_parameters() const;

  Config config_;
  std::string format_param_name_;
  std::string png_level_param_name_;
  std::string jpeg_quality_param_name_;
  std::string tiff_res_unit_param_name_;
  std::string tiff_xdpi_param_name_;
  std::string tiff_ydpi_param_name_;
  rclcpp::Logger logger_;
  rclcpp::Node * node_;
};

} //namespace compressed_image_transport
