set -e

rm -rf build
CHESSAI_VERSION="bare" python -m pip install . --force-reinstall
python chesspy/test/extract_benchmark_data.py bare true

rm -rf build
CHESSAI_VERSION="pvs" python -m pip install . --force-reinstall
python chesspy/test/extract_benchmark_data.py pvs false

rm -rf build
CHESSAI_VERSION="pvs+qs" python -m pip install . --force-reinstall
python chesspy/test/extract_benchmark_data.py pvs+qs false

rm -rf build
CHESSAI_VERSION="aspiration" python -m pip install . --force-reinstall
python chesspy/test/extract_benchmark_data.py aspiration false

rm -rf build
CHESSAI_VERSION="hash" python -m pip install . --force-reinstall
python chesspy/test/extract_benchmark_data.py hash false

rm -rf build
CHESSAI_VERSION="hash+aspiration" python -m pip install . --force-reinstall
python chesspy/test/extract_benchmark_data.py hash+aspiration false