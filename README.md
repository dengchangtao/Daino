# Diano
Diano is a High-level Framework for Parallel and Efficient AMR on GPUs. Adaptive Mesh Refinement methods reduce computational requirements of problems by increasing resolution for only areas of interest. However, in practice, efficient AMR implementations are difficult considering that the mesh hierarchy management must be optimized for the underlying hardware. Architecture complexity of GPUs can render efficient AMR to be particularity challenging in GPU-accelerated supercomputers. This project presents a compiler-based high-level framework that can automatically transform serial uniform mesh code annotated by the user into parallel adaptive mesh code optimized for GPU-accelerated supercomputers. Diano also includes a method for empirical analysis of a uniform mesh to project an upper-bound on achievable speedup of a GPU-optimized AMR code. We experimented on three production applications. The speedups of code generated by our framework are comparable to hand-written AMR code while achieving good and weak scaling up to 1000 GPUs.
<The source code and technical guide will be released by November 2016. Meanwhile, a full paper, appearing in SC16, can be found in the "documents" directory>
