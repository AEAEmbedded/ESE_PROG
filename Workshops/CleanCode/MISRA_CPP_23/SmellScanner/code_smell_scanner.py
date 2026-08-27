#!/usr/bin/env python3
"""
Code Smell Scanner for C++ - SOLID Refactored Version
======================================================

Refactored to follow SOLID principles:
- Single Responsibility Principle (SRP)
- Open/Closed Principle (OCP)
- Liskov Substitution Principle (LSP)
- Interface Segregation Principle (ISP)
- Dependency Inversion Principle (DIP)

Usage:
    python3 code_smell_scanner_solid.py <file.cpp> [--verbose]
    python3 code_smell_scanner_solid.py <directory> [--verbose]
"""

import re
import sys
import argparse
from pathlib import Path
from typing import List, Dict, Protocol, Optional
from dataclasses import dataclass
from enum import Enum
from collections import defaultdict
from abc import ABC, abstractmethod


# ============================================================================
# DOMAIN MODELS (Value Objects)
# ============================================================================

class Color:
    """ANSI color codes - immutable value object"""
    RED = '\033[91m'
    GREEN = '\033[92m'
    YELLOW = '\033[93m'
    BLUE = '\033[94m'
    MAGENTA = '\033[95m'
    CYAN = '\033[96m'
    WHITE = '\033[97m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'
    RESET = '\033[0m'


class Severity(Enum):
    """Code smell severity levels"""
    CRITICAL = "Critical"
    MAJOR = "Major"
    MINOR = "Minor"
    INFO = "Info"


@dataclass(frozen=True)
class CodeSmell:
    """Immutable code smell value object"""
    name: str
    severity: Severity
    file_path: str
    line_number: int
    description: str
    suggestion: str
    context: str = ""


@dataclass(frozen=True)
class CodeFile:
    """Immutable code file value object"""
    path: str
    lines: List[str]


# ============================================================================
# INTERFACES (ISP - Interface Segregation Principle)
# ============================================================================

class SmellDetector(Protocol):
    """Interface for smell detectors (ISP)"""
    def detect(self, code_file: CodeFile) -> List[CodeSmell]:
        """Detect code smells in a file"""
        ...


class ReportFormatter(Protocol):
    """Interface for report formatters (ISP)"""
    def format(self, results: Dict[str, List[CodeSmell]]) -> str:
        """Format scan results into a report"""
        ...


class OutputWriter(Protocol):
    """Interface for output writers (ISP)"""
    def write(self, content: str) -> None:
        """Write content to output"""
        ...


# ============================================================================
# BASE CLASSES (OCP - Open/Closed Principle)
# ============================================================================

class AbstractSmellDetector(ABC):
    """
    Abstract base class for smell detectors (OCP)

    Open for extension (subclass), closed for modification
    """

    def __init__(self, name: str, severity: Severity):
        self._name = name
        self._severity = severity

    @abstractmethod
    def _detect_smells(self, code_file: CodeFile) -> List[CodeSmell]:
        """Subclasses implement specific detection logic"""
        pass

    def detect(self, code_file: CodeFile) -> List[CodeSmell]:
        """Template method - calls abstract _detect_smells"""
        return self._detect_smells(code_file)

    def _create_smell(self, code_file: CodeFile, line_number: int,
                     description: str, suggestion: str, context: str = "") -> CodeSmell:
        """Factory method for creating smells"""
        return CodeSmell(
            name=self._name,
            severity=self._severity,
            file_path=code_file.path,
            line_number=line_number,
            description=description,
            suggestion=suggestion,
            context=context
        )


# ============================================================================
# CONCRETE DETECTORS (SRP - Single Responsibility Principle)
# ============================================================================

