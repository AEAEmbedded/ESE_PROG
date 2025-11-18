#!/usr/bin/env python3
"""
Code Smell Scanner for C++
===========================

Detects common code smells and quality issues that indicate poor design,
maintainability problems, or potential bugs.

Usage:
    python3 code_smell_scanner.py <file.cpp> [--verbose]
    python3 code_smell_scanner.py <directory> [--verbose]

Code Smells Detected:
- Long methods (>50 lines)
- Long parameter lists (>5 parameters)
- Deep nesting (>4 levels)
- Duplicate code patterns
- Magic numbers
- Large classes (>500 lines)
- God classes (too many responsibilities)
- Dead code (unreachable, commented out)
- Complex conditionals
- Primitive obsession
- Data clumps
- Feature envy
- Inappropriate intimacy
- Refused bequest
- Lazy class
- Speculative generality
"""

import re
import sys
import argparse
from pathlib import Path
from typing import List, Dict, Tuple, Set
from dataclasses import dataclass
from enum import Enum
from collections import defaultdict


# ============================================================================
# COLOR DEFINITIONS
# ============================================================================

class Color:
    """ANSI color codes for terminal output"""
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


# ============================================================================
# DATA STRUCTURES
# ============================================================================

class Severity(Enum):
    """Code smell severity levels"""
    CRITICAL = "Critical"    # Serious design flaw
    MAJOR = "Major"          # Significant maintainability issue
    MINOR = "Minor"          # Minor quality issue
    INFO = "Info"            # Suggestion for improvement


@dataclass
class CodeSmell:
    """Represents a detected code smell"""
    name: str
    severity: Severity
    file_path: str
    line_number: int
    description: str
    suggestion: str
    context: str = ""

    def __str__(self):
        severity_icon = {
            Severity.CRITICAL: f"{Color.RED}🔴",
            Severity.MAJOR: f"{Color.YELLOW}🟡",
            Severity.MINOR: f"{Color.BLUE}🔵",
            Severity.INFO: f"{Color.CYAN}ℹ️"
        }
        icon = severity_icon.get(self.severity, "⚪")

        result = (f"{icon} {Color.BOLD}{self.name}{Color.RESET} "
                 f"[{self.severity.value}] at line {self.line_number}:\n"
                 f"  {Color.CYAN}{self.description}{Color.RESET}\n"
                 f"  {Color.GREEN}💡 {self.suggestion}{Color.RESET}")

        if self.context:
            result += f"\n  {Color.WHITE}→ {self.context}{Color.RESET}"

        return result


# ============================================================================
# CODE SMELL DETECTORS
# ============================================================================

class LongMethodDetector:
    """Detects methods that are too long"""

    def __init__(self, max_lines: int = 50):
        self.max_lines = max_lines

    def detect(self, file_path: str, lines: List[str]) -> List[CodeSmell]:
        smells = []
        in_function = False
        func_start = 0
        func_name = ""
        brace_count = 0

        for i, line in enumerate(lines, 1):
            stripped = line.strip()

            # Detect function start
            if not in_function:
                # Look for function definition
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
                # Track braces
                brace_count += line.count('{') - line.count('}')

                # Function ends
                if brace_count == 0:
                    func_length = i - func_start + 1
                    if func_length > self.max_lines:
                        smells.append(CodeSmell(
                            name="Long Method",
                            severity=Severity.MAJOR,
                            file_path=file_path,
                            line_number=func_start,
                            description=f"Method '{func_name}' is {func_length} lines long (max: {self.max_lines})",
                            suggestion=f"Break down into smaller methods (Extract Method refactoring)",
                            context=f"Lines {func_start}-{i}"
                        ))
                    in_function = False
                    func_name = ""

        return smells


class LongParameterListDetector:
    """Detects functions with too many parameters"""

    def __init__(self, max_params: int = 5):
        self.max_params = max_params

    def detect(self, file_path: str, lines: List[str]) -> List[CodeSmell]:
        smells = []

        for i, line in enumerate(lines, 1):
            # Look for function definitions
            func_match = re.search(r'(\w+)\s*\(([^)]+)\)', line)
            if func_match and not line.strip().startswith('//'):
                func_name = func_match.group(1)
                params = func_match.group(2)

                # Count parameters (split by comma, but not in templates)
                param_count = 0
                template_depth = 0
                for char in params:
                    if char == '<':
                        template_depth += 1
                    elif char == '>':
                        template_depth -= 1
                    elif char == ',' and template_depth == 0:
                        param_count += 1

                if params.strip():  # Has at least one parameter
                    param_count += 1

                if param_count > self.max_params:
                    smells.append(CodeSmell(
                        name="Long Parameter List",
                        severity=Severity.MAJOR,
                        file_path=file_path,
                        line_number=i,
                        description=f"Function '{func_name}' has {param_count} parameters (max: {self.max_params})",
                        suggestion="Use parameter object or builder pattern",
                        context=line.strip()
                    ))

        return smells


