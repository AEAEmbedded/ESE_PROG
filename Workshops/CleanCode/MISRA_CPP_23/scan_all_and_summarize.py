#!/usr/bin/env python3
"""
Comprehensive MISRA C++ Scanner
Scans all code files and generates a summary report
"""

import subprocess
import os
from pathlib import Path
import re


class Colors:
    """ANSI color codes"""
    RESET = '\033[0m'
    BOLD = '\033[1m'
    GREEN = '\033[92m'
    YELLOW = '\033[93m'
    BLUE = '\033[96m'
    RED = '\033[91m'


def scan_file(checker_path, file_path):
    """Scan a single file and return violation counts"""
    try:
        result = subprocess.run(
            ['python3', checker_path, file_path],
            capture_output=True,
            text=True,
            timeout=30
        )

        output = result.stdout + result.stderr

        # Extract violation counts
        mandatory = len(re.findall(r'Mandatory Violations', output))
        required_match = re.search(r'Required:\s+(\d+)', output)
        advisory_match = re.search(r'Advisory:\s+(\d+)', output)
        total_match = re.search(r'Total Violations:\s+(\d+)', output)

        required = int(required_match.group(1)) if required_match else 0
        advisory = int(advisory_match.group(1)) if advisory_match else 0
        total = int(total_match.group(1)) if total_match else 0

        return {
            'file': file_path,
            'mandatory': mandatory,
            'required': required,
            'advisory': advisory,
            'total': total,
            'output': output
        }

    except Exception as e:
        print(f"{Colors.RED}Error scanning {file_path}: {e}{Colors.RESET}")
        return None


def main():
    base_dir = "/Users/jakorten/Library/Mobile Documents/com~apple~CloudDocs/Desktop/MISRA"
    code_dir = f"{base_dir}/MoreCode_Anonymized"
    checker = f"{base_dir}/misra_cpp_checker_pretty.py"
    report_dir = f"{base_dir}/scan_results"

    # Create report directory
    os.makedirs(report_dir, exist_ok=True)

    print(f"{Colors.BLUE}{Colors.BOLD}")
    print("=" * 80)
    print(" " * 20 + "MISRA C++ Compliance Scan")
    print("=" * 80)
    print(Colors.RESET)

    # Find all C++ files
    code_path = Path(code_dir)
    cpp_files = list(code_path.rglob('*.cpp'))
    h_files = list(code_path.rglob('*.h'))
    all_files = sorted(cpp_files + h_files)

    print(f"{Colors.GREEN}Found {len(all_files)} files to scan{Colors.RESET}\n")

    results = []
    total_violations = 0
    total_required = 0
    total_advisory = 0

    # Scan each file
    for i, file_path in enumerate(all_files, 1):
        rel_path = file_path.relative_to(code_path)
        print(f"[{i}/{len(all_files)}] Scanning: {Colors.BOLD}{rel_path}{Colors.RESET}")

        result = scan_file(checker, str(file_path))

        if result:
            results.append(result)
            total_violations += result['total']
            total_required += result['required']
            total_advisory += result['advisory']

            # Save individual report
            report_file = report_dir + f"/{file_path.stem}_report.txt"
            with open(report_file, 'w') as f:
                f.write(result['output'])

            # Print summary
            if result['total'] > 0:
                print(f"   {Colors.YELLOW}Required: {result['required']}, Advisory: {result['advisory']}, Total: {result['total']}{Colors.RESET}")
            else:
                print(f"   {Colors.GREEN}✓ No violations{Colors.RESET}")

        print()

    # Print summary
    print(f"{Colors.BLUE}{Colors.BOLD}")
    print("=" * 80)
    print(" " * 28 + "SCAN SUMMARY")
    print("=" * 80)
    print(Colors.RESET)
    print()

    print(f"{Colors.BOLD}Files scanned:{Colors.RESET}       {len(results)}")
    print(f"{Colors.BOLD}Total violations:{Colors.RESET}   {total_violations}")
    print(f"  - Required:              {total_required}")
    print(f"  - Advisory:              {total_advisory}")
    print()

    # Print file-by-file summary
    print(f"{Colors.BOLD}Per-file violations:{Colors.RESET}")
    print("-" * 80)

    for result in results:
        file_name = Path(result['file']).name
        if result['total'] > 0:
            color = Colors.YELLOW if result['required'] > 0 else Colors.BLUE
            print(f"{color}{file_name:30s} : {result['total']:3d} violations (R:{result['required']:2d}, A:{result['advisory']:2d}){Colors.RESET}")
        else:
            print(f"{Colors.GREEN}{file_name:30s} : ✓ No violations{Colors.RESET}")

    print()
    print(f"{Colors.BOLD}Reports saved in:{Colors.RESET} {report_dir}")

    # Create summary report
    summary_file = f"{report_dir}/SUMMARY_REPORT.txt"
    with open(summary_file, 'w') as f:
        f.write("=" * 80 + "\n")
        f.write(" " * 20 + "MISRA C++ COMPLIANCE SCAN SUMMARY\n")
        f.write("=" * 80 + "\n\n")

        f.write(f"Files scanned:       {len(results)}\n")
        f.write(f"Total violations:    {total_violations}\n")
        f.write(f"  - Required:        {total_required}\n")
        f.write(f"  - Advisory:        {total_advisory}\n\n")

        f.write("Per-file violations:\n")
        f.write("-" * 80 + "\n")

        for result in results:
            file_name = Path(result['file']).name
            f.write(f"{file_name:30s} : {result['total']:3d} violations (Required:{result['required']:2d}, Advisory:{result['advisory']:2d})\n")

        f.write("\n" + "=" * 80 + "\n")

    print(f"{Colors.GREEN}{Colors.BOLD}Summary report created:{Colors.RESET} {summary_file}")
    print()


if __name__ == "__main__":
    main()
