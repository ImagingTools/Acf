#!/usr/bin/env python3
"""
Generate repository statistics for ACF project.
This script collects various metrics about the codebase including:
- Lines of code (LOC)
- Number of files
- Number of classes
- Number of components
- Library and package information
"""

import os
import re
import json
from pathlib import Path
from datetime import datetime
from collections import defaultdict

class RepositoryStats:
    def __init__(self, repo_path):
        self.repo_path = Path(repo_path)
        self.stats = {
            'generated_at': datetime.now().isoformat(),
            'total_files': 0,
            'total_lines': 0,
            'source_files': {},
            'libraries': {},
            'packages': {},
            'components': [],
            'classes': [],
            'interfaces': [],
            'test_files': 0,
            'documentation_files': 0,
            'quality_metrics': {
                'large_files': [],
                'todos': 0,
                'fixmes': 0,
                'hacks': 0,
                'warnings': 0,
            },
        }
        
    def count_lines(self, file_path):
        """Count lines in a file, excluding empty lines and comments."""
        try:
            with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
                lines = f.readlines()
                total = len(lines)
                code = 0
                comment = 0
                blank = 0
                in_multiline_comment = False
                
                # Quality metrics
                todos = 0
                fixmes = 0
                hacks = 0
                warnings = 0
                
                for line in lines:
                    stripped = line.strip()
                    
                    # Count technical debt markers
                    line_upper = stripped.upper()
                    if 'TODO' in line_upper:
                        todos += 1
                    if 'FIXME' in line_upper:
                        fixmes += 1
                    if 'HACK' in line_upper:
                        hacks += 1
                    if 'WARNING' in line_upper or 'WARN' in line_upper:
                        warnings += 1
                    
                    if not stripped:
                        blank += 1
                        continue
                    
                    # Handle multi-line comments
                    if '/*' in stripped and not in_multiline_comment:
                        in_multiline_comment = True
                        comment += 1
                        if '*/' in stripped:
                            in_multiline_comment = False
                        continue
                    
                    if in_multiline_comment:
                        comment += 1
                        if '*/' in stripped:
                            in_multiline_comment = False
                        continue
                    
                    # Single line comments
                    if stripped.startswith('//') or stripped.startswith('#'):
                        comment += 1
                        continue
                    
                    code += 1
                
                # Update quality metrics
                self.stats['quality_metrics']['todos'] += todos
                self.stats['quality_metrics']['fixmes'] += fixmes
                self.stats['quality_metrics']['hacks'] += hacks
                self.stats['quality_metrics']['warnings'] += warnings
                
                return {
                    'total': total,
                    'code': code,
                    'comment': comment,
                    'blank': blank,
                    'todos': todos,
                    'fixmes': fixmes,
                    'hacks': hacks,
                    'warnings': warnings
                }
        except Exception as e:
            print(f"Error reading {file_path}: {e}")
            return {'total': 0, 'code': 0, 'comment': 0, 'blank': 0, 
                   'todos': 0, 'fixmes': 0, 'hacks': 0, 'warnings': 0}
    
    def extract_classes(self, file_path):
        """Extract class names from a C++ header file."""
        classes = []
        try:
            with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
                content = f.read()
                # Match class declarations
                class_pattern = r'class\s+(?:ACF_\w+_EXPORT\s+)?(\w+)(?:\s*:\s*public\s+(\w+))?'
                matches = re.finditer(class_pattern, content)
                for match in matches:
                    class_name = match.group(1)
                    base_class = match.group(2) if match.group(2) else None
                    classes.append({
                        'name': class_name,
                        'base': base_class,
                        'file': str(file_path.relative_to(self.repo_path))
                    })
        except Exception as e:
            print(f"Error extracting classes from {file_path}: {e}")
        return classes
    
    def extract_components(self, file_path):
        """Extract component information from source files."""
        components = []
        try:
            with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
                content = f.read()
                # Look for component declarations
                component_pattern = r'I_BEGIN_COMPONENT\s*\(\s*(\w+)\s*\)'
                matches = re.finditer(component_pattern, content)
                for match in matches:
                    component_name = match.group(1)
                    components.append({
                        'name': component_name,
                        'file': str(file_path.relative_to(self.repo_path))
                    })
        except Exception as e:
            print(f"Error extracting components from {file_path}: {e}")
        return components
    
    def extract_interfaces(self, file_path):
        """Extract interface declarations."""
        interfaces = []
        try:
            with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
                content = f.read()
                # Match interface declarations (classes starting with I)
                interface_pattern = r'class\s+(?:ACF_\w+_EXPORT\s+)?(I[A-Z]\w+)'
                matches = re.finditer(interface_pattern, content)
                for match in matches:
                    interface_name = match.group(1)
                    interfaces.append({
                        'name': interface_name,
                        'file': str(file_path.relative_to(self.repo_path))
                    })
        except Exception as e:
            print(f"Error extracting interfaces from {file_path}: {e}")
        return interfaces
    
    def analyze_directory(self, directory, category):
        """Analyze a directory for source files."""
        dir_path = self.repo_path / directory
        if not dir_path.exists():
            return
        
        extensions = {'.h', '.cpp', '.c', '.hpp', '.cc'}
        large_file_threshold = 1000  # lines
        
        for file_path in dir_path.rglob('*'):
            if file_path.is_file() and file_path.suffix in extensions:
                self.stats['total_files'] += 1
                
                # Count lines
                line_counts = self.count_lines(file_path)
                self.stats['total_lines'] += line_counts['total']
                
                # Store file info
                rel_path = str(file_path.relative_to(self.repo_path))
                self.stats['source_files'][rel_path] = {
                    'lines': line_counts,
                    'category': category,
                    'extension': file_path.suffix
                }
                
                # Track large files (quality metric)
                if line_counts['total'] > large_file_threshold:
                    self.stats['quality_metrics']['large_files'].append({
                        'file': rel_path,
                        'lines': line_counts['total']
                    })
                
                # Extract classes from headers
                if file_path.suffix == '.h':
                    classes = self.extract_classes(file_path)
                    self.stats['classes'].extend(classes)
                    
                    interfaces = self.extract_interfaces(file_path)
                    self.stats['interfaces'].extend(interfaces)
                    
                    components = self.extract_components(file_path)
                    self.stats['components'].extend(components)
    
    def analyze_libraries(self):
        """Analyze library structure in Include directory."""
        include_path = self.repo_path / 'Include'
        if not include_path.exists():
            return
        
        for lib_dir in include_path.iterdir():
            if lib_dir.is_dir() and not lib_dir.name.startswith('.'):
                lib_name = lib_dir.name
                files = list(lib_dir.rglob('*.h')) + list(lib_dir.rglob('*.cpp'))
                
                total_lines = 0
                for f in files:
                    if str(f.relative_to(self.repo_path)) in self.stats['source_files']:
                        total_lines += self.stats['source_files'][str(f.relative_to(self.repo_path))]['lines']['total']
                
                self.stats['libraries'][lib_name] = {
                    'file_count': len(files),
                    'total_lines': total_lines
                }
    
    def analyze_packages(self):
        """Analyze package structure in Impl directory."""
        impl_path = self.repo_path / 'Impl'
        if not impl_path.exists():
            return
        
        for pkg_dir in impl_path.iterdir():
            if pkg_dir.is_dir() and not pkg_dir.name.startswith('.'):
                pkg_name = pkg_dir.name
                files = list(pkg_dir.rglob('*.h')) + list(pkg_dir.rglob('*.cpp'))
                
                total_lines = 0
                for f in files:
                    if str(f.relative_to(self.repo_path)) in self.stats['source_files']:
                        total_lines += self.stats['source_files'][str(f.relative_to(self.repo_path))]['lines']['total']
                
                self.stats['packages'][pkg_name] = {
                    'file_count': len(files),
                    'total_lines': total_lines
                }
    
    def analyze_tests(self):
        """Analyze test files."""
        tests_path = self.repo_path / 'Tests'
        if not tests_path.exists():
            return
        
        test_files = list(tests_path.rglob('*.cpp')) + list(tests_path.rglob('*.h'))
        self.stats['test_files'] = len(test_files)
    
    def analyze_docs(self):
        """Analyze documentation files."""
        docs_path = self.repo_path / 'Docs'
        if not docs_path.exists():
            return
        
        doc_files = list(docs_path.rglob('*.md')) + list(docs_path.rglob('*.dox'))
        self.stats['documentation_files'] = len(doc_files)
    
    def generate(self):
        """Generate all statistics."""
        print("Analyzing Include directory...")
        self.analyze_directory('Include', 'library')
        
        print("Analyzing Impl directory...")
        self.analyze_directory('Impl', 'package')
        
        print("Analyzing library structure...")
        self.analyze_libraries()
        
        print("Analyzing package structure...")
        self.analyze_packages()
        
        print("Analyzing tests...")
        self.analyze_tests()
        
        print("Analyzing documentation...")
        self.analyze_docs()
        
        # Calculate summary statistics
        self.stats['summary'] = {
            'total_lines': self.stats['total_lines'],
            'total_files': self.stats['total_files'],
            'total_libraries': len(self.stats['libraries']),
            'total_packages': len(self.stats['packages']),
            'total_classes': len(self.stats['classes']),
            'total_interfaces': len(self.stats['interfaces']),
            'total_components': len(self.stats['components']),
            'test_files': self.stats['test_files'],
            'documentation_files': self.stats['documentation_files'],
        }
        
        # Calculate code vs comment ratio
        total_code = sum(f['lines']['code'] for f in self.stats['source_files'].values())
        total_comment = sum(f['lines']['comment'] for f in self.stats['source_files'].values())
        total_blank = sum(f['lines']['blank'] for f in self.stats['source_files'].values())
        
        self.stats['summary']['code_lines'] = total_code
        self.stats['summary']['comment_lines'] = total_comment
        self.stats['summary']['blank_lines'] = total_blank
        
        # Quality metrics calculations
        if total_code > 0:
            self.stats['summary']['comment_to_code_ratio'] = round(total_comment / total_code, 3)
        else:
            self.stats['summary']['comment_to_code_ratio'] = 0
        
        if self.stats['total_files'] > 0:
            self.stats['summary']['avg_lines_per_file'] = round(self.stats['total_lines'] / self.stats['total_files'], 1)
        else:
            self.stats['summary']['avg_lines_per_file'] = 0
        
        if len(self.stats['classes']) > 0:
            # Estimate average lines per class (rough approximation)
            self.stats['summary']['avg_lines_per_class'] = round(total_code / len(self.stats['classes']), 1)
        else:
            self.stats['summary']['avg_lines_per_class'] = 0
        
        # Sort large files by size
        self.stats['quality_metrics']['large_files'].sort(key=lambda x: x['lines'], reverse=True)
        
        # Add quality summary
        self.stats['summary']['quality_score'] = self.calculate_quality_score()
        
        return self.stats
    
    def calculate_quality_score(self):
        """Calculate a simple quality score based on various metrics."""
        score = 100.0
        
        # Deduct points for technical debt markers
        todos = self.stats['quality_metrics']['todos']
        fixmes = self.stats['quality_metrics']['fixmes']
        hacks = self.stats['quality_metrics']['hacks']
        
        # Deduct based on density of technical debt
        total_code = sum(f['lines']['code'] for f in self.stats['source_files'].values())
        if total_code > 0:
            debt_density = (todos + fixmes * 2 + hacks * 3) / total_code * 1000
            score -= min(debt_density * 2, 20)  # Max 20 points deduction
        
        # Bonus for good comment ratio (0.2 - 0.5 is ideal)
        comment_ratio = self.stats['summary'].get('comment_to_code_ratio', 0)
        if 0.2 <= comment_ratio <= 0.5:
            score += 5
        elif comment_ratio < 0.1:
            score -= 10
        
        # Deduct for large files
        large_file_count = len(self.stats['quality_metrics']['large_files'])
        if large_file_count > 0:
            large_file_penalty = min(large_file_count * 0.5, 15)
            score -= large_file_penalty
        
        return max(0, min(100, round(score, 1)))
    
    def save_json(self, output_path):
        """Save statistics to JSON file."""
        with open(output_path, 'w', encoding='utf-8') as f:
            json.dump(self.stats, f, indent=2)
        print(f"Statistics saved to {output_path}")