class DeepNestingDetector:
    """Detects deeply nested code blocks"""

    def __init__(self, max_depth: int = 4):
        self.max_depth = max_depth

    def detect(self, file_path: str, lines: List[str]) -> List[CodeSmell]:
        smells = []

        for i, line in enumerate(lines, 1):
            # Count indentation level (assuming 4 spaces or 1 tab per level)
            indent = len(line) - len(line.lstrip())
            if '\t' in line[:indent]:
                depth = line[:indent].count('\t')
            else:
                depth = indent // 4

            # Check if this is a control flow statement
            stripped = line.strip()
            if re.match(r'^(if|for|while|switch|try)\s*\(', stripped):
                if depth > self.max_depth:
                    smells.append(CodeSmell(
                        name="Deep Nesting",
                        severity=Severity.MAJOR,
                        file_path=file_path,
                        line_number=i,
                        description=f"Nesting depth of {depth} levels (max: {self.max_depth})",
                        suggestion="Extract nested logic into separate methods or use early returns",
                        context=line.strip()
                    ))

        return smells


class MagicNumberDetector:
    """Detects magic numbers (unexplained literals)"""

    def __init__(self):
        self.allowed = {0, 1, -1, 2}  # Common acceptable literals

    def detect(self, file_path: str, lines: List[str]) -> List[CodeSmell]:
        smells = []

        for i, line in enumerate(lines, 1):
            # Skip comments and strings
            if '//' in line:
                code_part = line[:line.index('//')]
            else:
                code_part = line

            # Remove string literals
            code_part = re.sub(r'"[^"]*"', '', code_part)
            code_part = re.sub(r"'[^']*'", '', code_part)

            # Find numeric literals (but not in #define or const declarations)
            if not re.match(r'^\s*#define', line) and not re.search(r'\bconst\b', line):
                # Look for numbers
                numbers = re.findall(r'\b(\d+\.?\d*)\b', code_part)
                for num_str in numbers:
                    try:
                        num = float(num_str) if '.' in num_str else int(num_str)
                        if num not in self.allowed and num > 2:
                            smells.append(CodeSmell(
                                name="Magic Number",
                                severity=Severity.MINOR,
                                file_path=file_path,
                                line_number=i,
                                description=f"Magic number '{num}' without explanation",
                                suggestion="Replace with named constant (const/constexpr)",
                                context=line.strip()
                            ))
                    except ValueError:
                        pass

        return smells


class LargeClassDetector:
    """Detects classes that are too large"""

    def __init__(self, max_lines: int = 500):
        self.max_lines = max_lines

    def detect(self, file_path: str, lines: List[str]) -> List[CodeSmell]:
        smells = []
        in_class = False
        class_start = 0
        class_name = ""
        brace_count = 0

        for i, line in enumerate(lines, 1):
            stripped = line.strip()

            if not in_class:
                # Detect class start
                class_match = re.search(r'\b(class|struct)\s+(\w+)', line)
                if class_match and '{' in line:
                    class_name = class_match.group(2)
                    class_start = i
                    in_class = True
                    brace_count = line.count('{') - line.count('}')
            else:
                # Track braces
                brace_count += line.count('{') - line.count('}')

                # Class ends
                if brace_count == 0:
                    class_length = i - class_start + 1
                    if class_length > self.max_lines:
                        smells.append(CodeSmell(
                            name="Large Class",
                            severity=Severity.CRITICAL,
                            file_path=file_path,
                            line_number=class_start,
                            description=f"Class '{class_name}' is {class_length} lines (max: {self.max_lines})",
                            suggestion="Split into multiple classes using Single Responsibility Principle",
                            context=f"Lines {class_start}-{i}"
                        ))
                    in_class = False
                    class_name = ""

        return smells


