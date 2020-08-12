#include "write_ppm.h"

#include <cassert>
#include <fstream>
#include <iostream>

bool write_ppm(
    const std::string& filename,
    const std::vector<unsigned char>& data,
    const int width,
    const int height,
    const int num_channels) {
  ////////////////////////////////////////////////////////////////////////////

  // Open file and check if successful
  std::ofstream image_file;
  image_file.open(filename);
  if (image_file.fail() || !image_file.is_open()) return false;

  try {
    // Write header
    if (num_channels == 1) {
      image_file << "P2\n";
    } else {
      image_file << "P3\n";
    }
    image_file << width << " " << height << "\n"
               << "255"
               << "\n";

    // Write the raster values
    int index = 0;
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        index = num_channels * (width * i + j);
        image_file << " " << (int)data[index];
        if (num_channels == 3) {
          image_file << " " << (int)data[index + 1];
          image_file << " " << (int)data[index + 2];
        }
      }

      // Add new line at the end of each row
      image_file << "\n";
    }
  } catch (const std::exception&) {
    if (image_file.is_open()) image_file.close();
    return false;
  } catch (...) {
    if (image_file.is_open()) image_file.close();
    return false;
  }

  image_file.close();
  return true;
  ////////////////////////////////////////////////////////////////////////////
}
