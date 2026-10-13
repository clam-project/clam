#!/bin/bash
# Runs the built applications on the bundled examples.
# Usage: .github/scripts/smoke-tests.sh <build-dir>
# The GUI checks need xvfb-run.
set -euo pipefail

build=$(realpath "$1")
source=$(realpath "$(dirname "$0")/../..")
work=$(mktemp -d)
trap 'rm -rf "$work"' EXIT

echo "== SMSConsole: analysis, transformation and synthesis"
cp -r "$source/SMSTools/example-data" "$work/"
(
	cd "$work"
	# Menu choices: load configuration, analyze, store the analysis (SDIF),
	# load the transformation score, transform, synthesize, store, finish.
	printf '%s\n' 1 example-data/Elvis-config.xml 3 4 analysis.sdif \
		7 example-data/tranformation-femaleAndTimeStretch.xml 8 9 10 0 \
		| "$build/SMSTools/SMSConsole" > console.log 2>&1 \
		|| { cat console.log; exit 1; }
	if grep -q 'ASSERTION FAILED\|Abnormal Program Termination' console.log; then
		cat console.log
		exit 1
	fi
	python3 - example-data/Elvis-outputSound*.wav analysis.sdif <<'PYTHON'
import math, struct, sys
for name in sys.argv[1:]:
	data = open(name, 'rb').read()
	if not name.endswith('.wav'):
		assert len(data) > 0, name + ' is empty'
		continue
	# Find the 32 bit float data chunk the SMS tools write
	pos, samples = 12, None
	while pos < len(data):
		chunk, size = data[pos:pos+4], struct.unpack('<I', data[pos+4:pos+8])[0]
		if chunk == b'data':
			samples = struct.unpack('<%df' % (size // 4), data[pos+8:pos+8+size])
		pos += 8 + size + (size & 1)
	assert samples, name + ' has no samples'
	bad = sum(1 for x in samples if not math.isfinite(x))
	assert not bad, '%s: %d non-finite samples' % (name, bad)
	print('%s: %d samples, ok' % (name, len(samples)))
PYTHON
)

echo "== SMSConsole and SMSBatch exit cleanly"
printf '0\n' | "$build/SMSTools/SMSConsole" > /dev/null
printf '0\n' | "$build/SMSTools/SMSBatch" > /dev/null
"$build/SMSTools/SMSConsole" < /dev/null > /dev/null

echo "== CheckClamNetwork on the example networks"
(
	cd "$source/NetworkEditor"
	for network in example-data/*.clamnetwork; do
		# lpc uses processings from a CLAM plugin that CMake does not build
		[ "$network" = example-data/lpc.clamnetwork ] && continue
		"$build/NetworkEditor/CheckClamNetwork" "$network" > "$work/check.log" 2>&1 \
			|| { echo "$network:"; cat "$work/check.log"; exit 1; }
	done
)

# Starts a GUI application under a virtual X server for a few seconds.
# It must still be running then (timeout's 124) and Qt must not have
# reported connections to missing signals or slots.
gui_smoke()
{
	local log="$work/gui.log" status=0
	xvfb-run -a -s '-screen 0 1280x1024x24' timeout 15 "$@" > "$log" 2>&1 || status=$?
	if [ "$status" -ne 124 ] || grep -q 'No such signal\|No such slot' "$log"; then
		echo "$* (exit status $status):"
		cat "$log"
		exit 1
	fi
}

echo "== NetworkEditor"
(cd "$source/NetworkEditor" && gui_smoke "$build/NetworkEditor/NetworkEditor")
for network in tonalAnalysis-file spectralNetwork monitors-file; do
	(cd "$source/NetworkEditor" && gui_smoke "$build/NetworkEditor/NetworkEditor" "example-data/$network.clamnetwork")
done

echo "== SMSTools"
(cd "$source/SMSTools" && gui_smoke "$build/SMSTools/SMSTools")

echo "All smoke tests passed"
