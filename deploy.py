import os, time

def run(cmd):
    ret = os.system(cmd)
    if ret != 0:
        raise Exception("ERROR %s" % cmd)

URL_TGZ="http://192.168.1.41:8081/artifactory/list/conan-local/diego/image-filter/0.0.1/testing/package/283703a4caf5b5bbec000125a4ca8b0dd5394d36/conan_package.tgz"

def run_check(): 
    os.system("rm -r check")
    os.system("mkdir check")
    os.chdir("check")
    run("wget %s" % URL_TGZ)
    run("tar -xzf conan_package.tgz")
    print "Unzipped, removing .tgz"
    run("rm conan_package.tgz")
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
            os.system("killall -9 bin/app")
            os.remove("bin/app")
            proc = True
        os.system("mkdir bin")
        os.system("cp check/bin/app bin/app")
        os.system("bin/app &") 
    time.sleep(5)
