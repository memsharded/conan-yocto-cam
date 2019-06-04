import os, time, shutil
import subprocess

URL_TGZ="http://192.168.1.41:8081/artifactory/list/conan-local/diego/image-filter/0.0.1/testing/package/8df15f70e52241368355aaa0bed65e4b0bd9a084/conan_package.tgz"

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
        shutil.copy2("check/bin/app", "bin/app")
        proc = subprocess.Popen(["bin/app"]) 
    time.sleep(5)