class LongMethodDetector(AbstractSmellDetector):
    """Detects methods that are too long (SRP - single responsibility)"""

    def __init__(self, max_lines: int = 50):
        super().__init__("Long Method", Severity.MAJOR)
        self._max_lines = max_lines

    def _detect_smells(self, code_file: CodeFile) -> List[CodeSmell]:
        smells = []
        in_function = False
        func_start = 0
        func_name = ""
        brace_count = 0

        for i, line in enumerate(code_file.lines, 1):
            stripped = line.strip()

            if not in_function:
                func_match = re.search(r'^\s*(?:virtual\s+)?(?:static\s+)?(?:inline\s+)?'
                                      r'(?:\w+(?:\s*::\s*\w+)*\s+)?'
                                      r'(\w+)\s*\([^)]*\)\s*(?:const\s*)?(?:override\s*)?'
                                      r'(?:final\s*)?(?:noexcept\s*)?(?:\{|$)', line)
                if func_match and '{' in line:
                    func_name = func_match.group(1)
                    func_start = i
                    in_function = True
                    brace_count = line.count('{') - line.count('}')
            else:
                brace_count += line.count('{') - line.count('}')

                if brace_count == 0:
                    func_length = i - func_start + 1
                    if func_length > self._max_lines:
                        smell = self._create_smell(
                            code_file, func_start,
                            f"Method '{func_name}' is {func_length} lines long (max: {self._max_lines})",
                            "Break down into smaller methods (Extract Method refactoring)",
                            f"Lines {func_start}-{i}"
                        )
                        smells.append(smell)
                    in_function = False
                    func_name = ""

        return smells


class MagicNumberDetector(AbstractSmellDetector):
    """Detects magic numbers (SRP)"""

    def __init__(self, allowed_numbers: set = None):
        super().__init__("Magic Number", Severity.MINOR)
        self._allowed = allowed_numbers or {0, 1, -1, 2}

    def _detect_smells(self, code_file: CodeFile) -> List[CodeSmell]:
        smells = []

        for i, line in enumerate(code_file.lines, 1):
            # Skip comments and const declarations
            if '//' in line:
                code_part = line[:line.index('//')]
            else:
                code_part = line

            # Remove strings
            code_part = re.sub(r'"[^"]*"', '', code_part)
            code_part = re.sub(r"'[^']*'", '', code_part)

            # Skip const/constexpr declarations
            if re.match(r'^\s*#define', line) or re.search(r'\b(const|constexpr)\b', line):
                continue

            # Find numeric literals
            numbers = re.findall(r'\b(\d+\.?\d*)\b', code_part)
            for num_str in numbers:
                try:
                    num = float(num_str) if '.' in num_str else int(num_str)
                    if num not in self._allowed and num > 2:
                        smell = self._create_smell(
                            code_file, i,
                            f"Magic number '{num}' without explanation",
                            "Replace with named constant (const/constexpr)",
                            line.strip()
                        )
                        smells.append(smell)
                except ValueError:
                    pass

        return smells


class DeadCodeDetector(AbstractSmellDetector):
    """Detects dead/unreachable code (SRP)"""

    def __init__(self):
        super().__init__("Dead Code", Severity.MAJOR)

    def _detect_smells(self, code_file: CodeFile) -> List[CodeSmell]:
        smells = []

        for i, line in enumerate(code_file.lines, 1):
            stripped = line.strip()

            # Code after return
            if re.search(r'\breturn\b.*;', stripped):
                if i < len(code_file.lines):
                    next_line = code_file.lines[i].strip()
                    if next_line and not next_line.startswith('}') and not next_line.startswith('//'):
                        smell = self._create_smell(
                            code_file, i + 1,
                            "Code after return statement is unreachable",
                            "Remove unreachable code",
                            next_line
                        )
                        smells.append(smell)

        return smells