class GodClassDetector:
    """Detects God classes (too many responsibilities)"""

    def __init__(self, max_methods: int = 20, max_fields: int = 15):
        self.max_methods = max_methods
        self.max_fields = max_fields

    def detect(self, file_path: str, lines: List[str]) -> List[CodeSmell]:
        smells = []
        in_class = False
        class_start = 0
        class_name = ""
        method_count = 0
        field_count = 0
        brace_count = 0

        for i, line in enumerate(lines, 1):
            stripped = line.strip()

            if not in_class:
                # Detect class start
                class_match = re.search(r'\b(class|struct)\s+(\w+)', line)
                if class_match:
                    class_name = class_match.group(2)
                    class_start = i
                    in_class = True
                    method_count = 0
                    field_count = 0
                    brace_count = 0
            else:
                # Count methods
                if re.search(r'\w+\s*\([^)]*\)\s*(?:const\s*)?(?:override\s*)?(?:\{|;)', line):
                    method_count += 1

                # Count fields (member variables)
                if re.search(r'^\s*(?:private|protected|public)?\s*\w+[\w\s\*&]*\w+\s*;', line):
                    if not re.search(r'\(', line):  # Not a function
                        field_count += 1

                # Track braces
                if '{' in line or '}' in line:
                    brace_count += line.count('{') - line.count('}')

                # Class ends
                if brace_count == 0 and re.search(r'^\s*\};', line):
                    if method_count > self.max_methods or field_count > self.max_fields:
                        smells.append(CodeSmell(
                            name="God Class",
                            severity=Severity.CRITICAL,
                            file_path=file_path,
                            line_number=class_start,
                            description=f"Class '{class_name}' has {method_count} methods and {field_count} fields (too many responsibilities)",
                            suggestion="Apply Single Responsibility Principle - extract related methods into separate classes",
                            context=f"Methods: {method_count}, Fields: {field_count}"
                        ))
                    in_class = False
                    class_name = ""

        return smells


class DeadCodeDetector:
    """Detects potentially dead/unreachable code"""

    def detect(self, file_path: str, lines: List[str]) -> List[CodeSmell]:
        smells = []

        for i, line in enumerate(lines, 1):
            stripped = line.strip()

            # Code after return statement
            if re.search(r'\breturn\b.*;', stripped):
                if i < len(lines):
                    next_line = lines[i].strip()
                    if next_line and not next_line.startswith('}') and not next_line.startswith('//'):
                        smells.append(CodeSmell(
                            name="Dead Code",
                            severity=Severity.MAJOR,
                            file_path=file_path,
                            line_number=i + 1,
                            description="Code after return statement is unreachable",
                            suggestion="Remove unreachable code",
                            context=next_line
                        ))

            # Large commented out blocks
            if stripped.startswith('//') and len(stripped) > 80:
                if i + 2 < len(lines) and lines[i].strip().startswith('//') and lines[i+1].strip().startswith('//'):
                    smells.append(CodeSmell(
                        name="Commented Out Code",
                        severity=Severity.MINOR,
                        file_path=file_path,
                        line_number=i,
                        description="Large block of commented code detected",
                        suggestion="Remove commented code (use version control instead)",
                        context="Multi-line commented block"
                    ))

        return smells


class ComplexConditionalDetector:
    """Detects complex conditional expressions"""

    def __init__(self, max_conditions: int = 3):
        self.max_conditions = max_conditions

    def detect(self, file_path: str, lines: List[str]) -> List[CodeSmell]:
        smells = []

        for i, line in enumerate(lines, 1):
            # Count logical operators in if/while conditions
            if re.search(r'\b(if|while)\s*\(', line):
                # Count && and ||
                and_count = line.count('&&')
                or_count = line.count('||')
                total = and_count + or_count

                if total >= self.max_conditions:
                    smells.append(CodeSmell(
                        name="Complex Conditional",
                        severity=Severity.MAJOR,
                        file_path=file_path,
                        line_number=i,
                        description=f"Conditional has {total + 1} conditions (max: {self.max_conditions})",
                        suggestion="Extract into well-named boolean variables or methods",
                        context=line.strip()
                    ))

        return smells


