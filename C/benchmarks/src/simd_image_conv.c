
#include "benchmark.h"


#include <stdio.h>
#include <xmmintrin.h>  // Header for SSE intrinsics

#define WIDTH 1024
#define HEIGHT 1024
#define KERNEL_SIZE 3

void sse_image_convolution(float *image, float *kernel, float *output, int width, int height) {
    int half_kernel = KERNEL_SIZE / 2;

    for (int y = half_kernel; y < height - half_kernel; ++y) {
        for (int x = half_kernel; x < width - half_kernel; x += 4) {
            __m128 sum = _mm_setzero_ps();

            for (int ky = -half_kernel; ky <= half_kernel; ++ky) {
                for (int kx = -half_kernel; kx <= half_kernel; ++kx) {
                    __m128 pixel = _mm_loadu_ps(&image[(y + ky) * width + (x + kx)]);
                    __m128 kvalue = _mm_set1_ps(kernel[(ky + half_kernel) * KERNEL_SIZE + (kx + half_kernel)]);
                    sum = _mm_add_ps(sum, _mm_mul_ps(pixel, kvalue));
                }
            }

            _mm_storeu_ps(&output[y * width + x], sum);
        }
    }
}

void simd_sse() {
    float *image = (float *)_mm_malloc(WIDTH * HEIGHT * sizeof(float), 16);
    float *output = (float *)_mm_malloc(WIDTH * HEIGHT * sizeof(float), 16);
    float kernel[KERNEL_SIZE * KERNEL_SIZE] = {
            0.111f, 0.111f, 0.111f,
            0.111f, 0.111f, 0.111f,
            0.111f, 0.111f, 0.111f
    };

    // Initialize the image with some values
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        image[i] = i * 0.1f;
    }

    // Perform SIMD image convolution
    sse_image_convolution(image, kernel, output, WIDTH, HEIGHT);

//    printf("SSE image convolution completed.\n");

    _mm_free(image);
    _mm_free(output);


}
