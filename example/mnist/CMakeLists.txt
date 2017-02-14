
find_package(OpenCV REQUIRED)
include_directories(.)

add_executable(vgram_mnist vgram_mnist.cpp read_mnist.cpp)
target_link_libraries(vgram_mnist vgram ${OpenCV_LIBS})

