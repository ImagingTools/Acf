#!/bin/bash

# UpdateVersion.sh - Linux/Unix version generation script
# Processes template files (.xtrsvn) and replaces version placeholders

set -e

# --- Parameter check ---
if [ -z "$1" ]; then
    echo "Usage: $(basename "$0") path/to/template.xtrsvn"
    exit 1
fi

FILE="$1"

if [ ! -f "$FILE" ]; then
    echo "File \"$FILE\" does not exist."
    exit 1
fi

# --- Get revision count ---
# Try to fetch and unshallow if needed (suppress errors if already complete)
git fetch --prune --unshallow 2>/dev/null || true

# Try origin/master first, then fall back to HEAD
REV=$(git rev-list --count origin/master 2>/dev/null || git rev-list --count HEAD 2>/dev/null || echo "")

if [ -z "$REV" ]; then
    echo "Failed to compute revision count."
    exit 1
fi

# --- Check dirty state ---
if git diff-index --quiet HEAD -- 2>/dev/null; then
    DIRTY=0
else
    DIRTY=1
fi

echo "Git revision: $REV, dirty: $DIRTY"
echo "Processing file: $FILE"

# --- Output file (remove .xtrsvn extension) ---
OUT="${FILE%.xtrsvn}"

# --- Process file line by line, replacing placeholders ---
while IFS= read -r line; do
    line="${line//\$WCREV\$/$REV}"
    line="${line//\$WCMODS?1:0\$/$DIRTY}"
    echo "$line"
done < "$FILE" > "$OUT"

echo "Wrote $OUT with WCREV=$REV and WCMODS=$DIRTY"
