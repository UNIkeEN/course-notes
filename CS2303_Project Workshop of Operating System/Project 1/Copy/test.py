import random
import string
import subprocess
import re
import matplotlib.pyplot as plt
from tabulate import tabulate

def generate_random_file(size_in_mib):
    filename = f"test_{size_in_mib}.in"
    size_in_bytes = size_in_mib * 1024 * 1024

    with open(filename, "w") as f:
        for _ in range(size_in_bytes):
            random_char = random.choice(string.ascii_letters)
            f.write(random_char)

def run_copy_and_extract(filename, x):
    cmd = f"./Copy {filename} test.out {x}"
    output = subprocess.check_output(cmd, shell=True).decode()
    result = re.findall(r'\d+\.\d+', output)
    return [float(num) for num in result]

def plot_curve(sizes_in_mib, x_values, extracted_numbers):
    plt.figure(figsize=(8, 5))
    for size in sizes_in_mib:
        filename = f"test_{size}.in"
        
        y_values = [sum(numbers) / len(numbers) / size for numbers in extracted_numbers[filename]]
        plt.plot(x_values, y_values, label=f"{size} MiB", marker="o")

    plt.xscale("log")
    plt.xlabel("Buffer size(Bytes)")
    plt.ylabel("Average time per MiB (ms/MiB)")
    plt.legend()
    plt.savefig("test_output.png", dpi=600, bbox_inches="tight")

def generate_latex_table(sizes_in_mib, x_values, extracted_numbers):
    header = ["File Size (MiB)"] + [f"{x}" for x in x_values]
    table_data = []

    for size in sizes_in_mib:
        filename = f"test_{size}.in"
        y_values = [sum(numbers) / len(numbers) / size for numbers in extracted_numbers[filename]]
        row_data = [f"{size} MiB"] + y_values
        table_data.append(row_data)

    # Manually create LaTeX table with merged cell
    latex_table = "\\begin{tabular}{l|" + "c" * len(x_values) + "}\n"
    latex_table += "\\toprule\n"
    latex_table += " & \\multicolumn{" + str(len(x_values)) + "}{c}{Buffer Size}\\\\\n"
    latex_table += "\\cmidrule(lr){2-" + str(len(x_values) + 1) + "}\n"
    latex_table += " & ".join(header) + " \\\\\n"
    latex_table += "\\midrule\n"

    for row in table_data:
        formatted_row = [str(cell) for cell in row[:1]] + ["{:.2f}".format(cell) for cell in row[1:]]
        latex_table += " & ".join(formatted_row) + " \\\\\n"

    latex_table += "\\bottomrule\n"
    latex_table += "\\end{tabular}"

    return latex_table



if __name__ == "__main__":
    sizes_in_mib = [1, 4, 8, 16, 32, 64, 128, 256]
    buffer_values = [4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384]
    extracted_numbers = {}

    for size in sizes_in_mib:
        generate_random_file(size)

    for size in sizes_in_mib:
        filename = f"test_{size}.in"
        extracted_numbers[filename] = []
        for x in buffer_values:
            numbers = run_copy_and_extract(filename, x)
            extracted_numbers[filename].append(numbers)

    plot_curve(sizes_in_mib, buffer_values, extracted_numbers)
    latex_table = generate_latex_table(sizes_in_mib, buffer_values, extracted_numbers)
    print(latex_table)