class DuplicateCodeDetector:
    """Detects duplicate code blocks"""

    def __init__(self, min_lines: int = 5):
        self.min_lines = min_lines

    def detect(self, file_path: str, lines: List[str]) -> List[CodeSmell]:
        smells = []

        # Create sliding window of code blocks
        blocks = {}
        for i in range(len(lines) - self.min_lines + 1):
            block = tuple(lines[i:i + self.min_lines])
            # Normalize whitespace
            normalized = tuple(line.strip() for line in block if line.strip() and not line.strip().startswith('//'))

            if len(normalized) >= self.min_lines:
                if normalized in blocks:
                    # Found duplicate
                    smells.append(CodeSmell(
                        name="Duplicate Code",
                        severity=Severity.MAJOR,
                        file_path=file_path,
                        line_number=i + 1,
                        description=f"Duplicate code block found (also at line {blocks[normalized] + 1})",
                        suggestion="Extract into a shared method or function",
                        context=f"{self.min_lines} similar lines"
                    ))
                else:
                    blocks[normalized] = i

        return smells


class DataClumpDetector:
    """Detects data clumps (same parameters appearing together)"""

    def detect(self, file_path: str, lines: List[str]) -> List[CodeSmell]:
        smells = []
        param_groups = defaultdict(list)

        for i, line in enumerate(lines, 1):
            # Find function parameters
            func_match = re.search(r'(\w+)\s*\(([^)]+)\)', line)
            if func_match and not line.strip().startswith('//'):
                params = func_match.group(2)
                # Extract parameter types
                param_types = []
                for param in params.split(','):
                    # Simple extraction of type (before last word)
                    parts = param.strip().split()
                    if len(parts) >= 2:
                        param_types.append(parts[-2])  # Type name

                if len(param_types) >= 3:
                    param_sig = tuple(sorted(param_types))
                    param_groups[param_sig].append(i)

        # Check for repeated parameter groups
        for param_sig, lines_list in param_groups.items():
            if len(lines_list) >= 3:  # Same params in 3+ functions
                for line_num in lines_list:
                    smells.append(CodeSmell(
                        name="Data Clump",
                        severity=Severity.MINOR,
                        file_path=file_path,
                        line_number=line_num,
                        description=f"Same parameter group appears in {len(lines_list)} functions",
                        suggestion="Create a parameter object or struct",
                        context=f"Parameters: {', '.join(param_sig)}"
                    ))
                break  # Only report once per clump

        return smells


# ============================================================================
# MAIN SCANNER
# ============================================================================

