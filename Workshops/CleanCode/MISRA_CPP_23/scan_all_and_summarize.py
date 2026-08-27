#!/usr/bin/env python3
"""
MISRA C++ Scanner - SOLID Refactored Version
==============================================

Refactored to follow SOLID principles:
- Single Responsibility Principle (SRP)
- Open/Closed Principle (OCP)
- Liskov Substitution Principle (LSP)
- Interface Segregation Principle (ISP)
- Dependency Inversion Principle (DIP)

Usage:
    python3 scan_all_and_summarize_solid.py
"""

import subprocess
import os
from pathlib import Path
import re
from typing import List, Dict, Protocol, Optional
from dataclasses import dataclass
from abc import ABC, abstractmethod


# ============================================================================
# DOMAIN MODELS (Value Objects) - SRP
# ============================================================================

class Color:
    """Immutable color codes value object"""
    RESET = '\033[0m'
    BOLD = '\033[1m'
    GREEN = '\033[92m'
    YELLOW = '\033[93m'
    BLUE = '\033[96m'
    CYAN = '\033[96m'
    RED = '\033[91m'


@dataclass(frozen=True)
class ScanResult:
    """Immutable scan result value object"""
    file_path: str
    mandatory: int
    required: int
    advisory: int
    total: int
    output: str


@dataclass(frozen=True)
class ScanSummary:
    """Immutable summary statistics value object"""
    files_scanned: int
    total_violations: int
    total_required: int
    total_advisory: int
    total_mandatory: int
    results: List[ScanResult]


@dataclass(frozen=True)
class ScanConfiguration:
    """Immutable configuration value object"""
    code_directory: str
    checker_path: str
    report_directory: str
    timeout_seconds: int = 30


# ============================================================================
# INTERFACES (ISP - Interface Segregation Principle)
# ============================================================================

class FileScanner(Protocol):
    """Interface for scanning files (ISP)"""
    def scan(self, file_path: str) -> Optional[ScanResult]:
        """Scan a single file"""
        ...


class FileFinder(Protocol):
    """Interface for finding files (ISP)"""
    def find_cpp_files(self, directory: str) -> List[str]:
        """Find C++ files in directory"""
        ...


class ReportWriter(Protocol):
    """Interface for writing reports (ISP)"""
    def write_individual_report(self, result: ScanResult, output_path: str) -> None:
        """Write individual file report"""
        ...

    def write_summary_report(self, summary: ScanSummary, output_path: str) -> None:
        """Write summary report"""
        ...


class ProgressReporter(Protocol):
    """Interface for reporting progress (ISP)"""
    def report_start(self, total_files: int) -> None:
        """Report scan start"""
        ...

    def report_file_progress(self, current: int, total: int, file_name: str) -> None:
        """Report file scan progress"""
        ...

    def report_file_result(self, result: ScanResult) -> None:
        """Report individual file result"""
        ...

    def report_summary(self, summary: ScanSummary) -> None:
        """Report final summary"""
        ...


# ============================================================================
# RESULT PARSER (SRP - Single Responsibility)
# ============================================================================

class MISRAScanResultParser:
    """Parses MISRA checker output - single responsibility"""

    def parse(self, output: str, file_path: str) -> ScanResult:
        """Parse scanner output into structured result"""
        mandatory = len(re.findall(r'Mandatory Violations', output))

        required_match = re.search(r'Required:\s+(\d+)', output)
        advisory_match = re.search(r'Advisory:\s+(\d+)', output)
        total_match = re.search(r'Total Violations:\s+(\d+)', output)

        required = int(required_match.group(1)) if required_match else 0
        advisory = int(advisory_match.group(1)) if advisory_match else 0
        total = int(total_match.group(1)) if total_match else 0

        return ScanResult(
            file_path=file_path,
            mandatory=mandatory,
            required=required,
            advisory=advisory,
            total=total,
            output=output
        )


# ============================================================================
# CONCRETE IMPLEMENTATIONS (SRP)
# ============================================================================

