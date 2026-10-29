
import argparse
import json
import platform
import subprocess
import sys
from pathlib import Path

from salt import get_student_params


PROJECT_DIR = Path(__file__).resolve().parent.parent
RESULTS_DIR = PROJECT_DIR / "results"


def run_command(command):
    try:
        result = subprocess.run(
            command,
            capture_output=True,
            text=True,
            check=True,
        )
        return result.stdout.strip()
    except (subprocess.CalledProcessError, FileNotFoundError):
        return None


def get_sysctl_value(key):
    return run_command(["sysctl", "-n", key])


def get_integer_sysctl(key):
    value = get_sysctl_value(key)
    if value is None:
        return None
    try:
        return int(value)
    except ValueError:
        return None


def collect_host_profile():
    physical_cores = get_integer_sysctl("hw.physicalcpu")
    logical_cores = get_integer_sysctl("hw.logicalcpu")

    cpu_name = get_sysctl_value("machdep.cpu.brand_string")

    if cpu_name is None:
        cpu_name = platform.processor() or "Unknown"

    os_version = run_command(["sw_vers", "-productVersion"])
    kernel_version = platform.release()

    return {
        "cpu_model": cpu_name,
        "architecture": platform.machine(),
        "physical_cores": physical_cores,
        "logical_threads": logical_cores,
        "smt_or_hyperthreading": (
            logical_cores > physical_cores
            if physical_cores is not None and logical_cores is not None
            else None
        ),
        "cache": {
            "cache_line_bytes": get_integer_sysctl("hw.cachelinesize"),
            "l1_data_bytes": get_integer_sysctl("hw.l1dcachesize"),
            "l2_bytes": get_integer_sysctl("hw.l2cachesize"),
            "l3_bytes": None,
            "l3_note": (
                "No verified shared L3 value obtained from the queried sysctl keys."
            ),
            "performance_cores": {
                "l1_data_bytes": get_integer_sysctl(
                    "hw.perflevel0.l1dcachesize"
                ),
                "l2_bytes": get_integer_sysctl(
                    "hw.perflevel0.l2cachesize"
                ),
            },
            "efficiency_cores": {
                "l1_data_bytes": get_integer_sysctl(
                    "hw.perflevel1.l1dcachesize"
                ),
                "l2_bytes": get_integer_sysctl(
                    "hw.perflevel1.l2cachesize"
                ),
            },
        },
        "operating_system": platform.system(),
        "os_version": os_version,
        "kernel_version": kernel_version,
        "python_implementation": platform.python_implementation(),
        "python_version": platform.python_version(),
        "python_executable": sys.executable,
    }


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--student-id", required=True)
    args = parser.parse_args()

    RESULTS_DIR.mkdir(parents=True, exist_ok=True)

    host_profile = collect_host_profile()
    salted_params = get_student_params(args.student_id)

    host_path = RESULTS_DIR / "host_profile.json"
    params_path = RESULTS_DIR / "salted_params.json"

    host_path.write_text(
        json.dumps(host_profile, indent=2),
        encoding="utf-8",
    )
    params_path.write_text(
        json.dumps(salted_params, indent=2),
        encoding="utf-8",
    )

    print(f"Saved: {host_path}")
    print(f"Saved: {params_path}")
    print(json.dumps(salted_params, indent=2))


if __name__ == "__main__":
    main()
