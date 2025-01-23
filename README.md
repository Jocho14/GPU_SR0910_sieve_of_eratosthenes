# Sieve of Eratosthenes Project

This project implements the Sieve of Eratosthenes algorithm in three different environments:

- **Single-threaded CPU**: A basic implementation of the algorithm using a single thread to compute prime numbers.
- **Multi-threaded CPU**: An optimized version of the algorithm that leverages multiple threads to divide the workload and improve performance on multi-core processors.
- **GPU Implementation**: A highly parallelized version of the algorithm designed to run on GPU hardware, utilizing CUDA for efficient computation.

The goal of this project is to explore and demonstrate the efficiency of different computational approaches for solving the same problem, particularly highlighting the performance benefits of parallel computing.

### Single-threaded CPU Implementation
The single-threaded version serves as a baseline for comparison. It processes the sieve algorithm sequentially to mark non-prime numbers in a range up to a specified limit. While simple and straightforward, this approach can become computationally expensive for very large limits.

### Multi-threaded CPU Implementation
The multi-threaded implementation divides the range of numbers among multiple threads, allowing concurrent marking of non-prime numbers. This approach makes efficient use of multi-core processors, significantly reducing computation time for large input sizes compared to the single-threaded version.

### GPU Implementation
The GPU implementation is designed to leverage the massively parallel architecture of modern GPUs. Using CUDA, the sieve algorithm is adapted to process thousands of threads simultaneously, offering unparalleled speed for very large ranges of numbers. This implementation showcases the potential of GPU acceleration for computationally intensive tasks.

### Applications
The Sieve of Eratosthenes is a fundamental algorithm in number theory with applications in:
- Cryptography (e.g., generating large prime numbers for encryption keys)
- Mathematics research
- Educational purposes (as an example of efficient algorithms)

### About the Project
This project was developed as an academic exercise to analyze the trade-offs and performance characteristics of different computing paradigms (single-threaded, multi-threaded, and GPU-based). It demonstrates how algorithm design and hardware capabilities influence computational efficiency.