class SubprocessFileScanner:
    """Scans files using subprocess - single responsibility"""

    def __init__(self, checker_path: str, parser: MISRAScanResultParser, timeout: int = 30):
        self._checker_path = checker_path
        self._parser = parser
        self._timeout = timeout

    def scan(self, file_path: str) -> Optional[ScanResult]:
        """Scan a single file using MISRA checker"""
        try:
            result = subprocess.run(
                ['python3', self._checker_path, file_path],
                capture_output=True,
                text=True,
                timeout=self._timeout
            )

            output = result.stdout + result.stderr
            return self._parser.parse(output, file_path)

        except subprocess.TimeoutExpired:
            print(f"{Color.RED}✗ Timeout scanning {file_path}{Color.RESET}")
            return None
        except Exception as e:
            print(f"{Color.RED}✗ Error scanning {file_path}: {e}{Color.RESET}")
            return None


class CppFileFinder:
    """Finds C++ files in directory - single responsibility"""

    def find_cpp_files(self, directory: str) -> List[str]:
        """Find all .cpp and .h files recursively"""
        code_path = Path(directory)

        if not code_path.exists():
            return []

        cpp_files = list(code_path.rglob('*.cpp'))
        h_files = list(code_path.rglob('*.h'))
        hpp_files = list(code_path.rglob('*.hpp'))

        all_files = sorted(cpp_files + h_files + hpp_files)
        return [str(f) for f in all_files]


class FileSystemReportWriter:
    """Writes reports to filesystem - single responsibility"""

    def __init__(self, report_directory: str):
        self._report_directory = report_directory
        os.makedirs(report_directory, exist_ok=True)

    def write_individual_report(self, result: ScanResult, output_path: str) -> None:
        """Write individual file report"""
        try:
            with open(output_path, 'w') as f:
                f.write(result.output)
        except Exception as e:
            print(f"{Color.RED}✗ Error writing report: {e}{Color.RESET}")

    def write_summary_report(self, summary: ScanSummary, output_path: str) -> None:
        """Write summary report"""
        try:
            with open(output_path, 'w') as f:
                f.write("=" * 80 + "\n")
                f.write(" " * 20 + "MISRA C++ COMPLIANCE SCAN SUMMARY\n")
                f.write("=" * 80 + "\n\n")

                f.write(f"Files scanned:       {summary.files_scanned}\n")
                f.write(f"Total violations:    {summary.total_violations}\n")
                f.write(f"  - Mandatory:       {summary.total_mandatory}\n")
                f.write(f"  - Required:        {summary.total_required}\n")
                f.write(f"  - Advisory:        {summary.total_advisory}\n\n")

                f.write("Per-file violations:\n")
                f.write("-" * 80 + "\n")

                for result in summary.results:
                    file_name = Path(result.file_path).name
                    f.write(f"{file_name:30s} : {result.total:3d} violations "
                           f"(M:{result.mandatory:2d}, R:{result.required:2d}, A:{result.advisory:2d})\n")

                f.write("\n" + "=" * 80 + "\n")
        except Exception as e:
            print(f"{Color.RED}✗ Error writing summary: {e}{Color.RESET}")


