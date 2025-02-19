# OpenGL shader Batch Image Manipulation

## Overview
This repository contains a C++ program designed for image manipulation through OpenGL shaders. It leverages GLFW and glad for managing window contexts and loading OpenGL functions, respectively, while employing stb_image (stbi) for reading images from an input folder. The program loads these images into GPU textures, applies transformations via fragment shaders, and saves the manipulated images to an output folder.

## Features
- **Image Loading**: Automatically reads image files from a specified input directory using stb_image.
- **GPU Texturing**: Uploads images as textures to the GPU for efficient manipulation.
- **Shader-based Manipulation**: Uses custom fragment shaders to perform various transformations on the images, such as color adjustments or geometric distortions.
- **Image Saving**: Writes the processed images back to an output directory using stb_image.

## Dependencies
- **GLFW**: For creating windows and contexts.
- **glad**: For loading OpenGL function pointers.
- **GLM**: For loading efficient math operations.
- **stb_image (stbi)**: For reading and writing image files.

## Usage
1. Clone the repository:
   ```bash
   git clone https://github.com/OperationDarkside/ClaudeOBIM.git
   cd ClaudeOBIM
   ```

2. Ensure all dependencies are installed and properly set up in your development environment.

3. Compile the program using Cmake and a C++ compiler with OpenGL support (e.g., g++, clang++).

4. Run the compiled executable:
   ```bash
   ./image_processor
   ```

5. Place input images in the designated `input` folder. The transformed images will be saved to the `output` folder.

## Notice
The code was generated using the large language model (LLM) Claude Sonnet 3.5, and as such, no copyright is claimed for the codebase itself. This repository is intended purely for educational purposes, demonstrating how OpenGL shaders can manipulate images in C++.

## License
This project is licensed under the MIT License - see the [LICENSE](https://opensource.org/licenses/MIT) file for details.

---

Feel free to modify and extend the program to suit your needs! If you find any issues or have suggestions, please open an issue on this repository.