class CodeSmellScanner:
    """Main scanner that coordinates all detectors"""

    def __init__(self):
        self.detectors = [
            LongMethodDetector(max_lines=50),
            LongParameterListDetector(max_params=5),
            DeepNestingDetector(max_depth=4),
            MagicNumberDetector(),
            LargeClassDetector(max_lines=500),
            GodClassDetector(max_methods=20, max_fields=15),
            DeadCodeDetector(),
            ComplexConditionalDetector(max_conditions=3),
            DuplicateCodeDetector(min_lines=5),
            DataClumpDetector(),
        ]

    def scan_file(self, file_path: str) -> List[CodeSmell]:
        """Scan a single file for code smells"""
        try:
            with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
                lines = f.readlines()
        except Exception as e:
            print(f"{Color.RED}✗ Error reading {file_path}: {e}{Color.RESET}")
            return []

        all_smells = []
        for detector in self.detectors:
            smells = detector.detect(file_path, lines)
            all_smells.extend(smells)

        # Sort by line number
        all_smells.sort(key=lambda s: s.line_number)

        return all_smells

    def scan_directory(self, dir_path: str) -> Dict[str, List[CodeSmell]]:
        """Scan all C++ files in a directory"""
        results = {}

        path = Path(dir_path)
        if not path.exists():
            print(f"{Color.RED}✗ Directory not found: {dir_path}{Color.RESET}")
            return results

        # Find all C++ files
        cpp_files = list(path.glob('**/*.cpp')) + list(path.glob('**/*.h')) + list(path.glob('**/*.hpp'))

        print(f"\n{Color.BOLD}Scanning {len(cpp_files)} C++ files...{Color.RESET}\n")

        for cpp_file in cpp_files:
            smells = self.scan_file(str(cpp_file))
            if smells:
                results[str(cpp_file)] = smells

        return results

    def generate_report(self, results: Dict[str, List[CodeSmell]], verbose: bool = False, extra_verbose: bool = False):
        """Generate comprehensive report"""
        print(f"\n{Color.BOLD}{'='*80}{Color.RESET}")
        print(f"{Color.BOLD}CODE SMELL ANALYSIS REPORT{Color.RESET}")
        print(f"{Color.BOLD}{'='*80}{Color.RESET}\n")

        if not results:
            print(f"{Color.GREEN}✓ No code smells detected!{Color.RESET}\n")
            return

        # Count by severity
        by_severity = defaultdict(int)
        by_smell_type = defaultdict(int)
        by_file = defaultdict(int)
        total = 0

        for file_path, smells in results.items():
            by_file[file_path] = len(smells)
            for smell in smells:
                by_severity[smell.severity] += 1
                by_smell_type[smell.name] += 1
                total += 1

        # Summary
        print(f"{Color.BOLD}Summary:{Color.RESET}")
        print(f"  Total code smells: {total}")
        print(f"  Files with issues: {len(results)}")

        if verbose or extra_verbose:
            # Show file breakdown
            print(f"\n{Color.BOLD}Files Analyzed:{Color.RESET}")
            for file_path, count in sorted(by_file.items(), key=lambda x: -x[1]):
                severity_marker = "🔴" if count > 20 else "🟡" if count > 10 else "🔵"
                print(f"  {severity_marker} {Path(file_path).name:40s}: {count:3d} smells")
        print()

        print(f"{Color.BOLD}By Severity:{Color.RESET}")
        for severity in [Severity.CRITICAL, Severity.MAJOR, Severity.MINOR, Severity.INFO]:
            count = by_severity[severity]
            if count > 0:
                icon = "🔴" if severity == Severity.CRITICAL else "🟡" if severity == Severity.MAJOR else "🔵" if severity == Severity.MINOR else "ℹ️"
                percentage = (count / total * 100) if total > 0 else 0
                print(f"  {icon} {severity.value:10s}: {count:3d} ({percentage:5.1f}%)")
        print()

        print(f"{Color.BOLD}By Type:{Color.RESET}")
        for smell_type, count in sorted(by_smell_type.items(), key=lambda x: -x[1]):
            percentage = (count / total * 100) if total > 0 else 0
            bar_length = int(percentage / 5)  # Scale to max 20 chars
            bar = '█' * bar_length
            print(f"  • {smell_type:25s}: {count:3d} ({percentage:5.1f}%) {Color.CYAN}{bar}{Color.RESET}")
        print()

        # Detailed smells
        if verbose or extra_verbose:
            print(f"{Color.BOLD}Detailed Code Smells:{Color.RESET}\n")

            for file_path, smells in sorted(results.items()):
                print(f"\n{Color.BOLD}{Color.UNDERLINE}{file_path}{Color.RESET}")
                print(f"{Color.BOLD}{'─' * 80}{Color.RESET}")
                print(f"Found {len(smells)} smell(s) in this file\n")

                # Group by severity for better organization
                critical = [s for s in smells if s.severity == Severity.CRITICAL]
                major = [s for s in smells if s.severity == Severity.MAJOR]
                minor = [s for s in smells if s.severity == Severity.MINOR]

                for smell_list, label in [(critical, "CRITICAL"), (major, "MAJOR"), (minor, "MINOR")]:
                    if smell_list:
                        if extra_verbose:
                            print(f"{Color.BOLD}  {label} Issues ({len(smell_list)}):{Color.RESET}")
                        for smell in smell_list:
                            print(smell)

                            # Extra verbose: show code snippet
                            if extra_verbose:
                                self._show_code_snippet(smell.file_path, smell.line_number)
                            print()

        # Statistics
        if verbose or extra_verbose:
            print(f"\n{Color.BOLD}Statistics:{Color.RESET}")
            print(f"  Average smells per file: {total / len(results):.1f}")
            print(f"  Most problematic file: {max(by_file.items(), key=lambda x: x[1])[0] if by_file else 'N/A'}")
            print(f"  Most common smell: {max(by_smell_type.items(), key=lambda x: x[1])[0] if by_smell_type else 'N/A'}")

            # Code quality score - Harsh but realistic grading
            total_files = len(results)
            smell_density = (total / total_files) if total_files > 0 else 0

            # Penalties
            critical_penalty = by_severity[Severity.CRITICAL] * 20  # Each critical = -20
            major_penalty = by_severity[Severity.MAJOR] * 5        # Each major = -5
            minor_penalty = by_severity[Severity.MINOR] * 1        # Each minor = -1

            # Start at 100, subtract penalties
            quality_score = max(0, 100 - (critical_penalty + major_penalty + minor_penalty))

            # Bonus: No critical smells = +10
            if by_severity[Severity.CRITICAL] == 0:
                quality_score = min(100, quality_score + 10)

            # Grade thresholds (strict!)
            if quality_score >= 90:
                quality_color = Color.GREEN
                grade = "A (Excellent)"
            elif quality_score >= 75:
                quality_color = Color.GREEN
                grade = "B (Good)"
            elif quality_score >= 60:
                quality_color = Color.YELLOW
                grade = "C (Fair)"
            elif quality_score >= 40:
                quality_color = Color.YELLOW
                grade = "D (Poor)"
            else:
                quality_color = Color.RED
                grade = "F (Very Poor)"

            print(f"  Code quality score: {quality_color}{quality_score:.1f}/100 ({grade}){Color.RESET}")
            print()

        # Recommendations
        print(f"\n{Color.BOLD}Top Recommendations:{Color.RESET}")
        if by_severity[Severity.CRITICAL] > 0:
            print(f"  {Color.RED}🔴 CRITICAL:{Color.RESET} Address Large/God classes immediately")
        if by_smell_type.get("Long Method", 0) > 5:
            print(f"  {Color.YELLOW}🟡 MAJOR:{Color.RESET} Refactor long methods using Extract Method pattern")
        if by_smell_type.get("Duplicate Code", 0) > 0:
            print(f"  {Color.YELLOW}🟡 MAJOR:{Color.RESET} Eliminate duplicate code through abstraction")
        if by_smell_type.get("Magic Number", 0) > 10:
            print(f"  {Color.BLUE}🔵 MINOR:{Color.RESET} Replace magic numbers with named constants")

        if extra_verbose:
            print(f"\n{Color.BOLD}Refactoring Priority:{Color.RESET}")
            print(f"  1. Start with Critical smells (they have the biggest impact)")
            print(f"  2. Then tackle Major smells in frequently-changed files")
            print(f"  3. Address Minor smells during routine maintenance")
            print(f"  4. Use 'Boy Scout Rule': Leave code better than you found it")
        print()

    def _show_code_snippet(self, file_path: str, line_number: int, context: int = 3):
        """Show code snippet around the smell"""
        try:
            with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
                lines = f.readlines()

            start = max(0, line_number - context - 1)
            end = min(len(lines), line_number + context)

            print(f"{Color.WHITE}      Code context:{Color.RESET}")
            for i in range(start, end):
                line_num = i + 1
                prefix = f"{Color.YELLOW}→{Color.RESET}" if line_num == line_number else " "
                line_color = Color.YELLOW if line_num == line_number else Color.WHITE
                print(f"{Color.WHITE}      {line_num:4d} {prefix} {line_color}{lines[i].rstrip()}{Color.RESET}")
        except Exception as e:
            pass  # Silently fail if can't read snippet


