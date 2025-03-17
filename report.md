# **Parallel Sorting Algorithms using OpenMP**  
**Lab 2 Report**  
**M1 Info/MOSIG: Parallel Algorithms and Programming**  
**Université Grenoble Alpes**  
**Authors: ROUILLARD Hugo, REBHI Hichem**

## **1. Introduction**  
This report presents the implementation and analysis of parallel sorting algorithms using OpenMP. The sorting algorithms implemented include. 
Additionally, a performance analysis of these algorithms was conducted to evaluate their efficiency with different numbers of threads.

---

## **2. Implementations and Results**  

### **2.1 Bubble Sort**  
#### **Implementation Details**  

We start the algorithm by splitting the array into equal-size chunks. Each thread handles one chunk and sorts it.
This method avoids any conflict between threads when sorting.
In the second step, we sort the borders between each pair of chunks.
If there is a change in any of the two steps, the algorithm runs again.

#### **Limitations & Bugs (if any)**  

The second part of our algorithm is not parallelizable, since it goes through the whole array to sort the borders in each of the chunks.
This limitation down-scales performance. 

---

### **2.2 MergeSort with Tasks**  
#### **Implementation Details**  


The parallelized merge-sort algorithm is very similar to the serialized one.
We  introduced the concept of tasks  in each recursive call. This allows a single thread to create a recursive call, and inside that call, the work is done in parallel.

We added a task wait at the end of each recursive call to ensure the array is sorted only after all sub-parts are processed. This prevents conflicts and poorly managed concurrency.

#### **Optimization Proposal**  
If the array is really small, we should use a sequential workspace, we should avoir intializing tasks for small work loads.
This could allow our algorithm to have a correct performance in every situation.

#### **Limitations**  
For the moment we have a parallel version that is slower than the sequential one.
at first we thought that this occured only in small arrays because of the heavy initialization of tasks, but the performance analysis proved us wrong.

---

### **2.3 Odd-Even Sort**  
#### **Why is it more parallelizable than Bubble Sort?**  

The bubble sort algorithm is a strictly sequential sort algorithm, there is dependencies between each iteration, forcing to have a part that is not parallelized.
Meanwhile the Odd-Even algorithm does not have this restriction, since each iteration is independant for others ( there is no shared element ).
This makes the Odd-Even algorithm more parallelizable.

#### **Implementation Details**  
There is a similar point with the bubble sort algorithm, we have a shared variable named swaps, that checks if there was any element swap. 
The sorting process continues until no further swaps occur, reaching a fixed point.

Processing Odd elements and then even elements allows us to have a set of distinct pairs to sort each time, Meaning that we can introduce parallelizing in each loop iteration ( of one the two parts ).
allowing us to upscale our performance since there is parallelism in each part of the algorithm.

We added the OPENMP directive " #pragma omp parallel for reduction(swaps)"  to ensure that the swap variable incrementation is handled correctly, this variable shared in a critical section, which could lead to a race condition.


---

## **3. Performance Analysis**  
**System Specifications:**  
- **Processor:** AMD Ryzen 5 7500F 6-Core 12-Threads Processor 3.70 GHz  
- **RAM:** 32GB  
- **Compiler:** GCC  
- **System:** Ubuntu 24.04 (through Windows WSL2)  

### **Speedup vs Threads**  
The following graphs illustrate the speedup achieved by each sorting algorithm when increasing the number of threads. The speedup is calculated as:  

#### **Bubble Sort**
![Bubble Sort Speedup](https://github.com/Rheshin/Parallel-Sort-algorithms/blob/main/perf_analysis/speedup_vs_threads_bubble.png?raw=true)

#### **Merge Sort**
![Merge Sort Speedup](https://github.com/Rheshin/Parallel-Sort-algorithms/blob/main/perf_analysis/speedup_vs_threads_mergesort.png?raw=true)

#### **Odd-Even Sort**
![Odd-Even Sort Speedup](https://github.com/Rheshin/Parallel-Sort-algorithms/blob/main/perf_analysis/speedup_vs_threads_odd-even.png?raw=true)

### **Expected vs Actual Behavior**  

#### **Bubble Sort**
- **Expected:** Limited speedup due to its inherently sequential nature, but a small improvement due to parallel processing of chunks.  
- **Actual:** Little to no speedup (0.004 → 0.012), confirming poor scalability.

#### **Merge Sort**
- **Expected:** Significant speedup due to its divide-and-conquer nature, where each recursive call should be independently parallelizable, leading to a logarithmic reduction in execution time.  
- **Actual:** Performance degraded (0.05 → 0.00 speedup). This result is surprising, as merge sort should benefit a lot from parallelization.

#### **Odd-Even Sort**
- **Expected:** Moderate speedup due to its parallel-friendly nature (each phase can be processed independently). 
- **Actual:** Speedup decreases (0.82 → 0.22). This suggests that beyond a small number of threads, synchronization overhead outweighs the benefits of parallelization.  

---

### **Execution Time vs Problem Size**  
The following graphs show how the execution time of each algorithm scales with increasing problem size. The tests were conducted with different numbers of threads, as shown in the plots.

#### **Bubble Sort**
![Bubble Sort Execution Time](https://github.com/Rheshin/Parallel-Sort-algorithms/blob/main/perf_analysis/execution_time_vs_problem_size_bubble.png?raw=true)

#### **Merge Sort**
![Merge Sort Execution Time](https://github.com/Rheshin/Parallel-Sort-algorithms/blob/main/perf_analysis/execution_time_vs_problem_size_mergesort.png?raw=true)

#### **Odd-Even Sort**
![Odd-Even Sort Execution Time](https://github.com/Rheshin/Parallel-Sort-algorithms/blob/main/perf_analysis/execution_time_vs_problem_size_odd-even.png?raw=true)

### **Expected vs Actual Behavior**  

#### **Bubble Sort**
- **Expected:** A sharp increase in execution time with problem size, since it has a time complexity of **O(n²)**.  
- **Actual:** Execution time increases significantly, with little benefit from parallelism. **Expected behavior confirmed.**  

#### **Merge Sort**
- **Expected:** Execution time should grow **logarithmically (O(n log n))**, with parallel execution reducing the time significantly for larger inputs.  
- **Actual:** **Execution time did not improve with parallelization.** This is unexpected and indicates a problem with:
  - **Task creation overhead.** Too many small tasks can slow execution.
  - **Inefficient memory access.** The merge phase may be causing excessive cache misses.
  - **Synchronization costs.** If threads are frequently waiting on each other, parallelism is ineffective.

#### **Odd-Even Sort**
- **Expected:** Execution time should scale better than Bubble Sort but still be less efficient than Merge Sort.  
- **Actual:** The parallel version **performs worse as the number of threads increases.** This suggests that at higher thread counts, **contention between threads for synchronization dominates execution time**, reducing performance.  

---

## **4. Conclusion**  
The test results mostly aligned with theoretical expectations, except for merge sort, which should have benefited the most from parallelization. Instead, it saw no speedup or even a decline in performance.  

Possible reasons include:  
- Too much overhead from task creation. OpenMP task parallelism may not be efficient for small subarrays.  
- Poor load balancing. Some threads may have finishde early while others handled larger portions of work.  

**Future improvements could involve:**  
- Optimizing Merge Sort task creation: Limit the depth at which recursive calls create new tasks to avoid excessive overhead.  
- Tuning thread scheduling: Experiment with different OpenMP scheduling policies to improve workload distribution.  

---

## **5. References** 
LLMs Usage: 
- Report formatting/styling
- Performance analysis scripting
