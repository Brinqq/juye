#!/usr/bin/env bash
# Compiles MSL sources to .metallib in the runtime data dir.
# Runnable from any cwd. Set DEBUG=1 for shader debug info.
set -uo pipefail

root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
source_dir="$root/src/shaders/msl"
output_dir="$root/build/bin/data"

cmd=(xcrun -sdk macosx metal)

# -frecord-sources embeds source so the Xcode Metal debugger can show it.
if [[ "${DEBUG:-0}" != "0" ]]; then
  cmd+=(-gline-tables-only -frecord-sources)
fi

files=(
  triangle.metal
)

# foo.metal -> $output_dir/foo.metallib
set_out_name() {
  local base="${1##*/}"
  printf '%s/%s.metallib' "$output_dir" "${base%.*}"
}

mkdir -p "$output_dir"

failed=0
for file in "${files[@]}"; do
  src="$source_dir/$file"
  out="$(set_out_name "$file")"

  if [[ ! -f "$src" ]]; then
    echo "missing  $src"
    failed=1
    continue
  fi

  # Single step, no -c. `metal -c` emits AIR bitcode, which the runtime
  # rejects as an invalid library file. Without it the driver compiles and
  # links a real .metallib (magic 'MTLB').
  if "${cmd[@]}" "$src" -o "$out"; then
    echo "ok       $file -> ${out#"$root"/}"
  else
    echo "FAIL     $file"
    failed=1
  fi
done

exit $failed
