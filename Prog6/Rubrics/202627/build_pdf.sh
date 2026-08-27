#!/usr/bin/env bash
# Regenerate the rubric PDFs from the Markdown sources (requires pandoc + xelatex).
set -euo pipefail
cd "$(dirname "$0")"
for md in *.md; do
  pandoc "$md" -o "${md%.md}.pdf" --pdf-engine=xelatex \
    -V geometry:a4paper -V geometry:landscape -V geometry:margin=1.5cm \
    -V fontsize=10pt -V mainfont="Helvetica" -V colorlinks=true
  echo "built ${md%.md}.pdf"
done