class ConsoleProgressReporter:
    """Reports progress to console - single responsibility"""

    def report_start(self, total_files: int) -> None:
        """Report scan start"""
        print(f"{Color.BLUE}{Color.BOLD}")
        print("=" * 80)
        print(" " * 20 + "MISRA C++ COMPLIANCE SCAN")
        print("=" * 80)
        print(Color.RESET)
        print(f"{Color.GREEN}Found {total_files} files to scan{Color.RESET}\n")

    def report_file_progress(self, current: int, total: int, file_name: str) -> None:
        """Report file scan progress"""
        print(f"[{current}/{total}] Scanning: {Color.BOLD}{file_name}{Color.RESET}")

    def report_file_result(self, result: ScanResult) -> None:
        """Report individual file result"""
        if result.total > 0:
            print(f"   {Color.YELLOW}Mandatory: {result.mandatory}, Required: {result.required}, "
                  f"Advisory: {result.advisory}, Total: {result.total}{Color.RESET}")
        else:
            print(f"   {Color.GREEN}✓ No violations{Color.RESET}")
        print()

    def report_summary(self, summary: ScanSummary) -> None:
        """Report final summary"""
        print(f"{Color.BLUE}{Color.BOLD}")
        print("=" * 80)
        print(" " * 28 + "SCAN SUMMARY")
        print("=" * 80)
        print(Color.RESET)
        print()

        print(f"{Color.BOLD}Files scanned:{Color.RESET}       {summary.files_scanned}")
        print(f"{Color.BOLD}Total violations:{Color.RESET}   {summary.total_violations}")
        print(f"  - Mandatory:             {summary.total_mandatory}")
        print(f"  - Required:              {summary.total_required}")
        print(f"  - Advisory:              {summary.total_advisory}")
        print()

        print(f"{Color.BOLD}Per-file violations:{Color.RESET}")
        print("-" * 80)

        for result in summary.results:
            file_name = Path(result.file_path).name
            if result.total > 0:
                color = Color.RED if result.mandatory > 0 else Color.YELLOW if result.required > 0 else Color.BLUE
                print(f"{color}{file_name:30s} : {result.total:3d} violations "
                      f"(M:{result.mandatory:2d}, R:{result.required:2d}, A:{result.advisory:2d}){Color.RESET}")
            else:
                print(f"{Color.GREEN}{file_name:30s} : ✓ No violations{Color.RESET}")
        print()


# ============================================================================
# STATISTICS CALCULATOR (SRP)
# ============================================================================

class ScanStatisticsCalculator:
    """Calculates summary statistics - single responsibility"""

    def calculate(self, results: List[ScanResult]) -> ScanSummary:
        """Calculate summary from scan results"""
        total_violations = sum(r.total for r in results)
        total_required = sum(r.required for r in results)
        total_advisory = sum(r.advisory for r in results)
        total_mandatory = sum(r.mandatory for r in results)

        return ScanSummary(
            files_scanned=len(results),
            total_violations=total_violations,
            total_required=total_required,
            total_advisory=total_advisory,
            total_mandatory=total_mandatory,
            results=results
        )


# ============================================================================
# SCAN ORCHESTRATOR (SRP - Orchestration only)
# ============================================================================

class MISRAScanOrchestrator:
    """
    Orchestrates the scanning process - single responsibility

    Uses dependency injection (DIP)
    """

    def __init__(self,
                 scanner: FileScanner,
                 file_finder: FileFinder,
                 report_writer: ReportWriter,
                 progress_reporter: ProgressReporter,
                 stats_calculator: ScanStatisticsCalculator):
        self._scanner = scanner
        self._file_finder = file_finder
        self._report_writer = report_writer
        self._progress_reporter = progress_reporter
        self._stats_calculator = stats_calculator

    def scan_directory(self, config: ScanConfiguration) -> ScanSummary:
        """Scan all C++ files in directory"""
        # Find files
        file_paths = self._file_finder.find_cpp_files(config.code_directory)

        # Report start
        self._progress_reporter.report_start(len(file_paths))

        # Scan each file
        results = []
        for i, file_path in enumerate(file_paths, 1):
            file_name = Path(file_path).name
            self._progress_reporter.report_file_progress(i, len(file_paths), file_name)

            result = self._scanner.scan(file_path)

            if result:
                results.append(result)
                self._progress_reporter.report_file_result(result)

                # Write individual report
                report_path = os.path.join(
                    config.report_directory,
                    f"{Path(file_path).stem}_report.txt"
                )
                self._report_writer.write_individual_report(result, report_path)

        # Calculate summary
        summary = self._stats_calculator.calculate(results)

        # Write summary report
        summary_path = os.path.join(config.report_directory, "SUMMARY_REPORT.txt")
        self._report_writer.write_summary_report(summary, summary_path)

        # Report summary
        self._progress_reporter.report_summary(summary)

        print(f"{Color.BOLD}Reports saved in:{Color.RESET} {config.report_directory}")
        print(f"{Color.GREEN}{Color.BOLD}Summary report created:{Color.RESET} {summary_path}")
        print()

        return summary