class GodClassDetector(AbstractSmellDetector):
    """Detects God classes (SRP)"""

    def __init__(self, max_methods: int = 20, max_fields: int = 15):
        super().__init__("God Class", Severity.CRITICAL)
        self._max_methods = max_methods
        self._max_fields = max_fields

    def _detect_smells(self, code_file: CodeFile) -> List[CodeSmell]:
        smells = []
        in_class = False
        class_start = 0
        class_name = ""
        method_count = 0
        field_count = 0
        brace_count = 0

        for i, line in enumerate(code_file.lines, 1):
            stripped = line.strip()

            if not in_class:
                class_match = re.search(r'\b(class|struct)\s+(\w+)', line)
                if class_match:
                    class_name = class_match.group(2)
                    class_start = i
                    in_class = True
                    method_count = 0
                    field_count = 0
                    brace_count = 0
            else:
                # Count methods and fields
                if re.search(r'\w+\s*\([^)]*\)\s*(?:const\s*)?(?:override\s*)?(?:\{|;)', line):
                    method_count += 1

                if re.search(r'^\s*(?:private|protected|public)?\s*\w+[\w\s\*&]*\w+\s*;', line):
                    if not re.search(r'\(', line):
                        field_count += 1

                if '{' in line or '}' in line:
                    brace_count += line.count('{') - line.count('}')

                if brace_count == 0 and re.search(r'^\s*\};', line):
                    if method_count > self._max_methods or field_count > self._max_fields:
                        smell = self._create_smell(
                            code_file, class_start,
                            f"Class '{class_name}' has {method_count} methods and {field_count} fields (too many responsibilities)",
                            "Apply Single Responsibility Principle - extract related methods into separate classes",
                            f"Methods: {method_count}, Fields: {field_count}"
                        )
                        smells.append(smell)
                    in_class = False

        return smells


# ============================================================================
# DETECTOR FACTORY (DIP - Dependency Inversion Principle)
# ============================================================================

class DetectorFactory:
    """Factory for creating detectors (DIP - depend on abstractions)"""

    @staticmethod
    def create_default_detectors() -> List[SmellDetector]:
        """Create default set of detectors"""
        return [
            LongMethodDetector(max_lines=50),
            MagicNumberDetector(),
            DeadCodeDetector(),
            GodClassDetector(max_methods=20, max_fields=15),
        ]


# ============================================================================
# FILE READER (SRP)
# ============================================================================

class CodeFileReader:
    """Reads code files - single responsibility"""

    def read(self, file_path: str) -> Optional[CodeFile]:
        """Read a code file"""
        try:
            with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
                lines = f.readlines()
            return CodeFile(path=file_path, lines=lines)
        except Exception as e:
            print(f"{Color.RED}✗ Error reading {file_path}: {e}{Color.RESET}")
            return None


class DirectoryScanner:
    """Scans directories for C++ files - single responsibility"""

    def scan(self, dir_path: str) -> List[str]:
        """Scan directory for C++ files"""
        path = Path(dir_path)
        if not path.exists():
            return []

        cpp_files = (list(path.glob('**/*.cpp')) +
                    list(path.glob('**/*.h')) +
                    list(path.glob('**/*.hpp')))

        return [str(f) for f in cpp_files]


# ============================================================================
# SCANNER SERVICE (SRP - orchestration)
# ============================================================================

class CodeSmellScanner:
    """
    Main scanner service (SRP - orchestrates detection)

    Uses dependency injection (DIP) - depends on abstractions
    """

    def __init__(self, detectors: List[SmellDetector], file_reader: CodeFileReader):
        self._detectors = detectors
        self._file_reader = file_reader

    def scan_file(self, file_path: str) -> List[CodeSmell]:
        """Scan a single file"""
        code_file = self._file_reader.read(file_path)
        if not code_file:
            return []

        all_smells = []
        for detector in self._detectors:
            smells = detector.detect(code_file)
            all_smells.extend(smells)

        # Sort by line number
        all_smells.sort(key=lambda s: s.line_number)
        return all_smells

    def scan_directory(self, dir_path: str, scanner: DirectoryScanner) -> Dict[str, List[CodeSmell]]:
        """Scan all C++ files in directory"""
        results = {}
        file_paths = scanner.scan(dir_path)

        print(f"\n{Color.BOLD}Scanning {len(file_paths)} C++ files...{Color.RESET}\n")

        for file_path in file_paths:
            smells = self.scan_file(file_path)
            if smells:
                results[file_path] = smells

        return results


# ============================================================================
# STATISTICS CALCULATOR (SRP)
# ============================================================================

