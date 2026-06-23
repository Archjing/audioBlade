#!/usr/bin/env bash
set -euo pipefail

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

load_env_file() {
  local env_file="$1"
  if [[ -f "$env_file" ]]; then
    set -a
    # shellcheck disable=SC1090
    source "$env_file"
    set +a
  fi
}

if [[ -n "${AUDIOBLADE_HARNESS_ENV:-}" ]]; then
  load_env_file "$AUDIOBLADE_HARNESS_ENV"
else
  load_env_file "$repo_root/.harness.env"
fi

runner_path="${AUDIOBLADE_HARNESS_RUNNER:-../codex-harness-runner}"
profile="${AUDIOBLADE_HARNESS_PROFILE:-audioBlade}"
python_path="${AUDIOBLADE_HARNESS_PYTHON:-.venv/bin/python3}"

if [[ "$runner_path" != /* ]]; then
  runner_path="$(cd "$repo_root/$runner_path" 2>/dev/null && pwd)"
fi

if [[ -z "$runner_path" || ! -d "$runner_path" ]]; then
  cat >&2 <<EOF
Harness runner not found.

Set AUDIOBLADE_HARNESS_RUNNER in .harness.env or the shell environment.
Current value: ${AUDIOBLADE_HARNESS_RUNNER:-../codex-harness-runner}
EOF
  exit 1
fi

if [[ "$python_path" != /* ]]; then
  python_path="$runner_path/$python_path"
fi

if [[ ! -x "$python_path" ]]; then
  cat >&2 <<EOF
Harness Python is not executable.

Set AUDIOBLADE_HARNESS_PYTHON in .harness.env or the shell environment.
Current resolved path: $python_path
EOF
  exit 1
fi

main_py="$runner_path/main.py"
if [[ ! -f "$main_py" ]]; then
  cat >&2 <<EOF
Harness main.py not found.

Expected: $main_py
EOF
  exit 1
fi

exec "$python_path" "$main_py" --profile "$profile" "$@"