# ============================================================================
# CONFIGURATION BUILDER (Builder Pattern)
# ============================================================================

class ScanConfigurationBuilder:
    """Builds scan configuration - single responsibility"""

    def __init__(self):
        self._base_dir = "/Users/jakorten/Library/Mobile Documents/com~apple~CloudDocs/Desktop/MISRA"
        self._code_dir = None
        self._checker = None
        self._report_dir = None
        self._timeout = 30

    def with_code_directory(self, code_dir: str) -> 'ScanConfigurationBuilder':
        """Set code directory"""
        self._code_dir = code_dir
        return self

    def with_checker(self, checker: str) -> 'ScanConfigurationBuilder':
        """Set checker path"""
        self._checker = checker
        return self

    def with_report_directory(self, report_dir: str) -> 'ScanConfigurationBuilder':
        """Set report directory"""
        self._report_dir = report_dir
        return self

    def with_timeout(self, timeout: int) -> 'ScanConfigurationBuilder':
        """Set timeout"""
        self._timeout = timeout
        return self

    def build_default(self) -> ScanConfiguration:
        """Build default configuration"""
        return ScanConfiguration(
            code_directory=f"{self._base_dir}/MoreCode_Anonymized",
            checker_path=f"{self._base_dir}/misra_cpp_checker_pretty.py",
            report_directory=f"{self._base_dir}/scan_results",
            timeout_seconds=30
        )

    def build(self) -> ScanConfiguration:
        """Build configuration from builder state"""
        if not all([self._code_dir, self._checker, self._report_dir]):
            return self.build_default()

        return ScanConfiguration(
            code_directory=self._code_dir,
            checker_path=self._checker,
            report_directory=self._report_dir,
            timeout_seconds=self._timeout
        )


# ============================================================================
# DEPENDENCY INJECTION CONTAINER (DIP)
# ============================================================================

class DIContainer:
    """Dependency injection container - wires everything together"""

    @staticmethod
    def create_scanner_application() -> MISRAScanOrchestrator:
        """Create scan application with all dependencies"""

        # Build configuration
        config_builder = ScanConfigurationBuilder()
        config = config_builder.build_default()

        # Create dependencies
        parser = MISRAScanResultParser()
        scanner = SubprocessFileScanner(config.checker_path, parser, config.timeout_seconds)
        file_finder = CppFileFinder()
        report_writer = FileSystemReportWriter(config.report_directory)
        progress_reporter = ConsoleProgressReporter()
        stats_calculator = ScanStatisticsCalculator()

        # Create and return orchestrator
        return MISRAScanOrchestrator(
            scanner=scanner,
            file_finder=file_finder,
            report_writer=report_writer,
            progress_reporter=progress_reporter,
            stats_calculator=stats_calculator
        )


# ============================================================================
# APPLICATION ENTRY POINT
# ============================================================================

class ScanApplication:
    """Main application - thin wrapper around orchestrator"""

    def __init__(self, orchestrator: MISRAScanOrchestrator, config: ScanConfiguration):
        self._orchestrator = orchestrator
        self._config = config

    def run(self) -> None:
        """Run the scan"""
        try:
            summary = self._orchestrator.scan_directory(self._config)

            # Could do something with summary here
            # (e.g., check thresholds, send notifications, etc.)

        except KeyboardInterrupt:
            print(f"\n{Color.YELLOW}Scan interrupted by user{Color.RESET}")
        except Exception as e:
            print(f"{Color.RED}✗ Scan failed: {e}{Color.RESET}")
            raise


def main():
    """Entry point using dependency injection"""

    # Create application using DI container
    orchestrator = DIContainer.create_scanner_application()

    # Build configuration
    config = ScanConfigurationBuilder().build_default()

    # Create and run application
    app = ScanApplication(orchestrator, config)
    app.run()


if __name__ == "__main__":
    main()
