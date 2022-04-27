# Chess-CPP Backend (C++ Package)
The Chess-CPP package is a backend for Chess-AI that can be used as a C++-extension package for python.

To ensure that the package can be developed and maintained on its own, this folder is equipped with 
`invoke` tasks that enable developers of this package with a small cli tool to build and run tests
on C++ level, using the testing library `google-tests`. 

The `.test.env` file defines source and target of the build process and can be used to configure additional build params in `cmake`. Using this file in conjunction with the `invoke` tasks, developers 
do not need to work with cmake directly and all necessary dependencies are loaded and built automatically.

To work with the `invoke` tool, you need to install dependencies via 

```bash
python -m pip install invoke python-dotenv
```

Tasks can be run with 
```bash 
invoke [task]
```
The `invoke` task list currently contains two tasks, `clean` and `test`. 

The `clean` task clears up the build folder within the build-target `/test`. This can be useful to ensure a working build process if running other commands/tasks fails for unkown reasons.

The `test` task builds the test suite and runs the tests specified in `test/main.cpp`. You can pass the arguments `--output-on-failure` and `--rerun-failed` to the invoke call. 
Additionally, you can attach you favorite debugger to the output file of the test build by running e.g.,

```bash
invoke test --debugger=lldb
```

GLHF!