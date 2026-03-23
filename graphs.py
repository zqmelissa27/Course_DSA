import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os

# Experimental data
data = {
    "Algorithm": ["QuickSort", "HeapSort", "AVL Tree"],
    "Time_ms": [128.604, 189.136, 128.413],
    "Memory_MB": [4.395, 4.395, 8.210],
}

df = pd.DataFrame(data)
colors = ['#534AB7', '#0F6E56', '#993C1D']

# Output folder
output_dir = "C:/Users/Zqmelissa27/Desktop/Data_Structures/graphs"
os.makedirs(output_dir, exist_ok=True)  

# -----------------------------
# 1) EXECUTION TIME
# -----------------------------
plt.figure(figsize=(10, 6))
bars = plt.bar(df["Algorithm"], df["Time_ms"], color=colors, edgecolor="black", linewidth=1.2)
plt.title("Execution Time Comparison", fontsize=16, fontweight="bold")
plt.ylabel("Time (ms)", fontsize=12)
plt.grid(axis="y", alpha=0.3, linestyle="--")

for bar, value in zip(bars, df["Time_ms"]):
    h = bar.get_height()
    plt.text(bar.get_x() + bar.get_width()/2, h + 6, f"{value:.2f}",
             ha="center", va="bottom", fontweight="bold")

plt.tight_layout()
plt.savefig(os.path.join(output_dir, "execution_time_comparison.png"), dpi=300, bbox_inches="tight")
plt.close()

# -----------------------------
# 2) MEMORY USAGE
# -----------------------------
plt.figure(figsize=(10, 6))
bars = plt.bar(df["Algorithm"], df["Memory_MB"], color=colors, edgecolor="black", linewidth=1.2)
plt.title("Memory Usage Comparison", fontsize=16, fontweight="bold")
plt.ylabel("Approximate Memory (MB)", fontsize=12)
plt.grid(axis="y", alpha=0.3, linestyle="--")

for bar, value in zip(bars, df["Memory_MB"]):
    h = bar.get_height()
    plt.text(bar.get_x() + bar.get_width()/2, h + 0.12, f"{value:.2f}",
             ha="center", va="bottom", fontweight="bold")

plt.tight_layout()
plt.savefig(os.path.join(output_dir, "memory_usage_comparison.png"), dpi=300, bbox_inches="tight")
plt.close()

# -----------------------------
# 3) TIME VS MEMORY
# -----------------------------
plt.figure(figsize=(10, 6))
plt.scatter(df["Time_ms"], df["Memory_MB"], s=260, edgecolor="black", linewidth=1.2)

for i, algo in enumerate(df["Algorithm"]):
    plt.annotate(algo, (df["Time_ms"].iloc[i], df["Memory_MB"].iloc[i]),
                 xytext=(8, 8), textcoords="offset points", fontsize=10, fontweight="bold")

plt.title("Time vs Memory Trade-off", fontsize=16, fontweight="bold")
plt.xlabel("Time (ms)", fontsize=12)
plt.ylabel("Approximate Memory (MB)", fontsize=12)
plt.grid(alpha=0.3, linestyle="--")
plt.tight_layout()
plt.savefig(os.path.join(output_dir, "time_vs_memory_tradeoff.png"), dpi=300, bbox_inches="tight")
plt.close()

# -----------------------------
# 4) THEORETICAL COMPLEXITY
# -----------------------------
# Same asymptotic class for average QuickSort, HeapSort, and AVL-based sort,

n = np.array([100, 1000, 10000, 100000, 1000000], dtype=float)

base_nlogn = n * np.log2(n)
quick_avg = 1.00 * base_nlogn / 1000
heap = 1.08 * base_nlogn / 1000
avl = 1.03 * base_nlogn / 1000

linear = n / 100
quick_worst = (n ** 2) / 10000

plt.figure(figsize=(11, 7))
plt.plot(n, quick_avg, marker="o", linewidth=2.4, label="QuickSort (average): O(n log n)")
plt.plot(n, heap, marker="s", linewidth=2.4, label="HeapSort: O(n log n)")
plt.plot(n, avl, marker="^", linewidth=2.4, label="AVL-based sort: O(n log n)")
plt.plot(n, linear, marker="D", linewidth=2.2, linestyle=":", label="Linear reference: O(n)")
plt.plot(n, quick_worst, marker="x", linewidth=2.2, linestyle="--", label="QuickSort (worst case): O(n^2)")

plt.xscale("log")
plt.yscale("log")
plt.title("Theoretical Algorithmic Complexity Comparison", fontsize=16, fontweight="bold")
plt.xlabel("Input Size (n)", fontsize=12)
plt.ylabel("Scaled Operations", fontsize=12)
plt.grid(True, which="both", alpha=0.3, linestyle="--")
plt.legend()
plt.tight_layout()
plt.savefig(os.path.join(output_dir, "theoretical_complexity_comparison.png"), dpi=300, bbox_inches="tight")
plt.close()

print("Saved files:")
for name in [
    "execution_time_comparison.png",
    "memory_usage_comparison.png",
    "time_vs_memory_tradeoff.png",
    "theoretical_complexity_comparison.png",
]:
    print(" -", os.path.join(output_dir, name))
