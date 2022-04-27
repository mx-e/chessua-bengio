# chess-ai
A symbolic chess AI algorithm playing the king of the hill chess variant.

## Build
You can build and test the entire package with one tiny command, namely:
```bash
invoke build
```

Pre-requisites are that you have installed `invoke`, `python-dotenv` and eventual dependcies of `chesspy`.
Additionally, the `build` task is configured to run python commands via

```bash
python -m ...
```

If your python is not available via `python` you can pass a link to your python to the tool via, e.g.,

```bash 
invoke build python=/../../python3
```

This task executes all unittests of the `chesscpp` package, builds the final python binding and then runs the post-build unittests of the `chesspy` module.

GLHF!