@dataclass(frozen=True)
class ScanStatistics:
    """Immutable statistics value object"""
    total_smells: int
    files_with_issues: int
    by_severity: Dict[Severity, int]
    by_type: Dict[str, int]
    by_file: Dict[str, int]
    average_per_file: float
    most_problematic_file: str
    most_common_smell: str
    quality_score: float
    quality_grade: str


class StatisticsCalculator:
    """Calculates statistics from scan results (SRP)"""

    def calculate(self, results: Dict[str, List[CodeSmell]]) -> ScanStatistics:
        """Calculate statistics"""
        by_severity = defaultdict(int)
        by_type = defaultdict(int)
        by_file = defaultdict(int)
        total = 0

        for file_path, smells in results.items():
            by_file[file_path] = len(smells)
            for smell in smells:
                by_severity[smell.severity] += 1
                by_type[smell.name] += 1
                total += 1

        # Quality score (harsh but realistic)
        critical_penalty = by_severity[Severity.CRITICAL] * 20
        major_penalty = by_severity[Severity.MAJOR] * 5
        minor_penalty = by_severity[Severity.MINOR] * 1

        quality_score = max(0, 100 - (critical_penalty + major_penalty + minor_penalty))

        # Bonus for no critical smells
        if by_severity[Severity.CRITICAL] == 0:
            quality_score = min(100, quality_score + 10)

        # Grade
        if quality_score >= 90:
            grade = "A (Excellent)"
        elif quality_score >= 75:
            grade = "B (Good)"
        elif quality_score >= 60:
            grade = "C (Fair)"
        elif quality_score >= 40:
            grade = "D (Poor)"
        else:
            grade = "F (Very Poor)"

        return ScanStatistics(
            total_smells=total,
            files_with_issues=len(results),
            by_severity=dict(by_severity),
            by_type=dict(by_type),
            by_file=dict(by_file),
            average_per_file=total / len(results) if results else 0,
            most_problematic_file=max(by_file.items(), key=lambda x: x[1])[0] if by_file else "N/A",
            most_common_smell=max(by_type.items(), key=lambda x: x[1])[0] if by_type else "N/A",
            quality_score=quality_score,
            quality_grade=grade
        )


# ============================================================================
# REPORT FORMATTERS (OCP, SRP)
# ============================================================================

