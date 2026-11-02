# Ray Tracing In One Weekend But Faster

A multithreaded implementation of Peter Shirley's 
[*Ray Tracing in One Weekend*](https://raytracing.github.io/books/RayTracingInOneWeekend.html)
cutting render time from **20 minutes to 6 minutes**.

![Final Render]
*Need to add*

## Performance

| Version        | Render Time | Speedup |
|----------------|-------------|---------|
| Single-threaded| 20 min      | 1.0x    |
| Multithreaded  | 6 min       | 3.3x    |

*No Optimization Compiler Flags* 

### Render Settings

-Aspect Ratio: 16:9
-Resolution: 1200x675
-Samples Per Pixel: 500
-Max Ray Depth: 50

## Setup Used:
-CPU: 
-GPU: NVIDIA RTX 3060 Ti
-OS: Fedora Linux 44
-Compiler: GCC 16.2.1

## How It Works





