set -e

# MOVEGEN
rm -rf build
CHESSAI_VERSION="all" python -m pip install . --force-reinstall
python chesspy/test/benchmarks/movegen_benchmark.py bitboards 1

# EVAL
rm -rf build
CHESSAI_VERSION="all" python -m pip install . --force-reinstall
python chesspy/test/benchmarks/eval_benchmark.py all_enabled 1

rm -rf build
CHESSAI_VERSION="eval_mat" python -m pip install . --force-reinstall
python chesspy/test/benchmarks/eval_benchmark.py mat_only 

rm -rf build
CHESSAI_VERSION="eval_mob" python -m pip install . --force-reinstall
python chesspy/test/benchmarks/eval_benchmark.py mob_only 

rm -rf build
CHESSAI_VERSION="eval_ps" python -m pip install . --force-reinstall
python chesspy/test/benchmarks/eval_benchmark.py ps_only 

rm -rf build
CHESSAI_VERSION="eval_ks" python -m pip install . --force-reinstall
python chesspy/test/benchmarks/eval_benchmark.py ks_only 

rm -rf build
CHESSAI_VERSION="eval_other" python -m pip install . --force-reinstall
python chesspy/test/benchmarks/eval_benchmark.py other_only 

# SEARCH
rm -rf build
CHESSAI_VERSION="bare" python -m pip install . --force-reinstall
python chesspy/test/benchmarks/search_benchmark.py bare 1

rm -rf build
CHESSAI_VERSION="pvs" python -m pip install . --force-reinstall
python chesspy/test/benchmarks/search_benchmark.py pvs 

rm -rf build
CHESSAI_VERSION="pvs+qs" python -m pip install . --force-reinstall
python chesspy/test/benchmarks/search_benchmark.py pvs+qs 

rm -rf build
CHESSAI_VERSION="aspiration" python -m pip install . --force-reinstall
python chesspy/test/benchmarks/search_benchmark.py aspiration 

rm -rf build
CHESSAI_VERSION="hash" python -m pip install . --force-reinstall
python chesspy/test/benchmarks/search_benchmark.py hash 

rm -rf build
CHESSAI_VERSION="hash+aspiration" python -m pip install . --force-reinstall
python chesspy/test/benchmarks/search_benchmark.py hash+aspiration 