class ConsoleReportFormatter:
    """Formats reports for console output (SRP)"""

    def __init__(self, verbose: bool = False, extra_verbose: bool = False):
        self._verbose = verbose
        self._extra_verbose = extra_verbose

    def format(self, results: Dict[str, List[CodeSmell]], stats: ScanStatistics) -> str:
        """Format scan results"""
        if not results:
            return f"{Color.GREEN}✓ No code smells detected!{Color.RESET}\n"

        report = self._format_header()
        report += self._format_summary(stats)

        if self._verbose or self._extra_verbose:
            report += self._format_file_breakdown(stats)

        report += self._format_severity_breakdown(stats)
        report += self._format_type_breakdown(stats)

        if self._verbose or self._extra_verbose:
            report += self._format_detailed_smells(results)
            report += self._format_statistics(stats)

        report += self._format_recommendations(stats)

        if self._extra_verbose:
            report += self._format_refactoring_priority()

        return report

    def _format_header(self) -> str:
        return (f"\n{Color.BOLD}{'='*80}{Color.RESET}\n"
                f"{Color.BOLD}CODE SMELL ANALYSIS REPORT{Color.RESET}\n"
                f"{Color.BOLD}{'='*80}{Color.RESET}\n\n")

    def _format_summary(self, stats: ScanStatistics) -> str:
        return (f"{Color.BOLD}Summary:{Color.RESET}\n"
                f"  Total code smells: {stats.total_smells}\n"
                f"  Files with issues: {stats.files_with_issues}\n")

    def _format_file_breakdown(self, stats: ScanStatistics) -> str:
        output = f"\n{Color.BOLD}Files Analyzed:{Color.RESET}\n"
        for file_path, count in sorted(stats.by_file.items(), key=lambda x: -x[1]):
            marker = "🔴" if count > 20 else "🟡" if count > 10 else "🔵"
            output += f"  {marker} {Path(file_path).name:40s}: {count:3d} smells\n"
        return output

    def _format_severity_breakdown(self, stats: ScanStatistics) -> str:
        output = f"\n{Color.BOLD}By Severity:{Color.RESET}\n"
        for severity in [Severity.CRITICAL, Severity.MAJOR, Severity.MINOR, Severity.INFO]:
            count = stats.by_severity.get(severity, 0)
            if count > 0:
                icon = {"CRITICAL": "🔴", "MAJOR": "🟡", "MINOR": "🔵", "INFO": "ℹ️"}[severity.name]
                percentage = (count / stats.total_smells * 100) if stats.total_smells > 0 else 0
                output += f"  {icon} {severity.value:10s}: {count:3d} ({percentage:5.1f}%)\n"
        return output

    def _format_type_breakdown(self, stats: ScanStatistics) -> str:
        output = f"\n{Color.BOLD}By Type:{Color.RESET}\n"
        for smell_type, count in sorted(stats.by_type.items(), key=lambda x: -x[1]):
            percentage = (count / stats.total_smells * 100) if stats.total_smells > 0 else 0
            bar_length = int(percentage / 5)
            bar = '█' * bar_length
            output += f"  • {smell_type:25s}: {count:3d} ({percentage:5.1f}%) {Color.CYAN}{bar}{Color.RESET}\n"
        return output + "\n"

    def _format_detailed_smells(self, results: Dict[str, List[CodeSmell]]) -> str:
        output = f"{Color.BOLD}Detailed Code Smells:{Color.RESET}\n\n"

        for file_path, smells in sorted(results.items()):
            output += f"\n{Color.BOLD}{Color.UNDERLINE}{file_path}{Color.RESET}\n"
            output += f"{Color.BOLD}{'─' * 80}{Color.RESET}\n"
            output += f"Found {len(smells)} smell(s) in this file\n\n"

            for smell in smells:
                output += self._format_smell(smell)
                output += "\n"

        return output

    def _format_smell(self, smell: CodeSmell) -> str:
        icon_map = {
            Severity.CRITICAL: f"{Color.RED}🔴",
            Severity.MAJOR: f"{Color.YELLOW}🟡",
            Severity.MINOR: f"{Color.BLUE}🔵",
            Severity.INFO: f"{Color.CYAN}ℹ️"
        }
        icon = icon_map.get(smell.severity, "⚪")

        return (f"{icon} {Color.BOLD}{smell.name}{Color.RESET} "
                f"[{smell.severity.value}] at line {smell.line_number}:\n"
                f"  {Color.CYAN}{smell.description}{Color.RESET}\n"
                f"  {Color.GREEN}💡 {smell.suggestion}{Color.RESET}\n"
                f"  {Color.WHITE}→ {smell.context}{Color.RESET}")

    def _format_statistics(self, stats: ScanStatistics) -> str:
        quality_color = (Color.GREEN if stats.quality_score >= 75
                        else Color.YELLOW if stats.quality_score >= 40
                        else Color.RED)

        return (f"\n{Color.BOLD}Statistics:{Color.RESET}\n"
                f"  Average smells per file: {stats.average_per_file:.1f}\n"
                f"  Most problematic file: {stats.most_problematic_file}\n"
                f"  Most common smell: {stats.most_common_smell}\n"
                f"  Code quality score: {quality_color}{stats.quality_score:.1f}/100 "
                f"({stats.quality_grade}){Color.RESET}\n")

    def _format_recommendations(self, stats: ScanStatistics) -> str:
        output = f"\n{Color.BOLD}Top Recommendations:{Color.RESET}\n"
        if stats.by_severity.get(Severity.CRITICAL, 0) > 0:
            output += f"  {Color.RED}🔴 CRITICAL:{Color.RESET} Address Large/God classes immediately\n"
        if stats.by_type.get("Long Method", 0) > 5:
            output += f"  {Color.YELLOW}🟡 MAJOR:{Color.RESET} Refactor long methods using Extract Method pattern\n"
        if stats.by_type.get("Duplicate Code", 0) > 0:
            output += f"  {Color.YELLOW}🟡 MAJOR:{Color.RESET} Eliminate duplicate code through abstraction\n"
        if stats.by_type.get("Magic Number", 0) > 10:
            output += f"  {Color.BLUE}🔵 MINOR:{Color.RESET} Replace magic numbers with named constants\n"
        return output

    def _format_refactoring_priority(self) -> str:
        return (f"\n{Color.BOLD}Refactoring Priority:{Color.RESET}\n"
                f"  1. Start with Critical smells (they have the biggest impact)\n"
                f"  2. Then tackle Major smells in frequently-changed files\n"
                f"  3. Address Minor smells during routine maintenance\n"
                f"  4. Use 'Boy Scout Rule': Leave code better than you found it\n")


