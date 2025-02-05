import matplotlib.pyplot as plt
import numpy as np

# List of permutations
permutations = ["mnk", "mkn", "nmk", "nkm", "kmn", "knm"]

plt.figure(figsize=(8, 6))

# Loop through each permutation and plot the data
for perm in permutations:
    filename = f"{perm}_results.dat"
    try:
        data = np.loadtxt(filename)
        mem_footprint = data[:, 0]
        lups = data[:, 1]
        plt.plot(mem_footprint, lups, marker='o', linestyle='-', label=perm)
    except Exception as e:
        print(f"Could not read {filename}: {e}")

# Set logarithmic scale for x-axis
plt.xscale("log")

# Cache border values in bytes
cache_borders = [192 * 1024, 1.5 * 1024 * 1024, 12 * 1024 * 1024]
cache_labels = ["L1: 192 KiB", "L2: 1.5 MiB", "L3: 12 MiB"]

# Add vertical lines for cache borders, each with different colors
colors = ['r', 'g', 'b']
for border, label, color in zip(cache_borders, cache_labels, colors):
    plt.axvline(x=border, linestyle='--', alpha=0.7, color=color, label=label)

plt.xlabel("Memory Footprint (bytes)")
plt.ylabel("LUPs")
plt.title("Performance of Different Permutations")
plt.legend()
plt.grid(True, which='both', linestyle='--', linewidth=0.5)
plt.savefig("results.png")
