import os, time
import subprocess

URL_TGZ="http://192.168.1.41:8081/artifactory/list/conan-local/diego/image-filter/0.0.1/testing/package/283703a4caf5b5bbec000125a4ca8b0dd5394d36/conan_package.tgz"

def run_check(): 
    os.system("rm -r check")
    os.system("mkdir check")
    os.chdir("check")
    os.system("wget %s" % URL_TGZ)
    os.system("tar -xzf conan_package.tgz")
    print "Unzipped, removing .tgz"
    os.remove("conan_package.tgz")
    with open("bin/app") as f:
        content = f.read()
    os.chdir("..")
    return content

previous = None
proc = None
while True:
    content = run_check()
    if content != previous:
        print "CHANGES! Launching app"
        previous = content
        if proc:
            proc.terminate()
            os.remove("bin/app")
        os.system("mkdir bin")
        os.system("cp check/bin/app bin/app")
        proc = subprocess.Popen(["bin/app"]) 
    time.sleep(5)