class ConsoleOutputWriter:
    """Writes output to console (SRP)"""

    def write(self, content: str) -> None:
        """Write to console"""
        print(content)


# ============================================================================
# APPLICATION SERVICE (SRP - Application logic)
# ============================================================================

class ScanApplication:
    """
    Application service - coordinates the scanning process (SRP)

    Uses dependency injection (DIP)
    """

    def __init__(self,
                 scanner: CodeSmellScanner,
                 dir_scanner: DirectoryScanner,
                 stats_calculator: StatisticsCalculator,
                 formatter: ReportFormatter,
                 writer: OutputWriter):
        self._scanner = scanner
        self._dir_scanner = dir_scanner
        self._stats_calculator = stats_calculator
        self._formatter = formatter
        self._writer = writer

    def scan_file(self, file_path: str) -> None:
        """Scan a single file"""
        print(f"\n{Color.BOLD}Scanning: {file_path}{Color.RESET}")
        smells = self._scanner.scan_file(file_path)
        results = {file_path: smells} if smells else {}

        stats = self._stats_calculator.calculate(results)
        report = self._formatter.format(results, stats)
        self._writer.write(report)

    def scan_directory(self, dir_path: str) -> None:
        """Scan a directory"""
        results = self._scanner.scan_directory(dir_path, self._dir_scanner)
        stats = self._stats_calculator.calculate(results)
        report = self._formatter.format(results, stats)
        self._writer.write(report)


# ============================================================================
# DEPENDENCY INJECTION CONTAINER (DIP)
# ============================================================================

class DIContainer:
    """Simple dependency injection container"""

    @staticmethod
    def create_application(verbose: bool = False, extra_verbose: bool = False) -> ScanApplication:
        """Create application with all dependencies"""

        # Create dependencies
        detectors = DetectorFactory.create_default_detectors()
        file_reader = CodeFileReader()
        scanner = CodeSmellScanner(detectors, file_reader)
        dir_scanner = DirectoryScanner()
        stats_calculator = StatisticsCalculator()
        formatter = ConsoleReportFormatter(verbose, extra_verbose)
        writer = ConsoleOutputWriter()

        # Create and return application
        return ScanApplication(scanner, dir_scanner, stats_calculator, formatter, writer)


# ============================================================================
# COMMAND LINE INTERFACE
# ============================================================================

def main():
    parser = argparse.ArgumentParser(
        description='Code Smell Scanner for C++ (SOLID Refactored)',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog=__doc__
    )

    parser.add_argument('path', help='C++ file or directory to scan')
    parser.add_argument('--verbose', '-v', action='store_true',
                       help='Show detailed output')
    parser.add_argument('--extra-verbose', '-vv', action='store_true',
                       help='Show extra details')

    args = parser.parse_args()

    # Create application using DI container
    verbose = args.verbose or args.extra_verbose
    extra_verbose = args.extra_verbose
    app = DIContainer.create_application(verbose, extra_verbose)

    # Run application
    path = Path(args.path)

    if path.is_file():
        app.scan_file(args.path)
    elif path.is_dir():
        app.scan_directory(args.path)
    else:
        print(f"{Color.RED}✗ Error: '{args.path}' not found{Color.RESET}")
        sys.exit(1)


if __name__ == '__main__':
    main()
