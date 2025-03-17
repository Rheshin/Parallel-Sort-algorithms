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
expliquer comment est faite la separation en chunks

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

expliquer l'utilisation des tasks

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
a


---

## **3. Performance Analysis**  
**System Specifications:**  
- **Processor:** AMD Ryzen 5 7500F 6-Core 12-Threads Processor 3.70 GHz
- **RAM:** 32GB
- **Compiler:** gcc
- **System:** Ubuntu 24.04 (through Windows WSL2)

### **Performance Evaluation**  
graph resultat -> observations -> conclusion

---

## **4. Conclusion**  


