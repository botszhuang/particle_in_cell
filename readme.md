# GPU-accelerated Particle-in-Cell (PIC) solver engineered in OpenCL for simulating complex plasma dynamics
![OpenCL](https://img.shields.io/badge/OpenCL-3.0-blue.svg) 
![C](https://img.shields.io/badge/C-Language-green.svg)
![License](https://img.shields.io/badge/License-All%20Rights%20Reserved-red.svg)

<div align="justify">
The Particle-In-Cell (PIC) algorithm provides a self-consistent physical framework and is widely applied across fields ranging from astrophysical and fusion plasmas to laser-plasma interactions and accelerator design.

Despite the high demand for Particle-in-Cell (PIC) simulations in both academia and industry, modern solvers are frequently bottlenecked by the immense computational overhead of field-solving. Utilizing complex numerical frameworks like the Finite Element Method (FEM) to solve Gauss’s Law for self-consistent fields demands significant processing time and extensive memory allocation. 

Even with significant computational resources invested, the particle-mesh weight scheme such as Cloud-in-Cell remains inherently limited by its first-order linear approach. While oversized clouds prevent singularities, they over-smooth fine-scale phenomena; conversely, undersized clouds trigger high-frequency numerical noise, regardless of raw processing power.

## 🚀
This project takes advantage on the multipole-based approach to solve the Gauss' law within asymmetric and complex geometric meshes, significantly enhancing geometric flexibility. The solution eliminates the heavy memory footprint overhead and data transport among the GPUs associated with traditional Sparse Matrix solvers. 

Furthermore, implemented in OpenCL, the solver ensures cross-platform portability across diverse hardware architectures. By asynchronous command queues and fine-grained memory management, the framework maximizes hardware utilization, enabling the massive parallelism required to process billions of particle-to-grid interactions on a single GPU.


🚧 **Project Status: Under Active Development**

The Multipole-based solver is currently under construction. While the foundational architecture is in place, I am actively refining the kernel performance and expanding support for complex geometric meshes.
</div>

## 💡 Community & Expert Feedback

I would like to seek advice from community experts on the following:

* Cross-Platform Compatibility: If you have access to AMD or NVIDIA environments, I would greatly appreciate your help in testing the execution performance across these different architectures.

**Any suggestions regarding code style, numerical stability, or performance optimization are incredibly valuable to me. Please feel free to try it out and share your insights!**


## 🛠️ Installation & Quick Start

### Prerequisites

* OpenCL-Capable Hardware: (NVIDIA / AMD / Intel GPU)

* OpenCL SDK: Ensure the appropriate SDK for your hardware is installed.

* Build Tools: A compilation environment supporting the **make** utility.

### Execution Steps

Run the following commands in your terminal to set up and execute the simulation:

```bash
# 1. Clone the repository
git clone https://github.com/botszhuang/particle_in_cell.git
cd particle_in_cell

# 2. Navigate to the example directory
cd 1_example

# 3. Create the output directory for simulation data
mkdir output

# 4. Clean, compile, and execute the solver
make clean
python3 initial_condition.py
make run