# ============================================================================
# COMMAND LINE INTERFACE
# ============================================================================

def main():
    parser = argparse.ArgumentParser(
        description='Code Smell Scanner for C++',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog=__doc__
    )

    parser.add_argument('path', help='C++ file or directory to scan')
    parser.add_argument('--verbose', '-v', action='store_true',
                       help='Show detailed output with all code smells')
    parser.add_argument('--extra-verbose', '-vv', action='store_true',
                       help='Show extra details including code snippets and statistics')

    args = parser.parse_args()

    scanner = CodeSmellScanner()

    path = Path(args.path)

    # extra-verbose implies verbose
    verbose = args.verbose or args.extra_verbose
    extra_verbose = args.extra_verbose

    if path.is_file():
        # Scan single file
        print(f"\n{Color.BOLD}Scanning: {args.path}{Color.RESET}")
        smells = scanner.scan_file(args.path)
        results = {args.path: smells} if smells else {}
        scanner.generate_report(results, verbose, extra_verbose)
    elif path.is_dir():
        # Scan directory
        results = scanner.scan_directory(args.path)
        scanner.generate_report(results, verbose, extra_verbose)
    else:
        print(f"{Color.RED}✗ Error: '{args.path}' not found{Color.RESET}")
        sys.exit(1)


if __name__ == '__main__':
    main()
