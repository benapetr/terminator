Terminator daemon
==================

This is a simple c++ service which can help you keep your system clean. It serves similar purposes as
OOM killer, but is easier to configure and not so fatal.

Basically this daemon watches your system and if any process exceeds the limits, it can be
killed based on preferences in order to prevent your system from crashing




Where it can be used
=====================

On every server which needs to carefuly handle OOM without risk that critical service would get killed.
You can specify very precise conditions of which processes may be killed and which must not be,
so that system will never get to point when kernel OOM killer start killing random processes.

Terminator has 2 uses: it can watch the processes for exceeding process limits (for example 1gb of ram
per process) and kill them if they exceed it, and it can watch the system resources and randomly
start killing processes when system is reaching OOM.




Build
======
You can compile this application just by running
    make

If you are using ubuntu or debian you can execute script ./debian to make .deb package

NOTE: You may need to switch to `Makefile_ubuntu` on older system, because default is expecting
libprocps to exist

You need to have either libprocps-dev or libproc-dev installed for it to build

Example installation on ubuntu:

    sudo apt-get install make g++ fakeroot libproc-dev
    ./build-package
    sudo dpkg -i ../terminatord*.deb


Configuration
==============
Program is configured using parameters, there is no configuration file, but you can create a wrapper for it

 -h [--help]: Display help

 -k [--kill]: Will kill processes if they exceed the limits, if this is not specified, the terminatord will only
              report the processes that exceeds the limit.

 --quiet:     Will completely ignore all limits for processes, terminator will only watch system memory, this is
              useful when you need to watch system resources only.

 --soft limit in MB: Set a soft memory limit, any process that exceed this will get killed if --kill option
                     is provided, if not it will get reported

 --hard limit in MB: Set a hard memory limit, any process that exceed this will get killed with -9 option

 --ssoft limit in MB: Set a soft system memory limit which is a lowest possible free memory, if free memory is lower
                      system will randomly kill processes that are causing most troubles

 --shard limit in MB: Set a hard system memory limit which is a lowest possible free memory, if free memory is lower
                      system will randomly kill processes with SIGKILL that are causing most troubles

 --root: Kill even system processes if they exceed the limit

 --ignore 1,2...: Set a list of uid to ignore, separated by comma (with no spaces), processes running from any of
                  these users will not be killed

 --dry: Never kill any process this is useful when you need to test your configuration

 -d: Run in a daemon mode, this is currently only supported mode and you always need to pass it in order to start

 -v [--verbose]: Increase verbosity





Examples
=========

Kill all processes that use more than 400mb of ram, except for user apache and root


    # get uid of apache
    grep apache /etc/passwd
    # let's say apache is user 20, now we test it
    terminatord -dvvv --soft 400 --hard 420 --ignore 20 --dry
    # if everything is ok
    terminatord -d --soft 400 --hard 420 --ignore 20 --kill


Kill random processes in case that system has less than 100mb of free ram, except for root

    # test it
    terminatord -dvvv --ssoft 100 --shard 60 --quiet --dry


Combine both examples, in this example unlike the previous one, apache processes will not be killed,
when system go OOM

    # let's say apache is user 20, now we test it
    terminatord -dvvv --soft 400 --hard 420 --ignore 20 --shard 60 --ssoft 100 --dry
    # if everything is ok
    terminatord -d --soft 400 --hard 420 --ignore 20 --kill --shard 60 --ssoft 100
