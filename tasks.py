from invoke import task

@task 
def build(c):

    print("\033[0;92m[PRE-BUILD]: test chesscpp")
    c.run("cd chesscpp && invoke test", pty=True)

    print("\033[0;92m[BUILD]: Chess-AI module\033[0;37m")
    c.run("python -m pip install .", pty=True)
    
    print("\033[0;92m[POST-BUILD]: discover python tests\033[0;37m")
    c.run("python -m unittest discover .", pty=True)