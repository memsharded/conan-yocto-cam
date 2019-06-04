#!/bin/bash

URL_TGZ="http://192.168.1.41:8081/artifactory/list/conan-local/diego/image-filter/0.0.1/testing/package/8df15f70e52241368355aaa0bed65e4b0bd9a084/conan_package.tgz"
APP_PATH="bin/app"


run_check_directory()
{
    rm -r check
    mkdir check
    cd check
    sleep 1
    wget "$URL_TGZ"
    tar -xzf conan_package.tgz
    rm conan_package.tgz
}

read_binary_content ()
{
    folder=$1
    binary_content=$(<$folder/$APP_PATH)
    return binary_content
}


run_check_directory
while [ 1 ]
do
    rm -r execute
    cp -R ./check ./execute
    cd execute
    $APP_PATH &
    app_pid=$!
    sleep 5
    deploy_content="$(read_binary_content execute)"
    check=1

    while [ $check -gt 0 ]
    do
        run_check_directory
        new_deploy_content="$(read_binary_content check)"

        if [ $deploy_content -ne $new_deploy_content ]; then
            kill -9 $app_pid
            check=0
        fi
    done
done