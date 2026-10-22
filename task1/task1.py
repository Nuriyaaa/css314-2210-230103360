import os
import platform
import subprocess
import time

import numpy as np

STUDENT_ID = 230103360
ARRAY_SIZE = 20_000_000


def get_cpu_model():
    """Detect the CPU model, including on macOS."""
    try:
        if platform.system() == "Darwin":
            return subprocess.check_output(
                ["sysctl", "-n", "machdep.cpu.brand_string"],
                text=True
            ).strip()

        if platform.system() == "Linux":
            with open("/proc/cpuinfo", "r") as file:
                for line in file:
                    if line.startswith("model name"):
                        return line.split(":", 1)[1].strip()

        return platform.processor() or "Not available"
    except (OSError, subprocess.SubprocessError):
        return platform.processor() or "Not available"


def main():
    print("=== TASK 1: Hardware Probe & Serial Baseline ===")
    print(f"Student ID: {STUDENT_ID}")
    print(f"Operating System: {platform.platform()}")
    print(f"CPU Model: {get_cpu_model()}")
    print(f"Logical CPU Cores: {os.cpu_count()}")

    start_time = time.perf_counter()

    rng = np.random.default_rng(seed=STUDENT_ID)
    data = rng.random(ARRAY_SIZE, dtype=np.float64)

    total_sum = np.sum(data, dtype=np.float64)
    standard_deviation = np.std(data, dtype=np.float64)

    end_time = time.perf_counter()
    elapsed_ms = (end_time - start_time) * 1000

    print(f"Array Size: {data.size:,}")
    print(f"Data Type: {data.dtype}")
    print(f"Sum: {total_sum:.10f}")
    print(f"Standard Deviation: {standard_deviation:.10f}")
    print(f"Wall-Clock Execution Time: {elapsed_ms:.3f} ms")


if __name__ == "__main__":
    main()