def main():
    import sys
    
    repo_path = sys.argv[1] if len(sys.argv) > 1 else '.'
    output_path = sys.argv[2] if len(sys.argv) > 2 else 'stats.json'
    
    print(f"Generating statistics for repository: {repo_path}")
    
    stats_gen = RepositoryStats(repo_path)
    stats = stats_gen.generate()
    
    print("\n=== Summary ===")
    print(f"Total Files: {stats['summary']['total_files']}")
    print(f"Total Lines: {stats['summary']['total_lines']}")
    print(f"Code Lines: {stats['summary']['code_lines']}")
    print(f"Comment Lines: {stats['summary']['comment_lines']}")
    print(f"Blank Lines: {stats['summary']['blank_lines']}")
    print(f"Libraries: {stats['summary']['total_libraries']}")
    print(f"Packages: {stats['summary']['total_packages']}")
    print(f"Classes: {stats['summary']['total_classes']}")
    print(f"Interfaces: {stats['summary']['total_interfaces']}")
    print(f"Components: {stats['summary']['total_components']}")
    print(f"Test Files: {stats['summary']['test_files']}")
    print(f"Documentation Files: {stats['summary']['documentation_files']}")
    
    print("\n=== Quality Metrics ===")
    print(f"Quality Score: {stats['summary']['quality_score']}/100")
    print(f"Comment-to-Code Ratio: {stats['summary']['comment_to_code_ratio']}")
    print(f"Average Lines per File: {stats['summary']['avg_lines_per_file']}")
    print(f"Average Lines per Class: {stats['summary']['avg_lines_per_class']}")
    print(f"TODOs: {stats['quality_metrics']['todos']}")
    print(f"FIXMEs: {stats['quality_metrics']['fixmes']}")
    print(f"HACKs: {stats['quality_metrics']['hacks']}")
    print(f"Large Files (>1000 lines): {len(stats['quality_metrics']['large_files'])}")
    
    stats_gen.save_json(output_path)

if __name__ == '__main__':
    main()
