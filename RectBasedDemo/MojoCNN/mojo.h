#ifndef MOJO_H
#define MOJO_H
//#define MOJO_AVX	// turn on AVX / SSE3 / SIMD optimizations
//#define MOJO_OMP	// allow multi-threading through openmp
#define MOJO_LUTS	// use look up tables, uses more memory
//#define MOJO_CV3	// use OpenCV 3.x utilities
//#define MOJO_CV2	// use OpenCV 2.x utilities
//#define MOJO_PROFILE_LAYERS  // std::cout layer names and latencies
//#define MOJO_INTERNAL_THREADING // try to speed forward pass with internal threading
#ifdef MOJO_OMP
#include <omp.h>
#ifndef MOJO_INTERNAL_THREADING
#define MOJO_THREAD_THIS_LOOP(a)
#define MOJO_THREAD_THIS_LOOP_DYNAMIC(a)
#else
#ifdef _WIN32
	// this macro uses OMP where the loop is split up into equal chunks per thread
#define MOJO_THREAD_THIS_LOOP(a) __pragma(omp parallel for num_threads(a))
// this macro uses OMP where the loop is split up dynamically and work is taken by next available thread
#define MOJO_THREAD_THIS_LOOP_DYNAMIC(a) __pragma(omp parallel for schedule(dynamic) num_threads(a))
#else
#define MOJO_THREAD_THIS_LOOP(a)
#define MOJO_THREAD_THIS_LOOP_DYNAMIC(a)
//#define MOJO_THREAD_THIS_LOOP(a) _Pragma("omp parallel for schedule(dynamic) num_threads(" #a ")")
//#define MOJO_THREAD_THIS_LOOP_DYNAMIC(a) _Pragma("omp parallel for schedule(dynamic) num_threads(" #a ")")
#endif
#endif //_WIN32
#else
#define MOJO_THREAD_THIS_LOOP(a)
#define MOJO_THREAD_THIS_LOOP_DYNAMIC(a)
#endif
#ifdef MOJO_AF
#include <arrayfire.h>
#define AF_RELEASE
#ifdef MOJO_CUDA
#define AF_CUDA
#pragma comment(lib, "afcuda")
#else
#define AF_CPU
#pragma comment(lib, "afcpu")
#endif
#endif
#include "core_math.h"
#include "network.h" // this is the important thing
#include "util.h"
#endif
